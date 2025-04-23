#include "main_window.h"
#include "utils.h"
#include "api.h"
#include "result.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <future>
#include <curl/curl.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <giomm/resource.h>

MainWindow::MainWindow()
:m_mainContainer(Gtk::Orientation::ORIENTATION_VERTICAL, 5),
 m_addForm(Gtk::Orientation::ORIENTATION_HORIZONTAL, 5),
 m_browseBtn("Browse"),
 m_checkBtn("Check"),
 m_loginBtn("Login"),
 m_resultImage()
{

  set_title("File Verifier");
  set_default_size(650,600);
  m_correct = Gdk::Pixbuf::create_from_resource("/images/correct.svg");
  m_wrong = Gdk::Pixbuf::create_from_resource("/images/wrong.svg");
  m_warning = Gdk::Pixbuf::create_from_resource("/images/warning.svg");



  m_mainContainer.set_homogeneous(false);
  m_fileNameText.set_width_chars(40);
  m_addForm.pack_start(m_fileNameText, false, false, 5);
  //m_browseBtn.set_sensitive(false);
  //m_checkBtn.set_sensitive(false);
  m_addForm.add(m_browseBtn);
  m_addForm.add(m_checkBtn);
  // Create a spinner to indicate waiting
  m_spinner.set_halign(Gtk::ALIGN_CENTER);
  m_spinner.set_valign(Gtk::ALIGN_CENTER);
  m_addForm.add(m_spinner);
  m_addForm.add(m_resultImage);
  m_addForm.add(m_loginBtn);
  m_mainContainer.pack_start(m_addForm, false, false, 5);
  m_addForm.set_valign(Gtk::ALIGN_BASELINE);
  m_mainContainer.set_valign(Gtk::ALIGN_FILL);
  m_mainContainer.pack_end(m_resultText, true, true, 5);
  add(m_mainContainer);

  m_browseBtn.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::selectFile));
  m_checkBtn.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::startVerifying));
  m_loginBtn.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::handleLoginAndLogout));
  m_loginWindow = new TokenWindow();
  m_loginWindow->setParentWindow(this);
  m_loginWindow->set_default_size(100,200);
  m_loginWindow->set_modal(true);
  set_position(Gtk::WIN_POS_CENTER);
  m_loginWindow->set_transient_for(*this);
  m_loginWindow->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);

  m_apiToken = Utils::getAccessToken();
  if (!m_apiToken.empty()){
    m_loginBtn.set_label("Logout");
  }

  m_resultText.set_left_margin(15);
  show_all_children();
  m_Dispatcher.connect(sigc::mem_fun(*this, &MainWindow::onResultReceived));
}

void MainWindow::selectFile(){
  //std::cout << "Opening File Chooser\n";
  Gtk::FileChooserDialog file_dialog("Select File", Gtk::FILE_CHOOSER_ACTION_OPEN);
  file_dialog.set_transient_for(*this);
  file_dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
  file_dialog.add_button("Select", Gtk::RESPONSE_OK);

  int result = file_dialog.run();

  switch(result){
    case(Gtk::RESPONSE_OK):{
      m_file_path = file_dialog.get_filename();
      //std::cout << "File Selected " << m_file_path << "\n";
      m_fileNameText.set_text(m_file_path);
      displayResult("",Result::RESULT_TYPE::EMPTY);
      break;
    }
    case(Gtk::RESPONSE_CANCEL):{
      break;
    }
    default:{
      break;
    }
  }

}

std::unique_ptr<Result> verifyFile(Glib::Dispatcher* p_dispatcher, std::string file_path, std::string apiToken){
  std::unique_ptr<Result> result = std::make_unique<Result>();
  std::string local_sha256 = Utils::calculateSha256Sum(file_path);
  std::transform(local_sha256.begin(), local_sha256.end(), local_sha256.begin(), static_cast<int(*)(int)>(std::tolower));
  std::string file_name = std::filesystem::path(file_path).filename();
  std::pair<short, std::string> response = Api::findBySha256(local_sha256, apiToken);
  result->m_httpStatus = response.first;
  result->m_message = response.second;
  
  if (result->m_httpStatus == 401){
    result->m_resultType = Result::RESULT_TYPE::WRONG;
    p_dispatcher->emit();
    return result;
  }

  if (result->m_httpStatus != 200 || result->m_message.empty()){
    result->m_message = "Network or system error!";
    result->m_resultType = Result::RESULT_TYPE::WRONG;
    p_dispatcher->emit();
    return result;
  }

  JsonParser json_parser;
  std::unique_ptr<JsonObject> json_obj = json_parser.parseJson(result->m_message);

  if (json_obj->type == JsonType::OBJECT && json_obj->arrayItems.size() > 1){
    result->m_message = Api::getResultToDisplay(json_obj.get(), local_sha256);
    result->m_resultType = Result::RESULT_TYPE::CORRECT;
    p_dispatcher->emit();
    return result;
  }

  response = Api::findByFileName(file_name, apiToken);
  result->m_httpStatus = response.first;
  result->m_message = response.second;

  if (result->m_httpStatus == 401){
    result->m_resultType = Result::RESULT_TYPE::WRONG;
    p_dispatcher->emit();
    return result;
  }

  if (result->m_httpStatus != 200 || result->m_message.empty()){
    result->m_message = "Network or system error!";
    result->m_resultType = Result::RESULT_TYPE::WRONG;
    p_dispatcher->emit();
    return result;
  }

  json_obj = json_parser.parseJson(result->m_message);

  std::string result_message;
  if (json_obj->arrayItems.size() == 0){
    result_message.append("Our database does not have any files with same sha256 checksum.");
    result_message.append("\n\nSha 256 of Selected file:\n");
    result_message.append(local_sha256);
    result->m_resultType = Result::RESULT_TYPE::WARNING;
    result->m_message = result_message;
    p_dispatcher->emit();
    return result;
  }
  else if (json_obj->arrayItems.size() > 1){
    result_message.append("Our database has multiple files with same name.");
    result_message.append("\nBut none of them have same sha 256!");
    result_message.append("\n\nSha 256 of Selected file:\n");
    result_message.append(local_sha256);
    result->m_resultType = Result::RESULT_TYPE::WARNING;
    result->m_message = result_message;
    p_dispatcher->emit();
    return result;
  }
  else if (json_obj->arrayItems.size() == 1){
    JsonObject* file_json = json_obj->arrayItems.at(0).get();
    result->m_message = Api::getResultToDisplay(file_json, local_sha256);
    result->m_resultType = Result::RESULT_TYPE::WRONG;
    p_dispatcher->emit();
    return result;
  }

  result->m_message = "Unknown Error!";
  result->m_resultType = Result::RESULT_TYPE::WRONG;
  p_dispatcher->emit();
  return result;
}

void MainWindow::onResultReceived()
{
  std::unique_ptr<Result> result = m_futureResult.get();
  m_spinner.stop();
  enableButtons(true);
  if (result->m_httpStatus == 401){
    Utils::showError("Your access token is invalid or expired.\n"
                     "Please click Check button again to verify a public file.\n"
                     "Please request and enter a new token to verify a personal file.\n"
                     );
    handleLoginAndLogout();
    return;
  }

  displayResult(result->m_message, result->m_resultType);
}

void MainWindow::startVerifying(){
  enableButtons(false);
  m_spinner.start();
  m_futureResult = std::async(std::launch::async,verifyFile, &m_Dispatcher, m_file_path, m_apiToken);
}

void MainWindow::handleLoginAndLogout()
{
  if (m_apiToken.empty()) {
    m_loginWindow->set_visible(true);
  }else {
    bool status = Utils::deleteAccessToken();
    if (!status){
      Utils::showError("Could not delete access token.");
    }else{
      m_loginBtn.set_label("Login");
      m_apiToken.clear();
    }   
  }
}


void MainWindow::displayResult(std::string message, Result::RESULT_TYPE result_type)
{
  m_resultText.get_buffer().get()->set_text(message);
  if (result_type == Result::RESULT_TYPE::CORRECT) {
    m_resultImage.set(m_correct);
  }
  else if (result_type == Result::RESULT_TYPE::WRONG) {
    m_resultImage.set(m_wrong);
  }
  else if (result_type == Result::RESULT_TYPE::WARNING) {
    m_resultImage.set(m_warning);
  }
  else{
    m_resultImage.clear();
  }
}

void MainWindow::enableButtons(bool enable)
{
  m_checkBtn.set_sensitive(enable);
  m_browseBtn.set_sensitive(enable);
  m_loginBtn.set_sensitive(enable);
}

std::string MainWindow::jsonFileToString(JsonObject *file_json, std::string local_file_sha256)
{
  bool matched = false;
  std::string result;
  JsonObject* remote_shasum_json = JsonParser::findByPropertyName(file_json, "sha256sum");
  JsonObject* software_name_json = JsonParser::findByPropertyName(file_json, "software_name");
  JsonObject* file_name_json = JsonParser::findByPropertyName(file_json, "file_name");
  JsonObject* public_json = JsonParser::findByPropertyName(file_json, "public");
  if (!remote_shasum_json || !public_json || !file_name_json){
    return "";
  }

  if (public_json->integerValue == 1 && !software_name_json){
    return "";
  }

  std::string remote_sha256sum = remote_shasum_json->stringValue;
  //std::transform(local_file_sha256.begin(), local_file_sha256.end(), local_file_sha256.begin(), static_cast<int(*)(int)>(std::tolower));
  std::transform(remote_sha256sum.begin(), remote_sha256sum.end(), remote_sha256sum.begin(), static_cast<int(*)(int)>(std::tolower));
  if (local_file_sha256 == remote_sha256sum){
    result.append("Sha256Sums matched !\n\n");
    matched = true;
  }else {
    result.append("Sha256Sums did NOT match!\n\n");
    matched = false;
  }
  result.append("File name: ");
  result.append(file_name_json->stringValue);
  result.append("\n");
  if (matched){
    result.append("Sha 256: ");
    result.append(remote_sha256sum);
  }else {
    result.append("\nSha 256 of remote file:\n ");
    result.append(remote_sha256sum);
    result.append("\n\nSha 256 of local file:\n ");
    result.append(local_file_sha256);
  }
  result.append("\n");

  if (public_json->integerValue == 1) {
    result.append("Software: ");
    result.append(software_name_json->stringValue);
    result.append("\n");
  }
  return result;
}

MainWindow::~MainWindow(){
  delete m_loginWindow;
}

void MainWindow::setLoginStatus(int status, std::string apiToken)
{
  if (status == 1){
    m_apiToken = apiToken;
    /*m_browseBtn.set_sensitive(true);
    m_checkBtn.set_sensitive(true);*/
    m_loginBtn.set_label("Logout");
  }
}

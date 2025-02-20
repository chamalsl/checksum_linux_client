#include "main_window.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
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
  //set_border_width(10);
  m_correct = Gdk::Pixbuf::create_from_resource("/images/correct.png");
  m_wrong = Gdk::Pixbuf::create_from_resource("/images/wrong.png");
  m_warning = Gdk::Pixbuf::create_from_resource("/images/warning.png");
  
  m_mainContainer.set_homogeneous(false);
  //m_fileNameText.set_valign(Gtk::ALIGN_START);
  m_fileNameText.set_width_chars(40);
  m_addForm.pack_start(m_fileNameText, false, false, 5);
  //m_browseBtn.set_sensitive(false);
  //m_checkBtn.set_sensitive(false);
  m_addForm.add(m_browseBtn);
  m_addForm.add(m_checkBtn);
  m_addForm.add(m_resultImage);
  m_addForm.add(m_loginBtn);
  m_mainContainer.pack_start(m_addForm, false, false, 5);
  m_addForm.set_valign(Gtk::ALIGN_BASELINE);
  m_mainContainer.set_valign(Gtk::ALIGN_FILL);
  m_mainContainer.pack_end(m_resultText, true, true, 5);
  add(m_mainContainer);

  m_browseBtn.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::selectFile));
  m_checkBtn.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::checkSha256Sum));
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
  show_all_children();

  
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
      displayResult("",RESULT::EMPTY);
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

void MainWindow::checkSha256Sum(){

  std::string result = "";
  std::string local_sha256sum = calculateSha256Sum(m_file_path);
  //std::cout << "Local file sha256 sum " << local_sha256sum << "\n";
  std::string file_name = std::filesystem::path(m_file_path).filename();
  std::pair<short, std::string> response = findFileBySha256(local_sha256sum);
  short http_status_code = response.first;
  std::string response_data = response.second;


  if (http_status_code == 401){
      Utils::showError("Your access token is invalid or expired.\n"
                       "Please click Check button again to verify a public file.\n"
                       "Please request and enter a new token to verify a personal file.\n"
                       );
      handleLoginAndLogout();
      return;
  }

  if (http_status_code != 200 || response_data.empty()){
    result.append("Network or system error!");
    displayResult(result, RESULT::WRONG);
    return;
  }

  JsonParser json_parser;
  std::unique_ptr<JsonObject> json_obj = json_parser.parseJson(response_data);

  /*if (json_obj->propertyName == "message") {
    if (json_obj->stringValue == "Unauthenticated."){
      Utils::showError("Your access token is invalid or expired.\n"
                       "Please click Check button again to verify a public file.\n"
                       "Please request and enter a new token to verify a personal file.\n"
                       );
      handleLoginAndLogout();
      return;
    }
  }*/

  if (json_obj->type == JsonType::OBJECT && json_obj->arrayItems.size() > 1){
    result = jsonFileToString(json_obj.get(), local_sha256sum);
    displayResult(result, RESULT::CORRECT);
    return;
  }

  response = findFilesByFileName(file_name);
  http_status_code = response.first;
  response_data = response.second;

  if (http_status_code == 401){
      Utils::showError("Your access token is invalid or expired.\n"
                       "Please click Check button again to verify a public file.\n"
                       "Please request and enter a new token to verify a personal file.\n"
                       );
      handleLoginAndLogout();
      return;
  }

  if (http_status_code != 200 || response_data.empty()){
    result.append("Network or system error!");
    displayResult(result, RESULT::WRONG);
    return;
  }

  json_obj = json_parser.parseJson(response_data);

  /*if (json_obj->propertyName == "message") {
    if (json_obj->stringValue == "Unauthenticated."){
      Utils::showError("Your access token is invalid or expired.\n"
                       "Please click Check button again to verify a public file.\n"
                       "Please request and enter a new token to verify a personal file.\n"
                       );
      handleLoginAndLogout();
      return;
    }
  }*/

  if (json_obj->arrayItems.size() == 0){
    result.append("Our database does not have any files with same sha256 checksum.");
    displayResult(result, RESULT::WARNING);
  }
  else if (json_obj->arrayItems.size() > 1){
    result.append("Our database does not have any files with same sha256 checksum.");
    displayResult(result, RESULT::WARNING);
  }
  else if (json_obj->arrayItems.size() == 1){
    JsonObject* file_json = json_obj->arrayItems.at(0).get();
    result = jsonFileToString(file_json, local_sha256sum);
    displayResult(result, RESULT::WRONG);
  }

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

std::string MainWindow::calculateSha256Sum(std::string file_path_str){
  std::filesystem::path file_path(file_path_str);
  if (!std::filesystem::exists(file_path)){
    return "";
  }

  size_t file_size = std::filesystem::file_size(file_path);

  if (file_size == 0){
    return "";
  }

  unsigned char *sha_256_hash;
  size_t read_size = 1024;
  char data[read_size];
  std::ifstream file_stream(file_path, std::ios_base::binary);

  EVP_MD_CTX *evp_ctx = EVP_MD_CTX_new();
  if (evp_ctx == NULL){
    return "";
  }

  if (EVP_DigestInit_ex(evp_ctx, EVP_sha256(), NULL) != 1){
    return "";
  }

  while (!file_stream.eof()) {
    file_stream.read(data, read_size);
    if (file_stream.gcount()){
      if (EVP_DigestUpdate(evp_ctx, data, file_stream.gcount()) != 1){
        return "";
      }
    }
  }

  size_t digest_size = EVP_MD_size(EVP_sha256());
  sha_256_hash = (unsigned char*)OPENSSL_malloc(digest_size);
  if (!sha_256_hash){
    return "";
  }

  EVP_DigestFinal_ex(evp_ctx, sha_256_hash, NULL);
  
  return Utils::toHex(sha_256_hash, digest_size);
}

static size_t getResponseFromCurl(void* contents, size_t size, size_t nmemb, void* user_data){
  //Do nothing, because response is not necessary.
  //std::cout << "Curl write data " << size << " " << nmemb << "\n";
  std::string* data = (std::string*)user_data;
  data->append((char*)contents, size * nmemb);
  return size*nmemb;
}

/*std::unique_ptr<JsonObject> MainWindow::fetchSha256Sum(std::string post_data, std::string url){
  std::string url_str = "";

  if (m_apiToken.empty()){
    url_str = "http://127.0.0.1:8000/api/shasum/";
  }else {
    url_str = "http://127.0.0.1:8000/api/shasum_personal/";
  }
  const char* url = url_str.c_str();
  CURL* curl_conn = curl_easy_init();
  curl_easy_setopt(curl_conn, CURLOPT_URL, url);
  curl_easy_setopt(curl_conn, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl_conn, CURLOPT_WRITEFUNCTION, getResponseFromCurl);
  std::string data;
  curl_easy_setopt(curl_conn, CURLOPT_WRITEDATA, (void *)&data);
  struct curl_slist *list = NULL;
  if (!m_apiToken.empty()){
    std::string auth_header = "Authorization: Bearer ";
    auth_header.append(m_apiToken);
    list = curl_slist_append(list, auth_header.c_str());
  }

  list = curl_slist_append(list, "Accept:application/json");
  curl_easy_setopt(curl_conn, CURLOPT_HTTPHEADER, list);
  
  int m_status = 0;
  CURLcode code = curl_easy_perform(curl_conn);
  curl_easy_getinfo(curl_conn, CURLINFO_RESPONSE_CODE ,&m_status);

  JsonParser json_parser;
  std::unique_ptr<JsonObject> json = json_parser.parseJson(data);
  //std::cout << "Response json " << json_parser.toString(json.get()) << "\n";
  curl_easy_cleanup(curl_conn);
  return json;
}*/

std::pair<short, std::string> MainWindow::findFileBySha256(std::string sha256)
{
    std::string url = URL_API_FIND_BY_SHA;
    if (!m_apiToken.empty()){
      url = URL_API_FIND_BY_SHA_AUTH;
    }

    std::map<std::string, std::string> post_data;
    post_data["sha256"] = sha256;
    std::map<std::string, std::string> headers;
    headers["Accept"] = "application/json";
    return Utils::requestURLWithPost(url, post_data, m_apiToken, headers);
}

std::pair<short, std::string> MainWindow::findFilesByFileName(std::string file_name)
{
    std::string url = URL_API_FIND_BY_NAME;
    if (!m_apiToken.empty()){
      url = URL_API_FIND_BY_NAME_AUTH;
    }

    std::map<std::string, std::string> post_data;
    post_data["filename"] = file_name;
    std::map<std::string, std::string> headers;
    headers["Accept"] = "application/json";
    return Utils::requestURLWithPost(url, post_data, m_apiToken, headers);
}

void MainWindow::displayResult(std::string message, RESULT result)
{
  m_resultText.get_buffer().get()->set_text(message);
  if (result == RESULT::CORRECT) {
    m_resultImage.set(m_correct);
  }
  else if (result == RESULT::WRONG) {
    m_resultImage.set(m_wrong);
  }
  else if (result == RESULT::WARNING) {
    m_resultImage.set(m_warning);
  }
  else{
    m_resultImage.clear();
  }
}

std::string MainWindow::jsonFileToString(JsonObject *file_json, std::string local_file_sha256)
{
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
  std::transform(local_file_sha256.begin(), local_file_sha256.end(), local_file_sha256.begin(), static_cast<int(*)(int)>(std::tolower));
  std::transform(remote_sha256sum.begin(), remote_sha256sum.end(), remote_sha256sum.begin(), static_cast<int(*)(int)>(std::tolower));
  if (local_file_sha256 == remote_sha256sum){
    result.append("Sha256Sums matched !\n\n");
  }else {
    result.append("Sha256Sums did NOT match!\n\n");
  }
  result.append("File name: ");
  result.append(file_name_json->stringValue);
  result.append("\n");
  result.append("Sha256Sum: ");
  result.append(remote_sha256sum);
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

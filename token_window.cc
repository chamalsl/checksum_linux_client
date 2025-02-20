#include "token_window.h"
#include "utils.h"
#include "third_party/json_parser/json_parser.h"
#include <iostream>


TokenWindow::TokenWindow() : m_tokenLabel("Token"),
                             m_saveTokenBtn("Save"),
                             m_cancelBtn("Cancel"),
                             m_getTokenBtn(),
                             m_buttonBox(Gtk::Orientation::ORIENTATION_HORIZONTAL, 5)
{
  set_title("Login");
  

  m_getTokenBtn.set_uri("http://localhost:8000/tokens/gettoken");
  m_getTokenBtn.set_label("Get Token");
  m_formGrid.set_row_spacing(3);
  m_formGrid.set_column_spacing(3);
  m_formGrid.attach(m_tokenLabel,0,0);
  m_formGrid.attach(m_tokenText,1,0);
  m_buttonBox.add(m_saveTokenBtn);
  m_buttonBox.add(m_getTokenBtn);
  m_buttonBox.add(m_cancelBtn);
  m_formGrid.attach(m_buttonBox, 0,2,3,1);
  add(m_formGrid);

  m_saveTokenBtn.signal_clicked().connect(sigc::mem_fun(*this, &TokenWindow::saveToken));
  m_cancelBtn.signal_clicked().connect(sigc::mem_fun(*this, &TokenWindow::cancel));
  show_all_children();

}

TokenWindow::~TokenWindow()
{
}

void TokenWindow::setParentWindow(MainWindow *parent)
{
  m_parent = parent;
}

void TokenWindow::saveToken()
{
  /*std::string login_url = "http://127.0.0.1:8000/api/authenticate";
  std::map<std::string, std::string> post_data;
  post_data.insert(std::pair<std::string, std::string>("email",m_tokenText.get_text()));
  //TODO: device name use random
  post_data.insert(std::pair<std::string, std::string>("device_name","desktop"));
  std::string json_str = Utils::requestURLWithPost(login_url, post_data);
  JsonParser json_parser;
  std::unique_ptr<JsonObject> json = json_parser.parseJson(json_str);
  //std::cout << json_str << "\n";
  
  if (!json.get()) {
    Utils::showError("Login Failed. Invalid Reponse!");
    m_parent->setLoginStatus(0, "");
    return;
  }

  JsonObject* statusObj = JsonParser::findByPropertyName(json.get(),"status");

  if (!statusObj || statusObj->integerValue != 1){
    Utils::showError("Login Failed!");
    m_parent->setLoginStatus(0, "");
    return;
  }

  JsonObject* tokenObj = JsonParser::findByPropertyName(json.get(),"token");

  if (!tokenObj) {
    Utils::showError("Login Failed. Empty Token!");
    m_parent->setLoginStatus(0, "");
    return;
  }

  std::string token = tokenObj->stringValue;*/
  std::string token = m_tokenText.get_text();
  if (token.empty()){
    Utils::showError("Token is empty!");
    return;
  }

  if (token.length() > 500) {
    Utils::showError("Token has more than 500 characters!");
    return;
  }
  m_parent->setLoginStatus(1, token);
  Utils::storeAccessToken(token.c_str());
  set_visible(false);
  //std::cout << token << "\n";
}

void TokenWindow::cancel(){
  set_visible(false);
}

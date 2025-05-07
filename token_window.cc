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
  m_formGrid.set_margin_top(4);
  m_formGrid.set_margin_left(3);
  m_formGrid.set_margin_right(3);
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
}

void TokenWindow::cancel(){
  set_visible(false);
}

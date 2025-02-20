#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "token_window.h"
#include "third_party/json_parser/json_parser.h"
#include <gtkmm.h>
#include <thread>
#include <map>
#include <cstdint>
#include <string>
#include <memory>

static const std::string URL_API_FIND_BY_NAME = "http://127.0.0.1:8000/api/findByFileName";
static const std::string URL_API_FIND_BY_NAME_AUTH = "http://127.0.0.1:8000/api/findByFileNamePrivate";
static const std::string URL_API_FIND_BY_SHA = "http://127.0.0.1:8000/api/findBySha256";
static const std::string URL_API_FIND_BY_SHA_AUTH = "http://127.0.0.1:8000/api/findBySha256Private";

enum RESULT{
  CORRECT,
  WRONG,
  WARNING,
  EMPTY
};
class TokenWindow;
class MainWindow: public Gtk::Window {

public:
  MainWindow();
  virtual ~MainWindow();
  void setLoginStatus(int status, std::string apiToken);

protected:
  void selectFile();
  void checkSha256Sum();
  void handleLoginAndLogout();
  Gtk::Entry m_fileNameText;
  Gtk::Button m_browseBtn;
  Gtk::Button m_checkBtn;
  Gtk::Button m_loginBtn;
  Gtk::Image m_resultImage;
  Gtk::TextView m_resultText;
  Gtk::Box m_addForm;
  Gtk::VBox m_mainContainer;
  TokenWindow* m_loginWindow;
  
private:
  std::string calculateSha256Sum(std::string file_path);
  //std::unique_ptr<JsonObject> fetchSha256Sum(std::string post_data, std::string url);
  std::pair<short, std::string> findFileBySha256(std::string sha256);
  std::pair<short, std::string> findFilesByFileName(std::string file_name);
  void displayResult(std::string message, RESULT result);
  std::string jsonFileToString(JsonObject* file_json, std::string local_file_sha256);
  std::string m_file_path;
  std::string m_apiToken;

  Glib::RefPtr<Gdk::Pixbuf> m_correct;
  Glib::RefPtr<Gdk::Pixbuf> m_wrong;
  Glib::RefPtr<Gdk::Pixbuf> m_warning;
  
};
#endif //MAINWINDOW_H

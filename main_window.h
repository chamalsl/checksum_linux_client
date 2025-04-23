#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "token_window.h"
#include "result.h"
#include "third_party/json_parser/json_parser.h"
#include <gtkmm.h>
#include <thread>
#include <map>
#include <cstdint>
#include <string>
#include <future>
#include <memory>

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
  Gtk::Spinner m_spinner;
  Gtk::Image m_resultImage;
  Gtk::TextView m_resultText;
  Gtk::Box m_addForm;
  Gtk::VBox m_mainContainer;
  TokenWindow* m_loginWindow;
  
private:
  void startVerifying();
  void onResultReceived();
  void displayResult(std::string message, Result::RESULT_TYPE result);
  void enableButtons(bool enable);
  std::string jsonFileToString(JsonObject* file_json, std::string local_file_sha256);
  std::string m_file_path;
  std::string m_apiToken;

  Glib::RefPtr<Gdk::Pixbuf> m_correct;
  Glib::RefPtr<Gdk::Pixbuf> m_wrong;
  Glib::RefPtr<Gdk::Pixbuf> m_warning;
  Glib::Dispatcher m_Dispatcher;
  std::future<std::unique_ptr<Result>> m_futureResult;
  
};
#endif //MAINWINDOW_H

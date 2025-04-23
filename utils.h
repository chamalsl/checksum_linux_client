#include <string>
#include <map>
#include <libsecret/secret.h>

class Utils{

  public:
  static std::string calculateSha256Sum(std::string file_path_str);
  static char getHexChar(unsigned short number);
  static std::string toHex(unsigned char c);
  static std::string toHex(unsigned char* str, size_t length);
  static std::string requestURL(std::string url);
  static std::pair<short, std::string> requestURLWithPost(std::string p_url, std::map<std::string, std::string> p_post_data, 
      std::string token, std::map<std::string, std::string> headers);

  static std::string getHomeDirectory();
  static std::string getDataDirectory();
  static bool storeAccessToken(const char* access_token);
  static bool deleteAccessToken();
  static std::string getAccessToken();
  static void showError(std::string error_msg);

  private:
  static const SecretSchema* getSecretStoreSchema();

};
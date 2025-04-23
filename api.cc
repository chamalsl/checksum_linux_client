#include "api.h"
#include "utils.h"
#include <string>
#include <algorithm>
#include <map>

std::pair<short, std::string>  Api::findByFileName(std::string file_name, std::string apiToken)
{
  std::string url = URL_API_FIND_BY_NAME;
  if (!apiToken.empty()){
    url = URL_API_FIND_BY_NAME_AUTH;
  }

  std::map<std::string, std::string> post_data;
  post_data["filename"] = file_name;
  std::map<std::string, std::string> headers;
  headers["Accept"] = "application/json";
  return Utils::requestURLWithPost(url, post_data, apiToken, headers);
}

std::pair<short, std::string>  Api::findBySha256(std::string sha256, std::string apiToken)
{
    std::string url = URL_API_FIND_BY_SHA;
    if (!apiToken.empty()){
      url = URL_API_FIND_BY_SHA_AUTH;
    }

    std::map<std::string, std::string> post_data;
    post_data["sha256"] = sha256;
    std::map<std::string, std::string> headers;
    headers["Accept"] = "application/json";
    return Utils::requestURLWithPost(url, post_data, apiToken, headers);
}

std::string Api::getResultToDisplay(JsonObject *file_json, std::string local_file_sha256)
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
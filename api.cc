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

std::pair<short, std::string>  Api::findByChecksums(std::string sha256, std::string sha512, std::string apiToken)
{
    std::string url = URL_API_FIND_BY_CHECKSUMS;
    if (!apiToken.empty()){
      url = URL_API_FIND_BY_CHECKSUMS_AUTH;
    }

    std::map<std::string, std::string> post_data;
    post_data["sha256"] = sha256;
    post_data["sha512"] = sha512;
    std::map<std::string, std::string> headers;
    headers["Accept"] = "application/json";
    return Utils::requestURLWithPost(url, post_data, apiToken, headers);
}

std::string Api::getResultToDisplay(JsonObject *file_json, std::string local_file_sha256, std::string local_file_sha512,
                                    Result::RESULT_TYPE &result_type)
{
  bool matched = false;
  std::string result;
  JsonObject* remote_sha256_json = JsonParser::findByPropertyName(file_json, "sha256sum");
  JsonObject* remote_sha512_json = JsonParser::findByPropertyName(file_json, "sha512sum");
  JsonObject* software_name_json = JsonParser::findByPropertyName(file_json, "software_name");
  JsonObject* release_date_json = JsonParser::findByPropertyName(file_json, "release_date");
  JsonObject* file_name_json = JsonParser::findByPropertyName(file_json, "file_name");
  JsonObject* public_json = JsonParser::findByPropertyName(file_json, "public");
  if (!remote_sha256_json || !remote_sha512_json || !public_json || !file_name_json){
    return "";
  }

  if (public_json->integerValue == 1 && !software_name_json){
    return "";
  }

  std::string remote_sha256sum = remote_sha256_json->stringValue;
  std::string remote_sha512sum = remote_sha512_json->stringValue;
  
  std::transform(remote_sha256sum.begin(), remote_sha256sum.end(), remote_sha256sum.begin(), static_cast<int(*)(int)>(std::tolower));
  std::transform(remote_sha512sum.begin(), remote_sha512sum.end(), remote_sha512sum.begin(), static_cast<int(*)(int)>(std::tolower));

  result.append("File name: ");
  result.append(file_name_json->stringValue);
  result.append("\n");

  if (local_file_sha512 == remote_sha512sum){
    matched = true;
    result.append("Sha512 matched !\n\n");
    result_type = Result::RESULT_TYPE::CORRECT;
  }
  else if (local_file_sha256 == remote_sha256sum){
    matched = true;
    result.append("Sha256 matched !\n\n");
    result_type = Result::RESULT_TYPE::CORRECT;
  }else {
    matched = false;
    result.append("Checksums did NOT match!\n\n");
    result_type = Result::RESULT_TYPE::WRONG;
  }

  result.append("Sha 512: ");
  result.append(local_file_sha512);
  result.append("\n");
  result.append("\n");

  result.append("Sha 256: ");
  result.append(local_file_sha256);
  result.append("\n");
  result.append("\n");

  if (!matched) {
    result.append("Checksums in our Database \n");
    result.append("Sha 512: ");
    if (remote_sha512sum.empty()){
      result.append("Not available");
    }else {
      result.append(remote_sha512sum);
    }
    
    result.append("\n");
    result.append("\n");

    result.append("Sha 256: ");
    if (remote_sha256sum.empty()){
      result.append("Not available");
    } else {
      result.append(remote_sha256sum);
    }
    result.append("\n");
  }
  
  result.append("\n");
  if (public_json->integerValue == 1) {
    result.append("Software: ");
    result.append(software_name_json->stringValue);
    result.append("\n");
    result.append("Release date: ");
    result.append(release_date_json->stringValue);
    result.append("\n");
  }
  return result;
}
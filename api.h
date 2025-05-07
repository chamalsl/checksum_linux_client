#ifndef API_H
#define API_H

#include <string>
#include "third_party/json_parser/json_parser.h"

static const std::string URL_API_GET_TOKEN = "https://checksums.rammini.com/tokens/gettoken";
static const std::string URL_API_FIND_BY_NAME = "https://checksums.rammini.com/api/findByFileName";
static const std::string URL_API_FIND_BY_NAME_AUTH = "https://checksums.rammini.com/api/findByFileNamePrivate";
static const std::string URL_API_FIND_BY_SHA = "https://checksums.rammini.com/api/findBySha256";
static const std::string URL_API_FIND_BY_SHA_AUTH = "https://checksums.rammini.com/api/findBySha256Private";

class Api{

    public:
        static std::pair<short, std::string> findByFileName(std::string file_name, std::string apiToken);
        static std::pair<short, std::string>  findBySha256(std::string sha256, std::string apiToken);
        static std::string getResultToDisplay(JsonObject *file_json, std::string local_file_sha256);
};

#endif //API_H
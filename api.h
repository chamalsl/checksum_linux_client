#ifndef API_H
#define API_H

#include <string>
#include "result.h"
#include "third_party/json_parser/json_parser.h"

static const std::string URL_API_GET_TOKEN = "https://checksums.rammini.com/tokens/gettoken";
static const std::string URL_API_FIND_BY_NAME = "https://checksums.rammini.com/api/findByFileName";
static const std::string URL_API_FIND_BY_NAME_AUTH = "https://checksums.rammini.com/api/findByFileNamePrivate";
static const std::string URL_API_FIND_BY_CHECKSUMS = "https://checksums.rammini.com/api/findByChecksums";
static const std::string URL_API_FIND_BY_CHECKSUMS_AUTH = "https://checksums.rammini.com/api/findByChecksumsPrivate";

class Api{

    public:
        static std::pair<short, std::string> findByFileName(std::string file_name, std::string apiToken);
        static std::pair<short, std::string>  findByChecksums(std::string sha256, std::string sha512, std::string apiToken);
        static std::string getResultToDisplay(JsonObject *file_json, std::string local_file_sha256, std::string local_file_sha512, Result::RESULT_TYPE &result_type);
};

#endif //API_H
#include <string>
#include <vector>
#include <map>
#include <memory>

#ifndef JSONPARSER_H
#define JSONPARSER_H

enum JsonType{
  ARRAY,
  OBJECT,
  STRING,
  STRING_VALUE_ONLY,
  INTEGER_VALUE_ONLY,
  INTEGER,
  NULL_VALUE,
  NULL_ONLY,
  UNKOWN
};

struct JsonObject{

  JsonObject(){
  }

  JsonObject(JsonType p_type){
    type = p_type;
  }

  JsonType type;
  JsonObject* parent = nullptr;
  std::vector<std::unique_ptr<JsonObject>> arrayItems;
  //std::map<std::string, JsonObject*> objectItems;
  std::string propertyName;
  std::string stringValue;
  int integerValue;
  size_t startBracketPos = 0;
};

class JsonParser{

    public:

    std::unique_ptr<JsonObject> parseJson(std::string json);
    std::string toString(JsonObject* jsonobj);
    std::string getError();
    static JsonObject* findByPropertyName(JsonObject* json_obj, std::string property_name);

    private:
    size_t skip_whitespaces(size_t current_idx, std::string str);
    JsonType getType(char c);

    std::string error;

};

#endif //JSONPARSER
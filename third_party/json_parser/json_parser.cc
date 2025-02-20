#include "json_parser.h"
#include <iostream>

std::unique_ptr<JsonObject> JsonParser::parseJson(std::string json){

    error.clear();
    if (json.empty()) {
        return nullptr;
    }
    
    char first_char = json.at(0);
    if (first_char != '[' && first_char != '{'){
        return nullptr;
    }

    JsonType root_type = (first_char == '[') ? ARRAY:OBJECT;
    std::unique_ptr<JsonObject> result(new JsonObject(root_type));
    
    JsonObject* current_jsonobj = result.get();

    for(size_t i=1; i < json.length(); i++){
        i = skip_whitespaces(i, json);
        char c = json.at(i);   
        if (c == '['){
          JsonObject* tmp_jsonobj = new JsonObject(ARRAY);
          tmp_jsonobj->startBracketPos = i;
          tmp_jsonobj->parent = current_jsonobj;
          current_jsonobj->arrayItems.emplace_back(tmp_jsonobj);
          current_jsonobj = tmp_jsonobj;
        }
        /*else if (c == '{' && !current_jsonobj->propertyName.empty()){
          current_jsonobj->type = OBJECT;
        }*/
        else if (c == '{' && !current_jsonobj->propertyName.empty()){
        }
        else if (c == '{'){
          JsonObject* tmp_jsonobj = new JsonObject(OBJECT);
          tmp_jsonobj->startBracketPos = i;
          tmp_jsonobj->parent = current_jsonobj;
          current_jsonobj->arrayItems.emplace_back(tmp_jsonobj);
          current_jsonobj = tmp_jsonobj;
        }
        else if (c == ','){
          size_t next_idx = skip_whitespaces(i+1, json);
          char next_char = json.at(next_idx);
          if (next_char == '}' || next_char == ']' || next_char == ':'){
            error.assign("Unnecessary comma at position - ");
            error.append(std::to_string(i+1));
            error.append(".");
            return nullptr;
          }
          current_jsonobj = current_jsonobj->parent;
        }
        else if (c == '}'){
          current_jsonobj = current_jsonobj->parent;
        }
        else if (c == ']'){
          current_jsonobj = current_jsonobj->parent;
        }
        else if (c == '"' && current_jsonobj->type == STRING && !current_jsonobj->propertyName.empty()){
          if ( i == json.length() -1) {
            return nullptr;
          }
          size_t end_quote = json.find_first_of('"', i + 1);

          if (end_quote == std::string::npos) {
            return nullptr;
          }
          
          std::string prop_value = json.substr(i+1, end_quote - (i+1));
          //std::cout << "Property Value : " << prop_value << "\n";
          current_jsonobj->type = STRING;
          current_jsonobj->stringValue = prop_value;
          i = end_quote;
        }
        else if (isdigit(c) && current_jsonobj->type == INTEGER && !current_jsonobj->propertyName.empty()){
          size_t next_idx;
          std::string number_str = json.substr(i, json.length()-i);
          int number = std::stoi(number_str, &next_idx);
          i = i + next_idx - 1;
          current_jsonobj->integerValue = number;
          
        }
        else if (isdigit(c)) {
          size_t next_idx;
          std::string number_str = json.substr(i, json.length()-i);
          int number = std::stoi(number_str, &next_idx);
          i = i + next_idx - 1;

          JsonObject* tmp_jsonobj = new JsonObject(INTEGER_VALUE_ONLY);
          tmp_jsonobj->integerValue = number;
          tmp_jsonobj->parent = current_jsonobj;
          current_jsonobj->arrayItems.push_back(std::unique_ptr<JsonObject>(tmp_jsonobj));
          current_jsonobj = tmp_jsonobj;
        }
        else if (c == '"'){
          if ( i == json.length() -1) {
            return nullptr;
          }
          size_t end_quote = json.find_first_of('"', i + 1);

          if (end_quote == std::string::npos) {
            return nullptr;
          }
          
          std::string prop_name = json.substr(i+1, end_quote - (i+1));
          //std::cout << "Property Name : " << prop_name << "\n";
          
          i = skip_whitespaces(end_quote+1, json);
          if ((end_quote+1) < json.length() && (json.at(end_quote+1) == ',' || 
             json.at(end_quote+1) == '}' || json.at(end_quote+1) == ']'))
          {
            JsonObject* tmp_jsonobj = new JsonObject(STRING_VALUE_ONLY);
            tmp_jsonobj->stringValue = prop_name;
            tmp_jsonobj->parent = current_jsonobj;
            current_jsonobj->arrayItems.push_back(std::unique_ptr<JsonObject>(tmp_jsonobj));
            current_jsonobj = tmp_jsonobj;
            i--;
            continue;
          }

          //i = end_quote+1;
          if (i > json.length()-2 && json.at(i) != ':'){
            return nullptr;
          }
          //i++;
          /*
          char prop_first_char = json.at(i);
          JsonType type = STRING;
          std::string prop_value;
          if ( prop_first_char == '"'){ //String property
            size_t end_quote_prop = json.find_first_of('"', i+1);
            if (end_quote_prop == std::string::npos) {
              return nullptr;
            }
            prop_value = json.substr(i+1, end_quote_prop - (i+1));
            std::cout << "Property Value : " << prop_value << "\n";
            i = end_quote_prop +1;
            type = STRING;
          } else if(isdigit(prop_first_char)) { //Integer property
            type = INTEGER;
          } else {
            return nullptr;
          }*/


          /*if (type == STRING) {
            tmp_jsonobj->stringValue = prop_value;
          } else if (type == INTEGER) {
            tmp_jsonobj->integerValue = 0;
          }*/
          
          /*if ((i +1) > (json.length() -1 )){
            return nullptr;
          }*/

          i = skip_whitespaces(i+1, json);
          JsonType type = getType(json.at(i));
          i--;
          if (type == UNKOWN) {
            return nullptr;
          }
          JsonObject* tmp_jsonobj = new JsonObject(type);
          tmp_jsonobj->propertyName = prop_name;
          tmp_jsonobj->parent = current_jsonobj;
          current_jsonobj->arrayItems.push_back(std::unique_ptr<JsonObject>(tmp_jsonobj));
          current_jsonobj = tmp_jsonobj;

        }
    }

    if (current_jsonobj!= nullptr && result.get() != current_jsonobj) {
      error.assign("Closing bracket not found for bracket at poition - ");
      error.append(std::to_string(current_jsonobj->startBracketPos + 1));
      error.append(".");
      return nullptr;
    }
    return result;
}

std::string JsonParser::toString(JsonObject* jsonobj) {

  if (jsonobj == nullptr) {
    return "";
  }

  if (jsonobj->type == STRING) {
    return "\"" + jsonobj->propertyName + "\":\"" + jsonobj->stringValue + "\"";
  }

  if (jsonobj->type == STRING_VALUE_ONLY) {
    return "\"" + jsonobj->stringValue + "\"";
  }

  if (jsonobj->type == INTEGER) {
    return "\"" + jsonobj->propertyName + "\":" + std::to_string(jsonobj->integerValue);
  }

  if (jsonobj->type == INTEGER_VALUE_ONLY) {
    return std::to_string(jsonobj->integerValue);
  }

  std::string json_str;
  if (jsonobj->type == OBJECT) {
    if (!jsonobj->propertyName.empty()){
      json_str.append("\"");
      json_str.append(jsonobj->propertyName);
      json_str.append("\"");
      json_str.append(":");
    }

    json_str.append("{");
    for(size_t i=0; i<jsonobj->arrayItems.size(); i++) {
      json_str.append(toString(jsonobj->arrayItems.at(i).get()));
      if (i < jsonobj->arrayItems.size() -1){
        json_str.append(",");
      }
    }
    json_str.append("}");
  }

  if (jsonobj->type == ARRAY) {
    json_str.append("[");
    for(size_t i=0; i<jsonobj->arrayItems.size(); i++) {
      json_str.append(toString(jsonobj->arrayItems.at(i).get()));
      if (i < jsonobj->arrayItems.size() -1){
        json_str.append(",");
      }
    }
    json_str.append("]");
  }
  return json_str;

}

std::string JsonParser::getError()
{
    return error;
}

JsonObject* JsonParser::findByPropertyName(JsonObject* p_json_obj, std::string property_name)
{
  if (!p_json_obj){
    return nullptr;
  }

  for(auto& json_obj: p_json_obj->arrayItems){
    if (json_obj->propertyName == property_name){
      return json_obj.get();
    }
  }
  return nullptr;
}

size_t JsonParser::skip_whitespaces(size_t current_idx, std::string str)
{
    for (size_t i=current_idx; i < str.length(); i++){
      if (str.at(i) != ' '){
         return i;
      }
    }
    //TODO throw error
    return -1;
}

JsonType JsonParser::getType(char c)
{
    if (c == '"') {
      return STRING;
    }
    else if (c == '{') {
      return OBJECT;
    }
    else if (c == '[') {
      return ARRAY;
    }
    else if (isdigit(c)) {
      return INTEGER;
    }
    return UNKOWN;
}

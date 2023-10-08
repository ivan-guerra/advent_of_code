#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include "rapidjson/document.h"

using namespace rapidjson;

static const char* kTypeNames[] = {"Null",  "False",  "True",  "Object",
                                   "Array", "String", "Number"};

bool HasBannedValue(rapidjson::Value::ConstMemberIterator& obj,
                    const std::string& filter = "red");
bool HasBannedValue(rapidjson::Value::ConstValueIterator& obj,
                    const std::string& filter = "red");
int SumInArray(rapidjson::Value::ConstValueIterator& item);
int SumInObject(rapidjson::Value::ConstMemberIterator& obj);

bool HasBannedValue(rapidjson::Value::ConstMemberIterator& obj,
                    const std::string& filter) {
  for (Value::ConstMemberIterator it = obj->value.MemberBegin();
       it != obj->value.MemberEnd(); ++it) {
    if (!std::strcmp("String", kTypeNames[it->value.GetType()])) {
      if (!std::strcmp(filter.c_str(), it->value.GetString())) {
        return true;
      }
    }
  }
  return false;
}

bool HasBannedValue(rapidjson::Value::ConstValueIterator& obj,
                    const std::string& filter) {
  for (Value::ConstMemberIterator it = obj->MemberBegin();
       it != obj->MemberEnd(); ++it) {
    if (!std::strcmp("String", kTypeNames[it->value.GetType()])) {
      if (!std::strcmp(filter.c_str(), it->value.GetString())) {
        return true;
      }
    }
  }
  return false;
}

int SumInArray(rapidjson::Value::ConstValueIterator& item) {
  int sum = 0;

  if (!std::strcmp(kTypeNames[item->GetType()], "Number")) {
    sum += item->GetInt();
  } else if (!std::strcmp(kTypeNames[item->GetType()], "Array")) {
    for (Value::ConstValueIterator it = item->GetArray().Begin();
         it != item->GetArray().End(); ++it) {
      sum += SumInArray(it);
    }
  } else if (!std::strcmp(kTypeNames[item->GetType()], "Object") &&
             !HasBannedValue(item)) {
    for (Value::ConstMemberIterator it = item->MemberBegin();
         it != item->MemberEnd(); ++it) {
      sum += SumInObject(it);
    }
  }
  return sum;
}

int SumInObject(rapidjson::Value::ConstMemberIterator& obj) {
  int sum = 0;
  int type_idx = obj->value.GetType();
  if (!std::strcmp(kTypeNames[type_idx], "Array")) {
    for (Value::ConstValueIterator it = obj->value.GetArray().Begin();
         it != obj->value.GetArray().End(); ++it) {
      sum += SumInArray(it);
    }
  } else if (!std::strcmp(kTypeNames[type_idx], "Number")) {
    sum += obj->value.GetInt();
  } else if (!std::strcmp(kTypeNames[type_idx], "Object") &&
             !HasBannedValue(obj)) {
    for (Value::ConstMemberIterator it = obj->value.MemberBegin();
         it != obj->value.MemberEnd(); ++it) {
      sum += SumInObject(it);
    }
  }
  return sum;
}

int SumAllNumbers(const std::string& json) {
  rapidjson::Document doc;
  doc.Parse(json.c_str());

  int sum = 0;
  for (Value::ConstMemberIterator itr = doc.MemberBegin();
       itr != doc.MemberEnd(); ++itr) {
    sum += SumInObject(itr);
  }
  return sum;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string json;
  std::getline(input, json);

  std::cout << "Answer: " << SumAllNumbers(json) << std::endl;

  exit(EXIT_SUCCESS);
}

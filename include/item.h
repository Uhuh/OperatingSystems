#ifndef ITEM_H
#define ITEM_H
#include <string>

class item
{
public:
  std::string last_updated_time; //EG: "12:05"
  std::string last_updated_date;
  std::string user_perms;
  std::string name; // EG: "/dirName" or "file.cpp"
  item();
};

#endif
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <vector>

#include "../include/item.h"
#include "../include/file.h"

item::item()
{
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  // Set the date and time to (OS time) second and date they're made.
  std::ostringstream u_date;
  std::ostringstream u_time;
  u_date << std::put_time(&tm, "%d-%m-%Y");
  u_time << std::put_time(&tm, "%H-%M");
  this->last_updated_date = u_date.str();
  this->last_updated_time = u_time.str();
  // Assume all perms start with this
  this->user_perms = "-rwxrwxrwx";
}
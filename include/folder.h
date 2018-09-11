#ifndef FOLDER_H
#define FOLDER_H
#include "item.h"
#include "file.h"
#include <vector>

class folder : public item
{
public:
  std::vector<folder *> directories; //directories in current directory
  std::vector<file> files;           //files in current directory
  folder *parentDir;                 //parent of ths directory
  folder() : item(), parentDir(nullptr) {}
  ~folder();
  void list(const std::string &);
  void pwd(const folder*);
  folder *changeD(const std::string &);
  void mkdir(const std::string &);
  void chmod(const std::string &, const std::string &);
  void touch(const std::string &);
  void rmdir(const std::string &);
  void rm(const std::string &);
};

#endif
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

#include "../include/folder.h"

//map for group to perms
const std::map<int, std::string> PERMS_VAL = {
    {7, "rwx"}, {6, "rw-"}, {5, "r-x"}, {4, "r--"}, {3, "-rx"}, {2, "-w-"}, {1, "--x"}, {0, "---"}};

/** ~folder() 
 * @args: deconstructor to avoid memory leaks
 * desc: deletes all directory pointers.
*/

folder::~folder()
{
  std::for_each(this->directories.begin(), this->directories.end(), [](auto &dir) { delete dir; });
}

/** rm
 * @args: name is the file you want to remove
 * desc: removes only files, not a directory.
*/

void folder::rm(const std::string &name)
{
  //search for file, if not found error
  auto foundIt = std::find_if(
      this->files.begin(),
      this->files.end(),
      [name](auto currFile) { return currFile.name == name; });
  if (foundIt != this->files.end())
  {
    this->files.erase(foundIt);
    return;
  }
  //can't remove a directory with rm so error if they try to remove it.
  auto foundD = std::find_if(
      this->directories.begin(),
      this->directories.end(),
      [name](auto currDir) { return currDir->name == name; });
  if (foundD != this->directories.end())
  {
    std::cout << "rm: cannot remove '" << name << "': Is a directory" << std::endl;
    return;
  }
  std::cout << "rm: cannot remove '" << name << "': No such file" << std::endl;
}

/** pwd
 * @args: dir is so we can recursively go up the tree to output 
 * desc: recursively outputs pwd
*/

void folder::pwd(const folder *dir)
{
  if (dir->parentDir)
  {
    pwd(dir->parentDir);
  }
  std::cout << dir->name;
  std::cout << "/";
}

/** list
 * @args: cmd determines if the user used the "-l" flag
 * desc: list out files and folders with extra info or just by name
*/

void folder::list(const std::string &cmd)
{
  if (cmd == "-l")
  {
    //for "ls -l" output all info about files and directories.
    for (auto i : this->directories)
    {
      std::cout << i->user_perms << " " << i->last_updated_date
                << " " << i->last_updated_time << " \033[1;34m" << i->name << "/\033[0m" << std::endl;
    }
    for (auto i : this->files)
    {
      std::cout << i.user_perms << " " << i.last_updated_date
                << " " << i.last_updated_time << " " << i.name << std::endl;
    }
    return;
  }
  else if(cmd != "ls" && cmd != "-l" && cmd != "")
  {
    std::cout << "ls: flag not found '" << cmd << "'" << std::endl;
    return;
  }
  //otherwise the use doesn't use "-l"
  for (auto i : this->directories)
  {
    std::cout << "\033[1;34m" << i->name << "/\033[0m ";
  }
  for (auto i : this->files)
  {
    std::cout << i.name << " ";
  }
  if (!this->files.empty() || !this->directories.empty())
  {
    std::cout << std::endl;
  }
}

/** changeD
 * @args: dir is name of directory user wants to change to
 * desc: returns a folder* so currDir in main is up to date
*/

folder *folder::changeD(const std::string &dir)
{
  if (dir == "cd")
  {
    folder *currDir = this;
    //go up the parent tree,
    while (currDir->parentDir)
    {
      currDir = currDir->parentDir;
    }
    return currDir;
  }
  if ((dir == ".." || dir == "../") && this->parentDir)
  {
    return this->parentDir;
  }
  //using a lambda to find if the directory exist to "cd" to
  auto foundIt = std::find_if(
      this->directories.begin(),
      this->directories.end(),
      [dir](auto currDir) { return currDir->name == dir; });
  if (foundIt == this->directories.end())
  {
    std::cout << "cd: no such file or directory: " << dir << std::endl;
    return this;
  }

  return *foundIt;
}

/** mkdir
 * @args: name is the directory the user wants to make
 * desc: creates a directory upon request if a file or directory don't already exist with the name
*/

void folder::mkdir(const std::string &name)
{
  //if the user doesn't pass any name
  if (name == "mkdir")
  {
    std::cout << "mkdir: missing operand" << std::endl;
    return;
  }
  //warn the user if the directory already exist
  auto foundIt = std::find_if(
      this->directories.begin(),
      this->directories.end(),
      [name](auto currDir) { return currDir->name == name; });
  if (foundIt != this->directories.end())
  {
    std::cout << "mkdir: directory " << name << " already exist" << std::endl;
    return;
  }
  auto foundF = std::find_if(
      this->files.begin(),
      this->files.end(),
      [name](auto currFile) { return currFile.name == name; });
  if (foundF != this->files.end())
  {
    std::cout << "mkdir: cannot create directory '" << name << "': File exist" << std::endl;
    return;
  }
  //When the new folder is made push it to the vector or directories for the parent directory
  auto newFolder = new folder();
  newFolder->parentDir = this;
  newFolder->name = name;
  this->directories.push_back(newFolder);
}

/** rmdir
 * @args: name is directory name
 * desc: removes a requested directory
*/

void folder::rmdir(const std::string &name)
{
  if (name == "rmdir")
  {
    std::cout << "rmdir: missing operand" << std::endl;
    return;
  }

  auto foundIt = std::find_if(
      this->directories.begin(),
      this->directories.end(),
      [name](auto currDir) { return currDir->name == name; });
  if (foundIt == this->directories.end())
  {
    std::cout << "rmdir: directory not found" << std::endl;
    return;
  }

  delete *foundIt;
  this->directories.erase(foundIt);
}

/** touch
 * @args: name is the name of the file/directory
 * desc: updates the date and time of a file/directory
*/

void folder::touch(const std::string &name)
{
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::ostringstream u_time;
  std::ostringstream u_date;
  u_date << std::put_time(&tm, "%d-%m-%Y");
  u_time << std::put_time(&tm, "%H-%M");

  auto foundIt = std::find_if(
      this->files.begin(),
      this->files.end(),
      [name](auto &f) { return f.name == name; });
  if (foundIt != this->files.end())
  {
    // Update existing file
    foundIt->last_updated_time = u_time.str();
    foundIt->last_updated_date = u_date.str();
    return;
  }

  // Create new file
  file newFile;
  newFile.name = name;
  this->files.push_back(newFile);
  return;
}

/** chmod
 * @args: perm is eg: 777, file is the file/directory name
 * desc: updates user permissions for a file/directory
*/

void folder::chmod(const std::string &perm, const std::string &file)
{
  int permNum = std::stoi(perm);
  std::string userPerms = "";
  //parse the three numbers EG: 754 -> 7, 5, 4
  std::vector<int> perms = {permNum / 100, (permNum % 100) / 10, (permNum % 10)};
  if (permNum > 777)
  {
    std::cout << "chmod: invalid mode: '" << perm << "'" << std::endl;
    return;
  }
  //if the file exist
  auto foundIt = std::find_if(
      this->files.begin(),
      this->files.end(),
      [file](auto f) { return f.name == file; });
  if (foundIt != this->files.end())
  {
    //files don't have 'd' in front
    userPerms = "-";
    // the vector 'perms' above is iterated over the PERMS map and added to the perms string
    for (auto i : perms)
    {
      userPerms += PERMS_VAL.find(i)->second;
    }
    // update the file perms
    foundIt->user_perms = userPerms;
    return;
  }

  for (auto dir : this->directories)
  {
    if (dir->name == file)
    {
      userPerms = "d";
      for (auto i : perms)
      {
        userPerms += PERMS_VAL.find(i)->second;
      }
      dir->user_perms = userPerms;
      return;
    }
  }
  std::cout << "chmod: cannot access '" << file << "': No such file or directory" << std::endl;
}
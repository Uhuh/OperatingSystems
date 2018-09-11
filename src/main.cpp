#include <iostream>
#include <sstream>
#include <algorithm>

#include "../include/folder.h"

//enum to keep track of commands.
enum class command
{
  fail = -1,
  ls = 0,
  cd = 1,
  pwd = 2,
  mkdir = 3,
  rmdir = 4,
  touch = 5,
  rm = 6,
  chmod = 7,
  quit = 8,
  exit = 9,
};

//The list of commands.
const std::vector<std::string> COMMAND_NAMES = {
    "ls", "cd", "pwd", "mkdir", "rmdir", "touch", "rm", "chmod", "quit", "exit"};

command to_enum(const std::string &);

int main()
{
  folder root;
  auto *currDir = &root; // Directory we're in.
  root.name = "home";
  std::string cmd = "";
  std::string file = "";
  bool quit = false;

  while (!quit)
  {
    std::cout << "\033[1;32m➜  \033[0m" << "\033[1;34m" << currDir->name << "\033[0m$ ";
    std::getline(std::cin, cmd);
    std::istringstream ss(cmd);
    ss >> cmd;
    switch (to_enum(cmd))
    {
    case command::ls:
      ss >> cmd;
      currDir->list(cmd);
      break;
    case command::cd:
      ss >> cmd;
      currDir = currDir->changeD(cmd);
      break;
    case command::pwd:
      currDir->pwd(currDir);
      std::cout << std::endl;
      break;
    case command::mkdir:
      ss >> cmd;
      currDir->mkdir(cmd);
      break;
    case command::rmdir:
      ss >> cmd;
      currDir->rmdir(cmd);
      break;
    case command::rm:
      ss >> cmd;
      currDir->rm(cmd);
      break;
    case command::touch:
      ss >> cmd;
      currDir->touch(cmd);
      break;
    case command::chmod:
      ss >> cmd;
      ss >> file;
      currDir->chmod(cmd, file);
      break;
    case command::quit:
    case command::exit:
      return 1;
    case command::fail:
    default:
      std::cout << "bash: command not found: " << cmd << std::endl;
    }
  }

  return 0;
}

command to_enum(const std::string &str)
{
  // Search for the command, if not found return a fail "command not found"
  auto foundIt = std::find(COMMAND_NAMES.begin(), COMMAND_NAMES.end(), str);
  if (foundIt == COMMAND_NAMES.end())
  {
    return command::fail;
  }

  return static_cast<command>(
      std::distance(COMMAND_NAMES.begin(), foundIt));
}

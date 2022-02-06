/*
Deployment App of ModPlug Player for MSYS2 Platform
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <cstring>
#include <vector>
#include <exception>
#include <filesystem>

#define BUFFER_SIZE 2048

namespace fs = std::filesystem;

struct DynamicDependency {
  std::string fileName;
  std::string filePath;
  std::string address;
};

std::string trim(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return std::string(it, rit.base());
}

DynamicDependency getDynamicDependency(const std::string &line) {
  DynamicDependency dynamicDependency, emptyDependency;
  size_t greaterOrEqualPosition = line.rfind("=>");
  size_t openParanthesisPosition = line.rfind('(');
  size_t closeParanthesisPosition = line.rfind(')');
  size_t delimiterPos = line.find("=>");
  if(greaterOrEqualPosition == std::string::npos ||
      openParanthesisPosition == std::string::npos ||
      closeParanthesisPosition == std::string::npos
  ) return emptyDependency;

  dynamicDependency.fileName = trim(line.substr(0, greaterOrEqualPosition));
  dynamicDependency.address = trim(line.substr(openParanthesisPosition+1, closeParanthesisPosition - openParanthesisPosition-1));
  dynamicDependency.filePath = trim(line.substr(greaterOrEqualPosition + 2, openParanthesisPosition - 1 - greaterOrEqualPosition - 1));
  return dynamicDependency;
}

std::string getEnvVar( const std::string & key ) {
    char * val = getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}

std::string winPath2msysPath(const std::string &winPath) {
  std::string msysPath = winPath;
  std::erase(msysPath, ':');
  std::replace( msysPath.begin(), msysPath.end(), '\\', '/');
  msysPath.insert(0, 1, '/');
  return msysPath;
}

bool isContained(const std::vector<DynamicDependency> &dynamicDependencies, const DynamicDependency &dynamicDependency) {
  for(const DynamicDependency & currentDynamicDependency : dynamicDependencies)
    if(currentDynamicDependency.fileName == dynamicDependency.fileName)
      return true;
  return false;
}

void omitWindowsDlls(std::vector<DynamicDependency> &dynamicDependencies) {
  std::string winDir = getEnvVar("WinDir");
  winDir = winPath2msysPath(winDir);
  std::transform(winDir.begin(), winDir.end(), winDir.begin(), ::tolower);
  //std::cout<<winDir;
  for(int i=0; i<dynamicDependencies.size(); i++) {
    DynamicDependency & dynamicDependency = dynamicDependencies[i];
    std::string filePath = dynamicDependency.filePath;
    std::transform(filePath.begin(), filePath.end(), filePath.begin(), ::tolower);
    if(filePath.starts_with(winDir)) {
      dynamicDependencies.erase(dynamicDependencies.begin() + i);
      i--;
    }
  }
}

/*
void copyFile(std::string sourcePath, std::string destinationPath) {
    fs::path sourceFile = sourcePath;
    fs::path targetParent = destinationPath;
    auto target = targetParent / sourceFile.filename(); // sourceFile.filename() returns "sourceFile.ext".

    try // If you want to avoid exception handling, then use the error code overload of the following functions.
    {
        fs::create_directories(targetParent); // Recursively create target directory if not existing.
        fs::copy_file(sourceFile, target, fs::copy_options::overwrite_existing);
    }
    catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.
    {
        std::cout << e.what();
    }
}
*/
void copyFile(std::string sourcePath, std::string destinationPath) {
  char *command = new char[BUFFER_SIZE];
  sprintf(command, "cp \"%s\" \"%s\"", sourcePath.c_str(), destinationPath.c_str());
  system(command);
}

void deployQtDlls(std::string qtAppPath) {
  char *command = new char[BUFFER_SIZE];
  sprintf(command, "windeployqt-qt6.exe \"%s\"", qtAppPath.c_str());
  system(command);
}

int main(int argc, char ** argv)
{
    FILE *file;
    char *command = new char[BUFFER_SIZE];
    char c = 0;
    sprintf(command, "ldd \"%s\"", argv[1]);

    if (0 == (file = (FILE*)popen(command, "r")))
    {
        perror("popen() failed.");
        exit(EXIT_FAILURE);
    }

    char buf[BUFFER_SIZE];
    std::vector<DynamicDependency> dynamicDependencies;

    while (fgets(buf, sizeof(buf), file) != NULL) {
      buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
      DynamicDependency dynamicDependency = getDynamicDependency(buf);
      if(!isContained(dynamicDependencies, dynamicDependency))
        dynamicDependencies.push_back(dynamicDependency);
    }

    pclose(file);

    omitWindowsDlls(dynamicDependencies);

    for(auto dynamicDependency:dynamicDependencies){
      copyFile(dynamicDependency.filePath, argv[2]);
    }

    //deployQtDlls(argv[1]);

    return EXIT_SUCCESS;
}

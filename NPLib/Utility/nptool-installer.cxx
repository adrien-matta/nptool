#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#include "NPDetectorFactory.h"
int main(int argc , char** argv){
  int return_value;

#ifdef __APPLE__
  std::string CORRECT_LIB_EXTENSION = ".dylib";
  std::string INCORRECT_LIB_EXTENSION = ".so";
#endif
#ifdef __linux__
  std::string INCORRECT_LIB_EXTENSION = ".dylib";
  std::string CORRECT_LIB_EXTENSION = ".so";
#endif
#ifdef __FreeBSD__
  std::string INCORRECT_LIB_EXTENSION = ".dylib";
  std::string CORRECT_LIB_EXTENSION = ".so";
#endif


  // Build the lib list from the argument
  std::string detlist="";
  for(unsigned int i = 1 ; i < argc ; i++){
    detlist+=argv[i];
    detlist+=" ";
  }

  // Generate the Class list with Token for autoloading of the Detector classes
  DIR *dir;
  struct dirent *ent;
 std::string path = getenv("NPTOOL");
  path += "/NPLib/lib";
 std::string lib;
  if ((dir = opendir (path.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      lib= ent->d_name ;
      if(lib.find("lib")!=std::string::npos)
        dlopen(lib.c_str(),RTLD_LAZY);
    }
    closedir (dir);
  }

  NPL::DetectorFactory::getInstance()->CreateClassList("ClassList.txt");

  // Copy pcm and rootmap file
  path = getenv("NPTOOL");
  path+="/NPLib";

  if ((dir = opendir (path.c_str())) != NULL){
    while ((ent = readdir (dir)) != NULL) {
      if(ent->d_type == DT_DIR){
       std::string folderName = ent->d_name ;
        if(strncmp(folderName.c_str(),".",1)!=0 && folderName!="scripts"
            && folderName!="lib" && folderName!="include"  
            && folderName!="Utility" && folderName!="bin"
            && folderName!="CMakeFiles"){
          if(detlist.length()==0 || detlist.find(folderName)!=std::string::npos){
           std::string command = "ls "+ folderName+"/*.pcm > /dev/null 2>/dev/null";
            return_value=system(command.c_str());             
            if(!return_value){
             std::string cmd1 = "cp " + folderName+"/*.pcm lib/ > /dev/null 2>/dev/null"; 
              return_value=system(cmd1.c_str());
            }

            command = "ls "+ folderName+"/*.rootmap > /dev/null 2>/dev/null";
            return_value=system(command.c_str());             
            if(!return_value){
             std::string cmd2 = "cp " + folderName+"/*.rootmap lib/ > /dev/null 2>/dev/null" ;
              return_value=system(cmd2.c_str());
            }
          }
        }
      } 
    } 
  }
  
  path = getenv("NPTOOL");
  path+="/NPLib/Detectors";
  if ((dir = opendir (path.c_str())) != NULL){
    while ((ent = readdir (dir)) != NULL) {
      if(ent->d_type == DT_DIR){
       std::string folderName = ent->d_name ;
        if(strncmp(folderName.c_str(),".",1)!=0){
          if(detlist.length()==0 || detlist.find(folderName)!=std::string::npos){
           std::string command = "ls "+ folderName+"/*.pcm > /dev/null 2>/dev/null";
            return_value=system(command.c_str());             
            if(!return_value){
             std::string cmd1 = "cp " + folderName+"/*.pcm lib/ > /dev/null 2>/dev/null"; 
              return_value=system(cmd1.c_str());
            }
            command = "ls Detectors/"+ folderName+"/*.rootmap > /dev/null 2>/dev/null";
            return_value=system(command.c_str());             
            if(!return_value){
             std::string cmd2 = "cp Detectors/" + folderName+"/*.rootmap lib/ > /dev/null 2>/dev/null" ;
              return_value=system(cmd2.c_str());
            }
          }
        }
      } 
    } 
  }
  // same for detector level
  // Copy pcm and rootmap file
  path = getenv("NPTOOL");
  path+="/NPLib/Detectors";

  if ((dir = opendir (path.c_str())) != NULL){
    while ((ent = readdir (dir)) != NULL) {
      if(ent->d_type == DT_DIR){
       std::string folderName = ent->d_name ;
        if(strncmp(folderName.c_str(),".",1)!=0 && folderName!="scripts"
            && folderName!="lib" && folderName!="include"  
            && folderName!="Utility" && folderName!="bin"
            && folderName!="CMakeFiles"){
          if(detlist.length()==0 || detlist.find(folderName)!=std::string::npos){
           std::string command = "ls Detectors/"+ folderName+"/*.pcm > /dev/null 2>/dev/null";
            return_value=system(command.c_str());             
            if(!return_value){
             std::string cmd1 = "cp Detectors/" + folderName+"/*.pcm lib/ > /dev/null 2>/dev/null"; 
              return_value=system(cmd1.c_str());
            }

            command = "ls "+ folderName+"/*.rootmap > /dev/null 2>/dev/null";
            return_value=system(command.c_str());             
            if(!return_value){
             std::string cmd2 = "cp Detectors/" + folderName+"/*.rootmap lib/ > /dev/null 2>/dev/null" ;
              return_value=system(cmd2.c_str());
            }
          }
        }
      } 
    } 
  }

  //  Change all the .so to .dylib for Mac Os X in the installed rootmap    
#ifdef __APPLE__
  path = getenv("NPTOOL");
  path += "/NPLib/*/*.rootmap";
 std::string command = "ls "+ path +" > /dev/null 2>/dev/null";
  return_value=system(command.c_str());

  if(!return_value){
  std::string command = "sed -i '' -e 's/"+INCORRECT_LIB_EXTENSION+"/"+CORRECT_LIB_EXTENSION+"/g' "+path;  
  return_value=system(command.c_str());  
  }
#endif  

  return 0;
}

#include<iostream>
#include<stdlib.h>
#include"NPDetectorFactory.h"

int main(int argc , char** argv){
  // Go to npl
  string path = getenv("NPTOOL");
  string pathNPL =path+ "/NPLib";
  string pathNPS = path+"/NPSimulation";
  string pwd = getenv("PWD");

  string current="";
  
  if(pwd==pathNPL)
    current = "NPLib";
  else if(pwd==pathNPS)
    current = "NPSimulation";

  else{
    std::cout << "\r \033[1;31m ERROR: nptool-cleaner utility online clean NPLib and NPSimulation \033[0m" << endl;
    exit(1);
  }

 
  int return_value = -1 ;
  string answer;
  bool check = true;
  std::cout << "\r\033[1;31m" << std::endl << std::endl;
  std::cout << "********************************************************************************" << std::endl;
  std::cout << "********************************************************************************" << std::endl;
  std::cout << "**** You are about to clean " << current << std::endl;
  std::cout << "**** -> The folder will be revert to pre cmake stage" << std::endl;
  std::cout << "**** -> Only generated file will be suppressed" << std::endl;
  std::cout << "**** -> " << current << " will not be usable without recompilation" << std::endl;

  while(check){
    std::cout << "**** Do you want to proceed ? (y/n)  " ;
    std::cin >> answer;
    if(answer =="n" || answer =="N"){
      std::cout << "\033[0m" ;
      return 0;
    }
    else if(answer =="y" || answer =="Y"){
      std::cout << "Proceed" << std::endl;
      check = false;
    }
    else{
    std::cout << "Reply by y (yes) or n (no)" << std::endl;
    }
  }
  std::cout << "\033[0m" ;
  
  std::string command,command2;
  // Dict     
  command = "ls ./*/*/*Dict* > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./*/*/*Dict* > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }  
   // DetectorClassList
  command = "ls ./ClassList.txt > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./ClassList.txt > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
  
  // pcm 
  command = "ls ./*/*/*.pcm > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./*/*/*.pcm > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
 
  // rootmap 
  command = "ls ./*/*/*.rootmap > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./*/*/*.rootmap > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
  
   // Makefile 
  command = "ls ./*/*/Makefile > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./*/*/Makefile > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
   // ninja
  command = "ls ./*/*/.ninja > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
// Prefer global stter that guaranty the vector are al the same size
    command2 = "rm -f ./*/*/.ninja > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
   // lib 
  command = "ls ./lib/*> /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./lib/* > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
   // include
  command = "ls ./include/* > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./include/* > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
   // CMakeFiles
  command = "ls ./*/*/CMakeFiles > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f .Rf ./*/*/CMakeFiles > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
    command2 = "rm -f .Rf ./CMakeFiles > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   

  }
  
  // CMakeCache
  command = "ls ./CMakeCache.txt > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./CMakeCache.txt > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }

  // liblist 
  command = "ls ./liblist > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./liblist > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }

  // install manifest 
  command = "ls ./install_manifest > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./install_manifest > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
   // cmake
  command = "ls ./*/cmake_install.cmake > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./*/cmake_install.cmake > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
   // so 
  command = "ls ./*/*/*.so > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./*/*/*.so > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
   // dylib
  command = "ls ./*/*/*.dylib > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./*/*/*.dylib > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
    //bin 
  command = "ls ./bin/* > /dev/null 2>/dev/null";
  return_value=system(command.c_str());             
  if(!return_value){
    command2 = "rm -f ./bin/* > /dev/null 2>/dev/null";
    return_value=system(command2.c_str());   
  }
  
  return 0;
}

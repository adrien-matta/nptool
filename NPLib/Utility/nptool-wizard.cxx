// C++ headers
#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
// NPTool header
#include "NPVDetector.h" 


// A function to get the list of existing detector
void GetListOfDetector(std::set<string>& LowerName){
  DIR *dir;
  struct dirent *ent;
  string path = getenv("NPTOOL");
  path += "/NPLib/Detectors";
  string name;
  if ((dir = opendir (path.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      name= ent->d_name ;
      std::transform(name.begin(), name.end(),name.begin(), ::tolower);
      LowerName.insert(name);
    }
    closedir (dir);
  }
}


//// The main program
int main(int argc , char** argv){

  // check for the force flag
  bool force = false;
  for(unsigned int i = 0 ; i < argc ; i++ ){  
    string flag = argv[i];
    if(flag=="-f")
      force = true;
  }

  // List of existing detector name and close name
  std::set<string> LowerName;
  GetListOfDetector(LowerName);

  // Find the different paths
  std::string path    = getenv("NPTOOL");
  std::string pathNPL = path + "/NPLib/Detectors/";
  std::string pathNPS = path + "/NPSimulation/Detectors/";
  std::string pathInputs = path + "/Inputs/DetectorConfiguration/";

  std::string pwd     = getenv("PWD");

  int return_value = -1 ;
  std::string command;
  std::string DetectorName;
  std::string author,buffer;
  std::string email;

  // Get the current year and month
  time_t t = time(0); 
  struct tm* now = localtime( & t );
  std::string month;
  switch(now->tm_mon + 1){
    case 1:
      month = "January";
      break;

    case 2:
      month = "February";
      break;

    case 3:
      month = "March";
      break;

    case 4:
      month = "April";
      break;

    case 5:
      month = "May";
      break;

    case 6:
      month = "June";
      break;

    case 7:
      month = "July";
      break;

    case 8:
      month = "August";
      break;

    case 9:
      month = "September";
      break;

    case 10:
      month = "October";
      break;

    case 11:
      month = "November";
      break;

    case 12:
      month = "December";
      break;

  }
  std::string year = NPL::itoa(now->tm_year+1900);

  // start nptool-wizard
  bool check = true;
  std::cout << "\r\033[1;32m" << std::endl << std::endl;
  std::cout << "********************************************************************************"<< std::endl;
  std::cout << "********************************* nptool wizard ********************************"<< std::endl;
  std::cout << "********************************************************************************"<< std::endl;

  std::cout << "           This utility will create a new detector skeleton"<< std::endl;
  std::cout << "\033[1;36m-> What is the detector name ? \033[0m";
  std::cin >> DetectorName;

  // Cheking if the name does not already exist or is close to something else:
  if(!force){
    string lname = DetectorName;
    std::transform(lname.begin(), lname.end(),lname.begin(), ::tolower);
    if(LowerName.find(lname)!=LowerName.end()){
      cout << "\033[1;31m**** ERROR : A detector with name close to \033[1;36m" << DetectorName << " \033[1;31malready exist. use \033[1;36m-f\033[1;31m flag to force recreation.\033[0m" << endl;
      exit(1);
    }
  }

  std::cout << "\033[1;36m-> What is your name (firstname and surname)? \033[0m";
  std::cin >> author; 
  getline(cin, buffer);
  istringstream oss(buffer);
  while (oss>> buffer) author += " " + buffer;
  std::cout << "\033[1;36m-> What is your email? \033[0m";
  std::cin >> email;

  std::cout << std::endl << std::endl;

  // Creating the detector files name
  std::string DataFile_h      = "T"+DetectorName+"Data.h";
  std::string DataFile_cxx    = "T"+DetectorName+"Data.cxx";
  std::string PhysicsFile_h   = "T"+DetectorName+"Physics.h";
  std::string PhysicsFile_cxx = "T"+DetectorName+"Physics.cxx";
  std::string SpectraFile_h   = "T"+DetectorName+"Spectra.h";
  std::string SpectraFile_cxx = "T"+DetectorName+"Spectra.cxx";
  std::string SimFile_h       = DetectorName+".hh";
  std::string SimFile_cxx     = DetectorName+".cc";
  std::string InputFile       = DetectorName+".detector";

  // Create npl folder for new detector
  command = "mkdir " + pathNPL + DetectorName + " > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  // Copy template files to new detector directory
  command = "cp " + pathNPL + "../ressources/DetectorSkeleton/NPLib/* " 
    + pathNPL + DetectorName +"/ > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  // Edit npl file
  command = "sed -i '' -e \"s/DETECTORNAME/"+ DetectorName +"/g\" "
    + pathNPL + DetectorName +"/*.* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/XAUTHORX/"+ author+"/g\" "
    + pathNPL + DetectorName +"/*.* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/XMAILX/"+ email+"/g\" "
    + pathNPL + DetectorName +"/*.* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/XYEARX/"+ year +"  /g\" "
    + pathNPL + DetectorName +"/*.* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/XMONTHX/"+ month +"/g\" "
    + pathNPL + DetectorName +"/*.* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  // change files name
  command = "mv " + pathNPL + DetectorName +"/TDETECTORNAMEData.h " 
    + pathNPL + DetectorName + "/" + DataFile_h ;
  return_value = system(command.c_str());
  command = "mv " + pathNPL + DetectorName +"/TDETECTORNAMEData.cxx " 
    + pathNPL + DetectorName + "/" + DataFile_cxx ;
  return_value = system(command.c_str());

  command = "mv " + pathNPL + DetectorName +"/TDETECTORNAMEPhysics.h " 
    + pathNPL + DetectorName + "/" + PhysicsFile_h ;
  return_value = system(command.c_str());
  command = "mv " + pathNPL + DetectorName +"/TDETECTORNAMEPhysics.cxx " 
    + pathNPL + DetectorName + "/" + PhysicsFile_cxx ;
  return_value = system(command.c_str());

  command = "mv " + pathNPL + DetectorName +"/TDETECTORNAMESpectra.h " 
    + pathNPL + DetectorName + "/" + SpectraFile_h ;
  return_value = system(command.c_str());
  command = "mv " + pathNPL + DetectorName +"/TDETECTORNAMESpectra.cxx " 
    + pathNPL + DetectorName + "/" + SpectraFile_cxx ;
  return_value = system(command.c_str());


  // Create nps folder
  command = "mkdir " + pathNPS + DetectorName + " > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  // Add nps file
  command = "cp " + pathNPL + "../ressources/DetectorSkeleton/NPSimulation/*.* " 
    + pathNPS + DetectorName +"/ > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/DETECTORNAME/"+ DetectorName +"/g\" "
    + pathNPS + DetectorName +"/*.* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/XAUTHORX/"+ author+"/g\" "
    + pathNPS + DetectorName +"/*.* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/XMAILX/"+ email+"/g\" "
    + pathNPS + DetectorName +"/*.* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/XYEARX/"+ year +"  /g\" "
    + pathNPS + DetectorName +"/*.* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/XMONTHX/"+ month +"/g\" "
    + pathNPS + DetectorName +"/*.* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  // change files name
  command = "mv " + pathNPS + DetectorName +"/DETECTORNAME.hh " 
    + pathNPS + DetectorName + "/" + SimFile_h ;
  return_value = system(command.c_str());
  command = "mv " + pathNPS + DetectorName +"/DETECTORNAME.cc " 
    + pathNPS + DetectorName + "/" + SimFile_cxx ;
  return_value = system(command.c_str());

  // Add input file
  command = "cp " + pathNPL + "../ressources/DetectorSkeleton/Inputs/*.* " 
    + pathInputs +" > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/DETECTORNAME/"+ DetectorName +"/g\" "
    + pathInputs + "DETECTORNAME.detector > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  // change files name
  command = "mv " + pathInputs +"DETECTORNAME.detector " 
    + pathInputs + DetectorName + ".detector" ;
  return_value = system(command.c_str());

  // Provide a list of created file 
  std::cout << "\t List of created files to\033[1;35m edit \033[0m:" << std::endl;
  std::cout << "\t NPLib :" << std::endl;
  std::cout << "\t\t -> Folder: Detectors/" << DetectorName << std::endl;
  std::cout << "\t\t -> Data Class: \033[1;35m" << DataFile_h 
    << "\033[0m and \033[1;35m" << DataFile_cxx << "\033[0m" << std::endl;
  std::cout << "\t\t -> Physics Class: \033[1;35m" << PhysicsFile_h 
    << "\033[0m and \033[1;35m" << PhysicsFile_cxx << "\033[0m" << std::endl;
  std::cout << "\t\t -> Spectra Class: \033[1;35m" << SpectraFile_h 
    << "\033[0m and \033[1;35m" << SpectraFile_cxx << "\033[0m" << std::endl;
  std::cout << std::endl;
  std::cout << "\tNPSimulation :" << std::endl;
  std::cout << "\t\t -> Folder: Detectors/" << DetectorName << std::endl;
  std::cout << "\t\t -> Detector Class: \033[1;35m" << SimFile_h 
    << "\033[0m and \033[1;35m" << SimFile_cxx << "\033[0m" << std::endl;
  std::cout << std::endl;
  std::cout << "\tInputs :" << std::endl;
  std::cout << "\t\t -> Detector input file: \033[1;35m" 
    << InputFile << "\033[0m" << std::endl;
  std::cout << std::endl;

  // Adding file to git
  string git;
  std::cout << "\033[1;36m-> Do you want to add those files to the nptool repository ? (y/n) \033[0m";
  std::cin >> git;
  if(git == "y" || git == "Y"){
        command = "git add -f "   
      + pathNPL+DetectorName+"/CMakeLists.txt "       
      + pathNPL+DetectorName+"/"+DataFile_h + " "     
      + pathNPL+DetectorName+"/"+DataFile_cxx + " "  
      + pathNPL+DetectorName+"/"+PhysicsFile_h + " "  
      + pathNPL+DetectorName+"/"+PhysicsFile_cxx+ " " 
      + pathNPL+DetectorName+"/"+SpectraFile_h + " "  
      + pathNPL+DetectorName+"/"+SpectraFile_cxx+ " " 
      + pathNPS+DetectorName+"/CMakeLists.txt "       
      + pathNPS+DetectorName+"/"+SimFile_h + " "       
      + pathNPS+DetectorName+"/"+SimFile_cxx + " "     
      + pathInputs+InputFile;
    return_value = system(command.c_str());
    if(return_value==0)
      std::cout << "\033[1;32m**** Files added, use \033[1;36mgit commit -a \033[1;32m to commit them when ready ****\033[0m" << endl;

  }

  // Display help message
  std::cout << std::endl << "\033[1;31m**** For help editing those files visit \033[1;36mhttp://nptool.org/manual/adding-detectors \033[1;31m****\033[0m" << std::endl << std::endl;


  return 0;
}

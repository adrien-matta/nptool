#include<iostream>
#include<sstream>

#include<stdlib.h>
#include<ctime>
#include"NPVDetector.h" 
int main(int argc , char** argv){
  // Find the different path
  std::string path = getenv("NPTOOL");
  std::string pathNPL = path+ "/NPLib/Detectors/";
  std::string pathNPS = path+"/NPSimulation/";
  std::string pwd = getenv("PWD");

  int return_value = -1 ;
  std::string command;
  std::string answer;
  std::string author,buffer;
  std::string email;

  // Get the current year and month
  time_t t = time(0); 
  struct tm* now = localtime( & t );
  std::string month;
  switch(now->tm_mon + 1){
    case 1:
      month = "January";
    case 2:
      month = "February";
    case 3:
      month = "March";
    case 4:
      month = "April";
    case 5:
      month = "May";
    case 6:
      month = "June";
    case 7:
      month = "July";
    case 8:
      month = "August";
    case 9:
      month = "September";
    case 10:
      month = "October";
    case 11:
      month = "November";
    case 12:
      month = "December";
  }

  std::string year = NPL::itoa(now->tm_year+1900);
  //////////
  bool check = true;
  std::cout << "\r\033[1;32m" << std::endl << std::endl;
  std::cout << "********************************************************************************"<< std::endl;
  std::cout << "********************************* nptool wizard ********************************"<< std::endl;
  std::cout << "********************************************************************************"<< std::endl;

  std::cout << "           This utility will create a new detector skeleton"<< std::endl;
  std::cout << "\033[1;36m-> What is the detector name ? \033[0m" ;
  std::cin >> answer ;
  std::cout << "\033[1;36m-> What is your name (firstname and surname)? \033[0m" ;
  std::cin >> author; 
  getline(cin, buffer);
  istringstream oss(buffer);
  while(oss>> buffer)
    author +=" " + buffer ;
  std::cout << "\033[1;36m-> What is your email? \033[0m" ;
  std::cin >> email ;

  std::cout << std::endl << std::endl;

  // Cheking if the name does not already exist or is close to something else:
  // TODO //
  ////////// 

  // Creating the detector file name
  std::string DataFile_h = "T"+answer+"Data.h";
  std::string DataFile_cxx = "T"+answer+"Data.cxx";
  std::string PhysicsFile_h = "T"+answer+"Physics.h";
  std::string PhysicsFile_cxx = "T"+answer+"Physics.cxx";
  std::string SpectraFile_h = "T"+answer+"Spectra.h";
  std::string SpectraFile_cxx = "T"+answer+"Spectra.cxx";
  std::string SimFile_h = answer+".hh";
  std::string SimFile_cxx = answer+".cc";
  std::string InputFile = answer+".detector";

  // Create npl folder
  command = "mkdir " + pathNPL + answer + " > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  // Add npl file
  command = "cp " + pathNPL + " Utility/DetectorSkeleton/NPLib/* " 
    + pathNPL + answer +"/ > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  // Edit npl file
   command = "sed -i '' -e \"s/DETECTORNAME/"+ answer +"/g\" "
    + pathNPL + answer +"/* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());
 
  command = "sed -i '' -e \"s/XAUTHORX/"+ author+"/g\" "
    + pathNPL + answer +"/* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());
  
  command = "sed -i '' -e \"s/XMAILX/"+ email+"/g\" "
    + pathNPL + answer +"/* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());
  
  command = "sed -i '' -e \"s/XYEARX/"+ year +"  /g\" "
    + pathNPL + answer +"/* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());

  command = "sed -i '' -e \"s/XMONTHX/"+ month +"/g\" "
    + pathNPL + answer +"/* > /dev/null 2> /dev/null";
  return_value = system(command.c_str());


  command = "mv " + pathNPL + answer +"/TDETECTORNAMEData.h " 
    + pathNPL + answer + "/" + DataFile_h ;
  return_value = system(command.c_str());
  command = "mv " + pathNPL + answer +"/TDETECTORNAMEData.cxx " 
    + pathNPL + answer + "/" + DataFile_cxx ;
  return_value = system(command.c_str());

  command = "mv " + pathNPL + answer +"/TDETECTORNAMEPhysics.h " 
    + pathNPL + answer + "/" + PhysicsFile_h ;
  return_value = system(command.c_str());
  command = "mv " + pathNPL + answer +"/TDETECTORNAMEPhysics.cxx " 
    + pathNPL + answer + "/" + PhysicsFile_cxx ;
  return_value = system(command.c_str());

  command = "mv " + pathNPL + answer +"/TDETECTORNAMESpectra.h " 
    + pathNPL + answer + "/" + SpectraFile_h ;
  return_value = system(command.c_str());
  command = "mv " + pathNPL + answer +"/TDETECTORNAMESpectra.cxx " 
    + pathNPL + answer + "/" + SpectraFile_cxx ;
  return_value = system(command.c_str());

  // Create nps folder

  // Add nps file

  // Add input file

  // Provide a list of created file 
  std::cout << "\t List of created files to\033[1;35m edit \033[0m:" << std::endl;
  std::cout << "\t NPLib :" << std::endl;
  std::cout << "\t\t -> Folder: Detectors/" << answer << std::endl;
  std::cout << "\t\t -> Data Class: \033[1;35m" << DataFile_h 
    << "\033[0m and \033[1;35m" << DataFile_cxx << "\033[0m" << std::endl;
  std::cout << "\t\t -> Physics Class: \033[1;35m" << PhysicsFile_h 
    << "\033[0m and \033[1;35m" << PhysicsFile_cxx << "\033[0m" << std::endl;
  std::cout << "\t\t -> Spectra Class: \033[1;35m" << SpectraFile_h 
    << "\033[0m and \033[1;35m" << SpectraFile_cxx << "\033[0m" << std::endl;
  std::cout << std::endl;
  std::cout << "\tNPSimulation :" << std::endl;
  std::cout << "\t\t -> Folder: Detectors/" << answer << std::endl;
  std::cout << "\t\t -> Detector Class: \033[1;35m" << SimFile_h 
    << "\033[0m and \033[1;35m" << SimFile_cxx << "\033[0m" << std::endl;
  std::cout << std::endl;
  std::cout << "\tInputs :" << std::endl;
  std::cout << "\t\t -> Detector input file: \033[1;35m" 
    << InputFile << "\033[0m" << std::endl;
  std::cout << std::endl;

  // Display help message
  std::cout << "\033[1;31mFor help editing those files visit http://nptool.org/manual/adding-detectors \033[0m" << std::endl;

  // Adding file to git
  // TODO //
  //////////

  return 0;
}

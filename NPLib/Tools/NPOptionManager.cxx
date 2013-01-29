/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: A. MATTA  contact address: matta@ipno.in2p3.fr           *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class is a singleton class which deals with input        *
 * arguments of the different NPTool programm (NPS and NPA)                  *
 *---------------------------------------------------------------------------*
 * Comment: The singleton form allow users to call the object from anywhere  *
 * in the code                                                               *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "NPOptionManager.h"

#include <fstream>
#include <cstdlib>


NPOptionManager* NPOptionManager::instance = 0 ;

NPOptionManager* NPOptionManager::getInstance(int argc, char** argv)
{
  if (instance == 0) instance = new NPOptionManager(argc, argv);
  
  return instance ;
}



NPOptionManager::NPOptionManager(int argc, char** argv)
{
  // Default Setting
  fDefaultReactionFileName    = "defaultReaction.reaction";
  fDefaultDetectorFileName    = "defaultDetector.detector";
  fDefaultOutputFileName      = "myResult.root";
  fDefaultRunToReadFileName   = "defaultRunToTreat.txt";
  fDefaultCalibrationFileName = "defaultCalibration.txt";
  // Assigned values
  fReactionFileName           = fDefaultReactionFileName;
  fDetectorFileName           = fDefaultDetectorFileName;
  fOutputFileName             = fDefaultOutputFileName;
  fRunToReadFileName          = fDefaultRunToReadFileName;
  fCalibrationFileName        = fDefaultCalibrationFileName;
  fVerboseLevel               = 1;
  fDisableAllBranchOption = false;
  fInputPhysicalTreeOption = false;
  
  for (int i = 0; i < argc; i++) {
    string argument = argv[i];
    
    if (argument == "-H" || argument == "-h" || argument == "--help") DisplayHelp();
    
    else if (argument == "--event-generator" && argc >= i + 1)    fReactionFileName    = argv[i+1] ;
    
    else if (argument == "-E" && argc >= i + 1)                   fReactionFileName    = argv[i+1] ;
    
    else if (argument == "--detector" && argc >= i + 1)           fDetectorFileName    = argv[i+1] ;
    
    else if (argument == "-D" && argc >= i + 1)                   fDetectorFileName    = argv[i+1] ;
        
    else if (argument == "--output" && argc >= i + 1)             fOutputFileName      = argv[i+1] ;
    
    else if (argument == "-O" && argc >= i + 1)                   fOutputFileName      = argv[i+1] ;
    
    else if (argument == "--run" && argc >= i + 1)                fRunToReadFileName   = argv[i+1] ;
    
    else if (argument == "-R" && argc >= i + 1)                   fRunToReadFileName   = argv[i+1] ;
    
    else if (argument == "--cal" && argc >= i + 1)                fCalibrationFileName = argv[i+1] ;
    
    else if (argument == "-C" && argc >= i + 1)                   fCalibrationFileName = argv[i+1] ;
    
    else if (argument == "-V"  && argc >= i + 1)                  fVerboseLevel = atoi(argv[i+1]) ;
    
    else if (argument == "--verbose" && argc >= i + 1)            fVerboseLevel = atoi(argv[i+1]) ;
    
    else if (argument == "--disable-branch")                      fDisableAllBranchOption = true ;
    
    else if (argument == "--input-physical")                      fInputPhysicalTreeOption = true ;
    
    else if (argument == "-IP")                                   fInputPhysicalTreeOption = true ;
    
    //else ;
  }
  
  CheckArguments();
}



void NPOptionManager::CheckArguments()
{
  CheckEventGenerator();
  CheckDetectorConfiguration();
}



void NPOptionManager::CheckEventGenerator()
{
  bool checkFile = true;
  
  // NPTool path
  string GlobalPath = getenv("NPTOOL");
  string StandardPath = GlobalPath + "/Inputs/EventGenerator/" + fReactionFileName;
  
  // ifstream to configfile
  ifstream ConfigFile;
  
  // test if config file is in local path
  ConfigFile.open(fReactionFileName.c_str());
  if (!ConfigFile.is_open()) {
    ConfigFile.open(StandardPath.c_str());
    if (!ConfigFile.is_open()) {  // if not, assign standard path
      checkFile = false;
    }
    else {
      fReactionFileName = StandardPath;
    }
  }
  if (!checkFile && fReactionFileName != fDefaultReactionFileName) {   // if file does not exist
    SendErrorAndExit("EventGenerator");
  }
  
  // close ConfigFile
  ConfigFile.close();
}



void NPOptionManager::CheckDetectorConfiguration()
{
  bool checkFile = true;
  
  // NPTool path
  string GlobalPath = getenv("NPTOOL");
  string StandardPath = GlobalPath + "/Inputs/DetectorConfiguration/" + fDetectorFileName;
  
  // ifstream to configfile
  ifstream ConfigFile;
  
  // test if config file is in local path
  ConfigFile.open(fDetectorFileName.c_str());
  if (!ConfigFile.is_open()) {
    ConfigFile.open(StandardPath.c_str());
    if (!ConfigFile.is_open()) {  // if not, assign standard path
      checkFile = false;
    }
    else {
      fDetectorFileName = StandardPath;
    }
  }
  if (!checkFile && fDetectorFileName != fDefaultDetectorFileName) {   // if file does not exist
    SendErrorAndExit("DetectorConfiguration");
  }
  
  // close ConfigFile
  ConfigFile.close();
}



// This method tests if the input files are the default ones
bool NPOptionManager::IsDefault(const char* type) const
{
  bool result = false;
  
  string stype = type;
  if (stype == "EventGenerator") {
    if (fReactionFileName == fDefaultReactionFileName) result = true;
  }
  else if (stype == "DetectorConfiguration") {
    if (fDetectorFileName == fDefaultDetectorFileName) result = true;
  }
  else if (stype == "Calibration") {
    if (fCalibrationFileName == fDefaultCalibrationFileName) result = true;
  }
  else if (stype == "RunToTreat") {
    if (fRunToReadFileName == fDefaultRunToReadFileName) result = true;
  }
  else {
    cout << "NPOptionManager::IsDefault() unkwown keyword" << endl;
  }
  
  return result;
}



// This method tests if the input files are the default ones
void NPOptionManager::SendErrorAndExit(const char* type) const
{
  string stype = type;
  if (stype == "EventGenerator") {
    cout << endl;
    cout << "**********************************       Error       **********************************" << endl;
    cout << "* No event generator file found in $NPTool/Inputs/EventGenerator or local directories *" << endl;
    cout << "***************************************************************************************" << endl;
    cout << endl;
    exit(1);
  }
  else if (stype == "DetectorConfiguration") {
    cout << endl;
    cout << "***********************************       Error       ***********************************" << endl;
    cout << "* No detector geometry file found in $NPTool/Inputs/EventGenerator or local directories *" << endl;
    cout << "*****************************************************************************************" << endl;
    cout << endl;
    exit(1);
  }
  else if (stype == "Calibration") {
  }
  else if (stype == "RunToTreat") {
  }
  else {
    cout << "NPOptionManager::SendErrorAndExit() unkwown keyword" << endl;
  }
}



void NPOptionManager::DisplayHelp()
{
  cout << endl << "----NPOptionManager Help----" << endl << endl ;
  cout << "List of Option " << endl ;
  cout << "\t --help　-H -h\t \t \t \t \t \t \t　Display this help message" << endl ;
  cout << "\t --detector　-D <arg>\t \t \t \t \t \t　Set arg as the detector configuration file" << endl ;
  cout << "\t --event-generator　-E <arg>\t \t \t \t \t　Set arg as the event generator file" << endl ;
  cout << "\t --output　-O <arg>\t \t \t \t \t \t　Set arg as the Output File Name (output tree)" << endl ;
  cout << "\t --verbose -V <arg>\t \t \t \t \t \t \t　Set the verbose level of some of the object, 0 for nothing, 1 for normal printout. Error and warning are not affected" << endl ;
  cout << endl << "NPAnalysis only:"<<endl;
  cout << "\t --run -R <arg>\t \t \t \t \t \t \t　Set arg as the run to read file list" << endl  ;
  cout << "\t --cal -C <arg>\t \t \t \t \t \t \t　Set arg as the calibration file list" << endl ;
  cout << "\t --disable-branch\t \t \t \t \t \t　Disable of branch of Input tree except the one of the detector (faster)" << endl  ;
  cout << "\t --input-physical -IP\t \t \t \t \t \t　Consider the Input file is containing Physics Class instead of Data Class. Output branches associate to the detector are not activated" << endl  ;
  cout << endl << endl ;
  
  // exit current program
  exit(1);
}



void NPOptionManager::Destroy()
{
  if (instance != 0) {
    delete instance;
    instance = 0;
  }
}



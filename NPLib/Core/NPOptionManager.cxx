/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: A. MATTA  contact address: matta@lpccaen.in2p3.fr        *
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
#include "NPLibVersion.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>


NPOptionManager* NPOptionManager::instance = 0 ;
////////////////////////////////////////////////////////////////////////////////
NPOptionManager* NPOptionManager::getInstance(int argc, char** argv){
  if (instance == 0) instance = new NPOptionManager(argc, argv);

  return instance ;
}

////////////////////////////////////////////////////////////////////////////////
NPOptionManager* NPOptionManager::getInstance(string arg){

  if (instance == 0) instance = new NPOptionManager(arg);

  return instance ;

}
void NPOptionManager::ReadTheInputArgument(int argc, char** argv){
  // Default Setting
  fDefaultReactionFileName    = "defaultReaction.reaction";
  fDefaultDetectorFileName    = "defaultDetector.detector";
  fDefaultOutputFileName      = "myResult.root";
  fDefaultOutputTreeName      = "NPTool_Tree";
  fDefaultRunToReadFileName   = "defaultRunToTreat.txt";
  fDefaultCalibrationFileName = "defaultCalibration.txt";
  fDefaultG4MacroPath         = "defaultG4MacroPath.txt";
  // Assigned values
  fReactionFileName           = fDefaultReactionFileName;
  fDetectorFileName           = fDefaultDetectorFileName;
  fOutputFileName             = fDefaultOutputFileName;
  fOutputTreeName             = fDefaultOutputTreeName;
  fRunToReadFileName          = fDefaultRunToReadFileName;
  fCalibrationFileName        = fDefaultCalibrationFileName;
  fG4MacroPath                = fDefaultG4MacroPath;
  fLastSimFile = false; 
  fLastPhyFile = false;
  fLastResFile = false;
  fLastAnyFile = false;
  fVerboseLevel               = 1;
  fNumberOfEntryToAnalyse     = -1;
  fDisableAllBranchOption = false;
  fInputPhysicalTreeOption = false;
  fGenerateHistoOption = false ;
  fPROOFMode = false;
  fOnline = false;
  fG4BatchMode = false;
#ifdef __linux__
  fSharedLibExtension = ".so";
#endif

#ifdef __FreeBSD__
  fSharedLibExtension = ".so";
#endif

#ifdef __APPLE__
  fSharedLibExtension = ".dylib";
#endif




  for (int i = 0; i < argc; i++) {
    string argument = argv[i];
    if (argument == "-H" || argument == "-h" || argument == "--help") DisplayHelp();

    else if (argument == "--event-generator" && argc >= i + 1)    fReactionFileName    = argv[++i] ;

    else if (argument == "-E" && argc >= i + 1)                   fReactionFileName    = argv[++i] ;

    else if (argument == "--detector" && argc >= i + 1)           fDetectorFileName    = argv[++i] ;

    else if (argument == "-D" && argc >= i + 1)                   fDetectorFileName    = argv[++i] ;

    else if (argument == "--output" && argc >= i + 1)             fOutputFileName      = argv[++i] ;

    else if (argument == "-O" && argc >= i + 1)                   fOutputFileName      = argv[++i] ;

    else if (argument == "--tree-name" && argc >= i + 1)          fOutputTreeName      = argv[++i] ;

    else if (argument == "--run" && argc >= i + 1)                fRunToReadFileName   = argv[++i] ;

    else if (argument == "-R" && argc >= i + 1)                   fRunToReadFileName   = argv[++i] ;

    else if (argument == "--cal" && argc >= i + 1)                fCalibrationFileName = argv[++i] ;

    else if (argument == "-C" && argc >= i + 1)                   fCalibrationFileName = argv[++i] ;

    else if (argument == "-M" && argc >= i + 1)                   fG4MacroPath = argv[++i] ;

    else if (argument == "-B" && argc >= i + 1)                   {fG4MacroPath = argv[++i] ;fG4BatchMode=true;}

    else if (argument == "-V"  && argc >= i + 1)                  fVerboseLevel = atoi(argv[++i]) ;

    else if (argument == "--verbose" && argc >= i + 1)            fVerboseLevel = atoi(argv[++i]) ;

    else if (argument == "--disable-branch")                      fDisableAllBranchOption = true ;

    else if (argument == "--input-physical")                      fInputPhysicalTreeOption = true ;

    else if (argument == "-IP")                                   fInputPhysicalTreeOption = true ;

    else if (argument == "-GH")                                   fGenerateHistoOption = true ;

    else if (argument == "-CH")                                   fCheckHistoOption = true ;

    else if (argument == "-check-histo")                          fCheckHistoOption = true ;

    else if (argument == "--generate-histo")                      fGenerateHistoOption = true ;

    else if (argument == "--proof")                               fPROOFMode = true ;

    else if (argument == "-L")                                    fNumberOfEntryToAnalyse = atoi(argv[++i]) ;

    else if (argument == "--last-sim")                            fLastSimFile = true ;

    else if (argument == "--last-phy")                            fLastPhyFile = true ;

    else if (argument == "--last-res")                            fLastResFile = true ;

    else if (argument == "--last-any")                            fLastAnyFile = true ;

    else if (argument == "--online")                              {fOnline = true ;fGenerateHistoOption=true;}


    //else ;
  }
  CheckArguments();
  if(argc!=0)
    DisplayVersion();
}
////////////////////////////////////////////////////////////////////////////////
void NPOptionManager::DisplayVersion(){
  if(fVerboseLevel>0){
    string line;
    line.resize(80,'*');
    cout << line << endl;
    cout << "***********************************  NPTool  ***********************************"<< endl;
    cout << line << endl;
    cout << " NPLib version: nplib-"<< NPL::version_major <<"-" << NPL::version_minor << "-" << NPL::version_deta <<endl;
    cout << " Copyright: NPTool Collaboration "<<endl;
    cout << " GitHub: http://github.com/adrien-matta/nptool"<<endl; ;
    cout << line << endl;

  }
}
////////////////////////////////////////////////////////////////////////////////
NPOptionManager::NPOptionManager(int argc, char** argv){
  ReadTheInputArgument(argc,argv);
}
////////////////////////////////////////////////////////////////////////////////
NPOptionManager::NPOptionManager(string arg){  
  vector<char *> args;
  istringstream iss(arg);

  string token;
  while(iss >> token) {
    char *arg = new char[token.size() + 1];
    copy(token.begin(), token.end(), arg);
    arg[token.size()] = '\0';
    args.push_back(arg);
  }
  args.push_back(0);

  ReadTheInputArgument(args.size()-1, &args[0]);

  /*  for(size_t i = 0; i < args.size(); i++)
      delete[] args[i];
      */
}
////////////////////////////////////////////////////////////////////////////////
void NPOptionManager::CheckArguments(){
  CheckEventGenerator();
  CheckDetectorConfiguration();
  CheckG4Macro();
}

////////////////////////////////////////////////////////////////////////////////
void NPOptionManager::CheckEventGenerator(){
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

////////////////////////////////////////////////////////////////////////////////
void NPOptionManager::CheckDetectorConfiguration(){
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

////////////////////////////////////////////////////////////////////////////////
void NPOptionManager::CheckG4Macro(){
  if(IsDefault("G4MacroPath"))
    return ;

  // ifstream to configfile
  ifstream MacroFile( fG4MacroPath );

  if (!MacroFile.is_open()) {
    SendErrorAndExit("G4MacroPath");
  }
  
}



////////////////////////////////////////////////////////////////////////////////
// This method tests if the input files are the default ones
bool NPOptionManager::IsDefault(const char* type) const{
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
  else if (stype == "OutputFileName") {
    if (fOutputFileName == fDefaultOutputFileName) result = true;
  }
  else if (stype == "TreeName") {
    if (fOutputTreeName == fDefaultOutputTreeName) result = true;
  }
  else if (stype == "G4MacroPath") {
    if (fG4MacroPath== fDefaultG4MacroPath) result = true;
  }
 
  else {
    cout << "NPOptionManager::IsDefault() unkwown keyword" << endl;
  }

  return result;
}


////////////////////////////////////////////////////////////////////////////////
// This method tests if the input files are the default ones
void NPOptionManager::SendErrorAndExit(const char* type) const{
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
    cout << "* No detector geometry file found in $NPTool/Inputs/DetectorConfiguration or local directories *" << endl;
    cout << "*****************************************************************************************" << endl;
    cout << endl;
    exit(1);
  }
  else if (stype == "Calibration") {
  }
  else if (stype == "RunToTreat") {
  }
  else if (stype == "G4MacroPath") {
    cout << endl;
    cout << "***********************************       Error       ***********************************" << endl;
    cout << "*                  No Geant4 macro file found in the provided path                      *" << endl;
    cout << "*****************************************************************************************" << endl;
    cout << endl;
    exit(1);
  }

  else {
    cout << "NPOptionManager::SendErrorAndExit() unkwown keyword" << endl;
  }
}


////////////////////////////////////////////////////////////////////////////////
void NPOptionManager::DisplayHelp(){
  DisplayVersion();
  cout << endl << "----NPOptionManager Help----" << endl << endl ;
  cout << "List of Option " << endl ;
  cout << "\t--help　-H -h\t\t\tDisplay this help message" << endl ;
  cout << "\t--detector　-D <arg>\t\tSet arg as the detector configuration file" << endl ;
  cout << "\t--event-generator　-E <arg>\tSet arg as the event generator file" << endl ;
  cout << "\t--output　-O <arg>\t\tSet arg as the Output File Name (output tree)" << endl ;
  cout << "\t--tree-name <arg>\t\tSet arg as the Output Tree Name " << endl ;
  cout << "\t--verbose -V <arg>\t\tSet the verbose level, 0 for nothing, 1 for normal printout."<<endl;
 cout  << "\t\t\t\t\tError and warning are not affected" << endl ;
  cout << endl << "NPAnalysis only:"<<endl;
  cout << "\t--run -R <arg>\t\t\tSet arg as the run to read file list" << endl  ;
  cout << "\t--cal -C <arg>\t\t\tSet arg as the calibration file list" << endl ;
  cout << "\t--disable-branch\t\tDisable of branch of Input tree except the one of the detector (faster)" << endl  ;
  cout << "\t--generate-histo -GH\t\tInstantiate the T*Spectra class of each detector" << endl ;
  cout << "\t--check-histo -CH\t\tCheck if the Histogram looks ok and change there color if not" << endl ;
  cout << "\t--input-physical -IP\t\tConsider the Input file is containing Physics Class." << endl  ;
  cout << "\t-L <arg>\t\t\tLimit the number of envent to be analysed to arg" << endl ;
  cout << "\t--last-sim\t\t\tIgnore the list of Run to treat if any and analysed the last simulated file" << endl ;
  cout << "\t--last-phy\t\t\tIgnore the list of Run to treat if any and analysed the last Physics file" << endl ;
  cout << "\t--last-res\t\t\tIgnore the list of Run to treat if any and analysed the last Result file" << endl ;
  cout << "\t--last-any\t\t\tIgnore the list of Run to treat if any and analysed the last generated root file" << endl ;
  cout << "\t--online  \t\t\ttStart the spectra server" << endl ;
  cout << endl << "NPSimulation only:"<<endl;
  cout << "\t-M <arg>\t\t\tExecute Geant4 macro <arg> at startup" << endl ;
  cout << "\t-B <arg>\t\t\tExecute in batch mode (no ui) with Geant4 macro <arg> at startup" << endl ;

  cout << endl << endl ;

  // exit current program
  exit(0);
}


////////////////////////////////////////////////////////////////////////////////
void NPOptionManager::Destroy(){
  if (instance != 0) {
    delete instance;
    instance = 0;
  }
}
////////////////////////////////////////////////////////////////////////////////
string NPOptionManager::GetLastFile(){
  string path = getenv("NPTOOL");
  if(fLastSimFile)
    return (path+"/.last_sim_file");

  else if(fLastPhyFile)
    return (path+"/.last_phy_file");

  else if(fLastResFile)
    return (path+"/.last_res_file");

  else if(fLastAnyFile)
    return (path+"/.last_any_file");

  else
    return "VOID";

}

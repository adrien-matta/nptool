/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:              contact address:                            *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "NPDetectorManager.h"

//   STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

// NPL
#include "NPDetectorFactory.h"
#include "RootInput.h"
#include "NPOptionManager.h"
#include "NPCalibrationManager.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//   Default Constructor
DetectorManager::DetectorManager(){
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
//   Default Desstructor
DetectorManager::~DetectorManager(){
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//   Read stream at ConfigFile and pick-up Token declaration of Detector
void DetectorManager::ReadConfigurationFile(string Path)   {

   // Instantiate the Calibration Manager
  // All The detector will then add to it their parameter (see AddDetector)
  CalibrationManager::getInstance(NPOptionManager::getInstance()->GetCalibrationFile());


  // Access the DetectorFactory and ask it to load the Class List
  string classlist = getenv("NPTOOL");
  classlist += "/NPLib/DetectorClassList.txt";
  NPA::DetectorFactory* theFactory = NPA::DetectorFactory::getInstance();
  theFactory->ReadClassList(classlist);


  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());
  string LineBuffer;
  set<string> check;


  if (ConfigFile.is_open()) {
    cout << endl << "/////////// Detector geometry ///////////" << endl;
    cout << "Configuration file " << Path << " loading " << endl;
  }



  while (!ConfigFile.eof()) {
    // Pick-up next line
    getline(ConfigFile, LineBuffer);
    //Search for comment Symbol: %
    if (LineBuffer.compare(0, 1, "%") == 0) { ;}
    else{
      istringstream oss(LineBuffer);
      string token;
      oss >> token ;
      VDetector* detector = theFactory->Construct(token);
      if(detector!=NULL && check.find(token)==check.end()){
        cout << "/////////////////////////////////////////" << endl;
        cout << "//// Adding Detector " << token << endl; 
        detector->ReadConfiguration(Path);
        cout << "/////////////////////////////////////////" << endl;
        // Add array to the VDetector Vector
        AddDetector(token, detector);
        check.insert(token);
      }
      else if(detector!=NULL)
        delete detector;
    }
  } 
  
/*///////////////////////////////////////////////////////////////////////////////
//                           END OF DETECTOR                                 //
///////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////
    //////////// Search for Target /////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 13, "GeneralTarget") == 0 && GeneralTarget == false) {
      GeneralTarget = true ;
      cout << "////////// Target ///////////" << endl;

      // jump one line
      getline(ConfigFile, LineBuffer);
      getline(ConfigFile, LineBuffer);

      bool check_Thickness = false;
      //         bool check_Angle     = false;
      bool check_Radius    = false;
      bool check_Material  = false;
      bool check_X         = false;
      bool check_Y         = false;
      bool check_Z         = false;

      bool ReadingStatusTarget = true;
      while (ReadingStatusTarget) {
        ConfigFile >> DataBuffer;

        // Search for comment Symbol %
        if (DataBuffer.compare(0, 1, "%") == 0) {ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );getline(ConfigFile, LineBuffer);}

        else if (DataBuffer.compare(0, 10, "THICKNESS=") == 0) {
          check_Thickness = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetThickness = atof(DataBuffer.c_str()) * micrometer;
          m_TargetThickness = atof(DataBuffer.c_str());
          cout << "Target Thickness: " << m_TargetThickness << endl;
        }

        else if (DataBuffer.compare(0, 6, "ANGLE=") == 0) {
          //               check_Angle = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetAngle = atof(DataBuffer.c_str()) * deg;
          m_TargetAngle = atof(DataBuffer.c_str());
          cout << "Target Angle: " << m_TargetAngle << endl;
        }

        else if (DataBuffer.compare(0, 7, "RADIUS=") == 0) {
          check_Radius = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetRadius = atof(DataBuffer.c_str()) * mm;
          m_TargetRadius = atof(DataBuffer.c_str());
          cout << "Target Radius: " <<  m_TargetRadius << endl;
        }

        else if (DataBuffer.compare(0, 9, "MATERIAL=") == 0) {
          check_Material = true ;
          ConfigFile >> DataBuffer;
          m_TargetMaterial = DataBuffer;
          cout << "Target Material: " << m_TargetMaterial << endl;
        }

        else if (DataBuffer.compare(0, 2, "X=") == 0) {
          check_X = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetX = atoi(DataBuffer.c_str()) * mm;
          m_TargetX = atoi(DataBuffer.c_str());
          cout << "Target Coordinates (mm): ( " << m_TargetX << " ; ";
        }

        else if (DataBuffer.compare(0, 2, "Y=") == 0) {
          check_Y = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetY = atoi(DataBuffer.c_str()) * mm;
          m_TargetY = atoi(DataBuffer.c_str());
          cout << m_TargetY << " ; ";
        }

        else if (DataBuffer.compare(0, 2, "Z=") == 0) {
          check_Z = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetZ = atoi(DataBuffer.c_str()) * mm;
          m_TargetZ = atoi(DataBuffer.c_str());
          cout  << m_TargetZ << " )" << endl;
        }

        ///////////////////////////////////////////////////
        // If no Target Token and no comments, toggle out
        else {
          ReadingStatusTarget = false; 
          cout << "WARNING : Wrong Token Sequence: Getting out " << endl;
        }

        ///////////////////////////////////////////////////
        // If all Token found toggle out
        if (check_Thickness && check_Radius && check_Material && check_X && check_Y && check_Z)
          ReadingStatusTarget = false;

      }
    }

    //Nothing understandable
    //else ;
  }
*/
  ConfigFile.close();

  // Now That the detector lib are loaded, we can instantiate the root input
  string runToReadfileName = NPOptionManager::getInstance()->GetRunToReadFile();
  RootInput::getInstance(runToReadfileName);

  // Now that the detector are all added, they can initialise their Branch to the Root I/O
  InitializeRootInput();
  InitializeRootOutput();

  // If Requiered, they can also instiantiate their control histogramm
  if(NPOptionManager::getInstance()->GetGenerateHistoOption())
    InitSpectra();

  // The calibration Manager got all the parameter added, so it can load them from the calibration file
  CalibrationManager::getInstance()->LoadParameterFromFile();

  return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void DetectorManager::BuildPhysicalEvent(){
  ClearEventPhysics();
  map<string,VDetector*>::iterator it;
   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    it->second->BuildPhysicalEvent();
    if(NPOptionManager::getInstance()->GetGenerateHistoOption()){
      it->second->FillSpectra();
      if(NPOptionManager::getInstance()->GetCheckHistoOption())
        it->second->CheckSpectra();
    }  
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::BuildSimplePhysicalEvent(){
  ClearEventPhysics();
  map<string,VDetector*>::iterator it;
  
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    it->second->BuildSimplePhysicalEvent();
    if(NPOptionManager::getInstance()->GetGenerateHistoOption()){
        it->second->FillSpectra();
      if(NPOptionManager::getInstance()->GetCheckHistoOption())
        it->second->CheckSpectra();
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::InitializeRootInput(){

  if( NPOptionManager::getInstance()->GetDisableAllBranchOption() )
    RootInput::getInstance()->GetChain()->SetBranchStatus ( "*" , false ) ;

  map<string,VDetector*>::iterator it;

  if(NPOptionManager::getInstance()->GetInputPhysicalTreeOption())
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootInputPhysics();

  else // Default Case
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootInputRaw();
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void DetectorManager::InitializeRootOutput(){
  map<string,VDetector*>::iterator it;

  if(!NPOptionManager::getInstance()->GetInputPhysicalTreeOption())
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootOutput();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::AddDetector(string DetectorName , VDetector* newDetector){
  m_Detector[DetectorName] = newDetector;
  newDetector->AddParameterToCalibrationManager();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
VDetector* DetectorManager::GetDetector(string name){
  map<string,VDetector*>::iterator it;
  it = m_Detector.find(name);
  if ( it!=m_Detector.end() ) return it->second;
  else{
    cout << endl;
    cout << "**********************************       Error       **********************************" << endl;
    cout << " No Detector " << name << " found in the Detector Manager" << endl;
    cout << "***************************************************************************************" << endl;
    cout << endl;
    exit(1);
  }

}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::ClearEventPhysics(){
  map<string,VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->ClearEventPhysics();
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::ClearEventData(){
  map<string,VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it)
    it->second->ClearEventData();
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void DetectorManager::InitSpectra(){
  map<string,VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->InitSpectra();
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void DetectorManager::WriteSpectra(){
  map<string,VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->WriteSpectra();
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
vector< map< vector<string>, TH1* > > DetectorManager::GetSpectra()
{
   vector< map< vector<string>, TH1* > > myVector;

   map<string,VDetector*>::iterator it;
   // loop on detectors
   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
      myVector.push_back(it->second->GetSpectra());
   }

   return myVector;
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
vector<string> DetectorManager::GetDetectorList(){
  map<string,VDetector*>::iterator it;
  vector<string> DetectorList;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) { 
    DetectorList.push_back(it->first);
  }

  return DetectorList;
}


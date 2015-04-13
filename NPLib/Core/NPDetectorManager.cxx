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
#include <set>
// NPL
#include "NPDetectorFactory.h"
#include "RootInput.h"
#include "NPOptionManager.h"
#include "NPCalibrationManager.h"


/////////////////////////////////////////////////////////////////////////////////////////////////
//   Default Constructor
NPA::DetectorManager::DetectorManager(){
  m_BuildPhysicalPtr = &NPA::VDetector::BuildPhysicalEvent;
  m_ClearEventPhysicsPtr =  &NPA::VDetector::ClearEventPhysics;
  m_ClearEventDataPtr = &NPA::VDetector::ClearEventData ;
  m_FillSpectra = NULL; 
  m_CheckSpectra = NULL;   
  
  if(NPOptionManager::getInstance()->GetGenerateHistoOption()){
    m_FillSpectra =  &NPA::VDetector::FillSpectra ;
    if(NPOptionManager::getInstance()->GetCheckHistoOption())
      m_CheckSpectra = &NPA::VDetector::CheckSpectra ;
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////   
//   Default Desstructor
NPA::DetectorManager::~DetectorManager(){
#if __cplusplus > 199711L
  StopThread();
#endif

}

/////////////////////////////////////////////////////////////////////////////////////////////////
//   Read stream at ConfigFile and pick-up Token declaration of Detector
void NPA::DetectorManager::ReadConfigurationFile(string Path)   {
  cout << "\033[1;36m" ;

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
 cout << "\033[0m" ;

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
  
  // Start the thread if multithreading supported
  #if __cplusplus > 199711L
  InitThreadPool();
  #endif
  
  return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void NPA::DetectorManager::BuildPhysicalEvent(){
#if __cplusplus > 199711L
 // add new job
  map<string,VDetector*>::iterator it;
  unsigned int i = 0;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    m_Ready[i++]=true;
  }
  // Wait for all job to be done
  while(!IsDone()){};
#else 
  map<string,VDetector*>::iterator it;
   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
      (it->second->*m_ClearEventPhysicsPtr)();
      (it->second->*m_BuildPhysicalPtr)();
      if(m_FillSpectra){
        (it->second->*m_FillSpectra)();
        if(m_CheckSpectra)
          (it->second->*m_CheckSpectra)();
      }
  }
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void NPA::DetectorManager::BuildSimplePhysicalEvent(){
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
void NPA::DetectorManager::InitializeRootInput(){

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
void NPA::DetectorManager::InitializeRootOutput(){
  map<string,VDetector*>::iterator it;

  if(!NPOptionManager::getInstance()->GetInputPhysicalTreeOption())
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootOutput();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void NPA::DetectorManager::AddDetector(string DetectorName , VDetector* newDetector){
  m_Detector[DetectorName] = newDetector;
  newDetector->AddParameterToCalibrationManager();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
NPA::VDetector* NPA::DetectorManager::GetDetector(string name){
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
void NPA::DetectorManager::ClearEventPhysics(){
  map<string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    (it->second->*m_ClearEventPhysicsPtr)();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void NPA::DetectorManager::ClearEventData(){
  map<string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it)
    (it->second->*m_ClearEventDataPtr)();
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void NPA::DetectorManager::InitSpectra(){
  map<string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->InitSpectra();
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void NPA::DetectorManager::WriteSpectra(){
  map<string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->WriteSpectra();
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
vector< map< vector<string>, TH1* > > NPA::DetectorManager::GetSpectra(){
   vector< map< vector<string>, TH1* > > myVector;
   map<string,VDetector*>::iterator it;
   // loop on detectors
   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
      myVector.push_back(it->second->GetSpectra());
   }

   return myVector;
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
vector<string> NPA::DetectorManager::GetDetectorList(){
  map<string,VDetector*>::iterator it;
  vector<string> DetectorList;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) { 
    DetectorList.push_back(it->first);
  }

  return DetectorList;
}
#if __cplusplus > 199711L 
////////////////////////////////////////////////////////////////////////////////
void NPA::DetectorManager::InitThreadPool(){
  StopThread();
  m_ThreadPool.clear();
  m_Ready.clear();
  map<string,VDetector*>::iterator it;
  unsigned int i = 0;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) { 
    m_ThreadPool.push_back( thread( &NPA::DetectorManager::StartThread,this,it->second,i++) );
    m_Ready.push_back(false);
  }
  m_stop = false;
  for(auto& th: m_ThreadPool){
    th.detach();
  }

  cout << "\033[1;33m**** Detector Manager : Started " << i << " Threads ****\033[0m" << endl ;
}

////////////////////////////////////////////////////////////////////////////////
void NPA::DetectorManager::StartThread(NPA::VDetector* det,unsigned int id){ 
this_thread::sleep_for(chrono::milliseconds(100));
  while(!m_stop){
    if(m_Ready[id]){
      (det->*m_ClearEventPhysicsPtr)();
      (det->*m_BuildPhysicalPtr)();
      if(m_FillSpectra){
        (det->*m_FillSpectra)();
        if(m_CheckSpectra)
          (det->*m_CheckSpectra)();
      }
      m_Ready[id]=false;
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
void NPA::DetectorManager::StopThread(){
  m_stop=true;
}
////////////////////////////////////////////////////////////////////////////////
bool NPA::DetectorManager::IsDone(){
  for(vector<bool>::iterator i =  m_Ready.begin() ; i!=m_Ready.end() ; i++){
    if((*i))
      return false;
  }
  return true;
}
#endif


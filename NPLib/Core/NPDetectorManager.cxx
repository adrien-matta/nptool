/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address: matta@lpccaen.in2p3.fr   *
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
#include <limits>
#include <set>

#if __cplusplus > 199711L 
#include <chrono>
#endif

// NPL
#include "NPDetectorFactory.h"
#include "RootInput.h"
#include "NPOptionManager.h"
#include "NPCalibrationManager.h"
#include "NPInputParser.h"
#include "NPSystemOfUnits.h"
using namespace NPUNITS;
//Root
#include"TCanvas.h"
/////////////////////////////////////////////////////////////////////////////////////////////////
//   Default Constructor
NPL::DetectorManager::DetectorManager(){
  m_BuildPhysicalPtr = &NPL::VDetector::BuildPhysicalEvent;
  m_ClearEventPhysicsPtr =  &NPL::VDetector::ClearEventPhysics;
  m_ClearEventDataPtr = &NPL::VDetector::ClearEventData ;
  m_FillSpectra = NULL; 
  m_CheckSpectra = NULL;   
  m_SpectraServer = NULL;
  if(NPOptionManager::getInstance()->GetGenerateHistoOption()){
    m_FillSpectra =  &NPL::VDetector::FillSpectra ;
    if(NPOptionManager::getInstance()->GetCheckHistoOption())
      m_CheckSpectra = &NPL::VDetector::CheckSpectra ;
  }
  m_WindowsThickness=0;
  m_WindowsMaterial="";
}


/////////////////////////////////////////////////////////////////////////////////////////////////   
//   Default Desstructor
NPL::DetectorManager::~DetectorManager(){
#if __cplusplus > 199711L
  StopThread();
#endif
  if(m_SpectraServer)
    m_SpectraServer->Destroy();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//   Read stream at ConfigFile and pick-up Token declaration of Detector
void NPL::DetectorManager::ReadConfigurationFile(string Path)   {
  cout << "\033[1;36m" ;

  // Instantiate the Calibration Manager
  // All The detector will then add to it their parameter (see AddDetector)
  CalibrationManager::getInstance(NPOptionManager::getInstance()->GetCalibrationFile());

  // Access the DetectorFactory and ask it to load the Class List
  string classlist = getenv("NPTOOL");
  classlist += "/NPLib/ClassList.txt";
  NPL::DetectorFactory* theFactory = NPL::DetectorFactory::getInstance();
  theFactory->ReadClassList(classlist);

  set<string> check;
  NPL::InputParser parser(Path);

  ////////////////////////////////////////////
  //////////// Search for Target /////////////
  ////////////////////////////////////////////
  vector<NPL::InputBlock*>  starget = parser.GetAllBlocksWithToken("Target");
  vector<NPL::InputBlock*>  ctarget = parser.GetAllBlocksWithToken("CryoTarget");

  if(starget.size()==1){
    if(NPOptionManager::getInstance()->GetVerboseLevel()){
    cout << "////       TARGET      ////" << endl;
    cout << "//// Solid Target found " << endl;
    }
    vector<string> token = {"Thickness","Radius","Material","Angle","X","Y","Z"};
    if(starget[0]->HasTokenList(token)){
      m_TargetThickness= starget[0]->GetDouble("Thickness","micrometer");
      m_TargetAngle=starget[0]->GetDouble("Angle","deg");
      m_TargetMaterial=starget[0]->GetString("Material");
      m_TargetX=starget[0]->GetDouble("X","mm");
      m_TargetY=starget[0]->GetDouble("Y","mm");
      m_TargetZ=starget[0]->GetDouble("Z","mm");
    }
    else{
      cout << "ERROR: Target token list incomplete, check your input file" << endl;
      exit(1);
    }
  }
  else if(ctarget.size()==1){
    if(NPOptionManager::getInstance()->GetVerboseLevel())
      cout << "//// Cryogenic Target found " << endl;
    
    vector<string> token = {"Thickness","Radius","Material","Density","WindowsThickness","WindowsMaterial","Angle","X","Y","Z"};
    if(ctarget[0]->HasTokenList(token)){
      m_TargetThickness= ctarget[0]->GetDouble("Thickness","micrometer");
      m_TargetAngle=ctarget[0]->GetDouble("Angle","deg");
      m_TargetMaterial=ctarget[0]->GetString("Material");
      m_WindowsThickness=ctarget[0]->GetDouble("WindowsThickness","micrometer");
      m_WindowsMaterial=ctarget[0]->GetString("WindowsMaterial");
      m_TargetX=ctarget[0]->GetDouble("X","mm");
      m_TargetY=ctarget[0]->GetDouble("Y","mm");
      m_TargetZ =ctarget[0]->GetDouble("Z","mm");
    }
    else{
      cout << "ERROR: Target token list incomplete, check your input file" << endl;
      exit(1);
    }
  }
  else{
    cout << "ERROR: One and only one target shall be declared in your detector file" << endl;
  }

  ////////////////////////////////////////////
  /////////// Search for Detectors ///////////
  ////////////////////////////////////////////
  // Get the list of main token
  std::vector<std::string> token = parser.GetAllBlocksToken();
  // Look for detectors among them
  for(unsigned int i = 0 ; i < token.size() ; i++){
  VDetector* detector = theFactory->Construct(token[i]);
  if(detector!=NULL && check.find(token[i])==check.end()){
    if(NPOptionManager::getInstance()->GetVerboseLevel()){
      cout << "/////////////////////////////////////////" << endl;
      cout << "//// Adding Detector " << token[i] << endl; 
    }
    detector->ReadConfiguration(parser);
    if(NPOptionManager::getInstance()->GetVerboseLevel())
      cout << "/////////////////////////////////////////" << endl;
    
    // Add array to the VDetector Vector
    AddDetector(token[i], detector);
    check.insert(token[i]);
  }
  else if(detector!=NULL)
    delete detector;
  }
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
void NPL::DetectorManager::BuildPhysicalEvent(){
#if __cplusplus > 199711L
  // add new job
  map<string,VDetector*>::iterator it;
  unsigned int i = 0;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    m_Ready[i++]=true;
  }

  { // aquire the sub thread lock
    std::unique_lock<std::mutex> lk(m_ThreadMtx);
  }
  m_CV.notify_all();

  while(!IsDone()){
    // this_thread::yield();
  }

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
void NPL::DetectorManager::BuildSimplePhysicalEvent(){
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
void NPL::DetectorManager::InitializeRootInput(){

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
void NPL::DetectorManager::InitializeRootOutput(){
  map<string,VDetector*>::iterator it;

  if(!NPOptionManager::getInstance()->GetInputPhysicalTreeOption())
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootOutput();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::AddDetector(string DetectorName , VDetector* newDetector){
  m_Detector[DetectorName] = newDetector;
  newDetector->AddParameterToCalibrationManager();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* NPL::DetectorManager::GetDetector(string name){
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
void NPL::DetectorManager::ClearEventPhysics(){
  map<string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    (it->second->*m_ClearEventPhysicsPtr)();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::ClearEventData(){
  map<string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it)
    (it->second->*m_ClearEventDataPtr)();
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void NPL::DetectorManager::InitSpectra(){
  map<string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->InitSpectra();
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void NPL::DetectorManager::WriteSpectra(){
  std::cout << endl << "\r \033[1;36m *** Writing Spectra: this may take a while ***\033[0m"<<flush;
  map<string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->WriteSpectra();
  std::cout << "\r                                                  " << flush; 
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
vector< map< string, TH1* > > NPL::DetectorManager::GetSpectra(){
  vector< map< string, TH1* > > myVector;
  map<string,VDetector*>::iterator it;
  // loop on detectors
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    myVector.push_back(it->second->GetSpectra());
  }

  return myVector;
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
vector<string> NPL::DetectorManager::GetDetectorList(){
  map<string,VDetector*>::iterator it;
  vector<string> DetectorList;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) { 
    DetectorList.push_back(it->first);
  }

  return DetectorList;
}
#if __cplusplus > 199711L 
////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::InitThreadPool(){
  StopThread();
  m_ThreadPool.clear();
  m_Ready.clear();
  map<string,VDetector*>::iterator it;

  unsigned int i = 0;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) { 
    m_ThreadPool.push_back( thread( &NPL::DetectorManager::StartThread,this,it->second,i++) );
    m_Ready.push_back(false);
  }

  m_stop = false;
  for(auto& th: m_ThreadPool){
    th.detach();
  }

  cout << "\033[1;33m**** Detector Manager : Started " << i << " Threads ****\033[0m" << endl ;
}

////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::StartThread(NPL::VDetector* det,unsigned int id){ 
  this_thread::sleep_for(chrono::milliseconds(1));
  vector<bool>::iterator it = m_Ready.begin()+id;
  while(true){
    { // Aquire the lock
      std::unique_lock<std::mutex> lk(m_ThreadMtx);    
      // wait for work to be given
      while(!m_Ready[id]){
        m_CV.wait(lk);
      }

      // Do the job
      (det->*m_ClearEventPhysicsPtr)();
      (det->*m_BuildPhysicalPtr)();
      if(m_FillSpectra){
        (det->*m_FillSpectra)();
        if(m_CheckSpectra)
          (det->*m_CheckSpectra)();
      }
      // Reset Ready flag
      m_Ready[id]=false;

      // Quite if stopped
      if(m_stop)
        return;

    } // Realease the lock

  }   
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::StopThread(){
  // make sure the last thread are schedule before stopping;
  this_thread::yield();
  m_stop=true;
  m_CV.notify_all();
}
////////////////////////////////////////////////////////////////////////////////
bool NPL::DetectorManager::IsDone(){
  for(vector<bool>::iterator it =  m_Ready.begin() ; it!=m_Ready.end() ; it++){
    if((*it))
      return false;
  }
  return true;
}
#endif
////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::SetSpectraServer(){
  m_SpectraServer = NPL::SpectraServer::getInstance();

  map<string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it){ 
    vector<TCanvas*> canvas = it->second->GetCanvas();
    size_t mysize = canvas.size();
    for (size_t i = 0 ; i < mysize ; i++) 
      m_SpectraServer->AddCanvas(canvas[i]);
  }

  // Avoid warning on gcc
  int r;
  r=system("nponline localhost 9090 & ");
  m_SpectraServer->CheckRequest(); 
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::StopSpectraServer(){
  m_SpectraServer->Destroy();
}

////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::CheckSpectraServer(){
  m_SpectraServer->CheckRequest();
}


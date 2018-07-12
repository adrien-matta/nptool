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
#include "TROOT.h"
///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
//   Default Desstructor
NPL::DetectorManager::~DetectorManager(){
#if __cplusplus > 199711L
  StopThread();
#endif
  if(m_SpectraServer)
    m_SpectraServer->Destroy();
}

///////////////////////////////////////////////////////////////////////////////
//   Read stream at ConfigFile and pick-up Token declaration of Detector
void NPL::DetectorManager::ReadConfigurationFile(std::string Path)   {
  std::cout << "\033[1;36m" ;

  // Instantiate the Calibration Manager
  // All The detector will then add to it their parameter (see AddDetector)
  CalibrationManager::getInstance(NPOptionManager::getInstance()->GetCalibrationFile());

  // Access the DetectorFactory and ask it to load the Class List
  std::string classlist = getenv("NPTOOL");
  classlist += "/NPLib/ClassList.txt";
  NPL::DetectorFactory* theFactory = NPL::DetectorFactory::getInstance();
  theFactory->ReadClassList(classlist);

  std::set<std::string> check;
  NPL::InputParser parser(Path);

  ////////////////////////////////////////////
  //////////// Search for Target /////////////
  ////////////////////////////////////////////
  std::vector<NPL::InputBlock*>  starget = parser.GetAllBlocksWithToken("Target");
  std::vector<NPL::InputBlock*>  ctarget = parser.GetAllBlocksWithToken("CryoTarget");

  if(starget.size()==1){
    if(NPOptionManager::getInstance()->GetVerboseLevel()){
    std::cout << "////       TARGET      ////" << std::endl;
    std::cout << "//// Solid Target found " << std::endl;
    }
    std::vector<std::string> token = {"Thickness","Radius","Material","Angle","X","Y","Z"};
    if(starget[0]->HasTokenList(token)){
      m_TargetThickness= starget[0]->GetDouble("Thickness","micrometer");
      m_TargetAngle=starget[0]->GetDouble("Angle","deg");
      m_TargetMaterial=starget[0]->GetString("Material");
      m_TargetX=starget[0]->GetDouble("X","mm");
      m_TargetY=starget[0]->GetDouble("Y","mm");
      m_TargetZ=starget[0]->GetDouble("Z","mm");
    }
    else{
      std::cout << "ERROR: Target token list incomplete, check your input file" << std::endl;
      exit(1);
    }
  }
  else if(ctarget.size()==1){
    if(NPOptionManager::getInstance()->GetVerboseLevel())
      std::cout << "//// Cryogenic Target found " << std::endl;
    
    std::vector<std::string> token = {"Thickness","Radius","Material","Density","WindowsThickness","WindowsMaterial","Angle","X","Y","Z"};
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
      std::cout << "ERROR: Target token list incomplete, check your input file" << std::endl;
      exit(1);
    }
  }
  else{
    std::cout << "ERROR: One and only one target shall be declared in your detector file" << std::endl;
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
      std::cout << "/////////////////////////////////////////" << std::endl;
      std::cout << "//// Adding Detector " << token[i] << std::endl; 
    }
    detector->ReadConfiguration(parser);
    if(NPOptionManager::getInstance()->GetVerboseLevel())
      std::cout << "/////////////////////////////////////////" << std::endl;
    
    // Add array to the VDetector Vector
    AddDetector(token[i], detector);
    check.insert(token[i]);
  }
  else if(detector!=NULL)
    delete detector;
  }
  // Now That the detector lib are loaded, we can instantiate the root input
  std::string runToReadfileName = NPOptionManager::getInstance()->GetRunToReadFile();
  RootInput::getInstance(runToReadfileName);

  // Now that the detector are all added, they can initialise their Branch to the Root I/O
  //InitializeRootInput();
  //InitializeRootOutput();

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

///////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::BuildPhysicalEvent(){
#if __cplusplus > 199711L
  // add new job
//std::cout << "TEST0a" << std::endl;
  std::map<std::string,VDetector*>::iterator it;
  unsigned int i = 0;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
//std::cout << "TEST0" << std::endl;
    m_Ready[i++]=true;
  }
//std::cout << "TEST1" << std::endl;
  { // aquire the sub thread lock
    std::unique_lock<std::mutex> lk(m_ThreadMtx);
  }
  m_CV.notify_all();

//std::cout << "TEST2" << std::endl;
  while(!IsDone()){
//std::cout << "TEST2a" << std::endl;
     //std::this_thread::yield();
  }
//std::cout << "TEST2b" << std::endl;

#else 
//std::cout << "TEST3" << std::endl;
  std::map<std::string,VDetector*>::iterator it;
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

///////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::BuildSimplePhysicalEvent(){
  ClearEventPhysics();
  std::map<std::string,VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    it->second->BuildSimplePhysicalEvent();
    if(NPOptionManager::getInstance()->GetGenerateHistoOption()){
      it->second->FillSpectra();
      if(NPOptionManager::getInstance()->GetCheckHistoOption())
        it->second->CheckSpectra();
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::InitializeRootInput(){

  if( NPOptionManager::getInstance()->GetDisableAllBranchOption() )
    RootInput::getInstance()->GetChain()->SetBranchStatus ( "*" , false ) ;

  std::map<std::string,VDetector*>::iterator it;

  if(NPOptionManager::getInstance()->GetInputPhysicalTreeOption())
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootInputPhysics();

  else // Default Case
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootInputRaw();
}

///////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::InitializeRootOutput(){
  std::map<std::string,VDetector*>::iterator it;

  if(!NPOptionManager::getInstance()->GetInputPhysicalTreeOption())
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootOutput();
}

///////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::AddDetector(std::string DetectorName , VDetector* newDetector){
  m_Detector[DetectorName] = newDetector;
  newDetector->AddParameterToCalibrationManager();
}

///////////////////////////////////////////////////////////////////////////////
NPL::VDetector* NPL::DetectorManager::GetDetector(std::string name){
  std::map<std::string,VDetector*>::iterator it;
  it = m_Detector.find(name);
  if ( it!=m_Detector.end() ) return it->second;
  else{
    std::cout << std::endl;
    std::cout << "**********************************       Error       **********************************" << std::endl;
    std::cout << " No Detector " << name << " found in the Detector Manager" << std::endl;
		std::cout << " Available Detectors: " << std::endl;
		for(std::map<std::string,VDetector*>::iterator i = m_Detector.begin(); i != m_Detector.end(); ++i) {
			std::cout << "\t" << i->first << std::endl;
		}
    std::cout << "***************************************************************************************" << std::endl;
    std::cout << std::endl;
    exit(1);
  }

}

///////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::ClearEventPhysics(){
  std::map<std::string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    (it->second->*m_ClearEventPhysicsPtr)();
}

///////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::ClearEventData(){
  std::map<std::string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it)
    (it->second->*m_ClearEventDataPtr)();
}

///////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::InitSpectra(){
  bool batch = false;
  if(gROOT){
     batch = gROOT->IsBatch();
     gROOT->ProcessLine("gROOT->SetBatch()");
  }
  std::map<std::string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->InitSpectra();

  if(gROOT&&!batch)
   gROOT->ProcessLine("gROOT->SetBatch(kFALSE)");
}

///////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::WriteSpectra(){
  std::cout << std::endl << "\r \033[1;36m *** Writing Spectra: this may take a while ***\033[0m"<<std::flush;
  std::map<std::string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->WriteSpectra();
  std::cout << "\r                                                  " << std::flush; 
}

///////////////////////////////////////////////////////////////////////////////
std::vector< std::map< std::string, TH1* > > NPL::DetectorManager::GetSpectra(){
  std::vector< std::map< std::string, TH1* > > myVector;
  std::map<std::string,VDetector*>::iterator it;
  // loop on detectors
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    myVector.push_back(it->second->GetSpectra());
  }

  return myVector;
}

///////////////////////////////////////////////////////////////////////////////
std::vector<std::string> NPL::DetectorManager::GetDetectorList(){
  std::map<std::string,VDetector*>::iterator it;
  std::vector<std::string> DetectorList;
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
  std::map<std::string,VDetector*>::iterator it;

  unsigned int i = 0;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) { 
    m_ThreadPool.push_back( std::thread( &NPL::DetectorManager::StartThread,this,it->second,i++) );
    m_Ready.push_back(false);
  }

  m_stop = false;
  for(auto& th: m_ThreadPool){
    th.detach();
  }

  std::cout << "\033[1;33m**** Detector Manager : Started " << i << " Threads ****\033[0m" << std::endl ;
}

////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::StartThread(NPL::VDetector* det,unsigned int id){ 
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  std::vector<bool>::iterator it = m_Ready.begin()+id;
  while(true){
    { // Aquire the lock
////std::cout << "WWWW" << std::endl;
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
  std::this_thread::yield();
  m_stop=true;
  m_CV.notify_all();
}
////////////////////////////////////////////////////////////////////////////////
bool NPL::DetectorManager::IsDone(){
int ijk=0;
//std::cout << m_Ready.size() << " !" << std::endl;
  for(std::vector<bool>::iterator it =  m_Ready.begin() ; it!=m_Ready.end() ; it++){
    if((*it))
{
ijk++;
//std::cout << *it << std::endl;
//std::cout << ijk << std::endl;
      return false;
}
  }
  return true;
}
#endif
////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::SetSpectraServer(){
  m_SpectraServer = NPL::SpectraServer::getInstance();

  std::map<std::string,VDetector*>::iterator it;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it){ 
    std::vector<TCanvas*> canvas = it->second->GetCanvas();
    size_t mysize = canvas.size();
    for (size_t i = 0 ; i < mysize ; i++){} 
      //m_SpectraServer->AddCanvas(canvas[i]);
  }

  // Avoid warning on gcc
  int r;
  r=system("nponline localhost 9092 & ");
  m_SpectraServer->CheckRequest(); 
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::StopSpectraServer(){
  if(m_SpectraServer)
    m_SpectraServer->Destroy();
  else
    std::cout <<"WARNING: requesting to stop spectra server, which is not started" << std::endl; 
  
}

////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::CheckSpectraServer(){
  if(m_SpectraServer)
    m_SpectraServer->CheckRequest();
  else
    std::cout <<"WARNING: requesting to check spectra server, which is not started" << std::endl; 

}


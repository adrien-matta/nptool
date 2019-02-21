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
//double NPL::DetectorManager::GetTargetThickness    () {return  m_TargetThickness;}

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
  m_CryoTarget=false;
  m_TargetThickness    = 0   ;
  m_TargetAngle        = 0   ;
  m_TargetRadius       = 0   ;
  m_TargetDensity      = 0   ;
  m_TargetDensity = 0 ;
  m_FrontDeformation = 0 ;
  m_FrontThickness = 0 ;
  m_FrontRadius = 0 ;
  m_FrontMaterial = "" ;
  m_BackDeformation = 0 ;
  m_BackRadius = 0 ;
  m_BackThickness = 0 ;
  m_BackMaterial = "" ;
  m_FrameRadius = 0 ;
  m_FrameThickness = 0;
  m_FrontCone = 0 ;
  m_BackCone = 0 ;
  m_FrameMaterial = "" ;
  m_ShieldInnerRadius = 0 ;
  m_ShieldOuterRadius = 0 ;
  m_ShieldBottomLength = 0 ;
  m_ShieldTopLength = 0 ;
  m_ShieldFrontRadius = 0 ; 
  m_ShieldBackRadius = 0 ;
  m_ShieldMaterial = "" ;
}


///////////////////////////////////////////////////////////////////////////////
//   Default Desstructor
NPL::DetectorManager::~DetectorManager(){
#if __cplusplus > 199711L && NPMULTITHREADING
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
  std::vector<NPL::InputBlock*>  ctarget = parser.GetAllBlocksWithToken("CryogenicTarget");

  if(starget.size()==1){
    if(NPOptionManager::getInstance()->GetVerboseLevel()){
      std::cout << "////       TARGET      ////" << std::endl;
      std::cout << "//// Solid Target found " << std::endl;
    }
    std::vector<std::string> token = {"Thickness","Radius","Material","Angle","X","Y","Z"};
    if(starget[0]->HasTokenList(token)){
      m_TargetThickness = starget[0]->GetDouble("Thickness","micrometer");
      m_TargetAngle = starget[0]->GetDouble("Angle","deg");
      m_TargetMaterial = starget[0]->GetString("Material");
      m_TargetX = starget[0]->GetDouble("X","mm");
      m_TargetY = starget[0]->GetDouble("Y","mm");
      m_TargetZ = starget[0]->GetDouble("Z","mm");
    }
    else{
      std::cout << "ERROR: Target token list incomplete, check your input file" << std::endl;
      exit(1);
    }
  }
  else if(ctarget.size()==1){
    if(NPOptionManager::getInstance()->GetVerboseLevel())
    std::cout<< "//// Cryogenic Target found " << std::endl;
    m_CryoTarget = true;
    std::vector<std::string> CoreToken   = {"NominalThickness","Material","Density","Radius","Angle","X","Y","Z"};
    std::vector<std::string> FrontToken  = {"FrontDeformation","FrontThickness","FrontRadius","FrontMaterial"};
    std::vector<std::string> BackToken   = {"BackDeformation","BackThickness","BackRadius","BackMaterial"};
    std::vector<std::string> FrameToken  = {"FrameRadius","FrameThickness","FrontCone","BackCone","FrameMaterial"};
    std::vector<std::string> ShieldToken = {"ShieldInnerRadius","ShieldOuterRadius""ShieldBottomLength","ShieldTopLength","ShieldFrontRadius","ShieldBackRadius","ShieldMaterial"};

    if(ctarget[0]->HasTokenList(CoreToken)){
       // Target 
      m_TargetThickness = ctarget[0]->GetDouble("NominalThickness","micrometer");
      m_TargetAngle = ctarget[0]->GetDouble("Angle","deg");
      m_TargetMaterial = ctarget[0]->GetString("Material");
      m_TargetDensity = ctarget[0]->GetDouble("Density","g/cm3");
      m_TargetRadius = ctarget[0]->GetDouble("Radius","mm");
      m_TargetX = ctarget[0]->GetDouble("X","mm");
      m_TargetY = ctarget[0]->GetDouble("Y","mm");
      m_TargetZ = ctarget[0]->GetDouble("Z","mm");
      m_TargetDensity = ctarget[0]->GetDouble("Density","g/cm3"); 
      m_TargetRadius = ctarget[0]->GetDouble("Radius","mm");

      // Front Window
      m_FrontDeformation = ctarget[0]->GetDouble("FrontDeformation","mm");
      m_FrontThickness = ctarget[0]->GetDouble("FrontThickness","micrometer");
      m_FrontRadius = ctarget[0]->GetDouble("FrontRadius","mm");
      m_FrontMaterial = ctarget[0]->GetString("FrontMaterial");

      // Back Window
      m_BackDeformation = ctarget[0]->GetDouble("BackDeformation","mm");
      m_BackRadius = ctarget[0]->GetDouble("BackRadius","mm");
      m_BackThickness = ctarget[0]->GetDouble("BackThickness","micrometer");
      m_BackMaterial = ctarget[0]->GetString("BackMaterial");

      // Cell Frame
      m_FrameRadius = ctarget[0]->GetDouble("FrameRadius","mm");
      m_FrameThickness = ctarget[0]->GetDouble("FrameThickness","mm");
      m_FrontCone = ctarget[0]->GetDouble("FrontCone","deg");
      m_BackCone = ctarget[0]->GetDouble("BackCone","deg");
      m_FrameMaterial = ctarget[0]->GetString("FrameMaterial");
      // Heat Shield
      m_ShieldInnerRadius = ctarget[0]->GetDouble("ShieldInnerRadius","mm");
      m_ShieldOuterRadius = ctarget[0]->GetDouble("ShieldOuterRadius","mm");
      m_ShieldBottomLength = ctarget[0]->GetDouble("ShieldBottomLength","mm");
      m_ShieldTopLength = ctarget[0]->GetDouble("ShieldTopLength","mm");
      m_ShieldFrontRadius = ctarget[0]->GetDouble("ShieldFrontRadius","mm"); 
      m_ShieldBackRadius = ctarget[0]->GetDouble("ShieldBackRadius","mm");
      m_ShieldMaterial = ctarget[0]->GetString("ShieldMaterial");
    }
    else{
      std::cout<< "ERROR: CryogenicTarget token list incomplete, check your input file" << std::endl;
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

  // If Requiered, they can also instiantiate their control histogramm
  if(NPOptionManager::getInstance()->GetGenerateHistoOption())
    InitSpectra();

  // The calibration Manager got all the parameter added, so it can load them from the calibration file
  CalibrationManager::getInstance()->LoadParameterFromFile();

  // Start the thread if multithreading supported
#if __cplusplus > 199711L && NPMULTITHREADING
  InitThreadPool();
#endif

  return;
}

///////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::BuildPhysicalEvent(){
#if __cplusplus > 199711L && NPMULTITHREADING
    // add new job
    m_Ready.flip();
    std::this_thread::yield();

  while(!IsDone()){
    std::this_thread::yield();
    }

#else 
  static std::map<std::string,VDetector*>::iterator it;
  static std::map<std::string,VDetector*>::iterator begin=m_Detector.begin();
  static std::map<std::string,VDetector*>::iterator end= m_Detector.end();

  for (it =begin; it != end; ++it) {
    (it->second->*m_ClearEventPhysicsPtr)();
    (it->second->*m_BuildPhysicalPtr)();
    if(m_FillSpectra){
      (it->second->*m_FillSpectra)();
      if(m_CheckSpectra)
        (it->second->*m_CheckSpectra)();
    }
  }
#endif
  return;
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
#if __cplusplus > 199711L && NPMULTITHREADING

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
  // Let the main thread start 
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  while(true){
    // Do the job if possible
    if(m_Ready[id]){
      // Do the job
      (det->*m_ClearEventPhysicsPtr)();

      (det->*m_BuildPhysicalPtr)();

      if(m_FillSpectra){
        (det->*m_FillSpectra)();
       if(m_CheckSpectra)
          (det->*m_CheckSpectra)();
      }
     m_Ready[id].flip();
     std::this_thread::yield();
   }
   else{
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::this_thread::yield();
   }

    // Return if stopped
    if(m_stop){
      return;
      }
  }   
}
////////////////////////////////////////////////////////////////////////////////
void NPL::DetectorManager::StopThread(){
  // make sure the last thread are schedule before stopping;
  std::this_thread::yield();
  m_stop=true;
  std::this_thread::yield();
}
////////////////////////////////////////////////////////////////////////////////
bool NPL::DetectorManager::IsDone(){
  static std::vector<bool>::iterator it;
  static std::vector<bool>::iterator begin = m_Ready.begin(); 
  static std::vector<bool>::iterator end = m_Ready.end();
  unsigned int i = 0 ;
  for( it = begin ; it!=end ; it++){
    //std::cout << i++ << " " << *it << std::endl;
    if((*it))
      return false;
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
  // int r;
  //r=system("nponline localhost 9092 & ");
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
////////////////////////////////////////////////////////////////////////////////
bool NPL::DetectorManager::IsCryogenic() const {return  m_CryoTarget;};
double NPL::DetectorManager::GetTargetThickness() const {return m_TargetThickness;};
double NPL::DetectorManager::GetNominalTargetThickness() const {return  m_TargetThickness;};
double NPL::DetectorManager::GetTargetDensity() const {return  m_TargetDensity;};
double NPL::DetectorManager::GetFrontDeformation() const {return  m_FrontDeformation;};
double NPL::DetectorManager::GetFrontThickness() const {return  m_FrontThickness;};
double NPL::DetectorManager::GetFrontRadius() const {return  m_FrontRadius;};
std::string NPL::DetectorManager::GetFrontMaterial() const {return  m_FrontMaterial;};
double NPL::DetectorManager::GetBackDeformation() const {return  m_BackDeformation;};
double NPL::DetectorManager::GetBackRadius() const {return  m_BackRadius;};
double NPL::DetectorManager::GetBackThickness() const {return  m_BackThickness;};
std::string NPL::DetectorManager::GetBackMaterial() const {return  m_BackMaterial;};
double NPL::DetectorManager::GetFrameRadius() const {return  m_FrameRadius;};
double NPL::DetectorManager::GetFrameThickness() const {return  m_FrameThickness;};
double NPL::DetectorManager::GetFrontCone() const {return  m_FrontCone;};
double NPL::DetectorManager::GetBackCone() const {return  m_BackCone;};
std::string NPL::DetectorManager::GetFrameMaterial() const {return  m_FrameMaterial;};
double NPL::DetectorManager::GetShieldInnerRadius() const {return  m_ShieldInnerRadius;};
double NPL::DetectorManager::GetShieldOuterRadius() const {return  m_ShieldOuterRadius;};
double NPL::DetectorManager::GetShieldBottomLength() const {return  m_ShieldBottomLength;};
double NPL::DetectorManager::GetShieldTopLength() const {return  m_ShieldTopLength;};
double NPL::DetectorManager::GetShieldFrontRadius() const {return  m_ShieldFrontRadius;}; 
double NPL::DetectorManager::GetShieldBackRadius() const {return  m_ShieldBackRadius;};
std::string NPL::DetectorManager::GetShieldMaterial() const {return  m_ShieldMaterial;};
std::string NPL::DetectorManager::GetTargetMaterial() const {return m_TargetMaterial;};
double NPL::DetectorManager::GetWindowsThickness() const {return m_WindowsThickness;};
std::string NPL::DetectorManager::GetWindowsMaterial() const {return m_WindowsMaterial;};
double NPL::DetectorManager::GetTargetRadius() const {return m_TargetRadius;};
double NPL::DetectorManager::GetTargetAngle() const {return m_TargetAngle;};
double NPL::DetectorManager::GetTargetX() const {return m_TargetX;};
double NPL::DetectorManager::GetTargetY() const {return m_TargetY;};
double NPL::DetectorManager::GetTargetZ() const {return m_TargetZ;}; 



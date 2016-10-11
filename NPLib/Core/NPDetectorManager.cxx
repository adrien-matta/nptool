/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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

  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());
  string LineBuffer,DataBuffer;
  set<string> check;
  bool cGeneralTarget=false;

  if (ConfigFile.is_open()) {
    cout << endl << "/////////// Detector geometry ///////////" << endl;
    cout << "Configuration file " << Path << " loading " << endl;
  }

  while (!ConfigFile.eof()) {
    // Pick-up next line
    getline(ConfigFile, LineBuffer);
    //Search for comment Symbol: %
    if (LineBuffer.compare(0, 1, "%") == 0) { ;}
    ////////////////////////////////////////////
    //////////// Search for Target /////////////
    ////////////////////////////////////////////

    else if (LineBuffer.compare(0, 13, "GeneralTarget") == 0 && cGeneralTarget == false) {
      cGeneralTarget = true ;
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
      bool check_WinMaterial = false;
      bool check_WinThickness = false ;

      bool ReadingStatusTarget = true;
      while (ReadingStatusTarget) {
        ConfigFile >> DataBuffer;
        // Search for comment Symbol %
        if (DataBuffer.compare(0, 1, "%") == 0) {ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );getline(ConfigFile, LineBuffer);}

        else if (DataBuffer.compare(0, 10, "THICKNESS=") == 0) {
          check_Thickness = true ;
          ConfigFile >> DataBuffer;
          m_TargetThickness = atof(DataBuffer.c_str());
          cout << "Target Thickness: " << m_TargetThickness << endl;
        }

        else if (DataBuffer.compare(0, 6, "ANGLE=") == 0) {
          ConfigFile >> DataBuffer;
          m_TargetAngle = atof(DataBuffer.c_str());
          cout << "Target Angle: " << m_TargetAngle << endl;
        }

        else if (DataBuffer.compare(0, 7, "RADIUS=") == 0) {
          check_Radius = true ;
          ConfigFile >> DataBuffer;
          m_TargetRadius = atof(DataBuffer.c_str());
          cout << "Target Radius: " <<  m_TargetRadius << endl;
        }

        else if (DataBuffer.compare(0, 9, "MATERIAL=") == 0) {
          check_Material = true ;
          ConfigFile >> DataBuffer;
          m_TargetMaterial = DataBuffer;
          cout << "Target Material: " << m_TargetMaterial << endl;
        }

        else if (DataBuffer.compare(0, 17, "DENSITY=") == 0) {
          ConfigFile >> DataBuffer;
          /* Do Nothing */
        }


        else if (DataBuffer.compare(0, 17, "WINDOWSTHICKNESS=") == 0) {
        check_WinThickness = true ;
        ConfigFile >> DataBuffer;
        m_WindowsThickness = atof(DataBuffer.c_str());
        cout << "Windows Thickness: " 
            << m_WindowsThickness << " um" << endl   ;
        }

        else if (DataBuffer.compare(0, 16, "WINDOWSMATERIAL=") == 0) {
        check_WinMaterial = true ;
        ConfigFile >> DataBuffer;
        m_WindowsMaterial = DataBuffer;
        cout << "Windows Material: " << m_WindowsMaterial << endl;
        }

        else if (DataBuffer.compare(0, 2, "X=") == 0) {
          check_X = true ;
          ConfigFile >> DataBuffer;
          m_TargetX = atoi(DataBuffer.c_str());
          cout << "Target Coordinates (mm): ( " << m_TargetX << " ; ";
        }

        else if (DataBuffer.compare(0, 2, "Y=") == 0) {
          check_Y = true ;
          ConfigFile >> DataBuffer;
          m_TargetY = atoi(DataBuffer.c_str());
          cout << m_TargetY << " ; ";
        }

        else if (DataBuffer.compare(0, 2, "Z=") == 0) {
          check_Z = true ;
          ConfigFile >> DataBuffer;
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

    ///////////////////////////////////////////
    /////////// Search for Detectors ///////////
    ////////////////////////////////////////////
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


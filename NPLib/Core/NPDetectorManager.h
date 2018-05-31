#ifndef NPDetectorManager_h
#define NPDetectorManager_h
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address:  matta@lpccaen.in2p3.fr  *
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
 *****************************************************************************/
//   NPL
#include "NPVDetector.h"
#include "NPSpectraServer.h"
// ROOT
#include "TH1.h"

//   STL
#include <string>
#include <map>

#if __cplusplus > 199711L 
#include <thread>
#include <mutex>
#include <condition_variable>
#endif

using namespace std ;

typedef void(NPL::VDetector::*VDetector_FuncPtr)(void);
// This class manage a map of virtual detector
namespace NPL{
  class DetectorManager{
    public:
      DetectorManager();
      ~DetectorManager();

    public:
      void        ReadConfigurationFile(string Path);
      void        BuildPhysicalEvent();
      void        BuildSimplePhysicalEvent();
      void        InitializeRootInput();
      void        InitializeRootOutput();
      void        AddDetector(string,VDetector*);
      VDetector*  GetDetector(string);
      void        ClearEventPhysics();
      void        ClearEventData();
      void        InitSpectra();
      void        WriteSpectra();
      vector< map< string, TH1* > > GetSpectra();  
      vector<string> GetDetectorList();

    public: // for online spectra server
      void SetSpectraServer();
      void StopSpectraServer();
      void CheckSpectraServer();
    private :
      NPL::SpectraServer* m_SpectraServer;

    private:   
      // The map containning all detectors
      // Using a Map one can access to any detector using its name
      map<string,VDetector*> m_Detector;

    private: // Function pointer to accelerate the code execution
      VDetector_FuncPtr m_BuildPhysicalPtr;
      VDetector_FuncPtr m_ClearEventPhysicsPtr;
      VDetector_FuncPtr m_ClearEventDataPtr;
      VDetector_FuncPtr m_FillSpectra;
      VDetector_FuncPtr m_CheckSpectra;

#if __cplusplus > 199711L 
    private: // Thread Pool defined if C++11 is available
      vector<std::thread> m_ThreadPool;
      vector<bool> m_Ready;
      bool m_stop;
      std::mutex m_ThreadMtx;
      std::condition_variable m_CV;

    public: // Init the Thread Pool
      void StopThread();
      void StartThread(NPL::VDetector*,unsigned int);
      void InitThreadPool(); 
      bool IsDone();

#endif

    private: // Target property
     
      double m_TargetThickness;
      double m_TargetAngle;
      double m_TargetRadius;
      string m_TargetMaterial;
      double m_TargetX;
      double m_TargetY;
      double m_TargetZ;

      // Additional info for cryogenic target
      bool        m_CryoTarget;
      double      m_TargetDensity;
      double      m_FrontDeformation;
      double      m_FrontThickness;
      double      m_FrontRadius;
      string      m_FrontMaterial;
      double      m_BackDeformation;
      double      m_BackRadius;
      double      m_BackThickness;
      string      m_BackMaterial;
      double      m_FrameRadius;
      double      m_FrameThickness;
      double      m_FrontCone;
      double      m_BackCone;
      string      m_FrameMaterial;
      double      m_ShieldInnerRadius;
      double      m_ShieldOuterRadius;
      double      m_ShieldBottomLength;
      double      m_ShieldTopLength;
      double      m_ShieldFrontRadius; 
      double      m_ShieldBackRadius;
      string      m_ShieldMaterial;

    public:
      inline bool IsCryogenic(){return m_CryoTarget;};
      inline double GetTargetThickness      () {return m_TargetThickness;}
      inline double GetTargetDensity      () {return  m_TargetDensity;}
      inline double GetFrontDeformation   () {return  m_FrontDeformation;}
      inline double GetFrontThickness     () {return  m_FrontThickness;}
      inline double GetFrontRadius        () {return  m_FrontRadius;}
      inline string GetFrontMaterial      () {return  m_FrontMaterial;}
      inline double GetBackDeformation    () {return  m_BackDeformation;}
      inline double GetBackRadius         () {return  m_BackRadius;}
      inline double GetBackThickness      () {return  m_BackThickness;}
      inline string GetBackMaterial       () {return  m_BackMaterial;}
      inline double GetFrameRadius        () {return  m_FrameRadius;}
      inline double GetFrameThickness     () {return  m_FrameThickness;}
      inline double GetFrontCone          () {return  m_FrontCone;}
      inline double GetBackCone           () {return  m_BackCone;}
      inline string GetFrameMaterial      () {return  m_FrameMaterial;}
      inline double GetShieldInnerRadius  () {return  m_ShieldInnerRadius;}
      inline double GetShieldOuterRadius  () {return  m_ShieldOuterRadius;}
      inline double GetShieldBottomLength () {return  m_ShieldBottomLength;}
      inline double GetShieldTopLength    () {return  m_ShieldTopLength;}
      inline double GetShieldFrontRadius  () {return  m_ShieldFrontRadius;} 
      inline double GetShieldBackRadius   () {return  m_ShieldBackRadius;}
      inline string GetShieldMaterial     () {return  m_ShieldMaterial;}


      // Special treatment for the target for the moment
      // If necessary we should change it to treat it as 
      // a full "detector"

    public:
      string GetTargetMaterial()        {return m_TargetMaterial;}
      double GetTargetRadius()          {return m_TargetRadius;}
      double GetTargetAngle()           {return m_TargetAngle;}
      double GetTargetX()               {return m_TargetX;}
      double GetTargetY()               {return m_TargetY;}
      double GetTargetZ()               {return m_TargetZ;} 
  };
}

#endif 

#ifndef NPDetectorManager_h
#define NPDetectorManager_h
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:    contact address:                                      *
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

    private:
      double m_TargetThickness;
      double m_TargetAngle;
      double m_TargetRadius;
      string m_TargetMaterial;
      double m_TargetX;
      double m_TargetY;
      double m_TargetZ;

      // Special treatment for the target for the moment
      // If necessary we should change it to treat it as 
      // a full "detector"

    public:
      double GetTargetThickness()     {return m_TargetThickness;}
      string GetTargetMaterial()      {return m_TargetMaterial;}
      double GetTargetRadius()        {return m_TargetRadius;}
      double GetTargetAngle()         {return m_TargetAngle;}
      double GetTargetX()             {return m_TargetX;}
      double GetTargetY()             {return m_TargetY;}
      double GetTargetZ()             {return m_TargetZ;} 
    };
}

#endif 

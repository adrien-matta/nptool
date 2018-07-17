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
#include <vector>
 
#if __cplusplus > 199711L 
#include <thread>
#include <mutex>
#include <condition_variable>
#endif


typedef void(NPL::VDetector::*VDetector_FuncPtr)(void);
// This class manage a std::map of virtual detector
namespace NPL{
  class DetectorManager{
    public:
      DetectorManager();
      ~DetectorManager();

    public:
      void        ReadConfigurationFile(std::string Path);
      void        BuildPhysicalEvent();
      void        InitializeRootInput();
      void        InitializeRootOutput();
      void        AddDetector(std::string,VDetector*);
      VDetector*  GetDetector(std::string);
      void        ClearEventPhysics();
      void        ClearEventData();
      void        InitSpectra();
      void        WriteSpectra();
      std::vector< std::map< std::string, TH1* > > GetSpectra();  
      std::vector<std::string> GetDetectorList();

    public: // for online spectra server
      void SetSpectraServer();
      void StopSpectraServer();
      void CheckSpectraServer();
    private :
      NPL::SpectraServer* m_SpectraServer;

    private:   
      // The std::map containning all detectors
      // Using a Map one can access to any detector using its name
      std::map<std::string,VDetector*> m_Detector;

    private: // Function pointer to accelerate the code execution
      VDetector_FuncPtr m_BuildPhysicalPtr;
      VDetector_FuncPtr m_ClearEventPhysicsPtr;
      VDetector_FuncPtr m_ClearEventDataPtr;
      VDetector_FuncPtr m_FillSpectra;
      VDetector_FuncPtr m_CheckSpectra;
      
    #if __cplusplus > 199711L && NPMULTITHREADING 
    private: // Thread Pool defined if C++11 is available
      std::vector<std::thread> m_ThreadPool;
      std::vector<bool> m_Ready;
      bool m_stop;

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
      std::string m_TargetMaterial;
      double m_TargetX;
      double m_TargetY;
      double m_TargetZ;
  
      // Additional info for cryogenic target
      double m_WindowsThickness;
      std::string m_WindowsMaterial;
  

      // Special treatment for the target for the moment
      // If necessary we should change it to treat it as 
      // a full "detector"

    public:
      double GetTargetThickness()       {return m_TargetThickness;}
      std::string GetTargetMaterial()   {return m_TargetMaterial;}
      double GetWindowsThickness()      {return m_WindowsThickness;}
      std::string GetWindowsMaterial()  {return m_WindowsMaterial;}
      double GetTargetRadius()          {return m_TargetRadius;}
      double GetTargetAngle()           {return m_TargetAngle;}
      double GetTargetX()               {return m_TargetX;}
      double GetTargetY()               {return m_TargetY;}
      double GetTargetZ()               {return m_TargetZ;} 
    };
}

#endif 

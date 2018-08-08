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
#include <iostream>
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
      bool        m_CryoTarget;
      double      m_TargetDensity;
      double      m_FrontDeformation;
      double      m_FrontThickness;
      double      m_FrontRadius;
      std::string m_FrontMaterial;
      double      m_BackDeformation;
      double      m_BackRadius;
      double      m_BackThickness;
      std::string m_BackMaterial;
      double      m_FrameRadius;
      double      m_FrameThickness;
      double      m_FrontCone;
      double      m_BackCone;
      std::string m_FrameMaterial;
      double      m_ShieldInnerRadius;
      double      m_ShieldOuterRadius;
      double      m_ShieldBottomLength;
      double      m_ShieldTopLength;
      double      m_ShieldFrontRadius; 
      double      m_ShieldBackRadius;
      std::string m_ShieldMaterial;
      double      m_WindowsThickness;
      std::string m_WindowsMaterial;
 
    public: // those are define in cxx, otherwise the return value depend on context !?
      bool IsCryogenic() const; // {return  m_CryoTarget;};
      double GetTargetThickness() const;//{return m_TargetThickness;};
      double GetNominalTargetThickness() const; // {return  m_TargetThickness;};
      double GetTargetDensity() const; // {return  m_TargetDensity;};
      double GetFrontDeformation() const; // {return  m_FrontDeformation;};
      double GetFrontThickness() const; // {return  m_FrontThickness;};
      double GetFrontRadius() const; // {return  m_FrontRadius;};
      std::string GetFrontMaterial() const; // {return  m_FrontMaterial;};
      double GetBackDeformation() const; // {return  m_BackDeformation;};
      double GetBackRadius() const; // {return  m_BackRadius;};
      double GetBackThickness() const; // {return  m_BackThickness;};
      std::string GetBackMaterial() const; // {return  m_BackMaterial;};
      double GetFrameRadius() const; // {return  m_FrameRadius;};
      double GetFrameThickness() const; // {return  m_FrameThickness;};
      double GetFrontCone() const; // {return  m_FrontCone;};
      double GetBackCone() const; // {return  m_BackCone;};
      std::string GetFrameMaterial() const; // {return  m_FrameMaterial;};
      double GetShieldInnerRadius() const; // {return  m_ShieldInnerRadius;};
      double GetShieldOuterRadius() const; // {return  m_ShieldOuterRadius;};
      double GetShieldBottomLength() const; // {return  m_ShieldBottomLength;};
      double GetShieldTopLength() const; // {return  m_ShieldTopLength;};
      double GetShieldFrontRadius() const; // {return  m_ShieldFrontRadius;}; 
      double GetShieldBackRadius() const; // {return  m_ShieldBackRadius;};
      std::string GetShieldMaterial() const; // {return  m_ShieldMaterial;};
      std::string GetTargetMaterial() const; // {return m_TargetMaterial;};
      double GetWindowsThickness() const; // {return m_WindowsThickness;};
      std::string GetWindowsMaterial() const; // {return m_WindowsMaterial;};
      double GetTargetRadius() const; // {return m_TargetRadius;};
      double GetTargetAngle() const; // {return m_TargetAngle;};
      double GetTargetX() const; // {return m_TargetX;};
      double GetTargetY() const; // {return m_TargetY;};
      double GetTargetZ() const; // {return m_TargetZ;}; 

  };
}

#endif 

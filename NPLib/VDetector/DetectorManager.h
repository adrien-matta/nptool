#ifndef DetectorManager_h
#define DetectorManager_h
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
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
#include "VDetector.h"

// ROOT
#include "TH1.h"

//   STL
#include <string>
#include <map>
using namespace std ;

using namespace NPA ;

// This class manage a map of virtual detector
namespace NPA{
  class DetectorManager{
    public:
      DetectorManager();
      ~DetectorManager();

    public:
      // Read stream at Path and pick-up Token declaration of Detector
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
      vector< map< vector<string>, TH1* > > GetSpectra();  

    private:   
      // The map containning all detectors
      // Using a Map one can access to any detector using its name
      map<string,VDetector*> m_Detector;

      // Special treatment for the target for the moment
      // If necessary we should change it to treat it as 
      // a full "detector"
    private:
      double m_TargetThickness;
      double m_TargetAngle;
      double m_TargetRadius;
      string m_TargetMaterial;
      double m_TargetX;
      double m_TargetY;
      double m_TargetZ;

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

#ifndef DetectorManager_h
#define DetectorManager_h

//   NPL
#include "VDetector.h"

//   STL
#include <string>
#include <map>

using namespace std ;
using namespace NPA ;

// This class manage a map of virtual detector
namespace NPA
   {
      class DetectorManager
         {
            public:
               DetectorManager();
               ~DetectorManager();

            public:
               // Read stream at Path and pick-up Token declaration of Detector
               void   ReadConfigurationFile(string Path);
               void   BuildPhysicalEvent();
               void   BuildSimplePhysicalEvent();
               void   InitializeRootInput();
               void   InitializeRootOutput();
               void   AddDetector(string,VDetector*);
               void   ClearEventPhysics();
               void   ClearEventData();

            public:   
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

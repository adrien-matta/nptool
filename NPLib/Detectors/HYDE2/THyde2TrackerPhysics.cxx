/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 31/08/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the physical results after NPAnalysis is run*
 *             for the tracker part of the Hyde2 detector.                 *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "THyde2TrackerPhysics.h"
#include "NPDetectorFactory.h"

ClassImp(THyde2TrackerPhysics)


THyde2TrackerPhysics::THyde2TrackerPhysics() 
{
}



THyde2TrackerPhysics::~THyde2TrackerPhysics()
{
   Clear();
}



void THyde2TrackerPhysics::Clear()
{
   // General members
   fEventType.clear();
   fModuleNumber.clear();
   fTotalEnergy.clear();

   // First stage
   fFirstStage_Energy.clear();
   fFirstStage_Time.clear();
   fFirstStage_FrontPosition.clear();
   fFirstStage_BackPosition.clear();

   // Second stage
   fSecondStage_Energy.clear();
   fSecondStage_Time.clear();
   fSecondStage_Position.clear();

   // Third stage
   fThirdStage_Energy.clear();
   fThirdStage_Time.clear();
   fThirdStage_Position.clear();

   // Fourth stage
   fFourthStage_Energy.clear();
   fFourthStage_Time.clear();
   fFourthStage_Position.clear();
}
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* THyde2TrackerPhysics::Construct(){
  return (NPL::VDetector*) new THyde2TrackerPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_hyde2{
  public:
    proxy_hyde2(){
      NPL::DetectorFactory::getInstance()->AddToken("Hyde2Tracker","Hyde2Tracker");
      NPL::DetectorFactory::getInstance()->AddDetector("Hyde2Tracker",THyde2TrackerPhysics::Construct);
    }
};

proxy_hyde2 p;
}


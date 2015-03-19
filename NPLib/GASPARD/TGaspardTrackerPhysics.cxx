/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
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
 *             for the tracker part of the Gaspard detector.                 *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TGaspardTrackerPhysics.h"
#include "NPDetectorFactory.h"

ClassImp(TGaspardTrackerPhysics)


TGaspardTrackerPhysics::TGaspardTrackerPhysics() 
{
}



TGaspardTrackerPhysics::~TGaspardTrackerPhysics()
{
   Clear();
}



void TGaspardTrackerPhysics::Clear()
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
}
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPA::VDetector* TGaspardTrackerPhysics::Construct(){
  return (NPA::VDetector*) new TGaspardTrackerPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy{
  public:
    proxy(){
      NPA::DetectorFactory::getInstance()->AddToken("GaspardTracker","GaspardTracker");
      NPA::DetectorFactory::getInstance()->AddDetector("GaspardTracker",TGaspardTrackerPhysics::Construct);
    }
};

proxy p;
}


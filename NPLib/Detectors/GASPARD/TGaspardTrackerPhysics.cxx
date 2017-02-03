/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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

ClassImp(TGaspardTrackerPhysics)

////////////////////////////////////////////////////////////////////////////////
TGaspardTrackerPhysics::TGaspardTrackerPhysics() {
}

////////////////////////////////////////////////////////////////////////////////
TGaspardTrackerPhysics::~TGaspardTrackerPhysics(){
   Clear();
}

////////////////////////////////////////////////////////////////////////////////
void TGaspardTrackerPhysics::Clear(){
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


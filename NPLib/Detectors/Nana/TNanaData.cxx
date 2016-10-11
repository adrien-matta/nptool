/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 04/12/2009                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of the Nana detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include "TNanaData.h"
ClassImp(TNanaData)

////////////////////////////////////////////////////////////////////////////////
TNanaData::TNanaData(){
}

TNanaData::~TNanaData(){
}

////////////////////////////////////////////////////////////////////////////////
void TNanaData::Clear(){
   fNANA_LaBr3_DetectorNbr.clear();
   fNANA_LaBr3_EnergyShort.clear();
fNANA_LaBr3_EnergyLong.clear();
 fNANA_LaBr3_Time.clear();
 fNANA_LaBr3_PSD.clear();
  fNANA_LaBr3_Event.clear();
}

////////////////////////////////////////////////////////////////////////////////
void TNanaData::Dump() const{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
   for (UShort_t i = 0; i < fNANA_LaBr3_DetectorNbr.size(); i++){
      cout << "DetNbr: " << fNANA_LaBr3_DetectorNbr[i] 
	    << " Energy Short: " << fNANA_LaBr3_EnergyShort[i] 
	    << " Energy Long: " << fNANA_LaBr3_EnergyLong[i] 
	    << " Time: " << fNANA_LaBr3_Time[i] 
      << " PSD: " << fNANA_LaBr3_PSD[i]
      << " Event: " << fNANA_LaBr3_Event[i] << endl;
  }
}

/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
   fNANA_LaBr3_E_DetectorNbr.clear();
   fNANA_LaBr3_E_Energy.clear();
   // Time
   fNANA_LaBr3_T_DetectorNbr.clear();
   fNANA_LaBr3_T_Time.clear();
}

////////////////////////////////////////////////////////////////////////////////
void TNanaData::Dump() const{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
   // E
   for (UShort_t i = 0; i < fNANA_LaBr3_E_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fNANA_LaBr3_E_DetectorNbr[i] << " Energy: " << fNANA_LaBr3_E_Energy[i] << endl;
   
   // T
   for (UShort_t i = 0; i < fNANA_LaBr3_T_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fNANA_LaBr3_T_DetectorNbr[i] << " Time: " << fNANA_LaBr3_T_Time[i] << endl;
}

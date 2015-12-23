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
 *  This class described the raw data of the Fatima detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include "TFatimaData.h"
ClassImp(TFatimaData)

////////////////////////////////////////////////////////////////////////////////
TFatimaData::TFatimaData(){
}

TFatimaData::~TFatimaData(){
}

////////////////////////////////////////////////////////////////////////////////
void TFatimaData::Clear(){
   fFATIMA_LaBr3_E_DetectorNbr.clear();
   fFATIMA_LaBr3_E_Energy.clear();
   // Time
   fFATIMA_LaBr3_T_DetectorNbr.clear();
   fFATIMA_LaBr3_T_Time.clear();
}

////////////////////////////////////////////////////////////////////////////////
void TFatimaData::Dump() const{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
   // E
   for (UShort_t i = 0; i < fFATIMA_LaBr3_E_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fFATIMA_LaBr3_E_DetectorNbr[i] << " Energy: " << fFATIMA_LaBr3_E_Energy[i] << endl;
   
   // T
   for (UShort_t i = 0; i < fFATIMA_LaBr3_T_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fFATIMA_LaBr3_T_DetectorNbr[i] << " Time: " << fFATIMA_LaBr3_T_Time[i] << endl;
}

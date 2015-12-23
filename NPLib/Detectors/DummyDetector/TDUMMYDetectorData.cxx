/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : October 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of a very simple DUMMYDetector         *
 *   Use it as a template for your own detector!                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include "TDUMMYDetectorData.h"
ClassImp(TDUMMYDetectorData)

////////////////////////////////////////////////////////////////////////////////
TDUMMYDetectorData::TDUMMYDetectorData(){
}


////////////////////////////////////////////////////////////////////////////////
TDUMMYDetectorData::~TDUMMYDetectorData(){
}


////////////////////////////////////////////////////////////////////////////////
void TDUMMYDetectorData::Clear(){
    fDUMMYDetector_Energy.clear();
    fDUMMYDetector_Number.clear();
    fDUMMYDetector_Time.clear();
}


////////////////////////////////////////////////////////////////////////////////
void TDUMMYDetectorData::Dump() const{
   cout << "XXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   for(unsigned short i = 0 ; i<fDUMMYDetector_Energy.size() ; i ++)
      {
         cout << "DUMMYDetector Number " << fDUMMYDetector_Number[i] << " Energy: " << fDUMMYDetector_Energy[i]  << " Time: "<< fDUMMYDetector_Time[i] << endl;
      
      }
   
}

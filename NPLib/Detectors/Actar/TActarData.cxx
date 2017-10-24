/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : September 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Actar Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "TActarData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

ClassImp(TActarData)


//////////////////////////////////////////////////////////////////////
TActarData::TActarData() {
}



//////////////////////////////////////////////////////////////////////
TActarData::~TActarData() {
}



//////////////////////////////////////////////////////////////////////
void TActarData::Clear() {
  // Charge
    fActar_PadNumber.clear();
    fActar_PadRow.clear();
    fActar_PadColumn.clear();
    fActar_PadCharge.clear();
    fActar_PadTime.clear();
    fActar_dig_Charge.clear();
    fActar_dig_Time.clear();

    fSilicon_Energy.clear();
    fSilicon_Time.clear();
    fSilicon_DetectorNumber.clear();

    fCsI_Energy.clear();
    fCsI_CrystalNumber.clear();
}

////////////////////////////////////////////////////////////////////////////////
TGraph* TActarData::GetChargeAsGraph(){
    TGraph* res = new TGraph (fActar_dig_Charge.size(),&fActar_dig_Time[0],&fActar_dig_Charge[0]);
    res->Sort();
    return res;

}


//////////////////////////////////////////////////////////////////////
void TActarData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TActarData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Charge
  size_t mysize = fActar_PadNumber.size();
  cout << "Actar_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "Pad Number: " << fActar_PadNumber[i]
         << " Charge: " << fActar_PadCharge[i]
        << " Time: " << fActar_PadTime[i];
  }

}

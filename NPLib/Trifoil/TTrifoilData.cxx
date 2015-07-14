/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : December 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Trifoil Raw data                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

// NPL
#include "TTrifoilData.h"

//ROOT
#include "TDirectory.h"

ClassImp(TTrifoilData)

/////////////////////////
TTrifoilData::TTrifoilData(){
}

/////////////////////////
TTrifoilData::~TTrifoilData(){
}

/////////////////////////
void TTrifoilData::Clear(){
  fTrifoil_Waveform.clear();
  fTrifoil_TimeCFD.clear();
  fTrifoil_TimeLED.clear();
}
/////////////////////////
void TTrifoilData::Dump() const{
}

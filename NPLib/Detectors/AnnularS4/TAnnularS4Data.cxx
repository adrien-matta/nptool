/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : January 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularS4 Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TAnnularS4Data.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TAnnularS4Data)


//////////////////////////////////////////////////////////////////////
TAnnularS4Data::TAnnularS4Data() {
}



//////////////////////////////////////////////////////////////////////
TAnnularS4Data::~TAnnularS4Data() {
}



//////////////////////////////////////////////////////////////////////
void TAnnularS4Data::Clear() {
    // DSSD
    // (Th,E)
    fS4_E_DetNbr.clear();
    fS4_E_StripNbr.clear();
    fS4_E_SectorNbr.clear();
    fS4_E_Energy.clear();
    // (Th,T)
    fS4_T_DetNbr.clear();
    fS4_T_StripNbr.clear();
    fS4_T_SectorNbr.clear();
    fS4_T_Time.clear();
   
}



//////////////////////////////////////////////////////////////////////
void TAnnularS4Data::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TAnnularS4Data::Dump()] XXXXXXXXXXXXXXXXX" << endl;

    // DSSD
    // (Th,E)
    cout << "S4_MultThE = " << fS4_E_DetNbr.size() << endl;
    for (UShort_t i = 0; i < fS4_E_DetNbr.size(); i++)
        cout << "Det: " << fS4_E_DetNbr[i] << " Strip: " << fS4_E_StripNbr[i] << " Sector: " << fS4_E_SectorNbr[i] <<  " Energy: " << fS4_E_Energy[i] << endl;
    // (Th,T)
    cout << "S4_MultThT = " << fS4_T_DetNbr.size() << endl;
    for (UShort_t i = 0; i < fS4_T_DetNbr.size(); i++)
        cout << "Det: " << fS4_T_DetNbr[i] << " Strip: " << fS4_T_StripNbr[i] << " Sector: " << fS4_T_SectorNbr[i] <<" Time: " << fS4_T_Time[i] << endl;
}


















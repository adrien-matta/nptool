/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: m.moukaddam@surrey.ac.uk                                 *
 *                                                                           *
 * Creation Date  : November 2016                                            *
 * Last update    :                                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold FPDTamu Raw data                                         *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TFPDTamuData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TFPDTamuData)


//////////////////////////////////////////////////////////////////////
TFPDTamuData::TFPDTamuData() {
}



//////////////////////////////////////////////////////////////////////
TFPDTamuData::~TFPDTamuData() {
}



//////////////////////////////////////////////////////////////////////
void TFPDTamuData::Clear() {

    fFPDTamu_Delta_E_DetectorNbr.clear();
    fFPDTamu_Delta_Energy.clear();
    fFPDTamu_Delta_T_DetectorNbr.clear();
    fFPDTamu_Delta_Time.clear();

    fFPDTamu_AWire_E_DetectorNbr.clear();
    fFPDTamu_AWire_E_DetectorSide.clear();
    fFPDTamu_AWire_Energy.clear();
    fFPDTamu_AWire_T_DetectorNbr.clear();
    fFPDTamu_AWire_T_DetectorSide.clear();
    fFPDTamu_AWire_Time.clear();

    fFPDTamu_Micro_E_RowNbr.clear();
    fFPDTamu_Micro_E_ColNbr.clear(); 
    fFPDTamu_Micro_Energy.clear();
    fFPDTamu_Micro_T_RowNbr.clear(); 
    fFPDTamu_Micro_T_ColNbr.clear(); 
    fFPDTamu_Micro_Time.clear();

    fFPDTamu_Plast_E_DetectorSide.clear();
    fFPDTamu_Plast_Energy.clear();
    fFPDTamu_Plast_T_DetectorSide.clear();
    fFPDTamu_Plast_Time.clear();

}



//////////////////////////////////////////////////////////////////////
void TFPDTamuData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TFPDTamuData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  cout << "  ...oooOOOooo...   Delta  ...oooOOOooo...   " << endl;
  // Energy
  size_t mysize = fFPDTamu_Delta_E_DetectorNbr.size();
  cout << "E Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fFPDTamu_Delta_E_DetectorNbr[i]
         << " Energy: " << fFPDTamu_Delta_Energy[i]
         <<endl;
  }
  // Time
  mysize = fFPDTamu_Delta_T_DetectorNbr.size();
  cout << "T Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fFPDTamu_Delta_T_DetectorNbr[i]
         << " Time: " << fFPDTamu_Delta_Time[i]
         <<endl;
  }



 cout << "  ...oooOOOooo...   Avalanche Wire  ...oooOOOooo...   " << endl;
  // Energy
  mysize = fFPDTamu_AWire_E_DetectorNbr.size();
  cout << "Energy Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fFPDTamu_AWire_E_DetectorNbr[i]
         << " Side(L-0 R-1): " << fFPDTamu_AWire_E_DetectorSide[i]
         << " Energy: " << fFPDTamu_AWire_Energy[i]
         <<endl;
  }
  // Time
  mysize = fFPDTamu_AWire_T_DetectorNbr.size();
  cout << "Time Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fFPDTamu_AWire_T_DetectorNbr[i]
         << " Side(L-0 R-1): " << fFPDTamu_AWire_T_DetectorSide[i]
         << " Time: " << fFPDTamu_AWire_Time[i]
         <<endl;
  }



 cout << "  ...oooOOOooo...   Micromega  ...oooOOOooo...   " << endl;
  // Energy
  mysize = fFPDTamu_Micro_E_RowNbr.size();
  cout << "Energy Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "Row: " << fFPDTamu_Micro_E_RowNbr[i]
         << " Col: " << fFPDTamu_Micro_E_ColNbr[i]
         << " Energy: " << fFPDTamu_Micro_Energy[i]
         <<endl;
  }
  // Time
  mysize = fFPDTamu_Micro_T_RowNbr.size();
  cout << "Time Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "Row: " << fFPDTamu_Micro_T_RowNbr[i]
         << " Col: " << fFPDTamu_Micro_T_ColNbr[i]
         << " Time: " << fFPDTamu_Micro_Time[i]
         <<endl;
  }



 cout << "  ...oooOOOooo...   Plastic Scintillator  ...oooOOOooo...   " << endl;
  // Energy
  mysize = fFPDTamu_Plast_E_DetectorSide.size();
  cout << "Energy Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "Side(L-0 R-1): " << fFPDTamu_Plast_E_DetectorSide[i]
         << " Energy: " << fFPDTamu_Plast_Energy[i]
         << endl;
  }
  // Time
  mysize = fFPDTamu_Plast_T_DetectorSide.size();
  cout << "Time Mult: " << mysize << endl;
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "Side(L-0 R-1): " << fFPDTamu_Plast_T_DetectorSide[i]
         << " Time: " << fFPDTamu_Plast_Time[i]
         << endl;
  }

}

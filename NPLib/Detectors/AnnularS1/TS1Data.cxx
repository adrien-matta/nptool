/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the results of the G4 simulation for the S1 *
 *             detector.                                                     *
 *             The format is the same as the one which is used for the GANIL *
 *             experiments after conversion of the raw data with GRU. Ask    *
 *             N. de Sereville and J. Burgunder (burgunder@ganil.fr) for     *
 *             informations.                                                 *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
using namespace std;

#include "TS1Data.h"


ClassImp(TS1Data)

  TS1Data::TS1Data(){
    // Default constructor

    Clear();
  }



TS1Data::~TS1Data(){
}



void TS1Data::Clear(){
  // DSSD
  // (Th,E)
  fS1_Theta_E_DetNbr.clear();
  fS1_Theta_E_StripNbr.clear();
  fS1_Theta_E_Energy.clear();
  // (Th,T)
  fS1_Theta_T_DetNbr.clear();
  fS1_Theta_T_StripNbr.clear();
  fS1_Theta_T_Time.clear();
  // (Ph,E)
  fS1_Phi_E_DetNbr.clear();
  fS1_Phi_E_StripNbr.clear();
  fS1_Phi_E_Energy.clear();
  // (Ph,T)
  fS1_Phi_T_DetNbr.clear();
  fS1_Phi_T_StripNbr.clear();
  fS1_Phi_T_Time.clear();
}



void TS1Data::Dump() const{
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

  // DSSD
  // (Th,E)
  cout << "S1_MultThE = " << fS1_Theta_E_DetNbr.size() << endl;
  for (UShort_t i = 0; i < fS1_Theta_E_DetNbr.size(); i++)
    cout << "DetThE: " << fS1_Theta_E_DetNbr[i] << " StripThE: " << fS1_Theta_E_StripNbr[i] << " EnergyTh: " << fS1_Theta_E_Energy[i] << endl;
  // (Th,T)
  cout << "S1_MultThT = " << fS1_Theta_T_DetNbr.size() << endl;
  for (UShort_t i = 0; i < fS1_Theta_T_DetNbr.size(); i++)
    cout << "DetThT: " << fS1_Theta_T_DetNbr[i] << " StripThT: " << fS1_Theta_T_StripNbr[i] << " TimeTh: " << fS1_Theta_T_Time[i] << endl;
  // (Ph,E)
  cout << "S1_MultPhE = " << fS1_Phi_E_DetNbr.size() << endl;
  for (UShort_t i = 0; i < fS1_Phi_E_DetNbr.size(); i++)
    cout << "DetPhE: " << fS1_Phi_E_DetNbr[i] << " StripPhE: " << fS1_Phi_E_StripNbr[i] << " EnergyPh: " << fS1_Phi_E_Energy[i] << endl;
  // (Ph,T)
  cout << "S1_MultThT = " << fS1_Phi_T_DetNbr.size() << endl;
  for (UShort_t i = 0; i < fS1_Phi_T_DetNbr.size(); i++)
    cout << "DetThT: " << fS1_Phi_T_DetNbr[i] << " StripThT: " << fS1_Phi_T_StripNbr[i] << " TimeTh: " << fS1_Phi_T_Time[i] << endl;
}

/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Elidiano Tronchin  contact address: tronchin@lpccaen.in2p3.fr                        *
 *                                                                           *
 * Creation Date  : October 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Minos Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TMinosData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TMinosData)


//////////////////////////////////////////////////////////////////////
TMinosData::TMinosData() {
}



//////////////////////////////////////////////////////////////////////
TMinosData::~TMinosData() {
}



//////////////////////////////////////////////////////////////////////
void TMinosData::Clear() {
  // Energy
  fMinos_E_DetectorNbr.clear();
  fMinos_Energy.clear();
  // Time
  fMinos_T_DetectorNbr.clear();
  fMinos_Time.clear();
  
 
  // Minos_Pads
  fMinos_Pad.clear();
  fMinos_X.clear();
  fMinos_Y.clear();
  fMinos_DriftTime.clear();
  fMinos_Charge.clear();

//From Santamaria:

x_tpc.clear();
y_tpc.clear();
z_tpc.clear();
e_tpc.clear();

x_InRoh.clear();
y_InRoh.clear();
z_InRoh.clear();
e_InRoh.clear();

x_OutRoh.clear();
y_OutRoh.clear();
z_OutRoh.clear();
e_OutRoh.clear();

x_Kap.clear();
y_Kap.clear();
z_Kap.clear();
e_Kap.clear();

x_tar.clear();
y_tar.clear();
z_tar.clear();
e_tar.clear();

x_win.clear();
y_win.clear();
z_win.clear();
e_win.clear();

x_ch.clear();
y_ch.clear();
z_ch.clear();
e_ch.clear();

x_trigger.clear();
y_trigger.clear();
z_trigger.clear();
e_trigger.clear();

Et_tpc_tot=0.;

Et_trigger.clear();
Et_tar.clear();
Et_ch.clear();
Et_win.clear();
Et_tpc.clear();
Et_InnerRohacell.clear();
Et_OuterRohacell.clear();
Et_Kapton.clear();

/*  //unuseful, cause nptool should make already that
//initial conditions
x0.clear();
y0.clear();
z0.clear();
theta0.clear();
phi0.clear();
energy0.clear();
detection.clear();
event = 0;
*/

A.clear();
Z.clear();

trackID.clear();
parentID.clear();

 //For take fitpar values
MINOSx_0.clear();
MINOSy_0.clear();
MINOSz_0.clear();
 MINOS_D_min.clear();
 MINOS_Radius.clear();
 MINOS_NumberTracks.clear();
 theta0.clear();
 phi0.clear();
energy0.clear();




}



//////////////////////////////////////////////////////////////////////
void TMinosData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TMinosData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fMinos_E_DetectorNbr.size();
  cout << "Minos_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMinos_E_DetectorNbr[i]
         << " Energy: " << fMinos_Energy[i];
  }
  
  // Time
  mysize = fMinos_T_DetectorNbr.size();
  cout << "Minos_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMinos_T_DetectorNbr[i]
         << " Time: " << fMinos_Time[i];
  }







////////////////////////ADD DUMP FOR SANTAMARIA VARIABLES !!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!






}

/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : November 2013                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Tiara Raw data                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

#include "TTiaraHyballData.h"

ClassImp(TTiaraHyballData)
  /////////////////////////
  TTiaraHyballData::TTiaraHyballData(){

    // Reserve the space for the full array 
    fTiaraHyball_Ring_DetectorNbr.reserve(6*32);
    fTiaraHyball_Ring_StripNbr.reserve(6*32);
    fTiaraHyball_Ring_Energy.reserve(6*32);
    fTiaraHyball_Ring_Time.reserve(6*32);

    fTiaraHyball_Sector_DetectorNbr.reserve(6*24);
    fTiaraHyball_Sector_StripNbr.reserve(6*24);
    fTiaraHyball_Sector_Energy.reserve(6*24);
    fTiaraHyball_Sector_Time.reserve(6*24);
  }

/////////////////////////
TTiaraHyballData::~TTiaraHyballData(){
  Clear();
}

/////////////////////////
void TTiaraHyballData::Clear(){
  fTiaraHyball_Ring_DetectorNbr.clear();
  fTiaraHyball_Ring_StripNbr.clear();
  fTiaraHyball_Ring_Energy.clear();
  fTiaraHyball_Ring_Time.clear();

  fTiaraHyball_Sector_DetectorNbr.clear();
  fTiaraHyball_Sector_StripNbr.clear();
  fTiaraHyball_Sector_Energy.clear();
  fTiaraHyball_Sector_Time.clear();
}


/////////////////////////
void TTiaraHyballData::Dump() const{
  cout << "************* TTiaraHyballData Event ************"<< endl;
  cout << "*** Ring ***" << endl;
  for( unsigned int i = 0; i < fTiaraHyball_Ring_DetectorNbr.size() ; i ++){
    cout << "Detector Number = " 
      << fTiaraHyball_Ring_DetectorNbr[i] << endl;  
    cout << "Ring Number = " 
      << fTiaraHyball_Ring_StripNbr[i] << endl;  
    cout << "Energy = " 
      << fTiaraHyball_Ring_Energy[i] << endl ;
    cout << "Time = " 
      << fTiaraHyball_Ring_Time[i] << endl ;  
  }
  cout << "*** Sector ***" << endl;
  for( unsigned int i = 0; i < fTiaraHyball_Sector_DetectorNbr.size() ; i ++){
    cout << "Detector Number = " 
      << fTiaraHyball_Sector_DetectorNbr[i] << endl;  
    cout << "Sector Number = " 
      << fTiaraHyball_Sector_StripNbr[i] << endl;  
    cout << "Energy = " 
      << fTiaraHyball_Sector_Energy[i] << endl ;
    cout << "Time = " << fTiaraHyball_Sector_Time[i] << endl ;  
  }

}


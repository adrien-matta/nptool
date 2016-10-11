/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
    fTiaraHyball_RingE_DetectorNbr.reserve(6*32);
    fTiaraHyball_RingE_StripNbr.reserve(6*32);
    fTiaraHyball_RingE_Energy.reserve(6*32);

    fTiaraHyball_SectorE_DetectorNbr.reserve(6*24);
    fTiaraHyball_SectorE_StripNbr.reserve(6*24);
    fTiaraHyball_SectorE_Energy.reserve(6*24);
   
    fTiaraHyball_RingT_DetectorNbr.reserve(6*32);
    fTiaraHyball_RingT_StripNbr.reserve(6*32);
    fTiaraHyball_RingT_Time.reserve(6*32);

    fTiaraHyball_SectorT_DetectorNbr.reserve(6*24);
    fTiaraHyball_SectorT_StripNbr.reserve(6*24);
    fTiaraHyball_SectorT_Time.reserve(6*24);
}

/////////////////////////
TTiaraHyballData::~TTiaraHyballData(){
  Clear();
}

/////////////////////////
void TTiaraHyballData::Clear(){
    fTiaraHyball_RingE_DetectorNbr.clear();
    fTiaraHyball_RingE_StripNbr.clear();
    fTiaraHyball_RingE_Energy.clear();

    fTiaraHyball_SectorE_DetectorNbr.clear();
    fTiaraHyball_SectorE_StripNbr.clear();
    fTiaraHyball_SectorE_Energy.clear();
   
    fTiaraHyball_RingT_DetectorNbr.clear();
    fTiaraHyball_RingT_StripNbr.clear();
    fTiaraHyball_RingT_Time.clear();

    fTiaraHyball_SectorT_DetectorNbr.clear();
    fTiaraHyball_SectorT_StripNbr.clear();
    fTiaraHyball_SectorT_Time.clear();
}


/////////////////////////
void TTiaraHyballData::Dump() const{
  cout << "************* TTiaraHyballData Event ************"<< endl;
  cout << "*** Ring ***" << endl;
  for( unsigned int i = 0; i < fTiaraHyball_RingE_DetectorNbr.size() ; i ++){
    cout << "Detector Number = " 
      << fTiaraHyball_RingE_DetectorNbr[i] << " | ";  
    cout << "RingE Number = " 
      << fTiaraHyball_RingE_StripNbr[i] << " | ";  
    cout << "Energy = " 
      << fTiaraHyball_RingE_Energy[i] << endl ;
  }

  for( unsigned int i = 0; i < fTiaraHyball_RingT_DetectorNbr.size() ; i ++){
    cout << "Detector Number = " 
      << fTiaraHyball_RingT_DetectorNbr[i] << " | ";  
    cout << "RingT Number = " 
      << fTiaraHyball_RingT_StripNbr[i] << " | ";  
    cout << "Time = " 
      << fTiaraHyball_RingT_Time[i] << endl ;  
  }



  cout << "*** Sector ***" << endl;
  for( unsigned int i = 0; i < fTiaraHyball_SectorE_DetectorNbr.size() ; i ++){
    cout << "Detector Number = " 
      << fTiaraHyball_SectorE_DetectorNbr[i] << " | ";  
    cout << "SectorE Number = " 
      << fTiaraHyball_SectorE_StripNbr[i] << " | ";  
    cout << "Energy = " 
      << fTiaraHyball_SectorE_Energy[i] << endl ;
  }
  
  for( unsigned int i = 0; i < fTiaraHyball_SectorT_DetectorNbr.size() ; i ++){
    cout << "Detector Number = " 
      << fTiaraHyball_SectorT_DetectorNbr[i] << " | ";  
    cout << "SectorT Number = " 
      << fTiaraHyball_SectorT_StripNbr[i] << " | ";  
    cout << "Time = " 
      << fTiaraHyball_SectorT_Time[i] << endl;  
  }

}


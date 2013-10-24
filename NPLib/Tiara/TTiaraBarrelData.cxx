/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : November 2012                                            *
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

#include "TTiaraBarrelData.h"

ClassImp(TTiaraBarrelData)
  /////////////////////////
  TTiaraBarrelData::TTiaraBarrelData(){

    // Reserve the space for the full array 
    fTiaraBarrel_Front_Upstream_DetectorNbr.reserve(8*4);         
    fTiaraBarrel_Front_Upstream_StripNbr.reserve(8*4);         
    fTiaraBarrel_Front_Upstream_Energy.reserve(8*4);        
    fTiaraBarrel_Front_Upstream_Time.reserve(8*4);          

    fTiaraBarrel_Front_Downstream_DetectorNbr.reserve(8*4);     
    fTiaraBarrel_Front_Downstream_StripNbr.reserve(8*4);           
    fTiaraBarrel_Front_Downstream_Energy.reserve(8*4);          
    fTiaraBarrel_Front_Downstream_Time.reserve(8*4);            

    fTiaraBarrel_Back_DetectorNbr.reserve(8);                  
    fTiaraBarrel_Back_Energy.reserve(8);                    
    fTiaraBarrel_Back_Time.reserve(8);                      

    fTiaraBarrel_Outer_DetectorNbr.reserve(8*4);               
    fTiaraBarrel_Outer_StripNbr.reserve(8*4);                    
    fTiaraBarrel_Outer_Energy.reserve(8*4);                        
    fTiaraBarrel_Outer_Time.reserve(8*4);
  }

/////////////////////////
TTiaraBarrelData::~TTiaraBarrelData(){
  Clear();
}

/////////////////////////
void TTiaraBarrelData::Clear(){
  fTiaraBarrel_Front_Upstream_DetectorNbr.clear();         
  fTiaraBarrel_Front_Upstream_StripNbr.clear();         
  fTiaraBarrel_Front_Upstream_Energy.clear();        
  fTiaraBarrel_Front_Upstream_Time.clear();          

  fTiaraBarrel_Front_Downstream_DetectorNbr.clear();     
  fTiaraBarrel_Front_Downstream_StripNbr.clear();           
  fTiaraBarrel_Front_Downstream_Energy.clear();          
  fTiaraBarrel_Front_Downstream_Time.clear();            

  fTiaraBarrel_Back_DetectorNbr.clear();                  
  fTiaraBarrel_Back_Energy.clear();                    
  fTiaraBarrel_Back_Time.clear();                      

  fTiaraBarrel_Outer_DetectorNbr.clear();               
  fTiaraBarrel_Outer_StripNbr.clear();                    
  fTiaraBarrel_Outer_Energy.clear();                        
  fTiaraBarrel_Outer_Time.clear();
}

/////////////////////////
void TTiaraBarrelData::Dump() const{
  cout << "************* TTiaraBarrelData Event ************"<< endl;
  cout << "*** Inner Barrel Front Upstream ***" << endl ;
  for( unsigned int i = 0; i < fTiaraBarrel_Front_Upstream_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fTiaraBarrel_Front_Upstream_DetectorNbr[i] << endl ;
    cout << "Strip Number = " 
      << fTiaraBarrel_Front_Upstream_StripNbr[i] << endl;
    cout << "Energy = " 
      << fTiaraBarrel_Front_Upstream_Energy[i]<< endl ;
    cout << "Time = " 
      << fTiaraBarrel_Front_Upstream_Time[i] << endl ;  
  }


  cout << "*** Inner Barrel Front Downstream ***" << endl ;
  for( unsigned int i = 0; i < fTiaraBarrel_Front_Downstream_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fTiaraBarrel_Front_Downstream_DetectorNbr[i] << endl;  
    cout << "Strip Number = " 
      << fTiaraBarrel_Front_Downstream_StripNbr[i] << endl;
    cout << "Energy = " 
      << fTiaraBarrel_Front_Downstream_Energy[i] << endl ;
    cout << "Time = " 
      << fTiaraBarrel_Front_Downstream_Time[i] << endl ;  
  }

  cout << "*** Inner Barrel Back ***" << endl ;
  for( unsigned int i = 0; i < fTiaraBarrel_Back_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fTiaraBarrel_Back_DetectorNbr[i] << endl;  
    cout << "Energy = " 
      << fTiaraBarrel_Back_Energy[i] << endl ;
    cout << "Time = " 
      << fTiaraBarrel_Back_Time[i] << endl ;  
  }
  cout << "*** Outer Barrel ***" << endl;
  for( unsigned int i = 0; i < fTiaraBarrel_Outer_DetectorNbr.size() ; i ++){
    cout << "Detector Number = " 
      << fTiaraBarrel_Outer_DetectorNbr[i] << endl;  
    cout << "Energy = " 
      << fTiaraBarrel_Outer_Energy[i] << endl ;
    cout << "Time = " 
      << fTiaraBarrel_Outer_Time[i] << endl ;  
  }
}
/////////////////////////
inline void TTiaraBarrelData::Set_Front_Upstream(const unsigned short& DetNbr , const unsigned short StripNbr , const double& Energy , const double& Time){

  Set_Front_Upstream_DetectorNbr(DetNbr);
  Set_Front_Upstream_StripNbr(StripNbr);
  Set_Front_Upstream_Energy(Energy);
  Set_Front_Upstream_Time(Time);
}

/////////////////////////
inline void TTiaraBarrelData::Set_Front_Downstreamm( const unsigned short& DetNbr , const unsigned short StripNbr , const double& Energy , const double& Time){

  Set_Front_Downstream_DetectorNbr(DetNbr);
  Set_Front_Downstream_StripNbr(StripNbr);
  Set_Front_Downstream_Energy(Energy);
  Set_Front_Downstream_Time(Time);
}

/////////////////////////
inline void TTiaraBarrelData::Set_Back( const unsigned short& DetNbr , const double& Energy , const double& Time){

  Set_Back_DetectorNbr(DetNbr);
  Set_Back_Energy(Energy);
  Set_Back_Time(Time);
}

/////////////////////////
inline void TTiaraBarrelData::Set_Outer( const unsigned short& DetNbr , const unsigned short StripNbr, const double& Energy , const double& Time){

  Set_Outer_DetectorNbr(DetNbr);
  Set_Outer_StripNbr(StripNbr);
  Set_Outer_Energy(Energy);
  Set_Outer_Time(Time);
}


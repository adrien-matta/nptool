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
}

/////////////////////////
TTiaraBarrelData::~TTiaraBarrelData(){
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
   fTiaraBarrel_Front_Dowstream_Time.clear();            

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
  cout << "************* TTiaraBarrel Event ************"<< endl;
  cout << "*** Inner Barrel Front Upstream ***"
    for( unsigned int i = 0; i < fTiaraBarrel_Front_Upstream_DetectorNbr.size(); i ++){
      cout << "Detector Number = " 
        << fTiaraBarrel_Front_Upstream_DetectorNbr << endl ;
      cout << "Strip Number = " 
        << fTiaraBarrel_Front_Upstream_StripNbr << endl;
      cout << "Energy = " 
        << fTiaraBarrel_Front_Upstream_Energy << endl ;
      cout << "Time = " 
        << fTiaraBarrel_Front_Upstream_Time << endl ;  
    }


  cout << "*** Inner Barrel Front Downstream ***"
    for( unsigned int i = 0; i < fTiaraBarrel_Front_Downstream_DetectorNbr.size(); i ++){
      cout << "Detector Number = " 
        << fTiaraBarrel_Front_Downstream_DetectorNbr << endl;  
      cout << "Strip Number = " 
        << fTiaraBarrel_Front_Downstream_StripNbr << endl;
      cout << "Energy = " 
        << fTiaraBarrel_Front_Downstream_Energy << endl ;
      cout << "Time = " 
        << fTiaraBarrel_Front_Downstream_Time << endl ;  
    }

  cout << "*** Inner Barrel Back ***"
    for( unsigned int i = 0; i < fTiaraBarrel_Back_DetectorNbr.size(); i ++){
      cout << "Detector Number = " 
        << fTiaraBarre_Back_DetectorNbr << endl;  
      cout << "Strip Number = " 
        << fTiaraBarrel_Back_StripNbr << endl;
      cout << "Energy = " 
        << fTiaraBarrel_Back_Energy << endl ;
      cout << "Time = " 
        << fTiaraBarrel_Back_Time << endl ;  
    }
  cout << "*** Outer Barrel ***"
    for( unsigned int i = 0; i < fTiaraBarrel_Outer_DetectorNbr.size() ; i ++){
      cout << "Detector Number = " 
        << fTiaraBarrel_Outer_DetectorNbr << endl;  
      cout << "Energy = " 
        << fTiaraBarrel_Outer_Energy << endl ;
      cout << "Time = " 
        << fTiaraBarrel_Outer_Time << endl ;  
    }
}

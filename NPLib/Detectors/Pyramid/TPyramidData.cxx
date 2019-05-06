/*****************************************************************************
 * Copyright (C) 2009-2011   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: mohamad.moukaddam@iphc.cnrs.fr                           *
 *                                                                           *
 * Creation Date  : November 2011                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Pyramid Raw data                                         *
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

#include "TPyramidData.h"

ClassImp(TPyramidData)
  /////////////////////////
  TPyramidData::TPyramidData(){

    // Reserve the space for the full array 
    fPyramid_FrontUpstreamE_DetectorNbr.reserve(1*4);         
    fPyramid_FrontUpstreamE_StripNbr.reserve(1*4);         
    fPyramid_FrontUpstreamE_Energy.reserve(1*4);        

    fPyramid_FrontDownstreamE_DetectorNbr.reserve(1*4);     
    fPyramid_FrontDownstreamE_StripNbr.reserve(1*4);           
    fPyramid_FrontDownstreamE_Energy.reserve(1*4);          

    fPyramid_BackE_DetectorNbr.reserve(1);                  
    fPyramid_BackE_Energy.reserve(1);                    

    fPyramid_OuterE_DetectorNbr.reserve(1*4);               
    fPyramid_OuterE_StripNbr.reserve(1*4);                    
    fPyramid_OuterE_Energy.reserve(1*4);                        
  
    fPyramid_FrontUpstreamT_DetectorNbr.reserve(1*4);         
    fPyramid_FrontUpstreamT_StripNbr.reserve(1*4);         
    fPyramid_FrontUpstreamT_Time.reserve(1*4);          

    fPyramid_FrontDownstreamT_DetectorNbr.reserve(1*4);     
    fPyramid_FrontDownstreamT_StripNbr.reserve(1*4);           
    fPyramid_FrontDownstreamT_Time.reserve(1*4);            

    fPyramid_BackT_DetectorNbr.reserve(1);                  
    fPyramid_BackT_Time.reserve(1);                      

    fPyramid_OuterT_DetectorNbr.reserve(1*4);               
    fPyramid_OuterT_StripNbr.reserve(1*4);                    
    fPyramid_OuterT_Time.reserve(1*4);


}

/////////////////////////
TPyramidData::~TPyramidData(){
  Clear();
}

/////////////////////////
void TPyramidData::Clear(){
  fPyramid_FrontUpstreamE_DetectorNbr.clear();         
  fPyramid_FrontUpstreamE_StripNbr.clear();         
  fPyramid_FrontUpstreamE_Energy.clear();        

  fPyramid_FrontDownstreamE_DetectorNbr.clear();     
  fPyramid_FrontDownstreamE_StripNbr.clear();           
  fPyramid_FrontDownstreamE_Energy.clear();          

  fPyramid_BackE_DetectorNbr.clear();                  
  fPyramid_BackE_Energy.clear();                    

  fPyramid_OuterE_DetectorNbr.clear();               
  fPyramid_OuterE_StripNbr.clear();                    
  fPyramid_OuterE_Energy.clear();                        
  
  fPyramid_FrontUpstreamT_DetectorNbr.clear();        
  fPyramid_FrontUpstreamT_StripNbr.clear();         
  fPyramid_FrontUpstreamT_Time.clear();          

  fPyramid_FrontDownstreamT_DetectorNbr.clear();     
  fPyramid_FrontDownstreamT_StripNbr.clear();           
  fPyramid_FrontDownstreamT_Time.clear();            

  fPyramid_BackT_DetectorNbr.clear();                  
  fPyramid_BackT_Time.clear();                      

  fPyramid_OuterT_DetectorNbr.clear();               
  fPyramid_OuterT_StripNbr.clear();                    
  fPyramid_OuterT_Time.clear();



}

/////////////////////////
void TPyramidData::Dump() const{
  cout << "************* TPyramidData Event ************"<< endl;
  cout << "*** Inner Barrel Front Upstream ***" << endl ;
  for( unsigned int i = 0; i < fPyramid_FrontUpstreamE_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fPyramid_FrontUpstreamE_DetectorNbr[i] << " | " ;
    cout << "Strip Number = " 
      << fPyramid_FrontUpstreamE_StripNbr[i] << " | ";
    cout << "Energy = " 
      << fPyramid_FrontUpstreamE_Energy[i]<< endl ;
  }
  
  for( unsigned int i = 0; i < fPyramid_FrontUpstreamT_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fPyramid_FrontUpstreamT_DetectorNbr[i] << " | " ;
    cout << "Strip Number = " 
      << fPyramid_FrontUpstreamT_StripNbr[i] << " | ";
    cout << "Time = " 
      << fPyramid_FrontUpstreamT_Time[i] << endl ;  
  }


  cout << "*** Inner Barrel Front Downstream ***" << endl ;
  for( unsigned int i = 0; i < fPyramid_FrontDownstreamE_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fPyramid_FrontDownstreamE_DetectorNbr[i] << " | ";  
    cout << "Strip Number = " 
      << fPyramid_FrontDownstreamE_StripNbr[i] << " | ";
    cout << "Energy = " 
      << fPyramid_FrontDownstreamE_Energy[i] << endl ;
  }

  for( unsigned int i = 0; i < fPyramid_FrontDownstreamT_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fPyramid_FrontDownstreamT_DetectorNbr[i] << " | ";  
    cout << "Strip Number = " 
      << fPyramid_FrontDownstreamT_StripNbr[i] << " | ";
    cout << "Time = " 
      << fPyramid_FrontDownstreamT_Time[i] << endl ;  
  }

  cout << "*** Inner Barrel Back ***" << endl ;
  for( unsigned int i = 0; i < fPyramid_BackE_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fPyramid_BackE_DetectorNbr[i] << " | ";  
    cout << "Energy = " 
      << fPyramid_BackE_Energy[i] << endl ;
  }

  for( unsigned int i = 0; i < fPyramid_BackT_DetectorNbr.size(); i ++){
    cout << "Detector Number = " 
      << fPyramid_BackT_DetectorNbr[i] << " | ";  
    cout << "Time = " 
      << fPyramid_BackT_Time[i] << endl ;  
  }

  cout << "*** Outer Barrel ***" << endl;
  for( unsigned int i = 0; i < fPyramid_OuterE_DetectorNbr.size() ; i ++){
    cout << "Detector Number = " 
      << fPyramid_OuterE_DetectorNbr[i] << " | ";  
    cout << "Energy = " 
      << fPyramid_OuterE_Energy[i] << endl ;
  }

  for( unsigned int i = 0; i < fPyramid_OuterT_DetectorNbr.size() ; i ++){
    cout << "Detector Number = " 
      << fPyramid_OuterT_DetectorNbr[i] << " | ";  
    cout << "Time = " 
      << fPyramid_OuterT_Time[i] << endl ;  
  }
}


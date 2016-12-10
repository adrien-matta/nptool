/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : December 2013                                            *
 *																			 *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Charissa treated data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TCharissaPhysics.h"
using namespace CHARISSA_LOCAL;

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPOptionManager.h"
#include "NPDetectorFactory.h"

//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TCharissaPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TCharissaPhysics::TCharissaPhysics(){
    EventMultiplicity   = 0 ;
    m_EventData         = new TCharissaData ;
    m_PreTreatedData    = new TCharissaData ;
    m_EventPhysics      = this ;
    m_Spectra           = NULL;
    m_NumberOfTelescope = 0 ;
    m_MaximumStripMultiplicityAllowed = 100;
    m_StripEnergyMatchingSigma = 0.1    ;
    m_StripEnergyMatchingNumberOfSigma = 300;
    // Raw Threshold
    m_Si_X_E_RAW_Threshold = 1500 ;
    m_Si_Y_E_RAW_Threshold = 1500 ;
    m_CsI_E_RAW_Threshold  = 0 ;
    // Calibrated Threshold
    m_Si_X_E_Threshold = 0 ;
    m_Si_Y_E_Threshold = 0 ;
    m_CsI_E_Threshold  = 0;
    m_NumberOfStrip = 16;

    m_Take_E_Y=false;
    m_Take_T_Y=true;

  }



///////////////////////////////////////////////////////////////////////////
TCharissaPhysics::~TCharissaPhysics(){

}
///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////

void TCharissaPhysics::BuildPhysicalEvent(){
  
  PreTreat();
  
  // Layer 1
  vector< TVector2 > Layer1_couple = Layer1_Match_X_Y() ;
  vector< TVector2 > Layer2_couple = Layer2_Match_X_Y() ;
  
  // Check out the different mult needed later on
  m_Layer1_StripXEMult = m_PreTreatedData->GetCharissaLayer1StripXEMult();
  m_Layer1_StripYEMult = m_PreTreatedData->GetCharissaLayer1StripYEMult();
  m_Layer1_StripXTMult = m_PreTreatedData->GetCharissaLayer1StripXTMult();
  m_Layer1_StripYTMult = m_PreTreatedData->GetCharissaLayer1StripYTMult();
  m_Layer2_StripXEMult = m_PreTreatedData->GetCharissaLayer2StripXEMult();
  m_Layer2_StripYEMult = m_PreTreatedData->GetCharissaLayer2StripYEMult();
  m_Layer2_StripXTMult = m_PreTreatedData->GetCharissaLayer2StripXTMult();
  m_Layer2_StripYTMult = m_PreTreatedData->GetCharissaLayer2StripYTMult();
  m_CsIEMult = m_PreTreatedData->GetCharissaCsIEMult();
  m_CsITMult = m_PreTreatedData->GetCharissaCsITMult();

  // Case were Layer 1 has been fired
  if(Layer1_couple.size()>0){

    for(unsigned int i = 0 ; i < Layer1_couple.size() ; ++i){
      int Layer1_N = m_PreTreatedData->GetCharissaLayer1StripXEDetectorNbr(Layer1_couple[i].X()) ;
      int Layer1_X = m_PreTreatedData->GetCharissaLayer1StripXEStripNbr(Layer1_couple[i].X()) ;
      int Layer1_Y = m_PreTreatedData->GetCharissaLayer1StripYEStripNbr(Layer1_couple[i].Y()) ;
      
      double Layer1_Si_X_E = m_PreTreatedData->GetCharissaLayer1StripXEEnergy( Layer1_couple[i].X() ) ;
      double Layer1_Si_Y_E = m_PreTreatedData->GetCharissaLayer1StripYEEnergy( Layer1_couple[i].Y() ) ;

      //  Search for associate Time
      double Layer1_Si_X_T = -1000 ;

      for(unsigned int t = 0 ; t < m_Layer1_StripXTMult ; ++t ){
        if(  m_PreTreatedData->GetCharissaLayer1StripXTStripNbr( Layer1_couple[i].X() ) == m_PreTreatedData->GetCharissaLayer1StripXTStripNbr(t)
            ||m_PreTreatedData->GetCharissaLayer1StripXTDetectorNbr( Layer1_couple[i].X() ) == m_PreTreatedData->GetCharissaLayer1StripXTDetectorNbr(t))
          Layer1_Si_X_T = m_PreTreatedData->GetCharissaLayer1StripXTTime(t);
      }

      double Layer1_Si_Y_T = -1000 ;

      for(unsigned int t = 0 ; t < m_Layer1_StripYTMult ; ++t ){
        if(  m_PreTreatedData->GetCharissaLayer1StripYTStripNbr( Layer1_couple[i].Y() ) == m_PreTreatedData->GetCharissaLayer1StripYTStripNbr(t)
            ||m_PreTreatedData->GetCharissaLayer1StripYTDetectorNbr( Layer1_couple[i].Y() ) == m_PreTreatedData->GetCharissaLayer1StripYTDetectorNbr(t))
          Layer1_Si_Y_T = m_PreTreatedData->GetCharissaLayer1StripYTTime(t);
      }

      Layer1_Si_X.push_back(Layer1_X) ; Layer1_Si_Y.push_back(Layer1_Y) ; TelescopeNumber.push_back(Layer1_N) ;

      if(m_Take_E_Y) Layer1_Si_E.push_back(Layer1_Si_Y_E);
      else           Layer1_Si_E.push_back(Layer1_Si_X_E);

      if(m_Take_T_Y) Layer1_Si_T.push_back(Layer1_Si_Y_T);
      else           Layer1_Si_T.push_back(Layer1_Si_X_T);

      // Store the other value for checking purpose
      Layer1_Si_EX.push_back(Layer1_Si_X_E);            
      Layer1_Si_TX.push_back(Layer1_Si_X_T);            
      Layer1_Si_EY.push_back(Layer1_Si_Y_E);            
      Layer1_Si_TY.push_back(Layer1_Si_Y_T); 

      // Look for associate layer 2 
      double tLayer2_X = -1000;
      double tLayer2_Y= -1000;
      double tLayer2_Si_X_E= -1000;
      double tLayer2_Si_Y_E= -1000;
      double tLayer2_Si_X_T= -1000;
      double tLayer2_Si_Y_T= -1000;
//      double tLayer2_Si_E= -1000;
//      double tLayer2_Si_T = -1000;
      
      for(unsigned int ll = 0 ; ll < Layer2_couple.size() ; ++ll){	
        int Layer2_N = m_PreTreatedData->GetCharissaLayer2StripXEDetectorNbr(Layer2_couple[ll].X()) ;
        // Same detector
        // This requierement should maybe stronger (same strip number?)
        if(Layer2_N==Layer1_N){
          tLayer2_X = m_PreTreatedData->GetCharissaLayer2StripXEStripNbr(Layer2_couple[ll].X()) ;
          tLayer2_Y = m_PreTreatedData->GetCharissaLayer2StripYEStripNbr(Layer2_couple[ll].Y()) ;

          tLayer2_Si_X_E = m_PreTreatedData->GetCharissaLayer2StripXEEnergy( Layer2_couple[ll].X() ) ;
          tLayer2_Si_Y_E = m_PreTreatedData->GetCharissaLayer2StripYEEnergy( Layer2_couple[ll].Y() ) ;

          //  Search for associate Time
          tLayer2_Si_X_T = -1000 ;

          for(unsigned int t = 0 ; t < m_Layer2_StripXTMult ; ++t ){
            if(  m_PreTreatedData->GetCharissaLayer2StripXTStripNbr( Layer2_couple[ll].X() ) == m_PreTreatedData->GetCharissaLayer2StripXTStripNbr(t)
                ||m_PreTreatedData->GetCharissaLayer2StripXTDetectorNbr( Layer2_couple[ll].X() ) == m_PreTreatedData->GetCharissaLayer2StripXTDetectorNbr(t))
              tLayer2_Si_X_T = m_PreTreatedData->GetCharissaLayer2StripXTTime(t);
          }

          tLayer2_Si_Y_T = -1000 ;

          for(unsigned int t = 0 ; t < m_Layer2_StripYTMult ; ++t ){
            if(  m_PreTreatedData->GetCharissaLayer2StripYTStripNbr( Layer2_couple[ll].Y() ) == m_PreTreatedData->GetCharissaLayer2StripYTStripNbr(t)
                ||m_PreTreatedData->GetCharissaLayer2StripYTDetectorNbr( Layer2_couple[ll].Y() ) == m_PreTreatedData->GetCharissaLayer2StripYTDetectorNbr(t))
              tLayer2_Si_Y_T = m_PreTreatedData->GetCharissaLayer2StripYTTime(t);
          }
        }
      }

      Layer2_Si_EX.push_back(tLayer2_Si_X_E);            
      Layer2_Si_TX.push_back(tLayer2_Si_X_T);            
      Layer2_Si_EY.push_back(tLayer2_Si_Y_E);            
      Layer2_Si_TY.push_back(tLayer2_Si_Y_T); 
      Layer2_Si_X.push_back(tLayer2_X) ; 
      Layer2_Si_Y.push_back(tLayer2_Y) ; 
      
      if(m_Take_E_Y) Layer2_Si_E.push_back(tLayer2_Si_Y_E);
      else           Layer2_Si_E.push_back(tLayer2_Si_X_E);

      if(m_Take_T_Y) Layer2_Si_T.push_back(tLayer2_Si_Y_T);
      else           Layer2_Si_T.push_back(tLayer2_Si_X_T);

      double CsIE = -1000;
      int CsIN = -1000;
      
      // Look for an associate CsI E
      for(unsigned int t = 0 ; t < m_CsIEMult ; ++t ){
        if(m_PreTreatedData->GetCharissaLayer1StripXEDetectorNbr( Layer1_couple[i].X() ) == m_PreTreatedData->GetCharissaCsIEDetectorNbr(t)){
          CsIE = m_PreTreatedData->GetCharissaCsIEEnergy(t);
          CsIN = m_PreTreatedData->GetCharissaCsIECristalNbr(t);
        }
      }
      double CsIT = -1000;
      // Look for an associate CsI T
      for(unsigned int t = 0 ; t < m_CsITMult ; ++t ){
          if(m_PreTreatedData->GetCharissaLayer1StripXEDetectorNbr( Layer1_couple[i].X() ) == m_PreTreatedData->GetCharissaCsITDetectorNbr(t))
            CsIT = m_PreTreatedData->GetCharissaCsITTime(t);
      }
      
      if(CsIE>-1000){
        CsI_E.push_back(CsIE);
        CsI_N.push_back(CsIN);
        CsI_T.push_back(CsIT); 
      }
      else{
        CsI_E.push_back(-1000);
        CsI_N.push_back(-1000);
        CsI_T.push_back(-1000); 
      }
    }

  }

  // Case were Layer 1 has not been fired (then layer2 take main role)
  else if(Layer2_couple.size()>0){
    for(unsigned int i = 0 ; i < Layer2_couple.size() ; ++i){
      int Layer2_N = m_PreTreatedData->GetCharissaLayer2StripXEDetectorNbr(Layer2_couple[i].X()) ;
      int Layer2_X = m_PreTreatedData->GetCharissaLayer2StripXEStripNbr(Layer2_couple[i].X()) ;
      int Layer2_Y = m_PreTreatedData->GetCharissaLayer2StripYEStripNbr(Layer2_couple[i].Y()) ;

      double Layer2_Si_X_E = m_PreTreatedData->GetCharissaLayer2StripXEEnergy( Layer2_couple[i].X() ) ;
      double Layer2_Si_Y_E = m_PreTreatedData->GetCharissaLayer2StripYEEnergy( Layer2_couple[i].Y() ) ;

      //  Search for associate Time
      double Layer2_Si_X_T = -1000 ;

      for(unsigned int t = 0 ; t < m_Layer2_StripXTMult ; ++t ){
        if(  m_PreTreatedData->GetCharissaLayer2StripXTStripNbr( Layer2_couple[i].X() ) == m_PreTreatedData->GetCharissaLayer2StripXTStripNbr(t)
            ||m_PreTreatedData->GetCharissaLayer2StripXTDetectorNbr( Layer2_couple[i].X() ) == m_PreTreatedData->GetCharissaLayer2StripXTDetectorNbr(t))
          Layer2_Si_X_T = m_PreTreatedData->GetCharissaLayer2StripXTTime(t);
      }

      double Layer2_Si_Y_T = -1000 ;

      for(unsigned int t = 0 ; t < m_Layer2_StripYTMult ; ++t ){
        if(  m_PreTreatedData->GetCharissaLayer2StripYTStripNbr( Layer2_couple[i].Y() ) == m_PreTreatedData->GetCharissaLayer2StripYTStripNbr(t)
            ||m_PreTreatedData->GetCharissaLayer2StripYTDetectorNbr( Layer2_couple[i].Y() ) == m_PreTreatedData->GetCharissaLayer2StripYTDetectorNbr(t))
          Layer2_Si_Y_T = m_PreTreatedData->GetCharissaLayer2StripYTTime(t);
      }

      Layer2_Si_X.push_back(Layer2_X) ; Layer2_Si_Y.push_back(Layer2_Y) ; TelescopeNumber.push_back(Layer2_N) ;

      if(m_Take_E_Y) Layer2_Si_E.push_back(Layer2_Si_Y_E);
      else           Layer2_Si_E.push_back(Layer2_Si_X_E);

      if(m_Take_T_Y) Layer2_Si_T.push_back(Layer2_Si_Y_T);
      else           Layer2_Si_T.push_back(Layer2_Si_X_T);

      // Store the other value for checking purpose
      Layer2_Si_EX.push_back(Layer2_Si_X_E);            
      Layer2_Si_TX.push_back(Layer2_Si_X_T);            
      Layer2_Si_EY.push_back(Layer2_Si_Y_E);            
      Layer2_Si_TY.push_back(Layer2_Si_Y_T); 

      double CsIE = -1000;
      int CsIN = -1000;
      // Look for an associate CsI E
      for(unsigned int t = 0 ; t < m_CsIEMult ; ++t ){
        if(m_PreTreatedData->GetCharissaLayer2StripXEDetectorNbr( Layer2_couple[i].X() ) == m_PreTreatedData->GetCharissaCsIEDetectorNbr(t)){
          CsIE = m_PreTreatedData->GetCharissaCsIEEnergy(t);
          CsIN = m_PreTreatedData->GetCharissaCsIECristalNbr(t);
        }
      }


      double CsIT = -1000;
      // Look for an associate CsI T
      for(unsigned int t = 0 ; t < m_CsITMult ; ++t ){
          if(m_PreTreatedData->GetCharissaLayer2StripXEDetectorNbr( Layer2_couple[i].X() ) == m_PreTreatedData->GetCharissaCsITDetectorNbr(t))
            CsIT = m_PreTreatedData->GetCharissaCsITTime(t);
      }
      

      if(CsIE>-1000){
        CsI_E.push_back(CsIE);
        CsI_N.push_back(CsIN);
        CsI_T.push_back(CsIT); 
      }
      else{
        CsI_E.push_back(-1000);
        CsI_N.push_back(-1000);
        CsI_T.push_back(-1000); 
      }

      Layer1_Si_EX.push_back(-1000);            
      Layer1_Si_TX.push_back(-1000);            
      Layer1_Si_EY.push_back(-1000);            
      Layer1_Si_TY.push_back(-1000); 
      Layer1_Si_X.push_back(-1000) ; 
      Layer1_Si_Y.push_back(-1000) ; 
      Layer1_Si_E.push_back(-1000);
      Layer1_Si_T.push_back(-1000);
    }

  }
EventMultiplicity = Layer1_Si_E.size();
  return;

}

///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::PreTreat(){

  ClearPreTreatedData();
  m_Layer1_StripXEMult = m_EventData->GetCharissaLayer1StripXEMult();
  m_Layer1_StripYEMult = m_EventData->GetCharissaLayer1StripYEMult();
  m_Layer1_StripXTMult = m_EventData->GetCharissaLayer1StripXTMult();
  m_Layer1_StripYTMult = m_EventData->GetCharissaLayer1StripYTMult();

  m_Layer2_StripXEMult = m_EventData->GetCharissaLayer2StripXEMult();
  m_Layer2_StripYEMult = m_EventData->GetCharissaLayer2StripYEMult();
  m_Layer2_StripXTMult = m_EventData->GetCharissaLayer2StripXTMult();
  m_Layer2_StripYTMult = m_EventData->GetCharissaLayer2StripYTMult();

  m_CsIEMult = m_EventData->GetCharissaCsIEMult();
  m_CsITMult = m_EventData->GetCharissaCsITMult();

  //   X Layer1
  //   E
  for(unsigned int i = 0 ; i < m_Layer1_StripXEMult ; ++i){
    if( m_EventData->GetCharissaLayer1StripXEEnergy(i)>m_Si_X_E_RAW_Threshold){
      double Layer1_EX = fDE_X_E(m_EventData , i);
      if( Layer1_EX > m_Si_X_E_Threshold ){
        m_PreTreatedData->SetCharissaLayer1StripXEDetectorNbr( m_EventData->GetCharissaLayer1StripXEDetectorNbr(i) );
        m_PreTreatedData->SetCharissaLayer1StripXEStripNbr( m_EventData->GetCharissaLayer1StripXEStripNbr(i) );
        m_PreTreatedData->SetCharissaLayer1StripXEEnergy( Layer1_EX );
      }
    }
  }

  //   T
  for(unsigned int i = 0 ; i < m_Layer1_StripXTMult ; ++i){
    if(IsValidChannel("X", m_EventData->GetCharissaLayer1StripXTDetectorNbr(i), m_EventData->GetCharissaLayer1StripXTStripNbr(i))){
      m_PreTreatedData->SetCharissaLayer1StripXTDetectorNbr( m_EventData->GetCharissaLayer1StripXTDetectorNbr(i) );
      m_PreTreatedData->SetCharissaLayer1StripXTStripNbr( m_EventData->GetCharissaLayer1StripXTStripNbr(i) );
      m_PreTreatedData->SetCharissaLayer1StripXTTime( fDE_X_T(m_EventData , i) );
    }
  }

  //   Y  Layer1
  //   E
  for(unsigned int i = 0 ; i < m_Layer1_StripYEMult ; ++i){
    if( m_EventData->GetCharissaLayer1StripYEEnergy(i)>m_Si_Y_E_RAW_Threshold){
      double Layer1_EY = fDE_Y_E(m_EventData , i);
      if( Layer1_EY >m_Si_Y_E_Threshold ){
        m_PreTreatedData->SetCharissaLayer1StripYEDetectorNbr( m_EventData->GetCharissaLayer1StripYEDetectorNbr(i) );
        m_PreTreatedData->SetCharissaLayer1StripYEStripNbr( m_EventData->GetCharissaLayer1StripYEStripNbr(i) );
        m_PreTreatedData->SetCharissaLayer1StripYEEnergy( Layer1_EY );
      }
    }
  }

  //   T
  for(unsigned int i = 0 ; i < m_Layer1_StripYTMult ; ++i){
    if(IsValidChannel("Y", m_EventData->GetCharissaLayer1StripYTDetectorNbr(i), m_EventData->GetCharissaLayer1StripYTStripNbr(i))){
      m_PreTreatedData->SetCharissaLayer1StripYTDetectorNbr( m_EventData->GetCharissaLayer1StripYTDetectorNbr(i) );
      m_PreTreatedData->SetCharissaLayer1StripYTStripNbr( m_EventData->GetCharissaLayer1StripYTStripNbr(i) );
      m_PreTreatedData->SetCharissaLayer1StripYTTime( fDE_Y_T(m_EventData , i) );
    }
  }


  //   X Layer2
  //   E
  for(unsigned int i = 0 ; i < m_Layer2_StripXEMult ; ++i){
    if( m_EventData->GetCharissaLayer2StripXEEnergy(i)>m_Si_X_E_RAW_Threshold){
      double Layer2_EX = fE_X_E(m_EventData , i);
      if( Layer2_EX > m_Si_X_E_Threshold ){

        m_PreTreatedData->SetCharissaLayer2StripXEDetectorNbr( m_EventData->GetCharissaLayer2StripXEDetectorNbr(i) );
        m_PreTreatedData->SetCharissaLayer2StripXEStripNbr( m_EventData->GetCharissaLayer2StripXEStripNbr(i) );
        m_PreTreatedData->SetCharissaLayer2StripXEEnergy( Layer2_EX );
      }
    }
  }


  //   T
  for(unsigned int i = 0 ; i < m_Layer2_StripXTMult ; ++i){
    if(IsValidChannel("X", m_EventData->GetCharissaLayer2StripXTDetectorNbr(i), m_EventData->GetCharissaLayer2StripXTStripNbr(i))){
      m_PreTreatedData->SetCharissaLayer2StripXTDetectorNbr( m_EventData->GetCharissaLayer2StripXTDetectorNbr(i) );
      m_PreTreatedData->SetCharissaLayer2StripXTStripNbr( m_EventData->GetCharissaLayer2StripXTStripNbr(i) );
      m_PreTreatedData->SetCharissaLayer2StripXTTime( fE_X_T(m_EventData , i) );
    }
  }

  //   Y  Layer2
  //   E
  for(unsigned int i = 0 ; i < m_Layer2_StripYEMult ; ++i){
    if( m_EventData->GetCharissaLayer2StripYEEnergy(i)>m_Si_Y_E_RAW_Threshold){
      double Layer2_EY = fE_Y_E(m_EventData , i);
      if( Layer2_EY >m_Si_Y_E_Threshold ){
        m_PreTreatedData->SetCharissaLayer2StripYEDetectorNbr( m_EventData->GetCharissaLayer2StripYEDetectorNbr(i) );
        m_PreTreatedData->SetCharissaLayer2StripYEStripNbr( m_EventData->GetCharissaLayer2StripYEStripNbr(i) );
        m_PreTreatedData->SetCharissaLayer2StripYEEnergy( Layer2_EY );
      }
    }
  }

  //   T
  for(unsigned int i = 0 ; i < m_Layer2_StripYTMult ; ++i){
    if( IsValidChannel("Y", m_EventData->GetCharissaLayer2StripYTDetectorNbr(i), m_EventData->GetCharissaLayer1StripYTStripNbr(i))){
      m_PreTreatedData->SetCharissaLayer2StripYTDetectorNbr( m_EventData->GetCharissaLayer2StripYTDetectorNbr(i) );
      m_PreTreatedData->SetCharissaLayer2StripYTStripNbr( m_EventData->GetCharissaLayer2StripYTStripNbr(i) );
      m_PreTreatedData->SetCharissaLayer2StripYTTime( fE_Y_T(m_EventData,i) );
    }
  }

  //   CsI
  //   E
  for(unsigned int i = 0 ; i < m_CsIEMult ; ++i){
    if( m_EventData->GetCharissaCsIEEnergy(i)>m_CsI_E_RAW_Threshold && IsValidChannel("CsI", m_EventData->GetCharissaCsIEDetectorNbr(i), m_EventData->GetCharissaCsIECristalNbr(i))){
      double ECsI = fCsI_E(m_EventData , i);
      if( ECsI > m_CsI_E_Threshold ){
        m_PreTreatedData->SetCharissaCsIEDetectorNbr( m_EventData->GetCharissaCsIEDetectorNbr(i) );
        m_PreTreatedData->SetCharissaCsIECristalNbr( m_EventData->GetCharissaCsIECristalNbr(i) );
        m_PreTreatedData->SetCharissaCsIEEnergy( ECsI );
      }
    }
  }

  //   T
  for(unsigned int i = 0 ; i < m_CsITMult ; ++i){
    if(IsValidChannel("CsI", m_EventData->GetCharissaCsITDetectorNbr(i), m_EventData->GetCharissaCsITCristalNbr(i))){
      m_PreTreatedData->SetCharissaCsITDetectorNbr( m_EventData->GetCharissaCsITDetectorNbr(i) );
      m_PreTreatedData->SetCharissaCsITCristalNbr( m_EventData->GetCharissaCsITCristalNbr(i) );
      m_PreTreatedData->SetCharissaCsITTime( fCsI_T(m_EventData , i) );
    }
  }

  return;
}


///////////////////////////////////////////////////////////////////////////
int TCharissaPhysics :: Layer1_CheckEvent(){
  return 1;
}

///////////////////////////////////////////////////////////////////////////
int TCharissaPhysics :: Layer2_CheckEvent(){
  return 1 ; 

}

///////////////////////////////////////////////////////////////////////////
bool TCharissaPhysics :: ResolvePseudoEvent(){
  return false;
}

///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TCharissaPhysics :: Layer1_Match_X_Y(){
  vector < TVector2 > ArrayOfGoodCouple ;
  m_Layer1_StripXEMult = m_PreTreatedData->GetCharissaLayer1StripXEMult();
  m_Layer1_StripYEMult = m_PreTreatedData->GetCharissaLayer1StripYEMult();

  // Prevent code from treating very high multiplicity Event
  // Those event are not physical anyway and that improve speed.
  if( m_Layer1_StripXEMult > m_MaximumStripMultiplicityAllowed || m_Layer1_StripYEMult > m_MaximumStripMultiplicityAllowed )
    return ArrayOfGoodCouple;

  for(unsigned int i = 0 ; i < m_Layer1_StripXEMult ; ++i){
    for(unsigned int j = 0 ; j < m_Layer1_StripYEMult ; ++j){
      //   if same detector check energy
      if ( m_PreTreatedData->GetCharissaLayer1StripXEDetectorNbr(i) == m_PreTreatedData->GetCharissaLayer1StripYEDetectorNbr(j) ){
        //   Look if energy match
        if( abs( (m_PreTreatedData->GetCharissaLayer1StripXEEnergy(i)-m_PreTreatedData->GetCharissaLayer1StripYEEnergy(j))/2. ) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma )
          ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;
      }
    }
  }

  //   Prevent to treat event with ambiguous matching beetween X and Y
  if( ArrayOfGoodCouple.size() > m_Layer1_StripXEMult ) ArrayOfGoodCouple.clear() ;

  return ArrayOfGoodCouple;
}

///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TCharissaPhysics :: Layer2_Match_X_Y(){
  vector < TVector2 > ArrayOfGoodCouple ;
  m_Layer2_StripXEMult = m_PreTreatedData->GetCharissaLayer2StripXEMult();
  m_Layer2_StripYEMult = m_PreTreatedData->GetCharissaLayer2StripYEMult();

  // Prevent code from treating very high multiplicity Event
  // Those event are not physical anyway and that improve speed.
  if( m_Layer2_StripXEMult > m_MaximumStripMultiplicityAllowed || m_Layer2_StripYEMult > m_MaximumStripMultiplicityAllowed )
    return ArrayOfGoodCouple;

  for(unsigned int i = 0 ; i < m_Layer2_StripXEMult ; ++i){
    for(unsigned int j = 0 ; j < m_Layer2_StripYEMult ; ++j){
      //   if same detector check energy
      if ( m_PreTreatedData->GetCharissaLayer2StripXEDetectorNbr(i) == m_PreTreatedData->GetCharissaLayer2StripYEDetectorNbr(j) ){
        //   Look if energy match
        if( abs( (m_PreTreatedData->GetCharissaLayer2StripXEEnergy(i)-m_PreTreatedData->GetCharissaLayer2StripYEEnergy(j))/2. ) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma )
          ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;
      }
    }
  }

  //   Prevent to treat event with ambiguous matching beetween X and Y
  if( ArrayOfGoodCouple.size() > m_Layer2_StripXEMult ) ArrayOfGoodCouple.clear() ;

  return ArrayOfGoodCouple;
}





////////////////////////////////////////////////////////////////////////////
bool TCharissaPhysics :: IsValidChannel(const string DetectorType, const int telescope , const int channel){
  if(DetectorType == "CsI")
    return *(m_CsIChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "X")
    return *(m_XChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "Y")
    return *(m_YChannelStatus[telescope-1].begin()+channel-1);

  else return false;
}

///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::ReadAnalysisConfig(){
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigCharissa.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigCharissa.dat found: Default parameters loaded for Analysis " << FileName << endl;
    return;
  }
  cout << " Loading user parameters for Analysis from ConfigCharissa.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigCharissaCHARISSA.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 11, "ConfigCharissa") == 0) ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus ) {

      whatToDo="";
      AnalysisConfigFile >> whatToDo;

      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
      }

      else if (whatToDo=="MAX_STRIP_MULTIPLICITY") {
        AnalysisConfigFile >> DataBuffer;
        m_MaximumStripMultiplicityAllowed = atoi(DataBuffer.c_str() );
        cout << "MAXIMUN STRIP MULTIPLICITY " << m_MaximumStripMultiplicityAllowed << endl;
      }

      else if (whatToDo=="STRIP_ENERGY_MATCHING_SIGMA") {
        AnalysisConfigFile >> DataBuffer;
        m_StripEnergyMatchingSigma = atof(DataBuffer.c_str() );
        cout << "STRIP ENERGY MATCHING SIGMA " << m_StripEnergyMatchingSigma <<endl;
      }

      else if (whatToDo=="STRIP_ENERGY_MATCHING_NUMBER_OF_SIGMA") {
        AnalysisConfigFile >> DataBuffer;
        m_StripEnergyMatchingNumberOfSigma = atoi(DataBuffer.c_str() );
        cout << "STRIP ENERGY MATCHING NUMBER OF SIGMA " << m_StripEnergyMatchingNumberOfSigma << endl;
      }

      else if (whatToDo== "DISABLE_ALL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        int telescope = atoi(DataBuffer.substr(2,1).c_str());
        vector< bool > ChannelStatus;
        ChannelStatus.resize( m_NumberOfStrip,false);
        m_XChannelStatus[telescope-1] = ChannelStatus;
        m_YChannelStatus[telescope-1] = ChannelStatus;
        ChannelStatus.resize(m_NumberOfStrip,false);
        m_CsIChannelStatus[telescope-1]  = ChannelStatus;
      }

      else if (whatToDo == "DISABLE_CHANNEL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        int telescope = atoi(DataBuffer.substr(2,1).c_str());
        int channel = -1;
        if (DataBuffer.compare(3,4,"STRX") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_XChannelStatus[telescope-1].begin()+channel-1) = false;
        }

        else if (DataBuffer.compare(3,4,"STRY") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_YChannelStatus[telescope-1].begin()+channel-1) = false;
        }


        else if (DataBuffer.compare(3,3,"CSI") == 0) {
          channel = atoi(DataBuffer.substr(6).c_str());
          *(m_CsIChannelStatus[telescope-1].begin()+channel-1) = false;
        }

        else cout << "Warning: detector type for CharissaCHARISSA unknown!" << endl;

      }

      else if (whatToDo=="TAKE_E_Y") {
        m_Take_E_Y = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_T_Y") {
        m_Take_T_Y = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_E_X") {
        m_Take_E_Y = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_T_X") {
        m_Take_T_Y = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo== "IGNORE_NOT_MATCHING_CSI") {
        m_Ignore_not_matching_CsI = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="CSI_SIZE") {
        AnalysisConfigFile >> DataBuffer;
        m_CsI_Size = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_CsI_Size << endl;
      }


      else if (whatToDo=="SI_X_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Si_X_E_RAW_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_Si_X_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="SI_Y_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Si_Y_E_RAW_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_Si_Y_E_RAW_Threshold << endl;
      }


      else if (whatToDo== "CSI_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_CsI_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_CsI_E_Threshold << endl;
      }

      else if (whatToDo=="SI_X_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Si_X_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_Si_X_E_Threshold << endl;
      }

      else if (whatToDo== "SI_Y_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Si_Y_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_Si_Y_E_Threshold << endl;
      }


      else if (whatToDo=="CSI_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_CsI_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_CsI_E_Threshold << endl;
      }

      else {
        ReadingStatus = false;
      }

    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::Clear(){
  EventMultiplicity= 0 ;
  TelescopeNumber.clear();
  // Si X
  Layer1_Si_E.clear();
  Layer1_Si_T.clear();
  Layer1_Si_X.clear();
  Layer1_Si_Y.clear();
  Layer2_Si_E.clear();
  Layer2_Si_T.clear();
  Layer2_Si_X.clear();
  Layer2_Si_Y.clear();

  // CsI
  CsI_E.clear();
  CsI_T.clear();
  CsI_N.clear();

  Layer1_Si_EX.clear();
  Layer1_Si_TX.clear();
  Layer1_Si_EY.clear();
  Layer1_Si_TY.clear();
  Layer2_Si_EX.clear();
  Layer2_Si_TX.clear();
  Layer2_Si_EY.clear();
  Layer2_Si_TY.clear();
}


////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("CharissaTelescope");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " Telescope found " << endl; 
  for(unsigned int i  = 0 ; i < blocks.size() ; i++){
    // Cartesian Case
    vector<string> cart = {"X1_Y1","X1_Y16","X16_Y1","X16_Y16","SI","SILI","CSI"};
    // Spherical Case
    vector<string> sphe= {"R","THETA","PHI","BETA","SI","SILI","CSI"};

    if(blocks[i]->HasTokenList(cart)){
      cout << endl << "////  Charissa telecope " << i+1 <<  endl;
      TVector3 A = blocks[i]->GetTVector3("X1_Y1","mm");
      TVector3 B = blocks[i]->GetTVector3("X16_Y1","mm");
      TVector3 C = blocks[i]->GetTVector3("X1_Y16","mm");
      TVector3 D = blocks[i]->GetTVector3("X16_Y16","mm");
      AddTelescope(A,B,C,D) ;
    }

    else if(blocks[i]->HasTokenList(sphe)){
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi= blocks[i]->GetDouble("PHI","deg");
      double R = blocks[i]->GetDouble("R","mm");
      vector<double> beta = blocks[i]->GetVectorDouble("BETA","deg");
      AddTelescope(  Theta,Phi,R,beta[0],beta[1],beta[2]);
    }

    else{
      cout << "ERROR: Missing token for CharissaTelescope blocks, check your input file" << endl;
      exit(1);
    }

  }

  InitializeStandardParameter();
  ReadAnalysisConfig();
}
///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::InitSpectra(){  
  m_Spectra = new TCharissaSpectra(m_NumberOfTelescope);
}

///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::FillSpectra(){  
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra->FillPhysicsSpectra(m_EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::CheckSpectra(){  
  m_Spectra->CheckSpectra();  
}
///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TCharissaPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
} 
///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::AddParameterToCalibrationManager()
{
  CalibrationManager* Cal = CalibrationManager::getInstance();

  for(unsigned int i = 0 ; i < m_NumberOfTelescope ; ++i){

    for(unsigned int j = 0 ; j <  m_NumberOfStrip ; ++j){
      Cal->AddParameter("CHARISSA", "T"+ NPL::itoa(i+1)+"_DE_X"+ NPL::itoa(j+1)+"_E","CHARISSA_T"+ NPL::itoa(i+1)+"_DE_X"+ NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("CHARISSA", "T"+ NPL::itoa(i+1)+"_DE_Y"+ NPL::itoa(j+1)+"_E","CHARISSA_T"+ NPL::itoa(i+1)+"_DE_Y"+ NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("CHARISSA", "T"+ NPL::itoa(i+1)+"_DE_X"+ NPL::itoa(j+1)+"_T","CHARISSA_T"+ NPL::itoa(i+1)+"_DE_X"+ NPL::itoa(j+1)+"_T")   ;
      Cal->AddParameter("CHARISSA", "T"+ NPL::itoa(i+1)+"_DE_Y"+ NPL::itoa(j+1)+"_T","CHARISSA_T"+ NPL::itoa(i+1)+"_DE_Y"+ NPL::itoa(j+1)+"_T")   ;

      Cal->AddParameter("CHARISSA", "T"+ NPL::itoa(i+1)+"_E_X"+ NPL::itoa(j+1)+"_E","CHARISSA_T"+ NPL::itoa(i+1)+"_E_X"+ NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("CHARISSA", "T"+ NPL::itoa(i+1)+"_E_Y"+ NPL::itoa(j+1)+"_E","CHARISSA_T"+ NPL::itoa(i+1)+"_E_Y"+ NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("CHARISSA", "T"+ NPL::itoa(i+1)+"_E_X"+ NPL::itoa(j+1)+"_T","CHARISSA_T"+ NPL::itoa(i+1)+"_E_X"+ NPL::itoa(j+1)+"_T")   ;
      Cal->AddParameter("CHARISSA", "T"+ NPL::itoa(i+1)+"_E_Y"+ NPL::itoa(j+1)+"_T","CHARISSA_T"+ NPL::itoa(i+1)+"_E_Y"+ NPL::itoa(j+1)+"_T")   ;
    }

    Cal->AddParameter("CHARISSA", "T"+ NPL::itoa(i+1)+"_CsI_E","CHARISSA_T"+ NPL::itoa(i+1)+"_CsI_E")      ;
    Cal->AddParameter("CHARISSA", "T"+ NPL::itoa(i+1)+"_CsI_T","CHARISSA_T"+ NPL::itoa(i+1)+"_CsI_T")      ;

  }

  return;

}

///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::InitializeRootInputRaw()
{
  TChain* inputChain = RootInput::getInstance()->GetChain()   ;
  inputChain->SetBranchStatus( "Charissa" , true )               ;
  inputChain->SetBranchStatus( "fCharissa_*" , true )               ;
  inputChain->SetBranchAddress( "Charissa" , &m_EventData )      ;
}

///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::InitializeRootInputPhysics()
{
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "CHARISSA" , true );
  inputChain->SetBranchStatus( "EventMultiplicity" , true );
  inputChain->SetBranchStatus( "TelescopeNumber" , true );
  inputChain->SetBranchStatus( "Layer1_Si_E" , true );
  inputChain->SetBranchStatus( "Layer1_Si_T" , true );
  inputChain->SetBranchStatus( "Layer1_Si_X" , true );
  inputChain->SetBranchStatus( "Layer1_Si_Y" , true );
  inputChain->SetBranchStatus( "Layer1_Si_EX" , true );
  inputChain->SetBranchStatus( "Layer1_Si_TX" , true );
  inputChain->SetBranchStatus( "Layer1_Si_EY" , true );
  inputChain->SetBranchStatus( "Layer1_Si_TY" , true );
  inputChain->SetBranchStatus( "Layer2_Si_E" , true );
  inputChain->SetBranchStatus( "Layer2_Si_T" , true );
  inputChain->SetBranchStatus( "Layer2_Si_X" , true );
  inputChain->SetBranchStatus( "Layer2_Si_Y" , true );
  inputChain->SetBranchStatus( "Layer2_Si_EX" , true );
  inputChain->SetBranchStatus( "Layer2_Si_TX" , true );
  inputChain->SetBranchStatus( "Layer2_Si_EY" , true );
  inputChain->SetBranchStatus( "Layer2_Si_TY" , true );
  inputChain->SetBranchStatus( "CsI_E" , true );
  inputChain->SetBranchStatus( "CsI_T" , true );
  inputChain->SetBranchStatus( "CsI_N" , true );
  inputChain->SetBranchAddress( "CHARISSA" , &m_EventPhysics);
}

///////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::InitializeRootOutput()
{
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "CHARISSA" , "TCharissaPhysics" , &m_EventPhysics );
}


/////   Specific to CHARISSAArray   ////
/////////////////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::AddTelescope(TVector3 C_X1_Y1, TVector3 C_X16_Y1,
                                    TVector3 C_X1_Y16,TVector3 C_X16_Y16){
  // To avoid warning
  C_X16_Y16 *= 1;

  m_NumberOfTelescope++;

  //   Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
  TVector3 U = C_X16_Y1 - C_X1_Y1 ;
  double Ushift = (U.Mag()-50)/2.;
  U = U.Unit();
  //   Vector V on Telescope Face (parallele to X Strip)
  TVector3 V = C_X1_Y16 - C_X1_Y1 ;
  double Vshift = (V.Mag() -50)/2. ;
  V = V.Unit() ;

  //   Position Vector of Strip Center
  TVector3 StripCenter = TVector3(0,0,0);
  //   Position Vector of X=1 Y=1 Strip
  TVector3 Strip_1_1;

  //   Geometry Parameter
  double Face = 50; //mm
  double NumberOfStrip =  m_NumberOfStrip;
  double StripPitch = Face/NumberOfStrip ; //mm
  //   Buffer object to fill Position Array
  vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

  vector< vector< double > >   OneTelescopeStripPositionX   ;
  vector< vector< double > >   OneTelescopeStripPositionY   ;
  vector< vector< double > >   OneTelescopeStripPositionZ   ;

  //   Moving StripCenter to 1.1 corner:
  Strip_1_1 = C_X1_Y1 + (U+V) * (StripPitch/2.)    ;
  Strip_1_1+= U*Ushift+V*Vshift ;

  for(unsigned int i = 0 ; i <  m_NumberOfStrip ; ++i )
  {
    lineX.clear()   ;
    lineY.clear()   ;
    lineZ.clear()   ;

    for(unsigned int j = 0 ; j <  m_NumberOfStrip ; ++j )
    {
      StripCenter  = Strip_1_1 + StripPitch*( i*U + j*V  );
      lineX.push_back( StripCenter.X() );
      lineY.push_back( StripCenter.Y() );
      lineZ.push_back( StripCenter.Z() );
    }

    OneTelescopeStripPositionX.push_back(lineX);
    OneTelescopeStripPositionY.push_back(lineY);
    OneTelescopeStripPositionZ.push_back(lineZ);

  }
  m_StripPositionX.push_back( OneTelescopeStripPositionX ) ;
  m_StripPositionY.push_back( OneTelescopeStripPositionY ) ;
  m_StripPositionZ.push_back( OneTelescopeStripPositionZ ) ;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::InitializeStandardParameter()
{
  //   Enable all channel
  vector< bool > ChannelStatus;
  m_XChannelStatus.clear()    ;
  m_YChannelStatus.clear()    ;
  m_CsIChannelStatus.clear()  ;

  ChannelStatus.resize(m_NumberOfStrip,true);
  for(unsigned int i = 0 ; i < m_NumberOfTelescope ; ++i)
  {
    m_XChannelStatus[i] = ChannelStatus;
    m_YChannelStatus[i] = ChannelStatus;
  }

  ChannelStatus.resize(m_NumberOfStrip,true);
  for(unsigned int i = 0 ; i < m_NumberOfTelescope ; ++i)
  {
    m_CsIChannelStatus[i]  = ChannelStatus;
  }


  m_MaximumStripMultiplicityAllowed = m_NumberOfTelescope   ;

  return;
}


//////////////////////////////////////////////////////////////////////////////////////////////
void TCharissaPhysics::AddTelescope(   double theta,
    double phi,
    double distance,
    double beta_u,
    double beta_v,
    double beta_w)
{


  m_NumberOfTelescope++;

  double Pi = 3.141592654 ;

  // convert from degree to radian:
  theta = theta * Pi/180. ;
  phi   = phi   * Pi/180. ;

  //Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
  TVector3 U ;
  //Vector V on Telescope Face (parallele to X Strip)
  TVector3 V ;
  //Vector W normal to Telescope Face (pointing CsI)
  TVector3 W ;
  //Vector position of Telescope Face center
  TVector3 C ;

  C = TVector3 ( distance * sin(theta) * cos(phi) ,
      distance * sin(theta) * sin(phi) ,
      distance * cos(theta) );

  TVector3 P = TVector3( cos(theta ) * cos(phi) ,
      cos(theta ) * sin(phi) ,
      -sin(theta) );

  W = C.Unit() ;
  U = W .Cross ( P ) ;
  V = W .Cross ( U );

  U = U.Unit();
  V = V.Unit();

  U.Rotate( beta_u * Pi/180. , U ) ;
  V.Rotate( beta_u * Pi/180. , U ) ;

  U.Rotate( beta_v * Pi/180. , V ) ;
  V.Rotate( beta_v * Pi/180. , V ) ;

  U.Rotate( beta_w * Pi/180. , W ) ;
  V.Rotate( beta_w * Pi/180. , W ) ;

  double Face = 98                     ; //mm
  double NumberOfStrip =  m_NumberOfStrip             ;
  double StripPitch = Face/NumberOfStrip   ; //mm

  vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

  vector< vector< double > >   OneTelescopeStripPositionX   ;
  vector< vector< double > >   OneTelescopeStripPositionY   ;
  vector< vector< double > >   OneTelescopeStripPositionZ   ;

  double X , Y , Z  ;

  //Moving C to the 1.1 corner:
  C.SetX( C.X() - ( Face/2 - StripPitch/2 ) * ( V.X() + U.X() ) )  ;
  C.SetY( C.Y() - ( Face/2 - StripPitch/2 ) * ( V.Y() + U.Y() ) )  ;
  C.SetZ( C.Z() - ( Face/2 - StripPitch/2 ) * ( V.Z() + U.Z() ) )  ;

  for(unsigned int i = 0 ; i <  m_NumberOfStrip ; ++i )
  {

    lineX.clear()   ;
    lineY.clear()   ;
    lineZ.clear()   ;

    for(unsigned int j = 0 ; j <  m_NumberOfStrip ; ++j )
    {
      X = C.X() + StripPitch * ( U.X()*i + V.X()*j )   ;
      Y = C.Y() + StripPitch * ( U.Y()*i + V.Y()*j )   ;
      Z = C.Z() + StripPitch * ( U.Z()*i + V.Z()*j )   ;

      lineX.push_back(X)   ;
      lineY.push_back(Y)   ;
      lineZ.push_back(Z)   ;

    }

    OneTelescopeStripPositionX.push_back(lineX)   ;
    OneTelescopeStripPositionY.push_back(lineY)   ;
    OneTelescopeStripPositionZ.push_back(lineZ)   ;

  }
  m_StripPositionX.push_back( OneTelescopeStripPositionX ) ;
  m_StripPositionY.push_back( OneTelescopeStripPositionY ) ;
  m_StripPositionZ.push_back( OneTelescopeStripPositionZ ) ;
}


//////////////////////////////////////////////////////////////////////////////////////////////
TVector3 TCharissaPhysics::GetPositionOfInteraction(const int i) const{
  TVector3 Position ;

  if(Layer1_Si_X[i] > -1000 &&  Layer1_Si_Y[i] >-1000)
  Position = TVector3 (   GetStripPositionX( TelescopeNumber[i] , Layer1_Si_X[i] , Layer1_Si_Y[i] )    ,
      GetStripPositionY( TelescopeNumber[i] , Layer1_Si_X[i] , Layer1_Si_Y[i] )      ,
      GetStripPositionZ( TelescopeNumber[i] , Layer1_Si_X[i] , Layer1_Si_Y[i] )      ) ;

  else if(Layer2_Si_X[i] > -1000 &&  Layer2_Si_Y[i] >-1000)
  Position = TVector3 (   GetStripPositionX( TelescopeNumber[i] , Layer2_Si_X[i] , Layer2_Si_Y[i] )    ,
      GetStripPositionY( TelescopeNumber[i] , Layer2_Si_X[i] , Layer2_Si_Y[i] )      ,
      GetStripPositionZ( TelescopeNumber[i] , Layer2_Si_X[i] , Layer2_Si_Y[i] )      ) ;

  else Position=TVector3(-1000,0,0);

  return(Position) ;

}

TVector3 TCharissaPhysics::GetTelescopeNormal( const int i) const
{
  TVector3 U =    TVector3 ( GetStripPositionX( TelescopeNumber[i] ,  m_NumberOfStrip , 1 ) ,
      GetStripPositionY( TelescopeNumber[i] ,  m_NumberOfStrip , 1 ) ,
      GetStripPositionZ( TelescopeNumber[i] ,  m_NumberOfStrip , 1 ) )

    -TVector3 ( GetStripPositionX( TelescopeNumber[i] , 1 , 1 ) ,
        GetStripPositionY( TelescopeNumber[i] , 1 , 1 ) ,
        GetStripPositionZ( TelescopeNumber[i] , 1 , 1 ) );

  TVector3 V =    TVector3 ( GetStripPositionX( TelescopeNumber[i] ,  m_NumberOfStrip ,  m_NumberOfStrip ) ,
      GetStripPositionY( TelescopeNumber[i] ,  m_NumberOfStrip ,  m_NumberOfStrip ) ,
      GetStripPositionZ( TelescopeNumber[i] ,  m_NumberOfStrip ,  m_NumberOfStrip ) )

    -TVector3 ( GetStripPositionX( TelescopeNumber[i] ,  m_NumberOfStrip , 1 ) ,
        GetStripPositionY( TelescopeNumber[i] ,  m_NumberOfStrip , 1 ) ,
        GetStripPositionZ( TelescopeNumber[i] ,  m_NumberOfStrip , 1 ) );

  TVector3 Normal = U.Cross(V);

  return(Normal.Unit()) ;
}

///////////////////////////////////////////////////////////////////////////
namespace CHARISSA_LOCAL
{
  //   tranform an integer to a string
  string itoa(int value)
  {
    char buffer [33];
    sprintf(buffer,"%d",value);
    return buffer;
  }

  //   E
  //   X
  double fDE_X_E(const TCharissaData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(   "CHARISSA/T" + NPL::itoa( m_EventData->GetCharissaLayer1StripXEDetectorNbr(i) ) + "_DE_X" + NPL::itoa( m_EventData->GetCharissaLayer1StripXEStripNbr(i) ) + "_E",
        m_EventData->GetCharissaLayer1StripXEEnergy(i) );
  }

  double fDE_X_T(const TCharissaData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(   "CHARISSA/T" + NPL::itoa( m_EventData->GetCharissaLayer1StripXTDetectorNbr(i) ) + "_DE_X" + NPL::itoa( m_EventData->GetCharissaLayer1StripXTStripNbr(i) ) +"_T",
        m_EventData->GetCharissaLayer1StripXTTime(i) );
  }

  //   Y
  double fDE_Y_E(const TCharissaData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(   "CHARISSA/T" + NPL::itoa( m_EventData->GetCharissaLayer1StripYEDetectorNbr(i) ) + "_DE_Y" + NPL::itoa( m_EventData->GetCharissaLayer1StripYEStripNbr(i) ) +"_E",
        m_EventData->GetCharissaLayer1StripYEEnergy(i) );
  }

  double fDE_Y_T(const TCharissaData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(   "CHARISSA/T" + NPL::itoa( m_EventData->GetCharissaLayer1StripYTDetectorNbr(i) ) + "_DE_Y" + NPL::itoa( m_EventData->GetCharissaLayer1StripYTStripNbr(i) ) +"_T",
        m_EventData->GetCharissaLayer1StripYTTime(i) );
  }

  //   E
  //   X
  double fE_X_E(const TCharissaData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(   "CHARISSA/T" + NPL::itoa( m_EventData->GetCharissaLayer2StripXEDetectorNbr(i) ) + "_E_X" + NPL::itoa( m_EventData->GetCharissaLayer2StripXEStripNbr(i) ) + "_E",
        m_EventData->GetCharissaLayer2StripXEEnergy(i) );
  }

  double fE_X_T(const TCharissaData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(   "CHARISSA/T" + NPL::itoa( m_EventData->GetCharissaLayer2StripXTDetectorNbr(i) ) + "_E_X" + NPL::itoa( m_EventData->GetCharissaLayer2StripXTStripNbr(i) ) +"_T",
        m_EventData->GetCharissaLayer2StripXTTime(i) );
  }

  //   Y
  double fE_Y_E(const TCharissaData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(   "CHARISSA/T" + NPL::itoa( m_EventData->GetCharissaLayer2StripYEDetectorNbr(i) ) + "_E_Y" + NPL::itoa( m_EventData->GetCharissaLayer2StripYEStripNbr(i) ) +"_E",
        m_EventData->GetCharissaLayer2StripYEEnergy(i) );
  }

  double fE_Y_T(const TCharissaData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(   "CHARISSA/T" + NPL::itoa( m_EventData->GetCharissaLayer2StripYTDetectorNbr(i) ) + "_E_Y" + NPL::itoa( m_EventData->GetCharissaLayer2StripYTStripNbr(i) ) +"_T",
        m_EventData->GetCharissaLayer2StripYTTime(i) );
  }


  //   CsI
  double fCsI_E(const TCharissaData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(   "CHARISSA/T" + NPL::itoa( m_EventData->GetCharissaCsIEDetectorNbr(i) ) + "_CsI" + NPL::itoa( m_EventData->GetCharissaCsIECristalNbr(i) ) +"_E",
        m_EventData->GetCharissaCsIEEnergy(i) );
  }

  double fCsI_T(const TCharissaData* m_EventData , const int i)
  {
    return CalibrationManager::getInstance()->ApplyCalibration(   "CHARISSA/T" + NPL::itoa( m_EventData->GetCharissaCsITDetectorNbr(i) ) + "_CsI" + NPL::itoa( m_EventData->GetCharissaCsITCristalNbr(i) ) +"_T",
        m_EventData->GetCharissaCsITTime(i) );
  }

}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TCharissaPhysics::Construct(){
  return (NPL::VDetector*) new TCharissaPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_charissa{
  public:
    proxy_charissa(){
      NPL::DetectorFactory::getInstance()->AddToken("CharissaTelescope","Charissa");
      NPL::DetectorFactory::getInstance()->AddDetector("CharissaTelescope",TCharissaPhysics::Construct);
    }
};

proxy_charissa p;
}


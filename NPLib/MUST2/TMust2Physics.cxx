/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    : october 2010                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold must2 treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TMust2Physics.h"
using namespace MUST2_LOCAL;

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

//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TMust2Physics)
  ///////////////////////////////////////////////////////////////////////////
  TMust2Physics::TMust2Physics(){
    EventMultiplicity   = 0 ;
    m_EventData         = new TMust2Data ;
    m_PreTreatedData    = new TMust2Data ;
    m_EventPhysics      = this ;
    m_Spectra           = NULL;
    m_NumberOfTelescope = 0 ;
    m_MaximumStripMultiplicityAllowed = 10;
    m_StripEnergyMatchingSigma = 0.020    ;
    m_StripEnergyMatchingNumberOfSigma = 3;
    // Raw Threshold
    m_Si_X_E_RAW_Threshold = 8200 ;
    m_Si_Y_E_RAW_Threshold = 8200 ;
    m_SiLi_E_RAW_Threshold = 8200 ;
    m_CsI_E_RAW_Threshold  = 8200 ;
    // Calibrated Threshold
    m_Si_X_E_Threshold = 0 ;
    m_Si_Y_E_Threshold = 0 ;
    m_SiLi_E_Threshold = 0 ;
    m_CsI_E_Threshold  = 0;

    m_Ignore_not_matching_SiLi = false ;
    m_Ignore_not_matching_CsI = false  ;

    m_Take_E_Y=false;
    m_Take_T_Y=true;

    m_SiLi_Size=32;
    m_SiLi_MatchingX.resize(16,0);
    m_SiLi_MatchingY.resize(16,0);

    for(int i = 0 ; i < 16 ; ++i){
      m_SiLi_MatchingX[0]=112;
      m_SiLi_MatchingY[0]=112;

      m_SiLi_MatchingX[1]=112;
      m_SiLi_MatchingY[1]=80;

      m_SiLi_MatchingX[2]=80;
      m_SiLi_MatchingY[2]=112;

      m_SiLi_MatchingX[3]=80;
      m_SiLi_MatchingY[3]=80;
      //
      m_SiLi_MatchingX[4]=48;
      m_SiLi_MatchingY[4]=80;

      m_SiLi_MatchingX[5]=48;
      m_SiLi_MatchingY[5]=112;

      m_SiLi_MatchingX[6]=16;
      m_SiLi_MatchingY[6]=80;

      m_SiLi_MatchingX[7]=16;
      m_SiLi_MatchingY[7]=112;
      //
      m_SiLi_MatchingX[8]=112;
      m_SiLi_MatchingY[8]=16;

      m_SiLi_MatchingX[9]=112;
      m_SiLi_MatchingY[9]=48;

      m_SiLi_MatchingX[10]=80;
      m_SiLi_MatchingY[10]=16;

      m_SiLi_MatchingX[11]=80;
      m_SiLi_MatchingY[11]=48;
      //
      m_SiLi_MatchingX[12]=48;
      m_SiLi_MatchingY[12]=48;

      m_SiLi_MatchingX[13]=48;
      m_SiLi_MatchingY[13]=16;

      m_SiLi_MatchingX[14]=16;
      m_SiLi_MatchingY[14]=48;

      m_SiLi_MatchingX[15]=16;
      m_SiLi_MatchingY[15]=16;
    }

    m_CsI_Size=32;
    m_CsI_MatchingX.resize(16,0);
    m_CsI_MatchingY.resize(16,0);
    for(int i = 0 ; i < 16 ; ++i){
      m_CsI_MatchingX[0]=112;
      m_CsI_MatchingY[0]=112;

      m_CsI_MatchingX[1]=112;
      m_CsI_MatchingY[1]=80;

      m_CsI_MatchingX[2]=112;
      m_CsI_MatchingY[2]=48;

      m_CsI_MatchingX[3]=112;
      m_CsI_MatchingY[3]=16;
      //
      m_CsI_MatchingX[4]=80;
      m_CsI_MatchingY[4]=16;

      m_CsI_MatchingX[5]=80;
      m_CsI_MatchingY[5]=48;

      m_CsI_MatchingX[6]=80;
      m_CsI_MatchingY[6]=80;

      m_CsI_MatchingX[7]=80;
      m_CsI_MatchingY[7]=112;
      //
      m_CsI_MatchingX[8]=48;
      m_CsI_MatchingY[8]=16;

      m_CsI_MatchingX[9]=48;
      m_CsI_MatchingY[9]=48;

      m_CsI_MatchingX[10]=48;
      m_CsI_MatchingY[10]=80;

      m_CsI_MatchingX[11]=48;
      m_CsI_MatchingY[11]=112;
      //
      m_CsI_MatchingX[12]=16;
      m_CsI_MatchingY[12]=16;

      m_CsI_MatchingX[13]=16;
      m_CsI_MatchingY[13]=48;

      m_CsI_MatchingX[14]=16;
      m_CsI_MatchingY[14]=80;

      m_CsI_MatchingX[15]=16;
      m_CsI_MatchingY[15]=112;
    }

  }

///////////////////////////////////////////////////////////////////////////
TMust2Physics::~TMust2Physics(){

}
///////////////////////////////////////////////////////////////////////////
void TMust2Physics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////

void TMust2Physics::BuildPhysicalEvent(){
  PreTreat();

  bool check_SILI = false ;
  bool check_CSI  = false ;

  m_StripXEMult = m_PreTreatedData->GetMMStripXEMult();
  m_StripYEMult = m_PreTreatedData->GetMMStripYEMult();
  m_StripXTMult = m_PreTreatedData->GetMMStripXTMult();
  m_StripYTMult = m_PreTreatedData->GetMMStripYTMult();
  m_SiLiEMult = m_PreTreatedData->GetMMSiLiEMult();
  m_SiLiTMult = m_PreTreatedData->GetMMSiLiTMult();
  m_CsIEMult = m_PreTreatedData->GetMMCsIEMult();
  m_CsITMult = m_PreTreatedData->GetMMCsITMult();

  if( CheckEvent() == 1 ){
    vector< TVector2 > couple = Match_X_Y() ;
    EventMultiplicity = couple.size();

    for(unsigned int i = 0 ; i < couple.size() ; ++i){
      check_SILI = false ;
      check_CSI = false ;

      int N = m_PreTreatedData->GetMMStripXEDetectorNbr(couple[i].X()) ;

      int X = m_PreTreatedData->GetMMStripXEStripNbr(couple[i].X()) ;
      int Y = m_PreTreatedData->GetMMStripYEStripNbr(couple[i].Y()) ;

      double Si_X_E = m_PreTreatedData->GetMMStripXEEnergy( couple[i].X() ) ;
      double Si_Y_E = m_PreTreatedData->GetMMStripYEEnergy( couple[i].Y() ) ;

      //  Search for associate Time
      double Si_X_T = -1000 ;
      for(unsigned int t = 0 ; t < m_StripXTMult ; ++t ){
        if(   m_PreTreatedData->GetMMStripXTStripNbr( couple[i].X() ) == m_PreTreatedData->GetMMStripXTStripNbr(t)
            && m_PreTreatedData->GetMMStripXTDetectorNbr( couple[i].X() ) == m_PreTreatedData->GetMMStripXTDetectorNbr(t)){
          Si_X_T = m_PreTreatedData->GetMMStripXTTime(t);
          break;
        }
      }

      double Si_Y_T = -1000 ;
      for(unsigned int t = 0 ; t < m_StripYTMult ; ++t ){
        if(   m_PreTreatedData->GetMMStripYTStripNbr( couple[i].Y() ) == m_PreTreatedData->GetMMStripYTStripNbr(t)
            && m_PreTreatedData->GetMMStripYTDetectorNbr( couple[i].Y() ) == m_PreTreatedData->GetMMStripYTDetectorNbr(t)){
          Si_Y_T = m_PreTreatedData->GetMMStripYTTime(t);
          break;
        }  
      }

      Si_X.push_back(X) ; Si_Y.push_back(Y) ; TelescopeNumber.push_back(N) ;

      if(m_Take_E_Y) Si_E.push_back(Si_Y_E);
      else           Si_E.push_back(Si_X_E);

      if(m_Take_T_Y) Si_T.push_back(Si_Y_T);
      else           Si_T.push_back(Si_X_T);

      // Store the other value for checking purpose
      Si_EX.push_back(Si_X_E);
      Si_TX.push_back(Si_X_T);

      Si_EY.push_back(Si_Y_E);
      Si_TY.push_back(Si_Y_T);

      for(unsigned int j = 0 ; j < m_SiLiEMult; ++j){
        if(m_PreTreatedData->GetMMSiLiEDetectorNbr(j)==N){
          // pad vs strip number match
          if( Match_Si_SiLi( X, Y , m_PreTreatedData->GetMMSiLiEPadNbr(j) ) ){
            SiLi_N.push_back(m_PreTreatedData->GetMMSiLiEPadNbr(j))   ;
            SiLi_E.push_back(m_PreTreatedData->GetMMSiLiEEnergy(j))   ;
            SiLi_T.push_back(-1000)   ;
            // Look for associate time
            for(unsigned int k =0 ; k  < m_SiLiTMult ; ++k){
              // Same Pad, same Detector
              if(    m_PreTreatedData->GetMMSiLiEPadNbr(j)==m_PreTreatedData->GetMMSiLiTPadNbr(k)
                  && m_PreTreatedData->GetMMSiLiEDetectorNbr(j)==m_PreTreatedData->GetMMSiLiTDetectorNbr(k)){
                SiLi_T[SiLi_T.size()-1]= m_PreTreatedData->GetMMSiLiTTime(k) ; 
                break ;
              }
            }

            check_SILI = true ;
          }
        }
      }

      for(unsigned int j = 0 ; j < m_CsIEMult ; ++j){
        if(m_PreTreatedData->GetMMCsIEDetectorNbr(j)==N){
          if(Match_Si_CsI( X, Y , m_PreTreatedData->GetMMCsIECristalNbr(j) ) ){
            CsI_N.push_back( m_PreTreatedData->GetMMCsIECristalNbr(j) ) ;
            CsI_E.push_back( m_PreTreatedData->GetMMCsIEEnergy(j) ) ;
            CsI_T.push_back(-1000);
            //   Look for associate Time
            for(unsigned int k =0 ; k  < m_CsITMult ; ++k){
              // Same Cristal, Same Detector
              if(   m_PreTreatedData->GetMMCsIECristalNbr(j)==m_PreTreatedData->GetMMCsITCristalNbr(k)
                  && m_PreTreatedData->GetMMCsIEDetectorNbr(j)==m_PreTreatedData->GetMMCsITDetectorNbr(k) ){
                CsI_T[CsI_T.size()-1]= m_PreTreatedData->GetMMCsITTime(j) ; 
                break ;
              }
            }

            check_CSI = true ;
          }
        }
      }


      if(!check_SILI){
        SiLi_N.push_back(0)       ;
        SiLi_E.push_back(-1000)   ;
        SiLi_T.push_back(-1000)   ;
      }

      if(!check_CSI){
        CsI_N.push_back(0)       ;
        CsI_E.push_back(-1000)   ;
        CsI_T.push_back(-1000)   ;
      }

    }
  }

  return;

}

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::PreTreat(){
  ClearPreTreatedData();
  m_StripXEMult = m_EventData->GetMMStripXEMult();
  m_StripYEMult = m_EventData->GetMMStripYEMult();
  m_StripXTMult = m_EventData->GetMMStripXTMult();
  m_StripYTMult = m_EventData->GetMMStripYTMult();
  m_SiLiEMult = m_EventData->GetMMSiLiEMult();
  m_SiLiTMult = m_EventData->GetMMSiLiTMult();
  m_CsIEMult = m_EventData->GetMMCsIEMult();
  m_CsITMult = m_EventData->GetMMCsITMult();

  //   X
  //   E
  for(unsigned int i = 0 ; i < m_StripXEMult ; ++i){
    if( m_EventData->GetMMStripXEEnergy(i)>m_Si_X_E_RAW_Threshold && IsValidChannel("X", m_EventData->GetMMStripXEDetectorNbr(i), m_EventData->GetMMStripXEStripNbr(i)) ){
      double EX = fSi_X_E(m_EventData , i);
      if( EX > m_Si_X_E_Threshold ){
        m_PreTreatedData->SetMMStripXEDetectorNbr( m_EventData->GetMMStripXEDetectorNbr(i) );
        m_PreTreatedData->SetMMStripXEStripNbr( m_EventData->GetMMStripXEStripNbr(i) );
        m_PreTreatedData->SetMMStripXEEnergy( EX );
      }

    }
  }

  //   T
  for(unsigned int i = 0 ; i < m_StripXTMult ; ++i){
    if(IsValidChannel("X", m_EventData->GetMMStripXTDetectorNbr(i), m_EventData->GetMMStripXTStripNbr(i))){
      m_PreTreatedData->SetMMStripXTDetectorNbr( m_EventData->GetMMStripXTDetectorNbr(i) );
      m_PreTreatedData->SetMMStripXTStripNbr( m_EventData->GetMMStripXTStripNbr(i) );
      m_PreTreatedData->SetMMStripXTTime( fSi_X_T(m_EventData , i) );
    }
  }


  //   Y
  //   E
  for(unsigned int i = 0 ; i < m_StripYEMult ; ++i){
    if( m_EventData->GetMMStripYEEnergy(i)<m_Si_Y_E_RAW_Threshold && IsValidChannel("Y", m_EventData->GetMMStripYEDetectorNbr(i), m_EventData->GetMMStripYEStripNbr(i))){
      double EY = fSi_Y_E(m_EventData , i);
      if( EY >m_Si_Y_E_Threshold ){
        m_PreTreatedData->SetMMStripYEDetectorNbr( m_EventData->GetMMStripYEDetectorNbr(i) );
        m_PreTreatedData->SetMMStripYEStripNbr( m_EventData->GetMMStripYEStripNbr(i) );
        m_PreTreatedData->SetMMStripYEEnergy( EY );
      }
    }
  }

  //   T
  for(unsigned int i = 0 ; i < m_StripYTMult ; ++i){
    if(IsValidChannel("Y", m_EventData->GetMMStripYTDetectorNbr(i), m_EventData->GetMMStripYTStripNbr(i))){
      m_PreTreatedData->SetMMStripYTDetectorNbr( m_EventData->GetMMStripYTDetectorNbr(i) );
      m_PreTreatedData->SetMMStripYTStripNbr( m_EventData->GetMMStripYTStripNbr(i) );
      m_PreTreatedData->SetMMStripYTTime( fSi_Y_T(m_EventData , i) );
    }
  }


  //   CsI
  //   E
  for(unsigned int i = 0 ; i < m_CsIEMult ; ++i){
    if( m_EventData->GetMMCsIEEnergy(i)>m_CsI_E_RAW_Threshold && IsValidChannel("CsI", m_EventData->GetMMCsIEDetectorNbr(i), m_EventData->GetMMCsIECristalNbr(i))){
      double ECsI = fCsI_E(m_EventData , i);
      if( ECsI > m_CsI_E_Threshold ){
        m_PreTreatedData->SetMMCsIEDetectorNbr( m_EventData->GetMMCsIEDetectorNbr(i) );
        m_PreTreatedData->SetMMCsIECristalNbr( m_EventData->GetMMCsIECristalNbr(i) );
        m_PreTreatedData->SetMMCsIEEnergy( ECsI );
      }
    }
  }

  //   T
  for(unsigned int i = 0 ; i < m_CsITMult ; ++i){
    if(IsValidChannel("CsI", m_EventData->GetMMCsITDetectorNbr(i), m_EventData->GetMMCsITCristalNbr(i))){
      m_PreTreatedData->SetMMCsITDetectorNbr( m_EventData->GetMMCsITDetectorNbr(i) );
      m_PreTreatedData->SetMMCsITCristalNbr( m_EventData->GetMMCsITCristalNbr(i) );
      m_PreTreatedData->SetMMCsITTime( fCsI_T(m_EventData , i) );
    }
  }


  //   SiLi
  //   E
  for(unsigned int i = 0 ; i < m_SiLiEMult ; ++i){
    if( m_EventData->GetMMSiLiEEnergy(i)>m_SiLi_E_RAW_Threshold && IsValidChannel("SiLi", m_EventData->GetMMSiLiEDetectorNbr(i), m_EventData->GetMMSiLiEPadNbr(i))){
      double ESiLi = fSiLi_E(m_EventData , i);
      if( ESiLi > m_SiLi_E_Threshold ){
        m_PreTreatedData->SetMMSiLiEDetectorNbr( m_EventData->GetMMSiLiEDetectorNbr(i) );
        m_PreTreatedData->SetMMSiLiEPadNbr( m_EventData->GetMMSiLiEPadNbr(i) );
        m_PreTreatedData->SetMMSiLiEEnergy( ESiLi );
      }
    }
  }

  //   T
  for(unsigned int i = 0 ; i < m_SiLiTMult ; ++i){
    if(IsValidChannel("SiLi", m_EventData->GetMMSiLiTDetectorNbr(i), m_EventData->GetMMSiLiTPadNbr(i))){
      m_PreTreatedData->SetMMSiLiTDetectorNbr( m_EventData->GetMMSiLiTDetectorNbr(i) );
      m_PreTreatedData->SetMMSiLiTPadNbr( m_EventData->GetMMSiLiTPadNbr(i) );
      m_PreTreatedData->SetMMSiLiTTime( fSiLi_T(m_EventData , i) );
    }
  }

  return;
}


///////////////////////////////////////////////////////////////////////////
int TMust2Physics :: CheckEvent(){
  // Check the size of the different elements
  if( m_PreTreatedData->GetMMStripXEMult() == m_PreTreatedData->GetMMStripYEMult() )
    return 1 ; // Regular Event

// INterstrip management is not coded, so waste of time to make this test
/*  else if(   m_PreTreatedData->GetMMStripXEMult() == m_PreTreatedData->GetMMStripYEMult()+1 
          || m_PreTreatedData->GetMMStripXEMult() == m_PreTreatedData->GetMMStripYEMult()-1  )
    return 2 ; // Pseudo Event, potentially interstrip*/

  else
    return -1 ; // Rejected Event

}

///////////////////////////////////////////////////////////////////////////
bool TMust2Physics :: ResolvePseudoEvent(){
  return false;
}

///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TMust2Physics :: Match_X_Y(){
  vector < TVector2 > ArrayOfGoodCouple ;
  m_StripXEMult = m_PreTreatedData->GetMMStripXEMult();
  m_StripYEMult = m_PreTreatedData->GetMMStripXEMult();

  // Prevent code from treating very high multiplicity Event
  // Those event are not physical anyway and that improve speed.
  if( m_StripXEMult > m_MaximumStripMultiplicityAllowed || m_StripYEMult > m_MaximumStripMultiplicityAllowed )
    return ArrayOfGoodCouple;

  for(unsigned int i = 0 ; i < m_StripXEMult ; ++i){
    for(unsigned int j = 0 ; j < m_StripYEMult ; j++){
      //   if same detector check energy
      if ( m_PreTreatedData->GetMMStripXEDetectorNbr(i) == m_PreTreatedData->GetMMStripYEDetectorNbr(j) ){
        //   Look if energy match
        if( abs( (m_PreTreatedData->GetMMStripXEEnergy(i)-m_PreTreatedData->GetMMStripYEEnergy(j))/2. ) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma ){
          // Special Option, if the event is between two CsI cristal, it is rejected.
          if(m_Ignore_not_matching_CsI){
            bool check_validity=false;
            for (unsigned int hh = 0 ; hh<16 ; ++hh ){
              if( Match_Si_CsI(m_PreTreatedData->GetMMStripXEStripNbr(i), m_PreTreatedData->GetMMStripYEStripNbr(j) , hh+1) )
                check_validity=true;
            }

            if(check_validity)
              ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;
          }

          // Special Option, if the event is between two SiLi pad , it is rejected.
          if(m_Ignore_not_matching_SiLi){
            bool check_validity=false;
            for (unsigned int hh = 0 ; hh<16 ; ++hh ){
              if( Match_Si_SiLi(m_PreTreatedData->GetMMStripXEStripNbr(i), m_PreTreatedData->GetMMStripYEStripNbr(j) , hh+1) )
                check_validity=true;
            }

            if(check_validity)
              ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;
          }

          // Regular case, keep the event
          else ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;
        }
      }
    }
  }

  //   Prevent to treat event with ambiguous matchin beetween X and Y
  if( ArrayOfGoodCouple.size() > m_StripXEMult ) ArrayOfGoodCouple.clear() ;

  return ArrayOfGoodCouple;
}


////////////////////////////////////////////////////////////////////////////
bool TMust2Physics :: IsValidChannel(const string DetectorType, const int telescope , const int channel){
  if(DetectorType == "CsI")
    return *(m_CsIChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "SiLi")
    return *(m_SiLiChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "X")
    return *(m_XChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "Y")
    return *(m_YChannelStatus[telescope-1].begin()+channel-1);

  else return false;
}

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::ReadAnalysisConfig(){
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigMust2.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigMust2.dat found: Default parameters loaded for Analysis " << FileName << endl;
    return;
  }
  cout << " Loading user parameters for Analysis from ConfigMust2.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigMust2.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 11, "ConfigMust2") == 0) ReadingStatus = true;

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
        ChannelStatus.resize(128,false);
        m_XChannelStatus[telescope-1] = ChannelStatus;
        m_YChannelStatus[telescope-1] = ChannelStatus;
        ChannelStatus.resize(16,false);
        m_SiLiChannelStatus[telescope-1] = ChannelStatus;
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

        else if (DataBuffer.compare(3,4,"SILI") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_SiLiChannelStatus[telescope-1].begin()+channel-1) = false;
        }

        else if (DataBuffer.compare(3,3,"CSI") == 0) {
          channel = atoi(DataBuffer.substr(6).c_str());
          *(m_CsIChannelStatus[telescope-1].begin()+channel-1) = false;
        }

        else cout << "Warning: detector type for Must2 unknown!" << endl;

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

      else if (whatToDo=="IGNORE_NOT_MATCHING_SILI") {
        m_Ignore_not_matching_SiLi = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="SILI_SIZE") {
        AnalysisConfigFile >> DataBuffer;
        m_SiLi_Size = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_SiLi_Size << endl;
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

      else if (whatToDo=="SILI_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_SiLi_E_RAW_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_SiLi_E_RAW_Threshold << endl;
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

      else if (whatToDo=="SILI_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_SiLi_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_SiLi_E_Threshold << endl;
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
bool TMust2Physics :: Match_Si_SiLi(int X, int Y , int PadNbr){

  //remove the central part and surrounding
  if( X < 8  ||  X > 120|| (Y < 68 &&  Y > 60) )
    return false;

  if( abs(m_SiLi_MatchingX[PadNbr-1] - X) < m_SiLi_Size/2.&&
      abs(m_SiLi_MatchingY[PadNbr-1] - Y) < m_SiLi_Size/2.)
    return true ;

  else return false;

}


///////////////////////////////////////////////////////////////////////////
bool TMust2Physics :: Match_Si_CsI(int X, int Y , int CristalNbr){

  if( abs(m_CsI_MatchingX[CristalNbr-1] - X) < m_CsI_Size/2.&&
      abs(m_CsI_MatchingY[CristalNbr-1] - Y) < m_CsI_Size/2.)

    return true ;

  else return false;

}

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::Clear(){
  EventMultiplicity= 0 ;

  TelescopeNumber.clear();
  EventType.clear();
  TotalEnergy.clear();

  // Si X
  Si_E.clear();
  Si_T.clear();
  Si_X.clear();
  Si_Y.clear();

  // Si(Li)
  SiLi_E.clear();
  SiLi_T.clear();
  SiLi_N.clear();

  // CsI
  CsI_E.clear();
  CsI_T.clear();
  CsI_N.clear();

  Si_EX.clear();
  Si_TX.clear();
  Si_EY.clear();
  Si_TY.clear();
  TelescopeNumber_X.clear();
  TelescopeNumber_Y.clear();
}
///////////////////////////////////////////////////////////////////////////

void TMust2Physics::ReadCalibrationRun(){
  m_StripXEMult = m_EventData->GetMMStripXEMult();
  m_StripYEMult = m_EventData->GetMMStripYEMult();
  m_StripXTMult = m_EventData->GetMMStripXTMult();
  m_StripYTMult = m_EventData->GetMMStripYTMult();
  m_SiLiEMult = m_EventData->GetMMSiLiEMult();
  m_SiLiTMult = m_EventData->GetMMSiLiTMult();
  m_CsIEMult = m_EventData->GetMMCsIEMult();
  m_CsITMult = m_EventData->GetMMCsITMult();

  //   X
  //   E
  for(unsigned int i = 0 ; i < m_StripXEMult;++i){
    TelescopeNumber_X.push_back(m_EventData->GetMMStripXEDetectorNbr(i));
    Si_EX.push_back( fSi_X_E( m_EventData , i) );
    Si_X.push_back(m_EventData->GetMMStripXEStripNbr(i));
  }
  //   T
  for(unsigned int i = 0 ; i < m_StripXTMult;++i){
    TelescopeNumber_X.push_back(m_EventData->GetMMStripXTDetectorNbr(i));
    Si_TX.push_back( fSi_X_T( m_EventData , i) );
    Si_X.push_back(m_EventData->GetMMStripXTStripNbr(i));
  }

  //   Y
  //   E
  for(unsigned int i = 0 ; i < m_StripYEMult;++i){
    TelescopeNumber_Y.push_back(m_EventData->GetMMStripYEDetectorNbr(i));
    Si_EY.push_back( fSi_Y_E( m_EventData , i) );
    Si_Y.push_back(m_EventData->GetMMStripYEStripNbr(i));
  }

  //   T
  for(unsigned int i = 0 ; i < m_StripYTMult;++i){
    TelescopeNumber_Y.push_back(m_EventData->GetMMStripYTDetectorNbr(i));
    Si_TY.push_back( fSi_Y_T( m_EventData , i) );
    Si_Y.push_back(m_EventData->GetMMStripYTStripNbr(i));
  }

  //CsI Energy
  for(unsigned int j = 0 ; j < m_CsIEMult ; ++j){
    CsI_N.push_back(m_EventData->GetMMCsIECristalNbr(j)) ;
    CsI_E.push_back(fCsI_E(m_EventData , j)) ;
  }

  //CsI Time
  for(unsigned int j = 0 ; j < m_CsITMult ; ++j){
    CsI_T.push_back(fCsI_T(m_EventData , j)) ;
  }

}

////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::ReadConfiguration(string Path){
  ifstream ConfigFile              ;
  ConfigFile.open(Path.c_str())    ;
  string LineBuffer                ;
  string DataBuffer                ;

  // A:X1_Y1     --> X:1    Y:1
  // B:X128_Y1   --> X:128  Y:1
  // C:X1_Y128   --> X:1    Y:128
  // D:X128_Y128 --> X:128  Y:128

  double Ax , Bx , Cx , Dx , Ay , By , Cy , Dy , Az , Bz , Cz , Dz;
  TVector3 A , B , C , D;
  double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0;

  bool check_A = false ;
  bool check_C = false ;
  bool check_B = false ;
  bool check_D = false ;

  bool check_Theta = false ;
  bool check_Phi   = false ;
  bool check_R     = false ;
  bool check_beta  = false ;

  bool ReadingStatus = false ;


  while (!ConfigFile.eof()){

    getline(ConfigFile, LineBuffer);

    //   If line is a Start Up Must2 bloc, Reading toggle to true
    if (LineBuffer.compare(0, 11, "M2Telescope")==0){
      cout << "///" << endl                 ;
      cout << "Telescope found: " << endl   ;
      ReadingStatus = true                ;

    }

    //   Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    //   Reading Block
    while(ReadingStatus){

      ConfigFile >> DataBuffer ;
      //   Comment Line
      if(DataBuffer.compare(0, 1, "%") == 0) {
        ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
      }

      //   Finding another telescope (safety), toggle out
      else if (DataBuffer=="M2Telescope") {
        cout << "WARNING: Another Telescope is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
        ReadingStatus = false ;
      }

      //   Position method

      else if (DataBuffer=="X1_Y1=") {
        check_A = true;
        ConfigFile >> DataBuffer ;
        Ax = atof(DataBuffer.c_str()) ;
        ConfigFile >> DataBuffer ;
        Ay = atof(DataBuffer.c_str()) ;
        ConfigFile >> DataBuffer ;
        Az = atof(DataBuffer.c_str()) ;

        A = TVector3(Ax, Ay, Az);
        cout << "X1 Y1 corner position : (" << A.X() << ";" << A.Y() << ";" << A.Z() << ")" << endl;
      }


      else if (DataBuffer=="X128_Y1=") {
        check_B = true;
        ConfigFile >> DataBuffer ;
        Bx = atof(DataBuffer.c_str()) ;
        ConfigFile >> DataBuffer ;
        By = atof(DataBuffer.c_str()) ;
        ConfigFile >> DataBuffer ;
        Bz = atof(DataBuffer.c_str()) ;

        B = TVector3(Bx, By, Bz);
        cout << "X128 Y1 corner position : (" << B.X() << ";" << B.Y() << ";" << B.Z() << ")" << endl;
      }


      else if (DataBuffer=="X1_Y128=") {
        check_C = true;
        ConfigFile >> DataBuffer ;
        Cx = atof(DataBuffer.c_str()) ;
        ConfigFile >> DataBuffer ;
        Cy = atof(DataBuffer.c_str()) ;
        ConfigFile >> DataBuffer ;
        Cz = atof(DataBuffer.c_str()) ;

        C = TVector3(Cx, Cy, Cz);
        cout << "X1 Y128 corner position : (" << C.X() << ";" << C.Y() << ";" << C.Z() << ")" << endl;
      }

      else if (DataBuffer=="X128_Y128=") {
        check_D = true;
        ConfigFile >> DataBuffer ;
        Dx = atof(DataBuffer.c_str()) ;
        ConfigFile >> DataBuffer ;
        Dy = atof(DataBuffer.c_str()) ;
        ConfigFile >> DataBuffer ;
        Dz = atof(DataBuffer.c_str()) ;

        D = TVector3(Dx, Dy, Dz);
        cout << "X128 Y128 corner position : (" << D.X() << ";" << D.Y() << ";" << D.Z() << ")" << endl;
      }

      //   End Position Method

      //   Angle method
      else if (DataBuffer=="THETA=") {
        check_Theta = true;
        ConfigFile >> DataBuffer ;
        Theta = atof(DataBuffer.c_str()) ;
        cout << "Theta:  " << Theta << endl;
      }

      //Angle method
      else if (DataBuffer=="PHI=") {
        check_Phi = true;
        ConfigFile >> DataBuffer ;
        Phi = atof(DataBuffer.c_str()) ;
        cout << "Phi:  " << Phi << endl;
      }

      //Angle method
      else if (DataBuffer=="R=") {
        check_R = true;
        ConfigFile >> DataBuffer ;
        R = atof(DataBuffer.c_str()) ;
        cout << "R:  " << R << endl;
      }

      //Angle method
      else if (DataBuffer=="BETA=") {
        check_beta = true;
        ConfigFile >> DataBuffer ;
        beta_u = atof(DataBuffer.c_str()) ;
        ConfigFile >> DataBuffer ;
        beta_v = atof(DataBuffer.c_str()) ;
        ConfigFile >> DataBuffer ;
        beta_w = atof(DataBuffer.c_str()) ;
        cout << "Beta:  " << beta_u << " " << beta_v << " " << beta_w << endl  ;
      }

      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out
      if (    (check_A && check_B && check_C && check_D) 
           || (check_Theta && check_Phi && check_R && check_beta)  ){
        ReadingStatus = false;

        ///Add The previously define telescope
        //With position method
        if ( check_A && check_B && check_C && check_D ){
          AddTelescope(A,B,C,D) ;
        }

        //with angle method
        else if ( check_Theta && check_Phi && check_R && check_beta ){
          AddTelescope(  Theta,
                         Phi,
                         R,
                         beta_u,
                         beta_v,
                         beta_w);
        }

        check_A = false ;
        check_B = false ;
        check_C = false ;
        check_D = false ;

        check_Theta = false ;
        check_Phi  = false ;
        check_R    = false ;
        check_beta = false ;
      }
    }
  }

  InitializeStandardParameter();
  ReadAnalysisConfig();

  cout << endl << "/////////////////////////////" << endl << endl;

}
///////////////////////////////////////////////////////////////////////////
void TMust2Physics::InitSpectra(){  
  m_Spectra = new TMust2Spectra(m_NumberOfTelescope);
}

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::FillSpectra(){  
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TMust2Physics::CheckSpectra(){  
  m_Spectra->CheckSpectra();  
}
///////////////////////////////////////////////////////////////////////////
void TMust2Physics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< vector<string> , TH1*> TMust2Physics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< vector<string> , TH1*> empty;
    return empty;
  }
} 
///////////////////////////////////////////////////////////////////////////
void TMust2Physics::AddParameterToCalibrationManager()
{
  CalibrationManager* Cal = CalibrationManager::getInstance();

  for(int i = 0 ; i < m_NumberOfTelescope ; ++i){

    for( int j = 0 ; j < 128 ; ++j){
      Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_Si_X"+itoa(j+1)+"_E","MUST2_T"+itoa(i+1)+"_Si_X"+itoa(j+1)+"_E")   ;
      Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_Si_Y"+itoa(j+1)+"_E","MUST2_T"+itoa(i+1)+"_Si_Y"+itoa(j+1)+"_E")   ;
      Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_Si_X"+itoa(j+1)+"_T","MUST2_T"+itoa(i+1)+"_Si_X"+itoa(j+1)+"_T")   ;
      Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_Si_Y"+itoa(j+1)+"_T","MUST2_T"+itoa(i+1)+"_Si_Y"+itoa(j+1)+"_T")   ;
    }

    for( int j = 0 ; j < 16 ; ++j){
      Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_SiLi"+itoa(j+1)+"_E","MUST2_T"+itoa(i+1)+"_SiLi"+itoa(j+1)+"_E")   ;
      Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_SiLi"+itoa(j+1)+"_T","MUST2_T"+itoa(i+1)+"_SiLi"+itoa(j+1)+"_T")   ;
    }

    for( int j = 0 ; j < 16 ; ++j){
      Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_CsI"+itoa(j+1)+"_E","MUST2_T"+itoa(i+1)+"_CsI"+itoa(j+1)+"_E")      ;
      Cal->AddParameter("MUST2", "T"+itoa(i+1)+"_CsI"+itoa(j+1)+"_T","MUST2_T"+itoa(i+1)+"_CsI"+itoa(j+1)+"_T")      ;
    }
  }

  return;

}

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain()   ;
  inputChain->SetBranchStatus( "MUST2" , true )               ;
  inputChain->SetBranchStatus( "fMM_*" , true )               ;
  inputChain->SetBranchAddress( "MUST2" , &m_EventData )      ;
}

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "MUST2" , true );
  inputChain->SetBranchStatus( "EventMultiplicity" , true );
  inputChain->SetBranchStatus( "EventType" , true );
  inputChain->SetBranchStatus( "TelescopeNumber" , true );
  inputChain->SetBranchStatus( "Si_E" , true );
  inputChain->SetBranchStatus( "Si_T" , true );
  inputChain->SetBranchStatus( "Si_X" , true );
  inputChain->SetBranchStatus( "Si_Y" , true );
  inputChain->SetBranchStatus( "Si_EX" , true );
  inputChain->SetBranchStatus( "Si_TX" , true );
  inputChain->SetBranchStatus( "Si_EY" , true );
  inputChain->SetBranchStatus( "Si_TY" , true );
  inputChain->SetBranchStatus( "TelescopeNumber_X" , true );
  inputChain->SetBranchStatus( "TelescopeNumber_Y" , true );
  inputChain->SetBranchStatus( "SiLi_E" , true );
  inputChain->SetBranchStatus( "SiLi_T" , true );
  inputChain->SetBranchStatus( "SiLi_N" , true );
  inputChain->SetBranchStatus( "CsI_E" , true );
  inputChain->SetBranchStatus( "CsI_T" , true );
  inputChain->SetBranchStatus( "CsI_N" , true );
  inputChain->SetBranchStatus( "TotalEnergy" , true );
  inputChain->SetBranchAddress( "MUST2" , &m_EventPhysics);
}

///////////////////////////////////////////////////////////////////////////
void TMust2Physics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "MUST2" , "TMust2Physics" , &m_EventPhysics );
}


/////   Specific to MUST2Array   ////

void TMust2Physics::AddTelescope( TVector3 C_X1_Y1,
                                  TVector3 C_X128_Y1,
                                  TVector3 C_X1_Y128,
                                  TVector3 C_X128_Y128){
  // To avoid warning
  C_X128_Y128 *= 1;

  m_NumberOfTelescope++;

  //   Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
  TVector3 U = C_X128_Y1 - C_X1_Y1 ;
  double Ushift = (U.Mag()-98)/2.;
  U = U.Unit();
  //   Vector V on Telescope Face (parallele to X Strip)
  TVector3 V = C_X1_Y128 - C_X1_Y1 ;
  double Vshift = (V.Mag() -98)/2. ;
  V = V.Unit() ;

  //   Position Vector of Strip Center
  TVector3 StripCenter = TVector3(0,0,0);
  //   Position Vector of X=1 Y=1 Strip
  TVector3 Strip_1_1;

  //   Geometry Parameter
  double Face = 98; //mm
  double NumberOfStrip = 128;
  double StripPitch = Face/NumberOfStrip ; //mm
  //   Buffer object to fill Position Array
  vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

  vector< vector< double > >   OneTelescopeStripPositionX   ;
  vector< vector< double > >   OneTelescopeStripPositionY   ;
  vector< vector< double > >   OneTelescopeStripPositionZ   ;

  //   Moving StripCenter to 1.1 corner:
  Strip_1_1 = C_X1_Y1 + (U+V) * (StripPitch/2.)    ;
  Strip_1_1+= U*Ushift+V*Vshift ;

  for( int i = 0 ; i < 128 ; ++i ){
    lineX.clear()   ;
    lineY.clear()   ;
    lineZ.clear()   ;

    for( int j = 0 ; j < 128 ; ++j ){
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


void TMust2Physics::InitializeStandardParameter(){
  //   Enable all channel
  vector< bool > ChannelStatus;
  m_XChannelStatus.clear()    ;
  m_YChannelStatus.clear()    ;
  m_SiLiChannelStatus.clear() ;
  m_CsIChannelStatus.clear()  ;

  ChannelStatus.resize(128,true);
  for(int i = 0 ; i < m_NumberOfTelescope ; ++i){
    m_XChannelStatus[i] = ChannelStatus;
    m_YChannelStatus[i] = ChannelStatus;
  }

  ChannelStatus.resize(16,true);
  for(int i = 0 ; i < m_NumberOfTelescope ; ++i){
    m_SiLiChannelStatus[i] = ChannelStatus;
    m_CsIChannelStatus[i]  = ChannelStatus;
  }

  m_MaximumStripMultiplicityAllowed = m_NumberOfTelescope   ;

  return;
}

void TMust2Physics::AddTelescope(   double theta,
    double phi,
    double distance,
    double beta_u,
    double beta_v,
    double beta_w){

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
  double NumberOfStrip = 128             ;
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

  for( int i = 0 ; i < 128 ; ++i ){

    lineX.clear()   ;
    lineY.clear()   ;
    lineZ.clear()   ;

    for( int j = 0 ; j < 128 ; ++j ){
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


TVector3 TMust2Physics::GetPositionOfInteraction(const int i) const{
  TVector3 Position = TVector3 (   GetStripPositionX( TelescopeNumber[i] , Si_X[i] , Si_Y[i] )    ,
      GetStripPositionY( TelescopeNumber[i] , Si_X[i] , Si_Y[i] )      ,
      GetStripPositionZ( TelescopeNumber[i] , Si_X[i] , Si_Y[i] )      ) ;

  return(Position) ;

}

TVector3 TMust2Physics::GetTelescopeNormal( const int i) const{
  TVector3 U =    TVector3 ( GetStripPositionX( TelescopeNumber[i] , 128 , 1 ) ,
      GetStripPositionY( TelescopeNumber[i] , 128 , 1 ) ,
      GetStripPositionZ( TelescopeNumber[i] , 128 , 1 ) )

    -TVector3 ( GetStripPositionX( TelescopeNumber[i] , 1 , 1 ) ,
        GetStripPositionY( TelescopeNumber[i] , 1 , 1 ) ,
        GetStripPositionZ( TelescopeNumber[i] , 1 , 1 ) );

  TVector3 V =    TVector3 ( GetStripPositionX( TelescopeNumber[i] , 128 , 128 ) ,
      GetStripPositionY( TelescopeNumber[i] , 128 , 128 ) ,
      GetStripPositionZ( TelescopeNumber[i] , 128 , 128 ) )

    -TVector3 ( GetStripPositionX( TelescopeNumber[i] , 128 , 1 ) ,
        GetStripPositionY( TelescopeNumber[i] , 128 , 1 ) ,
        GetStripPositionZ( TelescopeNumber[i] , 128 , 1 ) );

  TVector3 Normal = U.Cross(V);

  return(Normal.Unit()) ;
}

///////////////////////////////////////////////////////////////////////////
namespace MUST2_LOCAL{

  //   tranform an integer to a string
  string itoa(int value){
    char buffer [33];
    sprintf(buffer,"%d",value);
    return buffer;
  }

  //   DSSD
  //   X
  double fSi_X_E(const TMust2Data* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "MUST2/T" + itoa( m_EventData->GetMMStripXEDetectorNbr(i) ) + "_Si_X" + itoa( m_EventData->GetMMStripXEStripNbr(i) ) + "_E",
        m_EventData->GetMMStripXEEnergy(i) );
  }

  double fSi_X_T(const TMust2Data* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "MUST2/T" + itoa( m_EventData->GetMMStripXTDetectorNbr(i) ) + "_Si_X" + itoa( m_EventData->GetMMStripXTStripNbr(i) ) +"_T",
        m_EventData->GetMMStripXTTime(i) );
  }

  //   Y
  double fSi_Y_E(const TMust2Data* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "MUST2/T" + itoa( m_EventData->GetMMStripYEDetectorNbr(i) ) + "_Si_Y" + itoa( m_EventData->GetMMStripYEStripNbr(i) ) +"_E",
        m_EventData->GetMMStripYEEnergy(i) );
  }

  double fSi_Y_T(const TMust2Data* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "MUST2/T" + itoa( m_EventData->GetMMStripYTDetectorNbr(i) ) + "_Si_Y" + itoa( m_EventData->GetMMStripYTStripNbr(i) ) +"_T",
        m_EventData->GetMMStripYTTime(i) );
  }


  //   SiLi
  double fSiLi_E(const TMust2Data* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "MUST2/T" + itoa( m_EventData->GetMMSiLiEDetectorNbr(i) ) + "_SiLi" + itoa( m_EventData->GetMMSiLiEPadNbr(i) ) +"_E",
        m_EventData->GetMMSiLiEEnergy(i) );
  }

  double fSiLi_T(const TMust2Data* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "MUST2/T" + itoa( m_EventData->GetMMSiLiTDetectorNbr(i) ) + "_SiLi" + itoa( m_EventData->GetMMSiLiTPadNbr(i) )+"_T",
        m_EventData->GetMMSiLiTTime(i) );
  }

  //   CsI
  double fCsI_E(const TMust2Data* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "MUST2/T" + itoa( m_EventData->GetMMCsIEDetectorNbr(i) ) + "_CsI" + itoa( m_EventData->GetMMCsIECristalNbr(i) ) +"_E",
        m_EventData->GetMMCsIEEnergy(i) );
  }

  double fCsI_T(const TMust2Data* m_EventData , const int i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "MUST2/T" + itoa( m_EventData->GetMMCsITDetectorNbr(i) ) + "_CsI" + itoa( m_EventData->GetMMCsITCristalNbr(i) ) +"_T",
        m_EventData->GetMMCsITTime(i) );
  }

}


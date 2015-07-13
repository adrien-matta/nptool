/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                  Peter Bender  contact address: bender@triumf.ca          *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Tigress treated data                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// STL
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

#include "TTigressPhysics.h"

//   NPL
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
//   ROOT
#include "TChain.h"
#include "TLorentzVector.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TTigressPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TTigressPhysics::TTigressPhysics()  {
    m_EventData         = new TTigressData ;
    m_PreTreatedData    = new TTigressData ;
    m_EventPhysics      = this ;
    Clear();
  };


/////////////////////////////////////////////////
void TTigressPhysics::BuildPhysicalEvent(){
  PreTreat();

  for(unsigned int i = 0 ; i < m_EventData->GetMultiplicityGe() ; i++){
    if( m_EventData->GetGeSegmentNbr(i)==0 && m_EventData->GetGeEnergy(i)>20){
      int clover = m_EventData->GetGeCloverNbr(i);
      int cry = m_EventData->GetGeCrystalNbr(i);
      double Energy =  CalibrationManager::getInstance()->ApplyCalibration("TIGRESS/D"+ NPL::itoa(clover)+"_CRY"+ NPL::itoa(cry)+"_SEG"+ NPL::itoa(m_EventData->GetGeSegmentNbr(i))+"_E", m_EventData->GetGeEnergy(i));
      Gamma_Energy.push_back(Energy);
      Crystal_Number.push_back(m_EventData->GetGeCrystalNbr(i));
      Clover_Number.push_back(m_EventData->GetGeCloverNbr(i));
      Gamma_Time.push_back(m_EventData->GetGeTimeCFD(i));
      // Look for Associate BGO
      bool BGOcheck = false ;
      for(unsigned j = 0 ;  j <  m_EventData->GetMultiplicityBGO() ; j++){

        if( m_EventData->GetBGOCloverNbr(j)== m_EventData->GetGeCloverNbr(i) && m_EventData->GetBGOEnergy(j)>20 )
          BGOcheck = true ;
      }
      BGO.push_back(BGOcheck);
    }
  }

  map<int, double> map_E;
  for(unsigned int i = 0 ; i < 16 ; i ++) {
    unsigned int mysize = Gamma_Energy.size();
    for(unsigned int g = 0 ; g < mysize ; g++){
      if(Clover_Number[g] == i+1){
        map_E[i] += Gamma_Energy[g]  ; 
      }
    }
  }


  for(unsigned int i = 0 ; i < 16 ; i ++) {
    if(map_E[i]>0){
      AddBack_E.push_back(map_E[i]);
      if(i>12){
        TLorentzVector GammaLV( map_E[i]*sin(135*3.1459/180.),
            map_E[i]*sin(135*3.1459/180.),
            map_E[i]*cos(135*3.1459/180.),
            map_E[i]);
        
        GammaLV.Boost(0,0,-0.12);
        AddBack_DC.push_back(GammaLV.Energy());
      }
      else
         AddBack_DC.push_back(map_E[i]); 
    }
  }

}
/////////////////////////////////////////////////
TVector3 TTigressPhysics::GetPositionOfInteraction(int i){
  TVector3 dummy;

  return dummy;
}
/////////////////////////////////////////////////
void TTigressPhysics::PreTreat(){


} 

/////////////////////////////////////////////////
void TTigressPhysics::Clear() {
};


/////////////////////////////////////////////////
void TTigressPhysics::ReadConfiguration(string Path)  {
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;

  if(!ConfigFile.is_open()) cout << "Config File not Found" << endl ;

  string LineBuffer             ;
  string DataBuffer             ;

  bool check_CloverId= false          ;
  bool check_R= false          ; 
  bool check_Theta= false          ;
  bool check_Phi= false          ;
  bool ReadingStatus = true;

  int CloverId=0;
  double R=0;
  double Theta=0;
  double Phi=0;

  while (!ConfigFile.eof()) {

    getline(ConfigFile, LineBuffer);
    //   If line is a Start Up Tigress bloc, Reading toggle to true      
    if (LineBuffer.compare(0, 13, "TigressClover") == 0) {
      cout << "///" << endl ;
      cout << "Tigress Clover found: " << endl ;        
      ReadingStatus = true ;
    }
    //   Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    //   Reading Block
    while(ReadingStatus)  {
      // Pickup Next Word 

      ConfigFile >> DataBuffer ;
      //   Comment Line 

      if (DataBuffer.compare(0, 1, "%") == 0) {ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

      //   Finding another Clover toggle out (safety)
      else if (DataBuffer.compare(0, 13, "TigressClover") == 0) {
        cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in Clover definition" << endl ;
        ReadingStatus = false ;
      }

      else if (DataBuffer=="CloverId=") {
        check_CloverId = true;
        ConfigFile >> DataBuffer ;
        CloverId=atoi(DataBuffer.c_str());
        cout << "CloverId:  " << CloverId << endl;
      }

      else if (DataBuffer=="R=") {
        check_R = true;
        ConfigFile >> DataBuffer ;
        R = atof(DataBuffer.c_str());
        cout << "R:  " << R << "mm" << endl;
      }

      else if (DataBuffer=="Theta=") {
        check_Theta = true;
        ConfigFile >> DataBuffer ;
        Theta = atof(DataBuffer.c_str());
        cout << "Theta:  " << Theta << "deg" << endl;
      }

      else if (DataBuffer=="Phi=") {
        check_Phi = true;
        ConfigFile >> DataBuffer ;
        Phi = atof(DataBuffer.c_str());
        cout << "Phi:  " << Phi << "deg" << endl;
      }

      ///////////////////////////////////////////////////
      //   If no Detector Token and no comment, toggle out
      else {
        ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out
      if ( check_Theta && check_Phi && check_R && check_CloverId) { 
        ReadingStatus = false;
        check_CloverId= false;
        check_R= false; 
        check_Theta= false;
        check_Phi= false;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TTigressPhysics::InitializeRootInputRaw() 
{
  TChain* inputChain = RootInput::getInstance()->GetChain()   ;
  inputChain->SetBranchStatus( "Tigress" , true )               ;
  inputChain->SetBranchStatus( "fTIG_*" , true )               ;
  inputChain->SetBranchAddress( "Tigress" , &m_EventData )      ;
}

///////////////////////////////////////////////////////////////////////////
void TTigressPhysics::InitializeRootOutput()    
{
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "Tigress" , "TTigressPhysics" , &m_EventPhysics );
}
///////////////////////////////////////////////////////////////////////////  
void TTigressPhysics::ClearEventPhysics() {
  Gamma_Energy.clear();
  Gamma_Time.clear();
  Crystal_Number.clear();
  Clover_Number.clear();
  BGO.clear();
  AddBack_E.clear();
  AddBack_DC.clear();
}
///////////////////////////////////////////////////////////////////////////  
void TTigressPhysics::ClearEventData() {

  m_EventData->Clear();
  m_PreTreatedData->Clear();
}
///////////////////////////////////////////////////////////////////////////
void TTigressPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for(int i = 0 ; i < 16; ++i){
    for(int cry = 0 ; cry < 4 ; cry++){
      // core are 0 and 9 , segment 1 to 8
      for( int j = 0 ; j < 10 ; ++j){
        Cal->AddParameter("TIGRESS", "D"+ NPL::itoa(i+1)+"_CRY"+NPL::itoa(cry+1)+"_SEG"+ NPL::itoa(j)+"_E","TIGRESS_D"+ NPL::itoa(i+1)+"_CRY"+NPL::itoa(cry+1)+"_SEG"+NPL::itoa(j)+"_E");
      }
    }
  } 
  return;
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TTigressPhysics::Construct(){
  return (NPL::VDetector*) new TTigressPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy{
  public:
    proxy(){
      NPL::DetectorFactory::getInstance()->AddToken("Tigress","Tigress");
      NPL::DetectorFactory::getInstance()->AddDetector("Tigress",TTigressPhysics::Construct);
    }
};

proxy p;
}


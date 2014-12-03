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
#include "RootOutput.h"
#include "TAsciiFile.h"
//   ROOT
#include "TChain.h"
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

  //if(m_PreTreatedData->GetMultiplicityGe()<11){

    vector < vector < unsigned int > > HitIndex;
    vector < vector < unsigned int > > Seg_HitIndex;
    //vector<unsigned int> Number;
    //Number.resize(4);
    HitIndex.resize(16);
    Seg_HitIndex.resize(16);
    
    for(unsigned int i = 0 ; i < m_PreTreatedData->GetMultiplicityGe() ; i++){
      int Segment = m_PreTreatedData->GetGeSegmentNbr(i);
      if( Segment==9 && m_PreTreatedData->GetGeEnergy(i)>20){
        HitIndex[m_PreTreatedData->GetGeCloverNbr(i)-1].push_back(i);
      }
      else if( (Segment!=9 || Segment!=0) &&  m_PreTreatedData->GetGeEnergy(i)>20){
        Seg_HitIndex[m_PreTreatedData->GetGeCloverNbr(i)-1].push_back(i);
      }
    }

    //Applying Addback

    for(unsigned int clover = 0; clover<HitIndex.size(); clover++){
      bool BGOcheck;
      int seg_index, Core_Crystal;
      double E_sum, Seg_Energy, Max_Seg_Energy = -1000;

      if(HitIndex[clover].size() == 1) {
        Gamma_Energy.push_back(m_PreTreatedData->GetGeEnergy(HitIndex[clover][0]) );
        Clover_Number.push_back(m_PreTreatedData->GetGeCloverNbr(HitIndex[clover][0]) );
        Crystal_Number.push_back(m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][0]) );

        for(unsigned int seg = 0; seg<Seg_HitIndex[clover].size(); seg++ ){
          Seg_Energy = m_PreTreatedData->GetGeEnergy(Seg_HitIndex[clover][seg]);
          if(Seg_Energy > Max_Seg_Energy){
            Max_Seg_Energy = Seg_Energy;
            seg_index = Seg_HitIndex[clover][seg];
          }
        }
        //Segment_Number.push_back(m_PreTreatedData->GetGeSegmentNbr(seg_index));

        BGOcheck = false ;
        //if( m_PreTreatedData->GetMultiplicityBGO()== 0){cout << "This is zero\n";}
        for(unsigned j = 0 ;  j <  m_PreTreatedData->GetMultiplicityBGO() ; j++){
          int BGO_Crystal = m_PreTreatedData->GetBGOCrystalNbr(j);
          int Ge_Crystal = m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][0]);
          if( m_PreTreatedData->GetBGOCloverNbr(j) == clover+1 && BGO_Crystal == Ge_Crystal && m_PreTreatedData->GetBGOEnergy(j)>20 ){

            if(m_PreTreatedData->GetGeEnergy(HitIndex[clover][0]) > 119 && m_PreTreatedData->GetGeEnergy(HitIndex[clover][0]) < 125){
              cout << "\t\t" << m_PreTreatedData->GetBGOCloverNbr(j) << "\t" << m_PreTreatedData->GetGeCloverNbr(HitIndex[clover][0]) 
              << "\t" << BGO_Crystal << "\t" << Ge_Crystal << endl;
            }
            BGOcheck = true ;
          }        
        }
        BGO.push_back(BGOcheck);
      }
      else if(HitIndex[clover].size() == 2){
        unsigned int Cr1 = m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][0]);
        unsigned int Cr2 = m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][1]);
        if(AdjacentCrystal(Cr1, Cr2)) {         //Case where crystals are adjacent
          E_sum = m_PreTreatedData->GetGeEnergy(HitIndex[clover][0]) + m_PreTreatedData->GetGeCloverNbr(HitIndex[clover][1]);

          Gamma_Energy.push_back( E_sum );
          Clover_Number.push_back(m_PreTreatedData->GetGeCloverNbr(HitIndex[clover][0]) );
          Crystal_Number.push_back(m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][0]) );

          for(unsigned int seg = 0; seg<Seg_HitIndex[clover].size(); seg++ ){
            Seg_Energy = m_PreTreatedData->GetGeEnergy(Seg_HitIndex[clover][seg]);
              if(Seg_Energy > Max_Seg_Energy){
                Max_Seg_Energy = Seg_Energy;
                seg_index = Seg_HitIndex[clover][seg];
              }
          }
          //Segment_Number.push_back(m_PreTreatedData->GetGeSegmentNbr(seg_index));

          BGOcheck = false;
          for(unsigned int j = 0 ;  j <  m_PreTreatedData->GetMultiplicityBGO() ; j++){
          int BGO_Crystal = m_PreTreatedData->GetBGOCrystalNbr(j);
          int Ge_Crystal = m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][0]);
            if( m_PreTreatedData->GetBGOCloverNbr(j)== clover+1 && m_PreTreatedData->GetBGOEnergy(j)>20 ){  
              BGOcheck = true ;
              
            }
          
          }
          BGO.push_back(BGOcheck);
        }
        else if (!AdjacentCrystal(Cr1, Cr2)) {  //Case were crystals are not adjacent
          Core_Crystal = m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][0]);

          Gamma_Energy.push_back(m_PreTreatedData->GetGeEnergy(HitIndex[clover][0]) );
          Clover_Number.push_back(m_PreTreatedData->GetGeCloverNbr(HitIndex[clover][0]) );
          Crystal_Number.push_back(m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][0]) );

          for(unsigned int seg = 0; seg<Seg_HitIndex[clover].size(); seg++ ){
            Seg_Energy = m_PreTreatedData->GetGeEnergy(Seg_HitIndex[clover][seg]);
            if(Seg_Energy > Max_Seg_Energy && Core_Crystal == m_PreTreatedData->GetGeCrystalNbr(Seg_HitIndex[clover][seg])){
              Max_Seg_Energy = Seg_Energy;
              seg_index = Seg_HitIndex[clover][seg];
            }
          }
          //Segment_Number.push_back(m_PreTreatedData->GetGeSegmentNbr(seg_index));
          Max_Seg_Energy = -10000;

          BGOcheck = false ;
          for(unsigned int j = 0 ;  j <  m_PreTreatedData->GetMultiplicityBGO() ; j++){
          int BGO_Crystal = m_PreTreatedData->GetBGOCrystalNbr(j);
          int Ge_Crystal = m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][0]);
            if( m_PreTreatedData->GetBGOCloverNbr(j)== clover+1 && m_PreTreatedData->GetBGOEnergy(j)>20 ){
               BGOcheck = true ;
            }
          
          }
          BGO.push_back(BGOcheck);

          Core_Crystal = m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][1]);

          Gamma_Energy.push_back(m_PreTreatedData->GetGeEnergy(HitIndex[clover][1]) );
          Clover_Number.push_back(m_PreTreatedData->GetGeCloverNbr(HitIndex[clover][1]) );
          Crystal_Number.push_back(m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][1]) );

          for(unsigned int seg = 0; seg<Seg_HitIndex[clover].size(); seg++ ){
            Seg_Energy = m_PreTreatedData->GetGeEnergy(Seg_HitIndex[clover][seg]);
            if(Seg_Energy > Max_Seg_Energy && Core_Crystal == m_PreTreatedData->GetGeCrystalNbr(Seg_HitIndex[clover][seg])){
              Max_Seg_Energy = Seg_Energy;
              seg_index = Seg_HitIndex[clover][seg];
            }
          }
          //Segment_Number.push_back(m_PreTreatedData->GetGeSegmentNbr(seg_index));

          BGOcheck = false ;
          for(unsigned int j = 0 ;  j <  m_PreTreatedData->GetMultiplicityBGO() ; j++){
          int BGO_Crystal = m_PreTreatedData->GetBGOCrystalNbr(j);
          int Ge_Crystal = m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][0]);
            if( m_PreTreatedData->GetBGOCloverNbr(j)== clover+1 && m_PreTreatedData->GetBGOEnergy(j)>20 ){
               BGOcheck = true ;
            }
          
          }
          BGO.push_back(BGOcheck);
        }                 
      } 
      else  continue;
    }
    if(Gamma_Energy.size()!=BGO.size()){
      cout << "Herer" << BGO.size() << "\t" << Gamma_Energy.size() << endl;
    }
    HitIndex.clear();
  //}
  
  

}
/////////////////////////////////////////////////
TVector3 TTigressPhysics::GetPositionOfInteraction(int i){
}
/////////////////////////////////////////////////
void TTigressPhysics::PreTreat(){
  ClearPreTreatedData();
 
  //Calibration vector not standard in NPTool. Standardise later
  ifstream CalFile;
  //CalFile.open("/home/ak00128/Desktop/PhD/S1107/GammaCal/Calibration_Parameters_152Eu.dat");
  CalFile.open("/home/ak00128/Desktop/PhD/S1107/GammaCal/Calibration_Parameters_133Ba.dat");

  int Clover, Crystal, Segment, Count, u = 480;
  double par_a, par_b;
  vector< double > vClover;
  vector< double > vCrystal;
  vector< double > vSegment;
  vector< double > vCount;
  vector< double > vPar_A;
  vector< double > vPar_B;

  if(CalFile.is_open()){
    for(unsigned int loop = 0; loop<u; loop++){
      
      CalFile >> Clover >> Crystal >> Segment >> Count >> par_a >> par_b;
      vClover.push_back(Clover);
      vCrystal.push_back(Crystal);
      vSegment.push_back(Segment);
      vCount.push_back(Count);
      vPar_A.push_back(par_a);
      vPar_B.push_back(par_b);
    }
  }
  
  CalFile.close();
  //Ge Crystals
  for(unsigned int i = 0 ; i < m_EventData->GetMultiplicityGe(); ++i){
    double grad, intercept;
    for(unsigned int vec = 0; vec < u; vec++){
      if( (vClover[vec] == m_EventData->GetGeCloverNbr(i) ) && (vCrystal[vec] == m_EventData->GetGeCrystalNbr(i) )
      && (vSegment[vec] == m_EventData->GetGeSegmentNbr(i) ) ){
        grad = vPar_A[vec];
        intercept = vPar_B[vec];
        break;
      }
    }
    double Energy = (m_EventData->GetGeEnergy(i))*grad + intercept;
    m_PreTreatedData->SetGeCloverNbr( m_EventData->GetGeCloverNbr(i) );
    m_PreTreatedData->SetGeCrystalNbr( m_EventData->GetGeCrystalNbr(i) );
    m_PreTreatedData->SetGeSegmentNbr( m_EventData->GetGeSegmentNbr(i) );
    m_PreTreatedData->SetGeEnergy( Energy );
    m_PreTreatedData->SetGeTimeCFD( m_EventData->GetGeTimeCFD(i) );
    m_PreTreatedData->SetGeTimeLED( m_EventData->GetGeTimeLED(i) );
    //cout << grad << "\t" << intercept << "\t" << endl;
  }


  //BGO
  for(unsigned int i = 0 ; i < m_EventData->GetMultiplicityBGO(); ++i){
    
    m_PreTreatedData->SetBGOCloverNbr( m_EventData->GetBGOCloverNbr(i) );
    m_PreTreatedData->SetBGOCrystalNbr( m_EventData->GetBGOCrystalNbr(i) );
    m_PreTreatedData->SetBGOEnergy( m_EventData->GetBGOEnergy(i) );
    m_PreTreatedData->SetBGOTimeCFD( m_EventData->GetBGOTimeCFD(i) );
    m_PreTreatedData->SetBGOTimeLED( m_EventData->GetBGOTimeLED(i) );

  }
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
bool TTigressPhysics::AdjacentCrystal(unsigned int CrNbr1, unsigned int CrNbr2)
  {
    if(CrNbr2 == CrNbr1 + 1 || CrNbr2 == CrNbr1 - 1){
      return true;
    }
    else if( (CrNbr1 == 1 && CrNbr2 == 4) || (CrNbr1 == 4 && CrNbr2 == 1) ){
      return true;
    }
    else {
      return false;
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
      outputTree->Branch( "TigressPreTreated" , "TTigressData" , &m_PreTreatedData );
   }
////////////////////////////////////////////////////////////////////////////  
void TTigressPhysics::ClearEventPhysics() {
  Gamma_Energy.clear();
  Crystal_Number.clear();
  Clover_Number.clear();
  //Segment_Number.clear();
  BGO.clear();
}
 ///////////////////////////////////////////////////////////////////////////  
void TTigressPhysics::ClearEventData() {

m_EventData->Clear();
m_PreTreatedData->Clear();
}
//////////////////////////////////////////////////////////////////////////
void TTigressPhysics::ClearPreTreatedData() {

  m_PreTreatedData->Clear();
}


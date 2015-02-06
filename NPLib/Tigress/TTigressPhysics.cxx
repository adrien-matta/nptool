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
#include <fstream>
#include <sstream>
using namespace std;

#include "TTigressPhysics.h"

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
using namespace NPUNITS;
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


        for(unsigned int seg = 0; seg < Seg_HitIndex[clover].size(); seg++){
          Seg_Energy = m_PreTreatedData->GetGeEnergy(Seg_HitIndex[clover][seg]);
          if(Seg_Energy > Max_Seg_Energy && Core_Crystal == m_PreTreatedData->GetGeCrystalNbr(Seg_HitIndex[clover][seg])){
            Max_Seg_Energy = Seg_Energy;
            seg_index = Seg_HitIndex[clover][seg];
          }
        }
        Segment_Number.push_back(m_PreTreatedData->GetGeSegmentNbr(seg_index));

        BGOcheck = false ;
        //if( m_PreTreatedData->GetMultiplicityBGO()== 0){cout << "This is zero\n";}
        for(unsigned j = 0 ;  j <  m_PreTreatedData->GetMultiplicityBGO() ; j++){
          int BGO_Crystal = m_PreTreatedData->GetBGOCrystalNbr(j);
          int Ge_Crystal = m_PreTreatedData->GetGeCrystalNbr(HitIndex[clover][0]);
          if( m_PreTreatedData->GetBGOCloverNbr(j) == clover+1 && BGO_Crystal == Ge_Crystal && m_PreTreatedData->GetBGOEnergy(j)>20 ){

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
              if(Seg_Energy > Max_Seg_Energy && Core_Crystal == m_PreTreatedData->GetGeCrystalNbr(Seg_HitIndex[clover][seg])){
                Max_Seg_Energy = Seg_Energy;
                seg_index = Seg_HitIndex[clover][seg];
              }
          }
          Segment_Number.push_back(m_PreTreatedData->GetGeSegmentNbr(seg_index));

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
          Segment_Number.push_back(m_PreTreatedData->GetGeSegmentNbr(seg_index));
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
          Segment_Number.push_back(m_PreTreatedData->GetGeSegmentNbr(seg_index));

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
      cout << "Here" << BGO.size() << "\t" << Gamma_Energy.size() << endl;
    }
    HitIndex.clear();
  //}
  
  

}
/////////////////////////////////////////////////
TVector3 TTigressPhysics::GetPositionOfInteraction(unsigned int i){
	
	//in mm
	//double FrontFace_dist = 145.0;
	double x ;
	double y ;
	double z ;
	//double clover;
	//double r, theta, phi;

	TVector3 Segment_POS = GetPositionOfSegment(i);
	TVector3 Clover_POS(0,0,1);
	TVector3 POS;
	
	Clover_POS.SetMag(m_R[i]);
	Clover_POS.SetTheta(m_Theta[i]);
	Clover_POS.SetPhi(m_Phi[i]);
	
	POS = Segment_POS + Clover_POS;
	return(Segment_POS + Clover_POS);
	
}
/////////////////////////////////////////////////
TVector3 TTigressPhysics::GetPositionOfSegment(unsigned int i ){
	
	//in mm
	
	double rseg_x;
	double rseg_y;
	double rseg_z;
	
	double crystal = m_PreTreatedData->GetGeCrystalNbr(i);
	double segment = m_PreTreatedData->GetGeSegmentNbr(i);
	
	if(segment == 1){
		rseg_x = 31.5;
		rseg_y = 31.5;
		rseg_z = 10;
	}
	else if(segment == 2) {
		rseg_x = 10.5;
		rseg_y = 31.5;
		rseg_z = 10;
	}
	else if(segment == 3) {
		rseg_x = 10.5;
		rseg_y = 10.5;
		rseg_z = 10;
	}
	else if(segment == 4) {
		rseg_x = 31.5;
		rseg_y = 10.5;
		rseg_z = 10;
	}
	else if(segment == 5) {
		rseg_x = 31.5;
		rseg_y = 31.5;
		rseg_z = 55;
	}
	else if(segment == 6) {
		rseg_x = 10.5;
		rseg_y = 31.5;
		rseg_z = 55;
	}
	else if(segment == 7) {
		rseg_x = 10.5;
		rseg_y = 10.5;
		rseg_z = 55;
	}
	else if(segment == 8) {
		rseg_x = 31.5;
		rseg_y = 10.5;
		rseg_z = 55;
	}
	TVector3 Segment_POS(rseg_x, rseg_y, rseg_z);
	Segment_POS.RotateZ(crystal*90*M_PI/180);
	return (Segment_POS);
	
	
}
/////////////////////////////////////////////////
void TTigressPhysics::AddCloverStandard(vector<int> CloverId){
  
  for (unsigned int i = 0 ;  i < CloverId.size(); i++) {
		//cout << CloverId.size() << endl;
    if(CloverId[i] == 1 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(45*deg);
      m_Phi.push_back(22.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(0);
    }
    
    else if(CloverId[i] == 2 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145);
      m_Theta.push_back(45*deg);
      m_Phi.push_back(112.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(0);
    }
    
    else if(CloverId[i] == 3 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(45*deg);
      m_Phi.push_back(202.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(0);
    }
    
    else if(CloverId[i] == 4 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(45*deg);
      m_Phi.push_back(292.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(0);
    }
    
    else if(CloverId[i] == 5 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(22.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 6 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(67.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 7 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(112.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 8 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(157.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 9 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(202.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 10 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(247.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 11 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(292.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 12 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(337.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 13 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(135*deg);
      m_Phi.push_back(22.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 14 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(135*deg);
      m_Phi.push_back(112.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 15 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(135*deg);
      m_Phi.push_back(202.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
    
    else if(CloverId[i] == 16 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(135*deg);
      m_Phi.push_back(292.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
  }

}
/////////////////////////////////////////////////
void TTigressPhysics::PreTreat(){
  ClearPreTreatedData();
 
  //Calibration vector not standard in NPTool. Standardise later
  ifstream CalFile;
  //CalFile.open("/home/ak00128/Desktop/PhD/S1107/GammaCal/Calibration_Parameters_152Eu.dat");
  CalFile.open("/home/ak00128/Desktop/PhD/S1107/GammaCal/Temporary/Calibration_Parameters.dat");

  int Clover, Crystal, Segment, Count;
  double par_a, par_b;
  vector< double > vClover;
  vector< double > vCrystal;
  vector< double > vSegment;
  vector< double > vCount;
  vector< double > vPar_A;
  vector< double > vPar_B;

  while(CalFile >> Clover >> Crystal >> Segment >> par_a >> par_b){
      vClover.push_back(Clover);
      vCrystal.push_back(Crystal);
      vSegment.push_back(Segment);
      vPar_A.push_back(par_a);
      vPar_B.push_back(par_b);
  }
  
  int u = vClover.size();
  
  CalFile.close();
  //Ge Crystals
  for(unsigned int i = 0 ; i < m_EventData->GetMultiplicityGe(); ++i){
    double grad=-1;
    double intercept=0;
    for(unsigned int vec = 0; vec < u; vec++){
      if( (vClover[vec] == m_EventData->GetGeCloverNbr(i) ) && (vCrystal[vec] == m_EventData->GetGeCrystalNbr(i) )
      && (vSegment[vec] == m_EventData->GetGeSegmentNbr(i) ) ){
        grad = vPar_A[vec];
        intercept = vPar_B[vec];
        break;
      }
    }
    if(grad>0){
    double Energy = (m_EventData->GetGeEnergy(i))*grad + intercept;
    m_PreTreatedData->SetGeCloverNbr( m_EventData->GetGeCloverNbr(i) );
    m_PreTreatedData->SetGeCrystalNbr( m_EventData->GetGeCrystalNbr(i) );
    m_PreTreatedData->SetGeSegmentNbr( m_EventData->GetGeSegmentNbr(i) );
    m_PreTreatedData->SetGeEnergy( Energy );
    m_PreTreatedData->SetGeTimeCFD( m_EventData->GetGeTimeCFD(i) );
    m_PreTreatedData->SetGeTimeLED( m_EventData->GetGeTimeLED(i) );
    }
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
  string LineBuffer             ;
  string DataBuffer             ;
  istringstream LineStream      ;
  // Standard Case:
  bool check_CloverId = false;
  
  vector<int> CloverId;
  int    CloverId_Free = 0;
  double R     = 0;
  double Theta = 0;
  double Phi   = 0;
  double BetaX;
  double BetaY;
  double BetaZ;
  
  // Free postion case:
  bool check_R   = false ;
  bool check_Theta = false ;
  bool check_Phi = false ;
  bool check_Beta = false ;
  
  // Frame Case
  bool check_RightFrame = false ;
  bool check_LeftFrame = false ;
  
  bool ReadingStatusStandard = false ;
  bool ReadingStatusFree = false ;
  bool ReadingStatusFrame = false ;
  bool ReadingStatus = false ;
  
  while (!ConfigFile.eof()){
    int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();
    
    getline(ConfigFile, LineBuffer);
    
    if (LineBuffer.compare(0, 7, "Tigress") == 0)
      ReadingStatus = true;
    
    while (ReadingStatus && !ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      
      //   Comment Line
      while (LineBuffer.compare(0, 1, "%") == 0) {
        // Take the next line
        getline(ConfigFile, LineBuffer);
      }
      
      //   Standard case
      if (LineBuffer.compare(0, 15, "TigressStandard") == 0){
        if(VerboseLevel==1)
          cout << "/// Clovers in Standard Configuration : ///" << endl   ;
        ReadingStatusStandard = true ;
      }
      
      //  Free placing case
      else if (LineBuffer.compare(0, 13, "TigressClover") == 0){
        if(VerboseLevel==1)
          cout << "/// Free placed clover : ///" << endl   ;
        ReadingStatusFree = true ;
      }
      
      //  Frame case
      else if (LineBuffer.compare(0, 12, "TigressFrame") == 0){
        if(VerboseLevel==1)
          cout << "/// Support Frame : ///" << endl   ;
        ReadingStatusFrame = true ;
      }
      
      //   Reading Block
      while(ReadingStatusStandard){
        // Pickup Next Line
        getline(ConfigFile, LineBuffer);
        //   Comment Line
        while (LineBuffer.compare(0, 1, "%") == 0) {
          // Take the next line
          getline(ConfigFile, LineBuffer);
        }
        
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;

        if ( DataBuffer == "CloverId=" ) {
          check_CloverId = true;
                    
          if(VerboseLevel==1) cout << "CloverId: " ;
          while(LineStream >> DataBuffer){
            CloverId.push_back(atoi(DataBuffer.c_str()));
            if(VerboseLevel==1) cout << atoi(DataBuffer.c_str()) << " ";
          }
          if(VerboseLevel==1) cout << endl << endl;
        }
      
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusStandard = false;
          cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_CloverId){
          ReadingStatusStandard = false;
          AddCloverStandard(CloverId);
          CloverId.clear();
          check_CloverId   = false ;
        }
      }
      
      //   Reading Block
      while(ReadingStatusFree){
        // Pickup Next Line
        getline(ConfigFile, LineBuffer);
        //   Comment Line
        while (LineBuffer.compare(0, 1, "%") == 0) {
          // Take the next line
          getline(ConfigFile, LineBuffer);
        }
        
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        
        if ( DataBuffer == "CloverId=" ) {
          check_CloverId = true;
          LineStream >> DataBuffer;
          CloverId_Free = atoi(DataBuffer.c_str());
          if(VerboseLevel==1)
            cout << "CloverId: " << atoi(DataBuffer.c_str()) << " " << endl ;
        }
        
        else if ( DataBuffer == "R=" ) {
          check_R = true;
          LineStream >> DataBuffer;
          R = atof(DataBuffer.c_str())*mm;
          if(VerboseLevel==1)
            cout << "R: " << R/mm << " " << endl ;
        }
        
        else if ( DataBuffer == "Theta=" ) {
          check_Theta = true;
          LineStream >> DataBuffer;
          Theta = atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1)
            cout << "Theta: " << Theta/deg << " " << endl ;
        }
        
        else if ( DataBuffer == "Phi=" ) {
          check_Phi = true;
          LineStream >> DataBuffer;
          Phi = atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1)
            cout << "Phi: " << Phi/deg << " " << endl ;
        }
        
        else if ( DataBuffer == "Beta=" ) {
          check_Beta = true;
          LineStream >> DataBuffer;
          BetaX = atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1)
            cout << "BetaX: " << BetaX/deg << " " << endl ;
          LineStream >> DataBuffer;
          BetaY = atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1)
            cout << "BetaY: " << BetaY/deg << " " << endl ;
          LineStream >> DataBuffer;
          BetaZ = atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1)
            cout << "BetaZ: " << BetaZ/deg << " " << endl ;
        }
        
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusStandard = false;
          cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_CloverId && check_R && check_Theta && check_Phi && check_Beta){
          ReadingStatusFree = false;
          AddCloverFreePosition(CloverId_Free,R,Theta,Phi,BetaX,BetaY,BetaZ);
          check_CloverId = false ;
          check_R = false ;
          check_Theta = false ;
          check_Phi = false ;
          check_Beta = false ;
        }
      }
      
      //   Reading Block
      while(ReadingStatusFrame){
        // Pickup Next Line
        getline(ConfigFile, LineBuffer);
        //   Comment Line
        while (LineBuffer.compare(0, 1, "%") == 0) {
          // Take the next line
          getline(ConfigFile, LineBuffer);
        }
        
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        
        if ( DataBuffer == "RightFrame=" ) {
        // ignore token for simulation only
        }
        
        else if ( DataBuffer == "LeftFrame=" ) {
          // ignore token for simulation only
        }
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusStandard = false;
          cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << endl ;
          cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_RightFrame && check_LeftFrame){
          ReadingStatusFrame = false;
          AddCloverStandard(CloverId);
          CloverId.clear();
          check_RightFrame = false;
          check_LeftFrame = false;
        }
      }
      
    }
  }
}

///////////////////////////////////////////////////////////////////////////

void TTigressPhysics::AddCloverFreePosition(int CloverId,double R,double Theta,double Phi,double BetaX,double BetaY,double BetaZ){
  
  m_CloverId.push_back(CloverId);
  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
  m_BetaX.push_back(BetaX);
  m_BetaY.push_back(BetaY);
  m_BetaZ.push_back(BetaZ);
  
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
  Segment_Number.clear();
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


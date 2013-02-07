/*****************************************************************************
 * Copyright (C) 2008   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Vandebrouck   contact address: vandebro@ipno.in2p3.fr *
 *                  J. Gibelin                      gibelin@lpccaen.in2p3.fr *
 *                                                                           *
 * Creation Date  : November 2011                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds Maya raw data                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <TMath.h>
using namespace std;

#include "TMayaData.h"

ClassImp(TMayaData)

TMayaData::TMayaData()
  : fMaya_PAD_E_calpar(MAYA_PAD_NCHCAL,vector<Float_t>(MAYA_PAD_NPARCAL)),
    fMaya_QFIL_E_calpar(MAYA_QFIL_NCHCAL,vector<Float_t>(MAYA_QFIL_E_NPARCAL)),
    fMaya_QFIL_T_calpar(MAYA_QFIL_NCHCAL,vector<Float_t>(MAYA_QFIL_T_NPARCAL)),
    fMaya_Si_E_calpar(MAYA_SI_NCHCAL,vector<Float_t>(MAYA_SI_E_NPARCAL)),
    fMaya_Si_T_calpar(MAYA_SI_NCHCAL,vector<Float_t>(MAYA_SI_T_NPARCAL))
{
  // Default constructor
  fMaya_PAD_E_cal_threshold = 0. ;
  Clear();
  
}

TMayaData::~TMayaData()
{
  Clear();
  fMaya_PAD_E_calpar.clear();
  fMaya_QFIL_E_calpar.clear();
  fMaya_QFIL_T_calpar.clear();
  fMaya_Si_E_calpar.clear();
  fMaya_Si_T_calpar.clear();	
}

void TMayaData::Clear()
{
   // (PAD)
   bMaya_IsPAD_Calibrated = false ;
   fMaya_PAD_E.clear();
   fMaya_PAD_E_cal.clear();
   fMaya_PAD_Nbr.clear();
   fMaya_PAD_Col.clear();
   fMaya_PAD_Row.clear();
   fMaya_PAD_x.clear();
   fMaya_PAD_y.clear();
   fMaya_PAD_Sum = 0;
   fMaya_PAD_Sum_cal = 0;
   fMaya_PAD_Mult_cal = 0;

   fMaya_PAD_Neighbors.clear();	
   fMaya_PAD_map_idx.clear() ;
   memset(fMaya_PAD_Row_Mult,0,32*sizeof(UShort_t)) ;
   memset(fMaya_PAD_Row_Sum_cal,0,32*sizeof(Float_t)) ;
   memset(fMaya_PAD_Row_Max_cal,0,32*sizeof(Float_t)) ;
   memset(fMaya_PAD_Col_Mult,0,32*sizeof(UShort_t)) ;
   memset(fMaya_PAD_Col_Sum_cal,0,32*sizeof(Float_t)) ;
   memset(fMaya_PAD_Col_Max_cal,0,32*sizeof(Float_t)) ;

   // (QFIL)
   bMaya_IsQFIL_Calibrated = false ;
   fMaya_QFIL_E_Nbr.clear();
   fMaya_QFIL_E.clear();
   fMaya_QFIL_E_cal.clear();
   fMaya_QFIL_T.clear();
   fMaya_QFIL_T_cal.clear();
   fMaya_QFIL_T_Nbr.clear();
   fMaya_QFIL_Sum = 0 ;   
   fMaya_QFIL_Sum_cal = 0 ;   
   fMaya_QFil_Pulse_Nbr.clear();
   fMaya_QFil_Pulse_E.clear();
   fMaya_QFil_Pulse_Ch.clear();
   fMaya_QFil_Pulse_map_Current_Nbr.clear();
   fMaya_QFil_Pulse_Sum_Ch.clear();
   fMaya_QFil_Pulse_Sum.clear();
   // (Max) 
   //   fMaya_Max_cal = 0.;
   //   fMaya_Max_Nbr = 0 ;

   // Silicons
   fMaya_Si_E_Nbr.clear();
   fMaya_Si_E.clear();
   fMaya_Si_E_cal.clear();
   //
   fMaya_Si_T_Nbr.clear();
   fMaya_Si_T.clear();
   fMaya_Si_T_cal.clear();
}

void TMayaData::SetEnergyPad(UShort_t N, UShort_t E) {
  SetEnergyPad_CR(GetPadCol(N),GetPadRow(N),E);
}

void TMayaData::SetEnergyPad_CR(UShort_t C,UShort_t R, UShort_t E) {
  /*  cout << 
    fMaya_PAD_Col.size() << " " <<
    fMaya_PAD_Row.size() << " " <<
    fMaya_PAD_Nbr.size() << " " <<
    fMaya_PAD_E.size() << " " <<
    fMaya_PAD_map_idx.size() << endl ;
  */
  //
  
  fMaya_PAD_map_idx[GetPadIdx(C,R)] = fMaya_PAD_Nbr.size()-1 ;

  fMaya_PAD_Col.push_back(C);
  fMaya_PAD_Row.push_back(R);
  fMaya_PAD_x.push_back(MAYA_PAD_HEIGHT*(2*C-R%2));
  fMaya_PAD_y.push_back((1+(R-1)*1.5)*MAYA_PAD_SIZE);
  fMaya_PAD_Nbr.push_back(GetPadIdx(C,R));
  fMaya_PAD_E.push_back(E);
  
  //
  
  fMaya_PAD_map_idx[GetPadIdx(C,R)] = fMaya_PAD_Nbr.size()-1 ;

}

// SI
void  TMayaData::SetEnergySi(UShort_t N,UShort_t E)  {
  fMaya_Si_E.push_back(E) ;
  fMaya_Si_E_Nbr.push_back(N) ;

}

void  TMayaData::SetTimeSi(UShort_t N,UShort_t T)  {
  fMaya_Si_T.push_back(T) ;
  fMaya_Si_T_Nbr.push_back(N) ;

}

void TMayaData::SetQFilPulse(UShort_t N,UShort_t E) {
  fMaya_QFil_Pulse_map_Current_Nbr[N]++ ;
  if(E>0){
    fMaya_QFil_Pulse_Nbr.push_back(fMaya_QFil_Pulse_map_Current_Nbr[N]);
    fMaya_QFil_Pulse_Ch.push_back(N);
    fMaya_QFil_Pulse_E.push_back(E);
    for (Int_t i = 0; i<(Int_t)fMaya_QFil_Pulse_Sum_Ch.size();i++){
      // cout << fMaya_QFil_Pulse_Sum_Nbr.at(i) << endl;
      if(fMaya_QFil_Pulse_Sum_Ch.at(i) == N) {
	fMaya_QFil_Pulse_Sum.at(i) += E ;
	return ;
      }
    }
    fMaya_QFil_Pulse_Sum_Ch.push_back(N);
    fMaya_QFil_Pulse_Sum.push_back(E);
  }
}

// TOF
void  TMayaData::SetTOF(UShort_t N,UShort_t T)  {
  if(N==1) {
    fMaya_TOF_GAL_HF = T ;
    return ;
  }
  if(N==2) {
    fMaya_TOF_GAL_DIAM = T ;
    return ;
  }
  if(N==3) {
    fMaya_TOF_TM_TRIG = T ;
    return ;
  }
}


void TMayaData::Dump() const
{
   //cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
   /*
   // (PAD)
   cout << "Maya_PQD.Size = " << fMaya_PAD_E.size() << endl;
   for (UShort_t i = 0; i < fMaya_PAD_E.size(); i++)
     cout <<  " Energy: " << fMaya_PAD_E.at(i) << endl;
  //sum 
   cout<<"Maya_sum " << fMaya_Sum << endl;
  // QFIL 
   cout<<"Maya_QFIL_E " << fMaya_QFIL_E << endl;
  // Si
   cout<<"Maya_Si_E " << fMaya_Si_E << endl;
   */
}

bool TMayaData::InitCalibrate( std::vector<string> &list_of_cal_files)
{

  ConfigReader *cal_par = new ConfigReader(list_of_cal_files);
  cal_par->Read<Float_t>("PADS",fMaya_PAD_E_calpar);
  cal_par->Read<Float_t>("PADS_THR",fMaya_PAD_E_cal_threshold);
  cal_par->Read<bool>("PADS_CLEAN", bMaya_Clean_PADS);
  cal_par->Read<Int_t>("PADS_MIN_NEIGHBORS",  fMaya_PAD_Neighbors_Min);
  cal_par->Read<Float_t>("FILS_TIMECAL",  fMaya_QFIL_T_calpar);
  cal_par->Read<Float_t>("SI",  fMaya_Si_E_calpar);

  //cal_par->Read("tsi",fMaya_Si_E_calpar);
  //     cal_par->Dump();
//  vector<Float_t> u(MAYA_PAD_NPARCAL,0) ;
//  u.at(1) = 1 ;
//  for(unsigned int i=0;i<fMaya_PAD_E_calpar.size();i++){
//    fMaya_PAD_E_calpar.at(i) = u ;
//  }
    //fMaya_PAD_E_calpar.clear();
  delete cal_par ;
  return kTRUE ;
}


bool TMayaData::Calibrate(){

  //FILE *fichier_test;
  //fichier_test = fopen("f.dat","w");
  int tmpNbr ;
  Float_t tmpCal ;
  fMaya_PAD_Mult_cal = 0 ;
  fMaya_PAD_Sum_cal = 0 ;
  //possible example 
  //if(!fMaya_PAD_E.empty()&& !fMaya_PAD_E_calpar.empty() )
{

    fMaya_PAD_E_cal.resize(fMaya_PAD_E.size());

    for(unsigned int i=0;i<fMaya_PAD_E.size();i++){

      tmpCal = 0 ; 

      tmpNbr = fMaya_PAD_Nbr.at(i) - 1 ;

      if(fMaya_PAD_Nbr.at(i)<fMaya_PAD_E_calpar.size()){
	for(unsigned int j=0;j<fMaya_PAD_E_calpar.at(tmpNbr).size();j++){
	  tmpCal += pow((double)fMaya_PAD_E.at(i),(double)j)*fMaya_PAD_E_calpar.at(tmpNbr).at(j) ;
	}
	if(tmpCal>fMaya_PAD_E_cal_threshold){
	  fMaya_PAD_E_cal.at(i) = tmpCal ;
	  fMaya_PAD_Mult_cal++ ;
	  fMaya_PAD_Row_Mult[ fMaya_PAD_Row.at(i)-1]++ ;
	  fMaya_PAD_Col_Mult[ fMaya_PAD_Col.at(i)-1]++ ;
	  fMaya_PAD_Sum_cal += tmpCal ;
	} else {
	  fMaya_PAD_E_cal.at(i) = 0. ;
	}
	//cout << fMaya_PAD_E.at(i) << " " <<
        //fMaya_PAD_E_cal.at(i) << endl ;
      }

    }
    //    cout << endl ;
    bMaya_IsPAD_Calibrated = true ;
  } 


//else {
//cout << "out Pad " << fMaya_PAD_E.size() << " " << fMaya_PAD_E_calpar.size() << endl ;
 //   fMaya_PAD_E_cal.clear();
//    return false ;
 // }  

  if(bMaya_IsPAD_Calibrated && bMaya_Clean_PADS){
    // count the number of neighbors
    // two cases have to be considered :
    /*
      
         c-1  c   c+1  
       c-1  c  c+1
        |   |   |   |
       / \ / \ / \ / \
  r+2 |   |   |   |   |   odd 
       \ / \ / \ / \ /
  r+1   |   | B |   |     even 
       / \ / \ / \ / \
  r   |   | A |   |   |   odd  : ( r )%2 = 1
       \ / \ / \ / \ /
  r-1   |   |   |   |     even : (r-1)%2 = 0
       / \ / \ / \ / \
  r-2 |   |   |   |   |   odd  : (r-2)%2 = 1
       \ / \ / \ / \ /
     -------------------


	Example:

    * A(c,r) neighbors: (c-1,r-1)(c-1,r)(c-1,r+1)(c,r-1)(c+1,r)(c,r+1); 

    * B(c,r') neighbors: (c,r'-1)(c-1,r')(c,r'+1)(c+1,r'-1)(c+1,r')(c+1,r'+1)

Conclusion : all have two times r-1,r,r+1 combinaisons
	odd  raws (A) don't have (c+1,r-1)(c+1,r+1)  , note r %2 = 1
	even raws (B) don't have (c-1,r'-1)(c-1,r'+1), note r'%2 = 0

    */
	Int_t r0,c0 ;	
    for(unsigned int i=0;i<fMaya_PAD_E_cal.size();i++){
      fMaya_PAD_Neighbors.push_back(-1) ; // -1 because in the following
				      // the pad (c,r) will be also tested
      if(fMaya_PAD_E_cal.at(i)>0){
	r0 = fMaya_PAD_Row.at(i) ;
	c0 = fMaya_PAD_Col.at(i) ;

        for(int r=r0-1;r<=r0+1;r++){
	  for(int c=c0-1;c<=c0+1;c++){
	if(!(abs(r-r0)==1 && c==c0-1+(r0%2)*2) && r>0 && c>0 && r<33 && c<33){	
	    if(fMaya_PAD_map_idx.count(GetPadIdx(c,r))>0) {
//	    cout << c0 << " " << r0 << " -> " << c << " " << r << endl ;
	      if(fMaya_PAD_E_cal.at(fMaya_PAD_map_idx[GetPadIdx(c,r)])>0){
		fMaya_PAD_Neighbors.at(i)++;
	      }
	    }
	}
	  }
	}
      }
    }

    memset(fMaya_PAD_Row_Mult,0,32*sizeof(UShort_t)) ;
    memset(fMaya_PAD_Row_Sum_cal,0,32*sizeof(Float_t)) ;
    memset(fMaya_PAD_Row_Max_cal,0,32*sizeof(Float_t)) ;

    memset(fMaya_PAD_Col_Mult,0,32*sizeof(UShort_t)) ;
    memset(fMaya_PAD_Col_Sum_cal,0,32*sizeof(Float_t)) ;
    memset(fMaya_PAD_Col_Max_cal,0,32*sizeof(Float_t)) ;

    // clean accordingly
    for(unsigned int i=0;i<fMaya_PAD_E.size();i++){
      if(fMaya_PAD_Neighbors.at(i)<fMaya_PAD_Neighbors_Min) {
	fMaya_PAD_E_cal.at(i)=0.;
	fMaya_PAD_Mult_cal-- ;
      } else if (fMaya_PAD_E_cal.at(i)>0){
	fMaya_PAD_Row_Mult[ fMaya_PAD_Row.at(i)-1]++ ;
	fMaya_PAD_Row_Sum_cal[ fMaya_PAD_Row.at(i)-1] += fMaya_PAD_E_cal.at(i) ;
	if(fMaya_PAD_Row_Max_cal[fMaya_PAD_Row.at(i)-1] < fMaya_PAD_E_cal.at(i)) {
	  fMaya_PAD_Row_Max_cal[fMaya_PAD_Row.at(i)-1] = fMaya_PAD_E_cal.at(i) ;
	} ;
	fMaya_PAD_Col_Mult[ fMaya_PAD_Col.at(i)-1]++ ;
	fMaya_PAD_Col_Sum_cal[ fMaya_PAD_Col.at(i)-1] += fMaya_PAD_E_cal.at(i) ;
	if(fMaya_PAD_Col_Max_cal[fMaya_PAD_Col.at(i)-1] < fMaya_PAD_E_cal.at(i)) {
	  fMaya_PAD_Col_Max_cal[fMaya_PAD_Col.at(i)-1] = fMaya_PAD_E_cal.at(i) ;
	} ;
      }
    }

  }

 // if(!fMaya_QFIL_T.empty() && !fMaya_QFIL_T_calpar.empty() )
{

    fMaya_QFIL_T_cal.resize(fMaya_QFIL_T.size());

    for(unsigned int i=0;i<fMaya_QFIL_T.size();i++){
      
      tmpCal = 0 ; 
      
      tmpNbr = fMaya_QFIL_T_Nbr.at(i) - 1 ;
      if(fMaya_QFIL_T.at(i)>16380){ // timeout
	fMaya_QFIL_T_cal.at(i) = 0 ;
      } else {
	if(fMaya_QFIL_T_Nbr.at(i)<fMaya_QFIL_T_calpar.size()){
	  for(unsigned int j=0;j<fMaya_QFIL_T_calpar.at(tmpNbr).size();j++){
	    tmpCal += pow((double)fMaya_QFIL_T.at(i),(double)j)*fMaya_QFIL_T_calpar.at(tmpNbr).at(j) ;
	  }
	  fMaya_QFIL_T_cal.at(i) = tmpCal ;
	  //fprintf(fichier_test,"%f \n",fMaya_QFIL_T_cal.at(i));
	}
      } 

      
      //      std::cout << fMaya_QFIL_T.at(i) << " " << 
      //	fMaya_QFIL_T_cal.at(i) << std::endl ;
    }
    
    
  } 

/*else {
cout << "out Qfil " << endl ;
    fMaya_QFIL_T_cal.clear();

    //fclose(fichier_test);
    return false ;


  } */

//////////////////////////////

  

  //if(!fMaya_Si_E.empty()&& !fMaya_Si_E_calpar.empty() )
{
//std::cout << "Do the calibration" << std::endl;
    fMaya_Si_E_cal.resize(fMaya_Si_E.size());

    for(unsigned int i=0;i<fMaya_Si_E.size();i++){
      
      tmpCal = 0 ; 
      
      tmpNbr = fMaya_Si_E_Nbr.at(i) - 1 ;
      if(fMaya_Si_E.at(i)>16000){ // energy out
	fMaya_Si_E_cal.at(i) = 0 ;
      } else {
	if(fMaya_Si_E_Nbr.at(i)<fMaya_Si_E_calpar.size()){
	  for(unsigned int j=0;j<fMaya_Si_E_calpar.at(tmpNbr).size();j++){
	    tmpCal += pow((double)fMaya_Si_E.at(i),(double)j)*fMaya_Si_E_calpar.at(tmpNbr).at(j) ;
	  }
	  fMaya_Si_E_cal.at(i) = tmpCal ;
	}
      } 

      
          // std::cout << fMaya_Si_E.at(i) << " " << 
      	//fMaya_Si_E_cal.at(i) << std::endl ;
    }
    
    
  } 

/*else {
std::cout << "No calib  Si E Size:" << fMaya_Si_E.size() << "   Calib size " << fMaya_Si_E_calpar.size() << std::endl;
    fMaya_Si_E_cal.clear();
    return false ;
  } */




/////////////////////////////  
  return true ;
}







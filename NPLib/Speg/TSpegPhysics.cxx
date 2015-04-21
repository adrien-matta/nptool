/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre  contact address: lefebvrl@ipno.in2p3.fr     *
 *                                                                           *
 * Creation Date  : October 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the SPEG Spectrometer Physics                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TSpegPhysics.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"

//   STL
#include <iostream>
#include <fstream>
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TF1.h"
#include <vector>
#include <cmath>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <limits>
#include <TRandom1.h>
#include <TRandom2.h>
#include <TRandom3.h>

using namespace std;

//   tranform an integer to a string
string itoa(int value)
{
   char buffer [33];
   sprintf(buffer,"%d",value);
   return buffer;
}

ClassImp(TSpegPhysics)
///////////////////////////////////////////////////////////////////////////
TSpegPhysics::TSpegPhysics()
   {      
      EventDCData = new TSpegDCData ;
      EventCHIOData = new TSpegCHIOData ;
      EventPlasticData = new TSpegPlasticData ;
      EventPhysics = this ;
   }
   
///////////////////////////////////////////////////////////////////////////
TSpegPhysics::~TSpegPhysics()
   {}
   
///////////////////////////////////////////////////////////////////////////
void TSpegPhysics::Clear()
   {
     	Q_S11.clear() ;
 	Q_S12.clear() ;
      	Q_S21.clear() ;
      	Q_S22.clear() ;
	Charge.clear();
      	Strip_S11.clear() ;
      	Strip_S12.clear() ;
     	Strip_S21.clear() ;
      	Strip_S22.clear() ;
	Strip.clear();
      	M_over_Q.clear() ;
      	Z.clear();
      	X_FocalPlan.clear();
      	Y_FocalPlan.clear();
     	Theta_FocalPlan.clear();
      	Phi_FocalPlan.clear();
      	cor_xfoc_thetafoc.clear();
      	cor_xfoc_phifoc.clear();
      	cor_theta.clear();
      	cor_phi.clear();
      	correction_theta_with_Brho.clear();
      	correction_phi_with_Brho.clear();
      	cal_theta.clear();
      	cal_phi.clear();
      	calibration_theta_with_Brho.clear();
      	calibration_phi_with_Brho.clear();
	for(int l=0; l<4; l++)
	{
		Tab_xij[l]=0;
		Tab_yij[l]=0;
	}
   }
   
///////////////////////////////////////////////////////////////////////////
void TSpegPhysics::ReadConfiguration(string Path) 
   {

   ifstream ConfigFile              ;
   ConfigFile.open(Path.c_str())    ;
   string LineBuffer                ;
   string DataBuffer                ;  

//      bool check_nmesx = false          ;
      bool check_nomx  = false           ;
      bool check_zxx     = false          ;
      bool check_ievx     = false          ;
      bool check_dx     = false          ;
      bool check_x0     = false          ;
      bool check_etalx     = false          ;
      bool check_ievdx     = false          ;
      bool check_zx     = false          ;
      bool check_fx     = false          ;
      bool check_nax     = false          ;
      bool check_fax     = false          ;
      bool check_zax     = false          ;
      bool check_nbx     = false          ;
      bool check_fbx     = false          ;
      bool check_zbx     = false          ;
      bool check_ichix     = false          ;
      bool check_fchix     = false          ;
      bool check_nfx     = false          ;
      bool check_nfocx     = false          ;
      bool check_zfocx     = false          ;
      bool check_ffx     = false          ;
      bool check_zfx     = false          ;
      bool ReadingStatus1 = false ;

//      bool check_nmesy = false          ;
      bool check_zyy     = false          ;
      bool check_ievy     = false          ;
      bool check_dy     = false          ;
      bool check_y0     = false          ;
      bool check_etaly     = false          ;
      bool check_ievdy     = false          ;
      bool check_zy     = false          ;
      bool check_fy     = false          ;
      bool check_nay     = false          ;
      bool check_fay     = false          ;
      bool check_zay     = false          ;
      bool check_nby     = false          ;
      bool check_fby     = false          ;
      bool check_zby     = false          ;
      bool check_ichiy     = false          ;
      bool check_fchiy     = false          ;
      bool check_nfy     = false          ;
      bool check_nfocy     = false          ;
      bool check_zfocy     = false          ;
      bool check_ffy     = false          ;
      bool check_zfy     = false          ;
      bool ReadingStatus2 = false ;

      bool check_alpha0 = false          ;
      bool check_alpha1 = false          ;
      bool check_alpha2 = false          ;
      bool check_alpha3 = false          ;
      bool check_tema = false          ;
      bool check_temb = false          ;
      bool check_tcora = false          ;
      bool check_tcorb = false          ;
      bool check_cmsurqa = false          ;
      bool check_cmsurqb = false          ;
      bool check_za = false          ;
      bool check_zb = false          ;
      bool check_degree_of_calibration_angle_with_Brho = false ;
      bool check_degree_of_correction_angle_with_Brho = false ;
      bool ReadingStatus3 = false ;

    while (!ConfigFile.eof()) 
       {
         
         getline(ConfigFile, LineBuffer);

         //   If line is a Start Up SPEG bloc, Reading toggle to true      
         if (LineBuffer.compare(0, 5, "dotrx") == 0) 
            {
               cout << "///" << endl ;
               cout << "dotrx found : " << endl ;        
               ReadingStatus1 = true ;
            }
            
         //   Else don't toggle to Reading Block Status
         else {ReadingStatus1 = false ;}
         
         //   Reading Block
         while(ReadingStatus1)
            {
      	       // Pickup Next Word 
               ConfigFile >> DataBuffer ;

               //   Comment Line 
               if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

                  //   Finding another dotrx (safety), toggle out
               else if (DataBuffer.compare(0, 5, "dotrx") == 0) {
                  cout << "WARNING: Another dotrx is find before standard sequence of Token, Error may occured in SPEG definition" << endl ;
                  ReadingStatus1 = false ;
               }               
               else if (DataBuffer=="nmesx=") {
//		  check_nmesx = true          ;
                  ConfigFile >> DataBuffer ;
		  nmesx=atof(DataBuffer.c_str());
                  cout << "nmesx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="nomx=") {
       check_nomx  = true           ;
                  ConfigFile >> DataBuffer ;
                  cout << "nomx:  " << DataBuffer <<  endl;
               }
               else if (DataBuffer=="zx=") {
       check_zxx     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_zx.push_back(atof(DataBuffer.c_str()));
                  cout << "zx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="iev=") {
       check_ievx     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_nievcx.push_back(atof(DataBuffer.c_str()));
                  cout << "nievcx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="dx=") {
       check_dx     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_sigx.push_back(atof(DataBuffer.c_str()));
                  cout << "sigx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="x0=") {
       check_x0     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_x0f.push_back(atof(DataBuffer.c_str()));
                  cout << "x0f:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="etalx=") {
       check_etalx     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_etalx.push_back(atof(DataBuffer.c_str()));
                  cout << "etalx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="ievd=") {
       check_ievdx     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_iedevx.push_back(atof(DataBuffer.c_str()));
                  cout << "iedevx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="z=") {
       check_zx     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_zdevx.push_back(atof(DataBuffer.c_str()));
                  cout << "zdevx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="f=") {
       check_fx     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_fdevx.push_back(atof(DataBuffer.c_str()));
                  cout << "fdevx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="nax=") {
       check_nax     = true          ;
                  ConfigFile >> DataBuffer ;
		  nax=atof(DataBuffer.c_str());
                  cout << "nax:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="fax=") {
       check_fax     = true          ;
                  ConfigFile >> DataBuffer ;
		  fax=atof(DataBuffer.c_str());
                  cout << "fax:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="zax=") {
       check_zax     = true          ;
                  ConfigFile >> DataBuffer ;
		  zax=atof(DataBuffer.c_str());
                  cout << "zax:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="nbx=") {
       check_nbx     = true          ;
                  ConfigFile >> DataBuffer ;
		  nbx=atof(DataBuffer.c_str());
                  cout << "nbx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="fbx=") {
       check_fbx     = true          ;
                  ConfigFile >> DataBuffer ;
		  fbx=atof(DataBuffer.c_str());
                  cout << "fbx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="zbx=") {
       check_zbx     = true          ;
                  ConfigFile >> DataBuffer ;
		  zbx=atof(DataBuffer.c_str());
                  cout << "zbx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="ichix=") {
       check_ichix     = true          ;
                  ConfigFile >> DataBuffer ;
		  ichix=atof(DataBuffer.c_str());
                  cout << "ichix:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="fchix=") {
       check_fchix     = true          ;
                  ConfigFile >> DataBuffer ;
		  fchix=atof(DataBuffer.c_str());
                  cout << "fchix:  " << atof(DataBuffer.c_str()) <<  endl;
               }               
		else if (DataBuffer=="nfx=") {
       check_nfx     = true          ;
                  ConfigFile >> DataBuffer ;
		  nfx=atof(DataBuffer.c_str());
                  cout << "nfx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
		else if (DataBuffer=="nfoc=") {
       check_nfocx     = true          ;
                  ConfigFile >> DataBuffer ;
		  anfocx=atof(DataBuffer.c_str());
                  cout << "anfocx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
		else if (DataBuffer=="zfoc=") {
       check_zfocx     = true          ;
                  ConfigFile >> DataBuffer ;
		  zfocx=atof(DataBuffer.c_str());
                  cout << "zfocx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
		else if (DataBuffer=="ffx=") {
       check_ffx     = true          ;
                  ConfigFile >> DataBuffer ;
		  ffx=atof(DataBuffer.c_str());
                  cout << "ffx:  " << atof(DataBuffer.c_str()) <<  endl;
               }
		else if (DataBuffer=="zfx=") {
       check_zfx     = true          ;
                  ConfigFile >> DataBuffer ;
		  zfx=atof(DataBuffer.c_str());
                  cout << "zfx:  " << atof(DataBuffer.c_str()) <<  endl;
               }

///////////////////////////////////////////////////
               //   If no Detector Token and no comment, toggle out
               else 
                  {ReadingStatus1 = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
               
                  /////////////////////////////////////////////////
                  //   If All necessary information there, toggle out
               
               if (check_nomx && check_zxx && check_ievx && check_dx && check_x0 && check_etalx && check_ievdx && check_zx && check_fx && check_nax && check_fax && check_zax && check_nbx && check_fbx &&check_zbx &&check_ichix && check_fchix && check_nfx && check_nfocx && check_zfocx && check_ffx && check_zfx) 
                  { 

                     //   Reinitialisation of Check Boolean  
       check_nomx  =false            ;
       check_zxx     =false           ;
       check_ievx     =false           ;
       check_dx     =false           ;
       check_x0     =false           ;
       check_etalx     =false           ;
       check_ievdx     =false           ;
       check_zx    =false           ;
       check_fx     =false           ;
       check_nax     =false           ;
       check_fax     =false           ;
       check_zax     =false           ;
       check_nbx     =false           ;
       check_fbx     =false           ;
       check_zbx     =false           ;
       check_ichix     =false           ;
       check_fchix     =false           ;
       check_nfx     =false           ;
       check_nfocx     =false           ;
       check_zfocx     =false           ;
       check_ffx     =false           ;
       check_zfx     =false           ;
       ReadingStatus1 =false  ;   
                     cout << "///"<< endl         ;                
                  }
 	}

//   If line is a Start Up SPEG bloc, Reading toggle to true      
         if (LineBuffer.compare(0, 5, "dotry") == 0) 
            {
               cout << "///" << endl ;
               cout << "dotry found : " << endl ;        
               ReadingStatus2 = true ;
            }
            
         //   Else don't toggle to Reading Block Status
         else ReadingStatus2 = false ;
         
         //   Reading Block
         while(ReadingStatus2)
            {
      	       // Pickup Next Word 
               ConfigFile >> DataBuffer ;

               //   Comment Line 
               if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

                  //   Finding another dotry (safety), toggle out
               else if (DataBuffer.compare(0, 5, "dotry") == 0) {
                  cout << "WARNING: Another dotry is find before standard sequence of Token, Error may occured in SPEG definition" << endl ;
                  ReadingStatus2 = false ;
               }
                              
               else if (DataBuffer=="nmesy=") {
//		  check_nmesy = true          ;
                  ConfigFile >> DataBuffer ;
		  nmesy=atof(DataBuffer.c_str());
                  cout << "nmesy:  " << atof(DataBuffer.c_str()) <<  endl;
               }

               else if (DataBuffer=="zy=") {
       check_zyy     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_zy.push_back(atof(DataBuffer.c_str()));
                  cout << "zy:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="iev=") {
       check_ievy     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_nievcy.push_back(atof(DataBuffer.c_str()));
                  cout << "nievcy:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="dy=") {
       check_dy     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_sigy.push_back(atof(DataBuffer.c_str()));
                  cout << "sigy:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="y0=") {
       check_y0     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_y0f.push_back(atof(DataBuffer.c_str()));
                  cout << "y0f:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="etaly=") {
       check_etaly     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_etaly.push_back(atof(DataBuffer.c_str()));
                  cout << "etaly:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="ievd=") {
       check_ievdy     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_iedevy.push_back(atof(DataBuffer.c_str()));
                  cout << "iedevy:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="z=") {
       check_zy     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_zdevy.push_back(atof(DataBuffer.c_str()));
                  cout << "zdevy:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="f=") {
       check_fy     = true          ;
                  ConfigFile >> DataBuffer ;
		  Vector_fdevy.push_back(atof(DataBuffer.c_str()));
                  cout << "fdevy:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="nay=") {
       check_nay     = true          ;
                  ConfigFile >> DataBuffer ;
		  nay=atof(DataBuffer.c_str());
                  cout << "nay:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="fay=") {
       check_fay     = true          ;
                  ConfigFile >> DataBuffer ;
		  fay=atof(DataBuffer.c_str());
                  cout << "fay:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="zay=") {
       check_zay     = true          ;
                  ConfigFile >> DataBuffer ;
		  zay=atof(DataBuffer.c_str());
                  cout << "zay:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="nby=") {
       check_nby     = true          ;
                  ConfigFile >> DataBuffer ;
		  nby=atof(DataBuffer.c_str());
                  cout << "nby:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="fby=") {
       check_fby     = true          ;
                  ConfigFile >> DataBuffer ;
		  fby=atof(DataBuffer.c_str());
                  cout << "fby:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="zby=") {
       check_zby     = true          ;
                  ConfigFile >> DataBuffer ;
		  zby=atof(DataBuffer.c_str());
                  cout << "zby:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="ichiy=") {
       check_ichiy     = true          ;
                  ConfigFile >> DataBuffer ;
		  ichiy=atof(DataBuffer.c_str());
                  cout << "ichiy:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="fchiy=") {
       check_fchiy     = true          ;
                  ConfigFile >> DataBuffer ;
		  fchiy=atof(DataBuffer.c_str());
                  cout << "fchiy:  " << atof(DataBuffer.c_str()) <<  endl;
               }               
		else if (DataBuffer=="nfy=") {
       check_nfy     = true          ;
                  ConfigFile >> DataBuffer ;
		  nfy=atof(DataBuffer.c_str());
                  cout << "nfy:  " << atof(DataBuffer.c_str()) <<  endl;
               }
		else if (DataBuffer=="nfoc=") {
       check_nfocy     = true          ;
                  ConfigFile >> DataBuffer ;
		  anfocy=atof(DataBuffer.c_str());
                  cout << "nfoc:  " << atof(DataBuffer.c_str()) <<  endl;
               }
		else if (DataBuffer=="zfoc=") {
       check_zfocy     = true          ;
                  ConfigFile >> DataBuffer ;
		  zfocy=atof(DataBuffer.c_str());
                  cout << "zfoc:  " << atof(DataBuffer.c_str()) <<  endl;
               }
		else if (DataBuffer=="ffy=") {
       check_ffy     = true          ;
                  ConfigFile >> DataBuffer ;
		  ffy=atof(DataBuffer.c_str());
                  cout << "ffy:  " << atof(DataBuffer.c_str()) <<  endl;
               }
		else if (DataBuffer=="zfy=") {
       check_zfy     = true          ;
                  ConfigFile >> DataBuffer ;
		  zfy=atof(DataBuffer.c_str());
                  cout << "zfy:  " << atof(DataBuffer.c_str()) <<  endl;
               }

///////////////////////////////////////////////////
               //   If no Detector Token and no comment, toggle out
               else 
                  {ReadingStatus2 = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
               
                  /////////////////////////////////////////////////
                  //   If All necessary information there, toggle out
               
               if (check_zyy && check_ievy && check_dy && check_y0 && check_etaly && check_ievdy && check_zy && check_fy && check_nay && check_fay && check_zay && check_nby && check_fby &&check_zby &&check_ichiy && check_fchiy && check_nfy && check_nfocy && check_zfocy && check_ffy && check_zfy) 
                  { 

                     //   Reinitialisation of Check Boolean  
       check_zyy     =false           ;
       check_ievy     =false           ;
       check_dy     =false           ;
       check_y0     =false           ;
       check_etaly     =false           ;
       check_ievdy     =false           ;
       check_zy     =false           ;
       check_fy     =false           ;
       check_nay     =false           ;
       check_fay     =false           ;
       check_zay     =false           ;
       check_nby     =false           ;
       check_fby     =false           ;
       check_zby     =false           ;
       check_ichiy     =false           ;
       check_fchiy     =false           ;
       check_nfy     =false           ;
       check_nfocy     =false           ;
       check_zfocy    =false           ;
       check_ffy     =false           ;
       check_zfy     =false           ;
       ReadingStatus2 =false  ;   
                     cout << "///"<< endl         ;                
                  }

		}
 //   If line is a Start Up SPEG bloc, Reading toggle to true      
         if (LineBuffer.compare(0, 11, "Coefficient") == 0) 
            {
               cout << "///" << endl ;
               cout << "Coefficient found : " << endl ;        
               ReadingStatus3 = true ;
            }
            
         //   Else don't toggle to Reading Block Status
         else ReadingStatus3 = false ;
         
         //   Reading Block
         while(ReadingStatus3)
            {
      	       // Pickup Next Word 
               ConfigFile >> DataBuffer ;

               //   Comment Line 
               if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

                  //   Finding another Coefficient (safety), toggle out
               else if (DataBuffer.compare(0, 11, "Coefficient") == 0) {
                  cout << "WARNING: Another Coefficient is find before standard sequence of Token, Error may occured in SPEG definition" << endl ;
                  ReadingStatus3 = false ;
               }
                              
               else if (DataBuffer=="alpha0=") {
		  check_alpha0 = true          ;
                  ConfigFile >> DataBuffer ;
		  alpha[0]=atof(DataBuffer.c_str());
                  cout << "alpha0:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="alpha1=") {
		  check_alpha1 = true          ;
                  ConfigFile >> DataBuffer ;
		  alpha[1]=atof(DataBuffer.c_str());
                  cout << "alpha1:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="alpha2=") {
		  check_alpha2 = true          ;
                  ConfigFile >> DataBuffer ;
		  alpha[2]=atof(DataBuffer.c_str());
                  cout << "alpha2:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="alpha3=") {
		  check_alpha3 = true          ;
                  ConfigFile >> DataBuffer ;
		  alpha[3]=atof(DataBuffer.c_str());
                  cout << "alpha3:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="tema=") {
		  check_tema = true          ;
                  ConfigFile >> DataBuffer ;
		  tema=atof(DataBuffer.c_str());
                  cout << "tema:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="temb=") {
		  check_temb = true          ;
                  ConfigFile >> DataBuffer ;
		  temb=atof(DataBuffer.c_str());
                  cout << "temb:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="tcora=") {
		  check_tcora = true          ;
                  ConfigFile >> DataBuffer ;
		  tcora=atof(DataBuffer.c_str());
                  cout << "tcora:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="tcorb=") {
		  check_tcorb = true          ;
                  ConfigFile >> DataBuffer ;
		  tcorb=atof(DataBuffer.c_str());
                  cout << "tcorb:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="cmsurqa=") {
		  check_cmsurqa = true          ;
                  ConfigFile >> DataBuffer ;
		  cmsurqa=atof(DataBuffer.c_str());
                  cout << "cmsurqa:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="cmsurqb=") {
		  check_cmsurqb = true          ;
                  ConfigFile >> DataBuffer ;
		  cmsurqb=atof(DataBuffer.c_str());
                  cout << "cmsurqb:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="za=") {
		  check_za = true          ;
                  ConfigFile >> DataBuffer ;
		  za=atof(DataBuffer.c_str());
                  cout << "za:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="zb=") {
		  check_zb = true          ;
                  ConfigFile >> DataBuffer ;
		  zb=atof(DataBuffer.c_str());
                  cout << "zb:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="degree_of_calibration_angle_with_Brho=") {
		  check_degree_of_calibration_angle_with_Brho = true          ;
                  ConfigFile >> DataBuffer ;
		  degree_of_calibration_angle_with_Brho=atof(DataBuffer.c_str());
                  cout << "Degree of calibration angle with Brho:  " << atof(DataBuffer.c_str()) <<  endl;
               }
               else if (DataBuffer=="degree_of_correction_angle_with_Brho=") {
		  check_degree_of_correction_angle_with_Brho = true          ;
                  ConfigFile >> DataBuffer ;
		  degree_of_correction_angle_with_Brho=atof(DataBuffer.c_str());
                  cout << "Degree of correction angle with Brho:  " << atof(DataBuffer.c_str()) <<  endl;
               }

///////////////////////////////////////////////////
               //   If no Detector Token and no comment, toggle out
               else 
                  {ReadingStatus3 = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
               
                  /////////////////////////////////////////////////
                  //   If All necessary information there, toggle out
               
               if (check_alpha0 && check_alpha1 && check_alpha2 && check_alpha3 && check_tema && check_temb && check_tcora && check_tcorb && check_cmsurqa && check_cmsurqb && check_za && check_zb && check_degree_of_calibration_angle_with_Brho && check_degree_of_correction_angle_with_Brho) 
                  { 

                     //   Reinitialisation of Check Boolean  
      check_alpha0 = false          ;
      check_alpha1 = false          ;
      check_alpha2 = false          ;
      check_alpha3 = false          ;
      check_tema = false          ;
      check_temb = false          ;
      check_tcora = false          ;
      check_tcorb = false          ;
      check_cmsurqa = false          ;
      check_cmsurqb = false          ;
      check_za = false          ;
      check_zb = false  ;
      check_degree_of_calibration_angle_with_Brho = false;
      check_degree_of_correction_angle_with_Brho= false;
      ReadingStatus3 =false  ;        
                     cout << "///"<< endl         ;                
                  }

		}
	}
   	ReadAnalysisConfig();
}

///////////////////////////////////////////////
void TSpegPhysics::ReadAnalysisConfig()
{
   bool ReadingStatus = false;
   bool_CORRECTION_COEF_DC_11 = false;
   bool_CORRECTION_COEF_DC_12 = false;
   bool_CORRECTION_COEF_DC_21 = false;
   bool_CORRECTION_COEF_DC_22 = false;
   for(int l=0; l<128; l++)
   {
	m_BadStrip_SPEG_DC11[l] = 0;
	m_BadStrip_SPEG_DC12[l] = 0;
	m_BadStrip_SPEG_DC21[l] = 0;
	m_BadStrip_SPEG_DC22[l] = 0;
	m_CorrectionCoef_SPEG_DC11[l]=0;
	m_CorrectionCoef_SPEG_DC12[l]=0;
	m_CorrectionCoef_SPEG_DC21[l]=0;
	m_CorrectionCoef_SPEG_DC22[l]=0;
   }

   // path to file
   string FileName = "./configs/ConfigSPEG.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << " No ConfigSPEG.dat found: Default parameter loaded for Analayis " << FileName << endl;
      return;
   }
   cout << " Loading user parameter for Analysis from ConfigSPEG.dat " << endl;
   
   // Save it in a TAsciiFile
   TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
   asciiConfig->AppendLine("%%% ConfigSPEG.dat %%%");
   asciiConfig->Append(FileName.c_str());
   asciiConfig->AppendLine("");
   // read analysis config file
   string LineBuffer,DataBuffer,whatToDo;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 10, "ConfigSPEG") == 0) ReadingStatus = true;

      // loop on tokens and data
      while (ReadingStatus ) {

          whatToDo="";
          AnalysisConfigFile >> whatToDo;
         
         // Search for comment symbol (%)
         if (whatToDo.compare(0, 1, "%") == 0) {
            AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
         }  
		else if (whatToDo == "CORRECTION_COEF_DC_11") {
			AnalysisConfigFile >> DataBuffer;
			cout << whatToDo << "  " << DataBuffer;
 			int channel = -1;
			bool_CORRECTION_COEF_DC_11 = true;
			if (DataBuffer.compare(4,6,"STRIP_") == 0) {
 				channel = atoi(DataBuffer.substr(10).c_str());
				m_BadStrip_SPEG_DC11[channel-1] = channel;
				cout << endl;
				cout << "Bad strip : " << channel; 
				AnalysisConfigFile >> DataBuffer;
				m_CorrectionCoef_SPEG_DC11[channel-1] = atof(DataBuffer.c_str());
				cout << " | Correction coefficient : " << m_CorrectionCoef_SPEG_DC11[channel-1] << endl;
			}
		}

		else if (whatToDo == "CORRECTION_COEF_DC_12") {
			AnalysisConfigFile >> DataBuffer;
			cout << whatToDo << "  " << DataBuffer;
 			int channel = -1;
			bool_CORRECTION_COEF_DC_12 = true;
			if (DataBuffer.compare(4,6,"STRIP_") == 0) {
 				channel = atoi(DataBuffer.substr(10).c_str());
				m_BadStrip_SPEG_DC12[channel-1] = channel;
				cout << endl;
				cout << "Bad strip : " << channel; 
				AnalysisConfigFile >> DataBuffer;
				m_CorrectionCoef_SPEG_DC12[channel-1] = atof(DataBuffer.c_str());
				cout << " | Correction coefficient : " << m_CorrectionCoef_SPEG_DC12[channel-1] << endl;
			}
		}

		else if (whatToDo == "CORRECTION_COEF_DC_21") {
			AnalysisConfigFile >> DataBuffer;
			cout << whatToDo << "  " << DataBuffer;
 			int channel = -1;
			bool_CORRECTION_COEF_DC_21 = true;
			if (DataBuffer.compare(4,6,"STRIP_") == 0) {
 				channel = atoi(DataBuffer.substr(10).c_str());
				m_BadStrip_SPEG_DC21[channel-1] = channel;
				cout << endl;
				cout << "Bad strip : " << channel; 
				AnalysisConfigFile >> DataBuffer;
				m_CorrectionCoef_SPEG_DC21[channel-1] = atof(DataBuffer.c_str());
				cout << " | Correction coefficient : " << m_CorrectionCoef_SPEG_DC21[channel-1] << endl;
			}
		}

		else if (whatToDo == "CORRECTION_COEF_DC_22") {
			AnalysisConfigFile >> DataBuffer;
			cout << whatToDo << "  " << DataBuffer;
 			int channel = -1;
			bool_CORRECTION_COEF_DC_22 = true;
			if (DataBuffer.compare(4,6,"STRIP_") == 0) {
 				channel = atoi(DataBuffer.substr(10).c_str());
				m_BadStrip_SPEG_DC22[channel-1] = channel;
				cout << endl;
				cout << "Bad strip : " << channel; 
				AnalysisConfigFile >> DataBuffer;
				m_CorrectionCoef_SPEG_DC22[channel-1] = atof(DataBuffer.c_str());
				cout << " | Correction coefficient : " << m_CorrectionCoef_SPEG_DC22[channel-1] << endl;
			}
		}
		       
       else {ReadingStatus = false;}
       
      }
   }
} 

///////////////////////////////////////////////////////////////////////////
void TSpegPhysics::AddParameterToCalibrationManager()
   {
      CalibrationManager* Cal = CalibrationManager::getInstance();
      Cal->AddParameter("SPEG", "_PLASTIC_RIGHT","SPEG_PLASTIC_RIGHT")   ;
      CalibrationManager* Cal1 = CalibrationManager::getInstance();
      Cal1->AddParameter("SPEG", "_PLASTIC_LEFT","SPEG_PLASTIC_LEFT")   ;
      CalibrationManager* Cal2 = CalibrationManager::getInstance();
      Cal2->AddParameter("SPEG", "_xfoc","SPEG_xfoc")   ;
      CalibrationManager* Cal3 = CalibrationManager::getInstance();
      Cal3->AddParameter("SPEG", "_xfoc_cor_theta","SPEG_xfoc_cor_theta")   ;
      CalibrationManager* Cal4 = CalibrationManager::getInstance();
      Cal4->AddParameter("SPEG", "_xfoc_cor_phi","SPEG_xfoc_cor_phi")   ;
      CalibrationManager* Cal5 = CalibrationManager::getInstance();
      for(int i = 0 ; i < degree_of_calibration_angle_with_Brho+1 ; i++)
      {
      	Cal5->AddParameter("SPEG", "_tfoc_calibration_with_Brho_"+ NPL::itoa(i),"SPEG_tfoc_calibration_with_Brho_"+ NPL::itoa(i))   ;
      	Cal5->AddParameter("SPEG", "_phifoc_calibration_with_Brho_"+ NPL::itoa(i),"SPEG_phifoc_calibration_with_Brho_"+ NPL::itoa(i))   ;
      }
      CalibrationManager* Cal6 = CalibrationManager::getInstance();
      for(int i = 0 ; i < degree_of_correction_angle_with_Brho+1 ; i++)
      {
      	Cal6->AddParameter("SPEG", "_tfoc_correction_with_Brho_"+ NPL::itoa(i),"SPEG_tfoc_correction_with_Brho_"+ NPL::itoa(i))   ;
      	Cal6->AddParameter("SPEG", "_phifoc_correction_with_Brho_"+ NPL::itoa(i),"SPEG_phifoc_correction_with_Brho_"+ NPL::itoa(i))   ;
      }
      CalibrationManager* Cal7 = CalibrationManager::getInstance();
      for(int i = 0 ; i < 128 ; i++)
      {
      	Cal7->AddParameter("SPEG", "_DC_S11_decal_"+ NPL::itoa(i+1),"SPEG_DC_S11_decal_"+ NPL::itoa(i+1))   ;
      	Cal7->AddParameter("SPEG", "_DC_S12_decal_"+ NPL::itoa(i+1),"SPEG_DC_S12_decal_"+ NPL::itoa(i+1))   ;
      	Cal7->AddParameter("SPEG", "_DC_S21_decal_"+ NPL::itoa(i+1),"SPEG_DC_S21_decal_"+ NPL::itoa(i+1))   ;
      	Cal7->AddParameter("SPEG", "_DC_S22_decal_"+ NPL::itoa(i+1),"SPEG_DC_S22_decal_"+ NPL::itoa(i+1))   ;
      }
      CalibrationManager* Cal8 = CalibrationManager::getInstance();
      for(int i = 0 ; i < 128 ; i++)
      {
      	Cal8->AddParameter("SPEG", "_DC_S11_"+ NPL::itoa(i+1),"SPEG_DC_S11_"+ NPL::itoa(i+1))   ;
      	Cal8->AddParameter("SPEG", "_DC_S12_"+ NPL::itoa(i+1),"SPEG_DC_S12_"+ NPL::itoa(i+1))   ;
      	Cal8->AddParameter("SPEG", "_DC_S21_"+ NPL::itoa(i+1),"SPEG_DC_S21_"+ NPL::itoa(i+1))   ;
      	Cal8->AddParameter("SPEG", "_DC_S22_"+ NPL::itoa(i+1),"SPEG_DC_S22_"+ NPL::itoa(i+1))   ;
      }
   }

///////////////////////////////////////////////////////////////////////////
void TSpegPhysics::InitializeRootInputRaw() 
   {
      TChain* inputChain = RootInput::getInstance()->GetChain()     ;
      inputChain->SetBranchStatus ( "SPEG_DC"       , true )        ;
      inputChain->SetBranchStatus ( "fSpeg_*"    , true )        ;
      inputChain->SetBranchAddress( "SPEG_DC"       , &EventDCData )  ;
      inputChain->SetBranchStatus ( "Speg_CHIO"       , true )        ;
      inputChain->SetBranchStatus ( "fSpeg_*"    , true )        ;
      inputChain->SetBranchAddress( "Speg_CHIO"       , &EventCHIOData )  ;
      inputChain->SetBranchStatus ( "Speg_PLASTIC"       , true )        ;
      inputChain->SetBranchStatus ( "fSpeg_*"    , true )        ;
      inputChain->SetBranchAddress( "Speg_PLASTIC"       , &EventPlasticData )  ;
   }
///////////////////////////////////////////////////////////////////////////
void TSpegPhysics::InitializeRootInputPhysics()
   {
      TChain* inputChain = RootInput::getInstance()->GetChain();
      inputChain->SetBranchStatus ( "Speg", true );
      inputChain->SetBranchStatus ( "Q_S11", true );
      inputChain->SetBranchStatus ( "Q_S12", true );
      inputChain->SetBranchStatus ( "Q_S21", true );
      inputChain->SetBranchStatus ( "Q_S22", true );
      inputChain->SetBranchStatus ( "Strip_S11", true );
      inputChain->SetBranchStatus ( "Strip_S12", true );
      inputChain->SetBranchStatus ( "Strip_S21", true );
      inputChain->SetBranchStatus ( "Strip_S22", true );
      inputChain->SetBranchStatus ( "M_over_Q", true );
      inputChain->SetBranchStatus ( "Z", true );
      inputChain->SetBranchStatus ( "X_FocalPlan", true );
      inputChain->SetBranchStatus ( "Y_FocalPlan", true );
      inputChain->SetBranchStatus ( "Theta_FocalPlan", true );
      inputChain->SetBranchStatus ( "Phi_FocalPlan", true );
      inputChain->SetBranchStatus ( "Time_Plastic_Right", true );
      inputChain->SetBranchStatus ( "Time_Plastic_Left", true );
      inputChain->SetBranchStatus ( "SPEG_CHIO_anode", true );
      inputChain->SetBranchStatus ( "Khi2", true );
      inputChain->SetBranchAddress( "Speg", &EventPhysics );
   }
///////////////////////////////////////////////////////////////////////////
void TSpegPhysics::InitializeRootOutput()
   {
      TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
      outputTree->Branch( "Speg" , "TSpegPhysics" , &EventPhysics ) ;
   }

///////////////////////////////////////////////////////////////////////////
void TSpegPhysics::BuildPhysicalEvent()
   {
      BuildSimplePhysicalEvent()   ;
   }

///////////////////////////////////////////////////////////////////////////
void TSpegPhysics::BuildSimplePhysicalEvent()
{

int number_strip = 128;//Number of DC strip 
int number_strips_plan = 4;//Number of strips plan
double pi = 3.14159265358979312;//Pi

/*-------------Read (lectrx) parameters for trajectory calculation for Drift Chamber of SPEG-------*/

for(int l=0;l<number_strips_plan;l++)
{
	Vector_sigx2.push_back((Vector_sigx[l]*Vector_etalx[l]/2.35)*(Vector_sigx[l]*Vector_etalx[l]/2.35));
}
if(anfocx<0.000001)
{
	tanfox=1e8;
}
else
{
	tanfox=1./tan(0.01745*anfocx);
}

for(int l=0;l<number_strips_plan;l++)
{
	Vector_sigy2.push_back((Vector_sigy[l]*Vector_etaly[l]/2.35)*(Vector_sigy[l]*Vector_etaly[l]/2.35));
}

if(anfocy<0.000001)
{
	tanfoy=1e8;
}
else
{
	tanfoy=1./tan(0.01745*anfocy);
}

//b matrix calculation for x
bx11=0;
bx12=0;
bx21=0;
bx22=0;
for(int i=0; i<nmesx; i++)
{
	bx11=bx11+Vector_zx[i]*Vector_zx[i]/Vector_sigx2[i];
	bx12=bx12+Vector_zx[i]/Vector_sigx2[i];
	bx22=bx22+1./Vector_sigx2[i];
}
bx21=bx12;

//Inverse b matrix calculation
det=bx11*bx22-bx12*bx21;
if(det==0)
{
	ax11=0;
	ax12=0;
	ax21=0;
	ax22=0;
}
else
{
	ax11=bx22/det;
	ax22=bx11/det;
	ax21=-bx12/det;
	ax12=-bx21/det;
}
//b matrix calculation for y
by11=0;
by12=0;
by21=0;
by22=0;
for(int i=0; i<nmesy; i++)
{
	by11=by11+Vector_zy[i]*Vector_zy[i]/Vector_sigy2[i];
	by12=by12+Vector_zy[i]/Vector_sigy2[i];
	by22=by22+1./Vector_sigy2[i];
}
by21=by12;

//Inverse b matrix calculation
det=by11*by22-by12*by21;
if(det==0)
{
	ay11=0;
	ay12=0;
	ay21=0;
	ay22=0;
}
else
{
	ay11=by22/det;
	ay22=by11/det;
	ay21=-by12/det;
	ay12=-by21/det;
}

/*------------------------Subroutine Multi (multiplicity of the DC)--------------------------*/

	//Value of the qmax overflow
	seuil_haut = 3500;
	//Strips steps = 6mm (inter strip = 0.54)
	pas = 6;
	//Strips size (true size = 5.46)
	a1_1 = 6; 
	//Gravity center calculation for SPEG calibration
	x11 = 0; x12 = 0; x21 = 0; x22 = 0;
	//Y in the Drift chamber
	y11 = 0; y12 = 0; y21 = 0; y22 = 0;
	//Z (charge)
	z = 0;
	//variables SECHS methode
	x_c = 0;
	for(int k=0;k<number_strips_plan;k++)
	{
		//Centroides
		x3cg[k] = 0;
		xg[k] = 0;
        	xsh[k] = 0;

		//Charges and three strips max touched number
            	qmax[k] = 0;//Three strips the most touched for each plan (=4)
            	imax[k] = 0;//Number of the most touched three strips for each plan (=4)
	}

	//Time DC
	if(EventDCData->GetSpegDCTfilMult11()>0)
	{
		for(int j=0;j<EventDCData->GetSpegDCTplfilMult11();j++)
		{
			y11 = EventDCData->GetSpegDCTfilTime11(j)+gRandom->Uniform(0,1)-0.5;
		}
	}
	if(EventDCData->GetSpegDCTfilMult12()>0)
	{
		for(int j=0;j<EventDCData->GetSpegDCTplfilMult12();j++)
		{
			y12 = EventDCData->GetSpegDCTfilTime12(j)+gRandom->Uniform(0,1)-0.5;
		}
	}
	if(EventDCData->GetSpegDCTfilMult21()>0)
	{
		for(int j=0;j<EventDCData->GetSpegDCTplfilMult21();j++)
		{
			y21 = EventDCData->GetSpegDCTfilTime21(j)+gRandom->Uniform(0,1)-0.5;
		}
	}
	if(EventDCData->GetSpegDCTfilMult22()>0)
	{	
		for(int j=0;j<EventDCData->GetSpegDCTplfilMult22();j++)
		{
			y22 = EventDCData->GetSpegDCTfilTime22(j)+gRandom->Uniform(0,1)-0.5;
		}
	}

	//Anode CHIO
	if(EventCHIOData->GetMultSpegCHIOEnergy()>0)
	{
		for(int j=0;j<EventCHIOData->GetMultSpegCHIOEnergy();j++)
		{
			SPEG_CHIO_anode = EventCHIOData->GetSpegCHIOEnergy(j);
		}
	}

	//Time PLASTIC values
	SPEG_PLASTIC_tplg = EventPlasticData->GetTimeLeft();
	SPEG_PLASTIC_tpld = EventPlasticData->GetTimeRight();
	//Calibration TPLD and TPLG
        Time_Plastic_Right = (CalibrationManager::getInstance()->ApplyCalibration("SPEG/_PLASTIC_RIGHT", EventPlasticData->GetTimeRight()));
        Time_Plastic_Left = (CalibrationManager::getInstance()->ApplyCalibration("SPEG/_PLASTIC_LEFT", EventPlasticData->GetTimeLeft()));
	
	//First plan touched
	if(EventDCData->GetSpegDCMultStrip11()>0)
	{
		for(int j=0;j<EventDCData->GetSpegDCMultStrip11();j++)
		{
			if((EventDCData->GetSpegDCStripNbr11(j))>0 && (EventDCData->GetSpegDCStripNbr11(j))<=number_strip && (EventDCData->GetSpegDCEnergy11(j))<seuil_haut)
			{
				Strip_S11.push_back(EventDCData->GetSpegDCStripNbr11(j));
				if(bool_CORRECTION_COEF_DC_11 && EventDCData->GetSpegDCStripNbr11(j)==m_BadStrip_SPEG_DC11[EventDCData->GetSpegDCStripNbr11(j)-1])
				{
					Q_S11.push_back((EventDCData->GetSpegDCEnergy11(j)+gRandom->Uniform(0,1)-0.5)*m_CorrectionCoef_SPEG_DC11[EventDCData->GetSpegDCStripNbr11(j)-1]);
				}
				else
				{
					Q_S11.push_back((CalibrationManager::getInstance()->ApplyCalibration("SPEG/_DC_S11_"+ NPL::itoa(EventDCData->GetSpegDCStripNbr11(j)),(CalibrationManager::getInstance()->ApplyCalibration("SPEG/_DC_S11_decal_"+ NPL::itoa(EventDCData->GetSpegDCStripNbr11(j)), EventDCData->GetSpegDCEnergy11(j)))))+gRandom->Uniform(0,1)-0.5);
				}       		
			}
		}
	}
	Strip.push_back(Strip_S11); 
	Charge.push_back(Q_S11);

	//Second plan touched
	if(EventDCData->GetSpegDCMultStrip12()>0)
	{
		for(int j=0;j<EventDCData->GetSpegDCMultStrip12();j++)
		{
                	if((EventDCData->GetSpegDCStripNbr12(j))>0 && (EventDCData->GetSpegDCStripNbr12(j))<=number_strip && (EventDCData->GetSpegDCEnergy12(j))<seuil_haut)
			{  
				Strip_S12.push_back(EventDCData->GetSpegDCStripNbr12(j));
				if(bool_CORRECTION_COEF_DC_12 && EventDCData->GetSpegDCStripNbr12(j)==m_BadStrip_SPEG_DC12[EventDCData->GetSpegDCStripNbr12(j)-1])
				{
					Q_S12.push_back((EventDCData->GetSpegDCEnergy12(j)+gRandom->Uniform(0,1)-0.5)*m_CorrectionCoef_SPEG_DC12[EventDCData->GetSpegDCStripNbr12(j)-1]);
				}
				else
				{
					Q_S12.push_back((CalibrationManager::getInstance()->ApplyCalibration("SPEG/_DC_S12_"+ NPL::itoa(EventDCData->GetSpegDCStripNbr12(j)),(CalibrationManager::getInstance()->ApplyCalibration("SPEG/_DC_S12_decal_"+ NPL::itoa(EventDCData->GetSpegDCStripNbr12(j)), EventDCData->GetSpegDCEnergy12(j)))))+gRandom->Uniform(0,1)-0.5);
				}    
			}
		}
	}
	Strip.push_back(Strip_S12); 
	Charge.push_back(Q_S12);

	//Third plan touched (the geometry of the chamber is different)
	if(EventDCData->GetSpegDCMultStrip21()>0)
	{
		for(int j=0;j<EventDCData->GetSpegDCMultStrip21();j++)
		{
			if((EventDCData->GetSpegDCStripNbr21(j))>0 && (EventDCData->GetSpegDCStripNbr21(j))<=number_strip && (EventDCData->GetSpegDCEnergy21(j))<seuil_haut)
			{ 
				Strip_S21.push_back(129-EventDCData->GetSpegDCStripNbr21(j));
				if(bool_CORRECTION_COEF_DC_21 && 129-EventDCData->GetSpegDCStripNbr21(j)==m_BadStrip_SPEG_DC21[129-EventDCData->GetSpegDCStripNbr21(j)-1])
				{
					Q_S21.push_back((EventDCData->GetSpegDCEnergy21(j)+gRandom->Uniform(0,1)-0.5)*m_CorrectionCoef_SPEG_DC21[129-EventDCData->GetSpegDCStripNbr21(j)-1]);
				}
				else
				{
					Q_S21.push_back((CalibrationManager::getInstance()->ApplyCalibration("SPEG/_DC_S21_"+ NPL::itoa(129-EventDCData->GetSpegDCStripNbr21(j)),(CalibrationManager::getInstance()->ApplyCalibration("SPEG/_DC_S21_decal_"+ NPL::itoa(EventDCData->GetSpegDCStripNbr21(j)), EventDCData->GetSpegDCEnergy21(j)))))+gRandom->Uniform(0,1)-0.5);
				}    
			}
		}
	}
	Strip.push_back(Strip_S21);  
	Charge.push_back(Q_S21);

	//Fourth plan touched (the geometry of the chamber is different)
	if(EventDCData->GetSpegDCMultStrip22()>0)
	{
		for(int j=0;j<EventDCData->GetSpegDCMultStrip22();j++)
		{
               		if((EventDCData->GetSpegDCStripNbr22(j))>0 && (EventDCData->GetSpegDCStripNbr22(j))<=number_strip && (EventDCData->GetSpegDCEnergy22(j))<seuil_haut)
			{ 
				Strip_S22.push_back(129-EventDCData->GetSpegDCStripNbr22(j));
				if(bool_CORRECTION_COEF_DC_22 && 129-EventDCData->GetSpegDCStripNbr22(j)==m_BadStrip_SPEG_DC22[129-EventDCData->GetSpegDCStripNbr22(j)-1])
				{
					Q_S22.push_back((EventDCData->GetSpegDCEnergy22(j)+gRandom->Uniform(0,1)-0.5)*m_CorrectionCoef_SPEG_DC22[129-EventDCData->GetSpegDCStripNbr22(j)-1]);
				}
				else
				{
					Q_S22.push_back((CalibrationManager::getInstance()->ApplyCalibration("SPEG/_DC_S22_"+ NPL::itoa(129-EventDCData->GetSpegDCStripNbr22(j)),(CalibrationManager::getInstance()->ApplyCalibration("SPEG/_DC_S22_decal_"+ NPL::itoa(EventDCData->GetSpegDCStripNbr22(j)), EventDCData->GetSpegDCEnergy22(j)))))+gRandom->Uniform(0,1)-0.5);
				}    
			}
		}
	}
	Strip.push_back(Strip_S22);  
	Charge.push_back(Q_S22);

	//Loop over the strips plan (=4)
	for(int k=0;k<number_strips_plan;k++)
	{
		//Variables SECHS methode
		q_c = 0;//Central charge
		q_r = 0;//Right charge (right of the q_c)
		q_l = 0;//Left charge (left of the q_c)
		i_c = 0;//Central strip 
		i_l = 0;//Left strip (left of the central strip)
		i_r = 0;//Left strip (left of the central strip)

		//Event validation test : more than 3 strips touched
		if((int)Strip[k].size()>=number_strips_plan-1 && Strip.size()==Charge.size())
		{
        		qmax[k]=1;
        		imax[k]=0;
			//Multiplicity loop
			for(unsigned int j=1;j<Strip[k].size();j++)
			{
         			if(Charge[k][j]>qmax[k] && Strip[k][j]>3 && Strip[k][j]<126 && Charge[k][j]!=Charge[k].back())
				{
                 			qmax[k] = Charge[k][j];
                			imax[k] = Strip[k][j];
                 			q_c = qmax[k];
					i_c = imax[k];
                 			x_c = (imax[k]-0.5)*pas;

					//Inversion of the chamber 3 and 4 numerotation
					if(k<2) 
					{
                   				if(Strip[k][j-1]==(Strip[k][j]-1))              							{
							q_l = Charge[k][j-1];
							i_l = Strip[k][j-1];
						}
                   				if(Strip[k][j+1]==(Strip[k][j]+1))
						{ 
	               					q_r = Charge[k][j+1];
							i_r = Strip[k][j+1];
						}
					} 
                			else      
					{
                   				if(Strip[k][j-1]==(Strip[k][j]+1)) 
						{
                                 			q_l = Charge[k][j+1];
							i_l = Strip[k][j-1];
						}
                   				if(Strip[k][j+1]==(Strip[k][j]-1))
						{ 
                                 			q_r = Charge[k][j-1];
							i_r = Strip[k][j+1];
						}
					}
				}	
			}

			//Continuity and limits check
			if(q_c>q_r && q_c>q_l && q_r>0 && q_l>0 && q_c>0 && i_l!=0 && i_c!=0 && i_r !=0)
			{
				//Gravity center with 3 points (XCOG) calculation
				x3cg[k] =pas*(alpha[k]*(q_r - q_l)/(q_c + q_l + q_r)+imax[k]-0.5);
				//Gaussian center with 3 points
				xg[k] = 0.5*((log(q_c/q_l)*(pas*(i_r-0.5)*pas*(i_r-0.5)-pas*(i_c-0.5)*pas*(i_c-0.5))-log(q_c/q_r)*(pas*(i_l-0.5)*pas*(i_l-0.5)-pas*(i_c-0.5)*pas*(i_c-0.5)))/(log(q_c/q_l)*(pas*(i_r-0.5)-pas*(i_c-0.5))-log(q_c/q_r)*(pas*(i_l-0.5)-pas*(i_c-0.5))));
				//Gravity center with 3 points (SECHS) calculation (K.LAU et al., NIM A 366 (1995) 298-309)
				temp = 0.5*(sqrt(q_c/q_l)+sqrt(q_c/q_r));
				cosih = log(temp + sqrt(temp*temp - 1.));
				a3 = (pi*a1_1)/cosih;
				temp2 = 0.5*(sqrt(q_c/q_l)-sqrt(q_c/q_r));
				sinuh = sinh((pi*a1_1)/a3);
				temp3 = temp2/sinuh;
				a2 = (a3/pi)*0.5*(log((1.+temp3)/(1.-temp3)));
				//Calculated position value : we add the position of the most strip touched
				xsh[k] = x_c + a2;
			}
		}
	}
	x11 = xsh[0];
	x12 = xsh[1];
	x21 = xsh[2];
	x22 = xsh[3];

	//Conversion : mm->channel, 700 mm*40->28000 channel, 25 µm per channel with alpha coefficient taken into account
	x11 = x11 * 40.; 
	x12 = x12 * 40.; 
	x21 = x21 * 40.; 
	x22 = x22 * 40.; 
	x3cg[0] = x3cg[0] * 40.;
	x3cg[1] = x3cg[1] * 40.;
	x3cg[2] = x3cg[2]* 40.;
	x3cg[3] = x3cg[3] * 40.;
	xg[0] = xg[0]*40;
	xg[1] = xg[1]*40;
	xg[2] = xg[2]*40;
	xg[3] = xg[3]*40;
	Tab_xij[0]=x11;
	Tab_xij[1]=x12;
	Tab_xij[2]=x21;
	Tab_xij[3]=x22;
	Tab_yij[0]=y11;
	Tab_yij[1]=y12;
	Tab_yij[2]=y21;
	Tab_yij[3]=y22;

/*----------------------------------Subroutine Caltrx (reconstruction of the trajectory)---------------------*/
	//xij!=0
	if(Tab_xij[0]>0 && Tab_xij[1]>0 && Tab_xij[2]>0 && Tab_xij[3]>0)
	{
		b1=0;
		b2=0;
		for(int k=0;k<nmesx;k++)
		{ 
			Vector_x.push_back((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k));
			b1 = b1+Vector_x.at(k)*Vector_zx.at(k)/Vector_sigx2.at(k);
			b2 = b2+Vector_x.at(k)/Vector_sigx2.at(k);
		}
		ax = b1*ax11+b2*ax12;
		bx = b1*ax21+b2*ax22;
		//xfoc=(ax*zfocx+bx)*ffx+zfx;
		//Calculation with the angle
		xfoc=(ax*(tanfox*zfocx+bx)/(tanfox-ax)+bx)*ffx+zfx;	
		tfoc=ax*fax+zax;
		xPL=(ax*328.3+bx)*ffx+zfx;

		//Khi2 calculation
		Khi2=0;
		for(int k=0;k<nmesx;k++)
		{   
			xt=ax*Vector_zx.at(k)+bx;
			Khi2=Khi2+((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k)-xt)*((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k)-xt);
		}
		Khi2=Khi2/2;

		//Desallocation memory
		Vector_x.clear();
	}

	//x11==0
	else if(Tab_xij[1]>0 && Tab_xij[2]>0 && Tab_xij[3]>0)
	{
		b1=0;
		b2=0;
		for(int k=1;k<nmesx;k++)
		{ 
			Vector_x.push_back((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k));
			b1 = b1+Vector_x.at(k-1)*Vector_zx.at(k)/Vector_sigx2.at(k);
			b2 = b2+Vector_x.at(k-1)/Vector_sigx2.at(k);
		}
		ax = b1*ax11+b2*ax12;
		bx = b1*ax21+b2*ax22;
		//xfoc=(ax*zfocx+bx)*ffx+zfx;
		//Calculation with the angle
		xfoc=(ax*(tanfox*zfocx+bx)/(tanfox-ax)+bx)*ffx+zfx;	
		tfoc=ax*fax+zax;

		//Khi2 calculation
		Khi2=0;
		for(int k=1;k<nmesx;k++)
		{   
			xt=ax*Vector_zx.at(k)+bx;
			Khi2=Khi2+((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k)-xt)*((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k)-xt);
		}
		Khi2=Khi2/1;

		//Desallocation memory
		Vector_x.clear();
	}
	//x12==0
	else if(Tab_xij[0]>0 && Tab_xij[2]>0 && Tab_xij[3]>0)
	{
		b1=0;
		b2=0;

		Vector_x.push_back((Tab_xij[0]-Vector_x0f.at(0))*Vector_etalx.at(0));
		b1 = b1+Vector_x.at(0)*Vector_zx.at(0)/Vector_sigx2.at(0);
		b2 = b2+Vector_x.at(0)/Vector_sigx2.at(0);
		for(int k=2;k<nmesx;k++)
		{ 
			Vector_x.push_back((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k));
			b1 = b1+Vector_x.at(k-1)*Vector_zx.at(k)/Vector_sigx2.at(k);
			b2 = b2+Vector_x.at(k-1)/Vector_sigx2.at(k);
		}
		ax = b1*ax11+b2*ax12;
		bx = b1*ax21+b2*ax22;
		//xfoc=(ax*zfocx+bx)*ffx+zfx;
		//Calculation with the angle
		xfoc=(ax*(tanfox*zfocx+bx)/(tanfox-ax)+bx)*ffx+zfx;	
		tfoc=ax*fax+zax;

		//Khi2 calculation
		Khi2=0;
		xt=ax*Vector_zx.at(0)+bx;
		Khi2=((Tab_xij[0]-Vector_x0f.at(0))*Vector_etalx.at(0)-xt)*((Tab_xij[0]-Vector_x0f.at(0))*Vector_etalx.at(0)-xt);
		for(int k=2;k<nmesx;k++)
		{   
			xt=ax*Vector_zx.at(k)+bx;
			Khi2=Khi2+((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k)-xt)*((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k)-xt);
		}
		Khi2=Khi2/1;

		//Desallocation memory
		Vector_x.clear();
	}
	//x21==0
	else if(Tab_xij[0]>0 && Tab_xij[1]>0 && Tab_xij[3]>0)
	{
		b1=0;
		b2=0;

		for(int k=0;k<nmesx-2;k++)
		{ 
			Vector_x.push_back((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k));
			b1 = b1+Vector_x.at(k)*Vector_zx.at(k)/Vector_sigx2.at(k);
			b2 = b2+Vector_x.at(k)/Vector_sigx2.at(k);
		}
		Vector_x.push_back((Tab_xij[3]-Vector_x0f.at(3))*Vector_etalx.at(3));
		b1 = b1+Vector_x.at(2)*Vector_zx.at(3)/Vector_sigx2.at(3);
		b2 = b2+Vector_x.at(2)/Vector_sigx2.at(3);
		ax = b1*ax11+b2*ax12;
		bx = b1*ax21+b2*ax22;
		//xfoc=(ax*zfocx+bx)*ffx+zfx;
		//Calculation with the angle
		xfoc=(ax*(tanfox*zfocx+bx)/(tanfox-ax)+bx)*ffx+zfx;	
		tfoc=ax*fax+zax;

		//Khi2 calculation
		Khi2=0;
		for(int k=0;k<nmesx-2;k++)
		{   
			xt=ax*Vector_zx.at(k)+bx;
			Khi2=Khi2+((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k)-xt)*((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k)-xt);
		}
		xt=ax*Vector_zx.at(3)+bx;
		Khi2=Khi2+((Tab_xij[3]-Vector_x0f.at(3))*Vector_etalx.at(3)-xt)*((Tab_xij[3]-Vector_x0f.at(3))*Vector_etalx.at(3)-xt);
		Khi2=Khi2/1;	

		//Desallocation memory
		Vector_x.clear();
	}
	//x22==0
	else if(Tab_xij[0]>0 && Tab_xij[1]>0 && Tab_xij[2]>0)
	{
		b1=0;
		b2=0;
		for(int k=0;k<nmesx-1;k++)
		{ 
			Vector_x.push_back((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k));
			b1 = b1+Vector_x.at(k)*Vector_zx.at(k)/Vector_sigx2.at(k);
			b2 = b2+Vector_x.at(k)/Vector_sigx2.at(k);
		}
		ax = b1*ax11+b2*ax12;
		bx = b1*ax21+b2*ax22;
		//xfoc=(ax*zfocx+bx)*ffx+zfx;
		//Calculation with the angle
		xfoc=(ax*(tanfox*zfocx+bx)/(tanfox-ax)+bx)*ffx+zfx;	
		tfoc=ax*fax+zax;

		//Khi2 calculation
		Khi2=0;
		for(int k=0;k<nmesx-1;k++)
		{   
			xt=ax*Vector_zx.at(k)+bx;
			Khi2=Khi2+((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k)-xt)*((Tab_xij[k]-Vector_x0f.at(k))*Vector_etalx.at(k)-xt);
		}
		Khi2=Khi2/1;

		//Desallocation memory
		Vector_x.clear();
	}
	else
	{
		xfoc=-1000;
		tfoc=-1000;
	}

/*----------------------------------Subroutine Caltry (reconstruction of the trajectory)---------------------*/

	b1=0;
	b2=0;
	for(int k=0;k<nmesy;k++)
	{   
		Vector_y.push_back((Tab_yij[k]-Vector_y0f[k])*Vector_etaly[k]);
		b1 = b1+Vector_y[k]*Vector_zy[k]/Vector_sigy2[k];
		b2 = b2+Vector_y[k]/Vector_sigy2[k];
	}	
	ay = b1*ay11+b2*ay12;
	by = b1*ay21+b2*ay22; 
	phifoc=ay*fay+zay; 
	yfoc=(ay*(tanfoy*zfocy+by)/(tanfoy-ay)+by)*ffy+zfy;
	Y_FocalPlan.push_back(yfoc);

	//Desallocation memory
	Vector_y.clear();
	
/*--------------xfoc correction and calibration--------------------*/

	if(xfoc>0 && tfoc>0)
	{
		//Correction of xfoc by thetafoc and phifoc
		cor_xfoc_thetafoc=(CalibrationManager::getInstance()->GetCorrection("SPEG/_xfoc_cor_theta"));
		cor_xfoc_phifoc=(CalibrationManager::getInstance()->GetCorrection("SPEG/_xfoc_cor_phi"));
      	
		xfoc_cor_thetafoc=xfoc;
		for(unsigned int n = 0 ; n < cor_xfoc_thetafoc.size() ; n++)
		{
			xfoc_cor_thetafoc -= cor_xfoc_thetafoc[n]*pow(tfoc,(int)n);
		}
	
		xfoc_cor_phifoc=xfoc_cor_thetafoc;
		for(unsigned int n = 0 ; n < cor_xfoc_phifoc.size() ; n++)
		{
			xfoc_cor_phifoc -= cor_xfoc_phifoc[n]*pow(phifoc,(int)n);
		}

		//Calibration of xfoc
	      	xfoc_calibrated = (CalibrationManager::getInstance()->ApplyCalibration("SPEG/_xfoc",xfoc_cor_phifoc));
		X_FocalPlan.push_back(xfoc_calibrated);

/*--------------Angle correction and calibration--------------------*/

		//thetafoc Correction 
		thetafoc_cor=tfoc;
	     	for(int n = 0 ; n < degree_of_correction_angle_with_Brho+1 ; n++)
	     	{
			param_theta = 0;
			cor_theta = (CalibrationManager::getInstance()->GetCorrection("SPEG/_tfoc_correction_with_Brho_"+ NPL::itoa(n)));
			for(unsigned int l=0; l<cor_theta.size() ; l++)
			{
				param_theta += cor_theta[l]*pow(xfoc_cor_phifoc,(int)l);
			}
			correction_theta_with_Brho.push_back(param_theta);
			thetafoc_cor -= correction_theta_with_Brho[n]*pow(phifoc,n);
		}

		//phifoc Correction 
		phifoc_cor=phifoc;
	     	for(int n = 0 ; n < degree_of_correction_angle_with_Brho+1 ; n++)
	     	{
	     		param_phi = 0;
			cor_phi = (CalibrationManager::getInstance()->GetCorrection("SPEG/_phifoc_correction_with_Brho_"+ NPL::itoa(n)));
			for(unsigned int l=0; l<cor_phi.size() ; l++)
			{
				param_phi += cor_phi[l]*pow(xfoc_cor_phifoc,(int)l);
			}
			correction_phi_with_Brho.push_back(param_phi);
			phifoc_cor -= correction_phi_with_Brho[n]*pow(tfoc,n);
		}

		//Calibration of tfoc
		tfoc_calibrated = 0;
	     	for(int n = 0 ; n < degree_of_calibration_angle_with_Brho+1 ; n++)
	     	{
			param_theta = 0;
			cal_theta = (CalibrationManager::getInstance()->GetCorrection("SPEG/_tfoc_calibration_with_Brho_"+ NPL::itoa(n)));
			for(unsigned int l=0; l<cal_theta.size() ; l++)
			{
				param_theta += cal_theta[l]*pow(xfoc_cor_phifoc,(int)l);
			}
			calibration_theta_with_Brho.push_back(param_theta);
			tfoc_calibrated += calibration_theta_with_Brho[n]*pow(thetafoc_cor,n);
		}
		if(cal_theta.size()==0)
		{
			Theta_FocalPlan.push_back(tfoc);
		}
		else
		{
			Theta_FocalPlan.push_back(tfoc_calibrated);
		}

		//Calibration of phifoc
		phifoc_calibrated = 0;
	     	for(int n = 0 ; n < degree_of_calibration_angle_with_Brho+1 ; n++)
	     	{
			param_phi = 0;
			cal_phi = (CalibrationManager::getInstance()->GetCorrection("SPEG/_phifoc_calibration_with_Brho_"+ NPL::itoa(n)));
			for(unsigned int l=0; l<cal_phi.size() ; l++)
			{
				param_phi += cal_phi[l]*pow(xfoc_cor_phifoc,(int)l);
			}
			calibration_phi_with_Brho.push_back(param_phi);
			phifoc_calibrated += calibration_phi_with_Brho[n]*pow(phifoc_cor,n);
		}
		if(cal_phi.size()==0)
		{
			Phi_FocalPlan.push_back(phifoc);
		}
		else
		{
			Phi_FocalPlan.push_back(phifoc_calibrated);
		}
	}
	else
	{
		Theta_FocalPlan.push_back(-1000);
		X_FocalPlan.push_back(-1000);
	}

/*-----------------------------------Subroutine Aberration---------------------------------------------------*/
/*
	//Xfoc and yfoc calculation, position in the focal plan in centimeter
	double xfoc_aber=ax*(tanfox*zfocx+bx)/(tanfox-ax)+bx;
	xfoc_aber=xfoc_aber-8.04;
	double yfoc_aber=ay*(tanfoy*zfocy+by)/(tanfoy-ay)+by;
	//ax=theta angle in the focal plan in radian calculated in caltrx
	//ay=phi angle in the focal plan in radian calculated in caltry
	double thetafoc_aber=ax*1000.; 
	double phifoc_aber=ay*1000.;
	//Theta, delta calculation in first order
        double theta1=(0.076817*(xfoc_aber-0.2862)-8.1842556*(thetafoc_aber+0.1965))/10.086875;
        double delta1=(1.2323786*(xfoc_aber-0.2862)+0.0010068*(thetafoc_aber+0.1965))/10.086875;
	if(delta1<-10.)
	{
		delta1=-10.;
	}
	if(delta1>10)
	{
		delta1=10.;
	}
	//Phi calculation in first order. We calculate phi1 with yfoc or phifoc in respect with phi/phi = phisphi and with y/phi = =ysphi
        double phisphi=-0.15515+0.044196*delta1-7.8E-5*delta1*delta1;
        double ysphi=0.0280218+0.021471*delta1+5.3E-6*delta1*delta1;
        if(phisphi!=0 && ysphi!=0)
	{
        	phia1=phifoc_aber/phisphi;
       		phib1=yfoc_aber/ysphi;
       		sigmaphi1=1./phisphi;
       	 	sigmay1=1./ysphi;
        	phi1=(phia1/(sigmaphi1*sigmaphi1)+phib1/(sigmay1*sigmay1))/(1./(sigmaphi1*sigmaphi1)+1./(sigmay1*sigmay1));
		//Theta, delta calculation in second order
                xa=xfoc_aber-0.2862-0.00002*theta1*theta1+4.4E-5*phi1*phi1+0.12862*delta1*delta1;
                xb=thetafoc_aber+0.1965+0.002298*theta1*theta1-4.96E-5*phi1*phi1-0.0221*delta1*delta1+0.02441*theta1*delta1;
                theta2=(0.076817*xa-8.1842556*xb)/10.086875;
                delta2=(1.2323786*xa+0.0010068*xb)/10.086875;   
	        if(delta2<-10)
		{
			delta2=-10.;
		}
	        if(delta2>10)
		{
			delta2=10.;
		}  
	}
	//Phi calculation in second order
	double phi2= phi1+(8.13E-3-1.E-3*delta2-8.5E-4*delta2*delta2)*phi1*theta2+(-1.61E-4-1.38E-5*delta2+7.93E-6*delta2*delta2)*phi1*theta2*theta2;
	//Phi calculation in first order, correction of phi/phi in second order
	double help=(1.26-0.036*delta2-0.031*delta2*delta2);
        if(help!=0)
	{
        	phi2=phi2/help;
	}
	//Transformation of mradian and centimeter in channel
        delta1=delta1+0.98;
        delta2=delta2+0.98;
      	delta1=delta1*1500.+15000.;
      	delta2=delta2*1500.+15000.;
        theta1=theta1*50.+5750.;
        theta2=theta2*50.+5750.;
        phi1=phi1*50.+3750.; 
        phi2=phi2*50.+3750.; */

/*-----------------------------------Corrected parameters calculation-----------------------------*/

	if(xfoc>0 && tfoc>0)
	{
		//Time
		tem = (tema-SPEG_PLASTIC_tpld+gRandom->Uniform(0,1)-0.5)*temb;
		//Theta foc corrected
		tcor = tem*(tcora*(tfoc-tcorb)+1);
		//Correction compared to xfoc => m/q corrected
		cmsurq = (cmsurqa*(xfoc-cmsurqb)+1)*tcor;
		M_over_Q.push_back(cmsurq);
		//Charge z calculated with anode signal and theta corrected
		if(tcor>0 && SPEG_CHIO_anode>0)
		{       
			z = (sqrt(SPEG_CHIO_anode)/pow(tcor,za))*zb;
			Z.push_back(z);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TSpegPhysics::Construct(){
  return (NPL::VDetector*) new TSpegPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy{
  public:
    proxy(){
      NPL::DetectorFactory::getInstance()->AddToken("Speg","Speg");
      NPL::DetectorFactory::getInstance()->AddDetector("Speg",TSpegPhysics::Construct);
    }
};

proxy p;
}


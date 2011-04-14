/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: S. Giron   contact address: giron@ipno.in2p3.fr          *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold exogam treated data                                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#ifndef TEXOGAMPHYSICS_H
#define TEXOGAMPHYSICS_H

// STL
#include <vector>
#include <map>

// NPL
#include "../include/CalibrationManager.h"
#include "../include/VDetector.h"
#include "TExogamData.h"

// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"
#include "TH1.h"

using namespace std ;

class TExogamPhysics : public TObject, public NPA::VDetector
{
 public:
  TExogamPhysics()	;
  ~TExogamPhysics() {};

  
 public: 
  void Clear()	              ;	
  void Clear(const Option_t*) {};

  //	Provide Physical Multiplicity
  Int_t	 		EventMultiplicity	;
  Int_t                 ECC_Multiplicity        ;
  Int_t                 GOCCE_Multiplicity      ;
  Int_t                 NumberOfClover          ; //!
  
  // Clover
  Int_t                 NumberOfHitClover       ; //!
  Int_t                 NumberOfHitCristal      ; //!
  vector<int>		ECC_CloverNumber		;   
  vector<int>		ECC_CristalNumber		;
  vector<int>		GOCCE_CloverNumber		;   
  vector<int>		GOCCE_CristalNumber		;
  vector<int>		GOCCE_SegmentNumber		;
    	
  //	ECC
  vector<double>	ECC_E				;
  vector<double>	ECC_T				;
  
  //	GOCCE
  vector<double>	GOCCE_E				;
 
  //  Add-Back and Doppler correction
  
  vector<int>      CristalNumber                     ;
  vector<int>      SegmentNumber                     ;
  vector<int>      CloverNumber                      ;
  int              CloverMult                        ;
  
  vector<double>   TotalEnergy_lab                   ;
  vector<double>   Time                              ;
  vector<double>   DopplerCorrectedEnergy            ;
  vector<double>   Position                          ;
  vector<double>   Theta                             ;

  vector < vector < vector < vector <double> > > > Clover_Angles_Theta_Phi;   //!
 
  /* 
  TH1F*                 clover_mult                  ;  
  TH1F*                 cristal_mult                 ;  
  */

 public:		//	Innherited from VDetector Class
			
  //	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
  void ReadConfiguration(string) 				;
		

  //	Add Parameter to the CalibrationManger
  void AddParameterToCalibrationManager()	;		
			
		
  //	Activated associated Branches and link it to the private member DetectorData address
  //	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
  void InitializeRootInput() 					;


  //	Create associated branches and associated private member DetectorPhysics address
  void InitializeRootOutput() 		 		;
		
  //	This method is called at each event read from the Input Tree. Aim is to build a tree of calibrated data.
  void PreTreat()			       ;

  //	This method is called at each event read from the Input Tree. 
  void BuildPhysicalEvent()					;
		
		
  //	Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
  //	This method aimed to be used for analysis performed during experiment, when speed is requiered.
  //	NB: This method can eventually be the same as BuildPhysicalEvent.
  void BuildSimplePhysicalEvent()	       ;

  double DopplerCorrection(double Energy, double Theta);

  //	Those two method all to clear the Event Physics or Data
  void ClearEventPhysics()		{Clear();}		
  void ClearEventData()			{EventData->Clear();}	
  void ClearPreTreatedData()	        {PreTreatedData->Clear();}

 private:	//	Root Input and Output tree classes

 				
  TExogamData* 	          EventData		;//!
  TExogamData* 	          PreTreatedData	;//!
  TExogamPhysics* 	  EventPhysics		;//!
  

 public:		//	Specific to EXOGAM Array
  //	Add a Clover
  void AddClover(string AngleFile);

  Int_t GetClover_Mult()    { return(CloverNumber.size()); }
  //  Int_t GetECC_Mult()   { return(ECC_CristalNumber.size()); }
  //  Int_t GetGOCCE_Mult() { return(GOCCE_SegmentNumber.size()); }

  Double_t GetSegmentAnglePhi(int Clover, int Cristal, int Segment)    {return(Clover_Angles_Theta_Phi[Clover][Cristal][Segment][1]);};
  Double_t GetSegmentAngleTheta(int Clover, int Cristal, int Segment)  {return(Clover_Angles_Theta_Phi[Clover][Cristal][Segment][0]);};
  
		

  ClassDef(TExogamPhysics,1)  // ExogamPhysics structure
    };

namespace EXOGAM_LOCAL
{
   const double Threshold_ECC   = 50;
   const double Threshold_GOCCE = 0;
   const double RawThreshold_ECC   = 0;
   const double RawThreshold_GOCCE = 0;
   
   
   //	tranform an integer to a string
   string itoa(int value);

 
}


#endif

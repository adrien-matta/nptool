#ifndef TEXOGAMPHYSICS_H
#define TEXOGAMPHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: S. Giron   contact address: giron@ipno.in2p3.fr          *
 *                  B. Le Crom                  lecrom@ipno.in2p3.fr         *
 * Creation Date  : march 2014                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold exogam treated data                                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/


// STL
#include <vector>
#include <map>

// NPL
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "TExogamData.h"
#include "TExogamSpectra.h"
#include "NPInputParser.h"
// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"
//#include "TH1.h"

using namespace std ;
// Forward Declaration
class TExogamSpectra;

class TExogamPhysics : public TObject, public NPL::VDetector{
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
  void ReadConfiguration(NPL::InputParser) 				;
		

  //	Add Parameter to the CalibrationManger
  void AddParameterToCalibrationManager()	;		
			
		
  //	Activated associated Branches and link it to the private member DetectorData address
  //	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
  void InitializeRootInputRaw() 					;

    //   Activated associated Branches and link it to the private member DetectorPhysics address
    //   In this method mother Branches (Detector) AND daughter leaf (parameter) have to be activated
    void InitializeRootInputPhysics() ;/////////////////////////////////////////////////////////////////////////

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

    // Method related to the TSpectra classes, aimed at providing a framework for online applications
    // Instantiate the Spectra class and the histogramm throught it
    void InitSpectra();
    // Fill the spectra hold by the spectra class
    void FillSpectra();
    // Used for Online mainly, perform check on the histo and for example change their color if issues are found
    void CheckSpectra();
    // Used for Online only, clear all the spectra hold by the Spectra class
    void ClearSpectra();

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
 
  // Give and external TMustData object to TExogamPhysics. Needed for online analysis for example.
  void SetRawDataPointer(TExogamData* rawDataPointer) {EventData = rawDataPointer;}
  // Retrieve raw and pre-treated data
  TExogamData* GetRawData()        const {return EventData;}
  TExogamData* GetPreTreatedData() const {return PreTreatedData;}

 
  private: // Spectra Class   
    TExogamSpectra*      m_Spectra;//! 

  public: // Spectra Getter
    map< string , TH1*> GetSpectra(); 		

  public: // Static constructor to be passed to the Detector Factory
     static NPL::VDetector* Construct();
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

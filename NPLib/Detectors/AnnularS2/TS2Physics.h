#ifndef TS2PHYSICS_H
#define TS2PHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Sandra GIRON  contact address: giron@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : april 2011                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold annular S2 treated data                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// STL
#include <vector>

// NPL
#include "TS2Data.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPInputParser.h"
// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"

using namespace std ;

class TS2Physics : public TObject, public NPL::VDetector
{
 public:
  TS2Physics()	;
  ~TS2Physics() {};
  
 public: 
  void Clear()									;	
  void Clear(const Option_t*) {};
  
 public: 
  vector < TVector2 > Match_Theta_Phi() ;
  // bool Match_Theta_Phi(int Theta, int Phi);

  int  CheckEvent();
  bool ResolvePseudoEvent();
  
	public:
  
  //	Provide Physical Multiplicity
  Int_t			EventMultiplicity	;
  
  //	Provide a Classification of Event
	vector<int>		EventType		;

	// Annular
	vector<int>		AnnularNumber		;   
	
	
	//	Strips
	
	vector<double>	Si_E				;
	vector<double>	Si_T				;
	vector<int>	Si_Theta	                ;
	vector<int>	Si_Phi		                ;
	
	vector<int>	AnnularNumber_Theta		;   //!
 	vector<int>	AnnularNumber_Phi		;   //!
	
	// Use for checking purpose
	vector<double>	Si_ETheta			;
	vector<double>	Si_TTheta			;
	vector<double>	Si_EPhi				;
	vector<double>	Si_TPhi				;
	
	/*
	  vector<int>	NTheta   ; //!
	vector<int>     StrTheta ; 
	vector<double> 	ETheta   ;
	vector<double> 	TTheta    ;
	
	vector<int> 	NPhi   ; //!
	vector<int> 	StrPhi ;
	vector<double>  EPhi   ;
	vector<double>  TPhi 	;
	*/
	
 public:		//	Innherited from VDetector Class
	
	//	Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
	void ReadConfiguration(NPL::InputParser) 				;
	
	
	//	Add Parameter to the CalibrationManger
	void AddParameterToCalibrationManager()	;		
	
	
	//	Activated associated Branches and link it to the private member DetectorData address
	//	In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
	void InitializeRootInput() 					;
	
	
	//	Create associated branches and associated private member DetectorPhysics address
	void InitializeRootOutput() 		 		;
	
	
	//	This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
	void BuildPhysicalEvent()					;
	
	//	Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
	//	This method aimed to be used for analysis performed during experiment, when speed is requiered.
	//	NB: This method can eventually be the same as BuildPhysicalEvent.
	void BuildSimplePhysicalEvent()				;
	
	//	Those two method all to clear the Event Physics or Data
	void ClearEventPhysics()		{  Clear(); }		
	void ClearEventData()				{m_EventData->Clear();}	
	


 public:		//	Specific to S2 Array
	
	//	Clear The PreTeated object
	void ClearPreTreatedData()	{m_PreTreatedData->Clear();}
	
	//	Remove bad channel, calibrate the data and apply threshold
	void PreTreat();
	
	//	Return false if the channel is disabled by user
	//	First argument is either "THETA","PHI"
	bool IsValidChannel(string DetectorType, int telescope , int channel);
	
	//	Initialize the standard parameter for analysis
	//	ie: all channel enable, maximum multiplicity for strip = number of telescope (must2)
	void InitializeStandardParameter();
	
	//	Read the user configuration file; if no file found, load standard one
	void ReadAnalysisConfig();
	
	//	Add a Annular using Corner Coordinate information
	void AddAnnular(TVector3 C_Center       ,
			TVector3 C_Phi2_Phi3 	,
			TVector3 C_Phi6_Phi7 	, 
			TVector3 C_Phi10_Phi11	, 
			TVector3 C_Phi14_Phi15	);
	
	// Use for reading Calibration Run, very simple methods; only apply calibration, no condition
	void ReadCalibrationRun();

	bool Match_Theta_Phi_Position(int theta, int phi);
	
	// Use to access the strip position
	double GetStripPositionX( const int N, const int Phi , const int Theta )	const{ return m_StripPositionX[N-1][Phi-1][Theta-1] ; }  ;
	double GetStripPositionY( const int N, const int Phi , const int Theta )	const{ return m_StripPositionY[N-1][Phi-1][Theta-1] ; }  ;
	double GetStripPositionZ( const int N, const int Phi , const int Theta )	const{ return m_StripPositionZ[N-1][Phi-1][Theta-1] ; }  ;
	
	double GetNumberOfAnnular() const { return m_NumberOfAnnular ; }  	;

	// To be called after a build Physical Event 
	int GetEventMultiplicity() const { return EventMultiplicity; } ;
	
	void Dump_Positions();

	TVector3 GetPositionOfInteraction(const int i) const  ;	
	TVector3 GetAnnularNormal( const int i) const ;
	
 private:	//	Parameter used in the analysis
	
		// By default take ETheta and TPhi.
	bool m_Take_E_Phi;//!
	bool m_Take_T_Phi;//!
	
	
	//	Event over this value after pre-treatment are not treated / avoid long treatment time on spurious event	
	int m_MaximumStripMultiplicityAllowed  ;//!
	//	Give the allowance in percent of the difference in energy between Theta and Phi
	double m_StripEnergyMatchingSigma  ; //!
	double m_StripEnergyMatchingNumberOfSigma  ; //!
	
	// Raw Threshold
	int m_S2_Theta_E_RAW_Threshold ;//!
	int m_S2_Phi_E_RAW_Threshold ;//!
		
	// Calibrated Threshold
	double m_S2_Theta_T_Threshold;     //!
	double m_S2_Phi_T_Threshold;       //!
	double m_S2_Theta_E_Threshold ;    //!
	double m_S2_Phi_E_Threshold ;      //!
	double m_S2_Theta_E_Threshold_sup ;//!
	double m_S2_Phi_E_Threshold_sup ;  //!
		
			
 private:	//	Root Input and Output tree classes
				
	TS2Data* 	  	m_EventData		;//!
	TS2Data* 	  	m_PreTreatedData	;//!
	TS2Physics* 	m_EventPhysics		;//!


 private:	//	Map of activated channel
	map< int, vector<bool> > m_ThetaChannelStatus;//!
	map< int, vector<bool> > m_PhiChannelStatus;//! 

 private:	//	Spatial Position of Strip Calculated on bases of detector position
	
	int m_NumberOfAnnular	;//!
	
 private:
	//////////////////////////////
	// Geometry and strip number //
	//////////////////////////////
	int m_NumberOfStripsTheta;        //!
	int m_NumberOfStripsPhi;          //!
	int m_NumberOfQuadrants;          //!
	
	vector < vector < vector < double > > >	m_StripPositionX			;//!
	vector < vector < vector < double > > >	m_StripPositionY			;//!
	vector < vector < vector < double > > >	m_StripPositionZ			;//!


  public: // Static constructor to be passed to the Detector Factory
    static NPL::VDetector* Construct();
 
	ClassDef(TS2Physics,1)  // S2Physics structure
	  };

namespace S2_LOCAL
{

  //	tranform an integer to a string
  string itoa(int value);
  //	DSSD
  //	X
  double fS2_Theta_E(TS2Data* Data, const int i);
  double fS2_Theta_T(TS2Data* Data, const int i);
		
  //	Y	
  double fS2_Phi_E(TS2Data* Data, const int i);
  double fS2_Phi_T(TS2Data* Data, const int i);
				
}


#endif

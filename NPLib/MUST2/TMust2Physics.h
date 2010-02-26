#ifndef TMUST2PHYSICS_H
#define TMUST2PHYSICS_H
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold must2 treated data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  Only multiplicity one and multiplicity 2 are down.                       *
 *  Improvment needed                                                        *
 *                                                                           *
 *****************************************************************************/
// STL
#include <vector>

// NPL
#include "TMust2Data.h"
#include "../include/CalibrationManager.h"
#include "../include/VDetector.h"
// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"

using namespace std ;

class TMust2Physics : public TObject, public NPA::VDetector
{
	public:
		TMust2Physics()	;
		~TMust2Physics() {};

	public: 
	void Clear()	              ;	
	void Clear(const Option_t*) {};

	public: 
	vector < TVector2 > Match_X_Y() ;
	bool Match_Si_CsI(int X, int Y , int CristalNbr);
	bool Match_Si_SiLi(int X, int Y , int PadNbr);
	bool ResolvePseudoEvent();
	int  CheckEvent();
	
	public:
	
	//	Provide Physical Multiplicity
	Int_t			EventMultiplicity	;
				
	//	Provide a Classification of Event
	vector<int>		EventType			;
		
	// Telescope
	vector<int>		TelescopeNumber		;
	
	//	Si X
	vector<double>	Si_E				;
	vector<double>	Si_T				;
	vector<int>			Si_X				;
	vector<int>			Si_Y				;
	
	//	Si(Li)
	vector<double>	SiLi_E				;
	vector<double>	SiLi_T				;
	vector<int>			SiLi_N				;
	
	//	CsI
	vector<double>	CsI_E				;
	vector<double>	CsI_T				;
	vector<int>			CsI_N				;	
	
	// Physical Value  
	vector<double>	TotalEnergy			;
	
	
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
		
		
		//	This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
		void BuildPhysicalEvent()					;
		
		
		//	Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
		//	This method aimed to be used for analysis performed during experiment, when speed is requiered.
		//	NB: This method can eventually be the same as BuildPhysicalEvent.
		void BuildSimplePhysicalEvent()				;

		//	Those two method all to clear the Event Physics or Data
		void ClearEventPhysics()		{Clear();}		
		void ClearEventData()				{EventData->Clear();}		
	
	public:		//	Specific to MUST2 Array
		//	Add a Telescope using Corner Coordinate information
		void AddTelescope(	TVector3 C_X1_Y1 		,
	 						TVector3 C_X128_Y1 		, 
	 						TVector3 C_X1_Y128 		, 
	 						TVector3 C_X128_Y128	);
		
		//	Add a Telescope using R Theta Phi of Si center information
		void AddTelescope(	double theta 	, 
							double phi 		, 
							double distance , 
							double beta_u 	, 
							double beta_v 	, 
							double beta_w	);
							
		double GetStripPositionX( int N , int X , int Y )	{ return StripPositionX[N-1][X-1][Y-1] ; };
		double GetStripPositionY( int N , int X , int Y )	{ return StripPositionY[N-1][X-1][Y-1] ; };
		double GetStripPositionZ( int N , int X , int Y )	{ return StripPositionZ[N-1][X-1][Y-1] ; };

		double GetNumberOfTelescope() 	{ return NumberOfTelescope ; }			;

		// To be called after a build Physical Event 
		int GetEventMultiplicity()	{ return EventMultiplicity; };
		
		double GetEnergyDeposit(int i) { return TotalEnergy[i] ;};
		
		TVector3 GetPositionOfInteraction(int i)	 ;	
		TVector3 GetTelescopeNormal(int i)		;

	 	private:	//	Root Input and Output tree classes
				
		TMust2Data* 	  EventData				;//!
		TMust2Physics* 	  EventPhysics		;//!


		private:	//	Spatial Position of Strip Calculated on bases of detector position
	
			int NumberOfTelescope	;//!
		
			vector< vector < vector < double > > >	StripPositionX			;//!
			vector< vector < vector < double > > >	StripPositionY			;//!
			vector< vector < vector < double > > >	StripPositionZ			;//!
			
	ClassDef(TMust2Physics,1)  // Must2Physics structure
};

namespace LOCAL
	{
		// Threshold
		const double Si_X_E_Threshold = 0	;	const double Si_X_T_Threshold = 0 ;
		const double Si_Y_E_Threshold = 0	;	const double Si_Y_T_Threshold = 0	;
		const double SiLi_E_Threshold = 0	;	const double SiLi_T_Threshold = 0	;
		const double CsI_E_Threshold	= 0 ;	const double CsI_T_Threshold	= 0	;
		
		//	tranform an integer to a string
		string itoa(int value);
		//	DSSD
		//	X
		double fSi_X_E(TMust2Data* Data , int i);
		double fSi_X_T(TMust2Data* Data, int i);
		
		//	Y	
		double fSi_Y_E(TMust2Data* Data, int i);
		double fSi_Y_T(TMust2Data* Data, int i);
			
		//	SiLi
		double fSiLi_E(TMust2Data* Data, int i);
		double fSiLi_T(TMust2Data* Data, int i);
			
		//	CsI
		double fCsI_E(TMust2Data* Data, int i);
		double fCsI_T(TMust2Data* Data, int i);
	
	}


#endif

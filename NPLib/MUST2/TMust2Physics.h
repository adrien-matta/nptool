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

// ROOT 
#include "TVector2.h" 
#include "TVector3.h" 
#include "TObject.h"

using namespace std ;

class TMust2Physics : public TObject
{
	public:
		TMust2Physics()	;
		~TMust2Physics() {};

	public: 
	void Clear()									;	
  void Clear(const Option_t*) {};
	void BuildPhysicalEvent(TMust2Data* Data)						;
	void BuildSimplePhysicalEvent(TMust2Data* Data)			;

	public: 
	vector < TVector2 > Match_X_Y(TMust2Data* Data) ;
	bool Match_Si_CsI(int X, int Y , int CristalNbr);
	bool Match_Si_SiLi(int X, int Y , int PadNbr);
	bool ResolvePseudoEvent(TMust2Data* Data);
	int  CheckEvent(TMust2Data* Data);
	
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
	
	ClassDef(TMust2Physics,1)  // Must2Data structure
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

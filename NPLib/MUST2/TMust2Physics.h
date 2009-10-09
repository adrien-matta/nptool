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
#include <vector>
#include "TObject.h"
#include "TMust2Data.h"

using namespace std ;

class TMust2Physics : public TObject
{
	public:
		TMust2Physics()	;
		~TMust2Physics();

	public: 
	void Clear()									;	
        void Clear(const Option_t*) {};
	void BuildPhysicalEvent(TMust2Data* Data)		;
	void BuildSimplePhysicalEvent(TMust2Data* Data)	;
		
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
	vector<int>		Si_X				;
	vector<int>		Si_Y				;
	
	//	Si(Li)
	vector<double>	SiLi_E				;
	vector<double>	SiLi_T				;
	vector<int>		SiLi_N				;
	
	//	CsI
	vector<double>	CsI_E				;
	vector<double>	CsI_T				;
	vector<int>		CsI_N				;	
	
	// Physical Value  
	vector<double>	TotalEnergy			;
	
	ClassDef(TMust2Physics,1)  // Must2Data structure
};

#endif

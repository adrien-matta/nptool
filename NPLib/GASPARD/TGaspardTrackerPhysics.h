#ifndef TGASPARDTRACKERPHYSICS_H
#define TGASPARDTRACKERPHYSICS_H

#include <vector>
#include "TObject.h"
#include "TGaspardTrackerData.h"

using namespace std ;

class TGaspardTrackerPhysics : public TObject
{
	public:
		TGaspardTrackerPhysics()	;
		~TGaspardTrackerPhysics();

	public: 
	void Clear()									;	
	void BuildPhysicalEvent(TGaspardTrackerData* Data)		;
	void BuildSimplePhysicalEvent(TGaspardTrackerData* Data)	;
		
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
	
	ClassDef(TGaspardTrackerPhysics,1)  // GaspardTrackerPHysics structure
};

#endif

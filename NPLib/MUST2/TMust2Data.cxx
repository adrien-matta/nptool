#include <iostream>
using namespace std;

#include "TMust2Data.h"

ClassImp(TMust2Data)

TMust2Data::TMust2Data()
{
	// Default constructor

	// DSSD
	// (X,E)
	fMM_StripXE_DetectorNbr.clear()	;
	fMM_StripXE_StripNbr.clear()	;
	fMM_StripXE_Energy.clear()		;
	// (X,T)
	fMM_StripXT_DetectorNbr.clear()	;
	fMM_StripXT_StripNbr.clear()	;
	fMM_StripXT_Time.clear()		;
	// (Y,E)
	fMM_StripYE_DetectorNbr.clear()	;	
	fMM_StripYE_StripNbr.clear()	;
	fMM_StripYE_Energy.clear()		;
	// (Y,T)
	fMM_StripYT_DetectorNbr.clear()	;
	fMM_StripYT_StripNbr.clear()	;
	fMM_StripYT_Time.clear()		;

	// SiLi E
	fMM_SiLiE_DetectorNbr.clear()	;
	fMM_SiLiE_PadNbr.clear()		;
	fMM_SiLiE_Energy.clear()		;
	// SiLi T
	fMM_SiLiT_DetectorNbr.clear()	;
	fMM_SiLiT_PadNbr.clear()		;
	fMM_SiLiT_Time.clear()			;

	// CsI E
	fMM_CsIE_DetectorNbr.clear()	;
	fMM_CsIE_CristalNbr.clear()		;
	fMM_CsIE_Energy.clear()			;
	// CsI T
	fMM_CsIT_DetectorNbr.clear()	;
	fMM_CsIT_CristalNbr.clear()		;
	fMM_CsIT_Time.clear()			;
}

TMust2Data::~TMust2Data()
{}

void TMust2Data::Clear()
{
	// DSSD
	// (X,E)
	fMM_StripXE_DetectorNbr.clear()	;
	fMM_StripXE_StripNbr.clear()	;
	fMM_StripXE_Energy.clear()		;
	// (X,T)
	fMM_StripXT_DetectorNbr.clear()	;
	fMM_StripXT_StripNbr.clear()	;
	fMM_StripXT_Time.clear()		;
	// (Y,E)
	fMM_StripYE_DetectorNbr.clear()	;
	fMM_StripYE_StripNbr.clear()	;
	fMM_StripYE_Energy.clear()		;
	// (Y,T)
	fMM_StripYT_DetectorNbr.clear()	;
	fMM_StripYT_StripNbr.clear()	;
	fMM_StripYT_Time.clear()		;

	// SiLi E
	fMM_SiLiE_DetectorNbr.clear()	;
	fMM_SiLiE_PadNbr.clear()		;
	fMM_SiLiE_Energy.clear()		;
	// SiLi T
	fMM_SiLiT_DetectorNbr.clear()	;
	fMM_SiLiT_PadNbr.clear()		;
	fMM_SiLiT_Time.clear()			;

	// CsI E
	fMM_CsIE_DetectorNbr.clear()	;
	fMM_CsIE_CristalNbr.clear()		;
	fMM_CsIE_Energy.clear()			;
	// CsI T
	fMM_CsIT_DetectorNbr.clear()	;
	fMM_CsIT_CristalNbr.clear()		;
	fMM_CsIT_Time.clear()			;
}



void TMust2Data::Dump()
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // DSSD
   // (X,E)
   cout << "MM_StripXE_Mult = " << fMM_StripXE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fMM_StripXE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fMM_StripXE_DetectorNbr[i] << " Strip: " << fMM_StripXE_StripNbr[i] << " Energy: " << fMM_StripXE_Energy[i] << endl;
   // (X,T)
   cout << "MM_StripXT_Mult = " << fMM_StripXT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fMM_StripXT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fMM_StripXT_DetectorNbr[i] << " Strip: " << fMM_StripXT_StripNbr[i] << " Time: " << fMM_StripXT_Time[i] << endl;
   // (Y,E)
   cout << "MM_StripYE_Mult = " << fMM_StripYE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fMM_StripYE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fMM_StripYE_DetectorNbr[i] << " Strip: " << fMM_StripYE_StripNbr[i] << " Energy: " << fMM_StripYE_Energy[i] << endl;
   // (Y,T)
   cout << "MM_StripYT_Mult = " << fMM_StripYT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fMM_StripYT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fMM_StripYT_DetectorNbr[i] << " Strip: " << fMM_StripYT_StripNbr[i] << " Time: " << fMM_StripYT_Time[i] << endl;

   // SiLi
   // Energy
   cout << "MM_SiliE_Mult = " << fMM_SiLiE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fMM_SiLiE_DetectorNbr.size(); i++)
      cout << "Det: " << fMM_SiLiE_DetectorNbr[i] << " Pad: " << fMM_SiLiE_PadNbr[i] << " Energy: " << fMM_SiLiE_Energy[i] << endl;
   // Time
   cout << "MM_SiliT_Mult = " << fMM_SiLiT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fMM_SiLiT_DetectorNbr.size(); i++)
      cout << "Det: " << fMM_SiLiT_DetectorNbr[i] << " Pad: " << fMM_SiLiT_PadNbr[i] << " Time: " << fMM_SiLiT_Time[i] << endl;

   // CsI
   // Energy
   cout << "MM_CsIE_Mult = " << fMM_CsIE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fMM_CsIE_DetectorNbr.size(); i++)
      cout << "Det: " << fMM_CsIE_DetectorNbr[i] << " Pad: " << fMM_CsIE_CristalNbr[i] << " Energy: " << fMM_CsIE_Energy[i] << endl;
   // Time
   cout << "MM_CsIT_Mult = " << fMM_CsIT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fMM_CsIT_DetectorNbr.size(); i++)
      cout << "Det: " << fMM_CsIT_DetectorNbr[i] << " Pad: " << fMM_CsIT_CristalNbr[i] << " Time: " << fMM_CsIT_Time[i] << endl;
}

#include <iostream>
using namespace std;

#include "TTacData.h"

ClassImp(TTacData)

TTacData::TTacData()
{
	// Default constructor
	Clear();
}

TTacData::~TTacData()
{
}

void TTacData::Clear()
{
	// (TAC)
	fTAC_MM_CATS1	 = -1;
	fTAC_MM_CATS2  = -1;
	fTAC_PL_CATS1  = -1;
	fTAC_PL_CATS2  = -1;
	fTAC_CATS12		 = -1;
	fTAC_PL_GAL		 = -1;
	fTAC_CATS1_HF	 = -1;
	fTAC_CATS2_HF	 = -1;
	fTAC_MM_HF     = -1;
}

void TTacData::Dump() const
{
	cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
	// (TAC)
	cout << "fTAC_MM_CATS1 : "  << fTAC_MM_CATS1  << endl;
	cout << "fTAC_MM_CATS2 : "  << fTAC_MM_CATS2  << endl;
	cout << "fTAC_PL_CATS1 : "  << fTAC_PL_CATS1  << endl;
	cout << "fTAC_PL_CATS2 : "  << fTAC_PL_CATS2  << endl;
	cout << "fTAC_CATS12	 : "  << fTAC_CATS12	  << endl;
	cout << "fTAC_PL_GAL	 : "  << fTAC_PL_GAL	  << endl;
	cout << "fTAC_CATS1_HF : "  << fTAC_CATS1_HF  << endl;
	cout << "fTAC_CATS2_HF : "  << fTAC_CATS2_HF  << endl;
	cout << "fTAC_MM_HF	   : "  << fTAC_MM_HF	    << endl;
}

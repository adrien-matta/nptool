/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:                  contact address:                        *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
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
   fTAC_MM1_HF     = -1;
   fTAC_MM2_HF     = -1;
   fTAC_MM3_HF     = -1;
   fTAC_MM4_HF     = -1;
   fTAC_MM5_HF     = -1;
   fTAC_MM6_HF     = -1;
   fTAC_MM7_HF     = -1;
   fTAC_MM8_HF     = -1;
   fTAC_MM_HF_DetectorNbr.clear();
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
   cout << "fTAC_MM1_HF    : "  << fTAC_MM1_HF      << endl;
   cout << "fTAC_MM2_HF    : "  << fTAC_MM2_HF      << endl;
   cout << "fTAC_MM3_HF    : "  << fTAC_MM3_HF      << endl;
   cout << "fTAC_MM4_HF    : "  << fTAC_MM4_HF      << endl;
   cout << "fTAC_MM5_HF    : "  << fTAC_MM5_HF      << endl;
   cout << "fTAC_MM6_HF    : "  << fTAC_MM6_HF      << endl;
   cout << "fTAC_MM7_HF    : "  << fTAC_MM7_HF      << endl;
   cout << "fTAC_MM8_HF    : "  << fTAC_MM8_HF      << endl;

   cout << "TAC_MM_HF_DetectorNbr = " << fTAC_MM_HF_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fTAC_MM_HF_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fTAC_MM_HF_DetectorNbr[i] << endl;
}

/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Benjamin Le Crom   contact address: lecrom@ipno.in2p3.fr *
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
   fTAC_MM_CATS1 = -1;
   fTAC_CATS12 = -1;
   fTAC_CATS1_PLD4 = -1;
   fTAC_CATS1_HF = -1;
   fTAC_CATS1_EXO = -1;
   fTAC_MM_HF = -1;
   fTAC_MM_HF_DetectorNbr.clear();
   fTAC_MM_EXO = -1;
   fTAC_BARREL_EXO = -1;
   fTAC_BARREL_CATS1FAG = -1;
   fTAC_HYB_EXO = -1;
   fTAC_HYB_CATS1FAG = -1;
   fTAC_CHAR_EXO = -1;
   fTAC_CHAR_CATS1FAG = -1;
   fTAC_CHAR_PLD4 = -1;
   fTAC_CHAR_GALOTTE = -1;
   fTAC_CATS1_CATS2 = -1;
   fTAC_CATS1_GALOTTE = -1;
   fTAC_FAG_CATS2 = -1;
}

void TTacData::Dump() const
{
	cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
	// (TAC)
	cout << "fTAC_MM_CATS1 : "  << fTAC_MM_CATS1  << endl;
	cout << "fTAC_CATS12 : "  << fTAC_CATS12  << endl;
	cout << "fTAC_CATS1_PLD4 : "  << fTAC_CATS1_PLD4  << endl;
	cout << "fTAC_CATS1_HF : "  << fTAC_CATS1_HF  << endl;
	cout << "fTAC_CATS1_EXO : "  << fTAC_CATS1_EXO  << endl;
	cout << "fTAC_MM_HF : "  << fTAC_MM_HF  << endl;
  	cout << "fTAC_MM_EXO : "  << fTAC_MM_EXO  << endl;
	cout << "fTAC_BARREL_EXO : "  << fTAC_BARREL_EXO  << endl;
	cout << "fTAC_BARREL_CATS1FAG : "  << fTAC_BARREL_CATS1FAG  << endl;
	cout << "fTAC_HYB_EXO : "  << fTAC_HYB_EXO  << endl;
	cout << "fTAC_HYB_CATS1FAG : "  << fTAC_HYB_CATS1FAG  << endl;
	cout << "fTAC_CHAR_EXO : "  << fTAC_CHAR_EXO  << endl;
	cout << "fTAC_CHAR_CATS1FAG : "  << fTAC_CHAR_CATS1FAG  << endl;
	cout << "fTAC_CHAR_PLD4 : "  << fTAC_CHAR_PLD4  << endl;
	cout << "fTAC_CHAR_GALOTTE : "  << fTAC_CHAR_GALOTTE  << endl;
        cout << "fTAC_CATS1_CATS2 : "  << fTAC_CATS1_CATS2  << endl;
	cout << "fTAC_CATS1_GALOTTE : "  << fTAC_CATS1_GALOTTE  << endl;
	cout << "fTAC_FAG_CATS2 : "  << fTAC_FAG_CATS2 << endl;


   cout << "TAC_MM_HF_DetectorNbr = " << fTAC_MM_HF_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fTAC_MM_HF_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fTAC_MM_HF_DetectorNbr[i] << endl;
}

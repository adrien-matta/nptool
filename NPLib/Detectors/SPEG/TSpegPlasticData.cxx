/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre  contact address: lefebvrl@ipno.in2p3.fr     *
 *                                                                           *
 * Creation Date  : June 2011                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds SPEG Raw data                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TSpegPlasticData.h"


ClassImp(TSpegPlasticData)

TSpegPlasticData::TSpegPlasticData()
{
   // Default constructor
   Clear();
}



TSpegPlasticData::~TSpegPlasticData()
{
}



void TSpegPlasticData::Clear()
{
   fSpeg_Plastic_Energy_Left = 0;
   fSpeg_Plastic_Energy_Right = 0;
   fSpeg_Plastic_Time_Left = 0;
   fSpeg_Plastic_Time_Right = 0;
}



void TSpegPlasticData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // Speg Plastic
   // (E)
   cout << "Speg_Plastic_Energy_Left    = " << fSpeg_Plastic_Energy_Left << endl;
   cout << "Speg_Plastic_Energy_Right   = " << fSpeg_Plastic_Energy_Right << endl;
   cout << "Speg_Plastic_Time_Left = " << fSpeg_Plastic_Time_Left << endl;
   cout << "Speg_Plastic_Time_Right = " << fSpeg_Plastic_Time_Right << endl;
}

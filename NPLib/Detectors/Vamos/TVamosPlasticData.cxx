/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : July 2009                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds VAMOS Raw data                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TVamosPlasticData.h"


ClassImp(TVamosPlasticData)

TVamosPlasticData::TVamosPlasticData()
{
   // Default constructor
   Clear();
}



TVamosPlasticData::~TVamosPlasticData()
{
}



void TVamosPlasticData::Clear()
{
   fVamos_Plastic_Energy_Left = 0;
   fVamos_Plastic_Energy_Right = 0;
   fVamos_Plastic_Time_LeftRight = 0;
}



void TVamosPlasticData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // Vamos Plastic
   // (E)
   cout << "Vamos_Plastic_Energy_Left    = " << fVamos_Plastic_Energy_Left << endl;
   cout << "Vamos_Plastic_Energy_Right   = " << fVamos_Plastic_Energy_Right << endl;
   cout << "Vamos_Plastic_Time_LeftRight = " << fVamos_Plastic_Time_LeftRight << endl;
}

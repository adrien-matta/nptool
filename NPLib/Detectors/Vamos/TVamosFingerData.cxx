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

#include "TVamosFingerData.h"


ClassImp(TVamosFingerData)

TVamosFingerData::TVamosFingerData()
{
   // Default constructor
   Clear();
}



TVamosFingerData::~TVamosFingerData()
{
}



void TVamosFingerData::Clear()
{
   fVamos_Finger_Energy = 0;
}



void TVamosFingerData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // VamosFinger
   // (E)
   cout << "Vamos_Finger_Energy = " << fVamos_Finger_Energy << endl;
}

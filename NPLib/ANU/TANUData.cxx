/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 30/01/12                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the results of the G4 simulation for the    *
 *             tracker part of the Gaspard detector.                         *
 *             The format mimics what was used for the GANIL experiments     *
 *             after conversion of the raw data with GRU. Ask                *
 *             N. de Sereville for more informations.                        *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
using namespace std;

#include "TANUData.h"

ClassImp(TANUData)

TANUData::TANUData()
{
   // Default constructor

   Clear();
}



TANUData::~TANUData()
{
}



void TANUData::Clear()
{
   // DSSD
   // (X,E)
   fANU_FirstStage_E_DetectorNbr.clear();
   fANU_FirstStage_E_StripNbr.clear()  ;
   fANU_FirstStage_E_Energy.clear()    ;
   // (X,T)
   fANU_FirstStage_T_DetectorNbr.clear()  ;
   fANU_FirstStage_T_StripNbr.clear()  ;
   fANU_FirstStage_T_Time.clear()      ;

}



void TANUData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // DSSD
   // (X,E)
   cout << "ANU_FirstStage_FrontE_Mult = " << fANU_FirstStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fANU_FirstStage_E_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fANU_FirstStage_E_DetectorNbr[i] << " Strip: " << fANU_FirstStage_E_StripNbr[i] << " Energy: " << fANU_FirstStage_E_Energy[i] << endl;
   // (X,T)
   cout << "ANU_FirstStage_FrontT_Mult = " << fANU_FirstStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fANU_FirstStage_T_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fANU_FirstStage_T_DetectorNbr[i] << " Strip: " << fANU_FirstStage_T_StripNbr[i] << " Time: " << fANU_FirstStage_T_Time[i] << endl;
 
}

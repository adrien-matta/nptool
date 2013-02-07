/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
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

#include "THeliosData.h"

ClassImp(THeliosData)

THeliosData::THeliosData()
{
   // Default constructor

   Clear();
}



THeliosData::~THeliosData()
{
}



void THeliosData::Clear()
{
   // DSSD
   // (X,E)
   fHelios_FirstStage_E_DetectorNbr.clear();
   fHelios_FirstStage_E_StripNbr.clear()  ;
   fHelios_FirstStage_E_Energy.clear()    ;
   // (X,T)
   fHelios_FirstStage_T_DetectorNbr.clear()  ;
   fHelios_FirstStage_T_StripNbr.clear()  ;
   fHelios_FirstStage_T_Time.clear()      ;

}



void THeliosData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // DSSD
   // (X,E)
   cout << "Helios_FirstStage_FrontE_Mult = " << fHelios_FirstStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHelios_FirstStage_E_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fHelios_FirstStage_E_DetectorNbr[i] << " Strip: " << fHelios_FirstStage_E_StripNbr[i] << " Energy: " << fHelios_FirstStage_E_Energy[i] << endl;
   // (X,T)
   cout << "Helios_FirstStage_FrontT_Mult = " << fHelios_FirstStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHelios_FirstStage_T_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fHelios_FirstStage_T_DetectorNbr[i] << " Strip: " << fHelios_FirstStage_T_StripNbr[i] << " Time: " << fHelios_FirstStage_T_Time[i] << endl;
 
}

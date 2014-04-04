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
 * Last update    : 04/04/14                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the results of the G4 simulation for the    *
 *             tracker part of the ANU detector.                         	 *
 *             The format mimics what was used for the GANIL experiments     *
 *             after conversion of the raw data with GRU. Ask                *
 *             N. de Sereville for more informations.                        *
 *             The same format will be used for the ANU Setup                *
 *             for developpement puposes                                     *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *         The ANU detector is an array of 6 identical SiLi detectors        *
 *         having the shape of a camembert. It has no strips                 *
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
   fANU_SiLi_E_DetectorNbr.clear();
   fANU_SiLi_E_Energy.clear()    ;
   // (X,T)
   fANU_SiLi_T_DetectorNbr.clear()  ;
   fANU_SiLi_T_Time.clear()      ;

}



void TANUData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // DSSD
   // (X,E)
   cout << "ANU_SiLi_FrontE_Mult = " << fANU_SiLi_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fANU_SiLi_E_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fANU_SiLi_E_DetectorNbr[i] << " Energy: " << fANU_SiLi_E_Energy[i] << endl;
   // (X,T)
   cout << "ANU_SiLi_FrontT_Mult = " << fANU_SiLi_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fANU_SiLi_T_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fANU_SiLi_T_DetectorNbr[i] << " Time: " << fANU_SiLi_T_Time[i] << endl;
 
}

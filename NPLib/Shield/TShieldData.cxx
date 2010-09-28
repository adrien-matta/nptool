#include <iostream>
#include "TShieldData.h"
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 04/12/2009                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of the Shield of PARIS detector        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

ClassImp(TShieldData)

TShieldData::TShieldData()
{
  // Default constructor
  Clear();
}



TShieldData::~TShieldData()
{
}



void TShieldData::Clear()
{

  /*
    fShield_Energy.clear();
    fShield_Number.clear();
    fShield_Time.clear();
  */


   fPARIS_CsIShield_E_DetectorNbr.clear();
   fPARIS_CsIShield_E_CrystalNbr.clear();
   fPARIS_CsIShield_E_Energy.clear();
   fPARIS_CsIShield_Eff_phpeak.clear();
   // Time
   fPARIS_CsIShield_T_DetectorNbr.clear();
   fPARIS_CsIShield_T_CrystalNbr.clear();
   fPARIS_CsIShield_T_Time.clear();


}



void TShieldData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
   /*
   for (unsigned short i = 0; i<fShield_Energy.size(); i ++) {
      cout << "Shield Number " << fShield_Number[i] << " Energy: " << fShield_Energy[i]  << " Time: "<< fShield Time[i] << endl;
   }
   */

   for (UShort_t i = 0; i < fPARIS_CsIShield_E_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fPARIS_CsIShield_E_DetectorNbr[i] << " Crystal: " << fPARIS_CsIShield_E_CrystalNbr[i] << " Energy: " << fPARIS_CsIShield_E_Energy[i] << endl;
   // (X,T)
   cout << "PARIS_CsIShield_T_Mult = " << fPARIS_CsIShield_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fPARIS_CsIShield_T_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fPARIS_CsIShield_T_DetectorNbr[i] << " Crystal: " << fPARIS_CsIShield_T_CrystalNbr[i] << " Time: " << fPARIS_CsIShield_T_Time[i] << endl;




}

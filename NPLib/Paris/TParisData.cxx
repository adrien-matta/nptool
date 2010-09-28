#include <iostream>
#include "TParisData.h"
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
 *  This class described the raw data of the Paris detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

ClassImp(TParisData)

TParisData::TParisData()
{
  // Default constructor
  Clear();
}



TParisData::~TParisData()
{
}



void TParisData::Clear()
{

  /*
    fParis_Energy.clear();
    fParis_Number.clear();
    fParis_Time.clear();
  */


   fPARIS_LaBr3Stage_E_DetectorNbr.clear();
   fPARIS_LaBr3Stage_E_CrystalNbr.clear();
   fPARIS_LaBr3Stage_E_Energy.clear();
   fPARIS_LaBr3Stage_Eff_phpeak.clear();
   // Time
   fPARIS_LaBr3Stage_T_DetectorNbr.clear();
   fPARIS_LaBr3Stage_T_CrystalNbr.clear();
   fPARIS_LaBr3Stage_T_Time.clear();


   // Second Stage CsI
   // CsI
   // Energy
   fPARIS_CsIStage_E_DetectorNbr.clear();
   fPARIS_CsIStage_E_CrystalNbr.clear();
   fPARIS_CsIStage_E_Energy.clear();
   fPARIS_CsIStage_Eff_phpeak.clear();
   // Time
   fPARIS_CsIStage_T_DetectorNbr.clear();
   fPARIS_CsIStage_T_CrystalNbr.clear();
   fPARIS_CsIStage_T_Time.clear();

}



void TParisData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
   /*
   for (unsigned short i = 0; i<fParis_Energy.size(); i ++) {
      cout << "Paris Number " << fParis_Number[i] << " Energy: " << fParis_Energy[i]  << " Time: "<< fParis_Time[i] << endl;
   }
   */

   for (UShort_t i = 0; i < fPARIS_LaBr3Stage_E_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fPARIS_LaBr3Stage_E_DetectorNbr[i] << " Crystal: " << fPARIS_LaBr3Stage_E_CrystalNbr[i] << " Energy: " << fPARIS_LaBr3Stage_E_Energy[i] << endl;
   // (X,T)
   cout << "PARIS_LaBr3Stage_T_Mult = " << fPARIS_LaBr3Stage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fPARIS_LaBr3Stage_T_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fPARIS_LaBr3Stage_T_DetectorNbr[i] << " Crystal: " << fPARIS_LaBr3Stage_T_CrystalNbr[i] << " Time: " << fPARIS_LaBr3Stage_T_Time[i] << endl;

   // Second Stage
   // Energy
   cout << "PARIS_CsIStage_E_Mult = " << fPARIS_CsIStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fPARIS_CsIStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fPARIS_CsIStage_E_DetectorNbr[i] << " Crystal: " << fPARIS_CsIStage_E_CrystalNbr[i] << " Energy: " << fPARIS_CsIStage_E_Energy[i] << endl;
   // Time
   cout << "PARIS_CsIStage_T_Mult = " << fPARIS_CsIStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fPARIS_CsIStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fPARIS_CsIStage_T_DetectorNbr[i] << " Crystal: " << fPARIS_CsIStage_T_CrystalNbr[i] << " Time: " << fPARIS_CsIStage_T_Time[i] << endl;



}

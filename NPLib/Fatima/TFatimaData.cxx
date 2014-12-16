#include <iostream>
#include "TFatimaData.h"
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 04/01/2013                                               *
 * Last update    : 02/07/2014                                                         *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of the Fatima detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

ClassImp(TFatimaData)

TFatimaData::TFatimaData()
{
  // Default constructor
  Clear();
}



TFatimaData::~TFatimaData()
{
}



void TFatimaData::Clear()
{

  /*
    fFatima_Energy.clear();
    fFatima_Number.clear();
    fFatima_Time.clear();
  */


   fFATIMA_LaBr3Stage_E_DetectorNbr.clear();
   fFATIMA_LaBr3Stage_E_CrystalNbr.clear();
   fFATIMA_LaBr3Stage_E_Energy.clear();
   //   fFATIMA_LaBr3Stage_Eff_phpeak.clear();
   // Time
   fFATIMA_LaBr3Stage_T_DetectorNbr.clear();
   fFATIMA_LaBr3Stage_T_CrystalNbr.clear();
   fFATIMA_LaBr3Stage_T_Time.clear();


   /* 
  // Second Stage CsI
   // CsI
   // Energy
   fFATIMA_CsIStage_E_DetectorNbr.clear();
   fFATIMA_CsIStage_E_CrystalNbr.clear();
   fFATIMA_CsIStage_E_Energy.clear();
   //   fFATIMA_CsIStage_Eff_phpeak.clear();
   // Time
   fFATIMA_CsIStage_T_DetectorNbr.clear();
   fFATIMA_CsIStage_T_CrystalNbr.clear();
   fFATIMA_CsIStage_T_Time.clear();
   */
}



void TFatimaData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
   /*
   for (unsigned short i = 0; i<fFatima_Energy.size(); i ++) {
      cout << "Fatima Number " << fFatima_Number[i] << " Energy: " << fFatima_Energy[i]  << " Time: "<< fFatima_Time[i] << endl;
   }
   */

   for (UShort_t i = 0; i < fFATIMA_LaBr3Stage_E_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fFATIMA_LaBr3Stage_E_DetectorNbr[i] << " Crystal: " << fFATIMA_LaBr3Stage_E_CrystalNbr[i] << " Energy: " << fFATIMA_LaBr3Stage_E_Energy[i] << endl;
   // (X,T)
   cout << "FATIMA_LaBr3Stage_T_Mult = " << fFATIMA_LaBr3Stage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fFATIMA_LaBr3Stage_T_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fFATIMA_LaBr3Stage_T_DetectorNbr[i] << " Crystal: " << fFATIMA_LaBr3Stage_T_CrystalNbr[i] << " Time: " << fFATIMA_LaBr3Stage_T_Time[i] << endl;

   /*
   // Second Stage
   // Energy
   cout << "FATIMA_CsIStage_E_Mult = " << fFATIMA_CsIStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fFATIMA_CsIStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fFATIMA_CsIStage_E_DetectorNbr[i] << " Crystal: " << fFATIMA_CsIStage_E_CrystalNbr[i] << " Energy: " << fFATIMA_CsIStage_E_Energy[i] << endl;
   // Time
   cout << "FATIMA_CsIStage_T_Mult = " << fFATIMA_CsIStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fFATIMA_CsIStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fFATIMA_CsIStage_T_DetectorNbr[i] << " Crystal: " << fFATIMA_CsIStage_T_CrystalNbr[i] << " Time: " << fFATIMA_CsIStage_T_Time[i] << endl;

   */

}

#include <iostream>
#include "TNanaData.h"
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
 *  This class described the raw data of the Nana detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

ClassImp(TNanaData)

TNanaData::TNanaData()
{
  // Default constructor
  Clear();
}



TNanaData::~TNanaData()
{
}



void TNanaData::Clear()
{

  /*
    fNana_Energy.clear();
    fNana_Number.clear();
    fNana_Time.clear();
  */


   fNANA_LaBr3Stage_E_DetectorNbr.clear();
   fNANA_LaBr3Stage_E_CrystalNbr.clear();
   fNANA_LaBr3Stage_E_Energy.clear();
   //   fNANA_LaBr3Stage_Eff_phpeak.clear();
   // Time
   fNANA_LaBr3Stage_T_DetectorNbr.clear();
   fNANA_LaBr3Stage_T_CrystalNbr.clear();
   fNANA_LaBr3Stage_T_Time.clear();


   /* 
  // Second Stage CsI
   // CsI
   // Energy
   fNANA_CsIStage_E_DetectorNbr.clear();
   fNANA_CsIStage_E_CrystalNbr.clear();
   fNANA_CsIStage_E_Energy.clear();
   //   fNANA_CsIStage_Eff_phpeak.clear();
   // Time
   fNANA_CsIStage_T_DetectorNbr.clear();
   fNANA_CsIStage_T_CrystalNbr.clear();
   fNANA_CsIStage_T_Time.clear();
   */
}



void TNanaData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
   /*
   for (unsigned short i = 0; i<fNana_Energy.size(); i ++) {
      cout << "Nana Number " << fNana_Number[i] << " Energy: " << fNana_Energy[i]  << " Time: "<< fNana_Time[i] << endl;
   }
   */

   for (UShort_t i = 0; i < fNANA_LaBr3Stage_E_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fNANA_LaBr3Stage_E_DetectorNbr[i] << " Crystal: " << fNANA_LaBr3Stage_E_CrystalNbr[i] << " Energy: " << fNANA_LaBr3Stage_E_Energy[i] << endl;
   // (X,T)
   cout << "NANA_LaBr3Stage_T_Mult = " << fNANA_LaBr3Stage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fNANA_LaBr3Stage_T_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fNANA_LaBr3Stage_T_DetectorNbr[i] << " Crystal: " << fNANA_LaBr3Stage_T_CrystalNbr[i] << " Time: " << fNANA_LaBr3Stage_T_Time[i] << endl;

   /*
   // Second Stage
   // Energy
   cout << "NANA_CsIStage_E_Mult = " << fNANA_CsIStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fNANA_CsIStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fNANA_CsIStage_E_DetectorNbr[i] << " Crystal: " << fNANA_CsIStage_E_CrystalNbr[i] << " Energy: " << fNANA_CsIStage_E_Energy[i] << endl;
   // Time
   cout << "NANA_CsIStage_T_Mult = " << fNANA_CsIStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fNANA_CsIStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fNANA_CsIStage_T_DetectorNbr[i] << " Crystal: " << fNANA_CsIStage_T_CrystalNbr[i] << " Time: " << fNANA_CsIStage_T_Time[i] << endl;

   */

}

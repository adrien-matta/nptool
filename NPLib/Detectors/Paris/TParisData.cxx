/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
#include <iostream>
#include "TParisData.h"
ClassImp(TParisData)

////////////////////////////////////////////////////////////////////////////////
TParisData::TParisData(){
}

TParisData::~TParisData(){
}

////////////////////////////////////////////////////////////////////////////////
void TParisData::Clear(){
   fPARIS_LaBr3Stage_E_ClusterNbr.clear();
   fPARIS_LaBr3Stage_E_PhoswichNbr.clear();
   fPARIS_LaBr3Stage_E_Energy.clear();
   // Time
   fPARIS_LaBr3Stage_T_ClusterNbr.clear();
   fPARIS_LaBr3Stage_T_PhoswichNbr.clear();
   fPARIS_LaBr3Stage_T_Time.clear();

   // Second Stage NaI
   // NaI
   // Energy
   fPARIS_NaIStage_E_ClusterNbr.clear();
   fPARIS_NaIStage_E_PhoswichNbr.clear();
   fPARIS_NaIStage_E_Energy.clear();
   // Time
   fPARIS_NaIStage_T_ClusterNbr.clear();
   fPARIS_NaIStage_T_PhoswichNbr.clear();
   fPARIS_NaIStage_T_Time.clear();
}

////////////////////////////////////////////////////////////////////////////////
void TParisData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
   /*
   for (unsigned short i = 0; i<fParis_Energy.size(); i ++) {
      cout << "Paris Number " << fParis_Number[i] << " Energy: " << fParis_Energy[i]  << " Time: "<< fParis_Time[i] << endl;
   }
   */

   for (UShort_t i = 0; i < fPARIS_LaBr3Stage_E_ClusterNbr.size(); i++)
      cout << "DetNbr: " << fPARIS_LaBr3Stage_E_ClusterNbr[i] << " Phoswich: " << fPARIS_LaBr3Stage_E_PhoswichNbr[i] << " Energy: " << fPARIS_LaBr3Stage_E_Energy[i] << endl;
   // (X,T)
   cout << "PARIS_LaBr3Stage_T_Mult = " << fPARIS_LaBr3Stage_T_ClusterNbr.size() << endl;
   for (UShort_t i = 0; i < fPARIS_LaBr3Stage_T_ClusterNbr.size(); i++)
      cout << "DetNbr: " << fPARIS_LaBr3Stage_T_ClusterNbr[i] << " Phoswich: " << fPARIS_LaBr3Stage_T_PhoswichNbr[i] << " Time: " << fPARIS_LaBr3Stage_T_Time[i] << endl;

   // Second Stage
   // Energy
   cout << "PARIS_NaIStage_E_Mult = " << fPARIS_NaIStage_E_ClusterNbr.size() << endl;
   for (UShort_t i = 0; i < fPARIS_NaIStage_E_ClusterNbr.size(); i++)
      cout << "Det: " << fPARIS_NaIStage_E_ClusterNbr[i] << " Phoswich: " << fPARIS_NaIStage_E_PhoswichNbr[i] << " Energy: " << fPARIS_NaIStage_E_Energy[i] << endl;
   // Time
   cout << "PARIS_NaIStage_T_Mult = " << fPARIS_NaIStage_T_ClusterNbr.size() << endl;
   for (UShort_t i = 0; i < fPARIS_NaIStage_T_ClusterNbr.size(); i++)
      cout << "Det: " << fPARIS_NaIStage_T_ClusterNbr[i] << " Phoswich: " << fPARIS_NaIStage_T_PhoswichNbr[i] << " Time: " << fPARIS_NaIStage_T_Time[i] << endl;
}


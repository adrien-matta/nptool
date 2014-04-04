#ifndef __ANUDATA__
#define __ANUDATA__
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

#include <vector>
#include "TObject.h"

using namespace std ;

class TANUData : public TObject
{
protected:
   // First Stage
   // Energy
   vector<UShort_t>  fANU_FirstStage_E_DetectorNbr;
   vector<UShort_t>  fANU_FirstStage_E_StripNbr;
   vector<Double_t>  fANU_FirstStage_E_Energy;
   // Time
   vector<UShort_t>  fANU_FirstStage_T_DetectorNbr;
   vector<UShort_t>  fANU_FirstStage_T_StripNbr;
   vector<Double_t>  fANU_FirstStage_T_Time;



public:
   TANUData();
   virtual ~TANUData();

   void  Clear();
   void  Clear(const Option_t*) {};
   void  Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // RSD
   // (E)
   void  SetANUFirstStageEDetectorNbr(UShort_t DetNbr) {
      fANU_FirstStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetANUFirstStageEStripNbr(UShort_t StripNbr)  {
      fANU_FirstStage_E_StripNbr.push_back(StripNbr);
   }
   void  SetANUFirstStageEEnergy(Double_t Energy)      {
      fANU_FirstStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetANUFirstStageTDetectorNbr(UShort_t DetNbr) {
      fANU_FirstStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetANUFirstStageTStripNbr(UShort_t StripNbr)  {
      fANU_FirstStage_T_StripNbr.push_back(StripNbr);
   }
   void  SetANUFirstStageTTime(Double_t Time)          {
      fANU_FirstStage_T_Time.push_back(Time);
   }


   /////////////////////           GETTERS           ////////////////////////
   // RSD
   // (E)
   UShort_t GetANUFirstStageEMult()               {
      return fANU_FirstStage_E_DetectorNbr.size();
   }
   UShort_t GetANUFirstStageEDetectorNbr(Int_t i) {
      return fANU_FirstStage_E_DetectorNbr.at(i);
   }
   UShort_t GetANUFirstStageEStripNbr(Int_t i)    {
      return fANU_FirstStage_E_StripNbr.at(i);
   }
   Double_t GetANUFirstStageEEnergy(Int_t i)      {
      return fANU_FirstStage_E_Energy.at(i);
   }
   // (T)
   UShort_t GetANUFirstStageTMult()               {
      return fANU_FirstStage_T_DetectorNbr.size();
   }
   UShort_t GetANUFirstStageTDetectorNbr(Int_t i) {
      return fANU_FirstStage_T_DetectorNbr.at(i);
   }
   UShort_t GetANUFirstStageTStripNbr(Int_t i)    {
      return fANU_FirstStage_T_StripNbr.at(i);
   }
   Double_t GetANUFirstStageTTime(Int_t i)      {
      return fANU_FirstStage_T_Time.at(i);
   }

 

   ClassDef(TANUData, 1) // ANUData structure
};

#endif

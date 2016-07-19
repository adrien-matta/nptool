#ifndef __HELIOSDATA__
#define __HELIOSDATA__
/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 30/01/12                                                 *
 * Last update    : 31/08/15                                                 *
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

class THeliosData : public TObject
{
protected:
   // First Stage
/* for helisol
   // Energy
   vector<UShort_t>  fHelios_FirstStage_E_DetectorNbr;
   //vector<UShort_t>  fHelios_FirstStage_E_StripNbr;
   vector<UShort_t>  fHelios_FirstStage_E_StripLNbr;
   vector<UShort_t>  fHelios_FirstStage_E_StripTNbr;
   vector<Double_t>  fHelios_FirstStage_E_Energy;
   // Time
   vector<UShort_t>  fHelios_FirstStage_T_DetectorNbr;
   //vector<UShort_t>  fHelios_FirstStage_T_StripNbr;
   vector<UShort_t>  fHelios_FirstStage_T_StripLNbr;
   vector<UShort_t>  fHelios_FirstStage_T_StripTNbr;
   vector<Double_t>  fHelios_FirstStage_T_Time;
*/
   
   
/* for helios */   
   // Energy
   vector<UShort_t>  fHelios_FirstStage_E_DetectorNbr;
   vector<UShort_t>  fHelios_FirstStage_E_StripNbr;
   vector<Double_t>  fHelios_FirstStage_E_Energy;
   // Time
   vector<UShort_t>  fHelios_FirstStage_T_DetectorNbr;
   vector<UShort_t>  fHelios_FirstStage_T_StripNbr;
   vector<Double_t>  fHelios_FirstStage_T_Time;
   


public:
   THeliosData();
   virtual ~THeliosData();

   void  Clear();
   void  Clear(const Option_t*) {};
   void  Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // RSD

/* for helisol

   // (E)
   void  SetHeliosFirstStageEDetectorNbr(UShort_t DetNbr) {
      fHelios_FirstStage_E_DetectorNbr.push_back(DetNbr);
   }
   //void  SetHeliosFirstStageEStripNbr(UShort_t StripNbr)  {
   //   fHelios_FirstStage_E_StripNbr.push_back(StripNbr);
   //}
   void  SetHeliosFirstStageEStripLNbr(Double_t PosInStrip)  {
      fHelios_FirstStage_E_StripLNbr.push_back(PosInStrip);
   }
   void  SetHeliosFirstStageEStripTNbr(Double_t PosInStrip)  {
      fHelios_FirstStage_E_StripTNbr.push_back(PosInStrip);
   }
   void  SetHeliosFirstStageEEnergy(Double_t Energy)      {
      fHelios_FirstStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetHeliosFirstStageTDetectorNbr(UShort_t DetNbr) {
      fHelios_FirstStage_T_DetectorNbr.push_back(DetNbr);
   }
   //void  SetHeliosFirstStageTStripNbr(UShort_t StripNbr)  {
   //   fHelios_FirstStage_T_StripNbr.push_back(StripNbr);
   //}
   void  SetHeliosFirstStageTStripLNbr(Double_t PosInStrip)  {
      fHelios_FirstStage_T_StripLNbr.push_back(PosInStrip);
   }
   void  SetHeliosFirstStageTStripTNbr(Double_t PosInStrip)  {
      fHelios_FirstStage_T_StripTNbr.push_back(PosInStrip);
   }
   void  SetHeliosFirstStageTTime(Double_t Time)          {
      fHelios_FirstStage_T_Time.push_back(Time);
   }
*/

   /* for helios */
   void  SetHeliosFirstStageEDetectorNbr(UShort_t DetNbr) {
      fHelios_FirstStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetHeliosFirstStageEStripNbr(UShort_t StripNbr)  {
      fHelios_FirstStage_E_StripNbr.push_back(StripNbr);
   }
   void  SetHeliosFirstStageEEnergy(Double_t Energy)      {
      fHelios_FirstStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetHeliosFirstStageTDetectorNbr(UShort_t DetNbr) {
      fHelios_FirstStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetHeliosFirstStageTStripNbr(UShort_t StripNbr)  {
      fHelios_FirstStage_T_StripNbr.push_back(StripNbr);
   }
   void  SetHeliosFirstStageTTime(Double_t Time)          {
      fHelios_FirstStage_T_Time.push_back(Time);
   }
   

   /////////////////////           GETTERS           ////////////////////////
   // RSD
   
   /* for helisol 
   // (E)
   int GetHeliosFirstStageEMult()               {
      return fHelios_FirstStage_E_DetectorNbr.size();
   }
   UShort_t GetHeliosFirstStageEDetectorNbr(Int_t i) {
      return fHelios_FirstStage_E_DetectorNbr.at(i);
   }
   //UShort_t GetHeliosFirstStageEStripNbr(Int_t i)    {
   //   return fHelios_FirstStage_E_StripNbr.at(i);
   //}
   UShort_t GetHeliosFirstStageEStripLNbr(Int_t i)    {
      return fHelios_FirstStage_E_StripLNbr.at(i);
   }
   UShort_t GetHeliosFirstStageEStripTNbr(Int_t i)    {
      return fHelios_FirstStage_E_StripTNbr.at(i);
   }
     Double_t GetHeliosFirstStageEEnergy(Int_t i)      {
      return fHelios_FirstStage_E_Energy.at(i);
   }
   // (T)
   UShort_t GetHeliosFirstStageTMult()               {
      return fHelios_FirstStage_T_DetectorNbr.size();
   }
   UShort_t GetHeliosFirstStageTDetectorNbr(Int_t i) {
      return fHelios_FirstStage_T_DetectorNbr.at(i);
   }
   //UShort_t GetHeliosFirstStageTStripNbr(Int_t i)    {
   //   return fHelios_FirstStage_T_StripNbr.at(i);
   //}
   UShort_t GetHeliosFirstStageTStripLNbr(Int_t i)    {
      return fHelios_FirstStage_T_StripLNbr.at(i);
   }
   UShort_t GetHeliosFirstStageTStripTNbr(Int_t i)    {
      return fHelios_FirstStage_T_StripTNbr.at(i);
   }
   Double_t GetHeliosFirstStageTTime(Int_t i)      {
      return fHelios_FirstStage_T_Time.at(i);
   }
*/

   /* for helios */
   UShort_t GetHeliosFirstStageEMult()               {
      return fHelios_FirstStage_E_DetectorNbr.size();
   }
   UShort_t GetHeliosFirstStageEDetectorNbr(Int_t i) {
      return fHelios_FirstStage_E_DetectorNbr.at(i);
   }
   UShort_t GetHeliosFirstStageEStripNbr(Int_t i)    {
      return fHelios_FirstStage_E_StripNbr.at(i);
   }
   Double_t GetHeliosFirstStageEEnergy(Int_t i)      {
      return fHelios_FirstStage_E_Energy.at(i);
   }
   // (T)
   UShort_t GetHeliosFirstStageTMult()               {
      return fHelios_FirstStage_T_DetectorNbr.size();
   }
   UShort_t GetHeliosFirstStageTDetectorNbr(Int_t i) {
      return fHelios_FirstStage_T_DetectorNbr.at(i);
   }
   UShort_t GetHeliosFirstStageTStripNbr(Int_t i)    {
      return fHelios_FirstStage_T_StripNbr.at(i);
   }
   Double_t GetHeliosFirstStageTTime(Int_t i)      {
      return fHelios_FirstStage_T_Time.at(i);
   }
  
 

   ClassDef(THeliosData, 1) // HeliosData structure
};

#endif

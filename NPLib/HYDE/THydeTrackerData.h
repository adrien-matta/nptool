/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 10/06/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the results of the G4 simulation for the    *
 *             tracker part of the Hyde detector.                         *
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

#ifndef __HYDETRACKERDATA__
#define __HYDETRACKERDATA__

#include <vector>
#include "TObject.h"

using namespace std ;

class THydeTrackerData : public TObject
{
protected:
   // First Stage
   // Double Sided Silicon Detector
   // Front
   // Energy
   vector<UShort_t>  fHYDTrk_FirstStage_FrontE_DetectorNbr;
   vector<UShort_t>  fHYDTrk_FirstStage_FrontE_StripNbr;
   vector<Double_t>  fHYDTrk_FirstStage_FrontE_Energy;
   // Time
   vector<UShort_t>  fHYDTrk_FirstStage_FrontT_DetectorNbr;
   vector<UShort_t>  fHYDTrk_FirstStage_FrontT_StripNbr;
   vector<Double_t>  fHYDTrk_FirstStage_FrontT_Time;
   // Back
   // Energy
   vector<UShort_t>  fHYDTrk_FirstStage_BackE_DetectorNbr;
   vector<UShort_t>  fHYDTrk_FirstStage_BackE_StripNbr;
   vector<Double_t>  fHYDTrk_FirstStage_BackE_Energy;
   // Time
   vector<UShort_t>  fHYDTrk_FirstStage_BackT_DetectorNbr;
   vector<UShort_t>  fHYDTrk_FirstStage_BackT_StripNbr;
   vector<Double_t>  fHYDTrk_FirstStage_BackT_Time;

   // Second Stage
   // Energy
   vector<UShort_t>  fHYDTrk_SecondStage_E_DetectorNbr;
   vector<UShort_t>  fHYDTrk_SecondStage_E_PadNbr;
   vector<Double_t>  fHYDTrk_SecondStage_E_Energy;
   // Time
   vector<UShort_t>  fHYDTrk_SecondStage_T_DetectorNbr;
   vector<UShort_t>  fHYDTrk_SecondStage_T_PadNbr;
   vector<Double_t>  fHYDTrk_SecondStage_T_Time;

   // Third Stage
   // Energy
   vector<UShort_t>  fHYDTrk_ThirdStage_E_DetectorNbr;
   vector<UShort_t>  fHYDTrk_ThirdStage_E_PadNbr;
   vector<Double_t>  fHYDTrk_ThirdStage_E_Energy;
   // Time
   vector<UShort_t>  fHYDTrk_ThirdStage_T_DetectorNbr;
   vector<UShort_t>  fHYDTrk_ThirdStage_T_PadNbr;
   vector<Double_t>  fHYDTrk_ThirdStage_T_Time;


public:
   THydeTrackerData();
   virtual ~THydeTrackerData();

   void  Clear();
   void  Clear(const Option_t*) {};
   void  Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // DSSD
   // (Front, E)
   void  SetHYDTrkFirstStageFrontEDetectorNbr(UShort_t DetNbr) {
      fHYDTrk_FirstStage_FrontE_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYDTrkFirstStageFrontEStripNbr(UShort_t StripNbr)  {
      fHYDTrk_FirstStage_FrontE_StripNbr.push_back(StripNbr);
   }
   void  SetHYDTrkFirstStageFrontEEnergy(Double_t Energy)      {
      fHYDTrk_FirstStage_FrontE_Energy.push_back(Energy);
   }
   // (Front, T)
   void  SetHYDTrkFirstStageFrontTDetectorNbr(UShort_t DetNbr) {
      fHYDTrk_FirstStage_FrontT_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYDTrkFirstStageFrontTStripNbr(UShort_t StripNbr)  {
      fHYDTrk_FirstStage_FrontT_StripNbr.push_back(StripNbr);
   }
   void  SetHYDTrkFirstStageFrontTTime(Double_t Time)          {
      fHYDTrk_FirstStage_FrontT_Time.push_back(Time);
   }

   // (Back, E)
   void  SetHYDTrkFirstStageBackEDetectorNbr(UShort_t DetNbr) {
      fHYDTrk_FirstStage_BackE_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYDTrkFirstStageBackEStripNbr(UShort_t StripNbr)  {
      fHYDTrk_FirstStage_BackE_StripNbr.push_back(StripNbr);
   }
   void  SetHYDTrkFirstStageBackEEnergy(Double_t Energy)      {
      fHYDTrk_FirstStage_BackE_Energy.push_back(Energy);
   }
   // (Back, T)
   void  SetHYDTrkFirstStageBackTDetectorNbr(UShort_t DetNbr) {
      fHYDTrk_FirstStage_BackT_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYDTrkFirstStageBackTStripNbr(UShort_t StripNbr)  {
      fHYDTrk_FirstStage_BackT_StripNbr.push_back(StripNbr);
   }
   void  SetHYDTrkFirstStageBackTTime(Double_t Time)          {
      fHYDTrk_FirstStage_BackT_Time.push_back(Time);
   }

   // Second Stage
   // (E)
   void  SetHYDTrkSecondStageEDetectorNbr(UShort_t DetNbr)   {
      fHYDTrk_SecondStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYDTrkSecondStageEPadNbr(UShort_t PadNbr)        {
      fHYDTrk_SecondStage_E_PadNbr.push_back(PadNbr);
   }
   void  SetHYDTrkSecondStageEEnergy(Double_t Energy)        {
      fHYDTrk_SecondStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetHYDTrkSecondStageTDetectorNbr(UShort_t DetNbr)   {
      fHYDTrk_SecondStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYDTrkSecondStageTPadNbr(UShort_t PadNbr)        {
      fHYDTrk_SecondStage_T_PadNbr.push_back(PadNbr);
   }
   void  SetHYDTrkSecondStageTTime(Double_t Time)            {
      fHYDTrk_SecondStage_T_Time.push_back(Time);
   }

   // Third Stage
   // (E)
   void  SetHYDTrkThirdStageEDetectorNbr(UShort_t DetNbr)   {
      fHYDTrk_ThirdStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYDTrkThirdStageEPadNbr(UShort_t PadNbr)        {
      fHYDTrk_ThirdStage_E_PadNbr.push_back(PadNbr);
   }
   void  SetHYDTrkThirdStageEEnergy(Double_t Energy)        {
      fHYDTrk_ThirdStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetHYDTrkThirdStageTDetectorNbr(UShort_t DetNbr)   {
      fHYDTrk_ThirdStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYDTrkThirdStageTPadNbr(UShort_t PadNbr)        {
      fHYDTrk_ThirdStage_T_PadNbr.push_back(PadNbr);
   }
   void  SetHYDTrkThirdStageTTime(Double_t Time)            {
      fHYDTrk_ThirdStage_T_Time.push_back(Time);
   }


   /////////////////////           GETTERS           ////////////////////////
   // DSSD
   // (Front, E)
   UShort_t GetHYDTrkFirstStageFrontEMult()               {
      return fHYDTrk_FirstStage_FrontE_DetectorNbr.size();
   }
   UShort_t GetHYDTrkFirstStageFrontEDetectorNbr(Int_t i) {
      return fHYDTrk_FirstStage_FrontE_DetectorNbr.at(i);
   }
   UShort_t GetHYDTrkFirstStageFrontEStripNbr(Int_t i)    {
      return fHYDTrk_FirstStage_FrontE_StripNbr.at(i);
   }
   Double_t GetHYDTrkFirstStageFrontEEnergy(Int_t i)      {
      return fHYDTrk_FirstStage_FrontE_Energy.at(i);
   }
   // (Front, T)
   UShort_t GetHYDTrkFirstStageFrontTMult()               {
      return fHYDTrk_FirstStage_FrontT_DetectorNbr.size();
   }
   UShort_t GetHYDTrkFirstStageFrontTDetectorNbr(Int_t i) {
      return fHYDTrk_FirstStage_FrontT_DetectorNbr.at(i);
   }
   UShort_t GetHYDTrkFirstStageFrontTStripNbr(Int_t i)    {
      return fHYDTrk_FirstStage_FrontT_StripNbr.at(i);
   }
   Double_t GetHYDTrkFirstStageFrontTTnergy(Int_t i)      {
      return fHYDTrk_FirstStage_FrontT_Time.at(i);
   }
   // (Back, E)
   UShort_t GetHYDTrkFirstStageBackEMult()               {
      return fHYDTrk_FirstStage_BackE_DetectorNbr.size();
   }
   UShort_t GetHYDTrkFirstStageBackEDetectorNbr(Int_t i) {
      return fHYDTrk_FirstStage_BackE_DetectorNbr.at(i);
   }
   UShort_t GetHYDTrkFirstStageBackEStripNbr(Int_t i)    {
      return fHYDTrk_FirstStage_BackE_StripNbr.at(i);
   }
   Double_t GetHYDTrkFirstStageBackEEnergy(Int_t i)      {
      return fHYDTrk_FirstStage_BackE_Energy.at(i);
   }
   // (Back, T)
   UShort_t GetHYDTrkFirstStageBackTMult()               {
      return fHYDTrk_FirstStage_BackT_DetectorNbr.size();
   }
   UShort_t GetHYDTrkFirstStageBackTDetectorNbr(Int_t i) {
      return fHYDTrk_FirstStage_BackT_DetectorNbr.at(i);
   }
   UShort_t GetHYDTrkFirstStageBackTStripNbr(Int_t i)    {
      return fHYDTrk_FirstStage_BackT_StripNbr.at(i);
   }
   Double_t GetHYDTrkFirstStageBackTTnergy(Int_t i)      {
      return fHYDTrk_FirstStage_BackT_Time.at(i);
   }

   // Second Stage
   // (E)
   UShort_t GetHYDTrkSecondStageEMult()           {
      return fHYDTrk_SecondStage_E_DetectorNbr.size();
   }
   UShort_t GetHYDTrkSecondStageEDetectorNbr(Int_t i)  {
      return fHYDTrk_SecondStage_E_DetectorNbr.at(i);
   }
   UShort_t GetHYDTrkSecondStageEPadNbr(Int_t i)       {
      return fHYDTrk_SecondStage_E_PadNbr.at(i);
   }
   Double_t GetHYDTrkSecondStageEEnergy(Int_t i)       {
      return fHYDTrk_SecondStage_E_Energy.at(i);
   }
   // (T)
   UShort_t GetHYDTrkSecondStageTMult()           {
      return fHYDTrk_SecondStage_T_DetectorNbr.size();
   }
   UShort_t GetHYDTrkSecondStageTDetectorNbr(Int_t i)  {
      return fHYDTrk_SecondStage_T_DetectorNbr.at(i);
   }
   UShort_t GetHYDTrkSecondStageTPadNbr(Int_t i)       {
      return fHYDTrk_SecondStage_T_PadNbr.at(i);
   }
   Double_t GetHYDTrkSecondStageTTime(Int_t i)         {
      return fHYDTrk_SecondStage_T_Time.at(i);
   }

   // Third Stage
   // (E)
   UShort_t GetHYDTrkThirdStageEMult()           {
      return fHYDTrk_ThirdStage_E_DetectorNbr.size();
   }
   UShort_t GetHYDTrkThirdStageEDetectorNbr(Int_t i)  {
      return fHYDTrk_ThirdStage_E_DetectorNbr.at(i);
   }
   UShort_t GetHYDTrkThirdStageEPadNbr(Int_t i)       {
      return fHYDTrk_ThirdStage_E_PadNbr.at(i);
   }
   Double_t GetHYDTrkThirdStageEEnergy(Int_t i)       {
      return fHYDTrk_ThirdStage_E_Energy.at(i);
   }
   // (T)
   UShort_t GetHYDTrkThirdStageTMult()           {
      return fHYDTrk_ThirdStage_T_DetectorNbr.size();
   }
   UShort_t GetHYDTrkThirdStageTDetectorNbr(Int_t i)  {
      return fHYDTrk_ThirdStage_T_DetectorNbr.at(i);
   }
   UShort_t GetHYDTrkThirdStageTPadNbr(Int_t i)       {
      return fHYDTrk_ThirdStage_T_PadNbr.at(i);
   }
   Double_t GetHYDTrkThirdStageTTime(Int_t i)         {
      return fHYDTrk_ThirdStage_T_Time.at(i);
   }

   ClassDef(THydeTrackerData, 1) // HydeTrackerData structure
};

#endif

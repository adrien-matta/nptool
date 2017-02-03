#ifndef __GASPARDTRACKERDATA__
#define __GASPARDTRACKERDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
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

class TGaspardTrackerData : public TObject{
protected:
   // First Stage
   // Double Sided Silicon Detector
   // Front
   // Energy
   vector<UShort_t>  fGPDTrk_FirstStage_FrontE_DetectorNbr;
   vector<UShort_t>  fGPDTrk_FirstStage_FrontE_StripNbr;
   vector<Double_t>  fGPDTrk_FirstStage_FrontE_Energy;
   // Time
   vector<UShort_t>  fGPDTrk_FirstStage_FrontT_DetectorNbr;
   vector<UShort_t>  fGPDTrk_FirstStage_FrontT_StripNbr;
   vector<Double_t>  fGPDTrk_FirstStage_FrontT_Time;
   // Back
   // Energy
   vector<UShort_t>  fGPDTrk_FirstStage_BackE_DetectorNbr;
   vector<UShort_t>  fGPDTrk_FirstStage_BackE_StripNbr;
   vector<Double_t>  fGPDTrk_FirstStage_BackE_Energy;
   // Time
   vector<UShort_t>  fGPDTrk_FirstStage_BackT_DetectorNbr;
   vector<UShort_t>  fGPDTrk_FirstStage_BackT_StripNbr;
   vector<Double_t>  fGPDTrk_FirstStage_BackT_Time;

   // Second Stage
   // Energy
   vector<UShort_t>  fGPDTrk_SecondStage_E_DetectorNbr;
   vector<UShort_t>  fGPDTrk_SecondStage_E_PadNbr;
   vector<Double_t>  fGPDTrk_SecondStage_E_Energy;
   // Time
   vector<UShort_t>  fGPDTrk_SecondStage_T_DetectorNbr;
   vector<UShort_t>  fGPDTrk_SecondStage_T_PadNbr;
   vector<Double_t>  fGPDTrk_SecondStage_T_Time;

   // Third Stage
   // Energy
   vector<UShort_t>  fGPDTrk_ThirdStage_E_DetectorNbr;
   vector<UShort_t>  fGPDTrk_ThirdStage_E_PadNbr;
   vector<Double_t>  fGPDTrk_ThirdStage_E_Energy;
   // Time
   vector<UShort_t>  fGPDTrk_ThirdStage_T_DetectorNbr;
   vector<UShort_t>  fGPDTrk_ThirdStage_T_PadNbr;
   vector<Double_t>  fGPDTrk_ThirdStage_T_Time;


public:
   TGaspardTrackerData();
   virtual ~TGaspardTrackerData();

   void  Clear();
   void  Clear(const Option_t*) {};
   void  Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // DSSD
   // (Front, E)
   void  SetGPDTrkFirstStageFrontEDetectorNbr(UShort_t DetNbr){
      fGPDTrk_FirstStage_FrontE_DetectorNbr.push_back(DetNbr);
   }
   void  SetGPDTrkFirstStageFrontEStripNbr(UShort_t StripNbr){
      fGPDTrk_FirstStage_FrontE_StripNbr.push_back(StripNbr);
   }
   void  SetGPDTrkFirstStageFrontEEnergy(Double_t Energy){
      fGPDTrk_FirstStage_FrontE_Energy.push_back(Energy);
   }
   // (Front, T)
   void  SetGPDTrkFirstStageFrontTDetectorNbr(UShort_t DetNbr){
      fGPDTrk_FirstStage_FrontT_DetectorNbr.push_back(DetNbr);
   }
   void  SetGPDTrkFirstStageFrontTStripNbr(UShort_t StripNbr){
      fGPDTrk_FirstStage_FrontT_StripNbr.push_back(StripNbr);
   }
   void  SetGPDTrkFirstStageFrontTTime(Double_t Time){
      fGPDTrk_FirstStage_FrontT_Time.push_back(Time);
   }

   // (Back, E)
   void  SetGPDTrkFirstStageBackEDetectorNbr(UShort_t DetNbr){
      fGPDTrk_FirstStage_BackE_DetectorNbr.push_back(DetNbr);
   }
   void  SetGPDTrkFirstStageBackEStripNbr(UShort_t StripNbr){
     fGPDTrk_FirstStage_BackE_StripNbr.push_back(StripNbr);
   }
   void  SetGPDTrkFirstStageBackEEnergy(Double_t Energy){
      fGPDTrk_FirstStage_BackE_Energy.push_back(Energy);
   }
   // (Back, T)
   void  SetGPDTrkFirstStageBackTDetectorNbr(UShort_t DetNbr){
      fGPDTrk_FirstStage_BackT_DetectorNbr.push_back(DetNbr);
   }
   void  SetGPDTrkFirstStageBackTStripNbr(UShort_t StripNbr){
      fGPDTrk_FirstStage_BackT_StripNbr.push_back(StripNbr);
   }
   void  SetGPDTrkFirstStageBackTTime(Double_t Time){
      fGPDTrk_FirstStage_BackT_Time.push_back(Time);
   }

   // Second Stage
   // (E)
   void  SetGPDTrkSecondStageEDetectorNbr(UShort_t DetNbr){
      fGPDTrk_SecondStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetGPDTrkSecondStageEPadNbr(UShort_t PadNbr){
      fGPDTrk_SecondStage_E_PadNbr.push_back(PadNbr);
   }
   void  SetGPDTrkSecondStageEEnergy(Double_t Energy){
      fGPDTrk_SecondStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetGPDTrkSecondStageTDetectorNbr(UShort_t DetNbr){
      fGPDTrk_SecondStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetGPDTrkSecondStageTPadNbr(UShort_t PadNbr){
      fGPDTrk_SecondStage_T_PadNbr.push_back(PadNbr);
   }
   void  SetGPDTrkSecondStageTTime(Double_t Time){
      fGPDTrk_SecondStage_T_Time.push_back(Time);
   }

   // Third Stage
   // (E)
   void  SetGPDTrkThirdStageEDetectorNbr(UShort_t DetNbr){
      fGPDTrk_ThirdStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetGPDTrkThirdStageEPadNbr(UShort_t PadNbr){
      fGPDTrk_ThirdStage_E_PadNbr.push_back(PadNbr);
   }
   void  SetGPDTrkThirdStageEEnergy(Double_t Energy){
      fGPDTrk_ThirdStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetGPDTrkThirdStageTDetectorNbr(UShort_t DetNbr){
      fGPDTrk_ThirdStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetGPDTrkThirdStageTPadNbr(UShort_t PadNbr){
      fGPDTrk_ThirdStage_T_PadNbr.push_back(PadNbr);
   }
   void  SetGPDTrkThirdStageTTime(Double_t Time){
      fGPDTrk_ThirdStage_T_Time.push_back(Time);
   }


   /////////////////////           GETTERS           ////////////////////////
   // DSSD
   // (Front, E)
   UShort_t GetGPDTrkFirstStageFrontEMult()               {
     if(fGPDTrk_FirstStage_FrontE_DetectorNbr.size()!= fGPDTrk_FirstStage_FrontE_StripNbr.size() || fGPDTrk_FirstStage_FrontE_DetectorNbr.size()!= fGPDTrk_FirstStage_FrontE_Energy.size())
       return 0;
     
     return fGPDTrk_FirstStage_FrontE_DetectorNbr.size();
   }
   UShort_t GetGPDTrkFirstStageFrontEDetectorNbr(const int& i){
     return fGPDTrk_FirstStage_FrontE_DetectorNbr[i];
   }
   UShort_t GetGPDTrkFirstStageFrontEStripNbr(const int& i){
      return fGPDTrk_FirstStage_FrontE_StripNbr[i];
   }
   Double_t GetGPDTrkFirstStageFrontEEnergy(const int& i){
      return fGPDTrk_FirstStage_FrontE_Energy[i];
   }
   // (Front, T)
   UShort_t GetGPDTrkFirstStageFrontTMult(){
     if(fGPDTrk_FirstStage_FrontT_DetectorNbr.size()!= fGPDTrk_FirstStage_FrontT_StripNbr.size() || fGPDTrk_FirstStage_FrontT_DetectorNbr.size()!= fGPDTrk_FirstStage_FrontT_Time.size())
       return 0;
 
     return fGPDTrk_FirstStage_FrontT_DetectorNbr.size();
   }
   UShort_t GetGPDTrkFirstStageFrontTDetectorNbr(const int& i){
      return fGPDTrk_FirstStage_FrontT_DetectorNbr[i];
   }
   UShort_t GetGPDTrkFirstStageFrontTStripNbr(const int& i){
      return fGPDTrk_FirstStage_FrontT_StripNbr[i];
   }
   Double_t GetGPDTrkFirstStageFrontTTime(const int& i){
      return fGPDTrk_FirstStage_FrontT_Time[i];
   }
   // (Back, E)
   UShort_t GetGPDTrkFirstStageBackEMult(){
    if(fGPDTrk_FirstStage_BackE_DetectorNbr.size()!= fGPDTrk_FirstStage_BackE_StripNbr.size() || fGPDTrk_FirstStage_BackE_DetectorNbr.size()!= fGPDTrk_FirstStage_BackE_Energy.size())
       return 0;
     
     return fGPDTrk_FirstStage_BackE_DetectorNbr.size();
   }
   UShort_t GetGPDTrkFirstStageBackEDetectorNbr(const int& i){
      return fGPDTrk_FirstStage_BackE_DetectorNbr[i];
   }
   UShort_t GetGPDTrkFirstStageBackEStripNbr(const int& i){
      return fGPDTrk_FirstStage_BackE_StripNbr[i];
   }
   Double_t GetGPDTrkFirstStageBackEEnergy(const int& i){
      return fGPDTrk_FirstStage_BackE_Energy[i];
   }
   // (Back, T)
   UShort_t GetGPDTrkFirstStageBackTMult(){
    if(fGPDTrk_FirstStage_BackT_DetectorNbr.size()!= fGPDTrk_FirstStage_BackT_StripNbr.size() || fGPDTrk_FirstStage_BackT_DetectorNbr.size()!= fGPDTrk_FirstStage_BackT_Time.size())
       return 0;
 
     return fGPDTrk_FirstStage_BackT_DetectorNbr.size();
   }
   UShort_t GetGPDTrkFirstStageBackTDetectorNbr(const int& i){
      return fGPDTrk_FirstStage_BackT_DetectorNbr[i];
   }
   UShort_t GetGPDTrkFirstStageBackTStripNbr(const int& i){
      return fGPDTrk_FirstStage_BackT_StripNbr[i];
   }
   Double_t GetGPDTrkFirstStageBackTTime(const int& i){
      return fGPDTrk_FirstStage_BackT_Time[i];
   }

   // Second Stage
   // (E)
   UShort_t GetGPDTrkSecondStageEMult(){
      return fGPDTrk_SecondStage_E_DetectorNbr.size();
   }
   UShort_t GetGPDTrkSecondStageEDetectorNbr(const int& i){
      return fGPDTrk_SecondStage_E_DetectorNbr[i];
   }
   UShort_t GetGPDTrkSecondStageEPadNbr(const int& i){
      return fGPDTrk_SecondStage_E_PadNbr[i];
   }
   Double_t GetGPDTrkSecondStageEEnergy(const int& i){
      return fGPDTrk_SecondStage_E_Energy[i];
   }
   // (T)
   UShort_t GetGPDTrkSecondStageTMult(){
      return fGPDTrk_SecondStage_T_DetectorNbr.size();
   }
   UShort_t GetGPDTrkSecondStageTDetectorNbr(const int& i){
      return fGPDTrk_SecondStage_T_DetectorNbr[i];
   }
   UShort_t GetGPDTrkSecondStageTPadNbr(const int& i){
      return fGPDTrk_SecondStage_T_PadNbr[i];
   }
   Double_t GetGPDTrkSecondStageTTime(const int& i){
      return fGPDTrk_SecondStage_T_Time[i];
   }

   // Third Stage
   // (E)
   UShort_t GetGPDTrkThirdStageEMult(){
      return fGPDTrk_ThirdStage_E_DetectorNbr.size();
   }
   UShort_t GetGPDTrkThirdStageEDetectorNbr(const int& i){
      return fGPDTrk_ThirdStage_E_DetectorNbr[i];
   }
   UShort_t GetGPDTrkThirdStageEPadNbr(const int& i){
      return fGPDTrk_ThirdStage_E_PadNbr[i];
   }
   Double_t GetGPDTrkThirdStageEEnergy(const int& i){
      return fGPDTrk_ThirdStage_E_Energy[i];
   }
   // (T)
   UShort_t GetGPDTrkThirdStageTMult(){
      return fGPDTrk_ThirdStage_T_DetectorNbr.size();
   }
   UShort_t GetGPDTrkThirdStageTDetectorNbr(const int& i)  {
      return fGPDTrk_ThirdStage_T_DetectorNbr[i];
   }
   UShort_t GetGPDTrkThirdStageTPadNbr(const int& i){
      return fGPDTrk_ThirdStage_T_PadNbr[i];
   }
   Double_t GetGPDTrkThirdStageTTime(const int& i){
      return fGPDTrk_ThirdStage_T_Time[i];
   }

   ClassDef(TGaspardTrackerData, 1) // GaspardTrackerData structure
};

#endif

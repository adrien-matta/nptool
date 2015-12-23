/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
 *             tracker part of the Hyde2 detector.                         *
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

#ifndef __HYDE2TRACKERDATA__
#define __HYDE2TRACKERDATA__

#include <vector>
#include "TObject.h"

using namespace std ;

class THyde2TrackerData : public TObject
{
protected:
   // First Stage
   // Double Sided Silicon Detector
   // Front
   // Energy
   vector<UShort_t>  fHYD2Trk_FirstStage_FrontE_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_FirstStage_FrontE_StripNbr;
   vector<Double_t>  fHYD2Trk_FirstStage_FrontE_Energy;
   // Time
   vector<UShort_t>  fHYD2Trk_FirstStage_FrontT_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_FirstStage_FrontT_StripNbr;
   vector<Double_t>  fHYD2Trk_FirstStage_FrontT_Time;
   // Back
   // Energy
   vector<UShort_t>  fHYD2Trk_FirstStage_BackE_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_FirstStage_BackE_StripNbr;
   vector<Double_t>  fHYD2Trk_FirstStage_BackE_Energy;
   // Time
   vector<UShort_t>  fHYD2Trk_FirstStage_BackT_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_FirstStage_BackT_StripNbr;
   vector<Double_t>  fHYD2Trk_FirstStage_BackT_Time;

   // Second Stage
   // Energy
   vector<UShort_t>  fHYD2Trk_SecondStage_E_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_SecondStage_E_PadNbr;
   vector<Double_t>  fHYD2Trk_SecondStage_E_Energy;
   // Time
   vector<UShort_t>  fHYD2Trk_SecondStage_T_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_SecondStage_T_PadNbr;
   vector<Double_t>  fHYD2Trk_SecondStage_T_Time;

   // Third Stage
   // Energy
   vector<UShort_t>  fHYD2Trk_ThirdStage_E_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_ThirdStage_E_PadNbr;
   vector<Double_t>  fHYD2Trk_ThirdStage_E_Energy;
   // Time
   vector<UShort_t>  fHYD2Trk_ThirdStage_T_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_ThirdStage_T_PadNbr;
   vector<Double_t>  fHYD2Trk_ThirdStage_T_Time;

   // Fourth Stage
   // Energy
   vector<UShort_t>  fHYD2Trk_FourthStage_E_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_FourthStage_E_PadNbr;
   vector<Double_t>  fHYD2Trk_FourthStage_E_Energy;
   // Time
   vector<UShort_t>  fHYD2Trk_FourthStage_T_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_FourthStage_T_PadNbr;
   vector<Double_t>  fHYD2Trk_FourthStage_T_Time;

   // Fifth Stage
   // Energy
   vector<UShort_t>  fHYD2Trk_FifthStage_E_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_FifthStage_E_PadNbr;
   vector<Double_t>  fHYD2Trk_FifthStage_E_Energy;
   // Time
   vector<UShort_t>  fHYD2Trk_FifthStage_T_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_FifthStage_T_PadNbr;
   vector<Double_t>  fHYD2Trk_FifthStage_T_Time;

   // Sixth Stage
   // Energy
   vector<UShort_t>  fHYD2Trk_SixthStage_E_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_SixthStage_E_PadNbr;
   vector<Double_t>  fHYD2Trk_SixthStage_E_Energy;
   // Time
   vector<UShort_t>  fHYD2Trk_SixthStage_T_DetectorNbr;
   vector<UShort_t>  fHYD2Trk_SixthStage_T_PadNbr;
   vector<Double_t>  fHYD2Trk_SixthStage_T_Time;

public:
   THyde2TrackerData();
   virtual ~THyde2TrackerData();

   void  Clear();
   void  Clear(const Option_t*) {};
   void  Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // DSSD
   // (Front, E)
   void  SetHYD2TrkFirstStageFrontEDetectorNbr(UShort_t DetNbr) {
      fHYD2Trk_FirstStage_FrontE_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkFirstStageFrontEStripNbr(UShort_t StripNbr)  {
      fHYD2Trk_FirstStage_FrontE_StripNbr.push_back(StripNbr);
   }
   void  SetHYD2TrkFirstStageFrontEEnergy(Double_t Energy)      {
      fHYD2Trk_FirstStage_FrontE_Energy.push_back(Energy);
   }
   // (Front, T)
   void  SetHYD2TrkFirstStageFrontTDetectorNbr(UShort_t DetNbr) {
      fHYD2Trk_FirstStage_FrontT_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkFirstStageFrontTStripNbr(UShort_t StripNbr)  {
      fHYD2Trk_FirstStage_FrontT_StripNbr.push_back(StripNbr);
   }
   void  SetHYD2TrkFirstStageFrontTTime(Double_t Time)          {
      fHYD2Trk_FirstStage_FrontT_Time.push_back(Time);
   }

   // (Back, E)
   void  SetHYD2TrkFirstStageBackEDetectorNbr(UShort_t DetNbr) {
      fHYD2Trk_FirstStage_BackE_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkFirstStageBackEStripNbr(UShort_t StripNbr)  {
      fHYD2Trk_FirstStage_BackE_StripNbr.push_back(StripNbr);
   }
   void  SetHYD2TrkFirstStageBackEEnergy(Double_t Energy)      {
      fHYD2Trk_FirstStage_BackE_Energy.push_back(Energy);
   }
   // (Back, T)
   void  SetHYD2TrkFirstStageBackTDetectorNbr(UShort_t DetNbr) {
      fHYD2Trk_FirstStage_BackT_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkFirstStageBackTStripNbr(UShort_t StripNbr)  {
      fHYD2Trk_FirstStage_BackT_StripNbr.push_back(StripNbr);
   }
   void  SetHYD2TrkFirstStageBackTTime(Double_t Time)          {
      fHYD2Trk_FirstStage_BackT_Time.push_back(Time);
   }

   // Second Stage
   // (E)
   void  SetHYD2TrkSecondStageEDetectorNbr(UShort_t DetNbr)   {
      fHYD2Trk_SecondStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkSecondStageEPadNbr(UShort_t PadNbr)        {
      fHYD2Trk_SecondStage_E_PadNbr.push_back(PadNbr);
   }
   void  SetHYD2TrkSecondStageEEnergy(Double_t Energy)        {
      fHYD2Trk_SecondStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetHYD2TrkSecondStageTDetectorNbr(UShort_t DetNbr)   {
      fHYD2Trk_SecondStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkSecondStageTPadNbr(UShort_t PadNbr)        {
      fHYD2Trk_SecondStage_T_PadNbr.push_back(PadNbr);
   }
   void  SetHYD2TrkSecondStageTTime(Double_t Time)            {
      fHYD2Trk_SecondStage_T_Time.push_back(Time);
   }

   // Third Stage
   // (E)
   void  SetHYD2TrkThirdStageEDetectorNbr(UShort_t DetNbr)   {
      fHYD2Trk_ThirdStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkThirdStageEPadNbr(UShort_t PadNbr)        {
      fHYD2Trk_ThirdStage_E_PadNbr.push_back(PadNbr);
   }
   void  SetHYD2TrkThirdStageEEnergy(Double_t Energy)        {
      fHYD2Trk_ThirdStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetHYD2TrkThirdStageTDetectorNbr(UShort_t DetNbr)   {
      fHYD2Trk_ThirdStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkThirdStageTPadNbr(UShort_t PadNbr)        {
      fHYD2Trk_ThirdStage_T_PadNbr.push_back(PadNbr);
   }
   void  SetHYD2TrkThirdStageTTime(Double_t Time)            {
      fHYD2Trk_ThirdStage_T_Time.push_back(Time);
   }

   // Fourth Stage
   // (E)
   void  SetHYD2TrkFourthStageEDetectorNbr(UShort_t DetNbr)   {
      fHYD2Trk_FourthStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkFourthStageEPadNbr(UShort_t PadNbr)        {
      fHYD2Trk_FourthStage_E_PadNbr.push_back(PadNbr);
   }
   void  SetHYD2TrkFourthStageEEnergy(Double_t Energy)        {
      fHYD2Trk_FourthStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetHYD2TrkFourthStageTDetectorNbr(UShort_t DetNbr)   {
      fHYD2Trk_FourthStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkFourthStageTPadNbr(UShort_t PadNbr)        {
      fHYD2Trk_FourthStage_T_PadNbr.push_back(PadNbr);
   }
   void  SetHYD2TrkFourthStageTTime(Double_t Time)            {
      fHYD2Trk_FourthStage_T_Time.push_back(Time);
   }

   // Fifth Stage
   // (E)
   void  SetHYD2TrkFifthStageEDetectorNbr(UShort_t DetNbr)   {
      fHYD2Trk_FifthStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkFifthStageEPadNbr(UShort_t PadNbr)        {
      fHYD2Trk_FifthStage_E_PadNbr.push_back(PadNbr);
   }
   void  SetHYD2TrkFifthStageEEnergy(Double_t Energy)        {
      fHYD2Trk_FifthStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetHYD2TrkFifthStageTDetectorNbr(UShort_t DetNbr)   {
      fHYD2Trk_FifthStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkFifthStageTPadNbr(UShort_t PadNbr)        {
      fHYD2Trk_FifthStage_T_PadNbr.push_back(PadNbr);
   }
   void  SetHYD2TrkFifthStageTTime(Double_t Time)            {
      fHYD2Trk_FifthStage_T_Time.push_back(Time);
   }
   
   // Sixth Stage
   // (E)
   void  SetHYD2TrkSixthStageEDetectorNbr(UShort_t DetNbr)   {
      fHYD2Trk_SixthStage_E_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkSixthStageEPadNbr(UShort_t PadNbr)        {
      fHYD2Trk_SixthStage_E_PadNbr.push_back(PadNbr);
   }
   void  SetHYD2TrkSixthStageEEnergy(Double_t Energy)        {
      fHYD2Trk_SixthStage_E_Energy.push_back(Energy);
   }
   // (T)
   void  SetHYD2TrkSixthStageTDetectorNbr(UShort_t DetNbr)   {
      fHYD2Trk_SixthStage_T_DetectorNbr.push_back(DetNbr);
   }
   void  SetHYD2TrkSixthStageTPadNbr(UShort_t PadNbr)        {
      fHYD2Trk_SixthStage_T_PadNbr.push_back(PadNbr);
   }
   void  SetHYD2TrkSixthStageTTime(Double_t Time)            {
      fHYD2Trk_SixthStage_T_Time.push_back(Time);
   }


   /////////////////////           GETTERS           ////////////////////////
   // DSSD
   // (Front, E)
   UShort_t GetHYD2TrkFirstStageFrontEMult()               {
      return fHYD2Trk_FirstStage_FrontE_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkFirstStageFrontEDetectorNbr(Int_t i) {
      return fHYD2Trk_FirstStage_FrontE_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkFirstStageFrontEStripNbr(Int_t i)    {
      return fHYD2Trk_FirstStage_FrontE_StripNbr.at(i);
   }
   Double_t GetHYD2TrkFirstStageFrontEEnergy(Int_t i)      {
      return fHYD2Trk_FirstStage_FrontE_Energy.at(i);
   }
   // (Front, T)
   UShort_t GetHYD2TrkFirstStageFrontTMult()               {
      return fHYD2Trk_FirstStage_FrontT_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkFirstStageFrontTDetectorNbr(Int_t i) {
      return fHYD2Trk_FirstStage_FrontT_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkFirstStageFrontTStripNbr(Int_t i)    {
      return fHYD2Trk_FirstStage_FrontT_StripNbr.at(i);
   }
   Double_t GetHYD2TrkFirstStageFrontTTime(Int_t i)      {
      return fHYD2Trk_FirstStage_FrontT_Time.at(i);
   }
   // (Back, E)
   UShort_t GetHYD2TrkFirstStageBackEMult()               {
      return fHYD2Trk_FirstStage_BackE_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkFirstStageBackEDetectorNbr(Int_t i) {
      return fHYD2Trk_FirstStage_BackE_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkFirstStageBackEStripNbr(Int_t i)    {
      return fHYD2Trk_FirstStage_BackE_StripNbr.at(i);
   }
   Double_t GetHYD2TrkFirstStageBackEEnergy(Int_t i)      {
      return fHYD2Trk_FirstStage_BackE_Energy.at(i);
   }
   // (Back, T)
   UShort_t GetHYD2TrkFirstStageBackTMult()               {
      return fHYD2Trk_FirstStage_BackT_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkFirstStageBackTDetectorNbr(Int_t i) {
      return fHYD2Trk_FirstStage_BackT_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkFirstStageBackTStripNbr(Int_t i)    {
      return fHYD2Trk_FirstStage_BackT_StripNbr.at(i);
   }
   Double_t GetHYD2TrkFirstStageBackTTime(Int_t i)      {
      return fHYD2Trk_FirstStage_BackT_Time.at(i);
   }

   // Second Stage
   // (E)
   UShort_t GetHYD2TrkSecondStageEMult()           {
      return fHYD2Trk_SecondStage_E_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkSecondStageEDetectorNbr(Int_t i)  {
      return fHYD2Trk_SecondStage_E_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkSecondStageEPadNbr(Int_t i)       {
      return fHYD2Trk_SecondStage_E_PadNbr.at(i);
   }
   Double_t GetHYD2TrkSecondStageEEnergy(Int_t i)       {
      return fHYD2Trk_SecondStage_E_Energy.at(i);
   }
   // (T)
   UShort_t GetHYD2TrkSecondStageTMult()           {
      return fHYD2Trk_SecondStage_T_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkSecondStageTDetectorNbr(Int_t i)  {
      return fHYD2Trk_SecondStage_T_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkSecondStageTPadNbr(Int_t i)       {
      return fHYD2Trk_SecondStage_T_PadNbr.at(i);
   }
   Double_t GetHYD2TrkSecondStageTTime(Int_t i)         {
      return fHYD2Trk_SecondStage_T_Time.at(i);
   }

   // Third Stage
   // (E)
   UShort_t GetHYD2TrkThirdStageEMult()           {
      return fHYD2Trk_ThirdStage_E_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkThirdStageEDetectorNbr(Int_t i)  {
      return fHYD2Trk_ThirdStage_E_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkThirdStageEPadNbr(Int_t i)       {
      return fHYD2Trk_ThirdStage_E_PadNbr.at(i);
   }
   Double_t GetHYD2TrkThirdStageEEnergy(Int_t i)       {
      return fHYD2Trk_ThirdStage_E_Energy.at(i);
   }
   // (T)
   UShort_t GetHYD2TrkThirdStageTMult()           {
      return fHYD2Trk_ThirdStage_T_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkThirdStageTDetectorNbr(Int_t i)  {
      return fHYD2Trk_ThirdStage_T_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkThirdStageTPadNbr(Int_t i)       {
      return fHYD2Trk_ThirdStage_T_PadNbr.at(i);
   }
   Double_t GetHYD2TrkThirdStageTTime(Int_t i)         {
      return fHYD2Trk_ThirdStage_T_Time.at(i);
   }

   // Fourth Stage
   // (E)
   UShort_t GetHYD2TrkFourthStageEMult()           {
      return fHYD2Trk_FourthStage_E_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkFourthStageEDetectorNbr(Int_t i)  {
      return fHYD2Trk_FourthStage_E_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkFourthStageEPadNbr(Int_t i)       {
      return fHYD2Trk_FourthStage_E_PadNbr.at(i);
   }
   Double_t GetHYD2TrkFourthStageEEnergy(Int_t i)       {
      return fHYD2Trk_FourthStage_E_Energy.at(i);
   }
   // (T)
   UShort_t GetHYD2TrkFourthStageTMult()           {
      return fHYD2Trk_FourthStage_T_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkFourthStageTDetectorNbr(Int_t i)  {
      return fHYD2Trk_FourthStage_T_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkFourthStageTPadNbr(Int_t i)       {
      return fHYD2Trk_FourthStage_T_PadNbr.at(i);
   }
   Double_t GetHYD2TrkFourthStageTTime(Int_t i)         {
      return fHYD2Trk_FourthStage_T_Time.at(i);
   }

   // Fifth Stage
   // (E)
   UShort_t GetHYD2TrkFifthStageEMult()           {
      return fHYD2Trk_FifthStage_E_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkFifthStageEDetectorNbr(Int_t i)  {
      return fHYD2Trk_FifthStage_E_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkFifthStageEPadNbr(Int_t i)       {
      return fHYD2Trk_FifthStage_E_PadNbr.at(i);
   }
   Double_t GetHYD2TrkFifthStageEEnergy(Int_t i)       {
      return fHYD2Trk_FifthStage_E_Energy.at(i);
   }
   // (T)
   UShort_t GetHYD2TrkFifthStageTMult()           {
      return fHYD2Trk_FifthStage_T_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkFifthStageTDetectorNbr(Int_t i)  {
      return fHYD2Trk_FifthStage_T_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkFifthStageTPadNbr(Int_t i)       {
      return fHYD2Trk_FifthStage_T_PadNbr.at(i);
   }
   Double_t GetHYD2TrkFifthStageTTime(Int_t i)         {
      return fHYD2Trk_FifthStage_T_Time.at(i);
   }

   // Sixth Stage
   // (E)
   UShort_t GetHYD2TrkSixthStageEMult()           {
      return fHYD2Trk_SixthStage_E_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkSixthStageEDetectorNbr(Int_t i)  {
      return fHYD2Trk_SixthStage_E_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkSixthStageEPadNbr(Int_t i)       {
      return fHYD2Trk_SixthStage_E_PadNbr.at(i);
   }
   Double_t GetHYD2TrkSixthStageEEnergy(Int_t i)       {
      return fHYD2Trk_SixthStage_E_Energy.at(i);
   }
   // (T)
   UShort_t GetHYD2TrkSixthStageTMult()           {
      return fHYD2Trk_SixthStage_T_DetectorNbr.size();
   }
   UShort_t GetHYD2TrkSixthStageTDetectorNbr(Int_t i)  {
      return fHYD2Trk_SixthStage_T_DetectorNbr.at(i);
   }
   UShort_t GetHYD2TrkSixthStageTPadNbr(Int_t i)       {
      return fHYD2Trk_SixthStage_T_PadNbr.at(i);
   }
   Double_t GetHYD2TrkSixthStageTTime(Int_t i)         {
      return fHYD2Trk_SixthStage_T_Time.at(i);
   }


   ClassDef(THyde2TrackerData, 1) // Hyde2TrackerData structure
};

#endif

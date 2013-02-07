#ifndef __COMPTONTELESCOPEDATA__
#define __COMPTONTELESCOPEDATA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : July 2012                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds the raw data for a Compton telescope prototype made of  *
 *  a silicon tracker detector and a LaBr3 calorimeter                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>
#include "TObject.h"

using namespace std ;
class TComptonTelescopeData : public TObject {
   private:
      // DSSSD tracker
      // X strips
      // Energy
      vector<UShort_t>   fCT_Tracker_FrontE_TowerNbr;
      vector<UShort_t>   fCT_Tracker_FrontE_DetectorNbr;
      vector<UShort_t>   fCT_Tracker_FrontE_StripNbr;
      vector<Double_t>   fCT_Tracker_FrontE_Energy;
      // Time
      vector<UShort_t>   fCT_Tracker_FrontT_TowerNbr;
      vector<UShort_t>   fCT_Tracker_FrontT_DetectorNbr;
      vector<UShort_t>   fCT_Tracker_FrontT_StripNbr;
      vector<Double_t>   fCT_Tracker_FrontT_Time;
      // Y strips
      // Energy
      vector<UShort_t>   fCT_Tracker_BackE_TowerNbr;
      vector<UShort_t>   fCT_Tracker_BackE_DetectorNbr;
      vector<UShort_t>   fCT_Tracker_BackE_StripNbr;
      vector<Double_t>   fCT_Tracker_BackE_Energy;
      // Time
      vector<UShort_t>   fCT_Tracker_BackT_TowerNbr;
      vector<UShort_t>   fCT_Tracker_BackT_DetectorNbr;
      vector<UShort_t>   fCT_Tracker_BackT_StripNbr;
      vector<Double_t>   fCT_Tracker_BackT_Time;

      // Calorimeter
      // Energy
      vector<UShort_t>   fCT_Calorimeter_E_TowerNbr;
      vector<UShort_t>   fCT_Calorimeter_E_DetectorNbr;
      vector<UShort_t>   fCT_Calorimeter_E_ChannelNbr;
      vector<Double_t>   fCT_Calorimeter_E_Energy;
      // Time
      vector<UShort_t>   fCT_Calorimeter_T_TowerNbr;
      vector<UShort_t>   fCT_Calorimeter_T_DetectorNbr;
      vector<UShort_t>   fCT_Calorimeter_T_ChannelNbr;
      vector<Double_t>   fCT_Calorimeter_T_Time;

   public:
      TComptonTelescopeData();
      virtual ~TComptonTelescopeData();

      void Clear();
      void Clear(const Option_t*) {};
      void Dump() const;

      /////////////////////           SETTERS           ////////////////////////
      // DSSD
      // (X,E)
      void SetCTTrackerFrontETowerNbr(const UShort_t TowNbr)      {fCT_Tracker_FrontE_TowerNbr.push_back(TowNbr);}
      void SetCTTrackerFrontEDetectorNbr(const UShort_t DetNbr)   {fCT_Tracker_FrontE_DetectorNbr.push_back(DetNbr);}
      void SetCTTrackerFrontEStripNbr(const UShort_t StripNbr)    {fCT_Tracker_FrontE_StripNbr.push_back(StripNbr);}
      void SetCTTrackerFrontEEnergy(const Double_t Energy)        {fCT_Tracker_FrontE_Energy.push_back(Energy);}
      // (X,T)
      void SetCTTrackerFrontTTowerNbr(const UShort_t TowNbr)      {fCT_Tracker_FrontT_TowerNbr.push_back(TowNbr);}
      void SetCTTrackerFrontTDetectorNbr(const UShort_t DetNbr)   {fCT_Tracker_FrontT_DetectorNbr.push_back(DetNbr);}
      void SetCTTrackerFrontTStripNbr(const UShort_t StripNbr)    {fCT_Tracker_FrontT_StripNbr.push_back(StripNbr);}
      void SetCTTrackerFrontTTime(const Double_t Time)            {fCT_Tracker_FrontT_Time.push_back(Time);}
      // (Y,E)
      void SetCTTrackerBackETowerNbr(const UShort_t TowNbr)       {fCT_Tracker_BackE_TowerNbr.push_back(TowNbr);}
      void SetCTTrackerBackEDetectorNbr(const UShort_t DetNbr)    {fCT_Tracker_BackE_DetectorNbr.push_back(DetNbr);}
      void SetCTTrackerBackEStripNbr(const UShort_t StripNbr)     {fCT_Tracker_BackE_StripNbr.push_back(StripNbr);}
      void SetCTTrackerBackEEnergy(const Double_t Energy)         {fCT_Tracker_BackE_Energy.push_back(Energy);}
      // (Y,T)
      void SetCTTrackerBackTTowerNbr(const UShort_t TowNbr)       {fCT_Tracker_BackT_TowerNbr.push_back(TowNbr);}
      void SetCTTrackerBackTDetectorNbr(const UShort_t DetNbr)    {fCT_Tracker_BackT_DetectorNbr.push_back(DetNbr);}
      void SetCTTrackerBackTStripNbr(const UShort_t StripNbr)     {fCT_Tracker_BackT_StripNbr.push_back(StripNbr);}
      void SetCTTrackerBackTTime(const Double_t Time)             {fCT_Tracker_BackT_Time.push_back(Time);}

      // Calorimeter
      // (E)
      void SetCTCalorimeterETowerNbr(const UShort_t TowNbr)       {fCT_Calorimeter_E_TowerNbr.push_back(TowNbr);}
      void SetCTCalorimeterEDetectorNbr(const UShort_t DetNbr)    {fCT_Calorimeter_E_TowerNbr.push_back(DetNbr);}
      void SetCTCalorimeterEChannelNbr(const UShort_t ChannelNbr) {fCT_Calorimeter_E_ChannelNbr.push_back(ChannelNbr);}
      void SetCTCalorimeterEEnergy(const Double_t Energy)         {fCT_Calorimeter_E_Energy.push_back(Energy);}
      // (T)
      void SetCTCalorimeterTTowerNbr(const UShort_t TowNbr)       {fCT_Calorimeter_T_TowerNbr.push_back(TowNbr);}
      void SetCTCalorimeterTDetectorNbr(const UShort_t DetNbr)    {fCT_Calorimeter_T_TowerNbr.push_back(DetNbr);}
      void SetCTCalorimeterTChannelNbr(const UShort_t ChannelNbr) {fCT_Calorimeter_T_ChannelNbr.push_back(ChannelNbr);}
      void SetCTCalorimeterTTime(const Double_t Time)             {fCT_Calorimeter_T_Time.push_back(Time);}


      /////////////////////           GETTERS           ////////////////////////
      // DSSD
      // (X,E)
      UShort_t   GetCTTrackerFrontEMult()                      const {return fCT_Tracker_FrontE_TowerNbr.size();}
      UShort_t   GetCTTrackerFrontETowerNbr(const Int_t i)     const {return fCT_Tracker_FrontE_TowerNbr[i];}
      UShort_t   GetCTTrackerFrontEDetectorNbr(const Int_t i)  const {return fCT_Tracker_FrontE_DetectorNbr[i];}
      UShort_t   GetCTTrackerFrontEStripNbr(const Int_t i)     const {return fCT_Tracker_FrontE_StripNbr[i];}
      Double_t   GetCTTrackerFrontEEnergy(const Int_t i)       const {return fCT_Tracker_FrontE_Energy[i];}
      // (X,T)
      UShort_t   GetCTTrackerFrontTMult()                      const {return fCT_Tracker_FrontT_TowerNbr.size();}
      UShort_t   GetCTTrackerFrontTTowerNbr(const Int_t i)     const {return fCT_Tracker_FrontT_TowerNbr[i];}
      UShort_t   GetCTTrackerFrontTDetectorNbr(const Int_t i)  const {return fCT_Tracker_FrontT_DetectorNbr[i];}
      UShort_t   GetCTTrackerFrontTStripNbr(const Int_t i)     const {return fCT_Tracker_FrontT_StripNbr[i];}
      Double_t   GetCTTrackerFrontTTime(const Int_t i)         const {return fCT_Tracker_FrontT_Time[i];}
      // (Y,E)
      UShort_t   GetCTTrackerBackEMult()                       const {return fCT_Tracker_BackE_TowerNbr.size();}
      UShort_t   GetCTTrackerBackETowerNbr(const Int_t i)      const {return fCT_Tracker_BackE_TowerNbr[i];}
      UShort_t   GetCTTrackerBackEDetectorNbr(const Int_t i)   const {return fCT_Tracker_BackE_DetectorNbr[i];}
      UShort_t   GetCTTrackerBackEStripNbr(const Int_t i)      const {return fCT_Tracker_BackE_StripNbr[i];}
      Double_t   GetCTTrackerBackEEnergy(const Int_t i)        const {return fCT_Tracker_BackE_Energy[i];}
      // (Y,T)
      UShort_t   GetCTTrackerBackTMult()                       const {return fCT_Tracker_BackT_TowerNbr.size();}
      UShort_t   GetCTrackerTBackTTowerNbr(const Int_t i)      const {return fCT_Tracker_BackT_TowerNbr[i];}
      UShort_t   GetCTTrackerBackTDetectorNbr(const Int_t i)   const {return fCT_Tracker_BackT_DetectorNbr[i];}
      UShort_t   GetCTTrackerBackTStripNbr(const Int_t i)      const {return fCT_Tracker_BackT_StripNbr[i];}
      Double_t   GetCTTrackerBackTTime(const Int_t i)          const {return fCT_Tracker_BackT_Time[i];}

      // Calorimeter 
      //(E)
      UShort_t   GetCTCalorimeterEMult()                       const {return fCT_Calorimeter_E_TowerNbr.size();}
      UShort_t   GetCTCalorimeterETowerNbr(const Int_t i)      const {return fCT_Calorimeter_E_TowerNbr[i];}
      UShort_t   GetCTCalorimeterEDetectorNbr(const Int_t i)   const {return fCT_Calorimeter_E_DetectorNbr[i];}
      UShort_t   GetCTCalorimeterEChannelNbr(const Int_t i)    const {return fCT_Calorimeter_E_ChannelNbr[i];}
      Double_t   GetCTCalorimeterEEnergy(const Int_t i)        const {return fCT_Calorimeter_E_Energy[i];}
      //(T)
      UShort_t   GetCTCalorimeterTMult()                       const {return fCT_Calorimeter_T_TowerNbr.size();}
      UShort_t   GetCTCalorimeterTTowerNbr(const Int_t i)      const {return fCT_Calorimeter_T_TowerNbr[i];}
      UShort_t   GetCTCalorimeterTDetectorNbr(const Int_t i)   const {return fCT_Calorimeter_T_DetectorNbr[i];}
      UShort_t   GetCTCalorimeterTChannelNbr(const Int_t i)    const {return fCT_Calorimeter_T_ChannelNbr[i];}
      Double_t   GetCTCalorimeterTTime(const Int_t i)          const {return fCT_Calorimeter_T_Time[i];}

      ClassDef(TComptonTelescopeData,1)  // ComptonTelescopeData structure
};

#endif

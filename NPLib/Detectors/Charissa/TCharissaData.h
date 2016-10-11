#ifndef __CHARISSADATA__
#define __CHARISSADATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : december 2013                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds Charissa raw data                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "TObject.h"

#include <vector>
using namespace std;

class TCharissaData : public TObject {
   private:
      // DSSSD 1st layer
      // X strips
      // Energy
      vector<UShort_t>   fCharissa_Layer1_StripXE_DetectorNbr;
      vector<UShort_t>   fCharissa_Layer1_StripXE_StripNbr;
      vector<Double_t>   fCharissa_Layer1_StripXE_Energy;
      // Time
      vector<UShort_t>   fCharissa_Layer1_StripXT_DetectorNbr;
      vector<UShort_t>   fCharissa_Layer1_StripXT_StripNbr;
      vector<Double_t>   fCharissa_Layer1_StripXT_Time;
      // Y strips
      // Energy
      vector<UShort_t>   fCharissa_Layer1_StripYE_DetectorNbr;
      vector<UShort_t>   fCharissa_Layer1_StripYE_StripNbr;
      vector<Double_t>   fCharissa_Layer1_StripYE_Energy;
      // Time
      vector<UShort_t>   fCharissa_Layer1_StripYT_DetectorNbr;
      vector<UShort_t>   fCharissa_Layer1_StripYT_StripNbr;
      vector<Double_t>   fCharissa_Layer1_StripYT_Time;

      // DSSSD 2nd layer
      // X strips
      // Energy
      vector<UShort_t>   fCharissa_Layer2_StripXE_DetectorNbr;
      vector<UShort_t>   fCharissa_Layer2_StripXE_StripNbr;
      vector<Double_t>   fCharissa_Layer2_StripXE_Energy;
      // Time
      vector<UShort_t>   fCharissa_Layer2_StripXT_DetectorNbr;
      vector<UShort_t>   fCharissa_Layer2_StripXT_StripNbr;
      vector<Double_t>   fCharissa_Layer2_StripXT_Time;
      // Y strips
      // Energy
      vector<UShort_t>   fCharissa_Layer2_StripYE_DetectorNbr;
      vector<UShort_t>   fCharissa_Layer2_StripYE_StripNbr;
      vector<Double_t>   fCharissa_Layer2_StripYE_Energy;
      // Time
      vector<UShort_t>   fCharissa_Layer2_StripYT_DetectorNbr;
      vector<UShort_t>   fCharissa_Layer2_StripYT_StripNbr;
      vector<Double_t>   fCharissa_Layer2_StripYT_Time;

      // CsI
      // Energy
      vector<UShort_t>   fCharissa_CsIE_DetectorNbr;
      vector<UShort_t>   fCharissa_CsIE_CristalNbr;
      vector<Double_t>   fCharissa_CsIE_Energy;
      // Time
      vector<UShort_t>   fCharissa_CsIT_DetectorNbr;
      vector<UShort_t>   fCharissa_CsIT_CristalNbr;
      vector<Double_t>   fCharissa_CsIT_Time;

   public:
      TCharissaData();
      virtual ~TCharissaData();

      void  Clear();
      void  Clear(const Option_t*) {};
      void  Dump() const;

      /////////////////////           SETTERS           ////////////////////////
      // DSSSD 1st stage
      // (X,E)
      void   SetCharissaLayer1StripXEDetectorNbr(const UShort_t DetNbr) {fCharissa_Layer1_StripXE_DetectorNbr.push_back(DetNbr);}
      void   SetCharissaLayer1StripXEStripNbr(const UShort_t StripNbr)  {fCharissa_Layer1_StripXE_StripNbr.push_back(StripNbr);}
      void   SetCharissaLayer1StripXEEnergy(const Double_t Energy)      {fCharissa_Layer1_StripXE_Energy.push_back(Energy);}
      // (X,T)
      void   SetCharissaLayer1StripXTDetectorNbr(const UShort_t DetNbr) {fCharissa_Layer1_StripXT_DetectorNbr.push_back(DetNbr);}
      void   SetCharissaLayer1StripXTStripNbr(const UShort_t StripNbr)  {fCharissa_Layer1_StripXT_StripNbr.push_back(StripNbr);}
      void   SetCharissaLayer1StripXTTime(const Double_t Time)          {fCharissa_Layer1_StripXT_Time.push_back(Time);}
      // (Y,E)
      void   SetCharissaLayer1StripYEDetectorNbr(const UShort_t DetNbr) {fCharissa_Layer1_StripYE_DetectorNbr.push_back(DetNbr);}
      void   SetCharissaLayer1StripYEStripNbr(const UShort_t StripNbr)  {fCharissa_Layer1_StripYE_StripNbr.push_back(StripNbr);}
      void   SetCharissaLayer1StripYEEnergy(const Double_t Energy)      {fCharissa_Layer1_StripYE_Energy.push_back(Energy);}
      // (Y,T)
      void   SetCharissaLayer1StripYTDetectorNbr(const UShort_t DetNbr) {fCharissa_Layer1_StripYT_DetectorNbr.push_back(DetNbr);}
      void   SetCharissaLayer1StripYTStripNbr(const UShort_t StripNbr)  {fCharissa_Layer1_StripYT_StripNbr.push_back(StripNbr);}
      void   SetCharissaLayer1StripYTTime(const Double_t Time)          {fCharissa_Layer1_StripYT_Time.push_back(Time);}

      // DSSSD 2nd stage
      // (X,E)
      void   SetCharissaLayer2StripXEDetectorNbr(const UShort_t DetNbr) {fCharissa_Layer2_StripXE_DetectorNbr.push_back(DetNbr);}
      void   SetCharissaLayer2StripXEStripNbr(const UShort_t StripNbr)  {fCharissa_Layer2_StripXE_StripNbr.push_back(StripNbr);}
      void   SetCharissaLayer2StripXEEnergy(const Double_t Energy)      {fCharissa_Layer2_StripXE_Energy.push_back(Energy);}
      // (X,T)
      void   SetCharissaLayer2StripXTDetectorNbr(const UShort_t DetNbr) {fCharissa_Layer2_StripXT_DetectorNbr.push_back(DetNbr);}
      void   SetCharissaLayer2StripXTStripNbr(const UShort_t StripNbr)  {fCharissa_Layer2_StripXT_StripNbr.push_back(StripNbr);}
      void   SetCharissaLayer2StripXTTime(const Double_t Time)          {fCharissa_Layer2_StripXT_Time.push_back(Time);}
      // (Y,E)
      void   SetCharissaLayer2StripYEDetectorNbr(const UShort_t DetNbr) {fCharissa_Layer2_StripYE_DetectorNbr.push_back(DetNbr);}
      void   SetCharissaLayer2StripYEStripNbr(const UShort_t StripNbr)  {fCharissa_Layer2_StripYE_StripNbr.push_back(StripNbr);}
      void   SetCharissaLayer2StripYEEnergy(const Double_t Energy)      {fCharissa_Layer2_StripYE_Energy.push_back(Energy);}
      // (Y,T)
      void   SetCharissaLayer2StripYTDetectorNbr(const UShort_t DetNbr) {fCharissa_Layer2_StripYT_DetectorNbr.push_back(DetNbr);}
      void   SetCharissaLayer2StripYTStripNbr(const UShort_t StripNbr)  {fCharissa_Layer2_StripYT_StripNbr.push_back(StripNbr);}
      void   SetCharissaLayer2StripYTTime(const Double_t Time)          {fCharissa_Layer2_StripYT_Time.push_back(Time);}

      // CsI
      // (E)
      void   SetCharissaCsIEDetectorNbr(const UShort_t DetNbr)    {fCharissa_CsIE_DetectorNbr.push_back(DetNbr);}
      void   SetCharissaCsIECristalNbr(const UShort_t CristalNbr) {fCharissa_CsIE_CristalNbr.push_back(CristalNbr);}
      void   SetCharissaCsIEEnergy(const Double_t Energy)         {fCharissa_CsIE_Energy.push_back(Energy);}
      // (T)
      void   SetCharissaCsITDetectorNbr(const UShort_t DetNbr)    {fCharissa_CsIT_DetectorNbr.push_back(DetNbr);}
      void   SetCharissaCsITCristalNbr(const UShort_t CristalNbr) {fCharissa_CsIT_CristalNbr.push_back(CristalNbr);}
      void   SetCharissaCsITTime(const Double_t Time)             {fCharissa_CsIT_Time.push_back(Time);}

      /////////////////////           GETTERS           ////////////////////////
      // DSSSD 1st layer
      // (X,E)
      UShort_t   GetCharissaLayer1StripXEMult()                     const {return fCharissa_Layer1_StripXE_DetectorNbr.size();}
      UShort_t   GetCharissaLayer1StripXEDetectorNbr(const Int_t i) const {return fCharissa_Layer1_StripXE_DetectorNbr[i];}
      UShort_t   GetCharissaLayer1StripXEStripNbr(const Int_t i)    const {return fCharissa_Layer1_StripXE_StripNbr[i];}
      Double_t   GetCharissaLayer1StripXEEnergy(const Int_t i)      const {return fCharissa_Layer1_StripXE_Energy[i];}
      // (X,T)
      UShort_t   GetCharissaLayer1StripXTMult()                     const {return fCharissa_Layer1_StripXT_DetectorNbr.size();}
      UShort_t   GetCharissaLayer1StripXTDetectorNbr(const Int_t i) const {return fCharissa_Layer1_StripXT_DetectorNbr[i];}
      UShort_t   GetCharissaLayer1StripXTStripNbr(const Int_t i)    const {return fCharissa_Layer1_StripXT_StripNbr[i];}
      Double_t   GetCharissaLayer1StripXTTime(const Int_t i)        const {return fCharissa_Layer1_StripXT_Time[i];}
      // (Y,E)
      UShort_t   GetCharissaLayer1StripYEMult()                     const {return fCharissa_Layer1_StripYE_DetectorNbr.size();}
      UShort_t   GetCharissaLayer1StripYEDetectorNbr(const Int_t i) const {return fCharissa_Layer1_StripYE_DetectorNbr[i];}
      UShort_t   GetCharissaLayer1StripYEStripNbr(const Int_t i)    const {return fCharissa_Layer1_StripYE_StripNbr[i];}
      Double_t   GetCharissaLayer1StripYEEnergy(const Int_t i)      const {return fCharissa_Layer1_StripYE_Energy[i];}
      // (Y,T)
      UShort_t   GetCharissaLayer1StripYTMult()                     const {return fCharissa_Layer1_StripYT_DetectorNbr.size();}
      UShort_t   GetCharissaLayer1StripYTDetectorNbr(const Int_t i) const {return fCharissa_Layer1_StripYT_DetectorNbr[i];}
      UShort_t   GetCharissaLayer1StripYTStripNbr(const Int_t i)    const {return fCharissa_Layer1_StripYT_StripNbr[i];}
      Double_t   GetCharissaLayer1StripYTTime(const Int_t i)        const {return fCharissa_Layer1_StripYT_Time[i];}

      // DSSSD 2nd layer
      // (X,E)
      UShort_t   GetCharissaLayer2StripXEMult()                     const {return fCharissa_Layer2_StripXE_DetectorNbr.size();}
      UShort_t   GetCharissaLayer2StripXEDetectorNbr(const Int_t i) const {return fCharissa_Layer2_StripXE_DetectorNbr[i];}
      UShort_t   GetCharissaLayer2StripXEStripNbr(const Int_t i)    const {return fCharissa_Layer2_StripXE_StripNbr[i];}
      Double_t   GetCharissaLayer2StripXEEnergy(const Int_t i)      const {return fCharissa_Layer2_StripXE_Energy[i];}
      // (X,T)
      UShort_t   GetCharissaLayer2StripXTMult()                     const {return fCharissa_Layer2_StripXT_DetectorNbr.size();}
      UShort_t   GetCharissaLayer2StripXTDetectorNbr(const Int_t i) const {return fCharissa_Layer2_StripXT_DetectorNbr[i];}
      UShort_t   GetCharissaLayer2StripXTStripNbr(const Int_t i)    const {return fCharissa_Layer2_StripXT_StripNbr[i];}
      Double_t   GetCharissaLayer2StripXTTime(const Int_t i)        const {return fCharissa_Layer2_StripXT_Time[i];}
      // (Y,E)
      UShort_t   GetCharissaLayer2StripYEMult()                     const {return fCharissa_Layer2_StripYE_DetectorNbr.size();}
      UShort_t   GetCharissaLayer2StripYEDetectorNbr(const Int_t i) const {return fCharissa_Layer2_StripYE_DetectorNbr[i];}
      UShort_t   GetCharissaLayer2StripYEStripNbr(const Int_t i)    const {return fCharissa_Layer2_StripYE_StripNbr[i];}
      Double_t   GetCharissaLayer2StripYEEnergy(const Int_t i)      const {return fCharissa_Layer2_StripYE_Energy[i];}
      // (Y,T)
      UShort_t   GetCharissaLayer2StripYTMult()                     const {return fCharissa_Layer2_StripYT_DetectorNbr.size();}
      UShort_t   GetCharissaLayer2StripYTDetectorNbr(const Int_t i) const {return fCharissa_Layer2_StripYT_DetectorNbr[i];}
      UShort_t   GetCharissaLayer2StripYTStripNbr(const Int_t i)    const {return fCharissa_Layer2_StripYT_StripNbr[i];}
      Double_t   GetCharissaLayer2StripYTTime(const Int_t i)        const {return fCharissa_Layer2_StripYT_Time[i];}

      // CsI 
      //(E)
      UShort_t   GetCharissaCsIEMult()                                 const {return fCharissa_CsIE_DetectorNbr.size();}
      UShort_t   GetCharissaCsIEDetectorNbr(const Int_t i)   const {return fCharissa_CsIE_DetectorNbr[i];}
      UShort_t   GetCharissaCsIECristalNbr(const Int_t i)    const {return fCharissa_CsIE_CristalNbr[i];}
      Double_t   GetCharissaCsIEEnergy(const Int_t i)        const {return fCharissa_CsIE_Energy[i];}
      //(T)
      UShort_t   GetCharissaCsITMult()                             const {return fCharissa_CsIT_DetectorNbr.size();}
      UShort_t   GetCharissaCsITDetectorNbr(const Int_t i)   const {return fCharissa_CsIT_DetectorNbr[i];}
      UShort_t   GetCharissaCsITCristalNbr(const Int_t i)    const {return fCharissa_CsIT_CristalNbr[i];}
      Double_t   GetCharissaCsITTime(const Int_t i)          const {return fCharissa_CsIT_Time[i];}

      ClassDef(TCharissaData,1)  // CharissaData structure
};

#endif

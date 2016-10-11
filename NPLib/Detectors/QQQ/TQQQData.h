#ifndef __QQQDATA__
#define __QQQDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the Single Side Striped Detector raw data                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>
#include "TObject.h"

using namespace std ;
class TQQQData : public TObject {
   private:
      // QQQ
      // Energy
      vector<UShort_t>   fQQQ_StripE_DetectorNbr;
      vector<UShort_t>   fQQQ_StripE_StripNbr;
      vector<Double_t>   fQQQ_StripE_Energy;
      // Time
      vector<UShort_t>   fQQQ_StripT_DetectorNbr;
      vector<UShort_t>   fQQQ_StripT_StripNbr;
      vector<Double_t>   fQQQ_StripT_Time;


   public:
      TQQQData();
      virtual ~TQQQData();

      void   Clear();
    void    Clear(const Option_t*) {};
      void   Dump() const;

      /////////////////////           SETTERS           ////////////////////////
      // DSSD
      // (X,E)
      void   SetEnergyDetectorNbr   (const UShort_t DetNbr)   { fQQQ_StripE_DetectorNbr.push_back(DetNbr) ;}
      void   SetEnergyStripNbr      (const UShort_t StripNbr) { fQQQ_StripE_StripNbr.push_back(StripNbr) ;}
      void   SetEnergy              (const Double_t Energy)   { fQQQ_StripE_Energy.push_back(Energy) ;}
      // (X,T)
      void   SetTimeDetectorNbr     (const UShort_t DetNbr)   { fQQQ_StripT_DetectorNbr.push_back(DetNbr) ;}
      void   SetTimeStripNbr        (const UShort_t StripNbr) { fQQQ_StripT_StripNbr.push_back(StripNbr) ;}
      void   SetTime                (const Double_t Time)     { fQQQ_StripT_Time.push_back(Time) ;}

      /////////////////////           GETTERS           ////////////////////////
      // DSSD
      // (X,E)
      UShort_t   GetEnergyMult        ()              const {   return fQQQ_StripE_DetectorNbr.size() ;}
      UShort_t   GetEnergyDetectorNbr (const Int_t i) const {   return fQQQ_StripE_DetectorNbr[i] ;}
      UShort_t   GetEnergyStripNbr    (const Int_t i) const {   return fQQQ_StripE_StripNbr[i] ;}
      Double_t   GetEnergy            (const Int_t i) const {   return fQQQ_StripE_Energy[i] ;}
      // (X,T)
      UShort_t   GetTimeMult          ()              const {   return fQQQ_StripT_DetectorNbr.size() ;}
      UShort_t   GetTimeDetectorNbr   (const Int_t i) const {   return fQQQ_StripT_DetectorNbr[i] ;}
      UShort_t   GetTimeStripNbr      (const Int_t i) const {   return fQQQ_StripT_StripNbr[i] ;}
      Double_t   GetTime              (const Int_t i) const {   return fQQQ_StripT_Time[i] ;}
   
      ClassDef(TQQQData,1)  // QQQData structure
};

#endif

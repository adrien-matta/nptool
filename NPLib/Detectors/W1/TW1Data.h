#ifndef __W1DATA__
#define __W1DATA__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : january 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class holds the raw data storage for the W1 detector from Micron *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <vector>
using namespace std;

// ROOT headers
#include "TObject.h"


class TW1Data : public TObject
{
   private:
      // Front
      // Energy
      vector<UShort_t>  fW1_FrontE_DetectorNbr;
      vector<UShort_t>  fW1_FrontE_StripNbr;
      vector<Double_t>  fW1_FrontE_Energy;
      // Time
      vector<UShort_t>  fW1_FrontT_DetectorNbr;
      vector<UShort_t>  fW1_FrontT_StripNbr;
      vector<Double_t>  fW1_FrontT_Time;

      // Back
      // Energy
      vector<UShort_t>  fW1_BackE_DetectorNbr;
      vector<UShort_t>  fW1_BackE_StripNbr;
      vector<Double_t>  fW1_BackE_Energy;
      // Time
      vector<UShort_t>  fW1_BackT_DetectorNbr;
      vector<UShort_t>  fW1_BackT_StripNbr;
      vector<Double_t>  fW1_BackT_Time;


   public:
      TW1Data();
      virtual ~TW1Data();

      void Clear();
      void Clear(const Option_t*) {};
      void Dump() const;

      /////////////////////           SETTERS           ////////////////////////
      // DSSD
      // (Front, E)
      void SetFrontEDetectorNbr(UShort_t DetNbr)    {fW1_FrontE_DetectorNbr.push_back(DetNbr);}
      void SetFrontEStripNbr(UShort_t StripNbr)     {fW1_FrontE_StripNbr.push_back(StripNbr);}
      void SetFrontEEnergy(Double_t Energy)         {fW1_FrontE_Energy.push_back(Energy);}
      // (Front, T)
      void SetFrontTDetectorNbr(UShort_t DetNbr)    {fW1_FrontT_DetectorNbr.push_back(DetNbr);}
      void SetFrontTStripNbr(UShort_t StripNbr)     {fW1_FrontT_StripNbr.push_back(StripNbr);}
      void SetFrontTTime(Double_t Time)             {fW1_FrontT_Time.push_back(Time);}

      // (Back, E)
      void SetBackEDetectorNbr(UShort_t DetNbr)     {fW1_BackE_DetectorNbr.push_back(DetNbr);}
      void SetBackEStripNbr(UShort_t StripNbr)      {fW1_BackE_StripNbr.push_back(StripNbr);}
      void SetBackEEnergy(Double_t Energy)          {fW1_BackE_Energy.push_back(Energy);}
      // (Back, T)
      void SetBackTDetectorNbr(UShort_t DetNbr)     {fW1_BackT_DetectorNbr.push_back(DetNbr);}
      void SetBackTStripNbr(UShort_t StripNbr)      {fW1_BackT_StripNbr.push_back(StripNbr);}
      void SetBackTTime(Double_t Time)              {fW1_BackT_Time.push_back(Time);}

      // Front E
      void SetFrontE(UShort_t DetNbr, UShort_t StripNbr, Double_t Energy) {
         SetFrontEDetectorNbr(DetNbr);
         SetFrontEStripNbr(StripNbr);
         SetFrontEEnergy(Energy);
      }
      // Front T
      void SetFrontT(UShort_t DetNbr, UShort_t StripNbr, Double_t Time) {
         SetFrontTDetectorNbr(DetNbr);
         SetFrontTStripNbr(StripNbr);
         SetFrontTTime(Time);
      }

      // Back E
      void SetBackE(UShort_t DetNbr, UShort_t StripNbr, Double_t Energy) {
         SetBackEDetectorNbr(DetNbr);
         SetBackEStripNbr(StripNbr);
         SetBackEEnergy(Energy);
      }
      // Back T
      void SetBackT(UShort_t DetNbr, UShort_t StripNbr, Double_t Time) {
         SetBackTDetectorNbr(DetNbr);
         SetBackTStripNbr(StripNbr);
         SetBackTTime(Time);
      }


      /////////////////////           GETTERS           ////////////////////////
      // DSSD
      // (Front, E)
      UShort_t GetFrontEMult()                      const {return fW1_FrontE_DetectorNbr.size();}
      UShort_t GetFrontEDetectorNbr(Int_t i)        const {return fW1_FrontE_DetectorNbr[i];}
      UShort_t GetFrontEStripNbr(Int_t i)           const {return fW1_FrontE_StripNbr[i];}
      Double_t GetFrontEEnergy(Int_t i)             const {return fW1_FrontE_Energy[i];}
      // (Front, T)
      UShort_t GetFrontTMult()                      const {return fW1_FrontT_DetectorNbr.size();}
      UShort_t GetFrontTDetectorNbr(Int_t i)        const {return fW1_FrontT_DetectorNbr[i];}
      UShort_t GetFrontTStripNbr(Int_t i)           const {return fW1_FrontT_StripNbr[i];}
      Double_t GetFrontTTime(Int_t i)               const {return fW1_FrontT_Time[i];}

      // (Back, E)
      UShort_t GetBackEMult()                       const {return fW1_BackE_DetectorNbr.size();}
      UShort_t GetBackEDetectorNbr(Int_t i)         const {return fW1_BackE_DetectorNbr[i];}
      UShort_t GetBackEStripNbr(Int_t i)            const {return fW1_BackE_StripNbr[i];}
      Double_t GetBackEEnergy(Int_t i)              const {return fW1_BackE_Energy[i];}
      // (Back, T)
      UShort_t GetBackTMult()                       const {return fW1_BackT_DetectorNbr.size();}
      UShort_t GetBackTDetectorNbr(Int_t i)         const {return fW1_BackT_DetectorNbr[i];}
      UShort_t GetBackTStripNbr(Int_t i)            const {return fW1_BackT_StripNbr[i];}
      Double_t GetBackTTime(Int_t i)                const {return fW1_BackT_Time[i];}

      ClassDef(TW1Data, 1) // TW1Data raw data 
};

#endif

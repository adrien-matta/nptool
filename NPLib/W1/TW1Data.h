#ifndef __W1DATA__
#define __W1DATA__
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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

// ROOT headers
#include "TObject.h"

using namespace std ;
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
      void SetW1FrontEDetectorNbr(UShort_t DetNbr)    {fW1_FrontE_DetectorNbr.push_back(DetNbr);}
      void SetW1FrontEStripNbr(UShort_t StripNbr)     {fW1_FrontE_StripNbr.push_back(StripNbr);}
      void SetW1FrontEEnergy(Double_t Energy)         {fW1_FrontE_Energy.push_back(Energy);}
      // (Front, T)
      void SetW1FrontTDetectorNbr(UShort_t DetNbr)    {fW1_FrontT_DetectorNbr.push_back(DetNbr);}
      void SetW1FrontTStripNbr(UShort_t StripNbr)     {fW1_FrontT_StripNbr.push_back(StripNbr);}
      void SetW1FrontTTime(Double_t Time)             {fW1_FrontT_Time.push_back(Time);}

      // (Back, E)
      void SetW1BackEDetectorNbr(UShort_t DetNbr)     {fW1_BackE_DetectorNbr.push_back(DetNbr);}
      void SetW1BackEStripNbr(UShort_t StripNbr)      {fW1_BackE_StripNbr.push_back(StripNbr);}
      void SetW1BackEEnergy(Double_t Energy)          {fW1_BackE_Energy.push_back(Energy);}
      // (Back, T)
      void SetW1BackTDetectorNbr(UShort_t DetNbr)     {fW1_BackT_DetectorNbr.push_back(DetNbr);}
      void SetW1BackTStripNbr(UShort_t StripNbr)      {fW1_BackT_StripNbr.push_back(StripNbr);}
      void SetW1BackTTime(Double_t Time)              {fW1_BackT_Time.push_back(Time);}

      /////////////////////           GETTERS           ////////////////////////
      // DSSD
      // (Front, E)
      UShort_t GetW1FrontEMult()                      {return fW1_FrontE_DetectorNbr.size();}
      UShort_t GetW1FrontEDetectorNbr(Int_t i)        {return fW1_FrontE_DetectorNbr.at(i);}
      UShort_t GetW1FrontEStripNbr(Int_t i)           {return fW1_FrontE_StripNbr.at(i);}
      Double_t GetW1FrontEEnergy(Int_t i)             {return fW1_FrontE_Energy.at(i);}
      // (Front, T)
      UShort_t GetW1FrontTMult()                      {return fW1_FrontT_DetectorNbr.size();}
      UShort_t GetW1FrontTDetectorNbr(Int_t i)        {return fW1_FrontT_DetectorNbr.at(i);}
      UShort_t GetW1FrontTStripNbr(Int_t i)           {return fW1_FrontT_StripNbr.at(i);}
      Double_t GetW1FrontTTime(Int_t i)               {return fW1_FrontT_Time.at(i);}

      // (Back, E)
      UShort_t GetW1BackEMult()                      {return fW1_BackE_DetectorNbr.size();}
      UShort_t GetW1BackEDetectorNbr(Int_t i)        {return fW1_BackE_DetectorNbr.at(i);}
      UShort_t GetW1BackEStripNbr(Int_t i)           {return fW1_BackE_StripNbr.at(i);}
      Double_t GetW1BackEEnergy(Int_t i)             {return fW1_BackE_Energy.at(i);}
      // (Back, T)
      UShort_t GetW1BackTMult()                      {return fW1_BackT_DetectorNbr.size();}
      UShort_t GetW1BackTDetectorNbr(Int_t i)        {return fW1_BackT_DetectorNbr.at(i);}
      UShort_t GetW1BackTStripNbr(Int_t i)           {return fW1_BackT_StripNbr.at(i);}
      Double_t GetW1BackTTime(Int_t i)               {return fW1_BackT_Time.at(i);}

      ClassDef(TW1Data, 1) // TW1Data raw data 
};

#endif

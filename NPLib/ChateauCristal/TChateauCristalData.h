#ifndef __ChateauCristalDATA__
#define __ChateauCristalDATA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 04/12/2009                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describes the raw data of the ChateauCristal detector         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;


class TChateauCristalData : public TObject {

 private:
   vector<UShort_t>   fChateauCristal_EDetectorNbr;
   vector<Double_t>   fChateauCristal_Energy;
   vector<UShort_t>   fChateauCristal_TDetectorNbr;
   vector<Double_t>   fChateauCristal_Time;

 public:
   TChateauCristalData();
   virtual ~TChateauCristalData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   void   SetChateauCristalEDetectorNbr(const UShort_t DetNbr)    {fChateauCristal_EDetectorNbr.push_back(DetNbr);}
   void   SetChateauCristalEnergy      (const Double_t Energy)	   {fChateauCristal_Energy.push_back(Energy);}
   void   SetChateauCristalTDetectorNbr(const UShort_t DetNbr)    {fChateauCristal_TDetectorNbr.push_back(DetNbr);}
   void   SetChateauCristalTime        (const Double_t Time)      {fChateauCristal_Time.push_back(Time);}

   /////////////////////           GETTERS           ////////////////////////
   UShort_t  GetChateauCristalEnergyMult()		                  const {return fChateauCristal_EDetectorNbr.size();}
   UShort_t  GetChateauCristalEDetectorNbr(const Int_t i)         const {return fChateauCristal_EDetectorNbr[i];}
   Double_t   GetChateauCristalEnergy(const Int_t i)   	         const {return fChateauCristal_Energy[i];}

   UShort_t   GetChateauCristalTimeMult()		                     const {return fChateauCristal_TDetectorNbr.size();}
   UShort_t   GetChateauCristalTDetectorNbr(const Int_t i)        const {return fChateauCristal_TDetectorNbr[i];}
   Double_t   GetChateauCristalTime(const Int_t i)	               const {return fChateauCristal_Time[i];}
   
   ClassDef(TChateauCristalData,1)  // ChateauCristalData structure
};

#endif

#ifndef __TACDATA__
#define __TACDATA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:    contact address:                                      *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include <vector>
#include "TObject.h"



class TTacData : public TObject {
 private:
   // TAC
   UShort_t	fTAC_MM_CATS1;
   UShort_t	fTAC_MM_CATS2;
   UShort_t	fTAC_PL_CATS1;
   UShort_t	fTAC_PL_CATS2;
   UShort_t	fTAC_CATS12;
   UShort_t	fTAC_PL_GAL;
   UShort_t	fTAC_CATS1_HF;
   UShort_t	fTAC_CATS2_HF;
   UShort_t	fTAC_MM_HF;
   UShort_t fTAC_MM1_HF;
   UShort_t fTAC_MM2_HF;
   UShort_t fTAC_MM3_HF;
   UShort_t fTAC_MM4_HF;
   UShort_t fTAC_MM5_HF;
   UShort_t fTAC_MM6_HF;
   UShort_t fTAC_MM7_HF;
   UShort_t fTAC_MM8_HF;
   vector<UShort_t>   fTAC_MM_HF_DetectorNbr;

 public:
   TTacData();
   virtual ~TTacData();

   void Clear();
   void Clear(const Option_t*) {};
   void Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (TAC)
   UShort_t	GetTAC_MM_CATS1()             {return fTAC_MM_CATS1;}
   UShort_t	GetTAC_MM_CATS2()             {return fTAC_MM_CATS2;}
   UShort_t	GetTAC_PL_CATS1()             {return fTAC_PL_CATS1;}
   UShort_t	GetTAC_PL_CATS2()             {return fTAC_PL_CATS2;}
   UShort_t	GetTAC_CATS12()		         {return fTAC_CATS12;}
   UShort_t	GetTAC_PL_GAL()               {return fTAC_PL_GAL;}
   UShort_t	GetTAC_CATS1_HF()             {return fTAC_CATS1_HF;}
   UShort_t	GetTAC_CATS2_HF()             {return fTAC_CATS2_HF;}
   UShort_t	GetTAC_MM_HF()         	      {return fTAC_MM_HF;}
   Double_t GetTAC_MM1_HF()               {return fTAC_MM1_HF;}
   Double_t GetTAC_MM2_HF()               {return fTAC_MM2_HF;}
   Double_t GetTAC_MM3_HF()               {return fTAC_MM3_HF;}
   Double_t GetTAC_MM4_HF()               {return fTAC_MM4_HF;}
   Double_t GetTAC_MM5_HF()               {return fTAC_MM5_HF;}
   Double_t GetTAC_MM6_HF()               {return fTAC_MM6_HF;}
   Double_t GetTAC_MM7_HF()               {return fTAC_MM7_HF;}
   Double_t GetTAC_MM8_HF()               {return fTAC_MM8_HF;}
   Double_t GetTAC_MM_HF_DetectorNbr(const Int_t i) const {return fTAC_MM_HF_DetectorNbr[i];}

   /////////////////////           SETTERS           ////////////////////////
   // (TAC)
   void	SetTAC_MM_CATS1(UShort_t T)			{fTAC_MM_CATS1 = T;}
   void	SetTAC_MM_CATS2(UShort_t T)			{fTAC_MM_CATS2 = T;}
   void	SetTAC_PL_CATS1(UShort_t T)			{fTAC_PL_CATS1 = T;}
   void	SetTAC_PL_CATS2(UShort_t T)			{fTAC_PL_CATS2 = T;}
   void	SetTAC_CATS12(UShort_t T)			   {fTAC_CATS12 = T;}
   void	SetTAC_PL_GAL(UShort_t T)			   {fTAC_PL_GAL = T;}
   void	SetTAC_CATS1_HF(UShort_t T)			{fTAC_CATS1_HF = T;}
   void	SetTAC_CATS2_HF(UShort_t T)			{fTAC_CATS2_HF = T;}
   void	SetTAC_MM_HF(UShort_t T)			   {fTAC_MM_HF = T;}
   void  SetTAC_MM1_HF(UShort_t T)           {fTAC_MM1_HF = T;}
   void  SetTAC_MM2_HF(UShort_t T)           {fTAC_MM2_HF = T;}
   void  SetTAC_MM3_HF(UShort_t T)           {fTAC_MM3_HF = T;}
   void  SetTAC_MM4_HF(UShort_t T)           {fTAC_MM4_HF = T;}
   void  SetTAC_MM5_HF(UShort_t T)           {fTAC_MM5_HF = T;}
   void  SetTAC_MM6_HF(UShort_t T)           {fTAC_MM6_HF = T;}
   void  SetTAC_MM7_HF(UShort_t T)           {fTAC_MM7_HF = T;}
   void  SetTAC_MM8_HF(UShort_t T)           {fTAC_MM8_HF = T;}
   void  SetTAC_MM_HF_DetectorNbr(const UShort_t DetNbr) {fTAC_MM_HF_DetectorNbr.push_back(DetNbr);}


   ClassDef(TTacData,2)  // TacData structure
};

#endif

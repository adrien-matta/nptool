#ifndef __TACDATA__
#define __TACDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Benjamin Le Crom   contact address: lecrom@ipno.in2p3.fr *
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
   UShort_t	fTAC_CATS12;
   UShort_t	fTAC_CATS1_PLD4;
   UShort_t	fTAC_CATS1_HF;
   UShort_t	fTAC_CATS1_EXO;
   UShort_t	fTAC_MM_HF;
   vector<UShort_t>   fTAC_MM_HF_DetectorNbr;
   UShort_t fTAC_MM_EXO;
   UShort_t fTAC_BARREL_EXO;
   UShort_t fTAC_BARREL_CATS1FAG;
   UShort_t fTAC_HYB_EXO;
   UShort_t fTAC_HYB_CATS1FAG;
   UShort_t fTAC_CHAR_EXO;
   UShort_t fTAC_CHAR_CATS1FAG;
   UShort_t fTAC_CHAR_PLD4;
   UShort_t fTAC_CHAR_GALOTTE;
   UShort_t fTAC_CATS1_CATS2;
   UShort_t fTAC_CATS1_GALOTTE;
   UShort_t fTAC_FAG_CATS2;



 public:
   TTacData();
   virtual ~TTacData();

   void Clear();
   void Clear(const Option_t*) {};
   void Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (TAC)
   UShort_t GetTAC_MM_CATS1()             {return fTAC_MM_CATS1;}
   UShort_t GetTAC_CATS12()		  {return fTAC_CATS12;}
   UShort_t GetTAC_CATS1_PLD4()		  {return fTAC_CATS1_PLD4;}
   UShort_t GetTAC_CATS1_HF()             {return fTAC_CATS1_HF;}
   UShort_t GetTAC_CATS1_EXO()             {return fTAC_CATS1_EXO;}
   UShort_t GetTAC_MM_HF()         	  {return fTAC_MM_HF;}
   Double_t GetTAC_MM_HF_DetectorNbr(const Int_t i) const {return fTAC_MM_HF_DetectorNbr[i];}
   Double_t GetTAC_MM_EXO()		  {return fTAC_MM_EXO;}
   Double_t GetTAC_BARREL_EXO()		  {return fTAC_BARREL_EXO;}
   Double_t GetTAC_BARREL_CATS1FAG()	  {return fTAC_BARREL_CATS1FAG;}
   Double_t GetTAC_HYB_EXO()		  {return fTAC_HYB_EXO;}
   Double_t GetTAC_HYB_CATS1FAG()	  {return fTAC_HYB_CATS1FAG;}
   Double_t GetTAC_CHAR_EXO()		  {return fTAC_CHAR_EXO;}
   Double_t GetTAC_CHAR_CATS1FAG()	  {return fTAC_CHAR_CATS1FAG;}
   Double_t GetTAC_CHAR_PLD4()		  {return fTAC_CHAR_PLD4;}
   Double_t GetTAC_CHAR_GALOTTE()	  {return fTAC_CHAR_GALOTTE;}
   Double_t GetTAC_CATS1_CATS2()	  {return fTAC_CATS1_CATS2;}
   Double_t GetTAC_CATS1_GALOTTE()	  {return fTAC_CATS1_GALOTTE;}
   Double_t GetTAC_FAG_CATS2()	          {return fTAC_FAG_CATS2;}

   /////////////////////           SETTERS           ////////////////////////
   // (TAC)
   void	SetTAC_MM_CATS1(UShort_t T)			{fTAC_MM_CATS1 = T;}
   void	SetTAC_CATS12(UShort_t T)			{fTAC_CATS12 = T;}
   void	SetTAC_CATS1_PLD4(UShort_t T)			{fTAC_CATS1_PLD4 = T;}
   void	SetTAC_CATS1_HF(UShort_t T)			{fTAC_CATS1_HF = T;}
   void	SetTAC_CATS1_EXO(UShort_t T)			{fTAC_CATS1_EXO = T;}
   void	SetTAC_MM_HF(UShort_t T)			{fTAC_MM_HF = T;}
   void SetTAC_MM_HF_DetectorNbr(const UShort_t DetNbr) {fTAC_MM_HF_DetectorNbr.push_back(DetNbr);}
   void	SetTAC_MM_EXO(UShort_t T)			{fTAC_MM_EXO = T;}
   void	SetTAC_BARREL_EXO(UShort_t T)			{fTAC_BARREL_EXO = T;}
   void	SetTAC_BARREL_CATS1FAG(UShort_t T)		{fTAC_BARREL_CATS1FAG = T;}
   void	SetTAC_HYB_EXO(UShort_t T)			{fTAC_HYB_EXO = T;}
   void	SetTAC_HYB_CATS1FAG(UShort_t T)			{fTAC_HYB_CATS1FAG = T;}
   void	SetTAC_CHAR_EXO(UShort_t T)			{fTAC_CHAR_EXO = T;}
   void	SetTAC_CHAR_CATS1FAG(UShort_t T)		{fTAC_CHAR_CATS1FAG = T;}
   void	SetTAC_CHAR_PLD4(UShort_t T)			{fTAC_CHAR_PLD4 = T;}
   void	SetTAC_CHAR_GALOTTE(UShort_t T)			{fTAC_CHAR_GALOTTE = T;}
   void	SetTAC_CATS1_CATS2(UShort_t T)			{fTAC_CATS1_CATS2 = T;}
   void	SetTAC_CATS1_GALOTTE(UShort_t T)		{fTAC_CATS1_GALOTTE = T;}
   void	SetTAC_FAG_CATS2(UShort_t T)			{fTAC_FAG_CATS2 = T;}


   ClassDef(TTacData,3)  // TacData structure
};

#endif

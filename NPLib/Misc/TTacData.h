#ifndef __TACDATA__
#define __TACDATA__

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

 public:
   TTacData();
   virtual ~TTacData();

   void	Clear();
   void Clear(const Option_t*) {};
   void Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (TAC)
   UShort_t	GetTAC_MM_CATS1()         {return fTAC_MM_CATS1;}
   UShort_t	GetTAC_MM_CATS2()         {return fTAC_MM_CATS2;}
   UShort_t	GetTAC_PL_CATS1()         {return fTAC_PL_CATS1;}
   UShort_t	GetTAC_PL_CATS2()         {return fTAC_PL_CATS2;}
   UShort_t	GetTAC_CATS12()		  {return fTAC_CATS12;}
   UShort_t	GetTAC_PL_GAL()           {return fTAC_PL_GAL;}
   UShort_t	GetTAC_CATS1_HF()         {return fTAC_CATS1_HF;}
   UShort_t	GetTAC_CATS2_HF()         {return fTAC_CATS2_HF;}
   UShort_t	GetTAC_MM_HF()         	  {return fTAC_MM_HF;}

   /////////////////////           SETTERS           ////////////////////////
   // (TAC)
   void	SetTAC_MM_CATS1(UShort_t T)			{fTAC_MM_CATS1 = T;}
   void	SetTAC_MM_CATS2(UShort_t T)			{fTAC_MM_CATS2 = T;}
   void	SetTAC_PL_CATS1(UShort_t T)			{fTAC_PL_CATS1 = T;}
   void	SetTAC_PL_CATS2(UShort_t T)			{fTAC_PL_CATS2 = T;}
   void	SetTAC_CATS12(UShort_t T)			{fTAC_CATS12 = T;}
   void	SetTAC_PL_GAL(UShort_t T)			{fTAC_PL_GAL = T;}
   void	SetTAC_CATS1_HF(UShort_t T)			{fTAC_CATS1_HF = T;}
   void	SetTAC_CATS2_HF(UShort_t T)			{fTAC_CATS2_HF = T;}
   void	SetTAC_MM_HF(UShort_t T)			{fTAC_MM_HF = T;}

   ClassDef(TTacData,1)  // TacData structure
};

#endif

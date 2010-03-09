#ifndef __TACDATA__
#define __TACDATA__

#include <vector>

#include "TObject.h"



class TTacData : public TObject {
 private:
   // TAC
   UShort_t	fTAC_MM1_CATS2;
   UShort_t	fTAC_MM2_CATS2;
   UShort_t	fTAC_MM3_CATS2;
   UShort_t	fTAC_MM4_CATS2;
   UShort_t	fTAC_S1_CATS2;
   UShort_t	fTAC_PL_CATS2;
   UShort_t	fTAC_EXO_CATS2;
   UShort_t	fTAC_HF_CATS2;
   UShort_t	fTAC_CATS1_2;
   UShort_t	fTAC_PL_GAL;
   UShort_t	fTAC_MMS1_EXO;
   //TDC
   UShort_t	fTDC_MM1_CATS2;
   UShort_t	fTDC_MM2_CATS2;
   UShort_t	fTDC_MM3_CATS2;
   UShort_t	fTDC_MM4_CATS2;
   UShort_t	fTDC_S1_CATS2;
   UShort_t	fTDC_CATS1_2;
   UShort_t	fTDC_PL_CATS2;
   UShort_t	fTDC_EXO_CATS2;
   UShort_t	fTDC_HF_CATS2;
   UShort_t	fTDC_MM1_HF;
   UShort_t	fTDC_MM2_HF;
   UShort_t	fTDC_MM3_HF;
   UShort_t	fTDC_MM4_HF;
   UShort_t	fTDC_S1_HF;
   UShort_t	fTDC_CATS2_HF;

 public:
   TTacData();
   virtual ~TTacData();

   void	Clear();
   void	Dump();



   /////////////////////           GETTERS           ////////////////////////
   // (TAC)
   UShort_t	GetTAC_MM1_CATS2()         {return fTAC_MM1_CATS2;}
   UShort_t	GetTAC_MM2_CATS2()         {return fTAC_MM2_CATS2;}
   UShort_t	GetTAC_MM3_CATS2()         {return fTAC_MM3_CATS2;}
   UShort_t	GetTAC_MM4_CATS2()         {return fTAC_MM4_CATS2;}
   UShort_t	GetTAC_S1_CATS2()         {return fTAC_S1_CATS2;}
   UShort_t	GetTAC_PL_CATS2()         {return fTAC_PL_CATS2;}
   UShort_t	GetTAC_EXO_CATS2()         {return fTAC_EXO_CATS2;}
   UShort_t	GetTAC_HF_CATS2()         {return fTAC_HF_CATS2;}
   UShort_t	GetTAC_CATS1_2()         {return fTAC_CATS1_2;}
   UShort_t	GetTAC_PL_GAL()        {return fTAC_PL_GAL;}
   UShort_t	GetTAC_MMS1_EXO()        {return fTAC_MMS1_EXO;}
   // (TDC)
   UShort_t	GetTDC_MM1_CATS2()         {return fTDC_MM1_CATS2;}
   UShort_t	GetTDC_MM2_CATS2()         {return fTDC_MM2_CATS2;}
   UShort_t	GetTDC_MM3_CATS2()         {return fTDC_MM3_CATS2;}
   UShort_t	GetTDC_MM4_CATS2()         {return fTDC_MM4_CATS2;}
   UShort_t	GetTDC_S1_CATS2()         {return fTDC_S1_CATS2;}
   UShort_t	GetTDC_CATS1_2()         {return fTDC_CATS1_2;}
   UShort_t	GetTDC_PL_CATS2()         {return fTDC_PL_CATS2;}
   UShort_t	GetTDC_EXO_CATS2()         {return fTDC_EXO_CATS2;}
   UShort_t	GetTDC_HF_CATS2()         {return fTDC_HF_CATS2;}
   UShort_t	GetTDC_MM1_HF()        {return fTDC_MM1_HF;}
   UShort_t	GetTDC_MM2_HF()        {return fTDC_MM2_HF;}
   UShort_t	GetTDC_MM3_HF()        {return fTDC_MM3_HF;}
   UShort_t	GetTDC_MM4_HF()        {return fTDC_MM4_HF;}
   UShort_t	GetTDC_S1_HF()        {return fTDC_S1_HF;}
   UShort_t	GetTDC_CATS2_HF()        {return fTDC_CATS2_HF;}


   /////////////////////           SETTERS           ////////////////////////
   // (TAC)
   void	SetTAC_MM1_CATS2(UShort_t T)      {fTAC_MM1_CATS2 = T;}
   void	SetTAC_MM2_CATS2(UShort_t T)      {fTAC_MM2_CATS2 = T;}
   void	SetTAC_MM3_CATS2(UShort_t T)      {fTAC_MM3_CATS2 = T;}
   void	SetTAC_MM4_CATS2(UShort_t T)      {fTAC_MM4_CATS2 = T;}
   void	SetTAC_S1_CATS2(UShort_t T)      {fTAC_S1_CATS2 = T;}
   void	SetTAC_PL_CATS2(UShort_t T)      {fTAC_PL_CATS2 = T;}
   void	SetTAC_EXO_CATS2(UShort_t T)      {fTAC_EXO_CATS2 = T;}
   void	SetTAC_HF_CATS2(UShort_t T)      {fTAC_HF_CATS2 = T;}
   void	SetTAC_CATS1_2(UShort_t T)      {fTAC_CATS1_2 = T;}
   void	SetTAC_PL_GAL(UShort_t T)     {fTAC_PL_GAL = T;}
   void	SetTAC_MMS1_EXO(UShort_t T)     {fTAC_MMS1_EXO = T;}
   // (TDC)
   void	SetTDC_MM1_CATS2(UShort_t T)      {fTDC_MM1_CATS2 = T;}
   void	SetTDC_MM2_CATS2(UShort_t T)      {fTDC_MM2_CATS2 = T;}
   void	SetTDC_MM3_CATS2(UShort_t T)      {fTDC_MM3_CATS2 = T;}
   void	SetTDC_MM4_CATS2(UShort_t T)      {fTDC_MM4_CATS2 = T;}
   void	SetTDC_S1_CATS2(UShort_t T)      {fTDC_S1_CATS2 = T;}
   void	SetTDC_CATS1_2 (UShort_t T)      {fTDC_CATS1_2 = T;}
   void	SetTDC_PL_CATS2(UShort_t T)      {fTDC_PL_CATS2 = T;}
   void	SetTDC_EXO_CATS2(UShort_t T)      {fTDC_EXO_CATS2 = T;}
   void	SetTDC_HF_CATS2(UShort_t T)      {fTDC_HF_CATS2 = T;}
   void	SetTDC_MM1_HF(UShort_t T)     {fTDC_MM1_HF = T;}
   void	SetTDC_MM2_HF(UShort_t T)     {fTDC_MM2_HF = T;}
   void	SetTDC_MM3_HF(UShort_t T)     {fTDC_MM3_HF = T;}
   void	SetTDC_MM4_HF(UShort_t T)     {fTDC_MM4_HF = T;}
   void	SetTDC_S1_HF(UShort_t T)     {fTDC_S1_HF = T;}
   void	SetTDC_CATS2_HF(UShort_t T)     {fTDC_CATS2_HF = T;}

   ClassDef(TTacData,1)  // TacData structure
};

#endif

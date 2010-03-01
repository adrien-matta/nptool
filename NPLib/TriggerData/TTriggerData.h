#ifndef __TRIGGERDATA__
#define __TRIGGERDATA__

#include <vector>

#include "TObject.h"



class TTriggerData : public TObject {
 private:
   // TDC
   UShort_t	fTRIG_1;
   UShort_t	fTRIG_2;
   UShort_t	fTRIG_3;
   UShort_t	fTRIG_4;
   UShort_t	fTRIG_5;

 public:
   TTriggerData();
   virtual ~TTriggerData();

      void	Clear();
      void	Dump();



   /////////////////////           GETTERS           ////////////////////////
   // (T)
   UShort_t	GetTRIG1()        {return fTRIG_1;}
   UShort_t	GetTRIG2()        {return fTRIG_2;}
   UShort_t	GetTRIG3()        {return fTRIG_3;}
   UShort_t	GetTRIG4()        {return fTRIG_4;}
   UShort_t	GetTRIG5()        {return fTRIG_5;}


   /////////////////////           SETTERS           ////////////////////////
   // (T)
   void	SetTRIG1(UShort_t T)     {fTRIG_1 = T;}
   void	SetTRIG2(UShort_t T)     {fTRIG_2 = T;}
   void	SetTRIG3(UShort_t T)     {fTRIG_3 = T;}
   void	SetTRIG4(UShort_t T)     {fTRIG_4 = T;}
   void	SetTRIG5(UShort_t T)     {fTRIG_5 = T;}

   ClassDef(TTriggerData,1)  // TriggerData structure
};

#endif

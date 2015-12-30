#ifndef __CHIO_DIGDATA__
#define __CHIO_DIGDATA__

#include <vector>
#include "TObject.h"
using namespace std;


class TChio_digData : public TObject {
 private:
   // ADC
   vector<UShort_t>	fChio_dig_Energy;

   //sum 
   Int_t fChio_dig_Sum;

 public:
   TChio_digData();
   virtual ~TChio_digData();

   void Clear();
   void Clear(const Option_t*) {};
   void Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   UShort_t	GetEsize()           {return fChio_dig_Energy.size();}
   UShort_t	GetEnergy(Int_t i)   {return fChio_dig_Energy.at(i);}

   //sum
   Int_t    GetSum()             {return fChio_dig_Sum;}

   /////////////////////           SETTERS           ////////////////////////
   //sum
   void SetSum(UShort_t E)       {fChio_dig_Sum += (Int_t)E;}
   // (E)
   void SetEnergy(UShort_t E)    {fChio_dig_Energy.push_back(E);}

   ClassDef(TChio_digData,1)  // Chio_digData structure
};

#endif

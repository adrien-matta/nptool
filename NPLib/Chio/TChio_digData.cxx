#include <iostream>
using namespace std;

#include "TChio_digData.h"


ClassImp(TChio_digData)

TChio_digData::TChio_digData()
{
   // Default constructor

   // (E)
   fChio_dig_Energy.clear();
   //sum
   fChio_dig_Sum = 0;
}



TChio_digData::~TChio_digData()
{
}



void TChio_digData::Clear()
{
   // (E)
   fChio_dig_Energy.clear();
   //sum
   fChio_dig_Sum = 0;
}



void TChio_digData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // (E)
   cout << "Chio_dig_Esize = " << fChio_dig_Energy.size() << endl;
   for (UShort_t i = 0; i < fChio_dig_Energy.size(); i++)
      cout <<  " Energy: " << fChio_dig_Energy[i] << endl;
  //sum 
   cout<<"Chio_dig_sum" << fChio_dig_Sum << endl;
}

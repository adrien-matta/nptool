#include <iostream>
#include "TRIPSData.h"


ClassImp(TRIPSData)

TRIPSData::TRIPSData()
{
}



TRIPSData::~TRIPSData()
{
}



void TRIPSData::Clear()
{
   RunNumber = -1 ;
   EventNumber = -1;

   fRIPS_PPAC_N.clear();
   fRIPS_PPAC_X.clear();
   fRIPS_PPAC_Y.clear();
   fRIPS_PPAC_T.clear();
   
   fRIPS_Plastic_N.clear();
   fRIPS_Plastic_E.clear();
   fRIPS_Plastic_EL.clear();
   fRIPS_Plastic_ER.clear();
   fRIPS_Plastic_TL.clear();
   fRIPS_Plastic_TR.clear();
   
   fRIPS_RF_N.clear();
   fRIPS_RF_T.clear();
}



void TRIPSData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
}

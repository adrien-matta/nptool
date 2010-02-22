#include <iostream>
using namespace std;

#include "TCATSData.h"


ClassImp(TCATSData)

TCATSData::TCATSData()
{
   // Default constructor

   // X
   fCATS_DetX.clear();
   fCATS_StripX.clear();
   fCATS_ChargeX.clear();
   // Y
   fCATS_DetY.clear();
   fCATS_StripY.clear();
   fCATS_ChargeY.clear();
   // (Qfil)
   fCATS_DetQ.clear();
   fCATS_Charge.clear();
}



TCATSData::~TCATSData()
{
}



void TCATSData::Clear()
{
   // X
   fCATS_DetX.clear();
   fCATS_StripX.clear();
   fCATS_ChargeX.clear();
   // Y
   fCATS_DetY.clear();
   fCATS_StripY.clear();
   fCATS_ChargeY.clear();
   // (Qfil)
   fCATS_DetQ.clear();
   fCATS_Charge.clear();
}



void TCATSData::Dump()
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // X
   cout << "CATS_MultX = " << fCATS_DetX.size() << endl;
   for (UShort_t i = 0; i < fCATS_DetX.size(); i++)
      cout << "DetX: " << fCATS_DetX[i] << " StripX: " << fCATS_StripX[i] << " ChargeX: " << fCATS_ChargeX[i] << endl;
   // Y
   cout << "CATS_MultY = " << fCATS_DetY.size() << endl;
   for (UShort_t i = 0; i < fCATS_DetY.size(); i++)
      cout << "DetY: " << fCATS_DetY[i] << " StripY: " << fCATS_StripY[i] << " EnergyY: " << fCATS_ChargeY[i] << endl;
   // (Qfil)
   cout << "MM_MultQ = " << fCATS_DetQ.size() << endl;
   for (UShort_t i = 0; i < fCATS_DetQ.size(); i++)
      cout << "DetQ: " << fCATS_DetQ[i] << " Charge: " << fCATS_Charge[i] << endl;
}

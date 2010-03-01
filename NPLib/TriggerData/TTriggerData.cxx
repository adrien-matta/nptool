#include <iostream>
using namespace std;

#include "TTriggerData.h"


ClassImp(TTriggerData)

TTriggerData::TTriggerData()
{
   // Default constructor

   // (T)
   fTRIG_1 = -1;
   fTRIG_2 = -1;
   fTRIG_3 = -1;
   fTRIG_4 = -1;
   fTRIG_5 = -1;

}



TTriggerData::~TTriggerData()
{
}



void TTriggerData::Clear()
{
   // (T)
   fTRIG_1 = -1;
   fTRIG_2 = -1;
   fTRIG_3 = -1;
   fTRIG_4 = -1;
   fTRIG_5 = -1;
}



void TTriggerData::Dump()
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   cout << "TRIG_1 : " << fTRIG_1 << endl;
   cout << "TRIG_2 : " << fTRIG_2 << endl;
   cout << "TRIG_3 : " << fTRIG_3 << endl;
   cout << "TRIG_4 : " << fTRIG_4 << endl;
   cout << "TRIG_5 : " << fTRIG_5 << endl;
}

#include <iostream>
using namespace std;

#include "TTacData.h"


ClassImp(TTacData)

TTacData::TTacData()
{
   // Default constructor

  Clear();

}



TTacData::~TTacData()
{
}



void TTacData::Clear()
{
   // (TAC)
  fTAC_MM1_CATS2  = -1;
  fTAC_MM2_CATS2  = -1;
  fTAC_MM3_CATS2  = -1;
  fTAC_MM4_CATS2  = -1;
  fTAC_S1_CATS2  = -1;
  fTAC_PL_CATS2  = -1;
  fTAC_EXO_CATS2  = -1;
  fTAC_HF_CATS2  = -1;
  fTAC_CATS1_2  = -1;
  fTAC_PL_GAL = -1;
  fTAC_MMS1_EXO = -1;
   // (TDC)
  fTDC_MM1_CATS2  = -1;
  fTDC_MM2_CATS2  = -1;
  fTDC_MM3_CATS2  = -1;
  fTDC_MM4_CATS2  = -1;
  fTDC_S1_CATS2  = -1;
  fTDC_CATS1_2  = -1;
  fTDC_PL_CATS2  = -1;
  fTDC_EXO_CATS2  = -1;
  fTDC_HF_CATS2  = -1;
  fTDC_MM1_HF = -1;
  fTDC_MM2_HF  = -1;
  fTDC_MM3_HF  = -1;
  fTDC_MM4_HF  = -1;
  fTDC_S1_HF  = -1;
  fTDC_CATS2_HF  = -1;
}



void TTacData::Dump()
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;
   // (TAC)
   cout << "TAC_MM1_CATS2 : "  << fTAC_MM1_CATS2 << endl;
   cout << "TAC_MM2_CATS2 : "  << fTAC_MM2_CATS2 << endl;
   cout << "TAC_MM3_CATS2 : "  << fTAC_MM3_CATS2 << endl;
   cout << "TAC_MM4_CATS2 : "  << fTAC_MM4_CATS2 << endl;
   cout << "TAC_S1_CATS2 : "  << fTAC_S1_CATS2 << endl;
   cout << "TAC_PL_CATS2 : "  << fTAC_PL_CATS2 << endl;
   cout << "TAC_EXO_CATS2 : "  << fTAC_EXO_CATS2 << endl;
   cout << "TAC_HF_CATS2 : "  << fTAC_HF_CATS2 << endl;
   cout << "TAC_CATS1_2 : "  << fTAC_CATS1_2 << endl;
   cout << "TAC_PL_GAL : " << fTAC_PL_GAL << endl;
   cout << "TAC_MMS1_EXO : " << fTAC_MMS1_EXO << endl;
   // (TDC)
   cout << "TDC_MM1_CATS2 : "   << fTDC_MM1_CATS2 << endl;
   cout << "TDC_MM2_CATS2 : "   << fTDC_MM2_CATS2 << endl;
   cout << "TDC_MM3_CATS2 : "   << fTDC_MM3_CATS2 << endl;
   cout << "TDC_MM4_CATS2 : "   << fTDC_MM4_CATS2 << endl;
   cout << "TDC_S1_CATS2 : "   << fTDC_S1_CATS2 << endl;
   cout << "TDC_CATS1_2 : "   << fTDC_CATS1_2 << endl;
   cout << "TDC_PL_CATS2 : "   << fTDC_PL_CATS2 << endl;
   cout << "TDC_EXO_CATS2 : "   << fTDC_EXO_CATS2 << endl;
   cout << "TDC_HF_CATS2 : "   << fTDC_HF_CATS2 << endl;
   cout << "TDC_MM1_HF : "  << fTDC_MM1_HF << endl;
   cout << "TDC_MM2_HF : "  << fTDC_MM2_HF << endl;
   cout << "TDC_MM3_HF : "  << fTDC_MM3_HF << endl;
   cout << "TDC_MM4_HF : "  << fTDC_MM4_HF << endl;
   cout << "TDC_S1_HF : "  << fTDC_S1_HF << endl;
   cout << "TDC_CATS2_HF : "  << fTDC_CATS2_HF << endl;

}

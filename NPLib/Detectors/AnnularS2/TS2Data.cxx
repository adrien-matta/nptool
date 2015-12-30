#include <iostream>
using namespace std;

#include "TS2Data.h"


ClassImp(TS2Data)

TS2Data::TS2Data()
{
   // Default constructor

   Clear();
}



TS2Data::~TS2Data()
{
}



void TS2Data::Clear()
{
   // DSSD
   // (Th,E)
   fS2_Theta_E_DetNbr.clear();
   fS2_Theta_E_StripNbr.clear();
   fS2_Theta_E_Energy.clear();
   // (Th,T)
   fS2_Theta_T_DetNbr.clear();
   fS2_Theta_T_StripNbr.clear();
   fS2_Theta_T_Time.clear();
   // (Ph,E)
   fS2_Phi_E_DetNbr.clear();
   fS2_Phi_E_StripNbr.clear();
   fS2_Phi_E_Energy.clear();
   // (Ph,T)
   fS2_Phi_T_DetNbr.clear();
   fS2_Phi_T_StripNbr.clear();
   fS2_Phi_T_Time.clear();
}



void TS2Data::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // DSSD
   // (Th,E)
   cout << "S2_MultThE = " << fS2_Theta_E_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fS2_Theta_E_DetNbr.size(); i++)
      cout << "DetThE: " << fS2_Theta_E_DetNbr[i] << " StripThE: " << fS2_Theta_E_StripNbr[i] << " EnergyTh: " << fS2_Theta_E_Energy[i] << endl;
   // (Th,T)
   cout << "S2_MultThT = " << fS2_Theta_T_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fS2_Theta_T_DetNbr.size(); i++)
      cout << "DetThT: " << fS2_Theta_T_DetNbr[i] << " StripThT: " << fS2_Theta_T_StripNbr[i] << " TimeTh: " << fS2_Theta_T_Time[i] << endl;
   // (Ph,E)
   cout << "S2_MultPhE = " << fS2_Phi_E_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fS2_Phi_E_DetNbr.size(); i++)
      cout << "DetPhE: " << fS2_Phi_E_DetNbr[i] << " StripPhE: " << fS2_Phi_E_StripNbr[i] << " EnergyPh: " << fS2_Phi_E_Energy[i] << endl;
   // (Ph,T)
   cout << "S2_MultThT = " << fS2_Phi_T_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fS2_Phi_T_DetNbr.size(); i++)
      cout << "DetThT: " << fS2_Phi_T_DetNbr[i] << " StripThT: " << fS2_Phi_T_StripNbr[i] << " TimeTh: " << fS2_Phi_T_Time[i] << endl;
}

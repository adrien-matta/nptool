#include <iostream>
using namespace std;

#include "TS1Data.h"


ClassImp(TS1Data)

TS1Data::TS1Data()
{
   // Default constructor

   Clear();
}



TS1Data::~TS1Data()
{
}



void TS1Data::Clear()
{
   // DSSD
   // (Th,E)
   fS1_Theta_E_DetNbr.clear();
   fS1_Theta_E_StripNbr.clear();
   fS1_Theta_E_Energy.clear();
   // (Th,T)
   fS1_Theta_T_DetNbr.clear();
   fS1_Theta_T_StripNbr.clear();
   fS1_Theta_T_Time.clear();
   // (Ph,E)
   fS1_Phi_E_DetNbr.clear();
   fS1_Phi_E_StripNbr.clear();
   fS1_Phi_E_Energy.clear();
   // (Ph,T)
   fS1_Phi_T_DetNbr.clear();
   fS1_Phi_T_StripNbr.clear();
   fS1_Phi_T_Time.clear();
}



void TS1Data::Dump()
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // DSSD
   // (Th,E)
   cout << "S1_MultThE = " << fS1_Theta_E_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fS1_Theta_E_DetNbr.size(); i++)
      cout << "DetThE: " << fS1_Theta_E_DetNbr[i] << " StripThE: " << fS1_Theta_E_StripNbr[i] << " EnergyTh: " << fS1_Theta_E_Energy[i] << endl;
   // (Th,T)
   cout << "S1_MultThT = " << fS1_Theta_T_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fS1_Theta_T_DetNbr.size(); i++)
      cout << "DetThT: " << fS1_Theta_T_DetNbr[i] << " StripThT: " << fS1_Theta_T_StripNbr[i] << " TimeTh: " << fS1_Theta_T_Time[i] << endl;
   // (Ph,E)
   cout << "S1_MultPhE = " << fS1_Phi_E_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fS1_Phi_E_DetNbr.size(); i++)
      cout << "DetPhE: " << fS1_Phi_E_DetNbr[i] << " StripPhE: " << fS1_Phi_E_StripNbr[i] << " EnergyPh: " << fS1_Phi_E_Energy[i] << endl;
   // (Ph,T)
   cout << "S1_MultThT = " << fS1_Phi_T_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fS1_Phi_T_DetNbr.size(); i++)
      cout << "DetThT: " << fS1_Phi_T_DetNbr[i] << " StripThT: " << fS1_Phi_T_StripNbr[i] << " TimeTh: " << fS1_Phi_T_Time[i] << endl;
}

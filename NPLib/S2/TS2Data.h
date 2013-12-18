#ifndef __S2DATA__
#define __S2DATA__

#include <vector>

#include "TObject.h"

using namespace std;

class TS2Data : public TObject {
 private:
   // DSSD
   // Theta strips
   // ADC
  vector<UShort_t>	fS2_Theta_E_DetNbr;
  vector<UShort_t>	fS2_Theta_E_StripNbr;
  vector<Double_t>	fS2_Theta_E_Energy;
  // TDC
  vector<UShort_t>	fS2_Theta_T_DetNbr;
  vector<UShort_t>	fS2_Theta_T_StripNbr;
  vector<Double_t>	fS2_Theta_T_Time;
  // Phi strips
  // ADC
  vector<UShort_t>	fS2_Phi_E_DetNbr;
  vector<UShort_t>	fS2_Phi_E_StripNbr;
  vector<Double_t>	fS2_Phi_E_Energy;
  // TDC
  vector<UShort_t>	fS2_Phi_T_DetNbr;
  vector<UShort_t>	fS2_Phi_T_StripNbr;
  vector<Double_t>	fS2_Phi_T_Time;

 public:
   TS2Data();
   virtual ~TS2Data();

   void	Clear();
   void  Clear(const Option_t*) {};
   void	Dump() const;



   /////////////////////           GETTERS           ////////////////////////
   // (Th,E)
   UShort_t	GetS2ThetaEMult()		{return fS2_Theta_E_DetNbr.size();}
   UShort_t	GetS2ThetaEDetectorNbr(Int_t i)	{return fS2_Theta_E_DetNbr.at(i);}
   UShort_t	GetS2ThetaEStripNbr(Int_t i)	{return fS2_Theta_E_StripNbr.at(i);}
   Double_t	GetS2ThetaEEnergy(Int_t i)	{return fS2_Theta_E_Energy.at(i);}
   // (Th,T)
   UShort_t	GetS2ThetaTMult()		{return fS2_Theta_T_DetNbr.size();}
   UShort_t	GetS2ThetaTDetectorNbr(Int_t i)	{return fS2_Theta_T_DetNbr.at(i);}
   UShort_t	GetS2ThetaTStripNbr(Int_t i)	{return fS2_Theta_T_StripNbr.at(i);}
   Double_t	GetS2ThetaTTime(Int_t i)	{return fS2_Theta_T_Time.at(i);}
   // (Ph,E)
   UShort_t	GetS2PhiEMult()			{return fS2_Phi_E_DetNbr.size();}
   UShort_t	GetS2PhiEDetectorNbr(Int_t i)	{return fS2_Phi_E_DetNbr.at(i);}
   UShort_t	GetS2PhiEStripNbr(Int_t i)	{return fS2_Phi_E_StripNbr.at(i);}
   Double_t	GetS2PhiEEnergy(Int_t i)	{return fS2_Phi_E_Energy.at(i);}
   // (Ph,T)
   UShort_t	GetS2PhiTMult()			{return fS2_Phi_T_DetNbr.size();}
   UShort_t	GetS2PhiTDetectorNbr(Int_t i)	{return fS2_Phi_T_DetNbr.at(i);}
   UShort_t	GetS2PhiTStripNbr(Int_t i)	{return fS2_Phi_T_StripNbr.at(i);}
   Double_t	GetS2PhiTTime(Int_t i)		{return fS2_Phi_T_Time.at(i);}

   /////////////////////           SETTERS           ////////////////////////
   // (Th,E)
   void	SetS2ThetaEDetectorNbr(UShort_t det)	{fS2_Theta_E_DetNbr.push_back(det);}
   void	SetS2ThetaEStripNbr(UShort_t Nr)	{fS2_Theta_E_StripNbr.push_back(Nr);}
   void	SetS2ThetaEEnergy(Double_t E)		{fS2_Theta_E_Energy.push_back(E);}
   // (Th,T)
   void	SetS2ThetaTDetectorNbr(UShort_t det)	{fS2_Theta_T_DetNbr.push_back(det);}
   void	SetS2ThetaTStripNbr(UShort_t Nr)	{fS2_Theta_T_StripNbr.push_back(Nr);}
   void	SetS2ThetaTTime(Double_t T)		{fS2_Theta_T_Time.push_back(T);}
   // (Ph,E)
   void	SetS2PhiEDetectorNbr(UShort_t det)	{fS2_Phi_E_DetNbr.push_back(det);}
   void	SetS2PhiEStripNbr(UShort_t Nr)		{fS2_Phi_E_StripNbr.push_back(Nr);}
   void	SetS2PhiEEnergy(Double_t E)		{fS2_Phi_E_Energy.push_back(E);}
   // (Ph,T)
   void	SetS2PhiTDetectorNbr(UShort_t det)	{fS2_Phi_T_DetNbr.push_back(det);}
   void	SetS2PhiTStripNbr(UShort_t Nr)		{fS2_Phi_T_StripNbr.push_back(Nr);}
   void	SetS2PhiTTime(Double_t T)		{fS2_Phi_T_Time.push_back(T);}

   ClassDef(TS2Data,1)  // S2Data structure
};

#endif

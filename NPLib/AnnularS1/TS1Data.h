/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the results of the G4 simulation for the S1 *
 *             detector.                                                     *
 *             The format is the same as the one which is used for the GANIL *
 *             experiments after conversion of the raw data with GRU. Ask    *
 *             N. de Sereville and J. Burgunder (burgunder@ganil.fr) for     *
 *             informations.                                                 *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef __S1DATA__
#define __S1DATA__

#include <vector>

#include "TObject.h"



class TS1Data : public TObject {
 private:
   // DSSD
   // Theta strips
   // ADC
   vector<UShort_t>	fS1_Theta_E_DetNbr;
   vector<UShort_t>	fS1_Theta_E_StripNbr;
   vector<Double_t>	fS1_Theta_E_Energy;
   // TDC
   vector<UShort_t>	fS1_Theta_T_DetNbr;
   vector<UShort_t>	fS1_Theta_T_StripNbr;
   vector<Double_t>	fS1_Theta_T_Time;
   // Phi strips
   // ADC
   vector<UShort_t>	fS1_Phi_E_DetNbr;
   vector<UShort_t>	fS1_Phi_E_StripNbr;
   vector<Double_t>	fS1_Phi_E_Energy;
   // TDC
   vector<UShort_t>	fS1_Phi_T_DetNbr;
   vector<UShort_t>	fS1_Phi_T_StripNbr;
   vector<Double_t>	fS1_Phi_T_Time;

 public:
   TS1Data();
   virtual ~TS1Data();

   void	Clear();
   void Clear(const Option_t*) {};
   void	Dump() const;



   /////////////////////           GETTERS           ////////////////////////
   // (Th,E)
   UShort_t	GetS1ThetaEMult()		{return fS1_Theta_E_DetNbr.size();}
   UShort_t	GetS1ThetaEDetectorNbr(Int_t i)	{return fS1_Theta_E_DetNbr.at(i);}
   UShort_t	GetS1ThetaEStripNbr(Int_t i)	{return fS1_Theta_E_StripNbr.at(i);}
   Double_t	GetS1ThetaEEnergy(Int_t i)	{return fS1_Theta_E_Energy.at(i);}
   // (Th,T)
   UShort_t	GetS1ThetaTMult()		{return fS1_Theta_T_DetNbr.size();}
   UShort_t	GetS1ThetaTDetectorNbr(Int_t i)	{return fS1_Theta_T_DetNbr.at(i);}
   UShort_t	GetS1ThetaTStripNbr(Int_t i)	{return fS1_Theta_T_StripNbr.at(i);}
   Double_t	GetS1ThetaTTime(Int_t i)	{return fS1_Theta_T_Time.at(i);}
   // (Ph,E)
   UShort_t	GetS1PhiEMult()			{return fS1_Phi_E_DetNbr.size();}
   UShort_t	GetS1PhiEDetectorNbr(Int_t i)	{return fS1_Phi_E_DetNbr.at(i);}
   UShort_t	GetS1PhiEStripNbr(Int_t i)	{return fS1_Phi_E_StripNbr.at(i);}
   Double_t	GetS1PhiEEnergy(Int_t i)	{return fS1_Phi_E_Energy.at(i);}
   // (Ph,T)
   UShort_t	GetS1PhiTMult()			{return fS1_Phi_T_DetNbr.size();}
   UShort_t	GetS1PhiTDetectorNbr(Int_t i)	{return fS1_Phi_T_DetNbr.at(i);}
   UShort_t	GetS1PhiTStripNbr(Int_t i)	{return fS1_Phi_T_StripNbr.at(i);}
   Double_t	GetS1PhiTTime(Int_t i)		{return fS1_Phi_T_Time.at(i);}

   /////////////////////           SETTERS           ////////////////////////
   // (Th,E)
   void	SetS1ThetaEDetectorNbr(UShort_t det)	{fS1_Theta_E_DetNbr.push_back(det);}
   void	SetS1ThetaEStripNbr(UShort_t Nr)	{fS1_Theta_E_StripNbr.push_back(Nr);}
   void	SetS1ThetaEEnergy(Double_t E)		{fS1_Theta_E_Energy.push_back(E);}
   // (Th,T)
   void	SetS1ThetaTDetectorNbr(UShort_t det)	{fS1_Theta_T_DetNbr.push_back(det);}
   void	SetS1ThetaTStripNbr(UShort_t Nr)	{fS1_Theta_T_StripNbr.push_back(Nr);}
   void	SetS1ThetaTTime(Double_t T)		{fS1_Theta_T_Time.push_back(T);}
   // (Ph,E)
   void	SetS1PhiEDetectorNbr(UShort_t det)	{fS1_Phi_E_DetNbr.push_back(det);}
   void	SetS1PhiEStripNbr(UShort_t Nr)		{fS1_Phi_E_StripNbr.push_back(Nr);}
   void	SetS1PhiEEnergy(Double_t E)		{fS1_Phi_E_Energy.push_back(E);}
   // (Ph,T)
   void	SetS1PhiTDetectorNbr(UShort_t det)	{fS1_Phi_T_DetNbr.push_back(det);}
   void	SetS1PhiTStripNbr(UShort_t Nr)		{fS1_Phi_T_StripNbr.push_back(Nr);}
   void	SetS1PhiTTime(Double_t T)		{fS1_Phi_T_Time.push_back(T);}

   ClassDef(TS1Data,2)  // S1Data structure
};

#endif

#ifndef __NanaDATA__
#define __NanaDATA__
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 04/01/2013                                               *
 * Last update    : 02/07/2014                                                         *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of the Nana detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;


class TNanaData : public TObject {

 protected:
   // First Stage LaBr
   // LaBr3
   // Energy
   vector<UShort_t>  fNANA_LaBr3Stage_E_DetectorNbr;
   vector<UShort_t>  fNANA_LaBr3Stage_E_CrystalNbr;
   vector<Double_t>  fNANA_LaBr3Stage_E_Energy;
   //   vector<Double_t>  fNANA_LaBr3Stage_Eff_phpeak;
   // Time
   vector<UShort_t>  fNANA_LaBr3Stage_T_DetectorNbr;
   vector<UShort_t>  fNANA_LaBr3Stage_T_CrystalNbr;
   vector<Double_t>  fNANA_LaBr3Stage_T_Time;


   /*
   // Second Stage CsI
   // CsI
   // Energy
   vector<UShort_t>  fNANA_CsIStage_E_DetectorNbr;
   vector<UShort_t>  fNANA_CsIStage_E_CrystalNbr;
   vector<Double_t>  fNANA_CsIStage_E_Energy;
   //   vector<Double_t>  fNANA_CsIStage_Eff_phpeak;
   // Time
   vector<UShort_t>  fNANA_CsIStage_T_DetectorNbr;
   vector<UShort_t>  fNANA_CsIStage_T_CrystalNbr;
   vector<Double_t>  fNANA_CsIStage_T_Time;
   */

   /*
 private:
   vector<double>	fNana_Energy;
   vector<double>	fNana_Time;
   vector<short>	fNana_Number;
   */

 public:
   TNanaData();
   virtual ~TNanaData();

   void	Clear();
   void	Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           GETTERS           ////////////////////////

   //
   // First stage 
   //
   // (E)
   UShort_t GetNANALaBr3StageEMult()               {
     return fNANA_LaBr3Stage_E_DetectorNbr.size();  // TODO: Maybe change to CrystalNbr for multiplicity
   }
   UShort_t GetNANALaBr3StageEDetectorNbr(Int_t i) {
      return fNANA_LaBr3Stage_E_DetectorNbr.at(i);
   }
   UShort_t GetNANALaBr3StageECrystalNbr(Int_t i)    {
      return fNANA_LaBr3Stage_E_CrystalNbr.at(i);
   }
   Double_t GetNANALaBr3StageEEnergy(Int_t i)      {
      return fNANA_LaBr3Stage_E_Energy.at(i);
   }
   /*
   Double_t GetNANALaBr3StageEffphpeak(Int_t i)      {
      return fNANA_LaBr3Stage_Eff_phpeak.at(i);
   }
   */

   // (T)
   UShort_t GetNANALaBr3StageTMult()               {
      return fNANA_LaBr3Stage_E_DetectorNbr.size();
   }
   UShort_t GetNANALaBr3StageTDetectorNbr(Int_t i) {
      return fNANA_LaBr3Stage_T_DetectorNbr.at(i);
   }
   UShort_t GetNANALaBr3StageTCrystalNbr(Int_t i)    {
      return fNANA_LaBr3Stage_T_CrystalNbr.at(i);
   }
   Double_t GetNANALaBr3StageTTime(Int_t i)      {
      return fNANA_LaBr3Stage_T_Time.at(i);
   }

   /*
   //
   // Second stage (CsI
   //
   // (E)
   UShort_t GetNANACsIStageEMult()               {
      return fNANA_CsIStage_E_DetectorNbr.size();
   }
   UShort_t GetNANACsIStageEDetectorNbr(Int_t i) {
      return fNANA_CsIStage_E_DetectorNbr.at(i);
   }
   UShort_t GetNANACsIStageECrystalNbr(Int_t i)    {
      return fNANA_CsIStage_E_CrystalNbr.at(i);
   }
   Double_t GetNANACsIStageEEnergy(Int_t i)      {
      return fNANA_CsIStage_E_Energy.at(i);
   }
   */
   /*
   Double_t GetNANACsIStageEffphpeak(Int_t i)      {
      return fNANA_CsIStage_Eff_phpeak.at(i);
   }
   */
   /*
   // (T)
   UShort_t GetNANACsIStageTMult()               {
      return fNANA_CsIStage_E_DetectorNbr.size();
   }
   UShort_t GetNANACsIStageTDetectorNbr(Int_t i) {
      return fNANA_CsIStage_T_DetectorNbr.at(i);
   }
   UShort_t GetNANACsIStageTCrystalNbr(Int_t i)    {
      return fNANA_CsIStage_T_CrystalNbr.at(i);
   }
   Double_t GetNANACsIStageTTime(Int_t i)      {
      return fNANA_CsIStage_T_Time.at(i);
   }
   */

   /*
   // (E)
   //double	GetEnergy(int i)		{return fNana_Energy[i];}
   // (T)
   //double	GetTime(int i)			{return fNana_Time[i];}
   // (N)
   int		GetNanaNumber(int i)		{return fNana_Number[i];}   
   double	GetEnergySize()			{return fNana_Energy.size();}
   // (T)
   double	GetTimeSize()			{return fNana_Time.size();}
   // (N)
   int      	GetNanaNumberSize()		{return fNana_Number.size();}
   */
   
   /////////////////////           SETTERS           ////////////////////////

   //
   // First stage 
   //
   // (E)

   void SetNANALaBr3StageEDetectorNbr(UShort_t DetNbr) {
      fNANA_LaBr3Stage_E_DetectorNbr.push_back(DetNbr);
   }
   void SetNANALaBr3StageECrystalNbr(UShort_t CrystalNbr)    {
      fNANA_LaBr3Stage_E_CrystalNbr.push_back(CrystalNbr);
   }
   void SetNANALaBr3StageEEnergy(Double_t Energy)      {
      fNANA_LaBr3Stage_E_Energy.push_back(Energy);
   }
   /*
   void SetNANALaBr3StageEffphpeak(Double_t Energy)      {
      fNANA_LaBr3Stage_Eff_phpeak.push_back(Energy);
   }
   */

   // (T)
   void SetNANALaBr3StageTDetectorNbr(UShort_t DetNbr) {
      fNANA_LaBr3Stage_T_DetectorNbr.push_back(DetNbr);
   }
   void SetNANALaBr3StageTCrystalNbr(UShort_t CrystalNbr)    {
      fNANA_LaBr3Stage_T_CrystalNbr.push_back(CrystalNbr);
   }
   void SetNANALaBr3StageTTime(Double_t Time)      {
      fNANA_LaBr3Stage_T_Time.push_back(Time);
   }

   /*
   //
   // Second stage (CsI
   //
   // (E)
   void SetNANACsIStageEDetectorNbr(UShort_t DetNbr) {
      fNANA_CsIStage_E_DetectorNbr.push_back(DetNbr);
   }
   void SetNANACsIStageECrystalNbr(UShort_t CrystalNbr)    {
      fNANA_CsIStage_E_CrystalNbr.push_back(CrystalNbr);
   }
   void SetNANACsIStageEEnergy(Double_t Energy)      {
      fNANA_CsIStage_E_Energy.push_back(Energy);
   }
   */

   /*
   void SetNANACsIStageEffphpeak(Double_t Energy)      {
      fNANA_CsIStage_Eff_phpeak.push_back(Energy);
   }
   */

   /*
   // (T)
   void SetNANACsIStageTDetectorNbr(UShort_t DetNbr) {
      fNANA_CsIStage_T_DetectorNbr.push_back(DetNbr);
   }
   void SetNANACsIStageTCrystalNbr(UShort_t CrystalNbr)    {
      fNANA_CsIStage_T_CrystalNbr.push_back(CrystalNbr);
   }
   void SetNANACsIStageTTime(Double_t Time)  {
      fNANA_CsIStage_T_Time.push_back(Time);
   }
   */

   /*
   // (E)
   void	SetEnergy(double E)			{fNana_Energy.push_back(E);}
   void	SetTime(double T)			{fNana_Time.push_back(T);}
   void	SetNanaNumber(int N)			{fNana_Number.push_back(N);}
   */
   ClassDef(TNanaData,1)  // NanaData structure
};

#endif

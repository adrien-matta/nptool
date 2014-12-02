#ifndef __FatimaDATA__
#define __FatimaDATA__
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
 *  This class described the raw data of the Fatima detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;


class TFatimaData : public TObject {

 protected:
   // First Stage LaBr
   // LaBr3
   // Energy
   vector<UShort_t>  fFATIMA_LaBr3Stage_E_DetectorNbr;
   vector<UShort_t>  fFATIMA_LaBr3Stage_E_CrystalNbr;
   vector<Double_t>  fFATIMA_LaBr3Stage_E_Energy;
   //   vector<Double_t>  fFATIMA_LaBr3Stage_Eff_phpeak;
   // Time
   vector<UShort_t>  fFATIMA_LaBr3Stage_T_DetectorNbr;
   vector<UShort_t>  fFATIMA_LaBr3Stage_T_CrystalNbr;
   vector<Double_t>  fFATIMA_LaBr3Stage_T_Time;


   /*
   // Second Stage CsI
   // CsI
   // Energy
   vector<UShort_t>  fFATIMA_CsIStage_E_DetectorNbr;
   vector<UShort_t>  fFATIMA_CsIStage_E_CrystalNbr;
   vector<Double_t>  fFATIMA_CsIStage_E_Energy;
   //   vector<Double_t>  fFATIMA_CsIStage_Eff_phpeak;
   // Time
   vector<UShort_t>  fFATIMA_CsIStage_T_DetectorNbr;
   vector<UShort_t>  fFATIMA_CsIStage_T_CrystalNbr;
   vector<Double_t>  fFATIMA_CsIStage_T_Time;
   */

   /*
 private:
   vector<double>	fFatima_Energy;
   vector<double>	fFatima_Time;
   vector<short>	fFatima_Number;
   */

 public:
   TFatimaData();
   virtual ~TFatimaData();

   void	Clear();
   void	Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           GETTERS           ////////////////////////

   //
   // First stage 
   //
   // (E)
   UShort_t GetFATIMALaBr3StageEMult()               {
     return fFATIMA_LaBr3Stage_E_DetectorNbr.size();  // TODO: Maybe change to CrystalNbr for multiplicity
   }
   UShort_t GetFATIMALaBr3StageEDetectorNbr(Int_t i) {
      return fFATIMA_LaBr3Stage_E_DetectorNbr.at(i);
   }
   UShort_t GetFATIMALaBr3StageECrystalNbr(Int_t i)    {
      return fFATIMA_LaBr3Stage_E_CrystalNbr.at(i);
   }
   Double_t GetFATIMALaBr3StageEEnergy(Int_t i)      {
      return fFATIMA_LaBr3Stage_E_Energy.at(i);
   }
   /*
   Double_t GetFATIMALaBr3StageEffphpeak(Int_t i)      {
      return fFATIMA_LaBr3Stage_Eff_phpeak.at(i);
   }
   */

   // (T)
   UShort_t GetFATIMALaBr3StageTMult()               {
      return fFATIMA_LaBr3Stage_E_DetectorNbr.size();
   }
   UShort_t GetFATIMALaBr3StageTDetectorNbr(Int_t i) {
      return fFATIMA_LaBr3Stage_T_DetectorNbr.at(i);
   }
   UShort_t GetFATIMALaBr3StageTCrystalNbr(Int_t i)    {
      return fFATIMA_LaBr3Stage_T_CrystalNbr.at(i);
   }
   Double_t GetFATIMALaBr3StageTTime(Int_t i)      {
      return fFATIMA_LaBr3Stage_T_Time.at(i);
   }

   /*
   //
   // Second stage (CsI
   //
   // (E)
   UShort_t GetFATIMACsIStageEMult()               {
      return fFATIMA_CsIStage_E_DetectorNbr.size();
   }
   UShort_t GetFATIMACsIStageEDetectorNbr(Int_t i) {
      return fFATIMA_CsIStage_E_DetectorNbr.at(i);
   }
   UShort_t GetFATIMACsIStageECrystalNbr(Int_t i)    {
      return fFATIMA_CsIStage_E_CrystalNbr.at(i);
   }
   Double_t GetFATIMACsIStageEEnergy(Int_t i)      {
      return fFATIMA_CsIStage_E_Energy.at(i);
   }
   */
   /*
   Double_t GetFATIMACsIStageEffphpeak(Int_t i)      {
      return fFATIMA_CsIStage_Eff_phpeak.at(i);
   }
   */
   /*
   // (T)
   UShort_t GetFATIMACsIStageTMult()               {
      return fFATIMA_CsIStage_E_DetectorNbr.size();
   }
   UShort_t GetFATIMACsIStageTDetectorNbr(Int_t i) {
      return fFATIMA_CsIStage_T_DetectorNbr.at(i);
   }
   UShort_t GetFATIMACsIStageTCrystalNbr(Int_t i)    {
      return fFATIMA_CsIStage_T_CrystalNbr.at(i);
   }
   Double_t GetFATIMACsIStageTTime(Int_t i)      {
      return fFATIMA_CsIStage_T_Time.at(i);
   }
   */

   /*
   // (E)
   //double	GetEnergy(int i)		{return fFatima_Energy[i];}
   // (T)
   //double	GetTime(int i)			{return fFatima_Time[i];}
   // (N)
   int		GetFatimaNumber(int i)		{return fFatima_Number[i];}   
   double	GetEnergySize()			{return fFatima_Energy.size();}
   // (T)
   double	GetTimeSize()			{return fFatima_Time.size();}
   // (N)
   int      	GetFatimaNumberSize()		{return fFatima_Number.size();}
   */
   
   /////////////////////           SETTERS           ////////////////////////

   //
   // First stage 
   //
   // (E)

   void SetFATIMALaBr3StageEDetectorNbr(UShort_t DetNbr) {
      fFATIMA_LaBr3Stage_E_DetectorNbr.push_back(DetNbr);
   }
   void SetFATIMALaBr3StageECrystalNbr(UShort_t CrystalNbr)    {
      fFATIMA_LaBr3Stage_E_CrystalNbr.push_back(CrystalNbr);
   }
   void SetFATIMALaBr3StageEEnergy(Double_t Energy)      {
      fFATIMA_LaBr3Stage_E_Energy.push_back(Energy);
   }
   /*
   void SetFATIMALaBr3StageEffphpeak(Double_t Energy)      {
      fFATIMA_LaBr3Stage_Eff_phpeak.push_back(Energy);
   }
   */

   // (T)
   void SetFATIMALaBr3StageTDetectorNbr(UShort_t DetNbr) {
      fFATIMA_LaBr3Stage_T_DetectorNbr.push_back(DetNbr);
   }
   void SetFATIMALaBr3StageTCrystalNbr(UShort_t CrystalNbr)    {
      fFATIMA_LaBr3Stage_T_CrystalNbr.push_back(CrystalNbr);
   }
   void SetFATIMALaBr3StageTTime(Double_t Time)      {
      fFATIMA_LaBr3Stage_T_Time.push_back(Time);
   }

   /*
   //
   // Second stage (CsI
   //
   // (E)
   void SetFATIMACsIStageEDetectorNbr(UShort_t DetNbr) {
      fFATIMA_CsIStage_E_DetectorNbr.push_back(DetNbr);
   }
   void SetFATIMACsIStageECrystalNbr(UShort_t CrystalNbr)    {
      fFATIMA_CsIStage_E_CrystalNbr.push_back(CrystalNbr);
   }
   void SetFATIMACsIStageEEnergy(Double_t Energy)      {
      fFATIMA_CsIStage_E_Energy.push_back(Energy);
   }
   */

   /*
   void SetFATIMACsIStageEffphpeak(Double_t Energy)      {
      fFATIMA_CsIStage_Eff_phpeak.push_back(Energy);
   }
   */

   /*
   // (T)
   void SetFATIMACsIStageTDetectorNbr(UShort_t DetNbr) {
      fFATIMA_CsIStage_T_DetectorNbr.push_back(DetNbr);
   }
   void SetFATIMACsIStageTCrystalNbr(UShort_t CrystalNbr)    {
      fFATIMA_CsIStage_T_CrystalNbr.push_back(CrystalNbr);
   }
   void SetFATIMACsIStageTTime(Double_t Time)  {
      fFATIMA_CsIStage_T_Time.push_back(Time);
   }
   */

   /*
   // (E)
   void	SetEnergy(double E)			{fFatima_Energy.push_back(E);}
   void	SetTime(double T)			{fFatima_Time.push_back(T);}
   void	SetFatimaNumber(int N)			{fFatima_Number.push_back(N);}
   */
   ClassDef(TFatimaData,1)  // FatimaData structure
};

#endif

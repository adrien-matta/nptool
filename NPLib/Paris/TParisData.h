#ifndef __ParisDATA__
#define __ParisDATA__
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 04/12/2009                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of the Paris detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;


class TParisData : public TObject {

 protected:
   // First Stage LaBr
   // LaBr3
   // Energy
   vector<UShort_t>  fPARIS_LaBr3Stage_E_DetectorNbr;
   vector<UShort_t>  fPARIS_LaBr3Stage_E_CrystalNbr;
   vector<Double_t>  fPARIS_LaBr3Stage_E_Energy;
   vector<Double_t>  fPARIS_LaBr3Stage_Eff_phpeak;
   // Time
   vector<UShort_t>  fPARIS_LaBr3Stage_T_DetectorNbr;
   vector<UShort_t>  fPARIS_LaBr3Stage_T_CrystalNbr;
   vector<Double_t>  fPARIS_LaBr3Stage_T_Time;


   // Second Stage CsI
   // CsI
   // Energy
   vector<UShort_t>  fPARIS_CsIStage_E_DetectorNbr;
   vector<UShort_t>  fPARIS_CsIStage_E_CrystalNbr;
   vector<Double_t>  fPARIS_CsIStage_E_Energy;
   vector<Double_t>  fPARIS_CsIStage_Eff_phpeak;
   // Time
   vector<UShort_t>  fPARIS_CsIStage_T_DetectorNbr;
   vector<UShort_t>  fPARIS_CsIStage_T_CrystalNbr;
   vector<Double_t>  fPARIS_CsIStage_T_Time;

   /*
 private:
   vector<double>   fParis_Energy;
   vector<double>   fParis_Time;
   vector<short>   fParis_Number;
   */

 public:
   TParisData();
   virtual ~TParisData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////

   //
   // First stage 
   //
   // (E)
   UShort_t GetPARISLaBr3StageEMult()               {
     return fPARIS_LaBr3Stage_E_DetectorNbr.size();  // TODO: Maybe change to CrystalNbr for multiplicity
   }
   UShort_t GetPARISLaBr3StageEDetectorNbr(Int_t i) {
      return fPARIS_LaBr3Stage_E_DetectorNbr.at(i);
   }
   UShort_t GetPARISLaBr3StageECrystalNbr(Int_t i)    {
      return fPARIS_LaBr3Stage_E_CrystalNbr.at(i);
   }
   Double_t GetPARISLaBr3StageEEnergy(Int_t i)      {
      return fPARIS_LaBr3Stage_E_Energy.at(i);
   }
   Double_t GetPARISLaBr3StageEffphpeak(Int_t i)      {
      return fPARIS_LaBr3Stage_Eff_phpeak.at(i);
   }
   

   // (T)
   UShort_t GetPARISLaBr3StageTMult()               {
      return fPARIS_LaBr3Stage_E_DetectorNbr.size();
   }
   UShort_t GetPARISLaBr3StageTDetectorNbr(Int_t i) {
      return fPARIS_LaBr3Stage_T_DetectorNbr.at(i);
   }
   UShort_t GetPARISLaBr3StageTCrystalNbr(Int_t i)    {
      return fPARIS_LaBr3Stage_T_CrystalNbr.at(i);
   }
   Double_t GetPARISLaBr3StageTTime(Int_t i)      {
      return fPARIS_LaBr3Stage_T_Time.at(i);
   }

   //
   // Second stage (CsI
   //
   // (E)
   UShort_t GetPARISCsIStageEMult()               {
      return fPARIS_CsIStage_E_DetectorNbr.size();
   }
   UShort_t GetPARISCsIStageEDetectorNbr(Int_t i) {
      return fPARIS_CsIStage_E_DetectorNbr.at(i);
   }
   UShort_t GetPARISCsIStageECrystalNbr(Int_t i)    {
      return fPARIS_CsIStage_E_CrystalNbr.at(i);
   }
   Double_t GetPARISCsIStageEEnergy(Int_t i)      {
      return fPARIS_CsIStage_E_Energy.at(i);
   }
   Double_t GetPARISCsIStageEffphpeak(Int_t i)      {
      return fPARIS_CsIStage_Eff_phpeak.at(i);
   }
   
   // (T)
   UShort_t GetPARISCsIStageTMult()               {
      return fPARIS_CsIStage_E_DetectorNbr.size();
   }
   UShort_t GetPARISCsIStageTDetectorNbr(Int_t i) {
      return fPARIS_CsIStage_T_DetectorNbr.at(i);
   }
   UShort_t GetPARISCsIStageTCrystalNbr(Int_t i)    {
      return fPARIS_CsIStage_T_CrystalNbr.at(i);
   }
   Double_t GetPARISCsIStageTTime(Int_t i)      {
      return fPARIS_CsIStage_T_Time.at(i);
   }

   /*
   // (E)
   //double   GetEnergy(int i)      {return fParis_Energy[i];}
   // (T)
   //double   GetTime(int i)         {return fParis_Time[i];}
   // (N)
   int      GetParisNumber(int i)      {return fParis_Number[i];}   
   double   GetEnergySize()         {return fParis_Energy.size();}
   // (T)
   double   GetTimeSize()         {return fParis_Time.size();}
   // (N)
   int         GetParisNumberSize()      {return fParis_Number.size();}
   */
   
   /////////////////////           SETTERS           ////////////////////////

   //
   // First stage 
   //
   // (E)

   void SetPARISLaBr3StageEDetectorNbr(UShort_t DetNbr) {
      fPARIS_LaBr3Stage_E_DetectorNbr.push_back(DetNbr);
   }
   void SetPARISLaBr3StageECrystalNbr(UShort_t CrystalNbr)    {
      fPARIS_LaBr3Stage_E_CrystalNbr.push_back(CrystalNbr);
   }
   void SetPARISLaBr3StageEEnergy(Double_t Energy)      {
      fPARIS_LaBr3Stage_E_Energy.push_back(Energy);
   }
   void SetPARISLaBr3StageEffphpeak(Double_t Energy)      {
      fPARIS_LaBr3Stage_Eff_phpeak.push_back(Energy);
   }
   

   // (T)
   void SetPARISLaBr3StageTDetectorNbr(UShort_t DetNbr) {
      fPARIS_LaBr3Stage_T_DetectorNbr.push_back(DetNbr);
   }
   void SetPARISLaBr3StageTCrystalNbr(UShort_t CrystalNbr)    {
      fPARIS_LaBr3Stage_T_CrystalNbr.push_back(CrystalNbr);
   }
   void SetPARISLaBr3StageTTime(Double_t Time)      {
      fPARIS_LaBr3Stage_T_Time.push_back(Time);
   }

   //
   // Second stage (CsI
   //
   // (E)
   void SetPARISCsIStageEDetectorNbr(UShort_t DetNbr) {
      fPARIS_CsIStage_E_DetectorNbr.push_back(DetNbr);
   }
   void SetPARISCsIStageECrystalNbr(UShort_t CrystalNbr)    {
      fPARIS_CsIStage_E_CrystalNbr.push_back(CrystalNbr);
   }
   void SetPARISCsIStageEEnergy(Double_t Energy)      {
      fPARIS_CsIStage_E_Energy.push_back(Energy);
   }
   void SetPARISCsIStageEffphpeak(Double_t Energy)      {
      fPARIS_CsIStage_Eff_phpeak.push_back(Energy);
   }
   
   // (T)
   void SetPARISCsIStageTDetectorNbr(UShort_t DetNbr) {
      fPARIS_CsIStage_T_DetectorNbr.push_back(DetNbr);
   }
   void SetPARISCsIStageTCrystalNbr(UShort_t CrystalNbr)    {
      fPARIS_CsIStage_T_CrystalNbr.push_back(CrystalNbr);
   }
   void SetPARISCsIStageTTime(Double_t Time)  {
      fPARIS_CsIStage_T_Time.push_back(Time);
   }


   /*
   // (E)
   void   SetEnergy(double E)         {fParis_Energy.push_back(E);}
   void   SetTime(double T)         {fParis_Time.push_back(T);}
   void   SetParisNumber(int N)         {fParis_Number.push_back(N);}
   */
   ClassDef(TParisData,1)  // ParisData structure
};

#endif

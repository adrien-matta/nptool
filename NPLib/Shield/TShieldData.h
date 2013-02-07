#ifndef __ShieldDATA__
#define __ShieldDATA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
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
 *  This class described the raw data of the shield of PARIS detector        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <vector>

#include "TObject.h"
using namespace std ;


class TShieldData : public TObject {

 protected:
 
   // Energy
   vector<UShort_t>  fPARIS_CsIShield_E_DetectorNbr;
   vector<UShort_t>  fPARIS_CsIShield_E_CrystalNbr;
   vector<Double_t>  fPARIS_CsIShield_E_Energy;
   vector<Double_t>  fPARIS_CsIShield_Eff_phpeak;
   // Time
   vector<UShort_t>  fPARIS_CsIShield_T_DetectorNbr;
   vector<UShort_t>  fPARIS_CsIShield_T_CrystalNbr;
   vector<Double_t>  fPARIS_CsIShield_T_Time;


   /*
 private:
   vector<double>   fParis_Energy;
   vector<double>   fParis_Time;
   vector<short>   fParis_Number;
   */

 public:
   TShieldData();
   virtual ~TShieldData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////

   //
   // First stage 
   //
   // (E)
   UShort_t GetPARISCsIShieldEMult()               {
     return fPARIS_CsIShield_E_DetectorNbr.size();  // TODO: Maybe change to CrystalNbr for multiplicity
   }
   UShort_t GetPARISCsIShieldEDetectorNbr(Int_t i) {
      return fPARIS_CsIShield_E_DetectorNbr.at(i);
   }
   UShort_t GetPARISCsIShieldECrystalNbr(Int_t i)    {
      return fPARIS_CsIShield_E_CrystalNbr.at(i);
   }
   Double_t GetPARISCsIShieldEEnergy(Int_t i)      {
      return fPARIS_CsIShield_E_Energy.at(i);
   }
   Double_t GetPARISCsIShieldEffphpeak(Int_t i)      {
      return fPARIS_CsIShield_Eff_phpeak.at(i);
   }
   

   // (T)
   UShort_t GetPARISCsIShieldTMult()               {
      return fPARIS_CsIShield_E_DetectorNbr.size();
   }
   UShort_t GetPARISCsIShieldTDetectorNbr(Int_t i) {
      return fPARIS_CsIShield_T_DetectorNbr.at(i);
   }
   UShort_t GetPARISCsIShieldTCrystalNbr(Int_t i)    {
      return fPARIS_CsIShield_T_CrystalNbr.at(i);
   }
   Double_t GetPARISCsIShieldTTime(Int_t i)      {
      return fPARIS_CsIShield_T_Time.at(i);
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

   void SetPARISCsIShieldEDetectorNbr(UShort_t DetNbr) {
      fPARIS_CsIShield_E_DetectorNbr.push_back(DetNbr);
   }
   void SetPARISCsIShieldECrystalNbr(UShort_t CrystalNbr)    {
      fPARIS_CsIShield_E_CrystalNbr.push_back(CrystalNbr);
   }
   void SetPARISCsIShieldEEnergy(Double_t Energy)      {
      fPARIS_CsIShield_E_Energy.push_back(Energy);
   }
   void SetPARISCsIShieldEffphpeak(Double_t Energy)      {
      fPARIS_CsIShield_Eff_phpeak.push_back(Energy);
   }
   

   // (T)
   void SetPARISCsIShieldTDetectorNbr(UShort_t DetNbr) {
      fPARIS_CsIShield_T_DetectorNbr.push_back(DetNbr);
   }
   void SetPARISCsIShieldTCrystalNbr(UShort_t CrystalNbr)    {
      fPARIS_CsIShield_T_CrystalNbr.push_back(CrystalNbr);
   }
   void SetPARISCsIShieldTTime(Double_t Time)      {
      fPARIS_CsIShield_T_Time.push_back(Time);
   }


   /*
   // (E)
   void   SetEnergy(double E)         {fParis_Energy.push_back(E);}
   void   SetTime(double T)         {fParis_Time.push_back(T);}
   void   SetParisNumber(int N)         {fParis_Number.push_back(N);}
   */
   ClassDef(TShieldData,1)  // ParisData structure
};

#endif

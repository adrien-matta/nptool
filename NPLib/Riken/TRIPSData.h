#ifndef __RIPSDATA__
#define __RIPSDATA__

#include <vector>
#include <iostream>
#include "TObject.h"
using namespace std ;

class TRIPSData : public TObject {
 private:
 
   // PPAC
   int RunNumber;
   int EventNumber;
   vector<int>    fRIPS_PPAC_N;
   vector<double> fRIPS_PPAC_X;
   vector<double> fRIPS_PPAC_Y;
   vector<double> fRIPS_PPAC_T;
   
   // Plastic
   vector<int>    fRIPS_Plastic_N;
   vector<double> fRIPS_Plastic_E;
   vector<double> fRIPS_Plastic_EL;
   vector<double> fRIPS_Plastic_ER;
   vector<double> fRIPS_Plastic_TL;
   vector<double> fRIPS_Plastic_TR;
   
   // RF
   vector<int>    fRIPS_RF_N;
   vector<double> fRIPS_RF_T;
   
 public:
   TRIPSData();
   virtual ~TRIPSData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   int GetRunNumber()   {return RunNumber;}
   int GetEventNumber() {return EventNumber;}
   
   // PPAC
   int      GetMultPPAC()    {return fRIPS_PPAC_N.size();}
   int      GetPPACN(int i)  {return fRIPS_PPAC_N[i];}
   double   GetPPACX(int i)  {return fRIPS_PPAC_X[i];}
   double   GetPPACY(int i)  {return fRIPS_PPAC_Y[i];}
   double   GetPPACT(int i)  {return fRIPS_PPAC_T[i];}
   
   // Plastic
   int      GetMultPlastic()     {return fRIPS_Plastic_N.size();}
   int      GetPlasticN(int i)   {return fRIPS_Plastic_N[i];}
   double   GetPlasticE(int i)   {return fRIPS_Plastic_E[i];}
   double   GetPlasticER(int i)  {return fRIPS_Plastic_ER[i];}
   double   GetPlasticEL(int i)  {return fRIPS_Plastic_EL[i];}
   double   GetPlasticTL(int i)  {return fRIPS_Plastic_TL[i];}
   double   GetPlasticTR(int i)  {return fRIPS_Plastic_TR[i];}
   
   // RF
   int      GetMultRF()     {return fRIPS_RF_N.size();}
   double   GetRFN(int i)   {return fRIPS_RF_N[i];}
   double   GetRFT(int i)   {return fRIPS_RF_T[i];}
   
   /////////////////////           SETTERS           ////////////////////////
   void  SetRunNumber(int value)   {RunNumber=value;}
   void  SetEventNumber(int value) {EventNumber=value;}
   
   // PPAC
   void  SetPPACN(int value)     {fRIPS_PPAC_N.push_back(value);}
   void  SetPPACX(double value)  {fRIPS_PPAC_X.push_back(value);}
   void  SetPPACY(double value)  {fRIPS_PPAC_Y.push_back(value);}
   void  SetPPACT(double value)  {fRIPS_PPAC_T.push_back(value);}
   
   // Plastic
   void  SetPlasticN(int value)      {fRIPS_Plastic_N.push_back(value);}
   void  SetPlasticE(double value)   {fRIPS_Plastic_E.push_back(value);}
   void  SetPlasticER(double value)  {fRIPS_Plastic_ER.push_back(value);}
   void  SetPlasticEL(double value)  {fRIPS_Plastic_EL.push_back(value);}
   void  SetPlasticTL(double value)  {fRIPS_Plastic_TL.push_back(value);}
   void  SetPlasticTR(double value)  {fRIPS_Plastic_TR.push_back(value);}
   
   // RF
   void  SetRFN(int value)      {fRIPS_RF_N.push_back(value);}
   void  SetRFT(double value)   {fRIPS_RF_T.push_back(value);}
   //
   ClassDef(TRIPSData,1)  // PlasticData structure
};

#endif

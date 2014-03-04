#ifndef __SIRESDATA__
#define __SIRESDATA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:    contact address:                                      *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;


class TSiResData : public TObject {
 private:
   // Energy 
   vector<short>   fSiRes_E_Number;
   vector<short>   fSiRes_E_Channel;
   vector<double>  fSiRes_E_Energy;   
   // Time
   vector<short>   fSiRes_T_Number;
   vector<double>  fSiRes_T_Time;
   vector<double>  fSiRes_E_EnergyBack_Number;
   vector<double>  fSiRes_E_EnergyBack;
   
 public:
   TSiResData();
   virtual ~TSiResData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // Energy
   unsigned int   GetEnergyMult()   {return fSiRes_E_Number.size();}
   int            GetEChannel(int i) {return fSiRes_E_Channel[i];}
   int            GetENumber(int i) {return fSiRes_E_Number[i];}
   double         GetEEnergy(int i) {return fSiRes_E_Energy[i];}
   // Time 
   unsigned int   GetTimeMult()     {return fSiRes_T_Number.size();}
   int            GetTNumber(int i) {return fSiRes_T_Number[i];}
   double         GetTTime(int i)   {return fSiRes_T_Time[i];}
   double         GetEEnergyBack(int i)   	{return fSiRes_E_EnergyBack[i];}
   double         GetEEnergyBackNumber(int i)   {return fSiRes_E_EnergyBack_Number[i];}
   double         GetEnergyBackMult()   	{return fSiRes_E_EnergyBack.size();}

   /////////////////////           SETTERS           ////////////////////////
   // Energy
   void     SetENumber(int N)    {fSiRes_E_Number.push_back(N);}
   void     SetEChannel(int channel)    {fSiRes_E_Number.push_back(channel);}
   void     SetEEnergy(double E) {fSiRes_E_Energy.push_back(E);}
   // time
   void     SetTNumber(int N)    {fSiRes_T_Number.push_back(N);}
   void     SetTTime(double T)   {fSiRes_T_Time.push_back(T);}
   void     SetEEnergyBack(double T)   {fSiRes_E_EnergyBack.push_back(T);}

   ClassDef(TSiResData,1)  // SiResData structure
};

#endif

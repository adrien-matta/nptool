#ifndef __SIRESDATA__
#define __SIRESDATA__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
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
#include <iostream>

#include "TObject.h"
using namespace std ;


class TSiResData : public TObject {
 private:
   // Energy 
   vector<int>     fSiRes_E_Number;
   vector<int>     fSiRes_E_Channel;
   vector<double>  fSiRes_E_Energy;   
   // Time
   vector<int>     fSiRes_T_Number;
   vector<double>  fSiRes_T_Time;
   vector<int>     fSiRes_E_EnergyBack_Number;
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
   int            GetEChannelNumber(int i) {return fSiRes_E_Channel[i];}
   int            GetEDetectorNumber(int i) {return fSiRes_E_Number[i];}
   double         GetEEnergy(int i) {return fSiRes_E_Energy[i];}
   // Time 
   unsigned int   GetTimeMult()     {return fSiRes_T_Number.size();}
   int            GetTDetectorNumber(int i) {return fSiRes_T_Number[i];}
   double         GetTTime(int i)   {return fSiRes_T_Time[i];}
   double         GetEEnergyBack(int i)   	{return fSiRes_E_EnergyBack[i];}
   int            GetEEnergyBackDetectorNumber(int i)   {return fSiRes_E_EnergyBack_Number[i];}
   double         GetEEnergyBackMult()   	{return fSiRes_E_EnergyBack.size();}

   /////////////////////           SETTERS           ////////////////////////
   // Energy
   void     SetEDetectorNumber(int N)    	{fSiRes_E_Number.push_back(N);}
   void     SetEChannelNumber(int channel)    	{fSiRes_E_Channel.push_back(channel);}
   void     SetEEnergy(double E) 		{fSiRes_E_Energy.push_back(E);}
   // time
   void     SetTDetectorNumber(int N)    	{fSiRes_T_Number.push_back(N);}
   void     SetTTime(double T)   		{fSiRes_T_Time.push_back(T);}
   void     SetEEnergyBack(double E)   		{fSiRes_E_EnergyBack.push_back(E);}
   void     SetEEnergyBackDetectorNumber(int N) {fSiRes_E_EnergyBack_Number.push_back(N);}

   ClassDef(TSiResData,1)  // SiResData structure
};

#endif

#ifndef __SILIDATA__
#define __SILIDATA__
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


class TSiLiData : public TObject {
 private:
   // Energy 
   vector<short>   fSiLi_E_Number;
   vector<double>  fSiLi_E_Energy;   
   // Time
   vector<short>   fSiLi_T_Number;
   vector<double>  fSiLi_T_Time;
   
 public:
   TSiLiData();
   virtual ~TSiLiData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // Energy
   unsigned int   GetEnergyMult()   {return fSiLi_E_Number.size();}
   int            GetENumber(int i)  {return fSiLi_E_Number[i];}
   double         GetEEnergy(int i) {return fSiLi_E_Energy[i];}
   // Time 
   unsigned int   GetTimeMult()     {return fSiLi_T_Number.size();}
   int            GetTNumber(int i) {return fSiLi_T_Number[i];}
   double         GetTTime(int i)   {return fSiLi_T_Time[i];}

   /////////////////////           SETTERS           ////////////////////////
   // Energy
   void     SetENumber(int N)    {fSiLi_E_Number.push_back(N);}
   void     SetEEnergy(double E) {fSiLi_E_Energy.push_back(E);}
   // time
   void     SetTNumber(int N)    {fSiLi_T_Number.push_back(N);}
   void     SetTTime(double T)   {fSiLi_T_Time.push_back(T);}

   ClassDef(TSiLiData,1)  // SiLiData structure
};

#endif

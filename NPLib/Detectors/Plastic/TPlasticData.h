#ifndef __PLASTICDATA__
#define __PLASTICDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
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


class TPlasticData : public TObject {
 private:
   // ADC
   vector<double>  fPlastic_Energy;   
   vector<double>  fPlastic_Time ;
   vector<short>   fPlastic_Number ;
   
 public:
   TPlasticData();
   virtual ~TPlasticData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   double   GetEnergy(int i) { return fPlastic_Energy[i] ;}
   // (T)
   double   GetTime(int i)   { return fPlastic_Time[i] ;}
   // (N)
   int    GetPlasticNumber(int i) { return fPlastic_Number[i] ;}
   
   //Mult
   // E
   double   GetEnergyMult() { return fPlastic_Energy.size() ;}
   // (T)
   double   GetTimeMult()   { return fPlastic_Time.size() ;}
   // (N)
   int      GetPlasticNumberMult() { return fPlastic_Number.size() ;}
   
   /////////////////////           SETTERS           ////////////////////////
   // (E)
   void   SetEnergy(double E) { fPlastic_Energy.push_back(E) ;}
   void   SetTime(double T)   { fPlastic_Time.push_back(T) ;}
   void   SetPlasticNumber(int N) { fPlastic_Number.push_back(N) ;}
   //
   ClassDef(TPlasticData,1)  // PlasticData structure
};

#endif

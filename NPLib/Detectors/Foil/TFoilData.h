#ifndef __FoilDATA__
#define __FoilDATA__
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


class TFoilData : public TObject {
 private:
   // ADC
   vector<double>  fFoil_Energy;   
   vector<double>  fFoil_Time ;
   vector<short>   fFoil_Number ;
   
 public:
   TFoilData();
   virtual ~TFoilData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   inline double   GetEnergy(const int& i) const { return fFoil_Energy[i] ;}
   // (T)
   inline double   GetTime(const int& i)   const { return fFoil_Time[i] ;}
   // (N)
   inline int      GetFoilNumber(const int& i) const { return fFoil_Number[i] ;}
   
   //Mult
   // E
   inline double   GetEnergyMult() const { return fFoil_Energy.size() ;}
   // (T)
   inline double   GetTimeMult()   const { return fFoil_Time.size() ;}
   // (N)
   inline int      GetFoilNumberMult() const { return fFoil_Number.size() ;}
   
   /////////////////////           SETTERS           ////////////////////////
   // (E)
   inline void   SetEnergy(const int& N, const double& E)     { fFoil_Number.push_back(N); fFoil_Energy.push_back(E) ;}
   inline void   SetTime(const int& N, const double&  T)      { fFoil_Number.push_back(N); fFoil_Time.push_back(T) ;}
   inline void   SetFoilNumber(const int& N) { fFoil_Number.push_back(N) ;}
   //
   ClassDef(TFoilData,1)  // PlasticData structure
};

#endif

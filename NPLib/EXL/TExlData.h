#ifndef __EXLDATA__
#define __EXLDATA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $GANIL2ROOT/Licence                           *
 * For the list of contributors see $GANIL2ROOT/README/CREDITS               *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre  contact address: lefebvrl@ipno.in2p3.fr     *
 *                    							     *
 *                                                                           *
 * Creation Date  : June 2011                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds EXL Raw data                                 *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#include <vector>

#include "TObject.h"
using namespace std ;


class TExlData : public TObject {
 private:
   // ADC
   vector<double>  fExl_Energy;   
   vector<double>  fExl_Time;   
   vector<short>   fExl_Number ;
   
 public:
   TExlData();
   virtual ~TExlData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   double   GetEnergy(int i) { return fExl_Energy[i] ;}
   // (T)
   double   GetTime(int i) { return fExl_Time[i] ;}
   // (N)
   int    GetExlNumber(int i) { return fExl_Number[i] ;}
   
   //Mult
   // E
   double   GetEnergyMult() { return fExl_Energy.size() ;}
   // T
   double   GetTimeMult() { return fExl_Time.size() ;}
   // (N)
   int      GetExlNumberMult() { return fExl_Number.size() ;}
   
   /////////////////////           SETTERS           ////////////////////////
   // (E)
   void   SetEnergy(double E) { fExl_Energy.push_back(E) ;}
   // (T)
   void   SetTime(double T) { fExl_Time.push_back(T) ;}
   //(N)
   void   SetExlNumber(int N) { fExl_Number.push_back(N) ;}
   //
   ClassDef(TExlData,1)  // ExlData structure
};

#endif

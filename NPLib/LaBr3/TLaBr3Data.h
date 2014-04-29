#ifndef __LABR3DATA__
#define __LABR3DATA__
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

#include "TObject.h"
using namespace std ;


class TLaBr3Data : public TObject {
 private:
   // Energy 
   vector<short>   fLaBr3_E_Number;
   vector<double>  fLaBr3_E_Energy;   
   // Time
   vector<short>   fLaBr3_T_Number;
   vector<double>  fLaBr3_T_Time;
   
 public:
   TLaBr3Data();
   virtual ~TLaBr3Data();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // Energy
   unsigned int   GetEnergyMult()   {return fLaBr3_E_Number.size();}
   int            GetENumber(int i) {return fLaBr3_E_Number[i];}
   double         GetEEnergy(int i) {return fLaBr3_E_Energy[i];}
   // Time 
   unsigned int   GetTimeMult()     {return fLaBr3_T_Number.size();}
   int            GetTNumber(int i) {return fLaBr3_T_Number[i];}
   double         GetTTime(int i)   {return fLaBr3_T_Time[i];}

   /////////////////////           SETTERS           ////////////////////////
   // Energy
   void     SetENumber(int N)    {fLaBr3_E_Number.push_back(N);}
   void     SetEEnergy(double E) {fLaBr3_E_Energy.push_back(E);}
   // time
   void     SetTNumber(int N)    {fLaBr3_T_Number.push_back(N);}
   void     SetTTime(double T)   {fLaBr3_T_Time.push_back(T);}

   ClassDef(TLaBr3Data,1)  // LaBr3Data structure
};

#endif

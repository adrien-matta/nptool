#ifndef __LABR3DATA__
#define __LABR3DATA__
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


class TLaBr3Data : public TObject {
 private:
   // ADC
   vector<double>  fLaBr3_Energy;   
   vector<double>  fLaBr3_Time ;
   vector<short>   fLaBr3_Number ;
   
 public:
   TLaBr3Data();
   virtual ~TLaBr3Data();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   double   GetEnergy(int i)        {return fLaBr3_Energy[i];}
   // (T)
   double   GetTime(int i)          {return fLaBr3_Time[i];}
   // (N)
   int      GetLaBr3Number(int i)   {return fLaBr3_Number[i];}

   //Mult
   // E
   double   GetEnergyMult()      {return fLaBr3_Energy.size();}
   // (T)
   double   GetTimeMult()        {return fLaBr3_Time.size();}
   // (N)
   int      GetLaBr3NumberMult() {return fLaBr3_Number.size();}

   /////////////////////           SETTERS           ////////////////////////
   // (E)
   void     SetEnergy(double E)     {fLaBr3_Energy.push_back(E);}
   void     SetTime(double T)       {fLaBr3_Time.push_back(T);}
   void     SetLaBr3Number(int N)   {fLaBr3_Number.push_back(N);}
   //
   ClassDef(TLaBr3Data,1)  // LaBr3Data structure
};

#endif

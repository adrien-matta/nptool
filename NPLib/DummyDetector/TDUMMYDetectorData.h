#ifndef __DUMMYDetectorDATA__
#define __DUMMYDetectorDATA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : October 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of a very simple DUMMY detector        *
 *   Use it as a template for your own detector!                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;

class TDUMMYDetectorData : public TObject {
 private:
   // ADC
   vector<double> fDUMMYDetector_Energy;   
   vector<double> fDUMMYDetector_Time;
   vector<short>  fDUMMYDetector_Number;
   
 public:
   TDUMMYDetectorData();
   virtual ~TDUMMYDetectorData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   double   GetEnergy(int i)              {return fDUMMYDetector_Energy[i];}
   // (T)
   double   GetTime(int i)                {return fDUMMYDetector_Time[i];}
   // (N)
   int      GetDUMMYDetectorNumber(int i) {return fDUMMYDetector_Number[i];}
   
   
   double   GetEnergySize()               {return fDUMMYDetector_Energy.size();}
   // (T)
   double   GetTimeSize()                 {return fDUMMYDetector_Time.size();}
   // (N)
   int      GetDUMMYDetectorNumberSize()  {return fDUMMYDetector_Number.size();}
   
   
   /////////////////////           SETTERS           ////////////////////////
   // (E)
   void   SetEnergy(double E)             {fDUMMYDetector_Energy.push_back(E);}
   void   SetTime(double T)               {fDUMMYDetector_Time.push_back(T);}
   void   SetDUMMYDetectorNumber(int N)   {fDUMMYDetector_Number.push_back(N);}
   
   //
   ClassDef(TDUMMYDetectorData,1)  // DUMMYDetectorData structure
};

#endif

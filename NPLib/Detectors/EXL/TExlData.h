#ifndef __EXLDATA__
#define __EXLDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
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
   vector<short>   fExl_Crystal;

 public:
   TExlData();
   virtual ~TExlData();

   void   Clear();
   void   Clear(const Option_t*) {};
   void   Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   inline double   GetEnergy(const int& i)        const { return fExl_Energy[i] ;}
   inline double   GetTime(const int& i)          const { return fExl_Time[i] ;}
   inline int      GetExlNumber(const int& i)     const { return fExl_Number[i] ;}
   inline int      GetCrystalNumber(const int& i) const { return fExl_Crystal[i] ;}
   //Mult
   inline double   GetMult()    const { return fExl_Energy.size() ;}

   /////////////////////           SETTERS           ////////////////////////
   inline void SetEandTime(const int& N,const int& C,const double& E,const double& T){
     fExl_Energy.push_back(E);
     fExl_Time.push_back(T);    	   
     fExl_Number.push_back(N) ; 	   
     fExl_Crystal.push_back(C); 	   
   }

   ClassDef(TExlData,1)  // ExlData structure
};

#endif

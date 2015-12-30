#ifndef __CHIO_ANDATA__
#define __CHIO_ANDATA__
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
 *  This class hold CHIO Analog  raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include <vector>
#include "TObject.h"
using namespace std;


class TChio_anData : public TObject {
 private:
   // ADC
   vector<UShort_t>	fChio_an_Energy;
   vector<UShort_t>	fChio_an_Energy_pileup;

 public:
   TChio_anData();
   virtual ~TChio_anData();

   void	Clear();
   void  Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   UShort_t	GetMultE()                 {return fChio_an_Energy.size();}
   UShort_t	GetEnergy(Int_t i)         {return fChio_an_Energy.at(i);}
   UShort_t	GetMultE_pileup()          {return fChio_an_Energy_pileup.size();}
   UShort_t	GetEnergy_pileup(Int_t i)  {return fChio_an_Energy_pileup.at(i);}

   /////////////////////           SETTERS           ////////////////////////
   // (E)
   void SetEnergy(UShort_t E)          {fChio_an_Energy.push_back(E);}
   void SetEnergy_pileup(UShort_t E)   {fChio_an_Energy_pileup.push_back(E);}

   ClassDef(TChio_anData,1)  // Chio_anData structure
};

#endif

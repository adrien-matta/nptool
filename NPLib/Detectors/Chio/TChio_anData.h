#ifndef __CHIO_ANDATA__
#define __CHIO_ANDATA__
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
   vector<double>	fChio_an_Energy;
   vector<double>	fChio_an_Time;
   vector<double> fChio_an_DetectorNbr;

 public:
   TChio_anData();
   virtual ~TChio_anData();

   void	Clear();
   void Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   // (E)
   UShort_t	GetMult()                  {return fChio_an_Energy.size();}
   UShort_t	GetEnergy(unsigned int i)  {return fChio_an_Energy[i];}
   UShort_t	GetTime(unsigned int i)    {return fChio_an_Time[i];}
   UShort_t	GetDetectorNbr(unsigned i)  {return fChio_an_DetectorNbr[i];}

   /////////////////////           SETTERS           ////////////////////////
   // (E)
   inline void SetEnergyAndTime(double E, double T, unsigned int Det) {
      fChio_an_Energy.push_back(E);
      fChio_an_Time.push_back(T);
      fChio_an_DetectorNbr.push_back(Det);
  }

   ClassDef(TChio_anData,2)  // Chio_anData structure
};

#endif

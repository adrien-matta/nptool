#ifndef __EurogamDATA__
#define __EurogamDATA__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : October 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of a Eurogam germanium detector        *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;


class TEurogamData : public TObject {
 private:
   vector<UShort_t>     fEurogam_DetectorNumber;
   vector<Double_t>     fEurogam_Energy;	
   vector<Double_t>     fEurogam_Time;
   
 public:
   TEurogamData();
   virtual ~TEurogamData();

   void	Clear();
   void	Clear(const Option_t*) {};
   void	Dump() const;

   /////////////////////           GETTERS           ////////////////////////
   UShort_t GetEurogamDetectorNumber(Int_t i)   {return fEurogam_DetectorNumber.at(i);}
   Double_t	GetEurogamEnergy(Int_t i)           {return fEurogam_Energy.at(i);}
   Double_t GetEurogamTime(Int_t i)             {return fEurogam_Time.at(i);}

   Double_t GetEurogamEnergyMult()              {return fEurogam_Energy.size();}
   Double_t GetEurogamTimeMult()                {return fEurogam_Time.size();}
   
   
   /////////////////////           SETTERS           ////////////////////////
   void	SetEurogamDetectorNumber(Int_t N)      {fEurogam_DetectorNumber.push_back(N);}
   void	SetEurogamEnergy(Double_t E)           {fEurogam_Energy.push_back(E);}
   void	SetEurogamTime(Double_t T)             {fEurogam_Time.push_back(T);}
   
   ClassDef(TEurogamData,1)  // EurogamData structure
};

#endif

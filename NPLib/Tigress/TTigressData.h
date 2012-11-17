#ifndef __TigressDATA__
#define __TigressDATA__
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the Tigress  raw data (Made for TIG10 card)              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// STL
#include <vector>
using namespace std ;

// ROOT
#include "TObject.h"

class TTigressData : public TObject {
private:
  // Tigress
  // Energy
  vector<UShort_t> fTIG_CloverNbr;
  vector<UShort_t> fTIG_CrystalNbr;
  vector<Double_t> fTIG_Energy;
  vector<Double_t> fTIG_Time;
public:
  TTigressData();
  virtual ~TTigressData();
  
  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;
  
  /////////////////////           SETTERS           ////////////////////////
  void SetCloverNbr(UShort_t CloverNbr){fTIG_CloverNbr.push_back(CloverNbr); }
  void SetCrystalNbr(UShort_t CrystalNbr){fTIG_CrystalNbr.push_back(CrystalNbr);}
  void SetEnergy(UShort_t Energy){fTIG_Energy.push_back(Energy);}
  void SetTime(UShort_t Time){fTIG_Time.push_back(Time);}
  
  /////////////////////           GETTERS           ////////////////////////
  UShort_t GetCloverNbr(unsigned int i) {return fTIG_CloverNbr[i];}
  UShort_t GetCrystalNbr(unsigned int i){return fTIG_CrystalNbr[i];}
  Double_t GetEnergyl(unsigned int i)   {return fTIG_Energy[i];}
  Double_t GetTime(unsigned int i)      {return fTIG_Time[i];}
  
  ClassDef(TTigressData,1)  // TigressData structure
};

#endif

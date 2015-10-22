#ifndef __TrifoilDATA__
#define __TrifoilDATA__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : December 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the Trifoil Silicon array raw data (Made for TIG64 card) *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// STL
#include<stdlib.h>
#include <vector>
#include <map>
#include <iostream>
using namespace std ;

// ROOT
#include "TObject.h"
#include "TH1F.h"
class TTrifoilData : public TObject {
private:
  // Trifoil
  // Energy
  vector<TH1F> fTrifoil_Waveform;
  vector<Double_t>  fTrifoil_TimeCFD;
  vector<Double_t>  fTrifoil_TimeLED;
  vector<Double_t>  fTrifoil_TimeStamp;

public:
  TTrifoilData();
  virtual ~TTrifoilData();
  
  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;
  
  /////////////////////           SETTERS           ////////////////////////
  inline void SetWaveform(const TH1F &Waveform)   {fTrifoil_Waveform.push_back(Waveform);}
  inline void SetTimeCFD(const Double_t &TimeCFD) {fTrifoil_TimeCFD.push_back(TimeCFD);}
  inline void SetTimeLED(const Double_t &TimeLED) {fTrifoil_TimeLED.push_back(TimeLED);}
  inline void SetTimeStamp(const Double_t &TimeStamp) {fTrifoil_TimeStamp.push_back(TimeStamp);}

  /////////////////////           GETTERS           ////////////////////////
  inline TH1F* GetWaveform(const unsigned int &i)    {return &(fTrifoil_Waveform[i]);}
  inline Double_t GetTimeCFD(const unsigned int &i) {return fTrifoil_TimeCFD[i];}
  inline Double_t GetTimeLED(const unsigned int &i) {return fTrifoil_TimeLED[i];}
  inline Double_t GetTimeStamp(const unsigned int &i) {return fTrifoil_TimeStamp[i];}

  inline unsigned int GetMultiplicity() {return fTrifoil_Waveform.size();}
  ClassDef(TTrifoilData,1)  // TrifoilData structure
};

#endif

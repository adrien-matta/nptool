#ifndef __TrifoilDATA__
#define __TrifoilDATA__
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
 *  This class hold the Trifoil Silicon array raw data (Made for TIG64 card)   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// STL
#include <vector>
#include <map>
using namespace std ;

// NPL
#include "../Tigress/TTigEventFragment.h"

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
  
  
public:
  TTrifoilData();
  virtual ~TTrifoilData();
  
  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;
  
  /////////////////////           SETTERS           ////////////////////////
  void SetWaveform(TH1F Waveform)   {fTrifoil_Waveform.push_back(Waveform);}
  void SetTimeCFD(Double_t TimeCFD) {fTrifoil_TimeCFD.push_back(TimeCFD);}
  void SetTimeLED(Double_t TimeLED) {fTrifoil_TimeLED.push_back(TimeLED);}
  
  /////////////////////           GETTERS           ////////////////////////
  TH1F GetWaveform(int i)    {return fTrifoil_Waveform[i];}
  Double_t GetTimeCFD(int i) {return fTrifoil_TimeCFD[i];}
  Double_t GetTimeLED(int i) {return fTrifoil_TimeLED[i];}

  unsigned int GetMultiplicity() {return fTrifoil_TimeCFD.size();}
public: // Method and object to construct to fill the data event from a Tigress DAQ event tree
  void ReadFSPCFile(string FSPCPath);//!
  void FillData(TTigEventFragment* TigEvent);//!
  
private:
  int m_FSPC_Channel;//!
  
  ClassDef(TTrifoilData,1)  // TrifoilData structure
};

#endif

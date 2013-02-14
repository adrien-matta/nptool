#ifndef __SharcDATA__
#define __SharcDATA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
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
 *  This class hold the Sharc Silicon array raw data (Made for TIG64 card)   *
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
using namespace std ;

// ROOT
#include "TObject.h"

class TSharcData : public TObject {
private:
  // Sharc
  // Energy
  vector<UShort_t>   fSharc_StripFront_DetectorNbr;
  vector<UShort_t>   fSharc_StripFront_StripNbr;
  vector<Double_t>   fSharc_StripFront_Energy;
  vector<Double_t>   fSharc_StripFront_TimeCFD;
  vector<Double_t>   fSharc_StripFront_TimeLED;
  
  vector<UShort_t>   fSharc_StripBack_DetectorNbr;
  vector<UShort_t>   fSharc_StripBack_StripNbr;
  vector<Double_t>   fSharc_StripBack_Energy;
  vector<Double_t>   fSharc_StripBack_TimeCFD;
  vector<Double_t>   fSharc_StripBack_TimeLED;
  
  vector<UShort_t>   fSharc_PAD_DetectorNbr;
  vector<Double_t>   fSharc_PAD_Energy;
  vector<Double_t>   fSharc_PAD_TimeCFD;
  vector<Double_t>   fSharc_PAD_TimeLED;
  
public:
  TSharcData();
  virtual ~TSharcData();
  
  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;
  
  /////////////////////           SETTERS           ////////////////////////
  void SetFront_DetectorNbr(UShort_t DetNbr){fSharc_StripFront_DetectorNbr.push_back(DetNbr);}
  void SetFront_StripNbr(UShort_t StripNbr){fSharc_StripFront_StripNbr.push_back(StripNbr);}
  void SetFront_Energy(Double_t Energy){fSharc_StripFront_Energy.push_back(Energy);}
  void SetFront_TimeCFD(Double_t TimeCFD){fSharc_StripFront_TimeCFD.push_back(TimeCFD);}
  void SetFront_TimeLED(Double_t TimeLED){fSharc_StripFront_TimeLED.push_back(TimeLED);}
  
  void SetBack_DetectorNbr(UShort_t DetNbr){fSharc_StripBack_DetectorNbr.push_back(DetNbr);}
  void SetBack_StripNbr(UShort_t StripNbr){fSharc_StripBack_StripNbr.push_back(StripNbr);}
  void SetBack_Energy(Double_t Energy){fSharc_StripBack_Energy.push_back(Energy);}
  void SetBack_TimeCFD(Double_t TimeCFD){fSharc_StripBack_TimeCFD.push_back(TimeCFD);}
  void SetBack_TimeLED(Double_t TimeLED){fSharc_StripBack_TimeLED.push_back(TimeLED);}
 
  void SetPAD_DetectorNbr(UShort_t DetNbr){fSharc_PAD_DetectorNbr.push_back(DetNbr);}
  void SetPAD_Energy(Double_t Energy){fSharc_PAD_Energy.push_back(Energy);}
  void SetPAD_TimeCFD(Double_t TimeCFD){fSharc_PAD_TimeCFD.push_back(TimeCFD);}
  void SetPAD_TimeLED(Double_t TimeLED){fSharc_PAD_TimeLED.push_back(TimeLED);}

  
  /////////////////////           GETTERS           ////////////////////////
  UShort_t GetFront_DetectorNbr(unsigned int i) const {return fSharc_StripFront_DetectorNbr[i];}//!
  UShort_t GetFront_StripNbr(unsigned int i)    const {return fSharc_StripFront_StripNbr[i];}//!
  Double_t GetFront_Energy(unsigned int i)      const {return fSharc_StripFront_Energy[i];}//!
  Double_t GetFront_TimeCFD(unsigned int i)     const {return fSharc_StripFront_TimeCFD[i];}//!
  Double_t GetFront_TimeLED(unsigned int i)     const {return fSharc_StripFront_TimeLED[i];}//!

  UShort_t GetBack_DetectorNbr(unsigned int i) const {return fSharc_StripBack_DetectorNbr[i];}//!
  UShort_t GetBack_StripNbr(unsigned int i)    const {return fSharc_StripBack_StripNbr[i];}//!
  Double_t GetBack_Energy(unsigned int i)      const {return fSharc_StripBack_Energy[i];}//!
  Double_t GetBack_TimeCFD(unsigned int i)     const {return fSharc_StripBack_TimeCFD[i];}//!
  Double_t GetBack_TimeLED(unsigned int i)     const {return fSharc_StripBack_TimeLED[i];}//!

  UShort_t GetPAD_DetectorNbr(unsigned int i) const {return fSharc_PAD_DetectorNbr[i];}//!
  Double_t GetPAD_Energy(unsigned int i)      const {return fSharc_PAD_Energy[i];}//!
  Double_t GetPAD_TimeCFD(unsigned int i)     const {return fSharc_PAD_TimeCFD[i];}//!
  Double_t GetPAD_TimeLED(unsigned int i)     const {return fSharc_PAD_TimeLED[i];}//!

  unsigned int GetMultiplicityFront() const {return fSharc_StripFront_DetectorNbr.size();}//!
  unsigned int GetMultiplicityBack()  const {return fSharc_StripBack_DetectorNbr.size();}//!
  unsigned int GetMultiplicityPAD()   const {return fSharc_PAD_DetectorNbr.size();}//!
  
  ClassDef(TSharcData,1)  // SharcData structure
};

#endif

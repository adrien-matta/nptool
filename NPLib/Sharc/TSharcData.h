#ifndef __SharcDATA__
#define __SharcDATA__
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
 *  This class hold the Sharc Silicon array raw data (Made for TIG64 card)   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>
#include "TObject.h"
using namespace std ;

class TSharcData : public TObject {
private:
  // Sharc
  // Energy
  vector<UShort_t>   fSharc_StripFront_DetectorNbr;
  vector<UShort_t>   fSharc_StripFront_StripNbr;
  vector<Double_t>   fSharc_StripFront_Energy;
  vector<Double_t>   fSharc_StripFront_Time;
  
  vector<UShort_t>   fSharc_StripBack_DetectorNbr;
  vector<UShort_t>   fSharc_StripBack_StripNbr;
  vector<Double_t>   fSharc_StripBack_Energy;
  vector<Double_t>   fSharc_StripBack_Time;
  
  vector<UShort_t>   fSharc_PAD_DetectorNbr;
  vector<Double_t>   fSharc_PAD_Energy;
  vector<Double_t>   fSharc_PAD_Time;
  
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
  void SetFront_Time(Double_t Time){fSharc_StripFront_Time.push_back(Time);}
  
  void SetBack_DetectorNbr(UShort_t DetNbr){fSharc_StripBack_DetectorNbr.push_back(DetNbr);}
  void SetBack_StripNbr(UShort_t StripNbr){fSharc_StripBack_StripNbr.push_back(StripNbr);}
  void SetBack_Energy(Double_t Energy){fSharc_StripBack_Energy.push_back(Energy);}
  void SetBack_Time(Double_t Time){fSharc_StripBack_Time.push_back(Time);}
  
  void SetPAD_DetectorNbr(UShort_t DetNbr){fSharc_PAD_DetectorNbr.push_back(DetNbr);}
  void SetPAD_Energy(Double_t Energy){fSharc_PAD_Energy.push_back(Energy);}
  void SetPAD_Time(Double_t Time){fSharc_PAD_Time.push_back(Time);}
  
  
  /////////////////////           GETTERS           ////////////////////////
  UShort_t GetFront_DetectorNbr(unsigned int i) const {return fSharc_StripFront_DetectorNbr[i];}
  UShort_t GetFront_StripNbr(unsigned int i)    const {return fSharc_StripFront_StripNbr[i];}
  Double_t GetFront_Energy(unsigned int i)      const {return fSharc_StripFront_Energy[i];}
  Double_t GetFront_Time(unsigned int i)       const {return fSharc_StripFront_Time[i];}
  
  UShort_t GetBack_DetectorNbr(unsigned int i) const {return fSharc_StripBack_DetectorNbr[i];}
  UShort_t GetBack_StripNbr(unsigned int i)    const {return fSharc_StripBack_StripNbr[i];}
  Double_t GetBack_Energy(unsigned int i)      const {return fSharc_StripBack_Energy[i];}
  Double_t GetBack_Time(unsigned int i)        const {return fSharc_StripBack_Time[i];}
  
  UShort_t GetPAD_DetectorNbr(unsigned int i) const {return fSharc_PAD_DetectorNbr[i];}
  Double_t GetPAD_Energy(unsigned int i)      const {return fSharc_PAD_Energy[i];}
  Double_t GetPAD_Time(unsigned int i)        const {return fSharc_PAD_Time[i];}

  unsigned int GetMultiplicityFront() const {return fSharc_StripFront_DetectorNbr.size();}
  unsigned int GetMultiplicityBack()  const {return fSharc_StripBack_DetectorNbr.size();}
  unsigned int GetMultiplicityPAD()   const {return fSharc_PAD_DetectorNbr.size();}

  
  ClassDef(TSharcData,1)  // SharcData structure
};

#endif

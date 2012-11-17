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
  
public:
  TSharcData();
  virtual ~TSharcData();
  
  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;
  
  /////////////////////           SETTERS           ////////////////////////
  void SetFrontE_DetectorNbr(UShort_t DetNbr){fSharc_StripFront_DetectorNbr.push_back(DetNbr);}
  void SetFrontE_StripNbr(UShort_t StripNbr){fSharc_StripFront_StripNbr.push_back(StripNbr);}
  void SetFrontE_Energy(Double_t Energy){fSharc_StripFront_Energy.push_back(Energy);}
  void SetFrontE_Time(Double_t Time){fSharc_StripFront_Time.push_back(Time);}
  
  void SetBackE_DetectorNbr(UShort_t DetNbr){fSharc_StripBack_DetectorNbr.push_back(DetNbr);}
  void SetBackE_StripNbr(UShort_t StripNbr){fSharc_StripBack_StripNbr.push_back(StripNbr);}
  void SetBackE_Energy(Double_t Energy){fSharc_StripBack_Energy.push_back(Energy);}
  void SetBackE_Time(Double_t Time){fSharc_StripBack_Time.push_back(Time);}
  
  /////////////////////           GETTERS           ////////////////////////
  UShort_t GetFrontE_DetectorNbr(unsigned int i){return fSharc_StripFront_DetectorNbr[i];}
  UShort_t GetFrontE_StripNbr(unsigned int i)   {return fSharc_StripFront_StripNbr[i];}
  Double_t GetFrontE_Energy(unsigned int i)     {return fSharc_StripFront_Energy[i];}
  Double_t GetFrontE_Time(unsigned int i)       {return fSharc_StripFront_Time[i];}
  
  UShort_t GetBackE_DetectorNbr(unsigned int i) {return fSharc_StripFront_DetectorNbr[i];}
  UShort_t GetBackE_StripNbr(unsigned int i)    {return fSharc_StripFront_StripNbr[i];}
  Double_t GetBackE_Energy(unsigned int i)      {return fSharc_StripFront_Energy[i];}
  Double_t GetBackE_Time(unsigned int i)        {return fSharc_StripFront_Time[i];}
  
  ClassDef(TSharcData,1)  // SharcData structure
};

#endif

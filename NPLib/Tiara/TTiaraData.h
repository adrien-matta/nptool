#ifndef __TiaraDATA__
#define __TiaraDATA__
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
 *  This class hold the Tiara Silicon array raw data (Made for TIG64 card)   *
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
#include "TNamed.h"

class TTiaraData : public TNamed {
private:
  // Tiara
  // Energy
  vector<UShort_t>   fTiara_StripFront_DetectorNbr;
  vector<UShort_t>   fTiara_StripFront_StripNbr;
  vector<Double_t>   fTiara_StripFront_Energy;
  vector<Double_t>   fTiara_StripFront_TimeCFD;
  vector<Double_t>   fTiara_StripFront_TimeLED;
  vector<Double_t>   fTiara_StripFront_Time;
  
  vector<UShort_t>   fTiara_StripBack_DetectorNbr;
  vector<UShort_t>   fTiara_StripBack_StripNbr;
  vector<Double_t>   fTiara_StripBack_Energy;
  vector<Double_t>   fTiara_StripBack_TimeCFD;
  vector<Double_t>   fTiara_StripBack_TimeLED;
  vector<Double_t>   fTiara_StripBack_Time;

  vector<UShort_t>   fTiara_PAD_DetectorNbr;
  vector<Double_t>   fTiara_PAD_Energy;
  vector<Double_t>   fTiara_PAD_TimeCFD;
  vector<Double_t>   fTiara_PAD_TimeLED;
  vector<Double_t>   fTiara_PAD_Time;

public:
  TTiaraData();
  virtual ~TTiaraData();
  
  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;
  
  /////////////////////           SETTERS           ////////////////////////
  inline void SetFront_DetectorNbr(const UShort_t &DetNbr){fTiara_StripFront_DetectorNbr.push_back(DetNbr);}
  inline void SetFront_StripNbr(const UShort_t &StripNbr){fTiara_StripFront_StripNbr.push_back(StripNbr);}
  inline void SetFront_Energy(const Double_t &Energy){fTiara_StripFront_Energy.push_back(Energy);}
  inline void SetFront_TimeCFD(const Double_t &TimeCFD){fTiara_StripFront_TimeCFD.push_back(TimeCFD);}
  inline void SetFront_TimeLED(const Double_t &TimeLED){fTiara_StripFront_TimeLED.push_back(TimeLED);}
  inline void SetFront_Time(const Double_t &Time){fTiara_StripFront_Time.push_back(Time);}


  inline void SetBack_DetectorNbr(const UShort_t &DetNbr){fTiara_StripBack_DetectorNbr.push_back(DetNbr);}
  inline void SetBack_StripNbr(const UShort_t &StripNbr){fTiara_StripBack_StripNbr.push_back(StripNbr);}
  inline void SetBack_Energy(const Double_t &Energy){fTiara_StripBack_Energy.push_back(Energy);}
  inline void SetBack_TimeCFD(const Double_t &TimeCFD){fTiara_StripBack_TimeCFD.push_back(TimeCFD);}
  inline void SetBack_TimeLED(const Double_t &TimeLED){fTiara_StripBack_TimeLED.push_back(TimeLED);}
  inline void SetBack_Time(const Double_t &Time){fTiara_StripBack_Time.push_back(Time);}


  inline void SetPAD_DetectorNbr(const UShort_t &DetNbr){fTiara_PAD_DetectorNbr.push_back(DetNbr);}
  inline void SetPAD_Energy(const Double_t &Energy){fTiara_PAD_Energy.push_back(Energy);}
  inline void SetPAD_TimeCFD(const Double_t &TimeCFD){fTiara_PAD_TimeCFD.push_back(TimeCFD);}
  inline void SetPAD_TimeLED(const Double_t &TimeLED){fTiara_PAD_TimeLED.push_back(TimeLED);}
  inline void SetPAD_Time(const Double_t &Time){fTiara_PAD_Time.push_back(Time);}


  inline void SetFront(const UShort_t &DetNbr,const UShort_t &StripNbr,const Double_t &Energy,const Double_t &TimeCFD,const Double_t &TimeLED,const Double_t &Time = 0)	{
		SetFront_DetectorNbr(DetNbr);
		SetFront_StripNbr(StripNbr);
		SetFront_Energy(Energy);
		SetFront_TimeCFD(TimeCFD);
		SetFront_TimeLED(TimeLED);
		SetFront_Time(Time);
	};
	inline void SetBack(const UShort_t &DetNbr,const UShort_t &StripNbr,const Double_t &Energy,const Double_t &TimeCFD,const Double_t &TimeLED,const Double_t &Time = 0)	{
		SetBack_DetectorNbr(DetNbr);
		SetBack_StripNbr(StripNbr);
		SetBack_Energy(Energy);
		SetBack_TimeCFD(TimeCFD);
		SetBack_TimeLED(TimeLED);
		SetBack_Time(Time);
	};
	inline void SetPAD(const UShort_t &DetNbr,const Double_t &Energy,const Double_t &TimeCFD,const Double_t &TimeLED,const Double_t &Time = 0)	{
		SetPAD_DetectorNbr(DetNbr);
		SetPAD_Energy(Energy);
		SetPAD_TimeCFD(TimeCFD);
		SetPAD_TimeLED(TimeLED);
		SetPAD_Time(Time);
	};
  
  /////////////////////           GETTERS           ////////////////////////
  inline UShort_t GetFront_DetectorNbr(const unsigned int &i) const {return fTiara_StripFront_DetectorNbr[i];}//!
  inline UShort_t GetFront_StripNbr(const unsigned int &i)    const {return fTiara_StripFront_StripNbr[i];}//!
  inline Double_t GetFront_Energy(const unsigned int &i)      const {return fTiara_StripFront_Energy[i];}//!
  inline Double_t GetFront_TimeCFD(const unsigned int &i)     const {return fTiara_StripFront_TimeCFD[i];}//!
  inline Double_t GetFront_TimeLED(const unsigned int &i)     const {return fTiara_StripFront_TimeLED[i];}//!
  inline Double_t GetFront_Time(const unsigned int &i)     const {return fTiara_StripFront_Time[i];}//!


  inline UShort_t GetBack_DetectorNbr(const unsigned int &i) const {return fTiara_StripBack_DetectorNbr[i];}//!
  inline UShort_t GetBack_StripNbr(const unsigned int &i)    const {return fTiara_StripBack_StripNbr[i];}//!
  inline Double_t GetBack_Energy(const unsigned int &i)      const {return fTiara_StripBack_Energy[i];}//!
  inline Double_t GetBack_TimeCFD(const unsigned int &i)     const {return fTiara_StripBack_TimeCFD[i];}//!
  inline Double_t GetBack_TimeLED(const unsigned int &i)     const {return fTiara_StripBack_TimeLED[i];}//!
  inline Double_t GetBack_Time(const unsigned int &i)     const {return fTiara_StripBack_Time[i];}//!


  inline UShort_t GetPAD_DetectorNbr(const unsigned int &i) const {return fTiara_PAD_DetectorNbr[i];}//!
  inline Double_t GetPAD_Energy(const unsigned int &i)      const {return fTiara_PAD_Energy[i];}//!
  inline Double_t GetPAD_TimeCFD(const unsigned int &i)     const {return fTiara_PAD_TimeCFD[i];}//!
  inline Double_t GetPAD_TimeLED(const unsigned int &i)     const {return fTiara_PAD_TimeLED[i];}//!
  inline Double_t GetPAD_Time(const unsigned int &i)    	  const {return fTiara_PAD_Time[i];}//!


  inline unsigned int GetMultiplicityFront() const {return fTiara_StripFront_DetectorNbr.size();}//!
  inline unsigned int GetMultiplicityBack()  const {return fTiara_StripBack_DetectorNbr.size();}//!
  inline unsigned int GetMultiplicityPAD()   const {return fTiara_PAD_DetectorNbr.size();}//!
 
  ClassDef(TTiaraData,1)  // TiaraData structure
};

#endif

#ifndef __GeTAMUDATA__
#define __GeTAMUDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
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
 *  This class hold the GeTAMU  raw data (Made for TIG10 card)              *
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

class TGeTAMUData : public TObject {
private:
  // GeTAMU
  // Energy
  vector<UShort_t> fTIG_Ge_CloverNbr;
  vector<UShort_t> fTIG_Ge_CrystalNbr;
  vector<UShort_t> fTIG_Ge_SegmentNbr;
  vector<Double_t> fTIG_Ge_Energy;
  vector<Double_t> fTIG_Ge_TimeCFD;
  vector<Double_t> fTIG_Ge_TimeLED;

  vector<UShort_t> fTIG_BGO_CloverNbr;
  vector<UShort_t> fTIG_BGO_CrystalNbr;
  vector<UShort_t> fTIG_BGO_PmNbr;
  vector<Double_t> fTIG_BGO_Energy;
  vector<Double_t> fTIG_BGO_TimeCFD;
  vector<Double_t> fTIG_BGO_TimeLED;


public:
  TGeTAMUData();
  virtual ~TGeTAMUData();
  
  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;
  
  /////////////////////           SETTERS           ////////////////////////
  inline void SetGeCloverNbr(const UShort_t &GeCloverNbr){fTIG_Ge_CloverNbr.push_back(GeCloverNbr); }
  inline void SetGeCrystalNbr(const UShort_t &GeCrystalNbr){fTIG_Ge_CrystalNbr.push_back(GeCrystalNbr);}
  inline void SetGeSegmentNbr(const UShort_t &GeSegmentNbr){fTIG_Ge_SegmentNbr.push_back(GeSegmentNbr);}
  inline void SetGeEnergy(const Double_t &GeEnergy){fTIG_Ge_Energy.push_back(GeEnergy);}
  inline void SetGeTimeCFD(const Double_t &GeTimeCFD){fTIG_Ge_TimeCFD.push_back(GeTimeCFD);}
  inline void SetGeTimeLED(const Double_t &GeTimeLED){fTIG_Ge_TimeLED.push_back(GeTimeLED);}

  inline void SetBGOCloverNbr(const UShort_t &BGOCloverNbr){fTIG_BGO_CloverNbr.push_back(BGOCloverNbr); }
  inline void SetBGOCrystalNbr(const UShort_t &BGOCrystalNbr){fTIG_BGO_CrystalNbr.push_back(BGOCrystalNbr);}
  inline void SetBGOPmNbr(const UShort_t &BGOPmNbr){fTIG_BGO_PmNbr.push_back(BGOPmNbr);}
  inline void SetBGOEnergy(const Double_t &BGOEnergy){fTIG_BGO_Energy.push_back(BGOEnergy);}
  inline void SetBGOTimeCFD(const Double_t &BGOTimeCFD){fTIG_BGO_TimeCFD.push_back(BGOTimeCFD);}
  inline void SetBGOTimeLED(const Double_t &BGOTimeLED){fTIG_BGO_TimeLED.push_back(BGOTimeLED);}

  /////////////////////           GETTERS           ////////////////////////
  inline UShort_t GetGeCloverNbr(const unsigned int &i)   {return fTIG_Ge_CloverNbr[i]; }
  inline UShort_t GetGeCrystalNbr(const unsigned int &i)  {return fTIG_Ge_CrystalNbr[i]; }
  inline UShort_t GetGeSegmentNbr(const unsigned int &i)  {return fTIG_Ge_SegmentNbr[i]; }

  inline Double_t GetGeEnergy(const unsigned int &i)      {return fTIG_Ge_Energy[i];}
  inline Double_t GetGeTimeCFD(const unsigned int &i)     {return fTIG_Ge_TimeCFD[i];}
  inline Double_t GetGeTimeLED(const unsigned int &i)     {return fTIG_Ge_TimeLED[i];}

  inline UShort_t GetBGOCloverNbr(const unsigned int &i)   {return fTIG_BGO_CloverNbr[i]; }
  inline UShort_t GetBGOCrystalNbr(const unsigned int &i)  {return fTIG_BGO_CrystalNbr[i]; }
  inline UShort_t GetBGOPmNbr(const unsigned int &i)       {return fTIG_BGO_PmNbr[i]; }
  inline Double_t GetBGOEnergy(const unsigned int &i)      {return fTIG_BGO_Energy[i];}
  inline Double_t GetBGOTimeCFD(const unsigned int &i)     {return fTIG_BGO_TimeCFD[i];}
  inline Double_t GetBGOTimeLED(const unsigned int &i)     {return fTIG_BGO_TimeLED[i];}

  inline unsigned int GetMultiplicityGe()  {return fTIG_Ge_CloverNbr.size();}
  inline unsigned int GetMultiplicityBGO()  {return fTIG_BGO_CloverNbr.size();}
  
  ClassDef(TGeTAMUData,1)  // GeTAMUData structure
};

#endif


















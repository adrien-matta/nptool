#ifndef __TigressDATA__
#define __TigressDATA__
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
 *  This class hold the Tigress  raw data (Made for TIG10 card)              *
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

// NPL
#include "TTigEventFragment.h"

// ROOT
#include "TObject.h"

class TTigressData : public TObject {
private:
  // Tigress
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
  TTigressData();
  virtual ~TTigressData();
  
  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;
  
  /////////////////////           SETTERS           ////////////////////////
  void SetGeCloverNbr(UShort_t GeCloverNbr){fTIG_Ge_CloverNbr.push_back(GeCloverNbr); }
  void SetGeCrystalNbr(UShort_t GeCrystalNbr){fTIG_Ge_CrystalNbr.push_back(GeCrystalNbr);}
  void SetGeSegmentNbr(UShort_t GeSegmentNbr){fTIG_Ge_SegmentNbr.push_back(GeSegmentNbr);}
  void SetGeEnergy(UShort_t GeEnergy){fTIG_Ge_Energy.push_back(GeEnergy);}
  void SetGeTimeCFD(UShort_t GeTimeCFD){fTIG_Ge_TimeCFD.push_back(GeTimeCFD);}
  void SetGeTimeLED(UShort_t GeTimeLED){fTIG_Ge_TimeLED.push_back(GeTimeLED);}

  void SetBGOCloverNbr(UShort_t BGOCloverNbr){fTIG_BGO_CloverNbr.push_back(BGOCloverNbr); }
  void SetBGOCrystalNbr(UShort_t BGOCrystalNbr){fTIG_BGO_CrystalNbr.push_back(BGOCrystalNbr);}
  void SetBGOPmNbr(UShort_t BGOPmNbr){fTIG_BGO_PmNbr.push_back(BGOPmNbr);}
  void SetBGOEnergy(UShort_t BGOEnergy){fTIG_BGO_Energy.push_back(BGOEnergy);}
  void SetBGOTimeCFD(UShort_t BGOTimeCFD){fTIG_BGO_TimeCFD.push_back(BGOTimeCFD);}
  void SetBGOTimeLED(UShort_t BGOTimeLED){fTIG_BGO_TimeLED.push_back(BGOTimeLED);}

  /////////////////////           GETTERS           ////////////////////////
  UShort_t GetGeCloverNbr(unsigned int i)   {return fTIG_Ge_CloverNbr[i]; }
  UShort_t GetGeCrystalNbr(unsigned int i)  {return fTIG_Ge_CrystalNbr[i]; }
  UShort_t GetGeSegmentNbr(unsigned int i)  {return fTIG_Ge_SegmentNbr[i]; }
  Double_t GetGeEnergy(unsigned int i)      {return fTIG_Ge_Energy[i];}
  Double_t GetGeTimeCFD(unsigned int i)     {return fTIG_Ge_TimeCFD[i];}
  Double_t GetGeTimeLED(unsigned int i)     {return fTIG_Ge_TimeLED[i];}


  UShort_t GetBGOCloverNbr(unsigned int i)   {return fTIG_BGO_CloverNbr[i]; }
  UShort_t GetBGOCrystalNbr(unsigned int i)  {return fTIG_BGO_CrystalNbr[i]; }
  UShort_t GetBGOPmNbr(unsigned int i)       {return fTIG_BGO_PmNbr[i]; }
  Double_t GetBGOEnergy(unsigned int i)      {return fTIG_BGO_Energy[i];}
  Double_t GetBGOTimeCFD(unsigned int i)     {return fTIG_BGO_TimeCFD[i];}
  Double_t GetBGOTimeLED(unsigned int i)     {return fTIG_BGO_TimeLED[i];}

  unsigned int GetMultiplicityGe()  {return fTIG_Ge_CloverNbr.size();}
  unsigned int GetMultiplicityBGO()  {return fTIG_BGO_CloverNbr.size();}

public: // Method and object to construct to fill the data event from a Tigress DAQ event tree
  void ReadFSPCFile(string FSPCPath);//!
  void FillData(TTigEventFragment* TigEvent);//!
  void FillGe(unsigned short GeCloverNbr, unsigned short GeCrystalNbr , unsigned short GeSegmentNbr, int hit,TTigEventFragment* TigEvent);
  void FillBGO(unsigned short BGOCloverNbr, unsigned short BGOPmNbr , unsigned short BGOSegmentNbr, int hit,TTigEventFragment* TigEvent);

  map< int,vector<int> > GetFSPC2Detector() const {return m_FSPC2Detector;}//!
  
private:// Key is the FSPC channel,
        // vector[0] is the Clover Nbr
        // vector[1] is the crystal Nbr
        // vector[2] is the segment Nbr
        // vector[3] is 0: Ge , 1: BGO
  map< int,vector<int> > m_FSPC2Detector;//!

  
  ClassDef(TTigressData,1)  // TigressData structure
};

#endif


















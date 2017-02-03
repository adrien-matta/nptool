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
 * Creation Date  : 2016                                                     *
 * Last update    : December 2016 m.moukaddam@suurey.ac.uk                   *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the GeTAMU  raw data                                     *
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
  // Core 
  vector<unsigned short> fGeTAMU_Core_CloverNbr_E;
  vector<unsigned short> fGeTAMU_Core_CrystalNbr_E;
  vector<double> fGeTAMU_Core_Energy;
  vector<unsigned short> fGeTAMU_Core_CloverNbr_T;
  vector<unsigned short> fGeTAMU_Core_CrystalNbr_T;
  vector<double> fGeTAMU_Core_Time;
  // Segment
  vector<unsigned short> fGeTAMU_Segment_CloverNbr_E;
  vector<unsigned short> fGeTAMU_Segment_SegmentNbr_E;
  vector<double> fGeTAMU_Segment_Energy;
  vector<unsigned short> fGeTAMU_Segment_CloverNbr_T;
  vector<unsigned short> fGeTAMU_Segment_SegmentNbr_T;
  vector<double> fGeTAMU_Segment_Time;


public:
  TGeTAMUData();
  virtual ~TGeTAMUData();
  
  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;
  
  /////////////////////           SETTERS           ////////////////////////
  inline void SetCoreCloverNbrE(const unsigned short &CoreCloverNbr){fGeTAMU_Core_CloverNbr_E.push_back(CoreCloverNbr); }
  inline void SetCoreCrystalNbrE(const unsigned short &CoreCrystalNbr){fGeTAMU_Core_CrystalNbr_E.push_back(CoreCrystalNbr);}
  inline void SetCoreEnergy(const double &CoreEnergy){fGeTAMU_Core_Energy.push_back(CoreEnergy);}

  inline void SetCoreCloverNbrT(const unsigned short &CoreCloverNbr){fGeTAMU_Core_CloverNbr_T.push_back(CoreCloverNbr); }
  inline void SetCoreCrystalNbrT(const unsigned short &CoreCrystalNbr){fGeTAMU_Core_CrystalNbr_T.push_back(CoreCrystalNbr);}
  inline void SetCoreTime(const double &CoreTime){fGeTAMU_Core_Time.push_back(CoreTime);}

  inline void SetCoreE(const unsigned short &CoreCloverNbr,
                      const unsigned short &CoreCrystalNbr,
                      const double &CoreEnergy){
    fGeTAMU_Core_CloverNbr_E.push_back(CoreCloverNbr);
    fGeTAMU_Core_CrystalNbr_E.push_back(CoreCrystalNbr);
    fGeTAMU_Core_Energy.push_back(CoreEnergy);}

  inline void SetCoreT(const unsigned short &CoreCloverNbr,
                      const unsigned short &CoreCrystalNbr,
                      const double &CoreTime){
    fGeTAMU_Core_CloverNbr_T.push_back(CoreCloverNbr);
    fGeTAMU_Core_CrystalNbr_T.push_back(CoreCrystalNbr);
    fGeTAMU_Core_Time.push_back(CoreTime);}

  inline void SetSegmentCloverNbrE(const unsigned short &SegmentCloverNbr){fGeTAMU_Segment_CloverNbr_E.push_back(SegmentCloverNbr); }
  inline void SetSegmentSegmentNbrE(const unsigned short &SegmentNbr){fGeTAMU_Segment_SegmentNbr_E.push_back(SegmentNbr);}
  inline void SetSegmentEnergy(const double &SegmentEnergy){fGeTAMU_Segment_Energy.push_back(SegmentEnergy);}
  
  inline void SetSegmentCloverNbrT(const unsigned short &SegmentCloverNbr){fGeTAMU_Segment_CloverNbr_T.push_back(SegmentCloverNbr); }
  inline void SetSegmentSegmentNbrT(const unsigned short &SegmentNbr){fGeTAMU_Segment_SegmentNbr_T.push_back(SegmentNbr);}
  inline void SetSegmentTime(const double &SegmentTime){fGeTAMU_Segment_Time.push_back(SegmentTime);}
  
  inline void SetSegmentE(const unsigned short &SegmentCloverNbr,
                      const unsigned short &SegmentNbr,
                      const double &SegmentEnergy){
    fGeTAMU_Segment_CloverNbr_E.push_back(SegmentCloverNbr);
    fGeTAMU_Segment_SegmentNbr_E.push_back(SegmentNbr);
    fGeTAMU_Segment_Energy.push_back(SegmentEnergy);};

  inline void SetSegmentT(const unsigned short &SegmentCloverNbr,
                      const unsigned short &SegmentNbr,
                      const double &SegmentTime){
    fGeTAMU_Segment_CloverNbr_T.push_back(SegmentCloverNbr);
    fGeTAMU_Segment_SegmentNbr_T.push_back(SegmentNbr);
    fGeTAMU_Segment_Time.push_back(SegmentTime);};

  /////////////////////           GETTERS           ////////////////////////
  inline unsigned int GetMultiplicityCoreE()  {return fGeTAMU_Core_CloverNbr_E.size();}
  inline unsigned short GetCoreCloverNbrE(const unsigned int &i)   {return fGeTAMU_Core_CloverNbr_E[i]; }
  inline unsigned short GetCoreCrystalNbrE(const unsigned int &i)  {return fGeTAMU_Core_CrystalNbr_E[i]; }
  inline double GetCoreEnergy(const unsigned int &i)      {return fGeTAMU_Core_Energy[i];}

  inline unsigned int GetMultiplicityCoreT()  {return fGeTAMU_Core_CloverNbr_T.size();}
  inline unsigned short GetCoreCloverNbrT(const unsigned int &i)   {return fGeTAMU_Core_CloverNbr_T[i]; }
  inline unsigned short GetCoreCrystalNbrT(const unsigned int &i)  {return fGeTAMU_Core_CrystalNbr_T[i]; }
  inline double GetCoreTime(const unsigned int &i)     {return fGeTAMU_Core_Time[i];}
  
  inline unsigned int GetMultiplicitySegmentE()  {return fGeTAMU_Segment_CloverNbr_E.size();}
  inline unsigned short GetSegmentCloverNbrE(const unsigned int &i)   {return fGeTAMU_Segment_CloverNbr_E[i]; }
  inline unsigned short GetSegmentSegmentNbrE(const unsigned int &i)   {return fGeTAMU_Segment_SegmentNbr_E[i]; }
  inline double GetSegmentEnergy(const unsigned int &i)      {return fGeTAMU_Segment_Energy[i];}
  
  inline unsigned int GetMultiplicitySegmentT()  {return fGeTAMU_Segment_CloverNbr_T.size();}
  inline unsigned short GetSegmentCloverNbrT(const unsigned int &i)   {return fGeTAMU_Segment_CloverNbr_T[i]; }
  inline unsigned short GetSegmentSegmentNbrT(const unsigned int &i)   {return fGeTAMU_Segment_SegmentNbr_T[i]; }
  inline double GetSegmentTime(const unsigned int &i)     {return fGeTAMU_Segment_Time[i];}

  ClassDef(TGeTAMUData,1)  // GeTAMUData structure
};

#endif


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
 * Last update    :                                                          *
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
  // Energy
  vector<unsigned short> fGeTAMU_Core_CloverNbr;
  vector<unsigned short> fGeTAMU_Core_CristalNbr;
  vector<double> fGeTAMU_Core_Energy;
  vector<double> fGeTAMU_Core_Time;

  vector<unsigned short> fGeTAMU_Segment_CloverNbr;
  vector<unsigned short> fGeTAMU_Segment_SegmentNbr;
  vector<double> fGeTAMU_Segment_Energy;
  vector<double> fGeTAMU_Segment_Time;


public:
  TGeTAMUData();
  virtual ~TGeTAMUData();
  
  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;
  
  /////////////////////           SETTERS           ////////////////////////
  inline void SetCoreCloverNbr(const unsigned short &CoreCloverNbr){fGeTAMU_Core_CloverNbr.push_back(CoreCloverNbr); }
  inline void SetCoreCristalNbr(const unsigned short &CoreCristalNbr){fGeTAMU_Core_CristalNbr.push_back(CoreCristalNbr);}
  inline void SetCoreEnergy(const double &CoreEnergy){fGeTAMU_Core_Energy.push_back(CoreEnergy);}
  inline void SetCoreTime(const double &CoreTime){fGeTAMU_Core_Time.push_back(CoreTime);}
  inline void SetCore(const unsigned short &CoreCloverNbr,
                      const unsigned short &CoreCristalNbr,
                      const double &CoreEnergy,
                      const double &CoreTime){
    fGeTAMU_Core_CloverNbr.push_back(CoreCloverNbr);
    fGeTAMU_Core_CristalNbr.push_back(CoreCristalNbr);
    fGeTAMU_Core_Energy.push_back(CoreEnergy);
    fGeTAMU_Core_Time.push_back(CoreTime);
  };

  inline void SetSegmentCloverNbr(const unsigned short &SegmentCloverNbr){fGeTAMU_Segment_CloverNbr.push_back(SegmentCloverNbr); }
  inline void SetSegmentSegmentNbr(const unsigned short &SegmentNbr){fGeTAMU_Segment_SegmentNbr.push_back(SegmentNbr);}
  inline void SetSegmentEnergy(const double &SegmentEnergy){fGeTAMU_Segment_Energy.push_back(SegmentEnergy);}
  inline void SetSegmentTime(const double &SegmentTime){fGeTAMU_Segment_Time.push_back(SegmentTime);}
  inline void SetSegment(const unsigned short &SegmentCloverNbr,
                      const unsigned short &SegmentNbr,
                      const double &SegmentEnergy,
                      const double &SegmentTime){
    fGeTAMU_Segment_CloverNbr.push_back(SegmentCloverNbr);
    fGeTAMU_Segment_SegmentNbr.push_back(SegmentNbr);
    fGeTAMU_Segment_Energy.push_back(SegmentEnergy);
    fGeTAMU_Segment_Time.push_back(SegmentTime);
  };


  /////////////////////           GETTERS           ////////////////////////
  inline unsigned short GetCoreCloverNbr(const unsigned int &i)   {return fGeTAMU_Core_CloverNbr[i]; }
  inline unsigned short GetCoreCristalNbr(const unsigned int &i)  {return fGeTAMU_Core_CristalNbr[i]; }
  inline double GetCoreEnergy(const unsigned int &i)      {return fGeTAMU_Core_Energy[i];}
  inline double GetCoreTime(const unsigned int &i)     {return fGeTAMU_Core_Time[i];}
  inline unsigned int GetMultiplicityCore()  {return fGeTAMU_Core_CloverNbr.size();}
  
  inline unsigned short GetSegmentCloverNbr(const unsigned int &i)   {return fGeTAMU_Segment_CloverNbr[i]; }
  inline unsigned short GetSegmentSegmentNbr(const unsigned int &i)   {return fGeTAMU_Segment_SegmentNbr[i]; }
  inline double GetSegmentEnergy(const unsigned int &i)      {return fGeTAMU_Segment_Energy[i];}
  inline double GetSegmentTime(const unsigned int &i)     {return fGeTAMU_Segment_Time[i];}
  inline unsigned int GetMultiplicitySegment()  {return fGeTAMU_Segment_CloverNbr.size();}
  

  ClassDef(TGeTAMUData,1)  // GeTAMUData structure
};

#endif


















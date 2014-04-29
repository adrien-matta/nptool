#ifndef __TiaraHyballDATA__
#define __TiaraHyballDATA__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2013                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the Tiara Hyball Silicon array raw data                  *
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

class TTiaraHyballData : public TNamed {

  public: 
    TTiaraHyballData();
    ~TTiaraHyballData();

  public:
    void Clear();
    void Clear(const Option_t*) {};
    void Dump() const;

  private:
    vector<unsigned short>   fTiaraHyball_RingE_DetectorNbr;
    vector<unsigned short>   fTiaraHyball_RingE_StripNbr;
    vector<double>           fTiaraHyball_RingE_Energy;

    vector<unsigned short>   fTiaraHyball_RingT_DetectorNbr;
    vector<unsigned short>   fTiaraHyball_RingT_StripNbr;
    vector<double>           fTiaraHyball_RingT_Time;

    vector<unsigned short>   fTiaraHyball_SectorE_DetectorNbr;
    vector<unsigned short>   fTiaraHyball_SectorE_StripNbr;
    vector<double>           fTiaraHyball_SectorE_Energy;

    vector<unsigned short>   fTiaraHyball_SectorT_DetectorNbr;
    vector<unsigned short>   fTiaraHyball_SectorT_StripNbr;
    vector<double>           fTiaraHyball_SectorT_Time;

    ///////////// Multiplicity Getter  ////////////////////
  public:
    inline unsigned int GetRingEMult()    const {return fTiaraHyball_RingE_DetectorNbr.size();}
    inline unsigned int GetRingTMult()    const {return fTiaraHyball_RingT_DetectorNbr.size();}
    inline unsigned int GetSectorEMult()  const {return fTiaraHyball_SectorE_DetectorNbr.size();}
    inline unsigned int GetSectorTMult()  const {return fTiaraHyball_SectorT_DetectorNbr.size();}

    ///////////// Fast Setters ////////////////////
  public:
    inline void SetRingE(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy){
      SetRingEDetectorNbr(DetNbr);
      SetRingEStripNbr(StripNbr);
      SetRingEEnergy(Energy);
    };

    inline void SetRingT(const unsigned short& DetNbr, const unsigned short& StripNbr,const double& Time){
      SetRingTDetectorNbr(DetNbr);
      SetRingTStripNbr(StripNbr);
      SetRingTTime(Time);
    };

    inline void SetSectorE(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy){
      SetSectorEDetectorNbr(DetNbr);
      SetSectorEStripNbr(StripNbr);
      SetSectorEEnergy(Energy);
    };

    inline void SetSectorT(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Time){
      SetSectorTDetectorNbr(DetNbr);
      SetSectorTStripNbr(StripNbr);
      SetSectorTTime(Time);
    };

    ///////////// Getter and Setter ////////////////
  public:
    inline void SetRingEDetectorNbr(const unsigned short& RingE_DetectorNbr)
    {fTiaraHyball_RingE_DetectorNbr.push_back(RingE_DetectorNbr);}
    inline unsigned short GetRingEDetectorNbr(const unsigned int& i) const 
    {return fTiaraHyball_RingE_DetectorNbr[i];} 


    inline void SetRingEStripNbr(const unsigned short& RingE_StripNbr)
    {fTiaraHyball_RingE_StripNbr.push_back(RingE_StripNbr);}
    inline unsigned short GetRingEStripNbr(const unsigned int& i) const 
    {return fTiaraHyball_RingE_StripNbr[i];}

    inline void SetRingEEnergy(const double& RingE_Energy)
    {fTiaraHyball_RingE_Energy.push_back(RingE_Energy);}
    inline double GetRingEEnergy(const unsigned int& i) const 
    {return fTiaraHyball_RingE_Energy[i];}

    inline void SetRingTDetectorNbr(const unsigned short& RingT_DetectorNbr)
    {fTiaraHyball_RingT_DetectorNbr.push_back(RingT_DetectorNbr);}
    inline unsigned short GetRingTDetectorNbr(const unsigned int& i) const 
    {return fTiaraHyball_RingT_DetectorNbr[i];}

    inline void SetRingTStripNbr(const unsigned short& RingT_StripNbr)
    {fTiaraHyball_RingT_StripNbr.push_back(RingT_StripNbr);}
    inline unsigned short GetRingTStripNbr(const unsigned int& i) const 
    {return fTiaraHyball_RingT_StripNbr[i];}

    inline void SetRingTTime(const double& RingT_Time)
    {fTiaraHyball_RingT_Time.push_back(RingT_Time);}
    inline double GetRingTTime(const unsigned int& i) const 
    {return fTiaraHyball_RingT_Time[i];}

    inline void SetSectorEDetectorNbr(const unsigned short& SectorE_DetectorNbr)
    {fTiaraHyball_SectorE_DetectorNbr.push_back(SectorE_DetectorNbr);}
    inline unsigned short GetSectorEDetectorNbr(const unsigned int& i) const 
    {return fTiaraHyball_SectorE_DetectorNbr[i];}

    inline void SetSectorEStripNbr(const unsigned short& SectorE_StripNbr)
    {fTiaraHyball_SectorE_StripNbr.push_back(SectorE_StripNbr);}
    inline unsigned short GetSectorEStripNbr(const unsigned int& i) const 
    {return fTiaraHyball_SectorE_StripNbr[i];}

    inline void SetSectorEEnergy(const double& SectorE_Energy)
    {fTiaraHyball_SectorE_Energy.push_back(SectorE_Energy);}
    inline double GetSectorEEnergy(const unsigned int& i) const 
    {return fTiaraHyball_SectorE_Energy[i];}

    inline void SetSectorTDetectorNbr(const unsigned short& SectorT_DetectorNbr)
    {fTiaraHyball_SectorT_DetectorNbr.push_back(SectorT_DetectorNbr);}
    inline unsigned short GetSectorTDetectorNbr(const unsigned int& i) const 
    {return fTiaraHyball_SectorT_DetectorNbr[i];}

    inline void SetSectorTStripNbr(const unsigned short& SectorT_StripNbr)
    {fTiaraHyball_SectorT_StripNbr.push_back(SectorT_StripNbr);}
    inline unsigned short GetSectorTStripNbr(const unsigned int& i) const 
    {return fTiaraHyball_SectorT_StripNbr[i];}

    inline void SetSectorTTime(const double& SectorT_Time)
    {fTiaraHyball_SectorT_Time.push_back(SectorT_Time);}
    inline double GetSectorTTime(const unsigned int& i) const 
    {return fTiaraHyball_SectorT_Time[i];}


    ClassDef(TTiaraHyballData,1)  // TiaraData structure   
};

#endif

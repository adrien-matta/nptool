#ifndef __TiaraHyballDATA__
#define __TiaraHyballDATA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
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


    ///////////// Fast Setters ////////////////////
  public:
    inline void Set_RingE(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy){
      Set_RingE_DetectorNbr(DetNbr);
      Set_RingE_StripNbr(StripNbr);
      Set_RingE_Energy(Energy);
    };

    inline void Set_RingT(const unsigned short& DetNbr, const unsigned short& StripNbr,const double& Time){
      Set_RingT_DetectorNbr(DetNbr);
      Set_RingT_StripNbr(StripNbr);
      Set_RingT_Time(Time);
    };

    inline void Set_SectorE(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy){
      Set_SectorE_DetectorNbr(DetNbr);
      Set_SectorE_StripNbr(StripNbr);
      Set_SectorE_Energy(Energy);
    };

    inline void Set_SectorT(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Time){
      Set_SectorT_DetectorNbr(DetNbr);
      Set_SectorT_StripNbr(StripNbr);
      Set_SectorT_Time(Time);
    };

    ///////////// Getter and Setter ////////////////
  public:
    inline void Set_RingE_DetectorNbr(const unsigned short& RingE_DetectorNbr)
    {fTiaraHyball_RingE_DetectorNbr.push_back(RingE_DetectorNbr);}
    inline unsigned short Get_RingE_DetectorNbr(const unsigned int& i) const 
    {return fTiaraHyball_RingE_DetectorNbr[i];} 


    inline void Set_RingE_StripNbr(const unsigned short& RingE_StripNbr)
    {fTiaraHyball_RingE_StripNbr.push_back(RingE_StripNbr);}
    inline unsigned short Get_RingE_StripNbr(const unsigned int& i) const 
    {return fTiaraHyball_RingE_StripNbr[i];}

    inline void Set_RingE_Energy(const double& RingE_Energy)
    {fTiaraHyball_RingE_Energy.push_back(RingE_Energy);}
    inline double Get_RingE_Energy(const unsigned int& i) const 
    {return fTiaraHyball_RingE_Energy[i];}

    inline void Set_RingT_DetectorNbr(const unsigned short& RingT_DetectorNbr)
    {fTiaraHyball_RingT_DetectorNbr.push_back(RingT_DetectorNbr);}
    inline unsigned short Get_RingT_DetectorNbr(const unsigned int& i) const 
    {return fTiaraHyball_RingT_DetectorNbr[i];}

    inline void Set_RingT_StripNbr(const unsigned short& RingT_StripNbr)
    {fTiaraHyball_RingT_StripNbr.push_back(RingT_StripNbr);}
    inline unsigned short Get_RingT_StripNbr(const unsigned int& i) const 
    {return fTiaraHyball_RingT_StripNbr[i];}

    inline void Set_RingT_Time(const double& RingT_Time)
    {fTiaraHyball_RingT_Time.push_back(RingT_Time);}
    inline double Get_RingT_Time(const unsigned int& i) const 
    {return fTiaraHyball_RingT_Time[i];}

    inline void Set_SectorE_DetectorNbr(const unsigned short& SectorE_DetectorNbr)
    {fTiaraHyball_SectorE_DetectorNbr.push_back(SectorE_DetectorNbr);}
    inline unsigned short Get_SectorE_DetectorNbr(const unsigned int& i) const 
    {return fTiaraHyball_SectorE_DetectorNbr[i];}

    inline void Set_SectorE_StripNbr(const unsigned short& SectorE_StripNbr)
    {fTiaraHyball_SectorE_StripNbr.push_back(SectorE_StripNbr);}
    inline unsigned short Get_SectorE_StripNbr(const unsigned int& i) const 
    {return fTiaraHyball_SectorE_StripNbr[i];}

    inline void Set_SectorE_Energy(const double& SectorE_Energy)
    {fTiaraHyball_SectorE_Energy.push_back(SectorE_Energy);}
    inline double Get_SectorE_Energy(const unsigned int& i) const 
    {return fTiaraHyball_SectorE_Energy[i];}

    inline void Set_SectorT_DetectorNbr(const unsigned short& SectorT_DetectorNbr)
    {fTiaraHyball_SectorT_DetectorNbr.push_back(SectorT_DetectorNbr);}
    inline unsigned short Get_SectorT_DetectorNbr(const unsigned int& i) const 
    {return fTiaraHyball_SectorT_DetectorNbr[i];}

    inline void Set_SectorT_StripNbr(const unsigned short& SectorT_StripNbr)
    {fTiaraHyball_SectorT_StripNbr.push_back(SectorT_StripNbr);}
    inline unsigned short Get_SectorT_StripNbr(const unsigned int& i) const 
    {return fTiaraHyball_SectorT_StripNbr[i];}

    inline void Set_SectorT_Time(const double& SectorT_Time)
    {fTiaraHyball_SectorT_Time.push_back(SectorT_Time);}
    inline double Get_SectorT_Time(const unsigned int& i) const 
    {return fTiaraHyball_SectorT_Time[i];}


    ClassDef(TTiaraHyballData,1)  // TiaraData structure   
};

#endif

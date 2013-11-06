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
 * Creation Date  : November 2012                                            *
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
  private:
    vector<unsigned short>   fTiaraHyball_Ring_DetectorNbr;
    vector<unsigned short>   fTiaraHyball_Ring_StripNbr;
    vector<double>           fTiaraHyball_Ring_Energy;
    vector<double>           fTiaraHyball_Ring_Time;

    vector<unsigned short>   fTiaraHyball_Sector_DetectorNbr;
    vector<unsigned short>   fTiaraHyball_Sector_StripNbr;
    vector<double>           fTiaraHyball_Sector_Energy;
    vector<double>           fTiaraHyball_Sector_Time;

    ///////////// Fast Setters ////////////////////
  public:



    ///////////// Getter and Setter ////////////////
  public:
    inline void Set_Ring_DetectorNbr(unsigned short& Ring_DetectorNbr)
    {fTiaraHyball_Ring_DetectorNbr.push_back(Ring_DetectorNbr);}
    inline unsigned short Get_Ring_DetectorNbr(unsigned int& i)
    {return fTiaraHyball_Ring_DetectorNbr[i];}

    inline void Set_Ring_StripNbr(unsigned short& Ring_StripNbr)
    {fTiaraHyball_Ring_StripNbr.push_back(Ring_StripNbr);}
    inline unsigned short Get_Ring_StripNbr(unsigned int& i)
    {return fTiaraHyball_Ring_StripNbr[i];}

    inline void Set_Ring_Energy(double& Ring_Energy)
    {fTiaraHyball_Ring_Energy.push_back(Ring_Energy);}
    inline double Get_Ring_Energy(unsigned int& i)
    {return fTiaraHyball_Ring_Energy[i];}

    inline void Set_Ring_Time(double& Ring_Time)
    {fTiaraHyball_Ring_Time.push_back(Ring_Time);}
    inline double Get_Ring_Time(unsigned int& i)
    {return fTiaraHyball_Ring_Time[i];}

    inline void Set_Sector_DetectorNbr(unsigned short& Sector_DetectorNbr)
    {fTiaraHyball_Sector_DetectorNbr.push_back(Sector_DetectorNbr);}
    inline unsigned short Get_Sector_DetectorNbr(unsigned int& i)
    {return fTiaraHyball_Sector_DetectorNbr[i];}

    inline void Set_Sector_StripNbr(unsigned short& Sector_StripNbr)
    {fTiaraHyball_Sector_StripNbr.push_back(Sector_StripNbr);}
    inline unsigned short Get_Sector_StripNbr(unsigned int& i)
    {return fTiaraHyball_Sector_StripNbr[i];}

    inline void Set_Sector_Energy(double& Sector_Energy)
    {fTiaraHyball_Sector_Energy.push_back(Sector_Energy);}
    inline double Get_Sector_Energy(unsigned int& i)
    {return fTiaraHyball_Sector_Energy[i];}

    inline void Set_Sector_Time(double& Sector_Time)
    {fTiaraHyball_Sector_Time.push_back(Sector_Time);}
    inline double Get_Sector_Time(unsigned int& i)
    {return fTiaraHyball_Sector_Time[i];}


};

#endif

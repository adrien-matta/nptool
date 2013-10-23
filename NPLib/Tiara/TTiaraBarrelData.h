#ifndef __TiaraBarrelDATA__
#define __TiaraBarrelDATA__
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

class TTiaraBarrelData : public TNamed {
  private:
    // Tiara
    vector<unsigned short>   fTiaraBarrel_Front_Upstream_DetectorNbr;         
    vector<unsigned short>   fTiaraBarrel_Front_Upstream_StripNbr;         
    vector<double>           fTiaraBarrel_Front_Upstream_Energy;        
    vector<double>           fTiaraBarrel_Front_Upstream_Time;          

    vector<unsigned short>   fTiaraBarrel_Front_Downstream_DetectorNbr;     
    vector<unsigned short>   fTiaraBarrel_Front_Downstream_StripNbr;           
    vector<double>           fTiaraBarrel_Front_Downstream_Energy;          
    vector<double>           fTiaraBarrel_Front_Downstream_Time;            

    vector<unsigned short>   fTiaraBarrel_Back_DetectorNbr;                  
    vector<double>           fTiaraBarrel_Back_Energy;                    
    vector<double>           fTiaraBarrel_Back_Time;                      

    vector<unsigned short>   fTiaraBarrel_Outer_DetectorNbr;               
    vector<unsigned short>   fTiaraBarrel_Outer_StripNbr;                    
    vector<double>           fTiaraBarrel_Outer_Energy;                        
    vector<double>           fTiaraBarrel_Outer_Time;

  public:
    TTiaraBarrelData();
    virtual ~TTiaraBarrelData();

    void Clear();
    void Clear(const Option_t*) {};
    void Dump() const;

    /////////////////////           SETTERS           ////////////////////////
    inline void Set_Front_Upstream_DetectorNbr(const unsigned short& Front_Upstream_DetectorNbr)
    {fTiaraBarrel_Front_Upstream_DetectorNbr.push_back(Front_Upstream_DetectorNbr);}
    inline vector<unsigned short> Get_Front_Upstream_DetectorNbr()
    {return fTiaraBarrel_Front_Upstream_DetectorNbr;}

    inline void Set_Front_Upstream_StripNbr(const unsigned short& Front_Upstream_StripNbr)
    {fTiaraBarrel_Front_Upstream_StripNbr.push_back(Front_Upstream_StripNbr);}
    inline vector<unsigned short> Get_Front_Upstream_StripNbr()
    {return fTiaraBarrel_Front_Upstream_StripNbr;}

    inline void Set_Front_Upstream_Energy(const double& Front_Upstream_Energy)
    {fTiaraBarrel_Front_Upstream_Energy.push_back(Front_Upstream_Energy);}
    inline vector<double> Get_Front_Upstream_Energy()
    {return fTiaraBarrel_Front_Upstream_Energy;}

    inline void Set_Front_Upstream_Time(const double& Front_Upstream_Time)
    {fTiaraBarrel_Front_Upstream_Time.push_back(Front_Upstream_Time);}
    inline vector<double> Get_Front_Upstream_Time()
    {return fTiaraBarrel_Front_Upstream_Time;}

    inline void Set_Front_Downstream_DetectorNbr(const unsigned short& Front_Downstream_DetectorNbr)
    {fTiaraBarrel_Front_Downstream_DetectorNbr.push_back(Front_Downstream_DetectorNbr);}
    inline vector<unsigned short> Get_Front_Downstream_DetectorNbr()
    {return fTiaraBarrel_Front_Downstream_DetectorNbr;}


    inline void Set_Front_Downstream_StripNbr(const unsigned short& Front_Downstream_StripNbr)
    {fTiaraBarrel_Front_Downstream_StripNbr.push_back(Front_Downstream_StripNbr);}
    inline vector<unsigned short> Get_Front_Downstream_StripNbr()
    {return fTiaraBarrel_Front_Downstream_StripNbr;}

    inline void Set_Front_Downstream_Energy(const double& Front_Downstream_Energy)
    {fTiaraBarrel_Front_Downstream_Energy.push_back(Front_Downstream_Energy);}
    inline vector<double> Get_Front_Downstream_Energy()
    {return fTiaraBarrel_Front_Downstream_Energy;}

    inline void Set_Front_Downstream_Time(const double& Front_Downstream_Time)
    {fTiaraBarrel_Front_Downstream_Time.push_back(Front_Downstream_Time);}
    inline vector<double> Get_Front_Downstream_Time()
    {return fTiaraBarrel_Front_Downstream_Time;}

    inline void Set_Back_DetectorNbr(const unsigned short& Back_DetectorNbr)
    {fTiaraBarrel_Back_DetectorNbr.push_back(Back_DetectorNbr);}
    inline vector<unsigned short> Get_Back_DetectorNbr()
    {return fTiaraBarrel_Back_DetectorNbr;}

    inline void Set_Back_Energy(const double& Back_Energy)
    {fTiaraBarrel_Back_Energy.push_back(Back_Energy);}
    inline vector<double> Get_Back_Energy()
    {return fTiaraBarrel_Back_Energy;}

    inline void Set_Back_Time(const double& Back_Time)
    {fTiaraBarrel_Back_Time.push_back(Back_Time);}
    inline vector<double> Get_Back_Time()
    {return fTiaraBarrel_Back_Time;}

    inline void Set_Outer_DetectorNbr(const unsigned short& Outer_DetectorNbr)
    {fTiaraBarrel_Outer_DetectorNbr.push_back(Outer_DetectorNbr);}
    inline vector<unsigned short> Get_Outer_DetectorNbr()
    {return fTiaraBarrel_Outer_DetectorNbr;}

    inline void Set_Outer_StripNbr(const unsigned short& Outer_StripNbr)
    {fTiaraBarrel_Outer_StripNbr.push_back(Outer_StripNbr);}
    inline vector<unsigned short> Get_Outer_StripNbr()
    {return fTiaraBarrel_Outer_StripNbr;}

    inline void Set_Outer_Energy(const double& Outer_Energy)
    {fTiaraBarrel_Outer_Energy.push_back(Outer_Energy);}
    inline vector<double> Get_Outer_Energy()
    {return fTiaraBarrel_Outer_Energy;}

    inline void Set_Outer_Time(const double& Outer_Time)
    {fTiaraBarrel_Outer_Time.push_back(Outer_Time);}
    inline vector<double> Get_Outer_Time()
    {return fTiaraBarrel_Outer_Time;}

    ClassDef(TTiaraBarrelData,1)  // TiaraData structure
};

#endif

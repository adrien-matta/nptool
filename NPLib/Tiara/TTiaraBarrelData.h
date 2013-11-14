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
    vector<unsigned short>   fTiaraBarrel_FrontUpstreamE_DetectorNbr;         
    vector<unsigned short>   fTiaraBarrel_FrontUpstreamE_StripNbr;         
    vector<double>           fTiaraBarrel_FrontUpstreamE_Energy;        

    vector<unsigned short>   fTiaraBarrel_FrontUpstreamT_DetectorNbr;         
    vector<unsigned short>   fTiaraBarrel_FrontUpstreamT_StripNbr;         
    vector<double>           fTiaraBarrel_FrontUpstreamT_Time;          

    vector<unsigned short>   fTiaraBarrel_FrontDownstreamE_DetectorNbr;     
    vector<unsigned short>   fTiaraBarrel_FrontDownstreamE_StripNbr;           
    vector<double>           fTiaraBarrel_FrontDownstreamE_Energy;          

    vector<unsigned short>   fTiaraBarrel_FrontDownstreamT_DetectorNbr;     
    vector<unsigned short>   fTiaraBarrel_FrontDownstreamT_StripNbr;           
    vector<double>           fTiaraBarrel_FrontDownstreamT_Time;            

    vector<unsigned short>   fTiaraBarrel_BackE_DetectorNbr;                  
    vector<double>           fTiaraBarrel_BackE_Energy;                    

    vector<unsigned short>   fTiaraBarrel_BackT_DetectorNbr;                  
    vector<double>           fTiaraBarrel_BackT_Time;                      

    vector<unsigned short>   fTiaraBarrel_OuterE_DetectorNbr;               
    vector<unsigned short>   fTiaraBarrel_OuterE_StripNbr;                    
    vector<double>           fTiaraBarrel_OuterE_Energy;                        

    vector<unsigned short>   fTiaraBarrel_OuterT_DetectorNbr;               
    vector<unsigned short>   fTiaraBarrel_OuterT_StripNbr;                    
    vector<double>           fTiaraBarrel_OuterT_Time;

  public:
    TTiaraBarrelData();
    virtual ~TTiaraBarrelData();

    void Clear();
    void Clear(const Option_t*) {};
    void Dump() const;
    /////////////////////        FAST SETTERS         ////////////////////////
    // E
    inline void Set_FrontE_Upstream(const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Energy){
      Set_FrontUpstreamE_DetectorNbr(DetNbr);
      Set_FrontUpstreamE_StripNbr(StripNbr);
      Set_FrontUpstreamE_Energy(Energy);
    }

    // T
    inline void Set_FrontT_Upstream(const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Time){
      Set_FrontUpstreamT_DetectorNbr(DetNbr);
      Set_FrontUpstreamT_StripNbr(StripNbr);
      Set_FrontUpstreamT_Time(Time);
    }

    ///////////////////// 
    // E
    inline void Set_FrontE_Downstreamm( const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Energy){
      Set_FrontDownstreamE_DetectorNbr(DetNbr);
      Set_FrontDownstreamE_StripNbr(StripNbr);
      Set_FrontDownstreamE_Energy(Energy);
    }

    // T
    inline void Set_FrontT_Downstreamm( const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Time){
      Set_FrontDownstreamT_DetectorNbr(DetNbr);
      Set_FrontDownstreamT_StripNbr(StripNbr);
      Set_FrontDownstreamT_Time(Time);
    }

    ///////////////////// 
    // E
    inline void Set_BackE( const unsigned short& DetNbr , const double& Energy){
      Set_BackE_DetectorNbr(DetNbr);
      Set_BackE_Energy(Energy);
    }

    // T
    inline void Set_BackT( const unsigned short& DetNbr , const double& Time){
      Set_BackT_DetectorNbr(DetNbr);
      Set_BackT_Time(Time);
    }

    ///////////////////// 
    // E 
    inline void Set_OuterE( const unsigned short& DetNbr , const unsigned short StripNbr, const double& Energy){

      Set_OuterE_DetectorNbr(DetNbr);
      Set_OuterE_StripNbr(StripNbr);
      Set_OuterE_Energy(Energy);
    }

    // T 
    inline void Set_OuterT( const unsigned short& DetNbr , const unsigned short StripNbr , const double& Time){

      Set_OuterT_DetectorNbr(DetNbr);
      Set_OuterT_StripNbr(StripNbr);
      Set_OuterT_Time(Time);
    }



    //////////////////////     SETTERS And GETTERS    /////////////////////////
    inline void Set_FrontUpstreamE_DetectorNbr(const unsigned short& FrontUpstreamE_DetectorNbr)
    {fTiaraBarrel_FrontUpstreamE_DetectorNbr.push_back(FrontUpstreamE_DetectorNbr);}
    inline unsigned short Get_FrontUpstreamE_DetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamE_DetectorNbr[i];}

    inline void Set_FrontUpstreamE_StripNbr(const unsigned short& FrontUpstreamE_StripNbr)
    {fTiaraBarrel_FrontUpstreamE_StripNbr.push_back(FrontUpstreamE_StripNbr);}
    inline unsigned short Get_FrontUpstreamE_StripNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamE_StripNbr[i];}

    inline void Set_FrontUpstreamE_Energy(const double& FrontUpstreamE_Energy)
    {fTiaraBarrel_FrontUpstreamE_Energy.push_back(FrontUpstreamE_Energy);}
    inline double Get_FrontUpstreamE_Energy(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamE_Energy[i];}

    inline void Set_FrontUpstreamT_DetectorNbr(const unsigned short& FrontUpstreamT_DetectorNbr)
    {fTiaraBarrel_FrontUpstreamT_DetectorNbr.push_back(FrontUpstreamT_DetectorNbr);}
    inline unsigned short Get_FrontUpstreamT_DetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamT_DetectorNbr[i];}

    inline void Set_FrontUpstreamT_StripNbr(const unsigned short& FrontUpstreamT_StripNbr)
    {fTiaraBarrel_FrontUpstreamT_StripNbr.push_back(FrontUpstreamT_StripNbr);}
    inline unsigned short Get_FrontUpstreamT_StripNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamT_StripNbr[i];}

    inline void Set_FrontUpstreamT_Time(const double& FrontUpstreamT_Time)
    {fTiaraBarrel_FrontUpstreamT_Time.push_back(FrontUpstreamT_Time);}
    inline double Get_FrontUpstreamT_Time(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamT_Time[i];}

    inline void Set_FrontDownstreamE_DetectorNbr(const unsigned short& FrontDownstreamE_DetectorNbr)
    {fTiaraBarrel_FrontDownstreamE_DetectorNbr.push_back(FrontDownstreamE_DetectorNbr);}
    inline unsigned short Get_FrontDownstreamE_DetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamE_DetectorNbr[i];}

    inline void Set_FrontDownstreamE_StripNbr(const unsigned short& FrontDownstreamE_StripNbr)
    {fTiaraBarrel_FrontDownstreamE_StripNbr.push_back(FrontDownstreamE_StripNbr);}
    inline unsigned short Get_FrontDownstreamE_StripNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamE_StripNbr[i];}

    inline void Set_FrontDownstreamE_Energy(const double& FrontDownstreamE_Energy)
    {fTiaraBarrel_FrontDownstreamE_Energy.push_back(FrontDownstreamE_Energy);}
    inline double Get_FrontDownstreamE_Energy(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamE_Energy[i];}

    inline void Set_FrontDownstreamT_DetectorNbr(const unsigned short& FrontDownstreamT_DetectorNbr)
    {fTiaraBarrel_FrontDownstreamT_DetectorNbr.push_back(FrontDownstreamT_DetectorNbr);}
    inline unsigned short Get_FrontDownstreamT_DetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamT_DetectorNbr[i];}

    inline void Set_FrontDownstreamT_StripNbr(const unsigned short& FrontDownstreamT_StripNbr)
    {fTiaraBarrel_FrontDownstreamT_StripNbr.push_back(FrontDownstreamT_StripNbr);}
    inline unsigned short Get_FrontDownstreamT_StripNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamT_StripNbr[i];}

    inline void Set_FrontDownstreamT_Time(const double& FrontDownstreamT_Time)
    {fTiaraBarrel_FrontDownstreamT_Time.push_back(FrontDownstreamT_Time);}
    inline double Get_FrontDownstreamT_Time(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamT_Time[i];}

    inline void Set_BackE_DetectorNbr(const unsigned short& BackE_DetectorNbr)
    {fTiaraBarrel_BackE_DetectorNbr.push_back(BackE_DetectorNbr);}
    inline unsigned short Get_BackE_DetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_BackE_DetectorNbr[i];}

    inline void Set_BackE_Energy(const double& BackE_Energy)
    {fTiaraBarrel_BackE_Energy.push_back(BackE_Energy);}
    inline double Get_BackE_Energy(const unsigned int& i)
    {return fTiaraBarrel_BackE_Energy[i];}

    inline void Set_BackT_DetectorNbr(const unsigned short& BackT_DetectorNbr)
    {fTiaraBarrel_BackT_DetectorNbr.push_back(BackT_DetectorNbr);}
    inline unsigned short Get_BackT_DetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_BackT_DetectorNbr[i];}

    inline void Set_BackT_Time(const double& BackT_Time)
    {fTiaraBarrel_BackT_Time.push_back(BackT_Time);}
    inline double Get_BackT_Time(const unsigned int& i)
    {return fTiaraBarrel_BackT_Time[i];}

    inline void Set_OuterE_DetectorNbr(const unsigned short& OuterE_DetectorNbr)
    {fTiaraBarrel_OuterE_DetectorNbr.push_back(OuterE_DetectorNbr);}
    inline unsigned short Get_OuterE_DetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_OuterE_DetectorNbr[i];}

    inline void Set_OuterE_StripNbr(const unsigned short& OuterE_StripNbr)
    {fTiaraBarrel_OuterE_StripNbr.push_back(OuterE_StripNbr);}
    inline unsigned short Get_OuterE_StripNbr(const unsigned int& i)
    {return fTiaraBarrel_OuterE_StripNbr[i];}

    inline void Set_OuterE_Energy(const double& OuterE_Energy)
    {fTiaraBarrel_OuterE_Energy.push_back(OuterE_Energy);}
    inline double Get_OuterE_Energy(const unsigned int& i)
    {return fTiaraBarrel_OuterE_Energy[i];}


    inline void Set_OuterT_DetectorNbr(const unsigned short& OuterT_DetectorNbr)
    {fTiaraBarrel_OuterT_DetectorNbr.push_back(OuterT_DetectorNbr);}
    inline unsigned short Get_OuterT_DetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_OuterT_DetectorNbr[i];}

    inline void Set_OuterT_StripNbr(const unsigned short& OuterT_StripNbr)
    {fTiaraBarrel_OuterT_StripNbr.push_back(OuterT_StripNbr);}
    inline unsigned short Get_OuterT_StripNbr(const unsigned int& i)
    {return fTiaraBarrel_OuterT_StripNbr[i];}

    inline void Set_OuterT_Time(const double& OuterT_Time)
    {fTiaraBarrel_OuterT_Time.push_back(OuterT_Time);}
    inline double Get_OuterT_Time(const unsigned int& i)
    {return fTiaraBarrel_OuterT_Time[i];}






    ClassDef(TTiaraBarrelData,1)  // TiaraData structure
};

#endif

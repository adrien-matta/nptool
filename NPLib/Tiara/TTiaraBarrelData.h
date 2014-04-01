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

    //////////////////        MULTIPLICITY GETTER         /////////////////////
    // E
    inline unsigned int GetFrontUpstreamEMult(){
      return fTiaraBarrel_FrontUpstreamE_DetectorNbr.size();
    }

    // T
    inline unsigned int GetFrontUpstreamTMult(){
      return fTiaraBarrel_FrontUpstreamT_DetectorNbr.size();
    }

    ///////////////////// 
    // E
    inline unsigned int GetFrontDownstreamEMult(){
      return fTiaraBarrel_FrontDownstreamE_DetectorNbr.size();
    }

    // T
     inline unsigned int GetFrontDownstreamTMult(){
      return fTiaraBarrel_FrontDownstreamT_DetectorNbr.size();
    }
    
    ///////////////////// 
    // E
    inline unsigned int GetBackEMult(){
      return fTiaraBarrel_BackE_DetectorNbr.size();
    }

    // T
    inline unsigned int GetBackTMult(){
      return fTiaraBarrel_BackT_DetectorNbr.size();
    }

    ///////////////////// 
     // E
    inline unsigned int GetOuterEMult(){
      return fTiaraBarrel_OuterE_DetectorNbr.size();
    }

    // T
    inline unsigned int GetOuterTMult(){
      return fTiaraBarrel_OuterT_DetectorNbr.size();
    }
   
    /////////////////////        FAST SETTERS         ////////////////////////
    // E
    inline void SetFrontUpstreamE(const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Energy){
      SetFrontUpstreamEDetectorNbr(DetNbr);
      SetFrontUpstreamEStripNbr(StripNbr);
      SetFrontUpstreamEEnergy(Energy);
    }

    // T
    inline void SetFrontUpstreamT(const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Time){
      SetFrontUpstreamTDetectorNbr(DetNbr);
      SetFrontUpstreamTStripNbr(StripNbr);
      SetFrontUpstreamTTime(Time);
    }

    ///////////////////// 
    // E
    inline void SetFrontDownstreamE( const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Energy){
      SetFrontDownstreamEDetectorNbr(DetNbr);
      SetFrontDownstreamEStripNbr(StripNbr);
      SetFrontDownstreamEEnergy(Energy);
    }

    // T
    inline void SetFrontDownstreamT( const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Time){
      SetFrontDownstreamTDetectorNbr(DetNbr);
      SetFrontDownstreamTStripNbr(StripNbr);
      SetFrontDownstreamTTime(Time);
    }

    ///////////////////// 
    // E
    inline void SetBackE( const unsigned short& DetNbr , const double& Energy){
      SetBackEDetectorNbr(DetNbr);
      SetBackEEnergy(Energy);
    }

    // T
    inline void SetBackT( const unsigned short& DetNbr , const double& Time){
      SetBackTDetectorNbr(DetNbr);
      SetBackTTime(Time);
    }

    ///////////////////// 
    // E 
    inline void SetOuterE( const unsigned short& DetNbr , const unsigned short StripNbr, const double& Energy){
      SetOuterEDetectorNbr(DetNbr);
      SetOuterEStripNbr(StripNbr);
      SetOuterEEnergy(Energy);
    }

    // T 
    inline void SetOuterT( const unsigned short& DetNbr , const unsigned short StripNbr , const double& Time){
      SetOuterTDetectorNbr(DetNbr);
      SetOuterTStripNbr(StripNbr);
      SetOuterTTime(Time);
    }



    //////////////////////     SETTERS And GETTERS    /////////////////////////
    inline void SetFrontUpstreamEDetectorNbr(const unsigned short& FrontUpstreamE_DetectorNbr)
    {fTiaraBarrel_FrontUpstreamE_DetectorNbr.push_back(FrontUpstreamE_DetectorNbr);}
    inline unsigned short GetFrontUpstreamEDetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamE_DetectorNbr[i];}

    inline void SetFrontUpstreamEStripNbr(const unsigned short& FrontUpstreamE_StripNbr)
    {fTiaraBarrel_FrontUpstreamE_StripNbr.push_back(FrontUpstreamE_StripNbr);}
    inline unsigned short GetFrontUpstreamEStripNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamE_StripNbr[i];}

    inline void SetFrontUpstreamEEnergy(const double& FrontUpstreamE_Energy)
    {fTiaraBarrel_FrontUpstreamE_Energy.push_back(FrontUpstreamE_Energy);}
    inline double GetFrontUpstreamEEnergy(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamE_Energy[i];}

    inline void SetFrontUpstreamTDetectorNbr(const unsigned short& FrontUpstreamT_DetectorNbr)
    {fTiaraBarrel_FrontUpstreamT_DetectorNbr.push_back(FrontUpstreamT_DetectorNbr);}
    inline unsigned short GetFrontUpstreamTDetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamT_DetectorNbr[i];}

    inline void SetFrontUpstreamTStripNbr(const unsigned short& FrontUpstreamT_StripNbr)
    {fTiaraBarrel_FrontUpstreamT_StripNbr.push_back(FrontUpstreamT_StripNbr);}
    inline unsigned short GetFrontUpstreamTStripNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamT_StripNbr[i];}

    inline void SetFrontUpstreamTTime(const double& FrontUpstreamT_Time)
    {fTiaraBarrel_FrontUpstreamT_Time.push_back(FrontUpstreamT_Time);}
    inline double GetFrontUpstreamTTime(const unsigned int& i)
    {return fTiaraBarrel_FrontUpstreamT_Time[i];}

    inline void SetFrontDownstreamEDetectorNbr(const unsigned short& FrontDownstreamE_DetectorNbr)
    {fTiaraBarrel_FrontDownstreamE_DetectorNbr.push_back(FrontDownstreamE_DetectorNbr);}
    inline unsigned short GetFrontDownstreamEDetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamE_DetectorNbr[i];}

    inline void SetFrontDownstreamEStripNbr(const unsigned short& FrontDownstreamE_StripNbr)
    {fTiaraBarrel_FrontDownstreamE_StripNbr.push_back(FrontDownstreamE_StripNbr);}
    inline unsigned short GetFrontDownstreamEStripNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamE_StripNbr[i];}

    inline void SetFrontDownstreamEEnergy(const double& FrontDownstreamE_Energy)
    {fTiaraBarrel_FrontDownstreamE_Energy.push_back(FrontDownstreamE_Energy);}
    inline double GetFrontDownstreamEEnergy(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamE_Energy[i];}

    inline void SetFrontDownstreamTDetectorNbr(const unsigned short& FrontDownstreamT_DetectorNbr)
    {fTiaraBarrel_FrontDownstreamT_DetectorNbr.push_back(FrontDownstreamT_DetectorNbr);}
    inline unsigned short GetFrontDownstreamTDetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamT_DetectorNbr[i];}

    inline void SetFrontDownstreamTStripNbr(const unsigned short& FrontDownstreamT_StripNbr)
    {fTiaraBarrel_FrontDownstreamT_StripNbr.push_back(FrontDownstreamT_StripNbr);}
    inline unsigned short GetFrontDownstreamTStripNbr(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamT_StripNbr[i];}

    inline void SetFrontDownstreamTTime(const double& FrontDownstreamT_Time)
    {fTiaraBarrel_FrontDownstreamT_Time.push_back(FrontDownstreamT_Time);}
    inline double GetFrontDownstreamTTime(const unsigned int& i)
    {return fTiaraBarrel_FrontDownstreamT_Time[i];}

    inline void SetBackEDetectorNbr(const unsigned short& BackE_DetectorNbr)
    {fTiaraBarrel_BackE_DetectorNbr.push_back(BackE_DetectorNbr);}
    inline unsigned short GetBackEDetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_BackE_DetectorNbr[i];}

    inline void SetBackEEnergy(const double& BackE_Energy)
    {fTiaraBarrel_BackE_Energy.push_back(BackE_Energy);}
    inline double GetBackEEnergy(const unsigned int& i)
    {return fTiaraBarrel_BackE_Energy[i];}

    inline void SetBackTDetectorNbr(const unsigned short& BackT_DetectorNbr)
    {fTiaraBarrel_BackT_DetectorNbr.push_back(BackT_DetectorNbr);}
    inline unsigned short GetBackTDetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_BackT_DetectorNbr[i];}

    inline void SetBackTTime(const double& BackT_Time)
    {fTiaraBarrel_BackT_Time.push_back(BackT_Time);}
    inline double GetBackTTime(const unsigned int& i)
    {return fTiaraBarrel_BackT_Time[i];}

    inline void SetOuterEDetectorNbr(const unsigned short& OuterE_DetectorNbr)
    {fTiaraBarrel_OuterE_DetectorNbr.push_back(OuterE_DetectorNbr);}
    inline unsigned short GetOuterEDetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_OuterE_DetectorNbr[i];}

    inline void SetOuterEStripNbr(const unsigned short& OuterE_StripNbr)
    {fTiaraBarrel_OuterE_StripNbr.push_back(OuterE_StripNbr);}
    inline unsigned short GetOuterEStripNbr(const unsigned int& i)
    {return fTiaraBarrel_OuterE_StripNbr[i];}

    inline void SetOuterEEnergy(const double& OuterE_Energy)
    {fTiaraBarrel_OuterE_Energy.push_back(OuterE_Energy);}
    inline double GetOuterEEnergy(const unsigned int& i)
    {return fTiaraBarrel_OuterE_Energy[i];}


    inline void SetOuterTDetectorNbr(const unsigned short& OuterT_DetectorNbr)
    {fTiaraBarrel_OuterT_DetectorNbr.push_back(OuterT_DetectorNbr);}
    inline unsigned short GetOuterTDetectorNbr(const unsigned int& i)
    {return fTiaraBarrel_OuterT_DetectorNbr[i];}

    inline void SetOuterTStripNbr(const unsigned short& OuterT_StripNbr)
    {fTiaraBarrel_OuterT_StripNbr.push_back(OuterT_StripNbr);}
    inline unsigned short GetOuterTStripNbr(const unsigned int& i)
    {return fTiaraBarrel_OuterT_StripNbr[i];}

    inline void SetOuterTTime(const double& OuterT_Time)
    {fTiaraBarrel_OuterT_Time.push_back(OuterT_Time);}
    inline double GetOuterTTime(const unsigned int& i)
    {return fTiaraBarrel_OuterT_Time[i];}


    ClassDef(TTiaraBarrelData,1)  // TiaraData structure
};

#endif

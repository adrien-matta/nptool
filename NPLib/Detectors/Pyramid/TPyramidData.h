#ifndef __PyramidDATA__
#define __PyramidDATA__
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: mohamad.moukaddam@iphc.cnrs.fr                           *
 *                                                                           *
 * Creation Date  : November 2018                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Pyramid Raw data                                    *
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

class TPyramidData : public TNamed {
  private:
    // Pyramid
    vector<unsigned short>   fPyramid_FrontUpstreamE_DetectorNbr;         
    vector<unsigned short>   fPyramid_FrontUpstreamE_StripNbr;         
    vector<double>           fPyramid_FrontUpstreamE_Energy;        

    vector<unsigned short>   fPyramid_FrontUpstreamT_DetectorNbr;         
    vector<unsigned short>   fPyramid_FrontUpstreamT_StripNbr;         
    vector<double>           fPyramid_FrontUpstreamT_Time;          

    vector<unsigned short>   fPyramid_FrontDownstreamE_DetectorNbr;     
    vector<unsigned short>   fPyramid_FrontDownstreamE_StripNbr;           
    vector<double>           fPyramid_FrontDownstreamE_Energy;          

    vector<unsigned short>   fPyramid_FrontDownstreamT_DetectorNbr;     
    vector<unsigned short>   fPyramid_FrontDownstreamT_StripNbr;           
    vector<double>           fPyramid_FrontDownstreamT_Time;            

    vector<unsigned short>   fPyramid_BackE_DetectorNbr;                  
    vector<double>           fPyramid_BackE_Energy;                    

    vector<unsigned short>   fPyramid_BackT_DetectorNbr;                  
    vector<double>           fPyramid_BackT_Time;                      

    vector<unsigned short>   fPyramid_OuterE_DetectorNbr;               
    vector<unsigned short>   fPyramid_OuterE_StripNbr;                    
    vector<double>           fPyramid_OuterE_Energy;                        

    vector<unsigned short>   fPyramid_OuterT_DetectorNbr;               
    vector<unsigned short>   fPyramid_OuterT_StripNbr;                    
    vector<double>           fPyramid_OuterT_Time;

  public:
    TPyramidData();
    virtual ~TPyramidData();

    void Clear();
    void Clear(const Option_t*) {};
    void Dump() const;

    //////////////////        MULTIPLICITY GETTER         /////////////////////
    // E
    inline unsigned int GetFrontUpstreamEMult(){
      return fPyramid_FrontUpstreamE_DetectorNbr.size();
    }//!

    // T
    inline unsigned int GetFrontUpstreamTMult(){
      return fPyramid_FrontUpstreamT_DetectorNbr.size();
    }//!

    ///////////////////// 
    // E
    inline unsigned int GetFrontDownstreamEMult(){
      return fPyramid_FrontDownstreamE_DetectorNbr.size();
    }//!

    // T
     inline unsigned int GetFrontDownstreamTMult(){
      return fPyramid_FrontDownstreamT_DetectorNbr.size();
    }//!
    
    ///////////////////// 
    // E
    inline unsigned int GetBackEMult(){
      return fPyramid_BackE_DetectorNbr.size();
    }//!

    // T
    inline unsigned int GetBackTMult(){
      return fPyramid_BackT_DetectorNbr.size();
    }//!

    ///////////////////// 
     // E
    inline unsigned int GetOuterEMult(){
      return fPyramid_OuterE_DetectorNbr.size();
    }//!

    // T
    inline unsigned int GetOuterTMult(){
      return fPyramid_OuterT_DetectorNbr.size();
    }//!
   
    /////////////////////        FAST SETTERS         ////////////////////////
    // E
    inline void SetFrontUpstreamE(const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Energy){
      SetFrontUpstreamEDetectorNbr(DetNbr);
      SetFrontUpstreamEStripNbr(StripNbr);
      SetFrontUpstreamEEnergy(Energy);
    }//!

    // T
    inline void SetFrontUpstreamT(const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Time){
      SetFrontUpstreamTDetectorNbr(DetNbr);
      SetFrontUpstreamTStripNbr(StripNbr);
      SetFrontUpstreamTTime(Time);
    }//!

    ///////////////////// 
    // E
    inline void SetFrontDownstreamE( const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Energy){
      SetFrontDownstreamEDetectorNbr(DetNbr);
      SetFrontDownstreamEStripNbr(StripNbr);
      SetFrontDownstreamEEnergy(Energy);
    }//!

    // T
    inline void SetFrontDownstreamT( const unsigned short& DetNbr , const unsigned short& StripNbr , const double& Time){
      SetFrontDownstreamTDetectorNbr(DetNbr);
      SetFrontDownstreamTStripNbr(StripNbr);
      SetFrontDownstreamTTime(Time);
    }//!

    ///////////////////// 
    // E
    inline void SetBackE( const unsigned short& DetNbr , const double& Energy){
      SetBackEDetectorNbr(DetNbr);
      SetBackEEnergy(Energy);
    }//!

    // T
    inline void SetBackT( const unsigned short& DetNbr , const double& Time){
      SetBackTDetectorNbr(DetNbr);
      SetBackTTime(Time);
    }//!

    ///////////////////// 
    // E 
    inline void SetOuterE( const unsigned short& DetNbr , const unsigned short StripNbr, const double& Energy){
      SetOuterEDetectorNbr(DetNbr);
      SetOuterEStripNbr(StripNbr);
      SetOuterEEnergy(Energy);
    }//!

    // T 
    inline void SetOuterT( const unsigned short& DetNbr , const unsigned short StripNbr , const double& Time){
      SetOuterTDetectorNbr(DetNbr);
      SetOuterTStripNbr(StripNbr);
      SetOuterTTime(Time);
    }//!



    //////////////////////     SETTERS And GETTERS    /////////////////////////
    inline void SetFrontUpstreamEDetectorNbr(const unsigned short& FrontUpstreamE_DetectorNbr)
    {fPyramid_FrontUpstreamE_DetectorNbr.push_back(FrontUpstreamE_DetectorNbr);}//!
    inline unsigned short GetFrontUpstreamEDetectorNbr(const unsigned int& i)
    {return fPyramid_FrontUpstreamE_DetectorNbr[i];}//!

    inline void SetFrontUpstreamEStripNbr(const unsigned short& FrontUpstreamE_StripNbr)
    {fPyramid_FrontUpstreamE_StripNbr.push_back(FrontUpstreamE_StripNbr);}//!
    inline unsigned short GetFrontUpstreamEStripNbr(const unsigned int& i)
    {return fPyramid_FrontUpstreamE_StripNbr[i];}//!

    inline void SetFrontUpstreamEEnergy(const double& FrontUpstreamE_Energy)
    {fPyramid_FrontUpstreamE_Energy.push_back(FrontUpstreamE_Energy);}//!
    inline double GetFrontUpstreamEEnergy(const unsigned int& i)
    {return fPyramid_FrontUpstreamE_Energy[i];}//!

    inline void SetFrontUpstreamTDetectorNbr(const unsigned short& FrontUpstreamT_DetectorNbr)
    {fPyramid_FrontUpstreamT_DetectorNbr.push_back(FrontUpstreamT_DetectorNbr);}//!
    inline unsigned short GetFrontUpstreamTDetectorNbr(const unsigned int& i)
    {return fPyramid_FrontUpstreamT_DetectorNbr[i];}//!

    inline void SetFrontUpstreamTStripNbr(const unsigned short& FrontUpstreamT_StripNbr)
    {fPyramid_FrontUpstreamT_StripNbr.push_back(FrontUpstreamT_StripNbr);}//!
    inline unsigned short GetFrontUpstreamTStripNbr(const unsigned int& i)
    {return fPyramid_FrontUpstreamT_StripNbr[i];}//!

    inline void SetFrontUpstreamTTime(const double& FrontUpstreamT_Time)
    {fPyramid_FrontUpstreamT_Time.push_back(FrontUpstreamT_Time);}//!
    inline double GetFrontUpstreamTTime(const unsigned int& i)
    {return fPyramid_FrontUpstreamT_Time[i];}//!

    inline void SetFrontDownstreamEDetectorNbr(const unsigned short& FrontDownstreamE_DetectorNbr)
    {fPyramid_FrontDownstreamE_DetectorNbr.push_back(FrontDownstreamE_DetectorNbr);}//!
    inline unsigned short GetFrontDownstreamEDetectorNbr(const unsigned int& i)
    {return fPyramid_FrontDownstreamE_DetectorNbr[i];}//!

    inline void SetFrontDownstreamEStripNbr(const unsigned short& FrontDownstreamE_StripNbr)
    {fPyramid_FrontDownstreamE_StripNbr.push_back(FrontDownstreamE_StripNbr);}//!
    inline unsigned short GetFrontDownstreamEStripNbr(const unsigned int& i)
    {return fPyramid_FrontDownstreamE_StripNbr[i];}//!

    inline void SetFrontDownstreamEEnergy(const double& FrontDownstreamE_Energy)
    {fPyramid_FrontDownstreamE_Energy.push_back(FrontDownstreamE_Energy);}//!
    inline double GetFrontDownstreamEEnergy(const unsigned int& i)
    {return fPyramid_FrontDownstreamE_Energy[i];}//!

    inline void SetFrontDownstreamTDetectorNbr(const unsigned short& FrontDownstreamT_DetectorNbr)
    {fPyramid_FrontDownstreamT_DetectorNbr.push_back(FrontDownstreamT_DetectorNbr);}//!
    inline unsigned short GetFrontDownstreamTDetectorNbr(const unsigned int& i)
    {return fPyramid_FrontDownstreamT_DetectorNbr[i];}//!

    inline void SetFrontDownstreamTStripNbr(const unsigned short& FrontDownstreamT_StripNbr)
    {fPyramid_FrontDownstreamT_StripNbr.push_back(FrontDownstreamT_StripNbr);}//!
    inline unsigned short GetFrontDownstreamTStripNbr(const unsigned int& i)
    {return fPyramid_FrontDownstreamT_StripNbr[i];}//!

    inline void SetFrontDownstreamTTime(const double& FrontDownstreamT_Time)
    {fPyramid_FrontDownstreamT_Time.push_back(FrontDownstreamT_Time);}//!
    inline double GetFrontDownstreamTTime(const unsigned int& i)
    {return fPyramid_FrontDownstreamT_Time[i];}//!

    inline void SetBackEDetectorNbr(const unsigned short& BackE_DetectorNbr)
    {fPyramid_BackE_DetectorNbr.push_back(BackE_DetectorNbr);}//!
    inline unsigned short GetBackEDetectorNbr(const unsigned int& i)
    {return fPyramid_BackE_DetectorNbr[i];}//!

    inline void SetBackEEnergy(const double& BackE_Energy)
    {fPyramid_BackE_Energy.push_back(BackE_Energy);}//!
    inline double GetBackEEnergy(const unsigned int& i)
    {return fPyramid_BackE_Energy[i];}//!

    inline void SetBackTDetectorNbr(const unsigned short& BackT_DetectorNbr)
    {fPyramid_BackT_DetectorNbr.push_back(BackT_DetectorNbr);}//!
    inline unsigned short GetBackTDetectorNbr(const unsigned int& i)
    {return fPyramid_BackT_DetectorNbr[i];}//!

    inline void SetBackTTime(const double& BackT_Time)
    {fPyramid_BackT_Time.push_back(BackT_Time);}//!
    inline double GetBackTTime(const unsigned int& i)
    {return fPyramid_BackT_Time[i];}//!

    inline void SetOuterEDetectorNbr(const unsigned short& OuterE_DetectorNbr)
    {fPyramid_OuterE_DetectorNbr.push_back(OuterE_DetectorNbr);}//!
    inline unsigned short GetOuterEDetectorNbr(const unsigned int& i)
    {return fPyramid_OuterE_DetectorNbr[i];}//!

    inline void SetOuterEStripNbr(const unsigned short& OuterE_StripNbr)
    {fPyramid_OuterE_StripNbr.push_back(OuterE_StripNbr);}//!
    inline unsigned short GetOuterEStripNbr(const unsigned int& i)
    {return fPyramid_OuterE_StripNbr[i];}//!

    inline void SetOuterEEnergy(const double& OuterE_Energy)
    {fPyramid_OuterE_Energy.push_back(OuterE_Energy);}//!
    inline double GetOuterEEnergy(const unsigned int& i)
    {return fPyramid_OuterE_Energy[i];}//!


    inline void SetOuterTDetectorNbr(const unsigned short& OuterT_DetectorNbr)
    {fPyramid_OuterT_DetectorNbr.push_back(OuterT_DetectorNbr);}//!
    inline unsigned short GetOuterTDetectorNbr(const unsigned int& i)
    {return fPyramid_OuterT_DetectorNbr[i];}//!

    inline void SetOuterTStripNbr(const unsigned short& OuterT_StripNbr)
    {fPyramid_OuterT_StripNbr.push_back(OuterT_StripNbr);}//!
    inline unsigned short GetOuterTStripNbr(const unsigned int& i)
    {return fPyramid_OuterT_StripNbr[i];}//!

    inline void SetOuterTTime(const double& OuterT_Time)
    {fPyramid_OuterT_Time.push_back(OuterT_Time);}//!
    inline double GetOuterTTime(const unsigned int& i)
    {return fPyramid_OuterT_Time[i];}//!


    ClassDef(TPyramidData,1)  // PyramidData structure
};

#endif

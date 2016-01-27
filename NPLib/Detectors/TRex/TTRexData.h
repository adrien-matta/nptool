#ifndef __TRexDATA__
#define __TRexDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : January 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TRex Raw data                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// STL
#include <vector>
using namespace std;

// ROOT
#include "TObject.h"

class TTRexData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Front
    // Energy
    vector<UShort_t>   fTRexFront_E_DetectorNbr;
    vector<UShort_t>   fTRexFront_E_StripNbr;
    vector<Double_t>   fTRexFront_Energy;

    // Time
    vector<UShort_t>   fTRexFront_T_DetectorNbr;
    vector<UShort_t>   fTRexFront_T_StripNbr;
    vector<Double_t>   fTRexFront_Time;


    // Back
    // Energy
    vector<UShort_t>   fTRexBack_E_DetectorNbr;
    vector<UShort_t>   fTRexBack_E_StripNbr;
    vector<Double_t>   fTRexBack_Energy;

    // Time
    vector<UShort_t>   fTRexBack_T_DetectorNbr;
    vector<UShort_t>   fTRexBack_T_StripNbr;
    vector<Double_t>   fTRexBack_Time;

    // PAD
    // Energy
    vector<UShort_t>   fTRexPAD_E_DetectorNbr;
    vector<Double_t>   fTRexPAD_Energy;

    // Time
    vector<UShort_t>   fTRexPAD_T_DetectorNbr;
    vector<Double_t>   fTRexPAD_Time;



  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TTRexData();
    virtual ~TTRexData();
    

  //////////////////////////////////////////////////////////////
  // Inherited from TObject and overriden to avoid warnings
  public:
    void Clear();
    void Clear(const Option_t*) {};
    void Dump() const;


  //////////////////////////////////////////////////////////////
  // Getters and Setters
  // Prefer inline declaration to avoid unnecessary called of 
  // frequently used methods
  // add //! to avoid ROOT creating dictionnary for the methods
  public:
    //////////////////////    SETTERS    ////////////////////////
    // Front
    // Energy
    inline void SetFrontE_DetectorNbr(const UShort_t& DetNbr)
      {fTRexFront_E_DetectorNbr.push_back(DetNbr);} //!
    inline void SetFrontE_StripNbr(const UShort_t& StripNbr)
      {fTRexFront_E_StripNbr.push_back(StripNbr);} //!
    inline void SetFront_Energy(const Double_t& Energy)
      {fTRexFront_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetFrontEnergy(const UShort_t& DetNbr,const UShort_t& StripNbr, const Double_t& Energy) {
      SetFrontE_DetectorNbr(DetNbr);
      SetFrontE_StripNbr(StripNbr);
      SetFront_Energy(Energy);
    };//!

    // Time
    inline void SetFrontT_DetectorNbr(const UShort_t& DetNbr)
      {fTRexFront_T_DetectorNbr.push_back(DetNbr);} //!
     inline void SetFrontT_StripNbr(const UShort_t& StripNbr)
      {fTRexFront_T_StripNbr.push_back(StripNbr);} //!
    inline void SetFront_Time(const Double_t& Time)
      {fTRexFront_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetFrontTime(const UShort_t& DetNbr,const UShort_t& StripNbr,const Double_t& Time)	{
      SetFrontT_DetectorNbr(DetNbr);
      SetFrontT_StripNbr(StripNbr);
      SetFront_Time(Time);
    };//!

    // Back
    // Energy
    inline void SetBackE_DetectorNbr(const UShort_t& DetNbr)
      {fTRexBack_E_DetectorNbr.push_back(DetNbr);} //!
    inline void SetBackE_StripNbr(const UShort_t& StripNbr)
      {fTRexBack_E_StripNbr.push_back(StripNbr);} //!
    inline void SetBack_Energy(const Double_t& Energy)
      {fTRexBack_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetBackEnergy(const UShort_t& DetNbr,const UShort_t& StripNbr, const Double_t& Energy) {
      SetBackE_DetectorNbr(DetNbr);
      SetBackE_StripNbr(StripNbr);
      SetBack_Energy(Energy);
    };//!

    // Time
    inline void SetBackT_DetectorNbr(const UShort_t& DetNbr)
      {fTRexBack_T_DetectorNbr.push_back(DetNbr);} //!
     inline void SetBackT_StripNbr(const UShort_t& StripNbr)
      {fTRexBack_T_StripNbr.push_back(StripNbr);} //!
    inline void SetBack_Time(const Double_t& Time)
      {fTRexBack_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetBackTime(const UShort_t& DetNbr,const UShort_t& StripNbr,const Double_t& Time)	{
      SetBackT_DetectorNbr(DetNbr);
      SetBackT_StripNbr(StripNbr);
      SetBack_Time(Time);
    };//!

    // PAD
    // Energy
    inline void SetPADE_DetectorNbr(const UShort_t& DetNbr)
      {fTRexPAD_E_DetectorNbr.push_back(DetNbr);} //!
    inline void SetPAD_Energy(const Double_t& Energy)
      {fTRexPAD_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetPADEnergy(const UShort_t& DetNbr, const Double_t& Energy) {
      SetPADE_DetectorNbr(DetNbr);
      SetPAD_Energy(Energy);
    };//!

    // Time
    inline void SetPADT_DetectorNbr(const UShort_t& DetNbr)
      {fTRexPAD_T_DetectorNbr.push_back(DetNbr);} //!
    inline void SetPAD_Time(const Double_t& Time)
      {fTRexPAD_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetPADTime(const UShort_t& DetNbr,const Double_t& Time)	{
      SetPADT_DetectorNbr(DetNbr);
      SetPAD_Time(Time);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // Front
    // Energy
    inline UShort_t GetFrontMultEnergy() const
      {return fTRexFront_E_DetectorNbr.size();}
    inline UShort_t GetFrontE_DetectorNbr(const unsigned int &i) const 
      {return fTRexFront_E_DetectorNbr[i];}//!
    inline UShort_t GetFrontE_StripNbr(const unsigned int &i) const 
      {return fTRexFront_E_StripNbr[i];}//!
    inline Double_t GetFront_Energy(const unsigned int &i) const 
      {return fTRexFront_Energy[i];}//!

    // Time
    inline UShort_t GetFrontMultTime() const
      {return fTRexFront_T_DetectorNbr.size();}
    inline UShort_t GetFrontT_DetectorNbr(const unsigned int &i) const 
      {return fTRexFront_T_DetectorNbr[i];}//!
    inline UShort_t GetFrontT_StripNbr(const unsigned int &i) const 
      {return fTRexFront_T_StripNbr[i];}//!
    inline Double_t GetFront_Time(const unsigned int &i) const 
      {return fTRexFront_Time[i];}//!

    // Back
    // Energy
    inline UShort_t GetBackMultEnergy() const
      {return fTRexBack_E_DetectorNbr.size();}
    inline UShort_t GetBackE_DetectorNbr(const unsigned int &i) const 
      {return fTRexBack_E_DetectorNbr[i];}//!
    inline UShort_t GetBackE_StripNbr(const unsigned int &i) const 
      {return fTRexBack_E_StripNbr[i];}//!
    inline Double_t GetBack_Energy(const unsigned int &i) const 
      {return fTRexBack_Energy[i];}//!

    // Time
    inline UShort_t GetBackMultTime() const
      {return fTRexBack_T_DetectorNbr.size();}
    inline UShort_t GetBackT_DetectorNbr(const unsigned int &i) const 
      {return fTRexBack_T_DetectorNbr[i];}//!
    inline UShort_t GetBackT_StripNbr(const unsigned int &i) const 
      {return fTRexBack_T_StripNbr[i];}//!
    inline Double_t GetBack_Time(const unsigned int &i) const 
      {return fTRexBack_Time[i];}//!

    // PAD
    // Energy
    inline UShort_t GetPADMultEnergy() const
      {return fTRexPAD_E_DetectorNbr.size();}
    inline UShort_t GetPADE_DetectorNbr(const unsigned int &i) const 
      {return fTRexPAD_E_DetectorNbr[i];}//!
    inline Double_t GetPAD_Energy(const unsigned int &i) const 
      {return fTRexPAD_Energy[i];}//!

    // Time
    inline UShort_t GetPADMultTime() const
      {return fTRexPAD_T_DetectorNbr.size();}
    inline UShort_t GetPADT_DetectorNbr(const unsigned int &i) const 
      {return fTRexPAD_T_DetectorNbr[i];}//!
    inline Double_t GetPAD_Time(const unsigned int &i) const 
      {return fTRexPAD_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TTRexData,1)  // TRexData structure
};

#endif

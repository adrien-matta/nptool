#ifndef __HeliosDATA__
#define __HeliosDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : octobre 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Helios Raw data                                         *
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

class THeliosData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<UShort_t>   fHelios_EBack_DetectorNbr;
    vector<Double_t>   fHelios_EBack_Energy;
    vector<UShort_t>   fHelios_EUp_DetectorNbr;
    vector<Double_t>   fHelios_EUp_Energy;
    vector<UShort_t>   fHelios_EDw_DetectorNbr;
    vector<Double_t>   fHelios_EDw_Energy;


    // Time
    vector<UShort_t>   fHelios_TBack_DetectorNbr;
    vector<Double_t>   fHelios_TBack_Time;
    vector<UShort_t>   fHelios_TUp_DetectorNbr;
    vector<Double_t>   fHelios_TUp_Time;
    vector<UShort_t>   fHelios_TDw_DetectorNbr;
    vector<Double_t>   fHelios_TDw_Time;



  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    THeliosData();
    virtual ~THeliosData();
    

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
    // E Back
    inline void SetEBack_DetectorNbr(const UShort_t& DetNbr)
      {fHelios_EBack_DetectorNbr.push_back(DetNbr);} //!
    inline void SetEBack_Energy(const Double_t& Energy)
      {fHelios_EBack_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetEBack(const UShort_t& DetNbr,const Double_t& Energy) {
      SetEBack_DetectorNbr(DetNbr);
      SetEBack_Energy(Energy);
    };//!

    // E Up
    inline void SetEUp_DetectorNbr(const UShort_t& DetNbr)
      {fHelios_EUp_DetectorNbr.push_back(DetNbr);} //!
    inline void SetEUp_Energy(const Double_t& Energy)
      {fHelios_EUp_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetEUp(const UShort_t& DetNbr,const Double_t& Energy) {
      SetEUp_DetectorNbr(DetNbr);
      SetEUp_Energy(Energy);
    };//!

    // E Dw
    inline void SetEDw_DetectorNbr(const UShort_t& DetNbr)
      {fHelios_EDw_DetectorNbr.push_back(DetNbr);} //!
    inline void SetEDw_Energy(const Double_t& Energy)
      {fHelios_EDw_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetEDw(const UShort_t& DetNbr,const Double_t& Energy) {
      SetEDw_DetectorNbr(DetNbr);
      SetEDw_Energy(Energy);
    };//!


    // T Back
    inline void SetTBack_DetectorNbr(const UShort_t& DetNbr)
      {fHelios_TBack_DetectorNbr.push_back(DetNbr);} //!
    inline void SetTBack_Time(const Double_t& Time)
      {fHelios_TBack_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetTBack(const UShort_t& DetNbr,const Double_t& Time)	{
      SetTBack_DetectorNbr(DetNbr);
      SetTBack_Time(Time);
    };//!


    // T Up
    inline void SetTUp_DetectorNbr(const UShort_t& DetNbr)
      {fHelios_TUp_DetectorNbr.push_back(DetNbr);} //!
    inline void SetTUp_Time(const Double_t& Time)
      {fHelios_TUp_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetTUp(const UShort_t& DetNbr,const Double_t& Time)	{
      SetTUp_DetectorNbr(DetNbr);
      SetTUp_Time(Time);
    };//!

    // T Dw
    inline void SetTDw_DetectorNbr(const UShort_t& DetNbr)
      {fHelios_TDw_DetectorNbr.push_back(DetNbr);} //!
    inline void SetTDw_Time(const Double_t& Time)
      {fHelios_TDw_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetTDw(const UShort_t& DetNbr,const Double_t& Time)	{
      SetTDw_DetectorNbr(DetNbr);
      SetTDw_Time(Time);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // E Back
    inline UShort_t GetMultEBack() const
      {return fHelios_EBack_DetectorNbr.size();}
    inline UShort_t GetEBack_DetectorNbr(const unsigned int &i) const 
      {return fHelios_EBack_DetectorNbr[i];}//!
    inline Double_t GetEBack_Energy(const unsigned int &i) const 
      {return fHelios_EBack_Energy[i];}//!

    // E Up
    inline UShort_t GetMultEUp() const
      {return fHelios_EUp_DetectorNbr.size();}
    inline UShort_t GetEUp_DetectorNbr(const unsigned int &i) const 
      {return fHelios_EUp_DetectorNbr[i];}//!
    inline Double_t GetEUp_Energy(const unsigned int &i) const 
      {return fHelios_EUp_Energy[i];}//!
    
    // E Dw
    inline UShort_t GetMultEDw() const
      {return fHelios_EDw_DetectorNbr.size();}
    inline UShort_t GetEDw_DetectorNbr(const unsigned int &i) const 
      {return fHelios_EDw_DetectorNbr[i];}//!
    inline Double_t GetEDw_Energy(const unsigned int &i) const 
      {return fHelios_EDw_Energy[i];}//!

    // T Back
    inline UShort_t GetMultTBack() const
      {return fHelios_TBack_DetectorNbr.size();}
    inline UShort_t GetTBack_DetectorNbr(const unsigned int &i) const 
      {return fHelios_TBack_DetectorNbr[i];}//!
    inline Double_t GetTBack_Time(const unsigned int &i) const 
      {return fHelios_TBack_Time[i];}//!

    // T Up
    inline UShort_t GetMultTUp() const
      {return fHelios_TUp_DetectorNbr.size();}
    inline UShort_t GetTUp_DetectorNbr(const unsigned int &i) const 
      {return fHelios_TUp_DetectorNbr[i];}//!
    inline Double_t GetTUp_Time(const unsigned int &i) const 
      {return fHelios_TUp_Time[i];}//!

    // T Dw
    inline UShort_t GetMultTDw() const
      {return fHelios_TDw_DetectorNbr.size();}
    inline UShort_t GetTDw_DetectorNbr(const unsigned int &i) const 
      {return fHelios_TDw_DetectorNbr[i];}//!
    inline Double_t GetTDw_Time(const unsigned int &i) const 
      {return fHelios_TDw_Time[i];}//!

  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(THeliosData,1)  // HeliosData structure
};

#endif

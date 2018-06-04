#ifndef __SpedeDATA__
#define __SpedeDATA__
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Daniel Cox  contact address: daniel.m.cox@jyu.fi                        *
 *                                                                           *
 * Creation Date  : January 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Spede Raw data                                    *
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

class TSpedeData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<UShort_t>   fSpede_E_DetectorNbr;
    vector<UShort_t>   fSpede_E_PixelNbr;
    vector<Double_t>   fSpede_Energy;

    // Time
    vector<UShort_t>   fSpede_T_DetectorNbr;
    vector<UShort_t>   fSpede_T_PixelNbr;
    vector<Double_t>   fSpede_Time;


  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TSpedeData();
    virtual ~TSpedeData();
    

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
    // Energy
    inline void SetE_DetectorNbr(const UShort_t& DetNbr) {fSpede_E_DetectorNbr.push_back(DetNbr);} //!
    inline void SetE_PixelNbr(const UShort_t& PixNbr)	 {fSpede_E_PixelNbr.push_back(PixNbr);} //!
    inline void Set_Energy(const Double_t& Energy) 		 {fSpede_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetEnergy(const UShort_t& DetNbr,const UShort_t& PixNbr,const Double_t& Energy) {
      SetE_DetectorNbr(DetNbr);
      SetE_PixelNbr(PixNbr);
      Set_Energy(Energy);
    };//!

    // Time
    inline void SetT_DetectorNbr(const UShort_t& DetNbr) {fSpede_T_DetectorNbr.push_back(DetNbr);} //!
    inline void SetT_PixelNbr(const UShort_t& PixNbr) 	 {fSpede_T_PixelNbr.push_back(PixNbr);} //!
    inline void Set_Time(const Double_t& Time) {fSpede_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetTime(const UShort_t& DetNbr,const UShort_t& PixNbr,const Double_t& Time)	{
      SetT_DetectorNbr(DetNbr);
      SetT_PixelNbr(PixNbr);
      Set_Time(Time);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const						 	{return fSpede_E_DetectorNbr.size();}//!
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 	{return fSpede_E_DetectorNbr[i];}//!
    inline UShort_t GetE_PixelNbr(const unsigned int &i) const 		{return fSpede_E_PixelNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 		{return fSpede_Energy[i];}//!

    // Time
    inline UShort_t GetMultTime() const 							{return fSpede_T_DetectorNbr.size();}//!
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 	{return fSpede_T_DetectorNbr[i];}//!
    inline UShort_t GetT_PixelNbr(const unsigned int &i) const 		{return fSpede_T_PixelNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 			{return fSpede_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TSpedeData,1)  // SpedeData structure
};

#endif

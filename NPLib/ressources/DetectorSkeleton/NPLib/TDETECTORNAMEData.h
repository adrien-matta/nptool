#ifndef __DETECTORNAMEDATA__
#define __DETECTORNAMEDATA__
/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold DETECTORNAME Raw data                                    *
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


class TDETECTORNAMEData : public TObject 
{
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<UShort_t>   fDETECTORNAME_E_DetectorNbr;
    vector<Double_t>   fDETECTORNAME_Energy;

    // Time
    vector<UShort_t>   fDETECTORNAME_T_DetectorNbr;
    vector<Double_t>   fDETECTORNAME_Time;


  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TDETECTORNAMEData();
    virtual ~TDETECTORNAMEData();
    

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
    inline void SetE_DetectorNbr(const UShort_t& DetNbr)
      {fDETECTORNAME_E_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Energy(const Double_t& Energy)
      {fDETECTORNAME_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetEnergy(const UShort_t& DetNbr,const Double_t& Energy) {
      SetE_DetectorNbr(DetNbr);
      Set_Energy(Energy);
    };//!

    // Time
    inline void SetT_DetectorNbr(const UShort_t& DetNbr)
      {fDETECTORNAME_T_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Time(const Double_t& Time)
      {fDETECTORNAME_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetTime(const UShort_t& DetNbr,const Double_t& Time)	{
      SetT_DetectorNbr(DetNbr);
      Set_Time(Time);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const
      {return fDETECTORNAME_E_DetectorNbr.size();}
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fDETECTORNAME_E_DetectorNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fDETECTORNAME_Energy[i];}//!

    // Time
    inline UShort_t GetMultTime() const
      {return fDETECTORNAME_T_DetectorNbr.size();}
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fDETECTORNAME_T_DetectorNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fDETECTORNAME_Time[i];}//!


    // Required for ROOT dictionnary
    ClassDef(TDETECTORNAMEData,1)  // DETECTORNAMEData structure
};

#endif

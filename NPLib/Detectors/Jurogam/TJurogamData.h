#ifndef __JurogamDATA__
#define __JurogamDATA__
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
 *  This class hold Jurogam Raw data                                    *
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

class TJurogamData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<UShort_t>   fJurogam_E_DetectorNbr;
    vector<Double_t>   fJurogam_Energy;

    // Time
    vector<UShort_t>   fJurogam_T_DetectorNbr;
    vector<Double_t>   fJurogam_Time;

	// BGO
	// Energy
    vector<UShort_t>   fJurogamBGO_E_DetectorNbr;
    vector<Double_t>   fJurogamBGO_Energy;
	// Time
    vector<UShort_t>   fJurogamBGO_T_DetectorNbr;
    vector<Double_t>   fJurogamBGO_Time;


  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TJurogamData();
    virtual ~TJurogamData();
    

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
      {fJurogam_E_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Energy(const Double_t& Energy)
      {fJurogam_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetEnergy(const UShort_t& DetNbr,const Double_t& Energy) {
      SetE_DetectorNbr(DetNbr);
      Set_Energy(Energy);
    };//!

    // Time
    inline void SetT_DetectorNbr(const UShort_t& DetNbr)
      {fJurogam_T_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Time(const Double_t& Time)
      {fJurogam_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetTime(const UShort_t& DetNbr,const Double_t& Time)	{
      SetT_DetectorNbr(DetNbr);
      Set_Time(Time);
    };//!
	// BGO
    // Energy
    inline void Set_BGO_E_DetectorNbr(const UShort_t& DetNbr)
      {fJurogamBGO_E_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_BGO_Energy(const Double_t& Energy)
      {fJurogamBGO_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void Set_BGO_Energy(const UShort_t& DetNbr,const Double_t& Energy) {
      Set_BGO_E_DetectorNbr(DetNbr);
      Set_BGO_Energy(Energy);
    };//!

    // Time
    inline void Set_BGO_T_DetectorNbr(const UShort_t& DetNbr)
      {fJurogamBGO_T_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_BGO_Time(const Double_t& Time)
      {fJurogamBGO_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void Set_BGO_Time(const UShort_t& DetNbr,const Double_t& Time)	{
      Set_BGO_T_DetectorNbr(DetNbr);
      Set_BGO_Time(Time);
    };//!

    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const
      {return fJurogam_E_DetectorNbr.size();}
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fJurogam_E_DetectorNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fJurogam_Energy[i];}//!

    // Time
    inline UShort_t GetMultTime() const
      {return fJurogam_T_DetectorNbr.size();}
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fJurogam_T_DetectorNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fJurogam_Time[i];}//!

	// BGO
    // Energy
    inline UShort_t Get_BGO_MultEnergy() const
      {return fJurogamBGO_E_DetectorNbr.size();}
    inline UShort_t Get_BGO_E_DetectorNbr(const unsigned int &i) const 
      {return fJurogamBGO_E_DetectorNbr[i];}//!
    inline Double_t Get_BGO_Energy(const unsigned int &i) const 
      {return fJurogamBGO_Energy[i];}//!

    // Time
    inline UShort_t Get_BGO_MultTime() const
      {return fJurogamBGO_T_DetectorNbr.size();}
    inline UShort_t Get_BGO_T_DetectorNbr(const unsigned int &i) const 
      {return fJurogamBGO_T_DetectorNbr[i];}//!
    inline Double_t Get_BGO_Time(const unsigned int &i) const 
      {return fJurogamBGO_Time[i];}//!
	

  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TJurogamData,1)  // JurogamData structure
};

#endif

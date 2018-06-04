#ifndef __MiniballDATA__
#define __MiniballDATA__
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
 *  This class hold Miniball Raw data                                        *
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
#include "TLorentzVector.h"

class TMiniballData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<UShort_t>   fMiniball_E_DetectorNbr;
    vector<Double_t>   fMiniball_Energy;

    // Crystal
    vector<UShort_t>   fMiniball_C_DetectorNbr;
    vector<UShort_t>   fMiniball_Crystal;

    // Angle
    vector<UShort_t>   fMiniball_A_DetectorNbr;
    vector<TLorentzVector>   fMiniball_Angle;

    // Time
    vector<UShort_t>   fMiniball_T_DetectorNbr;
    vector<Double_t>   fMiniball_Time;


  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TMiniballData();
    virtual ~TMiniballData();
    

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
      {fMiniball_E_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Energy(const Double_t& Energy)
      {fMiniball_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetEnergy(const UShort_t& DetNbr,const Double_t& Energy) {
      SetE_DetectorNbr(DetNbr);
      Set_Energy(Energy);
    };//!

    // Crystal
    inline void SetC_DetectorNbr(const UShort_t& DetNbr)
      {fMiniball_C_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Crystal(const UShort_t& Crystal)
      {fMiniball_Crystal.push_back(Crystal);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetCrystal(const UShort_t& DetNbr,const UShort_t& Crystal) {
      SetC_DetectorNbr(DetNbr);
      Set_Crystal(Crystal);
    };//!

    // Angle
    inline void SetA_DetectorNbr(const UShort_t& DetNbr)
      {fMiniball_A_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Angle(const TLorentzVector& Angle)
      {fMiniball_Angle.push_back(Angle);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetAngle(const UShort_t& DetNbr,const TLorentzVector& Angle) {
      SetA_DetectorNbr(DetNbr);
      Set_Angle(Angle);
    };//!


    // Time
    inline void SetT_DetectorNbr(const UShort_t& DetNbr)
      {fMiniball_T_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Time(const Double_t& Time)
      {fMiniball_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetTime(const UShort_t& DetNbr,const Double_t& Time)	{
      SetT_DetectorNbr(DetNbr);
      Set_Time(Time);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const
      {return fMiniball_E_DetectorNbr.size();}
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fMiniball_E_DetectorNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fMiniball_Energy[i];}//!

    // Crystal
    inline UShort_t GetMultCrystal() const
      {return fMiniball_C_DetectorNbr.size();}
    inline UShort_t GetC_DetectorNbr(const unsigned int &i) const 
      {return fMiniball_C_DetectorNbr[i];}//!
    inline Double_t Get_Crystal(const unsigned int &i) const 
      {return fMiniball_Crystal[i];}//!

    // Angle
    inline UShort_t GetMultAngle() const
      {return fMiniball_A_DetectorNbr.size();}
    inline UShort_t GetA_DetectorNbr(const unsigned int &i) const 
      {return fMiniball_A_DetectorNbr[i];}//!
    inline TLorentzVector Get_Angle(const unsigned int &i) const 
      {return fMiniball_Angle[i];}//!


    // Time
    inline UShort_t GetMultTime() const
      {return fMiniball_T_DetectorNbr.size();}
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fMiniball_T_DetectorNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fMiniball_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TMiniballData,1)  // MiniballData structure
};

#endif

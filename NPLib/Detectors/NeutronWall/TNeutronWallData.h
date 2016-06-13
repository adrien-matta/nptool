#ifndef __NeutronWallDATA__
#define __NeutronWallDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : June 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold NeutronWall Raw data                                    *
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

class TNeutronWallData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<UShort_t>   fNeutronWall_E_DetectorNbr;
    vector<Double_t>   fNeutronWall_Energy;

    // Time
    vector<UShort_t>   fNeutronWall_T_DetectorNbr;
    vector<Double_t>   fNeutronWall_Time;


  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TNeutronWallData();
    virtual ~TNeutronWallData();
    

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
      {fNeutronWall_E_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Energy(const Double_t& Energy)
      {fNeutronWall_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetEnergy(const UShort_t& DetNbr,const Double_t& Energy) {
      SetE_DetectorNbr(DetNbr);
      Set_Energy(Energy);
    };//!

    // Time
    inline void SetT_DetectorNbr(const UShort_t& DetNbr)
      {fNeutronWall_T_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Time(const Double_t& Time)
      {fNeutronWall_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetTime(const UShort_t& DetNbr,const Double_t& Time)	{
      SetT_DetectorNbr(DetNbr);
      Set_Time(Time);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const
      {return fNeutronWall_E_DetectorNbr.size();}
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fNeutronWall_E_DetectorNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fNeutronWall_Energy[i];}//!

    // Time
    inline UShort_t GetMultTime() const
      {return fNeutronWall_T_DetectorNbr.size();}
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fNeutronWall_T_DetectorNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fNeutronWall_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TNeutronWallData,1)  // NeutronWallData structure
};

#endif

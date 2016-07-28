#ifndef __NeutronWallDATA__
#define __NeutronWallDATA__
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu *
 *                                                                           *
 * Creation Date  : June 2016                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold NeutronWall Raw data                                     *
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
    vector<UShort_t>   fNeutronWall_E_PadNbr;
    vector<Double_t>   fNeutronWall_Energy;

    // Time
    vector<UShort_t>   fNeutronWall_T_DetectorNbr;
    vector<UShort_t>   fNeutronWall_T_PadNbr;
    vector<Double_t>   fNeutronWall_Time;
    
    // Postion
    vector<UShort_t> fNeutronWall_Position_X;
    vector<UShort_t> fNeutronWall_Position_Y;
    vector<UShort_t> fNeutronWall_Position_Z;
    
    // Energy
    vector<UShort_t>   fVetoWall_E_DetectorNbr;
    vector<UShort_t>   fVetoWall_E_PadNbr;
    vector<Double_t>   fVetoWall_Energy;

    // Time
    vector<UShort_t>   fVetoWall_T_DetectorNbr;
    vector<UShort_t>   fVetoWall_T_PadNbr;
    vector<Double_t>   fVetoWall_Time;
    
    // Postion
    vector<UShort_t> fVetoWall_Position_X;
    vector<UShort_t> fVetoWall_Position_Y;
    vector<UShort_t> fVetoWall_Position_Z;


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
    //////////////////////    SETTERS NEUTRON WALL   ////////////////////////
    // Energy
    inline void SetE_DetectorNbr(const UShort_t& DetNbr)
      {fNeutronWall_E_DetectorNbr.push_back(DetNbr);} //!
    inline void SetE_PadNbr(const UShort_t& PadNbr)
      {fNeutronWall_E_PadNbr.push_back(PadNbr);} //!
    inline void Set_Energy(const Double_t& Energy)
      {fNeutronWall_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetEnergy(const UShort_t& DetNbr, const UShort_t& PadNbr, const Double_t& Energy) {
      SetE_DetectorNbr(DetNbr);
      SetE_PadNbr(PadNbr);
      Set_Energy(Energy);
    };//!

    // Time
    inline void SetT_DetectorNbr(const UShort_t& DetNbr)
      {fNeutronWall_T_DetectorNbr.push_back(DetNbr);} //!
    inline void SetT_PadNbr(const UShort_t& PadNbr)
      {fNeutronWall_T_PadNbr.push_back(PadNbr);} //!
    inline void Set_Time(const Double_t& Time)
      {fNeutronWall_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetTime(const UShort_t& DetNbr,const UShort_t& PadNbr,const Double_t& Time)	{
      SetT_DetectorNbr(DetNbr);
      SetT_PadNbr(PadNbr);
      Set_Time(Time);
    };//!
    
    // Position
    inline void Set_NeutronWall_PosX(const UShort_t& PosX)
    {fNeutronWall_Position_X.push_back(PosX);} //!
    inline void Set_NeutronWall_PosY(const UShort_t& PosY)
    {fNeutronWall_Position_Y.push_back(PosY);} //!
    inline void Set_NeutronWall_PosZ(const UShort_t& PosZ)
    {fNeutronWall_Position_Z.push_back(PosZ);} //!

    //////////////////////    SETTERS VETO WALL   ////////////////////////
    // Energy
    inline void SetE_VetoDetectorNbr(const UShort_t& DetNbr)
      {fVetoWall_E_DetectorNbr.push_back(DetNbr);} //!
    inline void SetE_VetoPadNbr(const UShort_t& PadNbr)
      {fVetoWall_E_PadNbr.push_back(PadNbr);} //!
    inline void Set_VetoEnergy(const Double_t& Energy)
      {fVetoWall_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetVetoEnergy(const UShort_t& DetNbr,const UShort_t& PadNbr,const Double_t& Energy) {
      SetE_VetoDetectorNbr(DetNbr);
      SetE_VetoPadNbr(PadNbr);
      Set_VetoEnergy(Energy);
    };//!

    // Time
    inline void SetT_VetoDetectorNbr(const UShort_t& DetNbr)
      {fVetoWall_T_DetectorNbr.push_back(DetNbr);} //!
    inline void SetT_VetoPadNbr(const UShort_t& PadNbr)
      {fVetoWall_T_PadNbr.push_back(PadNbr);} //!
    inline void Set_VetoTime(const Double_t& Time)
      {fVetoWall_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetVetoTime(const UShort_t& DetNbr,const UShort_t& PadNbr,const Double_t& Time)	{
      SetT_VetoDetectorNbr(DetNbr);
      SetT_VetoPadNbr(PadNbr);
      Set_VetoTime(Time);
    };//!
    
    // Position
    inline void Set_VetoWall_PosX(const UShort_t& PosX)
    {fVetoWall_Position_X.push_back(PosX);} //!
    inline void Set_VetoWall_PosY(const UShort_t& PosY)
    {fVetoWall_Position_Y.push_back(PosY);} //!
    inline void Set_VetoWall_PosZ(const UShort_t& PosZ)
    {fVetoWall_Position_Z.push_back(PosZ);} //!


    //////////////////////    GETTERS NEUTRON WALL    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const
      {return fNeutronWall_E_DetectorNbr.size();}
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fNeutronWall_E_DetectorNbr[i];}//!
    inline UShort_t GetE_PadNbr(const unsigned int &i) const 
      {return fNeutronWall_E_PadNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fNeutronWall_Energy[i];}//!

    // Time
    inline UShort_t GetMultTime() const
      {return fNeutronWall_T_DetectorNbr.size();}
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fNeutronWall_T_DetectorNbr[i];}//!
    inline UShort_t GetT_PadNbr(const unsigned int &i) const 
      {return fNeutronWall_T_PadNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fNeutronWall_Time[i];}//!
    
    // Position
    inline UShort_t GetMult_NeutronWall_Position() const
    {return fNeutronWall_Position_X.size();}
    inline UShort_t Get_NeutronWall_PosX(const unsigned int &i) const
    {return fNeutronWall_Position_X[i];}//!
    inline UShort_t Get_NeutronWall_PosY(const unsigned int &i) const
    {return fNeutronWall_Position_Y[i];}//!
    inline UShort_t Get_NeutronWall_PosZ(const unsigned int &i) const
    {return fNeutronWall_Position_Z[i];}//!

    //////////////////////    GETTERS VETO WALL    ////////////////////////
    // Energy
    inline UShort_t GetVetoMultEnergy() const
      {return fVetoWall_E_PadNbr.size();}
    inline UShort_t GetE_VetoDetectorNbr(const unsigned int &i) const 
      {return fVetoWall_E_DetectorNbr[i];}//!
    inline UShort_t GetE_VetoPadNbr(const unsigned int &i) const 
      {return fVetoWall_E_PadNbr[i];}//!
    inline Double_t Get_VetoEnergy(const unsigned int &i) const 
      {return fVetoWall_Energy[i];}//!

    // Time
    inline UShort_t GetVetoMultTime() const
      {return fVetoWall_T_PadNbr.size();}
    inline UShort_t GetT_VetoDetectorNbr(const unsigned int &i) const 
      {return fVetoWall_T_DetectorNbr[i];}//!
    inline UShort_t GetT_VetoPadNbr(const unsigned int &i) const 
      {return fVetoWall_T_PadNbr[i];}//!
    inline Double_t Get_VetoTime(const unsigned int &i) const 
      {return fVetoWall_Time[i];}//!
    
    // Position
    inline UShort_t GetMult_VetoWall_Position() const
    {return fVetoWall_Position_X.size();}
    inline UShort_t Get_VetoWall_PosX(const unsigned int &i) const
    {return fVetoWall_Position_X[i];}//!
    inline UShort_t Get_VetoWall_PosY(const unsigned int &i) const
    {return fVetoWall_Position_Y[i];}//!
    inline UShort_t Get_VetoWall_PosZ(const unsigned int &i) const
    {return fVetoWall_Position_Z[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TNeutronWallData,1)  // NeutronWallData structure
};

#endif

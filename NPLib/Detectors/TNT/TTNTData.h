#ifndef __TNTDATA__
#define __TNTDATA__
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Dustin Scriven  contact address: scrivend@tamu.edu                        *
 *                                                                           *
 * Creation Date  : July 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TNT Raw data                                    *
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

class TTNTData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<Double_t>   fTNT_E_DetectorNbr;
    vector<Double_t>   fTNT_Energy;

    // Time
    vector<Double_t>   fTNT_T_DetectorNbr;
    vector<Double_t>   fTNT_Time;


  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TTNTData();
    ~TTNTData();
    

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
    //inline void SetEnergy(const Double_t& DetNbr,const Double_t& Energy){
    inline void SetEnergy(const ULong_t& DetNbr,const Double_t& Energy){
      fTNT_E_DetectorNbr.push_back(DetNbr);
      fTNT_Energy.push_back(Energy);
    };//!

    // Time
    //inline void SetTime(const Double_t& DetNbr,const Double_t& Time)    { 
    inline void SetTime(const ULong_t& DetNbr,const Double_t& Time)	{
      fTNT_T_DetectorNbr.push_back(DetNbr);     
      fTNT_Time.push_back(Time);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline Double_t GetMultEnergy() const
      {return fTNT_E_DetectorNbr.size();}
    inline Double_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fTNT_E_DetectorNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fTNT_Energy[i];}//!

    // Time
    inline Double_t GetMultTime() const
      {return fTNT_T_DetectorNbr.size();}
    inline Double_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fTNT_T_DetectorNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fTNT_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TTNTData,1)  // TNTData structure
};

#endif

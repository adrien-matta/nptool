#ifndef __beam_dumpDATA__
#define __beam_dumpDATA__
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: valerian  contact address: girardalcindor@ganil.fr                        *
 *                                                                           *
 * Creation Date  : November 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold beam_dump Raw data                                    *
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

class Tbeam_dumpData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<UShort_t>   fbeam_dump_E_DetectorNbr;
    vector<Double_t>   fbeam_dump_Energy;

    // Time
    vector<UShort_t>   fbeam_dump_T_DetectorNbr;
    vector<Double_t>   fbeam_dump_Time;


  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    Tbeam_dumpData();
    ~Tbeam_dumpData();
    

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
    inline void SetEnergy(const UShort_t& DetNbr,const Double_t& Energy){
      fbeam_dump_E_DetectorNbr.push_back(DetNbr);
      fbeam_dump_Energy.push_back(Energy);
    };//!

    // Time
    inline void SetTime(const UShort_t& DetNbr,const Double_t& Time)	{
      fbeam_dump_T_DetectorNbr.push_back(DetNbr);     
      fbeam_dump_Time.push_back(Time);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const
      {return fbeam_dump_E_DetectorNbr.size();}
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fbeam_dump_E_DetectorNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fbeam_dump_Energy[i];}//!

    // Time
    inline UShort_t GetMultTime() const
      {return fbeam_dump_T_DetectorNbr.size();}
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fbeam_dump_T_DetectorNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fbeam_dump_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(Tbeam_dumpData,1)  // beam_dumpData structure
};

#endif

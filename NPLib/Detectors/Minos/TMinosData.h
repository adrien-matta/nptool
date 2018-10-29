#ifndef __MinosDATA__
#define __MinosDATA__
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Elidiano Tronchin  contact address: tronchin@lpccaen.in2p3.fr                        *
 *                                                                           *
 * Creation Date  : October 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Minos Raw data                                    *
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

class TMinosData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<UShort_t>   fMinos_E_DetectorNbr;
    vector<Double_t>   fMinos_Energy;

    // Time
    vector<UShort_t>   fMinos_T_DetectorNbr;
    vector<Double_t>   fMinos_Time;

    // maybe directions with angle varagles have to be added?
    
  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TMinosData();
    ~TMinosData();
    

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
      fMinos_E_DetectorNbr.push_back(DetNbr);
      fMinos_Energy.push_back(Energy);
    };//!

    // Time
    inline void SetTime(const UShort_t& DetNbr,const Double_t& Time)	{
      fMinos_T_DetectorNbr.push_back(DetNbr);     
      fMinos_Time.push_back(Time);
    };//!
    // (E&T)
    inline void SetEnergyAndTime(const UShort_t& DetNbr,const Double_t& Energy,const Double_t& Time){
      fMinos_Energy.push_back(Energy);
      fMinos_Time.push_back(Time);
      fMinos_T_DetectorNbr.push_back(DetNbr);
    };//!
    //

    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const
      {return fMinos_E_DetectorNbr.size();}
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fMinos_E_DetectorNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fMinos_Energy[i];}//!

    // Time
    inline UShort_t GetMultTime() const
      {return fMinos_T_DetectorNbr.size();}
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fMinos_T_DetectorNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fMinos_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TMinosData,1)  // MinosData structure
};

#endif

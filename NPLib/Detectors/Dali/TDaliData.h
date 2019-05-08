#ifndef __DaliDATA__
#define __DaliDATA__
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Elidiano Tronchin  contact address: elidiano.tronchin@studenti.unipd.it                        *
 *                                                                           *
 * Creation Date  : septembre 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Dali Raw data                                    *
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

class TDaliData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<UShort_t>   fDali_E_DetectorNbr;
    vector<Double_t>   fDali_ADC;
    vector<Double_t>   fDali_Energy;

    // Time
    vector<UShort_t>   fDali_T_DetectorNbr;
    vector<Double_t>   fDali_TDC;
    vector<Double_t>   fDali_Time;


  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TDaliData();
    ~TDaliData();
    

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
    // ADC
    inline void SetADC(const UShort_t& DetNbr,const Double_t& Energy){
      fDali_E_DetectorNbr.push_back(DetNbr);
      fDali_ADC.push_back(Energy);
    };//!
    inline void SetADC_Only(const Double_t& Energy){
      fDali_ADC.push_back(Energy);
    };//!

    
    // Energy
    inline void SetEnergy(const UShort_t& DetNbr,const Double_t& Energy){
      fDali_E_DetectorNbr.push_back(DetNbr);
      fDali_Energy.push_back(Energy);
    };//!

    // TDC
    inline void SetTDC(const UShort_t& DetNbr,const Double_t& Time)	{
      fDali_T_DetectorNbr.push_back(DetNbr);     
      fDali_TDC.push_back(Time);
    };//!
    // Time
    inline void SetTime(const UShort_t& DetNbr,const Double_t& Time)	{
      fDali_T_DetectorNbr.push_back(DetNbr);     
      fDali_Time.push_back(Time);
    };//!
    
    // (A&T DC)
    inline void SetADCAndTDC(const UShort_t& DetNbr,const Double_t& Energy,const Double_t& Time){
      fDali_ADC.push_back(Energy);
      fDali_TDC.push_back(Time);
      fDali_T_DetectorNbr.push_back(DetNbr);
      fDali_E_DetectorNbr.push_back(DetNbr);

    };//!
    // (E&T)
    inline void SetEnergyAndTime(const UShort_t& DetNbr,const Double_t& Energy,const Double_t& Time){
      fDali_Energy.push_back(Energy);
      fDali_Time.push_back(Time);
      fDali_T_DetectorNbr.push_back(DetNbr);
      fDali_E_DetectorNbr.push_back(DetNbr);

    };//!
    //

    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const
      {return fDali_E_DetectorNbr.size();}
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fDali_E_DetectorNbr[i];}//!
    inline Double_t Get_ADC(const unsigned int &i) const 
      {return fDali_ADC[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fDali_Energy[i];}//!
    
    // Time
    inline UShort_t GetMultTime() const
      {return fDali_T_DetectorNbr.size();}
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fDali_T_DetectorNbr[i];}//!
    inline Double_t Get_TDC(const unsigned int &i) const 
      {return fDali_TDC[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fDali_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TDaliData,1)  // DaliData structure
};

#endif

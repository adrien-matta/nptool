#ifndef AnnularCsIDATA_Include_Guard
#define AnnularCsIDATA_Include_Guard
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : March 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularCsI Raw data                                    *
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

class TAnnularCsIData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // Energy
    vector<UShort_t>   fAnnularCsI_E_DetectorNbr;
    vector<Double_t>   fAnnularCsI_Energy;

    // Time
    vector<UShort_t>   fAnnularCsI_T_DetectorNbr;
    vector<Double_t>   fAnnularCsI_Time;


  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TAnnularCsIData();
    ~TAnnularCsIData();
    

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
      fAnnularCsI_E_DetectorNbr.push_back(DetNbr);
      fAnnularCsI_Energy.push_back(Energy);
    };//!

    // Time
    inline void SetTime(const UShort_t& DetNbr,const Double_t& Time)	{
      fAnnularCsI_T_DetectorNbr.push_back(DetNbr);     
      fAnnularCsI_Time.push_back(Time);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const
      {return fAnnularCsI_E_DetectorNbr.size();}
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fAnnularCsI_E_DetectorNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fAnnularCsI_Energy[i];}//!

    // Time
    inline UShort_t GetMultTime() const
      {return fAnnularCsI_T_DetectorNbr.size();}
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fAnnularCsI_T_DetectorNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fAnnularCsI_Time[i];}//!


  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TAnnularCsIData,1)  // AnnularCsIData structure
};

#endif

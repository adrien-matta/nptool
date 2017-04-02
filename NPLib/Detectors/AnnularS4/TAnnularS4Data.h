#ifndef __AnnularS4DATA__
#define __AnnularS4DATA__
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : January 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularS4 Raw data                                    *
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

class TAnnularS4Data : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
    // ADC
    vector<UShort_t>   fS4_E_DetNbr;
    vector<UShort_t>   fS4_E_StripNbr;
    vector<UShort_t>   fS4_E_SectorNbr;
    vector<Double_t>   fS4_E_Energy;
    // TDC
    vector<UShort_t>   fS4_T_DetNbr;
    vector<UShort_t>   fS4_T_StripNbr;
    vector<UShort_t>   fS4_T_SectorNbr;
    vector<Double_t>   fS4_T_Time;

  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TAnnularS4Data();
    virtual ~TAnnularS4Data();
    

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
    // (Th,E)
    void   SetS4_E_DetectorNbr(const int& det)     {fS4_E_DetNbr.push_back(det);}
    void   SetS4_E_StripNbr(const int& Nr)         {fS4_E_StripNbr.push_back(Nr);}
    void   SetS4_E_SectorNbr(const int& Nr)        {fS4_E_SectorNbr.push_back(Nr);}
    void   SetS4_E_Energy(const double& E)          {fS4_E_Energy.push_back(E);}
    // (Th,T)
    void   SetS4_T_DetectorNbr(const int& det)      {fS4_T_DetNbr.push_back(det);}
    void   SetS4_T_StripNbr(const int& Nr)          {fS4_T_StripNbr.push_back(Nr);}
    void   SetS4_T_SectorNbr(const int& Nr)         {fS4_T_SectorNbr.push_back(Nr);}
    void   SetS4_T_Time(const double& T)            {fS4_T_Time.push_back(T);}
    
    //////////////////////    GETTERS    ////////////////////////
    // (Th,E)
    unsigned int   GetS4EMult()                 const {return fS4_E_StripNbr.size();}
    int   GetS4_E_DetectorNbr(const int& i)     const {return fS4_E_DetNbr.at(i);}
    int   GetS4_E_StripNbr(const int& i)        const {return fS4_E_StripNbr.at(i);}
    int   GetS4_E_SectorNbr(const int& i)       const {return fS4_E_SectorNbr.at(i);}
    double   GetS4_E_Energy(const int& i)       const {return fS4_E_Energy.at(i);}
    // (Th,T)
    unsigned int   GetS4TMult()                 const {return fS4_T_DetNbr.size();}
    int   GetS4_T_DetectorNbr(const int& i)     const {return fS4_T_DetNbr.at(i);}
    int   GetS4_T_StripNbr(const int& i)        const {return fS4_T_StripNbr.at(i);}
    int   GetS4_T_SectorNbr(const int& i)       const {return fS4_T_SectorNbr.at(i);}
    double   GetS4_T_Time(const int& i)         const {return fS4_T_Time.at(i);}
   

  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TAnnularS4Data,1)  // AnnularS4Data structure
};

#endif

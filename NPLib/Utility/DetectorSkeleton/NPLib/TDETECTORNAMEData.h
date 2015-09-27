#ifndef __DETECTORNAMEDATA__
#define __DETECTORNAMEDATA__
/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold DETECTORNAME Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// STL
#include<stdlib.h>
#include <vector>
#include <map>
using namespace std ;

// ROOT
#include "TObject.h"

class TDETECTORNAMEData : public TObject {

  private: // Data Member are hold into vector to allow multiplicity
    // Energy
    vector<UShort_t>   fDETECTORNAME_E_DetectorNbr;
    vector<Double_t>   fDETECTORNAME_Energy;

    // Time
    vector<UShort_t>   fDETECTORNAME_T_DetectorNbr;
    vector<Double_t>   fDETECTORNAME_Time;

  public: // Constructor
    TDETECTORNAMEData();
    virtual ~TDETECTORNAMEData();
    
  public: // Inherrited from TObject and overriden to avoid warning
    void Clear();
    void Clear(const Option_t*) {};
    void Dump() const;

  public: // Getters and Setters
          // Prefere inline declaration to avoid unnecessary called of 
          // frequently used methods
          // add //! to avoid root to create dictionnary to the method
    /////////////////////           SETTERS           ////////////////////////
    inline void SetE_DetectorNbr(const UShort_t& DetNbr)
      {fDETECTORNAME_E_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Energy(const Double_t& Energy)
      {fDETECTORNAME_Energy.push_back(Energy);}//!

    inline void SetEnergy(const UShort_t& DetNbr,const Double_t& Energy)	{
      SetE_DetectorNbr(DetNbr);
      Set_Energy(Energy);
    };//!

    inline void SetT_DetectorNbr(const UShort_t& DetNbr)
      {fDETECTORNAME_T_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Time(const Double_t& Time)
      {fDETECTORNAME_Time.push_back(Time);}//!


    inline void SetTime(const UShort_t& DetNbr,const Double_t& Time)	{
      SetT_DetectorNbr(DetNbr);
      Set_Time(Time);
    };//!


    /////////////////////           GETTERS           ////////////////////////
    // Energy
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fDETECTORNAME_E_DetectorNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fDETECTORNAME_Energy[i];}//!

    // Time
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fDETECTORNAME_T_DetectorNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fDETECTORNAME_Time[i];}//!


    // Requiered for Root dictionnary
    ClassDef(TDETECTORNAMEData,1)  // DETECTORNAMEData structure
};

#endif

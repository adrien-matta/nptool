#ifndef _FPDTamuDATA_
#define _FPDTamuDATA_
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: m.moukaddam@surrey.ac.uk                                 *
 *                                                                           *
 * Creation Date  : November 2016                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the Raw Data from the Focal Plane Detector at TexaA&M    *
 *  The full setup consists from upstream to downstream:                     *
 *  - 2 x Delta E ionisation chamber (might not be used all)                 *
 *  - 4 x Resistive Avalanche wires                                          *
 *  - 28 x pad (MicroMega type ionasation chaamber)                          *
 *  - 1 Plastic scintillator read by 2 PMTs (Left and right                  *
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

class TFPDTamuData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order 
  // to allow multiplicity treatment
  private: 
  	//DeltaE ionisation chamber
    // Energy 
    vector<UShort_t>   fFPDTamu_Delta_E_DetectorNbr;
    vector<Double_t>   fFPDTamu_Delta_Energy;
    // Time
    vector<UShort_t>   fFPDTamu_Delta_T_DetectorNbr;
    vector<Double_t>   fFPDTamu_Delta_Time;

    // Avalanche Resistive Wire, Resistive: reading on both ends
    // Energy
    vector<UShort_t>   fFPDTamu_AWire_E_DetectorNbr;
    vector<Double_t>   fFPDTamu_AWire_Energy_Left;
    vector<Double_t>   fFPDTamu_AWire_Energy_Right;
    // Time
    vector<UShort_t>   fFPDTamu_AWire_T_DetectorNbr;
    vector<Double_t>   fFPDTamu_AWire_Time_Left;
    vector<Double_t>   fFPDTamu_AWire_Time_Right;
    
	// MicroMega Plate
    // Energy
    vector<UShort_t>   fFPDTamu_Micro_E_RowNbr; // Row 0 upstream, Row 3 downstream
    vector<UShort_t>   fFPDTamu_Micro_E_ColNbr; // Col 0 Left, Col 6 Right
    vector<Double_t>   fFPDTamu_Micro_Energy;
    // Time
    vector<UShort_t>   fFPDTamu_Micro_T_RowNbr; 
    vector<UShort_t>   fFPDTamu_Micro_T_ColNbr; 
    vector<Double_t>   fFPDTamu_Micro_Time;

    // Plastic Scintillator, Left and Right PMTs
    // Energy
    vector<Double_t>   fFPDTamu_Plast_Energy_Left;
    vector<Double_t>   fFPDTamu_Plast_Energy_Right;
    // Time
    vector<Double_t>   fFPDTamu_Plast_Time_Left;
    vector<Double_t>   fFPDTamu_Plast_Time_Right;

  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public: 
    TFPDTamuData();
    virtual ~TFPDTamuData();
    

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
    //DeltaE ionisation chamber
    // Energy
    inline void Set_Delta_E_DetectorNbr(const UShort_t& DetNbr)
      {fFPDTamu_Delta_E_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Delta_Energy(const Double_t& Energy)
      {fFPDTamu_Delta_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void Set_Delta_E(const UShort_t& DetNbr,const Double_t& Energy) {
      Set_Delta_E_DetectorNbr(DetNbr);
      Set_Delta_Energy(Energy);
    };//!
	// Time
    inline void Set_Delta_T_DetectorNbr(const UShort_t& DetNbr)
      {fFPDTamu_Delta_T_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_Delta_Time(const Double_t& Time)
      {fFPDTamu_Delta_Time.push_back(Time);}//!
    inline void Set_Delta_T(const UShort_t& DetNbr,const Double_t& Time) {
      Set_Delta_T_DetectorNbr(DetNbr);
      Set_Delta_Time(Time);
    };//!
    
    // Avalanche Resistive Wire
    // Energy
    inline void Set_AWire_E_DetectorNbr(const UShort_t& DetNbr)
      {fFPDTamu_AWire_E_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_AWire_Energy_Left(const Double_t& Energy)
      {fFPDTamu_AWire_Energy_Left.push_back(Energy);}//!
    inline void Set_AWire_Energy_Right(const Double_t& Energy)
      {fFPDTamu_AWire_Energy_Right.push_back(Energy);}//!
    inline void Set_AWire_E(const UShort_t& DetNbr,const Double_t& EnergyLeft, const Double_t& EnergyRight) {
      Set_AWire_E_DetectorNbr(DetNbr);
      Set_AWire_Energy_Left(EnergyLeft);
      Set_AWire_Energy_Right(EnergyRight);
    };//!
    // Time
    inline void Set_AWire_T_DetectorNbr(const UShort_t& DetNbr)
      {fFPDTamu_AWire_T_DetectorNbr.push_back(DetNbr);} //!
    inline void Set_AWire_Time_Left(const Double_t& Time)
      {fFPDTamu_AWire_Time_Left.push_back(Time);}//!
    inline void Set_AWire_Time_Right(const Double_t& Time)
      {fFPDTamu_AWire_Time_Right.push_back(Time);}//!
    inline void Set_AWire_T(const UShort_t& DetNbr,const Double_t& TimeLeft, const Double_t& TimeRight) {
      Set_AWire_T_DetectorNbr(DetNbr);
      Set_AWire_Time_Left(TimeLeft);
      Set_AWire_Time_Right(TimeRight);
    };//!

    // MicroMega Plate
    // Energy
    inline void Set_Micro_E_RowNbr(const UShort_t& RowNbr)
      {fFPDTamu_Micro_E_RowNbr.push_back(RowNbr);} //!
    inline void Set_Micro_E_ColNbr(const UShort_t& ColNbr)
      {fFPDTamu_Micro_E_ColNbr.push_back(ColNbr);} //!
    inline void Set_Micro_Energy(const Double_t& Energy)
      {fFPDTamu_Micro_Energy.push_back(Energy);}//!
    inline void Set_Micro_E(const UShort_t& RowNbr, const UShort_t& ColNbr, const Double_t& Energy) {
      Set_Micro_E_RowNbr(RowNbr);
      Set_Micro_E_RowNbr(ColNbr);
      Set_Micro_Energy(Energy);
    };//!
    // Time
    inline void Set_Micro_T_RowNbr(const UShort_t& RowNbr)
      {fFPDTamu_Micro_T_RowNbr.push_back(RowNbr);} //!
    inline void Set_Micro_T_ColNbr(const UShort_t& ColNbr)
      {fFPDTamu_Micro_T_ColNbr.push_back(ColNbr);} //!
    inline void Set_Micro_Time(const Double_t& Time)
      {fFPDTamu_Micro_Time.push_back(Time);}//!
    inline void Set_Micro_T(const UShort_t& RowNbr, const UShort_t& ColNbr, const Double_t& Time) {
      Set_Micro_T_RowNbr(RowNbr);
      Set_Micro_T_RowNbr(ColNbr);
      Set_Micro_Time(Time);
    };//!

	// Plastic Scintillator
    // Energy
    inline void Set_Plast_Energy_Left(const Double_t& Energy)
      {fFPDTamu_Plast_Energy_Left.push_back(Energy);}//!
    inline void Set_Plast_Energy_Right(const Double_t& Energy)
      {fFPDTamu_Plast_Energy_Right.push_back(Energy);}//!
    inline void Set_Plast_E(const Double_t& EnergyLeft, const Double_t& EnergyRight) {
      Set_Plast_Energy_Left(EnergyLeft);
      Set_Plast_Energy_Right(EnergyRight);
    };//!
    // Time
    inline void Set_Plast_Time_Left(const Double_t& Time)
      {fFPDTamu_Plast_Time_Left.push_back(Time);}//!
    inline void Set_Plast_Time_Right(const Double_t& Time)
      {fFPDTamu_Plast_Time_Right.push_back(Time);}//!
    inline void Set_Plast_T(const Double_t& TimeLeft, const Double_t& TimeRight) {
      Set_Plast_Time_Left(TimeLeft);
      Set_Plast_Time_Right(TimeRight);
    };//!


    //////////////////////    GETTERS    ////////////////////////
	//DeltaE ionisation chamber    
	// Energy
    inline UShort_t Get_Delta_Energy_Mult() const
      {return fFPDTamu_Delta_E_DetectorNbr.size();}
    inline UShort_t Get_Delta_E_DetectorNbr(const unsigned int &i) const
      {return fFPDTamu_Delta_E_DetectorNbr[i];} //!
    inline Double_t Get_Delta_Energy(const unsigned int &i) const
      {return fFPDTamu_Delta_Energy[i];}//!
	// Time
    inline UShort_t Get_Delta_Time_Mult() const
      {return fFPDTamu_Delta_T_DetectorNbr.size();}
    inline UShort_t Get_Delta_T_DetectorNbr(const unsigned int &i) const
      {return fFPDTamu_Delta_T_DetectorNbr[i];} //!
    inline Double_t Get_Delta_Time(const unsigned int &i) const
      {return fFPDTamu_Delta_Time[i];}//!

    // Avalanche Resistive Wire
	// Energy
    inline UShort_t Get_AWire_Energy_Mult() const
      {return fFPDTamu_AWire_E_DetectorNbr.size();}
    inline UShort_t Get_AWire_E_DetectorNbr(const unsigned int &i) const
      {return fFPDTamu_AWire_E_DetectorNbr[i];} //!
    inline Double_t Get_AWire_Energy_Left(const unsigned int &i) const
      {return fFPDTamu_AWire_Energy_Left[i];}//!
	inline Double_t Get_AWire_Energy_Right(const unsigned int &i) const
      {return fFPDTamu_AWire_Energy_Right[i];}//!
	// Time
    inline UShort_t Get_AWire_Time_Mult() const
      {return fFPDTamu_AWire_T_DetectorNbr.size();}
    inline UShort_t Get_AWire_T_DetectorNbr(const unsigned int &i) const
      {return fFPDTamu_AWire_T_DetectorNbr[i];} //!
    inline Double_t Get_AWire_Time_Left(const unsigned int &i) const
      {return fFPDTamu_AWire_Time_Left[i];}//!
    inline Double_t Get_AWire_Time_Right(const unsigned int &i) const
      {return fFPDTamu_AWire_Time_Right[i];}//!

	// MicroMega Plate
	// Energy
    inline UShort_t Get_Micro_Energy_Mult() const
      {return fFPDTamu_Micro_E_RowNbr.size();}
    inline UShort_t Get_Micro_E_RowNbr(const unsigned int &i) const
      {return fFPDTamu_Micro_E_RowNbr[i];} //!
    inline UShort_t Get_Micro_E_ColNbr(const unsigned int &i) const
      {return fFPDTamu_Micro_E_ColNbr[i];} //!
    inline Double_t Get_Micro_Energy(const unsigned int &i) const
      {return fFPDTamu_Micro_Energy[i];}//!
	// Time
    inline UShort_t Get_Micro_Time_Mult() const
      {return fFPDTamu_Micro_T_RowNbr.size();}
    inline UShort_t Get_Micro_T_RowNbr(const unsigned int &i) const
      {return fFPDTamu_Micro_T_RowNbr[i];} //!
    inline UShort_t Get_Micro_T_ColNbr(const unsigned int &i) const
      {return fFPDTamu_Micro_T_ColNbr[i];} //!
    inline Double_t Get_Micro_Time(const unsigned int &i) const
      {return fFPDTamu_Micro_Time[i];}//!

    // Plastic Scintillator
	// Energy
    inline UShort_t Get_Plast_Energy_Mult() const
      {return fFPDTamu_Plast_Energy_Left.size();}
    inline Double_t Get_Plast_Energy_Left(const unsigned int &i) const
      {return fFPDTamu_Plast_Energy_Left[i];} //!
    inline Double_t Get_Plast_Energy_Right(const unsigned int &i) const
      {return fFPDTamu_Plast_Energy_Right[i];} //!
	// Time
    inline UShort_t Get_Plast_Time_Mult() const
      {return fFPDTamu_Plast_Time_Left.size();}
    inline Double_t Get_Plast_Time_Left(const unsigned int &i) const
      {return fFPDTamu_Plast_Time_Left[i];} //!
    inline Double_t Get_Plast_Time_Right(const unsigned int &i) const
      {return fFPDTamu_Plast_Time_Right[i];} //!

  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TFPDTamuData,1)  // FPDTamuData structure
};

#endif

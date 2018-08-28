#ifndef __ActarDATA__
#define __ActarDATA__
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouace@ganil.fr                        *
 *                                                                           *
 * Creation Date  : September 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Actar Raw data                                    *
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
#include "TGraph.h"

class TActarData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order
  // to allow multiplicity treatment
  private:
    // Charge
    vector<int>  fActar_PadNumber;
    vector<int>  fActar_PadX;
    vector<int>  fActar_PadY;
    vector<int>  fActar_PadZ;
    vector<double>  fActar_PadCharge;

    vector<double> fSilicon_Energy;
    vector<int> fSilicon_DetectorNumber;
    vector<double> fSilicon_Time;

    vector<double> fCsI_Energy;
    vector<int> fCsI_CrystalNumber;


  //////////////////////////////////////////////////////////////
  // Constructor and destructor
  public:
    TActarData();
    ~TActarData();


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
    // Charge
    inline void SetCharge(const Double_t& Charge){
        fActar_PadCharge.push_back(Charge);
    };//!
    // Pad
    inline void SetPadNumber(const UShort_t& PadNbr){
        fActar_PadNumber.push_back(PadNbr);
    };//!
    // Row Y
    inline void SetPadY(const UShort_t& RowNbr){
        fActar_PadY.push_back(RowNbr);
    }
    // Column X
    inline void SetPadX(const UShort_t& ColumnNbr){
        fActar_PadX.push_back(ColumnNbr);
    }
    // Time Z
    inline void SetPadZ(const Double_t& Time)	{
      fActar_PadZ.push_back(Time);
    };//!

    //Silicon
    inline void SetSiliconEnergy(const Double_t& Energy){
      fSilicon_Energy.push_back(Energy);
    }
    inline void SetSiliconDetectorNumber(const Int_t& Det){
      fSilicon_DetectorNumber.push_back(Det);
    }
    inline void SetSiliconTime(const Double_t& Time){
      fSilicon_Time.push_back(Time);
    }

    //CsI
    inline void SetCsIEnergy(const Double_t& Energy){
      fCsI_Energy.push_back(Energy);
    }
    inline void SetCsICrystalNumber(const Int_t& Det){
      fCsI_CrystalNumber.push_back(Det);
    }


    //////////////////////    GETTERS    ////////////////////////
    // Charge
    inline UShort_t GetPadMult() const
      {return fActar_PadX.size();}
    inline UShort_t GetPadNumber(const unsigned int &i) const
      {return fActar_PadNumber[i];}//!
    inline UShort_t GetPadX(const unsigned int &i) const
      {return fActar_PadX[i];}//!
    inline UShort_t GetPadY(const unsigned int &i) const
    {return fActar_PadY[i];}//!
    inline Double_t GetPadZ(const unsigned int &i) const
    {return fActar_PadZ[i];}//!
    inline Double_t GetPadCharge(const unsigned int &i) const
      {return fActar_PadCharge[i];}//!
   


    // Silicon
    inline Double_t Get_SiliconEnergy(const unsigned int &i) const
      {return fSilicon_Energy[i];}//!
    inline Double_t Get_SiliconTime(const unsigned int &i) const
      {return fSilicon_Time[i];}//!
    inline Int_t Get_SiliconDetectorNumber(const unsigned int &i) const
      {return fSilicon_DetectorNumber[i];}//!

    // CsI
    inline Double_t Get_CsIEnergy(const unsigned int &i) const
      {return fCsI_Energy[i];}//!
    inline Int_t Get_CsICrystalNumber(const unsigned int &i) const
      {return fCsI_CrystalNumber[i];}//!



  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TActarData,1)  // ActarData structure
};

#endif

#ifndef __ActarDATA__
#define __ActarDATA__
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
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
    vector<int>  fActar_PadTime;
    vector<int>  fActar_PadRow;
    vector<int>  fActar_PadColumn;
    vector<double>  fActar_PadCharge;

    vector<double> fActar_dig_Time;
    vector<double> fActar_dig_Charge;

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
    // Row
    inline void SetRowNumber(const UShort_t& RowNbr){
        fActar_PadRow.push_back(RowNbr);
    }
    // Column
    inline void SetColumnNumber(const UShort_t& ColumnNbr){
        fActar_PadColumn.push_back(ColumnNbr);
    }
    // Time
    inline void SetTime(const Double_t& Time)	{
      fActar_PadTime.push_back(Time);
    };//!

    // Digitizer
    void AddEnergyPoint(double& E,double& T){
        fActar_dig_Charge.push_back(E);
        fActar_dig_Time.push_back(T);
    }

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
    inline UShort_t GetMultCharge() const
      {return fActar_PadNumber.size();}
    inline UShort_t Get_PadNumber(const unsigned int &i) const
      {return fActar_PadNumber[i];}//!
    inline UShort_t Get_PadRow(const unsigned int &i) const
      {return fActar_PadRow[i];}//!
    inline UShort_t Get_PadColumn(const unsigned int &i) const
      {return fActar_PadColumn[i];}//!
    inline Double_t Get_Charge(const unsigned int &i) const
      {return fActar_PadCharge[i];}//!

    // Time
    inline Double_t Get_Time(const unsigned int &i) const
      {return fActar_PadTime[i];}//!

    inline vector<double> Get_dig_Charge()   {return fActar_dig_Charge;}
    TGraph* GetChargeAsGraph();

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

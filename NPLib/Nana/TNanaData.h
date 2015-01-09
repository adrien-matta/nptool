#ifndef __NanaDATA__
#define __NanaDATA__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 04/12/2009                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of the Nana detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;


class TNanaData : public TObject {

  protected:
    // LaBr3
    // Energy
    vector<UShort_t>  fNANA_LaBr3_E_DetectorNbr;
    vector<Double_t>  fNANA_LaBr3_E_Energy;
    // Time
    vector<UShort_t>  fNANA_LaBr3_T_DetectorNbr;
    vector<Double_t>  fNANA_LaBr3_T_Time;

  public:
    TNanaData();
    virtual ~TNanaData();

    void   Clear();
    void   Clear(const Option_t*) {};
    void   Dump() const;

    /////////////////////           GETTERS           ////////////////////////
    // (E)
    UShort_t GetNanaLaBr3EMult()               {
      return fNANA_LaBr3_E_DetectorNbr.size();
    }
    
    UShort_t GetNanaLaBr3EDetectorNbr(Int_t i)    {
      return fNANA_LaBr3_E_DetectorNbr.at(i);
    }
    Double_t GetNanaLaBr3EEnergy(Int_t i)      {
      return fNANA_LaBr3_E_Energy.at(i);
    }


    // (T)
    UShort_t GetNanaLaBr3TMult()               {
      return fNANA_LaBr3_E_DetectorNbr.size();
    }
    
    UShort_t GetNanaLaBr3TDetectorNbr(Int_t i)    {
      return fNANA_LaBr3_T_DetectorNbr.at(i);
    }
    Double_t GetNanaLaBr3TTime(Int_t i)      {
      return fNANA_LaBr3_T_Time.at(i);
    }

    /////////////////////           SETTERS           ////////////////////////
    // (E)
    void SetNanaLaBr3E(UShort_t DetectorNbr, Double_t E){
      fNANA_LaBr3_E_DetectorNbr.push_back(DetectorNbr); 
      fNANA_LaBr3_E_Energy.push_back(E); 
    }

    void SetNanaLaBr3EDetectorNbr(UShort_t DetectorNbr)    {
      fNANA_LaBr3_E_DetectorNbr.push_back(DetectorNbr);
    }
    void SetNanaLaBr3EEnergy(Double_t Energy)      {
      fNANA_LaBr3_E_Energy.push_back(Energy);
    }

    // (T)
    void SetNanaLaBr3T(UShort_t DetectorNbr, Double_t T){
      fNANA_LaBr3_T_DetectorNbr.push_back(DetectorNbr); 
      fNANA_LaBr3_T_Time.push_back(T); 
    }

    void SetNanaLaBr3TDetectorNbr(UShort_t DetectorNbr)    {
      fNANA_LaBr3_T_DetectorNbr.push_back(DetectorNbr);
    }
    void SetNanaLaBr3TTime(Double_t Time)      {
      fNANA_LaBr3_T_Time.push_back(Time);
    }

    ClassDef(TNanaData,1)  // NanaData structure
};

#endif

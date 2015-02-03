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
    vector<UShort_t>  fNANA_LaBr3_DetectorNbr;
    vector<Double_t>  fNANA_LaBr3_EnergyShort;
    vector<Double_t>  fNANA_LaBr3_EnergyLong;
    vector<ULong64_t>  fNANA_LaBr3_Time;
    vector<Double_t>  fNANA_LaBr3_PSD;

  public:
    TNanaData();
    virtual ~TNanaData();

    void   Clear();
    void   Clear(const Option_t*) {};
    void   Dump() const;

    /////////////////////           GETTERS           ////////////////////////
    // (E)
    UShort_t GetNanaLaBr3Mult()               {
      return fNANA_LaBr3_DetectorNbr.size();
    }
    
    UShort_t GetNanaLaBr3DetectorNbr(Int_t i)    {
      return fNANA_LaBr3_DetectorNbr[i];
    }
    Double_t GetNanaLaBr3EnergyLong(Int_t i)      {
      return fNANA_LaBr3_EnergyLong[i];
    }
    
    Double_t GetNanaLaBr3EnergyShort(Int_t i)      {
      return fNANA_LaBr3_EnergyShort[i];
    }
    ULong64_t GetNanaLaBr3Time(ULong64_t i)      {
      return fNANA_LaBr3_Time[i];
    }
    Double_t GetNanaLaBr3PSD(Double_t i)      {
      return fNANA_LaBr3_PSD[i];
    }
    /////////////////////           SETTERS           ////////////////////////
    // (E)
    void SetNanaLaBr3(UShort_t DetectorNbr, Double_t EL, Double_t ES, ULong64_t T,Double_t PSD){
      fNANA_LaBr3_DetectorNbr.push_back(DetectorNbr); 
      fNANA_LaBr3_EnergyShort.push_back(ES); 
      fNANA_LaBr3_EnergyLong.push_back(EL); 
      fNANA_LaBr3_Time.push_back(T); 
      fNANA_LaBr3_PSD.push_back(PSD);
    }

    ClassDef(TNanaData,1)  // NanaData structure
};

#endif

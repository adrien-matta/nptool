#ifndef __FatimaDATA__
#define __FatimaDATA__
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
 *  This class described the raw data of the Fatima detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;


class TFatimaData : public TObject {

  protected:
    // LaBr3
    // Energy
    vector<UShort_t>  fFATIMA_LaBr3_E_DetectorNbr;
    vector<Double_t>  fFATIMA_LaBr3_E_Energy;
    // Time
    vector<UShort_t>  fFATIMA_LaBr3_T_DetectorNbr;
    vector<Double_t>  fFATIMA_LaBr3_T_Time;

  public:
    TFatimaData();
    virtual ~TFatimaData();

    void   Clear();
    void   Clear(const Option_t*) {};
    void   Dump() const;

    /////////////////////           GETTERS           ////////////////////////
    // (E)
    UShort_t GetFatimaLaBr3EMult()               {
      return fFATIMA_LaBr3_E_DetectorNbr.size();
    }
    
    UShort_t GetFatimaLaBr3EDetectorNbr(Int_t i)    {
      return fFATIMA_LaBr3_E_DetectorNbr.at(i);
    }
    Double_t GetFatimaLaBr3EEnergy(Int_t i)      {
      return fFATIMA_LaBr3_E_Energy.at(i);
    }


    // (T)
    UShort_t GetFatimaLaBr3TMult()               {
      return fFATIMA_LaBr3_E_DetectorNbr.size();
    }
    
    UShort_t GetFatimaLaBr3TDetectorNbr(Int_t i)    {
      return fFATIMA_LaBr3_T_DetectorNbr.at(i);
    }
    Double_t GetFatimaLaBr3TTime(Int_t i)      {
      return fFATIMA_LaBr3_T_Time.at(i);
    }

    /////////////////////           SETTERS           ////////////////////////
    // (E)
    void SetFatimaLaBr3E(UShort_t DetectorNbr, Double_t E){
      fFATIMA_LaBr3_E_DetectorNbr.push_back(DetectorNbr); 
      fFATIMA_LaBr3_E_Energy.push_back(E); 
    }

    void SetFatimaLaBr3EDetectorNbr(UShort_t DetectorNbr)    {
      fFATIMA_LaBr3_E_DetectorNbr.push_back(DetectorNbr);
    }
    void SetFatimaLaBr3EEnergy(Double_t Energy)      {
      fFATIMA_LaBr3_E_Energy.push_back(Energy);
    }

    // (T)
    void SetFatimaLaBr3T(UShort_t DetectorNbr, Double_t T){
      fFATIMA_LaBr3_T_DetectorNbr.push_back(DetectorNbr); 
      fFATIMA_LaBr3_T_Time.push_back(T); 
    }

    void SetFatimaLaBr3TDetectorNbr(UShort_t DetectorNbr)    {
      fFATIMA_LaBr3_T_DetectorNbr.push_back(DetectorNbr);
    }
    void SetFatimaLaBr3TTime(Double_t Time)      {
      fFATIMA_LaBr3_T_Time.push_back(Time);
    }

    ClassDef(TFatimaData,1)  // FatimaData structure
};

#endif

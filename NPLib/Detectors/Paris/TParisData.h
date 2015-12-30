#ifndef __ParisDATA__
#define __ParisDATA__
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
 *  This class described the raw data of the Paris detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>

#include "TObject.h"
using namespace std ;


class TParisData : public TObject {

  protected:
    // First Stage LaBr3
    // Energy
    vector<UShort_t>  fPARIS_LaBr3Stage_E_ClusterNbr;
    vector<UShort_t>  fPARIS_LaBr3Stage_E_PhoswichNbr;
    vector<Double_t>  fPARIS_LaBr3Stage_E_Energy;
    // Time
    vector<UShort_t>  fPARIS_LaBr3Stage_T_ClusterNbr;
    vector<UShort_t>  fPARIS_LaBr3Stage_T_PhoswichNbr;
    vector<Double_t>  fPARIS_LaBr3Stage_T_Time;

    // Second Stage NaI
    // Energy
    vector<UShort_t>  fPARIS_NaIStage_E_ClusterNbr;
    vector<UShort_t>  fPARIS_NaIStage_E_PhoswichNbr;
    vector<Double_t>  fPARIS_NaIStage_E_Energy;
    // Time
    vector<UShort_t>  fPARIS_NaIStage_T_ClusterNbr;
    vector<UShort_t>  fPARIS_NaIStage_T_PhoswichNbr;
    vector<Double_t>  fPARIS_NaIStage_T_Time;

  public:
    TParisData();
    virtual ~TParisData();

    void   Clear();
    void   Clear(const Option_t*) {};
    void   Dump() const;

    /////////////////////           GETTERS           ////////////////////////
    // First stage 
    // (E)
    UShort_t GetPARISLaBr3StageEMult()               {
      return fPARIS_LaBr3Stage_E_ClusterNbr.size();
    }
    UShort_t GetPARISLaBr3StageEClusterNbr(Int_t i) {
      return fPARIS_LaBr3Stage_E_ClusterNbr.at(i);
    }
    UShort_t GetPARISLaBr3StageEPhoswichNbr(Int_t i)    {
      return fPARIS_LaBr3Stage_E_PhoswichNbr.at(i);
    }
    Double_t GetPARISLaBr3StageEEnergy(Int_t i)      {
      return fPARIS_LaBr3Stage_E_Energy.at(i);
    }


    // (T)
    UShort_t GetPARISLaBr3StageTMult()               {
      return fPARIS_LaBr3Stage_E_ClusterNbr.size();
    }
    UShort_t GetPARISLaBr3StageTClusterNbr(Int_t i) {
      return fPARIS_LaBr3Stage_T_ClusterNbr.at(i);
    }
    UShort_t GetPARISLaBr3StageTPhoswichNbr(Int_t i)    {
      return fPARIS_LaBr3Stage_T_PhoswichNbr.at(i);
    }
    Double_t GetPARISLaBr3StageTTime(Int_t i)      {
      return fPARIS_LaBr3Stage_T_Time.at(i);
    }

    // Second stage NaI
    // (E)
    UShort_t GetPARISNaIStageEMult()               {
      return fPARIS_NaIStage_E_ClusterNbr.size();
    }
    UShort_t GetPARISNaIStageEClusterNbr(Int_t i) {
      return fPARIS_NaIStage_E_ClusterNbr.at(i);
    }
    UShort_t GetPARISNaIStageEPhoswichNbr(Int_t i)    {
      return fPARIS_NaIStage_E_PhoswichNbr.at(i);
    }
    Double_t GetPARISNaIStageEEnergy(Int_t i)      {
      return fPARIS_NaIStage_E_Energy.at(i);
    }

    // (T)
    UShort_t GetPARISNaIStageTMult()               {
      return fPARIS_NaIStage_E_ClusterNbr.size();
    }
    UShort_t GetPARISNaIStageTClusterNbr(Int_t i) {
      return fPARIS_NaIStage_T_ClusterNbr.at(i);
    }
    UShort_t GetPARISNaIStageTPhoswichNbr(Int_t i)    {
      return fPARIS_NaIStage_T_PhoswichNbr.at(i);
    }
    Double_t GetPARISNaIStageTTime(Int_t i)      {
      return fPARIS_NaIStage_T_Time.at(i);
    }

    /////////////////////           SETTERS           ////////////////////////
    // First stage 
    // (E)
    void SetParisLaBr3E(UShort_t DetNbr,UShort_t PhoswichNbr, Double_t E){
      fPARIS_LaBr3Stage_E_ClusterNbr.push_back(DetNbr); 
      fPARIS_LaBr3Stage_E_PhoswichNbr.push_back(PhoswichNbr); 
      fPARIS_LaBr3Stage_E_Energy.push_back(E); 
    }

    void SetPARISLaBr3StageEClusterNbr(UShort_t DetNbr) {
      fPARIS_LaBr3Stage_E_ClusterNbr.push_back(DetNbr);
    }
    void SetPARISLaBr3StageEPhoswichNbr(UShort_t PhoswichNbr)    {
      fPARIS_LaBr3Stage_E_PhoswichNbr.push_back(PhoswichNbr);
    }
    void SetPARISLaBr3StageEEnergy(Double_t Energy)      {
      fPARIS_LaBr3Stage_E_Energy.push_back(Energy);
    }

    // (T)
    void SetParisLaBr3T(UShort_t DetNbr,UShort_t PhoswichNbr, Double_t T){
      fPARIS_LaBr3Stage_T_ClusterNbr.push_back(DetNbr); 
      fPARIS_LaBr3Stage_T_PhoswichNbr.push_back(PhoswichNbr); 
      fPARIS_LaBr3Stage_T_Time.push_back(T); 
    }



    void SetPARISLaBr3StageTClusterNbr(UShort_t DetNbr) {
      fPARIS_LaBr3Stage_T_ClusterNbr.push_back(DetNbr);
    }
    void SetPARISLaBr3StageTPhoswichNbr(UShort_t PhoswichNbr)    {
      fPARIS_LaBr3Stage_T_PhoswichNbr.push_back(PhoswichNbr);
    }
    void SetPARISLaBr3StageTTime(Double_t Time)      {
      fPARIS_LaBr3Stage_T_Time.push_back(Time);
    }

    // Second stage NaI
    // (E)
    void SetParisNaIE(UShort_t DetNbr,UShort_t PhoswichNbr, Double_t E){
      fPARIS_NaIStage_E_ClusterNbr.push_back(DetNbr); 
      fPARIS_NaIStage_E_PhoswichNbr.push_back(PhoswichNbr); 
      fPARIS_NaIStage_E_Energy.push_back(E); 
    }

    void SetPARISNaIStageEClusterNbr(UShort_t DetNbr) {
      fPARIS_NaIStage_E_ClusterNbr.push_back(DetNbr);
    }
    void SetPARISNaIStageEPhoswichNbr(UShort_t PhoswichNbr)    {
      fPARIS_NaIStage_E_PhoswichNbr.push_back(PhoswichNbr);
    }
    void SetPARISNaIStageEEnergy(Double_t Energy)      {
      fPARIS_NaIStage_E_Energy.push_back(Energy);
    }

    // (T)
    void SetParisNaIT(UShort_t DetNbr,UShort_t PhoswichNbr, Double_t T){
      fPARIS_NaIStage_T_ClusterNbr.push_back(DetNbr); 
      fPARIS_NaIStage_T_PhoswichNbr.push_back(PhoswichNbr); 
      fPARIS_NaIStage_T_Time.push_back(T); 
    }   

    void SetPARISNaIStageTClusterNbr(UShort_t DetNbr) {
      fPARIS_NaIStage_T_ClusterNbr.push_back(DetNbr);
    }
    void SetPARISNaIStageTPhoswichNbr(UShort_t PhoswichNbr)    {
      fPARIS_NaIStage_T_PhoswichNbr.push_back(PhoswichNbr);
    }
    void SetPARISNaIStageTTime(Double_t Time)  {
      fPARIS_NaIStage_T_Time.push_back(Time);
    }

    ClassDef(TParisData,1)  // ParisData structure
};

#endif

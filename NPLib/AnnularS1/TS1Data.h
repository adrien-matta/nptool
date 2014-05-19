#ifndef __S1DATA__
#define __S1DATA__
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the results of the G4 simulation for the S1 *
 *             detector.                                                     *
 *             The format is the same as the one which is used for the GANIL *
 *             experiments after conversion of the raw data with GRU. Ask    *
 *             N. de Sereville and J. Burgunder (burgunder@ganil.fr) for     *
 *             informations.                                                 *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <vector>
#include "TObject.h"
using namespace std;
class TS1Data : public TObject {
  private:
    // DSSD
    // Theta strips
    // ADC
    vector<UShort_t>   fS1_Theta_E_DetNbr;
    vector<UShort_t>   fS1_Theta_E_StripNbr;
    vector<Double_t>   fS1_Theta_E_Energy;
    // TDC
    vector<UShort_t>   fS1_Theta_T_DetNbr;
    vector<UShort_t>   fS1_Theta_T_StripNbr;
    vector<Double_t>   fS1_Theta_T_Time;
    // Phi strips
    // ADC
    vector<UShort_t>   fS1_Phi_E_DetNbr;
    vector<UShort_t>   fS1_Phi_E_StripNbr;
    vector<Double_t>   fS1_Phi_E_Energy;
    // TDC
    vector<UShort_t>   fS1_Phi_T_DetNbr;
    vector<UShort_t>   fS1_Phi_T_StripNbr;
    vector<Double_t>   fS1_Phi_T_Time;

  public:
    TS1Data();
    virtual ~TS1Data();

    void Clear();
    void Clear(const Option_t*) {};
    void Dump() const;

    /////////////////////           GETTERS           ////////////////////////
    // (Th,E)
    const unsigned int   GetS1ThetaEMult()                 const {return fS1_Theta_E_DetNbr.size();}
    const int   GetS1ThetaEDetectorNbr(const int& i)   const {return fS1_Theta_E_DetNbr.at(i);}
    const int   GetS1ThetaEStripNbr(const int& i)      const {return fS1_Theta_E_StripNbr.at(i);}
    const double   GetS1ThetaEEnergy(const int& i)        const {return fS1_Theta_E_Energy.at(i);}
    // (Th,T)
    const unsigned int   GetS1ThetaTMult()                 const {return fS1_Theta_T_DetNbr.size();}
    const int   GetS1ThetaTDetectorNbr(const int& i)   const {return fS1_Theta_T_DetNbr.at(i);}
    const int   GetS1ThetaTStripNbr(const int& i)      const {return fS1_Theta_T_StripNbr.at(i);}
    const double   GetS1ThetaTTime(const int& i)          const {return fS1_Theta_T_Time.at(i);}
    // (Ph,E)
    const unsigned int   GetS1PhiEMult()                const {return fS1_Phi_E_DetNbr.size();}
    const int   GetS1PhiEDetectorNbr(const int& i)  const {return fS1_Phi_E_DetNbr.at(i);}
    const int   GetS1PhiEStripNbr(const int& i)     const {return fS1_Phi_E_StripNbr.at(i);}
    const double   GetS1PhiEEnergy(const int& i)       const {return fS1_Phi_E_Energy.at(i);}
    // (Ph,T)
    const unsigned int   GetS1PhiTMult()                const {return fS1_Phi_T_DetNbr.size();}
    const int   GetS1PhiTDetectorNbr(const int& i)  const {return fS1_Phi_T_DetNbr.at(i);}
    const int   GetS1PhiTStripNbr(const int& i)     const {return fS1_Phi_T_StripNbr.at(i);}
    const double   GetS1PhiTTime(const int& i)         const {return fS1_Phi_T_Time.at(i);}

    /////////////////////           SETTERS           ////////////////////////
    // (Th,E)
    void   SetS1ThetaEDetectorNbr(const int& det)  {fS1_Theta_E_DetNbr.push_back(det);}
    void   SetS1ThetaEStripNbr(const int& Nr)      {fS1_Theta_E_StripNbr.push_back(Nr);}
    void   SetS1ThetaEEnergy(const double& E)         {fS1_Theta_E_Energy.push_back(E);}
    // (Th,T)
    void   SetS1ThetaTDetectorNbr(const int& det)  {fS1_Theta_T_DetNbr.push_back(det);}
    void   SetS1ThetaTStripNbr(const int& Nr)      {fS1_Theta_T_StripNbr.push_back(Nr);}
    void   SetS1ThetaTTime(const double& T)           {fS1_Theta_T_Time.push_back(T);}
    // (Ph,E)
    void   SetS1PhiEDetectorNbr(const int& det) {fS1_Phi_E_DetNbr.push_back(det);}
    void   SetS1PhiEStripNbr(const int& Nr)     {fS1_Phi_E_StripNbr.push_back(Nr);}
    void   SetS1PhiEEnergy(const double& E)        {fS1_Phi_E_Energy.push_back(E);}
    // (Ph,T)
    void   SetS1PhiTDetectorNbr(const int& det) {fS1_Phi_T_DetNbr.push_back(det);}
    void   SetS1PhiTStripNbr(const int& Nr)     {fS1_Phi_T_StripNbr.push_back(Nr);}
    void   SetS1PhiTTime(const double& T)          {fS1_Phi_T_Time.push_back(T);}

    ClassDef(TS1Data,2)  // S1Data structure
};

#endif

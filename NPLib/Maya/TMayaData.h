#ifndef __MAYADATA__
#define __MAYADATA__
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Vandebrouck   contact address: vandebro@ipno.in2p3.fr *
 *                  J. Gibelin                      gibelin@lpccaen.in2p3.fr *
 *                                                                           *
 * Creation Date  : November 2011                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds Maya raw data                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/


#include <vector>
#include <map>

#include "TObject.h"
#include <iostream>
#include "ConfigReader.h"

#define	MAYA_PAD_NPARCAL 3
#define	MAYA_PAD_NCHCAL	1024
#define MAYA_PAD_SIZE 5 //mm
#define MAYA_PAD_HEIGHT (MAYA_PAD_SIZE*0.8660254) //mm
#define MAYA_QFIL_NCHCAL 33
#define MAYA_QFIL_E_NPARCAL 2
#define MAYA_QFIL_T_NPARCAL 2
#define MAYA_SI_NCHCAL 20
#define MAYA_SI_E_NPARCAL 2
#define MAYA_SI_T_NPARCAL 2




class TMayaData : public TObject {
 private:

 
  // (PAD)/CRAMS
  vector<UShort_t> fMaya_PAD_Nbr;
  vector<UShort_t> fMaya_PAD_Col;
  vector<UShort_t> fMaya_PAD_Row;
  vector<UShort_t> fMaya_PAD_E;
  vector<Float_t> fMaya_PAD_x;
  vector<Float_t> fMaya_PAD_y;

  map<UShort_t,UShort_t> fMaya_PAD_map_idx ; //!
  

  bool bMaya_IsPAD_Calibrated ; 
  bool bMaya_Clean_PADS ;  // boolean, parameter -> if true clean the PAD calibrated matrix
  Int_t fMaya_PAD_Neighbors_Min; //! number of minimum Neighbors before cleaning
  vector<vector<Float_t> > fMaya_PAD_E_calpar;//! 
  vector<Float_t> fMaya_PAD_E_cal ;
  vector<UShort_t> fMaya_PAD_Neighbors ;
  Float_t fMaya_PAD_E_cal_threshold; //!
  UShort_t fMaya_PAD_Mult_cal  ;
  UShort_t fMaya_PAD_Row_Mult[32] ; 
  Float_t fMaya_PAD_Row_Sum_cal[32] ; 
  Float_t fMaya_PAD_Row_Max_cal[32] ; 
  UShort_t fMaya_PAD_Col_Mult[32] ; 
  Float_t fMaya_PAD_Col_Sum_cal[32] ; 
  Float_t fMaya_PAD_Col_Max_cal[32] ; 

  ULong64_t fMaya_PAD_Sum ;   
  Float_t fMaya_PAD_Sum_cal ;   

  // (QFIL)
  bool bMaya_IsQFIL_Calibrated ;
  vector<UShort_t> fMaya_QFIL_E_Nbr ;
  vector<UShort_t> fMaya_QFIL_T_Nbr ;

  vector<vector<Float_t> > fMaya_QFIL_E_calpar;//! 
  vector<UShort_t> fMaya_QFIL_E ;
  vector<Float_t> fMaya_QFIL_E_cal ;
  vector<vector<Float_t> > fMaya_QFIL_T_calpar;//! 
  vector<UShort_t> fMaya_QFIL_T ;
  vector<Float_t> fMaya_QFIL_T_cal ;

  ULong64_t fMaya_QFIL_Sum ;   
  Float_t fMaya_QFIL_Sum_cal ;   

  // (Max) 
  // Float_t fMaya_Max_cal ;
  // UShort_t fMaya_Max_Nbr ;

  // Silicons
  bool bMaya_IsSi_Calibrated ;
  //  vector<UShort_t>  fMaya_Si_Nbr; // TODO
  // * Energy * //
  vector<UShort_t>  fMaya_Si_E_Nbr;
  vector<UShort_t>  fMaya_Si_E;
  vector<vector<Float_t> > fMaya_Si_E_calpar;//! 
  vector<Float_t>  fMaya_Si_E_cal;
  // * Time * //
  //  map<UShort_t, UShort_t> fMaya_Si_EvsT_index; //! TODO
  vector<UShort_t>  fMaya_Si_T_Nbr;
  vector<UShort_t>  fMaya_Si_T;
  vector<vector<Float_t> > fMaya_Si_T_calpar;//! 
  vector<Float_t>  fMaya_Si_T_cal;

  // FADC : to record wire pulses
  vector<UShort_t> fMaya_QFil_Pulse_E; 
  vector<UShort_t> fMaya_QFil_Pulse_Nbr; // sample number  
  vector<UShort_t> fMaya_QFil_Pulse_Ch;  // FADC channel: [0:3]
  map<UShort_t, UShort_t> fMaya_QFil_Pulse_map_Current_Nbr; //! keep track of current FADC sanple number within on channel, not save
  vector<ULong64_t> fMaya_QFil_Pulse_Sum ;
  vector<ULong64_t> fMaya_QFil_Pulse_Sum_Ch ;

  // TOF
  UShort_t fMaya_TOF_GAL_HF ;
  UShort_t fMaya_TOF_GAL_DIAM ;
  UShort_t fMaya_TOF_TM_TRIG ;

 public:
  TMayaData();
  virtual ~TMayaData();

  void	Clear();
  void	Clear(const Option_t*){Clear();}
  void	Dump() const;
   
  /*           GETTERS           */
  // (E)
  UShort_t GetEsize(void)     {return fMaya_PAD_E.size();}
  UShort_t GetEnergy(Int_t i) {return fMaya_PAD_E.at(i);}
  UShort_t GetNbr(Int_t i)    {return fMaya_PAD_Nbr.at(i);}
  UShort_t GetCol(Int_t i)    {return GetPadCol(fMaya_PAD_Nbr.at(i));}
  UShort_t GetRow(Int_t i)    {return GetPadRow(fMaya_PAD_Nbr.at(i));}
  //
  Float_t GetEnergy_cal(Int_t i) {return fMaya_PAD_E_cal.at(i);}
  /*
  UShort_t *GetEnergy_cal(void) { 
    UShort_t *a = new UShort_t [fMaya_PAD_E_cal.size()];
    copy(fMaya_PAD_E_cal.begin(),fMaya_PAD_E_cal.end(), a);
    return a;
  }
  */
  UShort_t GetPADMult_cal(){ return fMaya_PAD_Mult_cal  ; }

  //sum
  ULong64_t GetPADSum()          {return fMaya_PAD_Sum;}
  Float_t   GetPADSum_cal()      {return fMaya_PAD_Sum_cal;}
  // si
  UShort_t  GetEnergySi(Int_t i) { return fMaya_Si_E.at(i) ;} // for backward compatibility

  UShort_t  GetSi_E_size()     {return fMaya_Si_E_Nbr.size();}
  UShort_t  GetSi_E(Int_t i)   {return fMaya_Si_E.at(i);}
  Float_t   GetSi_E_cal(Int_t i) {return fMaya_Si_E_cal.at(i);}
  UShort_t  GetSi_E_Nbr(Int_t i)  {return fMaya_Si_E_Nbr.at(i);}
  
  UShort_t  GetSi_T_size()     {return fMaya_Si_T_Nbr.size();}
  UShort_t  GetSi_T(Int_t i)   {return fMaya_Si_T.at(i);}
  UShort_t  GetSi_T_Nbr(Int_t i)  {return fMaya_Si_T_Nbr.at(i);}


  // wires
  UShort_t  GetQFil_E_size()     {return fMaya_QFIL_E_Nbr.size();}
  UShort_t  GetQFil_E(Int_t i)   {return fMaya_QFIL_E.at(i);}
  UShort_t  GetQFil(Int_t i)   {return fMaya_QFIL_E.at(i);} // for backward compatibility
  UShort_t  GetQFil_E_Nbr(Int_t i)  {return fMaya_QFIL_E_Nbr.at(i);}
  
  UShort_t  GetQFil_T_size()     {return fMaya_QFIL_T_Nbr.size();}
  UShort_t  GetQFil_T(Int_t i)   {return fMaya_QFIL_T.at(i);}
  Float_t  GetQFil_T_cal(Int_t i)   {return fMaya_QFIL_T_cal.at(i);}
  UShort_t  GetQFil_T_Nbr(Int_t i)  {return fMaya_QFIL_T_Nbr.at(i);}

  UShort_t  GetQFilPulse_size(void)   {return fMaya_QFil_Pulse_E.size();}
  UShort_t  GetQFilPulse(Int_t i)   {return fMaya_QFil_Pulse_E.at(i);}
  UShort_t  GetQFilPulseCh(Int_t i)   {return fMaya_QFil_Pulse_Ch.at(i);}
  UShort_t  GetQFilPulseNbr(Int_t i)   {return fMaya_QFil_Pulse_Nbr.at(i);}
  ULong64_t  GetQFilPulseSum(Int_t i)   {return fMaya_QFil_Pulse_Sum.at(i);}
  UShort_t  GetQFilPulseSumCh(Int_t i)   {return fMaya_QFil_Pulse_Sum_Ch.at(i);}


  /*           SETTERS           */
  //sum
  void SetPADSum(UShort_t E)        {fMaya_PAD_Sum += (Int_t)E ; }
  void SetQFILSum(UShort_t E)        {fMaya_QFIL_Sum += (Int_t)E ; }
  // (PAD)
  void	SetEnergyPad_CR(UShort_t C, UShort_t R, UShort_t E) ; // column, row, energy
  void	SetEnergyPad(UShort_t N, UShort_t E) ; // cumulated index, energy
  
  Int_t GetPadIdx(UShort_t C, UShort_t R){ return (R-1)*32+C;}; // from 1 to 1024
  Int_t GetPadCol(UShort_t I){return int((I-1)%32)+1;};
  Int_t GetPadRow(UShort_t I){return int((I-1)/32)+1;};


  // QFIL
  void SetQfil(UShort_t N,UShort_t E) {
    fMaya_QFIL_E_Nbr.push_back(N);
    fMaya_QFIL_E.push_back(E) ;
    if(bMaya_IsQFIL_Calibrated){
      fMaya_QFIL_E_cal.push_back(-1) ;
    }
  }

  void SetTfil(UShort_t N,UShort_t T) {
    fMaya_QFIL_T_Nbr.push_back(N);
    fMaya_QFIL_T.push_back(T) ;
    if(bMaya_IsQFIL_Calibrated){
      fMaya_QFIL_T_cal.push_back(-1) ;
    }
  }

  void SetQFilPulse(UShort_t N,UShort_t E); 

  // SI
  void SetEnergySi(UShort_t ,UShort_t );
  void SetTimeSi(UShort_t ,UShort_t );

  /*           CALIBRATION       */
  bool InitCalibrate(std::vector<string>&) ; //!
  bool Calibrate(void) ;                     //!
  
  // TOF

  void SetTOF(UShort_t ,UShort_t ) ;
  UShort_t GetTOF_GAL_HF() { return fMaya_TOF_GAL_HF ; } ;
  UShort_t GetTOF_GAL_DIAM() { return fMaya_TOF_GAL_DIAM ; } ;
  UShort_t GetTOF_TM_TRIG() { return fMaya_TOF_TM_TRIG ; } ;

  ClassDef(TMayaData,2)  // MayaData structure
    };

#endif

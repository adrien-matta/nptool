#ifndef TMUST2SPECTRA_H
#define TMUST2SPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : march 2011                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Must2                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ STL headers
#include <map>

// ROOT headers
#include "TObject.h"
#include <TH1.h>
#include <TH2.h>
#include <TString.h>

// NPLib headers
#include "TMust2Data.h"

using namespace std;

class TMust2Spectra {
  public:
    // constructor and destructor
    TMust2Spectra();
    TMust2Spectra(unsigned int NumberOfTelescope);
    ~TMust2Spectra();

  private:
    // Instantiate and register histo to maps
    TH1* AddHisto1D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString family);
    TH1* AddHisto2D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, 
        Int_t nbinsy, Double_t ylow, Double_t yup,TString family);

    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();

public:
    // Filling methods
    void FillRawSpectra(TMust2Data*);
    void FillPreTreatedSpectra(TMust2Data*);


  public:
    // get map histo which will be used for GSpectra in GUser
    map< vector<TString>, TH1* > GetMapHisto() const {return fMapHisto;}
    TH1* GetHisto(TString family,TString name);    
    void WriteHisto(TString filename="VOID");      

  private: // Information on MUST2
    unsigned int fNumberOfTelescope;
    unsigned int fStripX;
    unsigned int fStripY;
    unsigned int fPadSili;
    unsigned int fCrystalCsI;

  private:
    // map holding histo pointers and their family names
    map< vector<TString>, TH1* > fMapHisto;
};

#endif

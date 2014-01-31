#ifndef TCHARISSASPECTRA_H
#define TCHARISSASPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2013    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : dec 2013                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Charissa              *
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

// NPLib headers
#include "TCharissaData.h"
#include "TCharissaPhysics.h"
using namespace std;

// ForwardDeclaration
class TCharissaPhysics;

class TCharissaSpectra {
  public:
    // constructor and destructor
    TCharissaSpectra();
    TCharissaSpectra(unsigned int NumberOfTelescope);
    ~TCharissaSpectra();

  private:
    // Instantiate and register histo to maps
    TH1* AddHisto1D(string name, string title, Int_t nbinsx, Double_t xlow, Double_t xup, string family);
    TH1* AddHisto2D(string name, string title, Int_t nbinsx, Double_t xlow, Double_t xup, 
                                                 Int_t nbinsy, Double_t ylow, Double_t yup, string family);

    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TCharissaData*);
    void FillPreTreatedSpectra(TCharissaData*);
    void FillPhysicsSpectra(TCharissaPhysics*);
    // Check the Spectra
    void CheckSpectra();

  public:
    // get map histo which will be used for GSpectra in GUser
    map< vector<string>, TH1* > GetMapHisto() const {return fMapHisto;}
    TH1* GetHisto(string& family,string& name);    
    void WriteHisto(string filename="VOID");      

  private: // Information on CHARISSA
    unsigned int fNumberOfTelescope;
    unsigned int fStripX;
    unsigned int fStripY;
    unsigned int fCrystalCsI;

  private:
    // map holding histo pointers and their family names
    map< vector<string>, TH1* > fMapHisto;
};

#endif

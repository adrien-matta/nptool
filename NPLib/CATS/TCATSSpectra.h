#ifndef TCATSSPECTRA_H
#define TCATSSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: A. Matta         contact address: a.matta@surrey.ac.uk   *
 *                                                                           *
 * Creation Date  : dec 2013                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for CATS                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// ROOT headers
#include "TObject.h"
#include <TH1.h>
#include <TH2.h>

// NPLib headers
#include "../include/VSpectra.h"
#include "TCATSData.h"
#include "TCATSPhysics.h"

// C++ STL headers
#include <map>
using namespace std;


// Forward Declaration
class TCATSPhysics;


class TCATSSpectra:public VSpectra {
  public:
    // constructor and destructor
    TCATSSpectra();
    TCATSSpectra(unsigned int NumberOfCats);
    ~TCATSSpectra();

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
    void FillRawSpectra(TCATSData*);
    void FillPreTreatedSpectra(TCATSData*);
    void FillPhysicsSpectra(TCATSPhysics*);

  private: // Information on MUST2
    unsigned int fNumberOfCats;
    unsigned int fStripsNumber;
    int fEventLoopSize;
    int fEventLoopStep;
    int fEventLoopIndex;
    vector<double> fEventLoopQSum;

};

#endif

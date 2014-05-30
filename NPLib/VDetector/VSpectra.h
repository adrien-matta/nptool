#ifndef VSPECTRA_H
#define VSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: A. Matta         contact address: a.matta@surrey.ac.uk   *
 *                                                                           *
 * Creation Date  : may 2014                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Virtual class for the TDetectorSpectra classes                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// ROOT headers
#include "TObject.h"
#include "TH1.h"

// C++ STL headers
#include <map>
#include <vector>
#include <string>
using namespace std;

class VSpectra {
  public:
    // constructor and destructor
    VSpectra();
    virtual ~VSpectra();

//  private:
  public:
    // Instantiate and register histo to maps
    TH1* AddHisto1D(string name, string title, Int_t nbinsx, Double_t xlow, Double_t xup, string family);
    TH1* AddHisto2D(string name, string title, Int_t nbinsx, Double_t xlow, Double_t xup, 
        Int_t nbinsy, Double_t ylow, Double_t yup, string family);

  public:
    // Initialization methods
    virtual void InitRawSpectra()         = 0;
    virtual void InitPreTreatedSpectra()  = 0;
    virtual void InitPhysicsSpectra()     = 0;

  public:
    // Filling methods
    virtual void FillRawSpectra(void*) {};
    virtual void FillPreTreatedSpectra(void*) {};
    virtual void FillPhysicsSpectra(void*) {}; 
    virtual void CheckSpectra(){};

  public:
    // get map histo which will be used for GSpectra in GUser
    map< vector<string>, TH1* > GetMapHisto() const {return fMapHisto;}
    TH1* GetHisto(string& family, string& name);    
    void WriteHisto(string filename = "VOID");      

  private:
    // map holding histo pointers and their family names
    map< vector<string>, TH1* > fMapHisto;
};

#endif

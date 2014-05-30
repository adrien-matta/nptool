#ifndef TCHARISSASPECTRA_H
#define TCHARISSASPECTRA_H
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
 *  This class holds all the online spectra needed for Charissa              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "../include/VSpectra.h"
#include "TCharissaData.h"
#include "TCharissaPhysics.h"


// ForwardDeclaration
class TCharissaPhysics;

class TCharissaSpectra: public VSpectra {
  public:
    // constructor and destructor
    TCharissaSpectra();
    TCharissaSpectra(unsigned int NumberOfTelescope);
    ~TCharissaSpectra();

  private:
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
//    void CheckSpectra();


  private: // Information on CHARISSA
    unsigned int fNumberOfTelescope;
    unsigned int fStripX;
    unsigned int fStripY;
    unsigned int fCrystalCsI;
};

#endif

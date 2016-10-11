#ifndef TCOMPTONTELESCOPESPECTRA_H
#define TCOMPTONTELESCOPESPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : jan. 2015                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Compton Telescope     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TComptonTelescopeData.h"
#include "TComptonTelescopePhysics.h"

// ForwardDeclaration
class TComptonTelescopePhysics ;

class TComptonTelescopeSpectra:public VSpectra {
  public:
    // constructor and destructor
    TComptonTelescopeSpectra();
    TComptonTelescopeSpectra(unsigned int NumberOfTelescope);
    ~TComptonTelescopeSpectra();

  private:
    // Initialization methods
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  public:
    // Filling methods
    void FillRawSpectra(TComptonTelescopeData*);
    void FillPreTreatedSpectra(TComptonTelescopeData*);
    void FillPhysicsSpectra(TComptonTelescopePhysics*);

  private: // Information on Compton Telescope 
    unsigned int fNumberOfTelescope;
    unsigned int fStripX;
    unsigned int fStripY;
    unsigned int fCrystalCsI;
};

#endif

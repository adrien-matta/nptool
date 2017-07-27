#ifndef TFoilSPECTRA_H
#define TFoilSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mhd Moukaddam  contact address: m.moukaddam@surrey.ac.uk *
 *                                                                           *
 * Creation Date  : July 2017                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds the spectra of a simple generic foil normally           *
 *  used in experiments as a catcher before/after target or a screen of      *
 *  other detectors e.g. the common Aluminum foil placed upstream the        *
 *  TRIFOIL with SHARC experiments                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TFoilData.h"
#include "TFoilPhysics.h"

// Forward Declaration
class TFoilPhysics;


class TFoilSpectra : public VSpectra {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
  public:
    TFoilSpectra();
    TFoilSpectra(unsigned int NumberOfDetectors);
    ~TFoilSpectra();

  //////////////////////////////////////////////////////////////
  // Initialization methods
  private:
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  //////////////////////////////////////////////////////////////
  // Filling methods
  public:
    void FillRawSpectra(TFoilData*);
    void FillPreTreatedSpectra(TFoilData*);
    void FillPhysicsSpectra(TFoilPhysics*);

  //////////////////////////////////////////////////////////////
  // Detector parameters 
  private:
    unsigned int fNumberOfDetectors;
};

#endif

#ifndef THelios2SPECTRA_H
#define THelios2SPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: matta@lpccaen.in2p3.fr                        *
 *                                                                           *
 * Creation Date  : octobre 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Helios2 Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "THelios2Data.h"
#include "THelios2Physics.h"

// Forward Declaration
class THelios2Physics;


class THelios2Spectra : public VSpectra {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
  public:
    THelios2Spectra();
    THelios2Spectra(unsigned int NumberOfDetectors);
    ~THelios2Spectra();

  //////////////////////////////////////////////////////////////
  // Initialization methods
  private:
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  //////////////////////////////////////////////////////////////
  // Filling methods
  public:
    void FillRawSpectra(THelios2Data*);
    void FillPreTreatedSpectra(THelios2Data*);
    void FillPhysicsSpectra(THelios2Physics*);

  //////////////////////////////////////////////////////////////
  // Detector parameters 
  private:
    unsigned int fNumberOfDetectors;
};

#endif

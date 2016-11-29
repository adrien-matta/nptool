#ifndef TFPDTamuSPECTRA_H
#define TFPDTamuSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: m.moukaddam@surrey.ac.uk                                 *
 *                                                                           *
 * Creation Date  : November 2016                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold FPDTamu Spectra                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TFPDTamuData.h"
#include "TFPDTamuPhysics.h"

// Forward Declaration
class TFPDTamuPhysics;


class TFPDTamuSpectra : public VSpectra {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
  public:
    TFPDTamuSpectra();
    TFPDTamuSpectra(unsigned int NumberOfDetectors);
    ~TFPDTamuSpectra();

  //////////////////////////////////////////////////////////////
  // Initialization methods
  private:
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  //////////////////////////////////////////////////////////////
  // Filling methods
  public:
    void FillRawSpectra(TFPDTamuData*);
    void FillPreTreatedSpectra(TFPDTamuData*);
    void FillPhysicsSpectra(TFPDTamuPhysics*);

  //////////////////////////////////////////////////////////////
  // Detector parameters 
  private:
    unsigned int fNumberOfDetectors;
};

#endif

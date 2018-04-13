#ifndef TAnnularTelescopeSPECTRA_H
#define TAnnularTelescopeSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : March 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularTelescope Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TAnnularTelescopeData.h"
#include "TAnnularTelescopePhysics.h"

// Forward Declaration
class TAnnularTelescopePhysics;


class TAnnularTelescopeSpectra : public VSpectra {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
  public:
    TAnnularTelescopeSpectra();
    TAnnularTelescopeSpectra(unsigned int NumberOfDetectors);
    ~TAnnularTelescopeSpectra();

  //////////////////////////////////////////////////////////////
  // Initialization methods
  private:
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra();

  //////////////////////////////////////////////////////////////
  // Filling methods
  public:
    void FillRawSpectra(TAnnularTelescopeData*);
    void FillPreTreatedSpectra(TAnnularTelescopeData*);
    void FillPhysicsSpectra(TAnnularTelescopePhysics*);

  //////////////////////////////////////////////////////////////
  // Detector parameters 
  private:
    unsigned int fNumberOfDetectors;
};

#endif

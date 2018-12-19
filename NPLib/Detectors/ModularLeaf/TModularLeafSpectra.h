#ifndef TModularLeafSPECTRA_H
#define TModularLeafSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold DETECTORNAME Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// NPLib headers
#include "NPVSpectra.h"
#include "TModularLeafPhysics.h"

#include <string>
#include <vector>

// Forward Declaration
class TModularLeafPhysics;


class TModularLeafSpectra : public VSpectra {
  //////////////////////////////////////////////////////////////
  // constructor and destructor
  public:
    TModularLeafSpectra();
    TModularLeafSpectra(std::vector<std::string> leafs, std::vector<std::string> leafs_X, std::vector<std::string> leafs_Y);
    ~TModularLeafSpectra();

  //////////////////////////////////////////////////////////////
  // Initialization methods
  private:
    void InitRawSpectra();
    void InitPreTreatedSpectra();
    void InitPhysicsSpectra(std::vector<std::string>& leafs_X, std::vector<std::string>& leafs_Y);
    TH1F* f_ModularHisto;

  //////////////////////////////////////////////////////////////
  // Filling methods
  public:
    void FillRawSpectra(std::map<std::string,short>&);
    void FillPreTreatedSpectra(std::map<std::string, double>&);
    void FillPhysicsSpectra(std::vector<std::string>& leafs_X, std::vector<std::string>& leafs_Y,std::map<std::string, double>& CalibratedData);


public:
  //////////////////////////////////////////////////////////////
  // Detector parameters 
  private:
    std::vector<std::string> fleafs;
    
};

#endif

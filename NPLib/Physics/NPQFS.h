#ifndef NPQFS_h
#define NPQFS_h
/*****************************************************************************/
/*****************************************************************************
 * Copyright (C) 2009-2019   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  F. Flavigny contact address: flavigny@ipno.in2p3.fr    *
 *                                                                           *
 * Creation Date   : April 2019                                              *
 * Last update     : April 2019                                              *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class deal with Quasi Free Scattering Reaction in which a cluster   *
 *  or a nucleon is removed from a projectile  by interaction with a target  *
 *  nucleon (proton target in general)                                       *
 *                                                                           *
 * Labeling is:                                                              *
 *              A --> i  ==> B + (i -> a) =>  B + 1 + 2                      *
 *                                                                           *
 *  where:                                                                   *
 *    +  A is the beam nucleus                                               *
 *    +  i is the target nucleon (proton)                                    *
 *                                                                           *
 *    +  B is the residual fragment (beam-like)                              *
 *    +  1 is the scattered target nucleon  (former i)                       *
 *    +  2 is the knocked-out cluster/nucleon (noted a) in the intermediate  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    +                                                                      *
 *                                                                           *
 *****************************************************************************/

// C++ header
#include <string>

// NPL
#include "NPNucleus.h"
#include "NPInputParser.h"
using namespace NPL;

// ROOT header
#include "TLorentzVector.h"
#include "TLorentzRotation.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TRandom3.h"

using namespace std;

namespace NPL{

  class QFS{

    public:  // Constructors and Destructors
      QFS(){};
      ~QFS(){};

    private:
    int fVerboseLevel;
 
    ClassDef(QFS,0)
  };
}
#endif

#ifndef TTIGRESSCLOVER_H
#define TTIGRESSCLOVER_H
/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                  Peter Bender  contact address: bender@triumf.ca          *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Tigress treated data                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include <vector>
#include <string>
#include <cmath>
using namespace std;

// NPL
#include "TTigressCrystal.h"

// ROOT
#include "TVector3.h"

class TTigressClover {

  public:
    TTigressClover(int CloverId=0, double R=0, double Theta=0, double Phi=0);
    ~TTigressClover()  { };

    void Clear();
    TTigressCrystal GetCrystal(int i){return m_Crystal[i-1];};
    
  public:
    void AddCrystals(TVector3 CrystalPosition, TVector3 CrystalNormal);

    void RotateToPosition(double theta, double phi);

    vector<TTigressCrystal> m_Crystal;

  private:
    int m_CloverId; //clover number.
    TVector3 m_Position;

};

#endif

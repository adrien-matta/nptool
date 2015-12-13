#ifndef __NPFUNCTION__
#define __NPFUNCTION__

/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: a.matta@surrey.ac.uk  *
 *                                                                           *
 * Creation Date   : January 2013                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This file hold some method common to different object of the NPL Physics *
 * librairy.                                                                 *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// STL
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include<cmath>

using namespace std;

// ROOT
#include "TH1F.h"
#include "TH2F.h"
#include "TH1.h"
#include "TFile.h"
#include "TRandom.h"
#include "TRandom2.h"

namespace NPL{
  
  // Check the type of Filename (root or ASCII) and extract build/extract a 1D histogramm
  TH1F* Read1DProfile(string filename,string HistName);
  
  // Check the type of Filename (root or ASCII) and extract build/extract a 2D histogramm
  TH2F* Read2DProfile(string filename,string HistName);
  
  // Open a file at Filename after checking the type of file it is
  // true for a ASCII file
  // False for a Root file
  bool OpenASCIIorROOTFile(string filename, ifstream &ASCII , TFile &ROOT);
  
  void RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double &X, double &Y, double NumberOfSigma= 10000);
}

#endif

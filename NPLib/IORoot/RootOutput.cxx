/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class is a singleton class which deals with the ROOT     *
 *             output file and tree both for NPSimulation and NPAnalysis.    *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <cstdlib>

#include "RootOutput.h"

using namespace std;

RootOutput *RootOutput::instance = 0;

RootOutput* RootOutput::getInstance(TString fileNameBase, TString treeNameBase)
{
   // A new instance of RootOutput is created if it does not exist:
   if (instance == 0) {
      instance = new RootOutput(fileNameBase, treeNameBase);
   }

   // The instance of RootOutput is returned:
   return instance;
}


void RootOutput::Destroy()
{
   if (instance != 0) {
      delete instance;
      instance = 0;
   }
}


RootOutput::RootOutput(TString fileNameBase, TString treeNameBase)
{
   // The file extension is added to the file name:
   TString GlobalPath = getenv("NPTOOL");
   TString fileName = GlobalPath + "/Outputs/" + fileNameBase + ".root";

   // The ROOT file is created
   pRootFile = new TFile(fileName, "RECREATE");

   pRootTree = new TTree(treeNameBase, "Data created / analyzed with the NPTool package");
   pRootList = new TList();
}



RootOutput::~RootOutput()
{
   // The data is written to the file and the tree is closed:
   if (pRootFile) {
      cout << endl;
      cout << "Got histograms and Tree !" << endl;
      pRootFile->Write();
      pRootFile->Close();
   } else {
      cout << "No histograms and Tree !" << endl;
   }
}

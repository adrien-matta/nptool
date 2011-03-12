/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 07/01/11                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This script loads automatically the NPLib include path and    *
 *             shared libraries.                                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment: This script should be called in your rootlogon.C file            *
 *          See example in NPTool/Misc/rootlogon.C                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <iostream>
#include <vector>
using namespace std;

// ROOT headers
#include "TSystem.h"
#include "TROOT.h"
#include "TList.h"
#include "TSystemDirectory.h"
#include "TString.h"

void NPToolLogon(bool verbosemode = false)
{
   TString currentpath = gSystem->Getenv("PWD");
   TString path = gSystem->Getenv("NPTOOL");
   
   // Add include path
   if (verbosemode) cout << "NPTool: adding include path : " << path << "/NPLib/include" << endl;
   gROOT->ProcessLine(Form(".include %s/NPLib/include", path.Data()));

   // Add shared libraries
   if (verbosemode) cout << "NPTool: loading NPLib shared libraries ..." << endl;
   TString libpath = Form("%s/NPLib/lib", path.Data());
   TSystemDirectory libdir("libdir", libpath);
   TList* listfile = libdir.GetListOfFiles();
   
   // Since the list is ordered alphabetically and that the 
   // libVDetector.so library should be loaded before the 
   // lib*Physics.so libraries, it is then loaded manually 
   // first.
   // Test if the lib directory is empty or not
   if (listfile->GetEntries() > 2) gSystem->Load(libpath+"/libVDetector.so");

   // Since the libMust2Physics.so library uses TVector2
   // objects, the libPhysics.so ROOT library is loaded.
   gSystem->Load("libPhysics.so");

   // Loop on all libraries
   Int_t i = 0;
   while (listfile->At(i)) {
      TString libname = listfile->At(i++)->GetName();
      if (libname.Contains("so") && !libname.Contains("libVDetector.so")) {
         TString lib = libpath + "/" + libname;
         gSystem->Load(lib);
      }
   }
   
   gROOT->ProcessLine(".L $NPTOOL/NPLib/include/RootInput.h+");   
   
   // Since the libdir.GetListOfFiles() commands cds to the
   // libidr directory, one has to return to the initial
   // directory
   gSystem->cd(currentpath);
   
   if (verbosemode) cout << "NPTool: Ready" << endl;
}

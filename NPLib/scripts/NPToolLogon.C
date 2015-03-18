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
#include "TRandom.h"
#include "TRandom2.h"

void NPToolLogon(bool verbosemode = false)
{
   // Change the standard random generator to TRandom2
   gRandom = new TRandom2();

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
   // libVDetector.dylib library should be loaded before the 
   // lib*Physics.dylib libraries, it is then loaded manually 
   // first.
   // Test if the lib directory is empty or not
   if (listfile->GetEntries() > 2) gSystem->Load(libpath+"/libNPCore.dylib");

   gSystem->Load("libPhysics.so"); // Needed by Must2 and Sharc
   gSystem->Load("libHist.so"); // Needed by TSpectra Class
   gSystem->Load("libCore.so"); // Need by Maya
   // Loop on Data libraries
   Int_t i = 0;
   while (listfile->At(i)) {
      TString libname = listfile->At(i++)->GetName();
      if (libname.Contains(".dylib") && libname.Contains("Data") && !libname.Contains("libVDetector.dylib")) {
         TString lib = libpath + "/" + libname;
         gSystem->Load(lib);
      }
   }
	  
   // Loop on Physics Library
   i = 0;
   while (listfile->At(i)) {
      TString libname = listfile->At(i++)->GetName();
      if (libname.Contains(".dylib") && libname.Contains("Physics") &&!libname.Contains("libVDetector.dylib")) {
         TString lib = libpath + "/" + libname;
         gSystem->Load(lib);
      }
   }
 
   // Loop on the Reset of the Library
   i = 0;
   while (listfile->At(i)) {
      TString libname = listfile->At(i++)->GetName();
      if (libname.Contains(".dylib") && !libname.Contains("Physics") && !libname.Contains("Data")  &&!libname.Contains("libVDetector.dylib")) {
         TString lib = libpath + "/" + libname;
         gSystem->Load(lib);
      }
   }


  // gROOT->ProcessLine(".L $NPTOOL/NPLib/include/RootInput.h+");   
 
   // Since the libdir.GetListOfFiles() commands cds to the
   // libidr directory, one has to return to the initial
   // directory
   gSystem->cd(currentpath);
   
   if (verbosemode) cout << "NPTool: Ready" << endl;
}

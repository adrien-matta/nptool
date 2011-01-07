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
 *             shared.                                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment: This script should be called in your rootlogon.C file            *
 *          See example in NPTool/Misc/rootlogon.C                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
 
#include "TSystem.h"
#include "TROOT.h"
#include "TList.h"
#include "TSystemDirectory.h"
#include "TString.h"
 
#include <iostream>
using namespace std;

void InitNPTool(bool quietmode = false)
{
   TString currentpath = gSystem->Getenv("PWD");
   TString path = gSystem->Getenv("NPLIB");
   
   // Add include path
   if(quietmode)
    cout << "NPTool: adding include path : " << path << "/include" << endl;
    
   gROOT->ProcessLine(Form(".include %s/include", path.Data()));
   // Add shared libraries
   if(quietmode)
   cout << "NPTool: loading NPLib shared libraries ..." << endl;
   TString libpath = Form("%s/lib", path.Data());
   TSystemDirectory libdir("libdir", libpath);
   TList* listfile = libdir.GetListOfFiles();
   
   // Since the list is ordered alphabetically and that the 
   // libVDetector.so library should be loaded before the 
   // lib*Physics.so libraries, it is then loaded manually 
   // first.
   gSystem->Load(libpath+"/libVDetector.so");

   // Since the libMust2Physics.so library uses TVector2
   // objects, the libPhysics.so ROOT library is loaded.
   gSystem->Load("libPhysics.so");

   Int_t i = 0;
   while (listfile->At(i)) {
      TString libname = listfile->At(i++)->GetName();
      if (libname.Contains("so") && !libname.Contains("libVDetector.so")) {
         TString lib     = libpath + "/" + libname;
         gSystem->Load(lib);
      }
   }
   
   TSystemDirectory dir("dir",currentpath);
   TList* listfile2 = dir.GetListOfFiles();
   
   if(quietmode)
   cout << "NPTool: Ready" << endl;
   
//   delete listfile2, listfile ;
}

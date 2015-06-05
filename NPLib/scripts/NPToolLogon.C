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
#include<iostream>
#include<string>
#include<stdlib.h>
#include<dlfcn.h>
#include<dirent.h>
using namespace std;

// ROOT headers
#include"TSystem.h"
#include"TROOT.h"
#include"TList.h"
#include"TSystemDirectory.h"
#include"TString.h"
#include"TRandom.h"
#include"TRandom2.h"

#ifdef __APPLE__
std::string LOGON_LIB_EXTENSION = ".dylib";
#endif
#ifdef __linux__
std::string LOGON_LIB_EXTENSION = ".so";
#endif
#ifdef __FreeBSD__
std::string LOGON_LIB_EXTENSION = ".so";
#endif


//#include "Style_nptool.C"

void NPToolLogon(){ 
  // Create the NPTool Stype
  TString NPLPath = gSystem->Getenv("NPTOOL");  
  gROOT->ProcessLine(Form(".x %s/NPLib/scripts/Style_nptool.C",NPLPath.Data()));


  // Change the standard random generator to TRandom2
  gRandom = new TRandom2();

  TString currentpath = gSystem->Getenv("PWD");
  TString path = gSystem->Getenv("NPTOOL");
 
  // Add include path
  gROOT->ProcessLine(Form(".include %s/NPLib/include", path.Data()));

  // Test if the root map exist, 
  // if yes exit
  // if no load the nptool lib
  DIR* dir;
  struct dirent *ent;
  TString libpath = path+"/NPLib/lib";
    string file;
  bool check = false;
  if((dir=opendir(libpath))!= NULL){
    while((ent = readdir(dir))!=NULL){
      file = ent->d_name;    
      if(file.find(".rootmap")!=std::string::npos){
        check=true;
        break;
      }
    }
  }

  if(!check){
    // Add shared libraries
    TString libpath = Form("%s/NPLib/lib", path.Data());
    TSystemDirectory libdir("libdir", libpath);
    TList* listfile = libdir.GetListOfFiles();

    // Since the list is ordered alphabetically and that the 
    // libVDetector.dylib library should be loaded before the 
    // lib*Physics.dylib libraries, it is then loaded manually 
    // first.
    // Test if the lib directory is empty or not
    if (listfile->GetEntries() > 2) gSystem->Load(libpath+"/libNPCore"+LOGON_LIB_EXTENSION);

    gSystem->Load("libPhysics.so"); // Needed by Must2 and Sharc
    gSystem->Load("libHist.so"); // Needed by TSpectra Class
    gSystem->Load("libCore.so"); // Need by Maya
    // Loop on Data libraries
    Int_t i = 0;
    while (listfile->At(i)) {
      TString libname = listfile->At(i++)->GetName();
      if (libname.Contains(LOGON_LIB_EXTENSION) && libname.Contains("Data") && !libname.Contains("libVDetector"+LOGON_LIB_EXTENSION)) {
        TString lib = libpath + "/" + libname;
        gSystem->Load(lib);
      }
    }

    // Loop on Physics Library
    i = 0;
    while (listfile->At(i)) {
      TString libname = listfile->At(i++)->GetName();
      if (libname.Contains(LOGON_LIB_EXTENSION) && libname.Contains("Physics") &&!libname.Contains("libVDetector"+LOGON_LIB_EXTENSION)) {
        TString lib = libpath + "/" + libname;
        gSystem->Load(lib);
      }
    }

    // Loop on the Reset of the Library
    i = 0;
    while (listfile->At(i)) {
      TString libname = listfile->At(i++)->GetName();
      if (libname.Contains(LOGON_LIB_EXTENSION) && !libname.Contains("Physics") && !libname.Contains("Data")  &&!libname.Contains("libVDetector"+LOGON_LIB_EXTENSION)) {
        TString lib = libpath + "/" + libname;
        gSystem->Load(lib);
      }
    }


    // gROOT->ProcessLine(".L $NPTOOL/NPLib/include/RootInput.h+");   

    // Since the libdir.GetListOfFiles() commands cds to the
    // libidr directory, one has to return to the initial
    // directory
    gSystem->cd(currentpath);
  }
}

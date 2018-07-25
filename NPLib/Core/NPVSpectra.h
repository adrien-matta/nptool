#ifndef VSPECTRA_H
#define VSPECTRA_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: A. Matta       contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : may 2014                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Virtual class for the TDetectorSpectra classes                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// ROOT headers
#include "TObject.h"
#include "TH1.h"
#include "TCanvas.h"
// C++ STL headers
#include <map>
#include <vector>
#include <string>

class VSpectra {
  public:
    // constructor and destructor
    VSpectra();
    virtual ~VSpectra(){};

  public:
    // Instantiate and register histo to std::maps
    TH1* AddHisto1D(std::string name, std::string title, Int_t nbinsx, Double_t xlow, Double_t xup, std::string family);
    TH1* AddHisto2D(std::string name, std::string title, Int_t nbinsx, Double_t xlow, Double_t xup, 
        Int_t nbinsy, Double_t ylow, Double_t yup, std::string family);

  public:
    // Initialization methods
    virtual void InitRawSpectra()        {};
    virtual void InitPreTreatedSpectra() {};
    virtual void InitPhysicsSpectra()    {};

  public:
    // Filling methods
    virtual void FillRawSpectra(void*) {};
    virtual void FillPreTreatedSpectra(void*) {};
    virtual void FillPhysicsSpectra(void*) {}; 
    virtual void CheckSpectra(){};

  public:
    // get std::map histo which will be used for GSpectra in GUser
    std::map< std::string, TH1* > GetMapHisto() const {return fMapHisto;}
    TH1* GetSpectra(const std::string& family, const std::string& name);    
    TH1* GetSpectra(const std::string& FamilyAndName);    
    // TEMP FIX
//    TH1* GetHisto(const std::string& family, const std::string& name){return GetSpectra(family,name);};    
//    TH1* GetHisto(const std::string& FamilyAndName){return GetSpectra(FamilyAndName);};    

    void FillSpectra(const std::string& family, const std::string& name, double val);
    void FillSpectra(const std::string& family, const std::string& name, double x, double y);
    void FillSpectra(const std::string& familyAndname, double val);
    void FillSpectra(const std::string& familyAndname, double x, double y);


    void WriteSpectra(std::string filename = "VOID");      
     
  protected:
    // std::map holding histo pointers and their family names
    std::map< std::string, TH1* > fMapHisto;
    
  private: // Name of the Detector
   std::string m_name;

  public:
   inline void SetName(std::string name) {m_name=name;}
   inline std::string GetName() {return m_name;}
};

#endif

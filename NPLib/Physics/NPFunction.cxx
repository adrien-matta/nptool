/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: matta@ipno.in2p3.fr   *
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

#include "NPFunction.h"
#include<sstream>
using namespace NPL;

// ROOT
#include "TDirectory.h"
namespace NPL{

// Check the type of Filename (root or ASCII) and extract build/extract a 1D histogramm
TH1F* Read1DProfile(string filename,string HistName) 
{  
  ifstream ASCII;
  TFile ROOT;
  TH1F* h;
  
  // test whether file format is ASCII or ROOT
  bool type = OpenASCIIorROOTFile(filename, ASCII , ROOT);
  
  // ASCII File case
  if (type) {
    string LineBuffer;
    
    // storing vector
    vector <double> x, w;
    
    // variable buffer
    double xb, wb;
    
    // Read the file
/*    while(!ASCII.eof()){
      getline(ASCII,LineBuffer);
      stringstream LineStream(LineBuffer);
      // ignore comment lines
      if (LineBuffer.compare(0,1,"%")!=0 && 
          LineBuffer.compare(0,1,"#")!=0 &&
          LineBuffer.compare(0,1,"@")!=0) {
        LineStream >> xb >> wb ;
        cout << xb << "\t" << wb << endl;
        x.push_back(xb);
        w.push_back(wb);
      }
    }
*/
    // read the file
    Double_t xmin =  200;
    Double_t xmax = -200;
    Double_t size = 0;
    while (getline(ASCII, LineBuffer)) {
       stringstream iss(LineBuffer);
       if (!(iss >> xb >> wb)) {continue;}   // skip comment lines 
       // fill vectors
       x.push_back(xb);
       w.push_back(wb);
       // compute xmin / xmax / size of x array
       if (xb > xmax) xmax = xb;
       if (xb < xmin) xmin = xb;
       size++;
    }

    // fill histo
    h = new TH1F(HistName.c_str(), HistName.c_str(), size, xmin, xmax);
    for (unsigned int i = 0; i < size; i++) {
      int bin = h->FindBin(x[i]);
      h->SetBinContent(bin,w[i]);
    }
  }
  
  // ROOT File case
  else{
    h = (TH1F*) gDirectory->FindObjectAny(HistName.c_str());
    if(!h){
      cout << "Error: Histogramm " << HistName << " not found in file " << filename << endl;
      exit(1);
    }
  }
  
  return h;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Check the type of Filename (root or ASCII) and extract build/extract a 2D histogramm
TH2F* Read2DProfile(string filename,string HistName){
  
  ifstream ASCII;
  TFile ROOT;
  TH2F* h;
  
  bool type = OpenASCIIorROOTFile(filename, ASCII , ROOT);
  
  
  // ASCII File case
  if(type){
    string LineBuffer;
    
    // storing vector
    vector <double> x,y,w;
    
    // variable buffer
    double xb,yb,wb;
    
    // Read the file
    while(!ASCII.eof()){
      getline(ASCII,LineBuffer);
      stringstream LineStream(LineBuffer);
      // ignore comment line
      if (LineBuffer.compare(0,1,"%")!=0){
        LineStream >> xb >> yb >> wb ;
        x.push_back(xb);
        y.push_back(yb);
        w.push_back(wb);
      }
    }
    
    // Look for the step size, min and max of the distribution
    double xmin = 0;
    double xmax = 0;
    unsigned int xsize = x.size();
    
    double ymin = 0;
    double ymax = 0;
    unsigned int ysize = y.size();
    
    if(xsize > 0){
      xmin = x[0] ;
      xmax = x[0] ;
    }
    
    if(ysize > 0){
      ymin = y[0] ;
      ymax = y[0] ;
    }
    
    for(unsigned int i = 0 ; i < xsize ; i++){
      if(x[i] > xmax) xmax = x[i] ;
      if(x[i] < xmin) xmin = x[i] ;
    }
    
    for(unsigned int i = 0 ; i < ysize ; i++){
      if(y[i] > ymax) ymax = y[i] ;
      if(y[i] < ymin) ymin = y[i] ;
    }
    
    h = new TH2F(HistName.c_str(),HistName.c_str(),xsize,xmin,xmax,ysize,ymin,ymax);
    
    for(unsigned int i = 0 ; i < xsize ; i++){
      int bin = h->FindBin(x[i],y[i]);
      h->SetBinContent(bin,w[i]);
    }
  }
  
  // ROOT File case
  else{
    h = (TH2F*) gDirectory->FindObjectAny(HistName.c_str());
    if(!h){
      cout << "Error: Histogramm " << HistName << " not found in file " << filename << endl;
      exit(1);
    }
  }
  return h;
}

// Open a file at Filename after checking the type of file it is
// true for a ASCII file
// False for a Root file
bool OpenASCIIorROOTFile(string filename, ifstream &ASCII , TFile &ROOT){
  
  // look for .root extension
  size_t pos = filename.find(".root");
  
  string GlobalPath = getenv("NPTOOL");
  string StandardPath = GlobalPath + "/Inputs/CrossSection/" + filename;

  // extension not found, file is assume to be a ASCII file
  if(pos > filename.size()) {
    ASCII.open(filename.c_str());
    
    if(!ASCII.is_open()){
      ASCII.open(StandardPath.c_str());
      if(!ASCII.is_open()){
        cout << "Error, file " << filename << " not found " << endl ;
        exit(1);
      }
    }
    
    return true;
  }
  
  else {
    if(ROOT.Open(filename.c_str(),"READ")){
      return false;
    }
  
    else{
      
      if(ROOT.Open(StandardPath.c_str(),"READ")){
        return false;
      }
      else{
        cout << "Error, file " << StandardPath << " not found " << endl ;
        exit(1);
      }
    }
    
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double &X, double &Y, double NumberOfSigma)
{
  if (SigmaX != 0 && SigmaY!=0) {
    X = 2 * NumberOfSigma*SigmaX;
    while (X > NumberOfSigma*SigmaX) X = gRandom->Gaus(MeanX, SigmaX);
    
    double a = NumberOfSigma * SigmaX/2;
    double b = NumberOfSigma * SigmaY/2;
    double SigmaYPrim = b * sqrt(1 - X*X / (a*a));
    
    SigmaYPrim = 2*SigmaYPrim / NumberOfSigma;
    Y = gRandom->Gaus(MeanY, SigmaYPrim);
  }
  
  else if(SigmaX == 0 && SigmaY!=0) {
    X = MeanX;
    Y = gRandom->Gaus(MeanY, SigmaY);
  }
  
  else if(SigmaX != 0 && SigmaY==0) {
    Y = MeanY;
    X = gRandom->Gaus(MeanX, SigmaX);
  }
  
  else {
    X = MeanX;
    Y = MeanY;
  }
}

}

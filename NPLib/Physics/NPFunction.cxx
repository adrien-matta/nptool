/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA contact address: matta@lpccaen.in2p3.fr   *
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
    
    // read the file
    Double_t xmin =  200;
    Double_t xmax = -200;
    int mysize = 0;
    while (getline(ASCII, LineBuffer)) {
       stringstream iss(LineBuffer);
       if (!(iss >> xb >> wb)) {continue;}   // skip comment lines 
       // fill vectors
       x.push_back(xb);
       w.push_back(wb);
//       cout << xb << "\t" << wb << endl;
       // compute xmin / xmax / size of x array
       if (xb > xmax) xmax = xb;
       if (xb < xmin) xmin = xb;
       mysize++;
    }
    Double_t dx = (xmax - xmin) / (mysize);
//    cout << xmin << "\t" << xmax << "\t" << mysize << "\t" << dx << endl;

    // fill histo
    h = new TH1F(HistName.c_str(), HistName.c_str(), mysize, xmin, xmax+dx);
    for (unsigned int i = 0; i < mysize; i++) {
      int bin = h->FindBin(x[i]);
      h->SetBinContent(bin,w[i]);
//      cout << i << "\t" << x[i] << "\t" << bin << "\t" << w[i] << "\t" << h->GetBinContent(bin) << endl;
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


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
string ChangeNameToG4Standard(string OriginalName){
  string NumberOfMass ;
  string Nucleid;
  
  for (unsigned int i = 0; i < OriginalName.length(); i++) {
    ostringstream character;
    character << OriginalName[i];
    if      (character.str()=="0") NumberOfMass+="0";
    else if (character.str()=="1") NumberOfMass+="1";
    else if (character.str()=="2") NumberOfMass+="2";
    else if (character.str()=="3") NumberOfMass+="3";
    else if (character.str()=="4") NumberOfMass+="4";
    else if (character.str()=="5") NumberOfMass+="5";
    else if (character.str()=="6") NumberOfMass+="6";
    else if (character.str()=="7") NumberOfMass+="7";
    else if (character.str()=="8") NumberOfMass+="8";
    else if (character.str()=="9") NumberOfMass+="9";
    
    else if (character.str()=="A") Nucleid+="A";
    else if (character.str()=="B") Nucleid+="B";
    else if (character.str()=="C") Nucleid+="C";
    else if (character.str()=="D") Nucleid+="D";
    else if (character.str()=="E") Nucleid+="E";
    else if (character.str()=="F") Nucleid+="F";
    else if (character.str()=="G") Nucleid+="G";
    else if (character.str()=="H") Nucleid+="H";
    else if (character.str()=="I") Nucleid+="I";
    else if (character.str()=="J") Nucleid+="J";
    else if (character.str()=="K") Nucleid+="K";
    else if (character.str()=="L") Nucleid+="L";
    else if (character.str()=="M") Nucleid+="M";
    else if (character.str()=="N") Nucleid+="N";
    else if (character.str()=="O") Nucleid+="O";
    else if (character.str()=="P") Nucleid+="P";
    else if (character.str()=="Q") Nucleid+="Q";
    else if (character.str()=="R") Nucleid+="R";
    else if (character.str()=="S") Nucleid+="S";
    else if (character.str()=="T") Nucleid+="T";
    else if (character.str()=="U") Nucleid+="U";
    else if (character.str()=="V") Nucleid+="V";
    else if (character.str()=="W") Nucleid+="W";
    else if (character.str()=="X") Nucleid+="X";
    else if (character.str()=="Y") Nucleid+="Y";
    else if (character.str()=="Z") Nucleid+="Z";
    
    else if (character.str()=="a") Nucleid+="a";
    else if (character.str()=="b") Nucleid+="b";
    else if (character.str()=="c") Nucleid+="c";
    else if (character.str()=="d") Nucleid+="d";
    else if (character.str()=="e") Nucleid+="e";
    else if (character.str()=="f") Nucleid+="f";
    else if (character.str()=="g") Nucleid+="g";
    else if (character.str()=="h") Nucleid+="h";
    else if (character.str()=="i") Nucleid+="i";
    else if (character.str()=="j") Nucleid+="j";
    else if (character.str()=="k") Nucleid+="k";
    else if (character.str()=="l") Nucleid+="l";
    else if (character.str()=="m") Nucleid+="m";
    else if (character.str()=="n") Nucleid+="n";
    else if (character.str()=="o") Nucleid+="o";
    else if (character.str()=="p") Nucleid+="p";
    else if (character.str()=="q") Nucleid+="q";
    else if (character.str()=="r") Nucleid+="r";
    else if (character.str()=="s") Nucleid+="s";
    else if (character.str()=="t") Nucleid+="t";
    else if (character.str()=="u") Nucleid+="u";
    else if (character.str()=="v") Nucleid+="v";
    else if (character.str()=="w") Nucleid+="w";
    else if (character.str()=="x") Nucleid+="x";
    else if (character.str()=="y") Nucleid+="y";
    else if (character.str()=="z") Nucleid+="z";
  }
  
  // Special case for light particles
  string FinalName=Nucleid+NumberOfMass;
  if      (FinalName=="H1")       FinalName="proton";
  else if (FinalName=="H2")       FinalName="deuteron";
  else if (FinalName=="H3")       FinalName="triton";
  else if (FinalName=="He4")      FinalName="alpha";
  else if (FinalName=="p")        FinalName="proton";
  else if (FinalName=="d")        FinalName="deuteron";
  else if (FinalName=="t")        FinalName="triton";
  else if (FinalName=="a")        FinalName="alpha";
  else if (FinalName=="proton")   FinalName="proton";
  else if (FinalName=="deuteron") FinalName="deuteron";
  else if (FinalName=="triton")   FinalName="triton";
  else if (FinalName=="alpha")    FinalName="alpha";
  else if (FinalName=="n")        FinalName="neutron";
  else if (FinalName=="neutron")  FinalName="neutron";
  return(FinalName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
string ChangeNameFromG4Standard(string OriginalName){
  string NumberOfMass ;
  string Nucleid;
  
  for (unsigned int i = 0; i < OriginalName.length(); i++) {
    ostringstream character;
    character << OriginalName[i];
    if      (character.str()=="0") NumberOfMass+="0";
    else if (character.str()=="1") NumberOfMass+="1";
    else if (character.str()=="2") NumberOfMass+="2";
    else if (character.str()=="3") NumberOfMass+="3";
    else if (character.str()=="4") NumberOfMass+="4";
    else if (character.str()=="5") NumberOfMass+="5";
    else if (character.str()=="6") NumberOfMass+="6";
    else if (character.str()=="7") NumberOfMass+="7";
    else if (character.str()=="8") NumberOfMass+="8";
    else if (character.str()=="9") NumberOfMass+="9";
    
    else if (character.str()=="A") Nucleid+="A";
    else if (character.str()=="B") Nucleid+="B";
    else if (character.str()=="C") Nucleid+="C";
    else if (character.str()=="D") Nucleid+="D";
    else if (character.str()=="E") Nucleid+="E";
    else if (character.str()=="F") Nucleid+="F";
    else if (character.str()=="G") Nucleid+="G";
    else if (character.str()=="H") Nucleid+="H";
    else if (character.str()=="I") Nucleid+="I";
    else if (character.str()=="J") Nucleid+="J";
    else if (character.str()=="K") Nucleid+="K";
    else if (character.str()=="L") Nucleid+="L";
    else if (character.str()=="M") Nucleid+="M";
    else if (character.str()=="N") Nucleid+="N";
    else if (character.str()=="O") Nucleid+="O";
    else if (character.str()=="P") Nucleid+="P";
    else if (character.str()=="Q") Nucleid+="Q";
    else if (character.str()=="R") Nucleid+="R";
    else if (character.str()=="S") Nucleid+="S";
    else if (character.str()=="T") Nucleid+="T";
    else if (character.str()=="U") Nucleid+="U";
    else if (character.str()=="V") Nucleid+="V";
    else if (character.str()=="W") Nucleid+="W";
    else if (character.str()=="X") Nucleid+="X";
    else if (character.str()=="Y") Nucleid+="Y";
    else if (character.str()=="Z") Nucleid+="Z";
    
    else if (character.str()=="a") Nucleid+="a";
    else if (character.str()=="b") Nucleid+="b";
    else if (character.str()=="c") Nucleid+="c";
    else if (character.str()=="d") Nucleid+="d";
    else if (character.str()=="e") Nucleid+="e";
    else if (character.str()=="f") Nucleid+="f";
    else if (character.str()=="g") Nucleid+="g";
    else if (character.str()=="h") Nucleid+="h";
    else if (character.str()=="i") Nucleid+="i";
    else if (character.str()=="j") Nucleid+="j";
    else if (character.str()=="k") Nucleid+="k";
    else if (character.str()=="l") Nucleid+="l";
    else if (character.str()=="m") Nucleid+="m";
    else if (character.str()=="n") Nucleid+="n";
    else if (character.str()=="o") Nucleid+="o";
    else if (character.str()=="p") Nucleid+="p";
    else if (character.str()=="q") Nucleid+="q";
    else if (character.str()=="r") Nucleid+="r";
    else if (character.str()=="s") Nucleid+="s";
    else if (character.str()=="t") Nucleid+="t";
    else if (character.str()=="u") Nucleid+="u";
    else if (character.str()=="v") Nucleid+="v";
    else if (character.str()=="w") Nucleid+="w";
    else if (character.str()=="x") Nucleid+="x";
    else if (character.str()=="y") Nucleid+="y";
    else if (character.str()=="z") Nucleid+="z";
  }
  
  // Special case for light particles
  string FinalName=NumberOfMass+Nucleid;
  if      (FinalName=="H1")       FinalName="proton";
  else if (FinalName=="H2")       FinalName="deuteron";
  else if (FinalName=="H3")       FinalName="triton";
  else if (FinalName=="He4")      FinalName="alpha";
  else if (FinalName=="p")        FinalName="proton";
  else if (FinalName=="d")        FinalName="deuteron";
  else if (FinalName=="t")        FinalName="triton";
  else if (FinalName=="a")        FinalName="alpha";
  else if (FinalName=="proton")   FinalName="proton";
  else if (FinalName=="deuteron") FinalName="deuteron";
  else if (FinalName=="triton")   FinalName="triton";
  else if (FinalName=="alpha")    FinalName="alpha";
  else if (FinalName=="n")        FinalName="neutron";
  else if (FinalName=="neutron")  FinalName="neutron";
  return(FinalName);
}
}

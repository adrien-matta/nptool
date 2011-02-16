// STL
#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

// Root
#include "TString.h"
#include "TSpectrum.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TLine.h"

//NPTool
#include "NPEnergyLoss.h"
using namespace NPL;



/// DEFINING GLOBAL VARIABLE
double mean_extrapolation=100;

int Detector_Number;
const int Strip_Start=1;
const int Strip_End=16;
/// Parameter used in the macro
Double_t Angle[16] = {51.75,52.83,53.86,55.77,56.65,57.50,58.33,59.10,59.83,60.54,61.23,61.88,62.51,63.11,63.70};
//Double_t Angle[16] = {63.70,63.11,62.51,61.88,61.23,60.54,59.83,59.10,58.33,57.50,56.65,55.77,53.86,52.83,51.75};
EnergyLoss EL_Al("./EnergyLossTable/alpha_Al.G4table" , "G4Table", 100) ;
EnergyLoss EL_Si("./EnergyLossTable/alpha_Si.G4table" , "G4Table", 100) ;

Double_t AlThickness ;
Double_t SiThickness ;  

// Pedestal
Int_t PedestalRange_min = 0 ; Int_t PedestalRange_max = 1024 ;
  
// Alpha
Int_t AlphaRange_min = 1000 ; Int_t AlphaRange_max = 8000 ;
  
// Finder Method
Double_t resolsig=5;
Float_t  resolsigTSpec=5;
Double_t seuil=0.15;
Int_t    npeaks=5;   // nombre de pics maximum a prendre

// Rebin for low stat case, use 1 to disable
Int_t BinDivision=8 ;

// Information about the calibration condition (use Latex marks-up)
const TString Experiment          = "RRC66";
const TString Run_Period          = "June 2010, Riken, run 87-88";
const TString Operator            = "Adrien MATTA";
const TString Source              = "3 alpha peaks $^{239}$Pu, $^{241}$Am, $^{244}$Cm";
const TString Comment             = "Source at 0$^{\\circ}$ facing Detector 1,2,3,4, below -12.5cm";
const char* frun = "run_87-88";

// choosing a method for the fit
const TString method = "ZeroExtrapolation" ;
const bool    RefitWithSatellite = false ;
Int_t CurrentDetector = 0 ;
Int_t CurrentStrip     = 0 ;
TString folder;
TString main_name;
TCanvas* Tsummary;
TCanvas* Buffer;

map<int,string> BadStrip;

// Defining the array used after (order needs to be diffent for X and Y )
Int_t NumberOfIsotope;

// Source original value
Int_t Source_Number_Peak;
TString*  Source_isotope;
Double_t* Source_branching_ratio;
Double_t* Source_E;
Double_t* Source_Sig;

// Source corrected value
Double_t* energy;
Double_t* errors;

// Calibration Coefficient
Double_t a ;
Double_t b ;

Double_t* mean      = new Double_t[3]; 
Double_t* sigma     = new Double_t[3];
Double_t* error_par = new Double_t[3];

TGraph ZeroDispersion = TGraph(16);
ofstream peaks_file, calib_file, dispersion_file , calib_online_file, latex_file;; 

TH1F* sigma_fit  ;
TH1F* Dispersion ;
TGraph coeff_a = TGraph(16);
TGraph coeff_b = TGraph(16);

TFile *inFile;

/// Function Header
void AutoCalibration(int,int);
void EnergyCalibrator();

Double_t Pedestals(TH1F *);
void Alpha(TH1F*, Double_t);
bool Finder(TH1F* , Double_t*, Double_t*);
Double_t Calib_ZeroForceMethod(TGraphErrors*,float, Double_t*, Double_t*);
Double_t Calib_ZeroExtrapolationMethod(TH1F* hist,TGraphErrors*,float, Double_t*, Double_t*, Double_t &a , Double_t &b);
void LatexSummaryHeader();
void LatexSummaryEnder();
void LatexSummaryDetector();
void DefineSource(TString sourceName="3 alphas");


void Find_Satellites(TH1F *h);
Double_t source_Pu(Double_t *x, Double_t *par);
Double_t source_Am(Double_t *x, Double_t *par);
Double_t source_Cm(Double_t *x, Double_t *par);


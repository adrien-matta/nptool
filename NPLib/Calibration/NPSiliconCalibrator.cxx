// NPL
#include "NPSiliconCalibrator.h"

#include <algorithm>
using namespace std;

// Root
#include "TSpectrum.h"
#include "TH1.h"

namespace {
#if ROOT_VERSION_CODE > ROOT_VERSION(6,0,0)
  typedef Double_t* TSpectrumPosition_t;
#else
  typedef Float_t* TSpectrumPosition_t;
#endif
}

NPL::SiliconCalibrator::SiliconCalibrator(){
}

//////////////////////////////////////////////
NPL::SiliconCalibrator::~SiliconCalibrator(){
}

//////////////////////////////////////////////
double NPL::SiliconCalibrator::ZeroExtrapolation(TH1* histo, NPL::CalibrationSource* CS, NPL::EnergyLoss* EL, vector<double>& coeff, unsigned int pedestal, unsigned int max_iteration,double rmin,double rmax){
  if(histo->GetEntries()==0){
    coeff.clear();
    coeff.push_back(0);
    coeff.push_back(-1);
    return -1;
  }

  int counts=0;
  if(rmin == -1 && rmax == -1){
    rmin = histo->GetBinCenter(1);
    rmax = histo->GetBinCenter(histo->GetNbinsX()-1);
  }

  else{
    for(unsigned int i = 1 ; i < histo->GetNbinsX() ; i++){ 
      if(histo->GetBinCenter(i) < rmin || histo->GetBinCenter(i) > rmax)
        histo->SetBinContent(i,0);
      else
        counts+= histo->GetBinContent(i);
    }
  }

  if(counts == 0){
    coeff.clear();
    coeff.push_back(0);
    coeff.push_back(-1);
    return -2;
  }

  m_CalibrationSource= CS;
  m_EL_Al= EL;

  double DistanceToPedestal = 100000;
  double Step = 0.01*micrometer;
  bool   LastStepIncrease = false;
  bool   LastStepDecrease = false;

  // 1% precision on the total scale (typically 6 MeV ) 
  double my_precision = (rmax-pedestal)*0.01;
  // Energy of the Source and sigma on the value (MeV)
  double Assume_Thickness = 0 ; // micrometer
  vector<double> Assume_E; // Energie calculated assuming Assume_Thickness deadlayer of Al

  unsigned int k= 0;

  double* Source_E = new double[CS->GetEnergies().size()];
  double* Source_Sig = new double[CS->GetEnergies().size()];

  unsigned int source_size =  CS->GetEnergies().size() ;
  for(unsigned int i = 0 ; i < source_size ; i++){
    Source_E[i] = CS->GetEnergies()[i][0];
    Source_Sig[i] =  CS->GetEnergiesErrors()[i][0]; 
  }
  Assume_E.resize(source_size,0);
  TGraphErrors* g = FitSpectrum(histo,rmin,rmax);
  if (g->GetN()!=3)
    return -1;

  while(k++<max_iteration && abs(Assume_Thickness) < 100*micrometer){

    // Compute the new assumed energies
    double* x = g->GetX();
    for(unsigned int i = 0 ; i < source_size ; i++){
      Assume_E[i] = EL->Slow(Source_E[i] , Assume_Thickness, 0);
      g->SetPoint(i,x[i] ,Assume_E[i]);
    }

    DistanceToPedestal = FitPoints(g,&Assume_E[0] , Source_Sig, coeff, 0 );
    if(abs(DistanceToPedestal) < my_precision || Step < 0.01*micrometer)
      break;

    else if(DistanceToPedestal > 0 ){
      if(LastStepIncrease)
        Step *= 10.;

      Assume_Thickness += Step;
      LastStepIncrease = true;
    }

    else if(DistanceToPedestal < 0 ){
      if(LastStepDecrease)
        Step *= 10.;

      Assume_Thickness -= Step;
      LastStepDecrease = true;
    }

    // In this case the step size need to be changed
    if(LastStepIncrease && LastStepDecrease){
      Step *= 0.1;
      LastStepIncrease = false ;
      LastStepDecrease = false ;
    }

  }

  if(abs(DistanceToPedestal) > my_precision && Step > 0.01*micrometer){
    cout << "Zero extrapolation failed on : histo " << histo->GetName() << ". Final value is:" << Assume_Thickness / micrometer << "um with step "<< Step/micrometer << ", Dst to Pedestal = " << abs(DistanceToPedestal) << endl;
    return -1000;
  }

  else if(Assume_Thickness<0){
    DistanceToPedestal = FitPoints(g,Source_E , Source_Sig, coeff, 0 );
    return 0;
  }
  delete g;
  return Assume_Thickness/micrometer;
}
////////////////////////////////////////////////////////////////////////////////
double NPL::SiliconCalibrator::SimpleCalibration(TH1* histo, NPL::CalibrationSource* CS, NPL::EnergyLoss* EL, vector<double>& coeff, double rmin,double rmax){
  if(histo->GetEntries()==0){
    coeff.clear();
    coeff.push_back(0);
    coeff.push_back(-1);
    return -1;
  }

  int counts=0;
  if(rmin == -1 && rmax == -1){
    rmin = histo->GetBinCenter(1);
    rmax = histo->GetBinCenter(histo->GetNbinsX()-1);
  }

  else{
    for(unsigned int i = 1 ; i < histo->GetNbinsX() ; i++){ 
      if(histo->GetBinCenter(i) < rmin || histo->GetBinCenter(i) > rmax)
        histo->SetBinContent(i,0);
      else
        counts+= histo->GetBinContent(i);
    }
  }

  if(counts < 30){
    coeff.clear();
    coeff.push_back(0);
    coeff.push_back(-1);
    return -2;
  }

  m_CalibrationSource= CS;
  m_EL_Al= EL;

  double* Source_E = new double[CS->GetEnergies().size()];
  double* Source_Sig = new double[CS->GetEnergies().size()];

  unsigned int source_size =  CS->GetEnergies().size() ;
  for(unsigned int i = 0 ; i < source_size ; i++){
    Source_E[i] = CS->GetEnergies()[i][0];
    Source_Sig[i] = CS->GetEnergiesErrors()[i][0]; 
  }
  TGraphErrors* g = FitSpectrum(histo,rmin,rmax);
  double dist = FitPoints(g,Source_E , Source_Sig, coeff, 0 );

  delete g;
  if(dist!=-3)
    return abs(dist);
  else 
    return dist;
}


//////////////////////////////////////////////
double NPL::SiliconCalibrator::FitPoints(TGraphErrors* gre, double* Energies, double* ErrEnergies, vector<double>& coeff, double pedestal)
{
   if (gre->GetN() > 0) {
      gre->Fit("pol1","Q");
      coeff.clear();
      coeff.push_back(gre->GetFunction("pol1")->GetParameter(0));
      coeff.push_back(gre->GetFunction("pol1")->GetParameter(1));
      // Compute the Distance to pedestal:
      return (coeff[0]/coeff[1]-pedestal);
   }
   else {
      coeff.clear();
      coeff.push_back(0);
      coeff.push_back(-1);
      return -3;
  }
}



////////////////////////////////////////////////////////////////////////////////
TGraphErrors* NPL::SiliconCalibrator::FitSpectrum(TH1* histo, double rmin, double rmax)
{
//   for(unsigned int i = 0 ; i < histo->GetNbinsX() ; i++)
//      if(histo->GetBinCenter(i)<rmin || histo->GetBinCenter(i)>rmax )
//         histo->SetBinContent(i,0);

   // apply range for peak search
   histo->GetXaxis()->SetRangeUser(rmin, rmax);
   // Perform a peak search to get a hint of where are the peaks
   TSpectrum sp(4,1);
   //  nfound = sp->Search(histo,3,"",0.25);
   Int_t    nfound = sp.Search(histo,3,"",0.5);
   TSpectrumPosition_t xpeaks = sp.GetPositionX();

   // order list of peaks
   sort(xpeaks, xpeaks+nfound);

   m_FitFunction =  m_CalibrationSource->GetSourceSignal();
   if (nfound == 3) {
      // Set initial mean 
      m_FitFunction->SetParameter(1,xpeaks[0]);
      m_FitFunction->SetParameter(4,xpeaks[1]);
      m_FitFunction->SetParameter(7,xpeaks[2]);

      // Set Limit
      m_FitFunction->SetParLimits(1,xpeaks[0]*0.8,xpeaks[0]*1.2);
      m_FitFunction->SetParLimits(4,xpeaks[1]*0.8,xpeaks[1]*1.2);
      m_FitFunction->SetParLimits(7,xpeaks[2]*0.8,xpeaks[2]*1.2);


      // Set initial hight
      double H1 = histo->GetBinContent(histo->FindBin(xpeaks[0])); 
      double H2 = histo->GetBinContent(histo->FindBin(xpeaks[1])); 
      double H3 = histo->GetBinContent(histo->FindBin(xpeaks[2])); 

      m_FitFunction->SetParameter(0,H1);
      m_FitFunction->SetParameter(3,H2);
      m_FitFunction->SetParameter(6,H3);

      // Set Limit
      m_FitFunction->SetParLimits(0,H1*0.8,H1*1.2);
      m_FitFunction->SetParLimits(3,H2*0.8,H2*1.2);
      m_FitFunction->SetParLimits(6,H3*0.8,H3*1.2);

      // ballpark the sigma
      double sigma = (xpeaks[1]-xpeaks[0])/100;
      m_FitFunction->SetParameter(2,sigma);
      m_FitFunction->SetParameter(5,sigma);
      m_FitFunction->SetParameter(8,sigma);

      // Set range and fit spectrum
      histo->GetXaxis()->SetRangeUser(xpeaks[0]-xpeaks[0]/20.,xpeaks[2]+xpeaks[2]/20.);
      histo->Fit(m_FitFunction,"Q");

      TF1* fit = histo->GetFunction(m_FitFunction->GetName());
      // energies and associated uncertainties 
      vector< vector<double> > Energies    = m_CalibrationSource->GetEnergies();
      vector< vector<double> > ErrEnergies = m_CalibrationSource->GetEnergiesErrors();

      //    unsigned int mysize = Energies.size();
      TGraphErrors* gre = new TGraphErrors();
      int point = 0 ;
      for (unsigned int i = 0; i < Energies.size(); i++) {
         gre->SetPoint(point, fit->GetParameter(3*i+1), Energies[i][0]);
         gre->SetPointError(point++, fit->GetParError(3*i+1), ErrEnergies[i][0]);
      }
      return gre;
   }

   else
      return (new TGraphErrors());
}



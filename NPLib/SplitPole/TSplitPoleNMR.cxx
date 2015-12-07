/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : april 2012                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class reads the rmn file and provides some simple        *
 *             operations: apply delay, calculate mean value for subset of   *
 *             the data, ....                                                *
 *             Operations support both absolute and relative time            *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + Based on cSplitRun class from Iulian Stefan                          *
 *                                                                           *
 *****************************************************************************/

// class header
#include "TSplitPoleNMR.h"

// ROOT header
#include "TMath.h"

// C++ header
#include <sstream>
using namespace std;

//#define DEBUG


ClassImp(TSplitPoleNMR)


TSplitPoleNMR::TSplitPoleNMR()
   : fOpenFileTime(0),
     fDelay(6797),
     fFileName("Data_run.dat"),
     fRmnRelativeTime(new TGraph()),
     fRmn(new TGraph()),
     fIsRelativeTime(0),
     fIsLargeField(0),
     fMean(-1),
     fMin(10),
     fMax(-1)
{
}



TSplitPoleNMR::TSplitPoleNMR(const char* fileName)
   : fOpenFileTime(0),
     fDelay(6797),
     fFileName(fileName),
     fRmnRelativeTime(new TGraph()),
     fRmn(new TGraph()),
     fIsRelativeTime(0),
     fIsLargeField(0),
     fMean(-1),
     fMin(10),
     fMax(-1)
{
   ReadRmnFile();
}



TSplitPoleNMR::TSplitPoleNMR(const char* fileName, Double_t delay)
   : fOpenFileTime(0),
     fDelay(delay),
     fFileName(fileName),
     fRmnRelativeTime(new TGraph()),
     fRmn(new TGraph()),
     fIsRelativeTime(0),
     fIsLargeField(0),
     fMean(-1),
     fMin(10),
     fMax(-1)
{
   ReadRmnFile();
}



TSplitPoleNMR::~TSplitPoleNMR()
{
  if (fRmnRelativeTime) delete fRmnRelativeTime;
  if (fRmn)             delete fRmn;
}




Int_t TSplitPoleNMR::ReadRmnFile()
{
  ifstream in_rmn;
  Double_t old_x=-10.;
  std::cout<<"\nReading Rmn data file: "<<fFileName<<"..."<<std::flush;
//  DeleteRmnGraph();
//  fRmn= new TGraph();
  in_rmn.open(fFileName);

  if(!in_rmn.is_open()){
    std::cout<<"\n Cant open Rmn data file "<<fFileName<<" ...\n"<<std::flush;
    std::cout<<"\n Nor fRmnFile "<<fFileName<<" ...\n"<<std::flush;
    
    return -1;
  }
  
  char tmp[1000];
  in_rmn.getline(tmp,sizeof(tmp),'\n');
  //  TTimeStamp a;
  in_rmn.getline(tmp,sizeof(tmp),':');
  Int_t year,month,day,hour,minutes,seconds;
  
  char ch_tmp;
  in_rmn>>year;in_rmn>>ch_tmp;in_rmn>>month;in_rmn>>ch_tmp;in_rmn>>day;
  in_rmn.getline(tmp,sizeof(tmp),'-');
  in_rmn>>hour;in_rmn>>ch_tmp;in_rmn>>minutes;in_rmn>>ch_tmp;in_rmn>>seconds;
  
  fOpenFileTime.Set(year,month,day,hour,minutes,seconds, 0, 1, 0);
  fOpenFileTime.Print();
  Double_t time=fOpenFileTime.AsDouble(),x=1.,y;
  Int_t i=0;

  Double_t ymin = 10;
  Double_t ymax = -1;
  Double_t ymean = 0;

#ifdef DEBUG
//  std::cout << "time 0 = " << time << std::endl;
//  std::cout<<"\nx="<<x<<"\ty="<<y<<std::flush;
#endif

  in_rmn.getline(tmp,sizeof(tmp),'\n');
  in_rmn.getline(tmp,sizeof(tmp),'\n');
  for(;;){
    in_rmn>>x;in_rmn>>ch_tmp;in_rmn>>y;
    y*=1.e-7;
    // apply large field
    if (fIsLargeField) y += 1;
#ifdef DEBUG
//  std::cout<<"\nx="<<x<<"\ty="<<y<<std::flush;
#endif
    if(x > old_x) {
       // TGraph
       fRmnRelativeTime -> SetPoint(i,   x,             y);
       fRmn             -> SetPoint(i++, x+time+fDelay, y);
#ifdef DEBUG
//  std::cout<<"\nx="<<x+time+fDelay<<"\ty="<<y<<std::flush;
#endif
       // min, max
       if (y < ymin) ymin = y;
       if (y > ymax) ymax = y;
       // mean
       ymean += y;
    }
    else break;
    old_x=x;
    x=0.;
    
  }
  in_rmn.close();
  std::cout<<" Ok!"<<std::flush;
  std::cout << std::endl;

  // set mean, min and max field values
  fMin = ymin;
  fMax = ymax;
  fMean = ymean / fRmnRelativeTime->GetN();

  return 0;
}



void TSplitPoleNMR::SetIsRelativeTime(bool isRelativeTime)
{
   fIsRelativeTime = isRelativeTime;
}



void TSplitPoleNMR::SetLargeField(bool isLargeField)
{
   fIsLargeField = isLargeField;
   ReadRmnFile();
}



TGraph* TSplitPoleNMR::GetRmnGraph() const
{
   if (!fIsRelativeTime) return fRmn;
   else return fRmnRelativeTime;
}



void TSplitPoleNMR::Draw(const char* option) const
{
   if (fRmn->GetN() > 0) {
      if (!fIsRelativeTime) fRmn->Draw(option);
      else fRmnRelativeTime->Draw(option);
   }
}



void TSplitPoleNMR::ApplyDelay()
{
   if (!fRmnRelativeTime) {
      cout << "TGraph containing rmn data is missing" << endl;
   }
   else {
      Double_t timeRef = fOpenFileTime.AsDouble();
      Int_t   dim = fRmnRelativeTime->GetN();
      Double_t *x = fRmnRelativeTime->GetX();
      Double_t *y = fRmnRelativeTime->GetY();
      for (Int_t i = 0; i < dim; ++i) {   // loop on TGraph size
         fRmn->SetPoint(i, x[i] + fDelay + timeRef, y[i]);
      } // end loop on TGraph size
   }
}



void TSplitPoleNMR::Dump()
{
  std::cout<<"\nPrinting TSplitPoleNMR object "<<this<<"\n"<<std::flush;



  if(fRmn){
    std::cout<<"\nfRmn defined "<<fRmn->GetName();
    std::cout<<"\nfrom file "<<fFileName;
  }
  else
    std::cout<<"\nNo fRmn object";

}



Double_t TSplitPoleNMR::EvalB(Double_t Time) const
{
  if(!fRmn) return 0.;
  Double_t *x;
  x=fRmn->GetX();
  Int_t n= fRmn->GetN();

  if( Time>=x[0] && Time<=x[n-1])
    return fRmn->Eval(Time);
  else
    return fRmn->GetMean(2);

}



Double_t TSplitPoleNMR::Mean(Double_t t1, Double_t t2) const
{
   // get dimension and x array
   Int_t dim;
   Double_t *x, *y;
   if (fIsRelativeTime) {
      dim = fRmnRelativeTime->GetN();
      x   = fRmnRelativeTime->GetX();
      y   = fRmnRelativeTime->GetY();
   }
   else {
      dim = fRmn->GetN();
      x   = fRmn->GetX();
      y   = fRmn->GetY();
   }

   // find index for t1 and t2
   Double_t index1 = TMath::BinarySearch(dim, x, t1);
   Double_t index2 = TMath::BinarySearch(dim, x, t2);
   // if t1=0, BinarySearh returns index = -1, we force it to 0
   if (index1 < 0) index1 = 0;

   // calculate mean
   Double_t mean = 0;
   for (Int_t i = index1; i < index2+1; ++i) {   // loop on field TGraph
      mean += y[i];
   } // end loop on field TGraph


   return mean / (index2 - index1 + 1);
}

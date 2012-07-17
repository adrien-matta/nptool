/*****************************************************************************
 * Copyright (C) 2008   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Vandebrouck   contact address: vandebro@ipno.in2p3.fr *
 *                  J. Gibelin                      gibelin@lpccaen.in2p3.fr *
 *                                                                           *
 * Creation Date  : November 2011                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds a special 2D Maya histogram                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#ifndef __MAYAHISTO__
#define __MAYAHISTO__

#include "TObject.h"
#include "TROOT.h"
#include "TFile.h"
#include "TCutG.h"
#include "TH2F.h"
#include "TArrayF.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "string"
#include "TString.h"
#include "TStyle.h"
#include "TPad.h"
#include "TF1.h"
//#include "TGraph.h"
#include "TGraphErrors.h"
#include "TDirectory.h"
#include <iostream>
#include <vector>
#include <map>
#include "THistPainter.h"
#include "TMatrixFBase.h"
#include "TPaletteAxis.h"
#include "TVirtualHistPainter.h"
#include "TPolyMarker.h"
#include "TArrow.h"
#include "TSpline.h"
#include "TBuffer.h"
#include <math.h>

class TMayaHisto : public TH2F {

public:
   TMayaHisto();
   TMayaHisto(const char *name,const char *title,Int_t nbinsx=32, Int_t nbinsy=32,Double_t hsize=5);
   TMayaHisto(const TMayaHisto &h2f);

   virtual ~TMayaHisto();
   virtual void     AddBinContent(Int_t bin) {++fArray[bin];}
   virtual void     AddBinContent(Int_t bin, Double_t w)
                                 {fArray[bin] += Float_t (w);}
   virtual void     Copy(TObject &hnew) const;
   virtual TH1     *DrawCopy(Option_t *option="") const;
   virtual Double_t GetBinContent(Int_t bin) const;
   virtual Double_t GetBinContent(Int_t binx, Int_t biny) const {return GetBinContent(GetBin(binx,biny));}
   virtual Double_t GetBinContent(Int_t binx, Int_t biny, Int_t) const {return GetBinContent(GetBin(binx,biny));}
   virtual void     Reset(Option_t *option="");
   virtual void     SetBinContent(Int_t bin, Double_t content);
   virtual void     SetBinContent(Int_t binx, Int_t biny, Double_t content) {SetBinContent(GetBin(binx,biny),content);}
   virtual void     SetBinContent(Int_t binx, Int_t biny, Int_t, Double_t content) {SetBinContent(GetBin(binx,biny),content);}
   virtual void     SetBinsLength(Int_t n=-1);
   //   virtual TPolyLine* Hexagon(Float_t,Float_t);
   virtual TCutG* Hexagon(TString,Float_t,Float_t);
   virtual void CreateHoneyCombStructure() ;
//   virtual Int_t    Fill(Int_t C, Int_t R,Int_t W) { Int_t n = TH2::Fill(C,R,W); Draw(); return n ;};
   virtual void     Paint(Option_t *);
   virtual TAxis* GetCaxis() { return GetXaxis() ; }
   virtual TAxis* GetRaxis() { return GetYaxis() ; }

   virtual Int_t FillXY(Double_t,Double_t,Double_t);
   virtual Int_t FillXY(Double_t X,Double_t Y) { return FillXY(X,Y,1.);};
   virtual Double_t GetHoneyCombSize(void){ return fHoneyCombSize ;};
   virtual Int_t    GetShiftHexagon(void){ return bShiftHexagon ;};
   virtual Double_t SetHoneyCombSize(Double_t size){ fHoneyCombSize = size ; return fHoneyCombSize ;};
   virtual Int_t    SetShiftHexagon(Int_t shift){
     if(bShiftHexagon>0) { bShiftHexagon = 1 ; } else {  bShiftHexagon = 0 ;};
     return bShiftHexagon ;};
   Bool_t    FitTrack(void){return FitTrack("chi2",1,GetXaxis()->GetNbins(),1,GetYaxis()->GetNbins(),NULL,-1,-1); }; // *MENU*
   Bool_t    FitTrack(Option_t *,Int_t,Int_t,Int_t,Int_t,TCutG*,Double_t, Double_t); // *MENU*
   Bool_t    FitTrack(Option_t *o,Int_t c1,Int_t c2,Int_t r1,Int_t r2,Double_t x0, Double_t y0){return FitTrack(o,c1,c2,r1,r2,(TCutG*)NULL,x0,y0);};
   Bool_t    FitTrack(Option_t *o,Int_t c1,Int_t c2,Int_t r1,Int_t r2){return FitTrack(o,c1,c2,r1,r2,(TCutG*)NULL,-1,-1);};
   Bool_t    FitTrack(Option_t *o,TCutG *g,Double_t x0, Double_t y0){return FitTrack(o,1,GetXaxis()->GetNbins(),1,GetYaxis()->GetNbins(),g,x0,y0); };
   Bool_t    FitTrack(Option_t *o,TCutG *g){return FitTrack(o,1,GetXaxis()->GetNbins(),1,GetYaxis()->GetNbins(),g,-1,-1); };
   Bool_t    FitTrack(Option_t *o){return FitTrack(o,1,GetXaxis()->GetNbins(),1,GetYaxis()->GetNbins(),(TCutG*)NULL,-1,-1); };

   Int_t     FindClosestAxis(Int_t) ; //Find closest collinear axis to a give Track ID
   Int_t     FindClosestAxis(Double_t) ; // Find closest collinear axis to a give function coefficient
   TH1F*     ProjectOnTrack(Int_t, Option_t *,Double_t,Double_t) ; // Project pad value on track ID
   TH1F*     ProjectOnTrack(Int_t ID, Double_t xo,Double_t yo){ return ProjectOnTrack(ID,"origin",xo,yo);} ; // Project pad value on track ID
   TH1F*     ProjectOnTrack(Int_t ID){ return ProjectOnTrack(ID,"",-1,-1);} ; // Project pad value on track ID

   Double_t    FindRange(Int_t ID){return FindRange(ID,"",-1,-1);};
   Double_t    FindRange(Int_t ,Option_t *, Double_t, Double_t);
   Double_t    FindRange(Int_t ID, Double_t xo, Double_t yo){return FindRange(ID,"origin",xo,yo);};
   Int_t       GetSearchDiv(){ return fSearchDiv ;};
   Int_t       SetSearchDiv(Int_t d){ return fSearchDiv = d ;};
   Bool_t  IsInside(TCutG*,Int_t,Int_t);
   void    HighLightCut(TCutG*);
   void    HighLightCut(){ HighLightCut(NULL);};
   void    HighLightCutForTrack(Int_t ID){ HighLightCut(GetCutforTrack(ID));};
   void    HighLightCutForTrack() { HighLightCut(NULL);};

   Int_t   GetLastTrackNormalAxis(void) {return fLastNormalAxis+1 ; } ;
   Int_t   GetNormalTrackAxis(Int_t ID) {return fNormalTrackAxis[ID] ; } ;
   Int_t   GetAxisAngleDeg(Int_t ax) {return Axis_Angle[ax-1]*TMath::RadToDeg() ; } ;
   Int_t   GetAxisAngle(Int_t ax) {return Axis_Angle[ax-1] ; } ;
   TF1*    GetTrajectory(Int_t ID); // Pointer to the function decribing track #id
   TF1*    GetTrajectoryDisp(Int_t ID); // Pointer to the function decribing on screen track #id
   TH1F*    GetChargeProjection(Int_t ID); //  Pointer to the histogram, result of the charge projection on track ID
   //   Double_t GetRange(Int_t); // Get range for track #id. If does not exist, find it //TODO
   TCutG*  GetCutforTrack(Int_t);
   // Convertion routines :

   Double_t GetCdisp(Double_t, Double_t); // from C,R fractional to C as displayed
   Double_t GetRdisp(Double_t, Double_t); // from C,R fractional to R as displayed
   Double_t GetCdisp(Int_t C, Int_t R){ return GetCdisp((Double_t)C,(Double_t)R); }; // from C,R int to C as displayed
   Double_t GetRdisp(Int_t C, Int_t R){ return GetRdisp((Double_t)C,(Double_t)R); }; // from C,R int to R as displayed
   Double_t GetX(Int_t,Int_t); // from C,R int to X
   Double_t GetY(Int_t,Int_t); // from C,R int to Y
   //
   Bool_t GetCR(Double_t,Double_t, Int_t&, Int_t&); // from X,Y  to C,R int -> call this one
   Bool_t GetCR(Double_t,Double_t, Double_t&, Double_t&); // from X,Y  to C,R fractionnal  -> call this one
   Bool_t GetCRdisp_XY(Double_t,Double_t, Double_t&, Double_t&); // from X,Y  to C,R disp
   Bool_t GetXY_CRdisp(Double_t,Double_t, Double_t&, Double_t&); // from C,R disp to X,Y
   //

   //
   Int_t GetC(Double_t,Double_t); // from X,Y  to C int
   Int_t GetR(Double_t,Double_t); // from X,Y  to R int
   Double_t GetfC(Double_t,Double_t); // from X,Y  to C fractional
   Double_t GetfR(Double_t,Double_t); // from X,Y  to R fractional
   Int_t GetAxisLbl(Int_t, Int_t, Int_t); // from C,R int to Axis # label int   (note: axis # is last arg.)
   Int_t GetAxisIdx(Int_t, Int_t, Int_t); // from C,R int to Axis # index int   (note: axis # is last arg.)
   Int_t GetAxisCol(Int_t, Int_t, Int_t); // from Axis label,index int to C int (note: axis # is last arg.)
   Int_t GetAxisRow(Int_t, Int_t, Int_t); // from Axis label,index int to R int (note: axis # is last arg.)

   //

   TCutG* GetPAD(Int_t,Int_t);
   TCutG* GetPADXY(Double_t,Double_t);
   TClonesArray *GetArrayOfPADs(){ return fHoneyCombBins;} ;

   //

   Double_t solveBisect(TSpline3 *,Double_t,Double_t,Double_t);
   Double_t solveBisectD(TSpline3 *,Double_t,Double_t,Double_t);

   //
           TMayaHisto&    operator=(const TMayaHisto &h1);
   friend  TMayaHisto     operator*(Float_t c1, TMayaHisto &h1);
   friend  TMayaHisto     operator*(TMayaHisto &h1, Float_t c1);
   friend  TMayaHisto     operator+(TMayaHisto &h1, TMayaHisto &h2);
   friend  TMayaHisto     operator-(TMayaHisto &h1, TMayaHisto &h2);
   friend  TMayaHisto     operator*(TMayaHisto &h1, TMayaHisto &h2);
   friend  TMayaHisto     operator/(TMayaHisto &h1, TMayaHisto &h2);


 private:
   Bool_t __GetCR__(Double_t,Double_t, Double_t&, Double_t&, Bool_t); // from X,Y to C,R fractionnal (true) or int (false).
   TClonesArray *fHoneyCombBins ;//->Pointer to the array of HoneyCombBins
   Double_t fHoneyCombSize ; //size of the hexagon used for the honey comb
   Int_t bShiftHexagon ; // quantify if the first hexagon of the first line come before (1) or after (0) the fist hexagon of the second line
   Int_t fLastNormalAxis ; // Axis number (1,2 or 3) perpendicular to the track fitted
#define  Xview_corr .5
#define  Yview_corr .5
   Int_t fTCutGPatternStyle ;
   Double_t fOriginWeigh ;
   Int_t nTrack ;
   TList *fGCuts ; //->
   TList *fRanges ; //->
   Int_t fSearchDiv ; // number of primary subdivisions for maxima, root, ... search
   Bool_t fRangeSensX ; // search sens in X for range finding
   Bool_t fRangeSensY ; // search sens in Y for range finding
   Double_t fRangeThresRatio ; // % of the max of the "Bragg peak" to be considered above the background
   Double_t fRangePrecision ; // numerical precision on the finding of the range
   TF1 *fRangeFunction ; //->Function used to fit range, the parameter corresponding to the range MUST be named "range"

   static Double_t Axis_Angle[] ;
   static Double_t _dfltRangeFunction_(Double_t*,Double_t*); //! Default range function
   std::map<int,int> fNormalTrackAxis ;

   ClassDef(TMayaHisto,1);  //2-Dim histograms (one float per channel)
};

#endif

// Local IspellDict: english

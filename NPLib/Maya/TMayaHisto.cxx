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
#include "TMayaHisto.h"

//______________________________________________________________________________
//                     TMayaHisto methods
//  TMayaHisto a 2-D histogram with four bytes per cell (float)
//______________________________________________________________________________

ClassImp( TMayaHisto)

   //______________________________________________________________________________
TMayaHisto::TMayaHisto() :
	TH2F() {
	fHoneyCombBins = NULL;
	fGCuts = NULL;
	fRanges = NULL;
	fRangeFunction = NULL;

	fHoneyCombBins = new TClonesArray("TCutG", fNcells);
	fRangeFunction = new TF1();
	fGCuts = new TList();
	fRanges = new TList();

	fGCuts->SetOwner();
	fRanges->SetOwner();
	fHoneyCombBins->SetOwner();

	// Constructor.
}

//______________________________________________________________________________
TMayaHisto::~TMayaHisto() {
	if (fHoneyCombBins) {
		delete (fHoneyCombBins);
		fHoneyCombBins = NULL;
	}
	if (fRangeFunction) {
		delete (fRangeFunction);
		fRangeFunction = NULL;
	}
	if (fGCuts) {
		delete (fGCuts);
		fGCuts = NULL;
	}
	if (fRanges) {
		delete (fRanges);
		fRanges = NULL;
	}

	// Destructor.
}
//______________________________________________________________________________
TMayaHisto::TMayaHisto(const char *name, const char *title, Int_t nbinsx,
		Int_t nbinsy, Double_t hsize) :
	TH2F(name, title, nbinsx, 1.1, 1.1 + nbinsx, nbinsy, 1.1, 1.1 + nbinsx)
/*
 :TH2(name,title,
 nbinsx,0,(nbinsx+(nbinsx>1)/2.)*hsize*TMath::Sqrt(3),
 nbinsy,0,(int((nbinsy)/2)*3+.5+nbinsy%2*1.5)*hsize)
 */// fill by (x,y)
{

	// Constructor.

	fMinimum = -1111;
	fMaximum = -1111;
	fHoneyCombBins = NULL;
	fGCuts = NULL;
	fRanges = NULL;
	fRangeFunction = NULL;

	fHoneyCombSize = fabs(hsize);
	fLineColor = 1;
	if (hsize > 0) {
		bShiftHexagon = 1;
	} else {
		bShiftHexagon = 0;
	}
	GetXaxis()->SetTickLength(0);
	GetYaxis()->SetTickLength(0);
	GetXaxis()->SetAxisColor(gStyle->GetCanvasColor());
	GetYaxis()->SetAxisColor(gStyle->GetCanvasColor());
	GetXaxis()->SetLabelColor(gStyle->GetCanvasColor());
	GetYaxis()->SetLabelColor(gStyle->GetCanvasColor());

	//GetYaxis()->SetBinLabel(nbinsy,atoi(1));

	TArrayF::Set(fNcells);
	if (fgDefaultSumw2)
		Sumw2();
	CreateHoneyCombStructure();

	// Track
	fLastNormalAxis = -1;
	fOriginWeigh = .5;

	nTrack = 0;
	fGCuts = new TList;
	fRanges = new TList;

	fGCuts->SetOwner();
	fRanges->SetOwner();
	fHoneyCombBins->SetOwner();

	fRangeFunction = new TF1("_dfltRangeFunction_", _dfltRangeFunction_, 0,
			TMath::Sqrt(3* (TMath ::Power(nbinsx,2)+TMath::Power(nbinsy,2))),3);
			fRangeFunction->SetParNames("base","range","slope");
			fSearchDiv = 10;
			fRangeThresRatio = .1;
			fRangePrecision = fHoneyCombSize*.1;
			// Style
					fTCutGPatternStyle = 3001;
				}

	//______________________________________________________________________________
TMayaHisto::TMayaHisto(const TMayaHisto &h2f) :
	TH2F() {
	// Copy constructor.

	((TMayaHisto&) h2f).Copy(*this);
}
//______________________________________________________________________________

TCutG *TMayaHisto::Hexagon(TString name, Float_t x0, Float_t y0) {
	//  Float_t x [7] = {0},y[7] = {0} ;
	Float_t x[7] = { -.5, 0, 0.5, 0.5, 0, -.5, -.5 };
	Float_t y[7] = { 1. / 3., 2. / 3., 1. / 3., -1. / 3., -2. / 3., -1. / 3.,
			1. / 3. };
	for (int i = 0; i < 7; i++) {
		//    x[i] = x0 + TMath::Cos(TMath::Pi()/3.*i+TMath::PiOver2());//* fHoneyCombSize ;
		//    y[i] = y0 + TMath::Sin(TMath::Pi()/3.*i+TMath::PiOver2());//* fHoneyCombSize ;
		x[i] *= 1 + 1e-3; // to be sure that boundaries overlap !
		y[i] *= 1 + 1e-3; // to be sure that boundaries overlap !
		x[i] += x0;
		y[i] += y0;
	}
	TCutG *h = new TCutG(name.Data(), 7, x, y);
	h->SetFillStyle(1000);
	return h;
}

//______________________________________________________________________________
void TMayaHisto::CreateHoneyCombStructure() {
	fHoneyCombBins = new TClonesArray("TCutG", fNcells);
	fHoneyCombBins->BypassStreamer(kFALSE);
	fHoneyCombBins->SetOwner();
	TString name;
	TObject* obj;
	Int_t bin;
	Float_t x[7];
	Float_t y[7];
	// TClonesArray &ar = *fHoneyCombBins;
	for (int i = 0; i < this->GetXaxis()->GetNbins(); i++) {
		for (int j = 0; j < this->GetYaxis()->GetNbins(); j++) {
			name = TString("pad_") + (Long_t)(i + 1) + TString("_") + (Long_t)(
					j + 1);
			bin = GetBin(i, j);
			x[0]= -0.5;
			x[1]=  0.0;
			x[2]=  0.5;
			x[3]=  0.5;
			x[4]=  0.0;
			x[5]= -0.5;
			x[6]= -0.5;
			y[0]=  1./3.;
			y[1]=  2./3.;
			y[2]=  1./3.;
			y[3]= -1./3.;
			y[4]= -2./3.;
			y[5]= -1./3.;
			y[6]=  1./3.;
			for (int l = 0; l < 7; l++) {
				x[l] *= 1 + 1e-3; // to be sure that boundaries overlap !
				y[l] *= 1 + 1e-3; // to be sure that boundaries overlap !
				x[l] += GetCdisp(i + 1, j + 1);
				y[l] += GetRdisp(i + 1, j + 1);
			}

			new ((*fHoneyCombBins)[bin]) TCutG(name.Data(), 7, x, y);
			obj = (fHoneyCombBins)->At(bin);
			((TCutG*) obj)->SetFillStyle(1000);

		}
	}
}

//
// Fill histogram according to position, not PAD indexes.
//______________________________________________________________________________

Int_t TMayaHisto::FillXY(Double_t X, Double_t Y, Double_t W) {
	Int_t C, R;
	if (GetCR(X, Y, C, R))
		return Fill(C, R, W);
	return 0;
}

//______________________________________________________________________________

void TMayaHisto::Paint(Option_t *option = "") {
	GetPainter(option);
	//  gPad->SetFrameLineColor(gStyle->GetCanvasColor());
	//  gPad->SetFrameFillStyle(4000);
	//  gPad->SetFillStyle(4000);
	//  gPad->SetFrameLineColor(gStyle->GetCanvasColor());

	Double_t min, max;
	if (fMinimum == -1111) {
		min = GetMinimum();
		if (min < 0)
			min = 0;
		fMinimum = min;
	} else {
		min = fMinimum;
	}
	if (fMaximum == -1111) {
		max = GetMaximum();
	} else {
		max = fMaximum;
	}

	Int_t ncol = gStyle->GetNumberOfColors();

	GetZaxis()->Set(ncol, min, max);

	if (fPainter) {
		fPainter->Paint("colz,hist");
		fPainter->Paint("axis");
		gPad->Clear();
		if (GetEntries() != 0
				&& (GetZaxis()->GetXmin() < GetZaxis()->GetXmax()))
			((THistPainter*) fPainter)->PaintPalette();
	}
	gPad->Clear();

	Int_t q = 0;
	for (int i = 0; i < this->GetXaxis()->GetNbins(); i++) {
		for (int j = 0; j < this->GetYaxis()->GetNbins(); j++) {
			if (fArray[GetBin(i, j)] > min && max > 0) {
				if (gPad->GetLogz()) {
					q = int(log(fArray[GetBin(i, j)] - min + 1e-6) / log(max
							- min + 1e-6) * ncol);
				} else {
					q = int((fArray[GetBin(i, j)] - min) / (max - min) * ncol);
				}
				GetPAD(i + 1, j + 1)->SetFillColor(gStyle->GetColorPalette(q
						- 1));
			} else {
				if (GetPAD(i + 1, j + 1)->GetFillStyle() == 1000) {
					GetPAD(i + 1, j + 1)->SetFillStyle(0);
					GetPAD(i + 1, j + 1)->SetFillColor(
							GetPAD(i + 1, j + 1)->GetLineColor());
				} else {
					GetPAD(i + 1, j + 1)->SetFillColor(
							GetPAD(i + 1, j + 1)->GetLineColor());
				}
			}

			//    if(((TCutG*)fHoneyCombBins->At(GetBin(i,j)))->GetFillColor()>0){
			//    ((TCutG*)fHoneyCombBins->At(GetBin(i,j)))->SetLineWidth(fLineWidth);
			GetPAD(i + 1, j + 1)->SetLineColor(fLineColor);
			//    }
			GetPAD(i + 1, j + 1)->Draw("f");
			GetPAD(i + 1, j + 1)->Draw();

		}
	}

	TIter next(GetListOfFunctions());
	TObject *obj;

	while ((obj = next())) { // loop over all functions
		if (obj->InheritsFrom("TF1") && (strncmp(obj->GetName(), "disp", 4)
				== 0)) {
			obj->Draw("same");
		} else if (!obj->InheritsFrom("TF1") && !obj->InheritsFrom("TH1")) {
			obj->Draw("same");
		}
		/*
		 if (obj->InheritsFrom("TAttMarker") && !obj->InheritsFrom("TF1")){
		 obj->Draw("same");
		 }
		 if (obj->InheritsFrom("TAttMarker") && !obj->InheritsFrom("TF1")){
		 obj->Draw("same");
		 }
		 */
	}

	//  if(GetFunction("TPolyMarker")) GetFunction("TPolyMarker")->Draw();
}

//______________________________________________________________________________
void TMayaHisto::Copy(TObject &newth2) const {
	// Copy.

	TH2F::Copy((TMayaHisto&) newth2);
}

//______________________________________________________________________________
TH1 *TMayaHisto::DrawCopy(Option_t *option) const {
	// Draw copy.

	TString opt = option;
	opt.ToLower();
	if (gPad && !opt.Contains("same"))
		gPad->Clear();
	TMayaHisto *newth2 = (TMayaHisto*) Clone();
	newth2->SetDirectory(0);
	newth2->SetBit(kCanDelete);
	newth2->AppendPad(option);
	return newth2;
}

//______________________________________________________________________________
Double_t TMayaHisto::GetBinContent(Int_t bin) const {
	// Get bin content.

	if (fBuffer)
		((TH2C*) this)->BufferEmpty();
	if (bin < 0)
		bin = 0;
	if (bin >= fNcells)
		bin = fNcells - 1;
	if (!fArray)
		return 0;
	return Double_t(fArray[bin]);
}

//______________________________________________________________________________
void TMayaHisto::Reset(Option_t *option) {
	//*-*-*-*-*-*-*-*Reset this histogram: contents, errors, etc*-*-*-*-*-*-*-*
	//*-*            ===========================================

	TH2F::Reset(option);
	//TArrayF::Reset();
	nTrack = 0;
	fLastNormalAxis = -1;
	fGCuts->Clear();
	fRanges->Clear();

	TIter next(GetArrayOfPADs());
	TObject *obj;
	while ((obj = next())) {
		((TCutG*) obj)->SetFillStyle(1000);
		((TCutG*) obj)->SetFillColor(gStyle->GetCanvasColor());
		((TCutG*) obj)->SetLineColor(fLineColor);
	}

}

//______________________________________________________________________________
void TMayaHisto::SetBinContent(Int_t bin, Double_t content) {
	// Set bin content
	if (bin < 0)
		return;
	if (bin >= fNcells)
		return;
	fArray[bin] = Float_t(content);
	fEntries++;
	fTsumw = 0;
}

//______________________________________________________________________________
void TMayaHisto::SetBinsLength(Int_t n) {
	// Set total number of bins including under/overflow
	// Reallocate bin contents array

	if (n < 0)
		n = (fXaxis.GetNbins() + 2) * (fYaxis.GetNbins() + 2);
	fNcells = n;
	TArrayF::Set(n);
}

//
// Return a pointer on the PAD at C,R
//______________________________________________________________________________

TCutG* TMayaHisto::GetPAD(Int_t C, Int_t R) {
	return (TCutG*) fHoneyCombBins->At(GetBin(C - 1, R - 1));
}

//
// Return a pointer on the PAD at X,Y
//______________________________________________________________________________

TCutG* TMayaHisto::GetPADXY(Double_t X, Double_t Y) {
	Int_t C, R;
	if (GetCR(X, Y, C, R))
		return GetPAD(C, R);
	return NULL;
}

//
// Return the corresponding Y of the PAD in the ROOT referential
//
//    we want :                but we have in root :
//
//    +----------> x (C)       y (C)
//    |			       ^
//    |			       |
//    |			       |
//    V			       |
//    y (R)                    +----------> x (R)
//______________________________________________________________________________

Double_t TMayaHisto::GetCdisp(Double_t C, Double_t R) {
	Double_t fR, iR;
	fR = modf(R, &iR);
   fR *= 1;
	return C + (int(iR - bShiftHexagon) % 2) / 2. + Xview_corr;
}

Double_t TMayaHisto::GetRdisp(Double_t C, Double_t R) {
	return (GetYaxis()->GetNbins() - R) + Yview_corr + 1;
}

//
// Give the X position  corresponding to the PAD(c,r)
//______________________________________________________________________________

Double_t TMayaHisto::GetX(Int_t C, Int_t R) {
	if (C > 0 && R > 0 && C <= GetXaxis()->GetNbins() && R
			<= GetYaxis()->GetNbins()) {
		return fHoneyCombSize * TMath::Sqrt(3) / 2. * (2* C - (R + 1
				- bShiftHexagon) % 2); // tested
	} else {
		return 0;
	}
}

//
// Give the Y position  corresponding to the PAD(c,r)
//______________________________________________________________________________

Double_t TMayaHisto::GetY(Int_t C, Int_t R) {
	if (C > 0 && R > 0 && C <= GetXaxis()->GetNbins() && R
			<= GetYaxis()->GetNbins()) {
		return fHoneyCombSize * (1 + (R - 1) * 1.5); // tested
	} else {
		return 0;
	}
}

//
// Give the PAD C and R position  corresponding to the position (X,Y)
// To avoid complicated calculations :
// - find the "easiest" (in terms of analytical solution) pad
// - check if the point is inside this pad
// - otherwise check "intelligently" the neighbours
// - of course exit as soon as found !
//______________________________________________________________________________


Bool_t TMayaHisto::GetCR(Double_t X, Double_t Y, Int_t &C, Int_t &R) {
	Double_t fC = (Double_t) C, fR = (Double_t) R;
	if (__GetCR__(X, Y, fC, fR, kFALSE)) {
		C = int(fC);
		R = int(fR);
		return kTRUE;
	}
	return kFALSE;
}

//
// Return X,Y from C,R displayed
//______________________________________________________________________________


Bool_t TMayaHisto::GetXY_CRdisp(Double_t fCd, Double_t fRd, Double_t &X,
		Double_t &Y) { // from C,R fractional displayed to X

	if (fCd > GetXaxis()->GetXmin() && fRd > GetYaxis()->GetXmin() && fCd
			< GetXaxis()->GetXmax() && fRd < GetYaxis()->GetXmax()) { // strictly to avoid border effects !

		X = (fCd - (Xview_corr + .5)) * (fHoneyCombSize * TMath::Sqrt(3))
				* (GetXaxis()->GetXmax() - GetXaxis()->GetXmin())
				/ GetXaxis()->GetNbins();

		Y = ((Yview_corr - .4 - .1 / 3.) + GetYaxis()->GetXmax() - fRd)
				* (fHoneyCombSize * 1.5) * (GetYaxis()->GetXmax()
				- GetYaxis()->GetXmin()) / GetYaxis()->GetNbins();

		return kTRUE;
	} else {
		return kFALSE;
	}
	return kFALSE;
}

//Double_t GetY_CRdisp(Double_t,Double_t){ // from C,R fractional displayed to Y
//  return
//}

//
//
//______________________________________________________________________________

Bool_t TMayaHisto::GetCR(Double_t X, Double_t Y, Double_t &C, Double_t &R) {
	return __GetCR__(X, Y, C, R, kTRUE);
}

Bool_t TMayaHisto::GetCRdisp_XY(Double_t X, Double_t Y, Double_t &C,
		Double_t &R) {

	C = (Xview_corr + .5) + X / (fHoneyCombSize * TMath::Sqrt(3))
			/ (GetXaxis()->GetXmax() - GetXaxis()->GetXmin())
			* GetXaxis()->GetNbins();
	R = (Yview_corr - .4 - .1 / 3.) + GetYaxis()->GetXmax() - (Y
			/ fHoneyCombSize / 1.5) / (GetYaxis()->GetXmax()
			- GetYaxis()->GetXmin()) * GetYaxis()->GetNbins();

	if (C > GetXaxis()->GetXmin() && R > GetYaxis()->GetXmin() && C
			< GetXaxis()->GetXmax() && R < GetYaxis()->GetXmax()) { // strictly to avoid border effects !
		return kTRUE;
	} else {
		return kFALSE;
	}
}

Bool_t TMayaHisto::__GetCR__(Double_t X, Double_t Y, Double_t &C, Double_t &R,
		Bool_t IsFrac) {

	Double_t fR = (Y / fHoneyCombSize - 1.) / 1.5 + 1;
	Int_t iR = TMath::Nint(fR + 1e-6);

	Double_t fC = (X / (TMath::Sqrt(3) * fHoneyCombSize)) - .5*
			( (iR+bShiftHexagon)%2);
			Int_t iC = TMath::Nint(fC+1e-6);

			// std::cerr << "#C# C: "  << C <<
			//      " " << R << " " << fC << " " << fR << " " << GetCdisp(fC,fR) << " " << GetRdisp(fC,fR) << std::endl;
			C = -1; R = -1;
			if(GetPAD(iC,iR)==NULL) return kFALSE;

			if(GetPAD(iC,iR)->IsInside(GetCdisp(fC,fR),GetRdisp(fC,fR))) {
				if(IsFrac) {
					C = (iC) + (X-GetX(iC,iR))/(TMath::Sqrt(3)*fHoneyCombSize) ;
      R = (iR) + (Y-GetY(iC,iR))/(TMath::Sqrt(3)*fHoneyCombSize) ;
      //      std::cerr << iC << " + " << X << "-" << GetX(iC,iR) << " = " << C << std::endl ;
      //      std::cerr << iR << " + " << Y << "-" << GetY(iC,iR) << " = " << R << std::endl ;
      return kTRUE;
    } else {
      C = iC ; R = iR ;
      return kTRUE;
    }
  } else {
    for(int i=-1;i<=1;i++){
      for(int j=-1;j<=1;j++){
	if(GetPAD(iC+i,iR+j)!=NULL){
	  if(GetPAD(iC+i,iR+j)->IsInside(GetCdisp(fC,fR),GetRdisp(fC,fR))){
	    if(IsFrac){
	      C = (iC+i) + (X-GetX(iC+i,iR+j))/(TMath::Sqrt(3)*fHoneyCombSize) ;
	      R = (iR+j) + (Y-GetY(iC+i,iR+j))/(TMath::Sqrt(3)*fHoneyCombSize) ;
	      //	      std::cerr << iC+i << " + " << X << "-" << GetX(iC+i,iR+j) << " = " << C << " []" << std::endl ;
	      //	      std::cerr << iR+j << " + " << Y << "-" << GetY(iC+i,iR+j) << " = " << R << " []" << std::endl ;
	      return kTRUE;
	    } else {
	      C = iC+i ; R = iR+j ;
	      return kTRUE;
	    }
	  }
	}
      }
    }

  }
  C = -1 ;
  R = -1 ;
  return kFALSE ;
}

	//
	// Give the PAD C position  corresponding to the position (X,Y)
	//______________________________________________________________________________

Int_t TMayaHisto::GetC(Double_t X, Double_t Y) {
	Int_t C, R;
	GetCR(X, Y, C, R);
	return C;
}

//
// Give the PAD R position  corresponding to the position (X,Y)
//______________________________________________________________________________

Int_t TMayaHisto::GetR(Double_t X, Double_t Y) {
	Int_t C, R;
	GetCR(X, Y, C, R);
	return R;
}

//
// Give the PAD C position (fractional) corresponding to the position (X,Y)
//______________________________________________________________________________

Double_t TMayaHisto::GetfC(Double_t X, Double_t Y) {
	Double_t C, R;
	GetCR(X, Y, C, R);
	return C;
}

//
// Give the PAD R position (fractional) corresponding to the position (X,Y)
//______________________________________________________________________________

Double_t TMayaHisto::GetfR(Double_t X, Double_t Y) {
	Double_t C, R;
	GetCR(X, Y, C, R);
	return R;
}

Double_t TMayaHisto::Axis_Angle[] = { TMath::Pi() / 3., -TMath::Pi() / 3., 0. };

/*  HoneyComb symetry axis definition

 / \ / \ / \ / \            axis 2
 |   |   |   |   |     \   /
 / \ / \ / \ / \ /       \ /
 |   |   |   |   |    -----X------ axis 3
 / \ / \ / \ / \       / \
        |   |   |   |   |     /   \
	 \ / \ / \ / \ /           axis 1

 */

//______________________________________________________________________________
//
// Give the Label corresponding to the PAD(c,r) for the Axis n
//______________________________________________________________________________

Int_t TMayaHisto::GetAxisLbl(Int_t C, Int_t R, Int_t N) {
	switch (N) {
	// -- Axis 1
	case 1:
		return C - int((R + bShiftHexagon) / 2.) + int(GetYaxis()->GetNbins()
				/ 2.);
		// -- Axis 2
	case 2:
		return C + int((R + 1 - bShiftHexagon) / 2);
		// -- Axis 3
	case 3:
		return R;
	}
	return 0;
}

//______________________________________________________________________________
//
// Give the Index corresponding to the PAD(c,r) for the Axis N
//______________________________________________________________________________

Int_t TMayaHisto::GetAxisIdx(Int_t C, Int_t R, Int_t N) {
	switch (N) {
	// -- Axis 1
	case 1:
		if (bShiftHexagon == 1) {
			return (GetAxisLbl(C, R, N) < int(GetYaxis()->GetNbins() / 2.) ? 2*
					C - (R % 2) : R);
		} else {
			return (GetAxisLbl(C, R, N) <= int(GetYaxis()->GetNbins() / 2.) ? 2*
					C + (R % 2) - 1
					: R);
		}
		// -- Axis 2
	case 2:
		if (bShiftHexagon == 1) {
			return (GetAxisLbl(C, R, N) <= GetXaxis()->GetNbins() ? R : 2*
					(GetXaxis ()->GetNbins()-C)+R%2+1);
				} else {
					return (GetAxisLbl(C,R,N)<=GetXaxis()->GetNbins()?R:2*(GetXaxis()->GetNbins()-C)-R%2+2);
    }
			// -- Axis 3
			case 3: return C;
		}
		return 0;
	}

	// ******* REVERSE FUNCTIONS ***********

	//______________________________________________________________________________
	//
	// Give the Col corresponding to the PAD(Lbl,Idx) for the Axis N
	//______________________________________________________________________________

Int_t TMayaHisto::GetAxisCol(Int_t L, Int_t I, Int_t N) {
	switch (N) {
	// -- Axis 1
	case 1:
		return (L <= int(GetYaxis()->GetNbins() / 2) ? int(I / 2) + (I) % 2 : L
				+ int((I + bShiftHexagon) / 2.) - int(GetYaxis()->GetNbins()
				/ 2));
		// -- Axis 2
	case 2:
		if (bShiftHexagon == 1) {
			return (L <= GetXaxis()->GetNbins() ? L - int(I / 2)
					: GetXaxis()->GetNbins() - int((I - 1) / 2.));
		} else {
			return (L <= GetXaxis()->GetNbins() ? L - int((I + 1) / 2)
					: GetXaxis()->GetNbins() - int((I - 1) / 2.));
		}
		// -- Axis 3
	case 3:
		return I;
	}
	return 0;
}

//______________________________________________________________________________
//
// Give the Row corresponding to the PAD(Lbl,Idx) for the Axis N
//______________________________________________________________________________

Int_t TMayaHisto::GetAxisRow(Int_t L, Int_t I, Int_t N) {
	switch (N) {
	// -- Axis 1
	case 1:
		if (bShiftHexagon == 1) {
			return (L < int(GetYaxis()->GetNbins() / 2) ? I - 2* L + 2*
					int (GetYaxis()->GetNbins()/2):I);
				} else {
					return (L<int(GetYaxis()->GetNbins()/2)?1+I+2*(int(GetYaxis()->GetNbins()/2)-L):I);
    }
			// -- Axis 2
			case 2: if(bShiftHexagon==1) {
				return (L<=GetXaxis()->GetNbins()?I:2*(L-GetXaxis()->GetNbins())+I-1);
			} else {
				return (L<=GetXaxis()->GetNbins()?I:I+2*(L-GetXaxis()->GetNbins()-1));
			}
			// -- Axis 3
			case 3: return L;
		}
		return 0;
	}

	//______________________________________________________________________________
	//
	// Check if Row and Col are inside a graphical cut g
	//______________________________________________________________________________

Bool_t TMayaHisto::IsInside(TCutG*g, Int_t C, Int_t R) {
	if (g == NULL)
		return kTRUE; // By default is inside !
	return g->IsInside(GetCdisp(C, R), GetRdisp(C, R));
}

//______________________________________________________________________________
//
// Fit
//______________________________________________________________________________

Bool_t TMayaHisto::FitTrack(Option_t *o, Int_t Cmin, Int_t Cmax, // limits in Columns
		Int_t Rmin, Int_t Rmax, // limits in Rows
		TCutG *gFitRegion, // graphical cuts
		Double_t x0, Double_t y0) { // forced point => "origin" option
	TString opt = o;

	Double_t A, B, C, UEV, Q, X, Xg, Y, Yg;
	A = B = C = UEV = Q = X = Y = Xg = Yg = 0.;
	//

	Int_t Row, Col;
	/*
	 Int_t Cmin=1;
	 Int_t Cmax=GetXaxis()->GetNbins();
	 Int_t Rmin=1;
	 Int_t Rmax=GetYaxis()->GetNbins();
	 */
	Double_t min = TMath::Max(fMinimum, 0.);
	Double_t padVal;

	// Prefit : find the right axe

	typedef std::map<int, std::map<int, float> > axis_map;
	typedef std::map<int, float> axis_fscalar;
	typedef std::map<int, int> axis_iscalar;

	axis_map PAD_Axis_Content[3];
	axis_fscalar PAD_Axis_Max[3];
	axis_iscalar PAD_Axis_Max_idx[3];
	axis_fscalar PAD_Axis_PosX_Fit[3];
	axis_fscalar PAD_Axis_PosY_Fit[3];

	Char_t _id_[128] = "";
	Int_t AxisIdx, AxisLbl;
	Int_t c, r;
	Double_t cd, rd;

	if (gFitRegion == NULL) {
		gFitRegion = new TCutG("square_region", 4);
		gFitRegion->SetPoint(0, GetCdisp(Cmin, Rmin) - .1, GetRdisp(Cmin, Rmin)
				+ .1);
		gFitRegion->SetPoint(1, GetCdisp(Cmin, Rmax) - .1, GetRdisp(Cmin, Rmax)
				- .1);
		gFitRegion->SetPoint(2, GetCdisp(Cmax, Rmax) + .1, GetRdisp(Cmax, Rmax)
				- .1);
		gFitRegion->SetPoint(3, GetCdisp(Cmax, Rmin) + .1, GetRdisp(Cmax, Rmin)
				+ .1);
	}

	if (opt.Contains("sech")) {

		for (Row = Rmin; Row <= Rmax; Row++) {
			for (Col = Cmin; Col <= Cmax; Col++) {

				if (Col - GetAxisCol(GetAxisLbl(Col, Row, 1), GetAxisIdx(Col,
						Row, 1), 1) != 0) {
					std::cerr << "1) Problem w/ at (" << Col << "," << Row
							<< ") : " << GetAxisCol(GetAxisLbl(Col, Row, 1),
							GetAxisIdx(Col, Row, 1), 1) << "#"
							<< GetAxisRow(GetAxisLbl(Col, Row, 1), GetAxisIdx(
									Col, Row, 1), 1) << std::endl;
				}

				if (Col - GetAxisCol(GetAxisLbl(Col, Row, 2), GetAxisIdx(Col,
						Row, 2), 2) != 0) {
					std::cerr << "2) Problem w/ at (" << Col << "," << Row
							<< ") : " << GetAxisIdx(Col, Row, 2) << "#"
							<< GetAxisRow(GetAxisLbl(Col, Row, 2), GetAxisIdx(
									Col, Row, 2), 2) << std::endl;
				}

				if (Col - GetAxisCol(GetAxisLbl(Col, Row, 3), GetAxisIdx(Col,
						Row, 3), 3) != 0) {
					std::cerr << "3) Problem w/ at (" << Col << "," << Row
							<< ") : " << GetAxisCol(GetAxisLbl(Col, Row, 3),
							GetAxisIdx(Col, Row, 3), 3) << "#"
							<< GetAxisRow(GetAxisLbl(Col, Row, 3), GetAxisIdx(
									Col, Row, 3), 3) << std::endl;
				}

				GetPAD(Col, Row)->SetLineWidth(1);

				if (!IsInside(gFitRegion, Col, Row))
					continue;

				padVal = fArray[GetBin(Col - 1, Row - 1)];

				if (padVal > min) {

					// Axis 1
					AxisLbl = GetAxisLbl(Col, Row, 1);
					AxisIdx = GetAxisIdx(Col, Row, 1);
					if (AxisLbl > 0 && AxisIdx > 0) {
						PAD_Axis_Content[0][AxisLbl][AxisIdx] = padVal;
						if (padVal > PAD_Axis_Max[0][AxisLbl] && padVal > 0) {
							PAD_Axis_Max[0][AxisLbl] = padVal;
							PAD_Axis_Max_idx[0][AxisLbl] = AxisIdx;
						}
					}

					// Axis 2
					AxisLbl = GetAxisLbl(Col, Row, 2);
					AxisIdx = GetAxisIdx(Col, Row, 2);
					if (AxisLbl > 0 && AxisIdx > 0) {
						PAD_Axis_Content[1][AxisLbl][AxisIdx] = padVal;
						if (padVal > PAD_Axis_Max[1][AxisLbl] && padVal > 0) {
							PAD_Axis_Max[1][AxisLbl] = padVal;
							PAD_Axis_Max_idx[1][AxisLbl] = AxisIdx;
						}
					}

					// Axis 3
					AxisLbl = GetAxisLbl(Col, Row, 3);
					AxisIdx = GetAxisIdx(Col, Row, 3);
					if (AxisLbl > 0 && AxisIdx > 0) {
						PAD_Axis_Content[2][AxisLbl][AxisIdx] = padVal;
						if (padVal > PAD_Axis_Max[2][AxisLbl] && padVal > 0) {
							PAD_Axis_Max[2][AxisLbl] = padVal;
							PAD_Axis_Max_idx[2][AxisLbl] = AxisIdx;
						}
					}
				}
			}
		}

		Int_t max_itr = -1;
		for (Int_t i = 0; i < 3; i++) {
			//   std::cerr << i+1 << ") " << PAD_Axis_Content[i].size() << std::endl ;
			max_itr = TMath::Max(max_itr, (Int_t) GetAxisLbl(
					GetXaxis()->GetNbins(), GetXaxis()->GetNbins(), i + 1));
		}

		Int_t NMax[3] = { 0 }, idx;
		Double_t a1, a2, dR, Qo, Qp, Qm;
		for (Int_t l = 1; l <= max_itr; l++) {
			for (Int_t a = 0; a < 3; a++) {
				idx = PAD_Axis_Max_idx[a][l];
				if (idx > 0 && l > 0) {
					Qo = (Double_t) PAD_Axis_Content[a][l][idx];
					Qm = (Double_t) PAD_Axis_Content[a][l][idx - 1];
					Qp = (Double_t) PAD_Axis_Content[a][l][idx + 1];
					c = GetAxisCol(l, idx, a + 1);
					r = GetAxisRow(l, idx, a + 1);

					if (Qo > 0 && Qp > 0 && Qm > 0 && c > 0 && r > 0) {
						NMax[a]++;
						// why not doing the sech fit here already ?
						// look at Thomas ROGER thesis, page 22
						// note: here: Qo = Q0 ; Qp = Q+ ; Qm = Q-
						a2 = .5* (TMath ::Sqrt(Qo/Qp)+TMath::Sqrt(Qo/Qm));
						a1 = (TMath::Sqrt(Qo/Qp)-TMath::Sqrt(Qo/Qm))/(2.*TMath::SinH(a2));
	    dR = (fHoneyCombSize)*TMath::Log((1+a1)/(1-a1))/TMath::Log(a2+TMath::Sqrt(a2*a2-1.));
	    PAD_Axis_PosX_Fit[a][l] =  dR*TMath::Cos(Axis_Angle[a]) +  GetX(c,r) ;
	    PAD_Axis_PosY_Fit[a][l] =  dR*TMath::Sin(Axis_Angle[a]) +  GetY(c,r) ;
	    //	  std::cerr << "Fit axis " << a+1 << " (" << c << ","  << r << ") -> " ;
	    //	  std::cerr << PAD_Axis_PosX_Fit[a][l] << "," << PAD_Axis_PosY_Fit[a][l] << " from " ;
	    //	  std::cerr <<  GetX(c,r) << " " <<  GetY(c,r) ;
	    //	  std::cerr << " w/ dR = "  << dR << std::endl ;
	  }
					}
				}
			}
			Float_t max_tot = -1.;
			fLastNormalAxis = -1;
			for(Int_t i=0;i<3;i++) {
				//    std::cerr << i+1 << ") " << NMax[i] << std::endl ;
				if(NMax[i]>max_tot) {
					max_tot = NMax[i];
					fLastNormalAxis = i;
				}
			}

			std::cerr << "The trajectory is 'perpendicular' to the Axis" << fLastNormalAxis+1 << std::endl;

			// Highlight the maximums find on the axis
			// and start the fit
			TPolyMarker *MarkMax = new TPolyMarker();

			A=B=C=UEV=Q=X=Y=Xg=Yg=0.;

			for(Int_t l=1;l<=max_itr;l++) {
				c = GetAxisCol(l,PAD_Axis_Max_idx[fLastNormalAxis][l],fLastNormalAxis+1);
				r = GetAxisRow(l,PAD_Axis_Max_idx[fLastNormalAxis][l],fLastNormalAxis+1);
				if(c>0 && r>0 && PAD_Axis_Max[fLastNormalAxis][l]>0) {
					GetPAD(c,r)->SetLineWidth(3);
					if(PAD_Axis_PosX_Fit[fLastNormalAxis][l]>0 && PAD_Axis_PosY_Fit[fLastNormalAxis][l]>0) {

						// ** Fit **
						padVal = PAD_Axis_Max[fLastNormalAxis][l];
						//	std::cerr << padVal << " " << Q << " " << Xg << " " << Yg << std::endl ;
						if(padVal>min) {
							Q+=padVal;
							Xg+=PAD_Axis_PosX_Fit[fLastNormalAxis][l]*padVal;
							Yg+=PAD_Axis_PosY_Fit[fLastNormalAxis][l]*padVal;
						}

						// ** Graphics **
						cd = GetCdisp(c,r);
						rd = GetRdisp(c,r);
						GetCRdisp_XY(PAD_Axis_PosX_Fit[fLastNormalAxis][l] ,PAD_Axis_PosY_Fit[fLastNormalAxis][l],cd,rd);
						MarkMax->SetNextPoint(cd,rd);
						//	std::cerr << c << ": " << PAD_Axis_PosX_Fit[fLastNormalAxis][l] << "->" << cd << " # " ;
						//	std::cerr << r << ": " << PAD_Axis_PosY_Fit[fLastNormalAxis][l] << "->" << rd << std::endl ;
					}
				}
			}

			//
			MarkMax->SetMarkerStyle(29);
			MarkMax->SetMarkerSize(1);
			MarkMax->SetMarkerColor(gStyle->GetCanvasColor());
			fFunctions->Add(MarkMax);
			//  ----

			if(opt.Contains("origin")) {
				Xg = (x0*Q*fOriginWeigh + Xg)/(Q*(1.+fOriginWeigh)); // modified center of gravity X position
				Yg = (y0*Q*fOriginWeigh + Yg)/(Q*(1.+fOriginWeigh)); // modified center of gravity Y position
			} else {
				Xg/=Q; // center of gravity X position
				Yg/=Q; // center of gravity Y position
			}

			for(Int_t l=1;l<=max_itr;l++) {
				c = GetAxisCol(l,PAD_Axis_Max_idx[fLastNormalAxis][l],fLastNormalAxis+1);
				r = GetAxisRow(l,PAD_Axis_Max_idx[fLastNormalAxis][l],fLastNormalAxis+1);
				if(c>0 && r>0 && PAD_Axis_Max[fLastNormalAxis][l]>0) {
					if(PAD_Axis_PosX_Fit[fLastNormalAxis][l]>0 && PAD_Axis_PosY_Fit[fLastNormalAxis][l]>0) {

						// ** Fit **
						padVal = PAD_Axis_Max[fLastNormalAxis][l];
						if(padVal>min) {

							X = PAD_Axis_PosX_Fit[fLastNormalAxis][l];
							Y = PAD_Axis_PosY_Fit[fLastNormalAxis][l];

							A+=padVal*(X-Xg)*(X-Xg); //X^2
							B+=padVal*(X-Xg)*(Y-Yg); //YX
							C+=padVal*(Y-Yg)*(Y-Yg); //X^2


						}

					}
				}
			}

		}

		//  -----
		if(opt.Contains("chi2")) {
			//  Double_t A, B, C, UEV, Q, X, Xg, Y, Yg;
			A=B=C=UEV=Q=X=Y=Xg=Yg=0.;

			for (Row=Rmin;Row<=Rmax;Row++) {
				for (Col=Cmin;Col<=Cmax;Col++) {
					if(!IsInside(gFitRegion,Col,Row)) continue;
					padVal = fArray[GetBin(Col-1,Row-1)];
					if(padVal>min) {
						Q+=padVal;
						Xg+=GetX(Col,Row)*padVal;
						Yg+=GetY(Col,Row)*padVal;
						//	std::cerr << Col << " " <<  Row << " " << GetX(Col,Row) << " " << GetY(Col,Row) << " " << padVal << std::endl ;
					}
				}
			}
			if(opt.Contains("origin")) {
				Xg = (x0*Q*fOriginWeigh + Xg)/(Q*(1.+fOriginWeigh)); // modified center of gravity X position
				Yg = (y0*Q*fOriginWeigh + Yg)/(Q*(1.+fOriginWeigh)); // modified center of gravity Y position
			} else {
				Xg/=Q; // center of gravity X position
				Yg/=Q; // center of gravity Y position
			}

			for (Row=Rmin;Row<=Rmax;Row++) {
				for(Col=Cmin;Col<=Cmax;Col++) {
					if(!IsInside(gFitRegion,Col,Row)) continue;
					padVal = fArray[GetBin(Col-1,Row-1)];
					if(padVal>min) {
						X = GetX(Col,Row);
						Y = GetY(Col,Row);
						A+=padVal*(X-Xg)*(X-Xg); //X^2
						B+=padVal*(X-Xg)*(Y-Yg); //YX
						C+=padVal*(Y-Yg)*(Y-Yg); //X^2
					}
				}
			}

		}

		UEV=0.5*(A+C+sqrt((A+C)*(A+C)-4*(A*C-B*B))); // valeur propre de la matrice regression
		Double_t a=B/(UEV-C); // coefficient directeur
		Double_t b=Yg-a*Xg; // ordonnée a l origine Y = aX +b


		nTrack++; // Yahoo !! One more track found !
		if(opt.Contains("sech")) {
			fNormalTrackAxis[nTrack] = fLastNormalAxis + 1;
		} else {
			if(a==0) {
				fNormalTrackAxis[nTrack] = 1; // 2 can be Ok too anyway // TODO : fix this ?
			} else {
				Double_t _min_angle_tmp = 2*(TMath::TwoPi());
				for(int i=0;i<3;i++) {
					if(TMath::Abs(Axis_Angle[i] - TMath::ATan(-1./a /*= normal coef*/))<_min_angle_tmp) {
						fNormalTrackAxis[nTrack] = i+1;
						_min_angle_tmp = TMath::Abs(Axis_Angle[i] - TMath::ATan(-1./a));
					};
				}
			}
		}
		sprintf(_id_,"traj_%d",nTrack);

		TF1 *f = new TF1(_id_,"[1]*x+[0]",
				(GetXaxis()->GetXmin()-.5)*fHoneyCombSize*TMath::Sqrt(3),
				(GetXaxis()->GetXmax()+.5)*fHoneyCombSize*TMath::Sqrt(3));
		f->SetParameter(0,b);
		f->SetParameter(1,a);
		fFunctions->Add(f);

		if(gFitRegion!=NULL) {
			sprintf(_id_,"gcut_%d",nTrack);
			gFitRegion->SetName(_id_);
			gFitRegion->SetTitle(_id_);
			fGCuts->Add(gFitRegion);
		}

		std::cerr << "## " << _id_ << ":\t" << a << " " << b << " " << f->Eval(fHoneyCombSize*3) << std::endl;

		// Now we display on screen

		Double_t Cd0 = GetXaxis()->GetNbins(), Cd1 = 0.;
		Double_t Rd0 = 0. , Rd1 = 0.;
		Double_t Ctmp = 0, Rtmp =0.;
		Double_t Xtmp = -100., Ytmp=100.;

		if(a != 0) {
			for(Int_t i=1;i<=TMath::Max(GetXaxis()->GetNbins(),GetYaxis()->GetNbins());i+=1) {
				Xtmp = GetX(i,i);
				Ytmp = a*Xtmp+b;

				GetCRdisp_XY(Xtmp,Ytmp,Ctmp,Rtmp);

				if(Ctmp>0 && Ctmp<=GetXaxis()->GetNbins() &&
						Rtmp>0 && Rtmp<=GetYaxis()->GetNbins() ) {
					if(Cd0>Ctmp) {
						Cd0 = Ctmp;
						Rd0 = Rtmp;
					}
					if(Cd1<Ctmp) {
						Cd1 = Ctmp;
						Rd1 = Rtmp;
					}
				}
			}
		}

		std::cerr << " -- " << std::endl;
		std::cerr << a << " " << b << std::endl;
		std::cerr << " => " << std::endl;
		std::cerr << Cd0 << " " << Cd1 << " " << Rd0 << " " << Rd1 << std::endl;

		if(Cd1-Cd0 != 0) {
			Double_t na = (Rd1-Rd0)/(Cd1-Cd0);
			Double_t nb = Rd1-na*Cd1;

			//std::cerr << na << " " << nb << std::endl ;
			//    std::cerr << " -- " << std::endl ;
			sprintf(_id_,"disptraj_%d",nTrack);
			//    std::cerr << _id_ <<  std::endl ;
			TF1 *fi = new TF1(_id_,"[1]*x+[0]",GetXaxis()->GetXmin(),GetXaxis()->GetXmax());

			fi->SetLineColor(2);
			fi->SetParameter(0,nb);
			fi->SetParameter(1,na);
			fFunctions->Add(fi);

		}
		return kTRUE;
	}

	//______________________________________________________________________________
	//
	// Find closest collinear axis to a give function coefficient
	//______________________________________________________________________________
	//

Int_t TMayaHisto::FindClosestAxis(Double_t a) {
	Double_t _min_angle_tmp = 2* (TMath ::TwoPi());
	Int_t paxis = -1;
	for(int i=0;i<3;i++) {
		if(TMath::Abs(Axis_Angle[i] - TMath::ATan(a))<_min_angle_tmp) {
			paxis = i+1;
			_min_angle_tmp = TMath::Abs(Axis_Angle[i] - TMath::ATan(a));
		};
	}
	std::cout << "# coeficient of " << a << " close to Axis " << paxis << std::endl;
	return paxis;
}

//______________________________________________________________________________
//
// Find closest collinear axis to a give Track ID
//______________________________________________________________________________
//

Int_t TMayaHisto::FindClosestAxis(Int_t ID) {
	TF1 *t = GetTrajectory(ID);
	if (t == NULL)
		return -1;
	std::cout << "# Axis " << ID << " with ";
	return FindClosestAxis((Double_t) t->GetParameter(1));
}

//______________________________________________________________________________
//
// Project pad value on track ID i.e. on the most collinear axis, following natural direction
//______________________________________________________________________________
//

TH1F* TMayaHisto::ProjectOnTrack(Int_t ID, Option_t *o, Double_t xo,
		Double_t yo) {
	// Option:
	// +X -> xsens: sens in x (default previous value) become kTRUE
	// +Y -> ysens: sens in y (default previous value) become kTRUE
	// -X -> xsens: sens in x (default previous value) become kFALSE
	// -Y -> ysens: sens in y (default previous value) become kFALSE
	TString opt = o;
	if (opt.Contains("+X"))
		fRangeSensX = kTRUE; // TO BE USED !!
	if (opt.Contains("+Y"))
		fRangeSensY = kTRUE; // TO BE USED !!
	if (opt.Contains("-X"))
		fRangeSensX = kFALSE; // TO BE USED !!
	if (opt.Contains("-Y"))
		fRangeSensY = kFALSE; // TO BE USED !!

	//
	TF1 *t = GetTrajectory(ID);
	if (t == NULL)
		return NULL; //TEMP
	TF1 *td = GetTrajectoryDisp(ID);
	//  if(t==NULL || opt.Contains("refit") ) { // TODO
	//    if(FitTrack("id=")) return NULL ;
	//  }

	// Int_t ax = FindClosestAxis(ID);
	//  if(ax<=0) return NULL ;
	TCutG *g = GetCutforTrack(ID);
	if (g == NULL)
		return NULL;

	Double_t xp, yp;

	Double_t ad = td->GetParameter(1);
	Double_t bd = td->GetParameter(0);
	Double_t cu = (ad * ad) / (1. + ad * ad); // normal unitary vector in disp coord
	Double_t ru = (-ad) / (1. + ad * ad); // normal unitary vector in disp coord

	// Projection histogram
	Int_t Cmin = GetXaxis()->GetNbins() + 1, Cmax = -1;
	Int_t Rmin = GetYaxis()->GetNbins() + 1, Rmax = -1;
	for (Int_t C = 1; C <= GetXaxis()->GetNbins(); C++) {
		for (Int_t R = 1; R <= GetYaxis()->GetNbins(); R++) {
			if (IsInside(g, C, R)) {
				if (C < Cmin) {
					Cmin = C;
				};
				if (R < Rmin) {
					Rmin = R;
				};
				if (C > Cmax) {
					Cmax = C;
				};
				if (R > Rmax) {
					Rmax = R;
				};
			}
		}
	}

	Double_t cdo, rdo;
	if (!opt.Contains("origin")) {
		// origin of // TO BE
		// the track // IMPROVED
		xo = 0;
		yo = t->GetParameter(0);
	}

	GetCRdisp_XY(xo, yo, cdo, rdo);

	Double_t Xmax = GetX(TMath::Min(Cmax + 1, GetXaxis()->GetNbins()),
			TMath::Min(Rmax + 1, GetYaxis()->GetNbins()));
	Double_t Ymax = GetY(TMath::Min(Cmax + 1, GetXaxis()->GetNbins()),
			TMath::Min(Rmax + 1, GetYaxis()->GetNbins()));

	Double_t padVal;

	char l[40];
	sprintf(l, "cproj_%d", ID);
	if (gROOT->Get(l))
		gROOT->Delete(l);

	Double_t xmin = 0;
	Double_t xmax = TMath::Sqrt(TMath::Power(xo - Xmax, 2) + TMath::Power(yo
			- Ymax, 2));
	Int_t nbins = TMath::Floor(TMath::Sqrt(TMath::Power(Cmax - Cmin, 2)
			+ TMath::Power(Rmax - Rmin, 2)));

	TH1F *proj = new TH1F(l, l, nbins, xmin, xmax);

	Double_t dist_min = xmin, dist_max = xmax; // distance from origin

	// **Graphics**
	TLine *pline;
	TPolyMarker *MarkMax = new TPolyMarker();
	// ************


	Double_t dp_disp_min, dp_disp_max;

	Double_t cd, rd, cp, rp, distp;
	Double_t cd_min = xmax, rd_min = xmax;
	Double_t cd_max = xmin, rd_max = xmin;
	Double_t cp_min = xmax, rp_min = xmax;
	Double_t cp_max = xmin, rp_max = xmin;

	//

	for (Int_t C = 1; C <= GetXaxis()->GetNbins(); C++) {
		for (Int_t R = 1; R <= GetYaxis()->GetNbins(); R++) {
			padVal = fArray[GetBin(C - 1, R - 1)];
			if (IsInside(g, C, R) && (padVal > fMinimum)) {

				dp_disp_min = 10* TMath ::Sqrt(Cmax * Cmax + Rmax * Rmax);
				dp_disp_max = -1;

				for (Int_t i = 0; i < GetPAD(C, R)->GetN(); i++) {
					cd = GetPAD(C, R)->GetX()[i];
					rd = GetPAD(C, R)->GetY()[i];
					cp = (-(cd * ru - rd * cu) - bd * cu) / (ad * cu - ru);
					rp = (ad * (rd * cu - cd * ru) - bd * ru) / (ad * cu - ru);
					distp = TMath::Sqrt(TMath::Power(cdo - cp, 2)
							+ TMath::Power(rdo - rp, 2));
					if (distp < dp_disp_min) {
						dp_disp_min = distp;
						cd_min = cd;
						rd_min = rd;
						cp_min = cp;
						rp_min = rp;
						GetXY_CRdisp(cp_min, rp_min, xp, yp);
						dist_min = TMath::Sqrt(TMath::Power(xo - xp, 2)
								+ TMath::Power(yo - yp, 2));
						//	    printf("-- %d) %g %g \n",i,xp,yp);
					}
					if (distp > dp_disp_max) {
						dp_disp_max = distp;
						cd_max = cd;
						rd_max = rd;
						cp_max = cp;
						rp_max = rp;
						GetXY_CRdisp(cp_max, rp_max, xp, yp);
						dist_max = TMath::Sqrt(TMath::Power(xo - xp, 2)
								+ TMath::Power(yo - yp, 2));
						//  printf("++ %d) %g %g \n",i,xp,yp);
					}

					printf("%d) %g %g \n", i, dist_min, dist_max);

				}

				Int_t bin;
				distp = dist_max;
				const Double_t val_spread = dist_max - dist_min;
				Double_t ratio = 0;
				if (val_spread > 0) {
					bin = proj->FindBin(distp, 0., 0.);
					while (distp > dist_min) {

						ratio = (distp - proj->GetBinLowEdge(bin))
								/ (val_spread);
						std::cout << distp << " > " << dist_min << std::endl;
						proj->AddBinContent(bin, padVal * ratio);
						bin--;
						distp = TMath::Max(dist_min, proj->GetBinLowEdge(bin));

					}
				}

				// **Graphics **
				cd = GetCdisp(C, R);
				rd = GetRdisp(C, R);
				cp = (-(cd * ru - rd * cu) - bd * cu) / (ad * cu - ru); //seems Ok -> checked outside ROOT
				rp = (ad * (rd * cu - cd * ru) - bd * ru) / (ad * cu - ru); //seems Ok  -> checked outside ROOT
				pline = new TLine(cd, rd, cp, rp);
				pline->SetLineColor(gStyle->GetCanvasColor());
				fFunctions->Add(pline);
				MarkMax->SetNextPoint(cd, rd);
				MarkMax->SetNextPoint(cp, rp);

				// ** Graphics **

            // remove compilation warnings
            cd_min *= 1; cd_max *= 1;
            rd_min *= 1; rd_max *= 1;

				// pline = new TLine(cd_min,rd_min,cp_min,rp_min) ;
				// pline->SetLineColor(3);//gStyle->GetCanvasColor());
				// fFunctions->Add(pline);
				// MarkMax->SetNextPoint(cd_min,rd_min);
				// MarkMax->SetNextPoint(cp_min,rp_min);

				// pline = new TLine(cd_max,rd_max,cp_max,rp_max) ;
				// pline->SetLineColor(4);//gStyle->GetCanvasColor());
				// fFunctions->Add(pline);
				// MarkMax->SetNextPoint(cd_max,rd_max);
				// MarkMax->SetNextPoint(cp_max,rp_max);

			}
		}
	}

	// **Graphics **
	MarkMax->SetMarkerStyle(8);
	MarkMax->SetMarkerSize(.5);
	MarkMax->SetMarkerColor(2);//gStyle->GetCanvasColor());
	fFunctions->Add(MarkMax);
	fRanges->Add(proj);
	return proj;
}

//______________________________________________________________________________
//
// Pointer to the function describing track #id
//______________________________________________________________________________
//

TF1* TMayaHisto::GetTrajectory(Int_t ID) {
	char si[32];
	sprintf(si, "%d", ID);
	TString l = TString("traj_") + si;
	return (TF1*) fFunctions->FindObject(l.Data());
}

//______________________________________________________________________________
//
// Pointer to the function describing track #id
//______________________________________________________________________________
//

TF1* TMayaHisto::GetTrajectoryDisp(Int_t ID) {
	char si[32];
	sprintf(si, "%d", ID);
	TString l = TString("disptraj_") + si;
	return (TF1*) fFunctions->FindObject(l.Data());
}

//______________________________________________________________________________
//
// Pointer to the graphical cut used to fit the track ID
//______________________________________________________________________________
//

TCutG* TMayaHisto::GetCutforTrack(Int_t ID) {
	char si[32];
	sprintf(si, "%d", ID);
	TString l = TString("gcut_") + si;
	return (TCutG*) fGCuts->FindObject(l.Data());
}

//______________________________________________________________________________
//
// Pointer to the histogram, result of the charge projection on track ID
//______________________________________________________________________________
//

TH1F* TMayaHisto::GetChargeProjection(Int_t ID) {
	char si[32];
	sprintf(si, "%d", ID);
	TString l = TString("cproj_") + si;
	return (TH1F*) fRanges->FindObject(l.Data());
}

//______________________________________________________________________________
//
// Default range function.
// Reminder: if another function is given,
// the parameter corresponding to the range MUST be named "range" !
//______________________________________________________________________________
//

Double_t TMayaHisto::_dfltRangeFunction_(Double_t *x, Double_t *par) {

	/*.       |                  par[1]
	 par[0]..+----------------. :
	 .       |                 \:
	 .       |                  \
    .       |                  :\  par[2]
	 .       |                  : \
    ________|__________________:__\__________> x[0]

	 Three parameter, one variable
	 x[0] -> x
	 par[0] -> Jump base height
	 par[1] -> Range itself, thus named "range"
	 par[2] -> Bragg peak end slope
	 */
	Double_t f;
	if (par[2] >= 0) {
		par[2] = -TMath::Abs(par[0] / par[1]);
	}
	if (x[0] < (par[1] - TMath::Abs(par[0] / (2. * par[2])))) { // par[1]-TMath::Abs(par[0]/(2.*par[2]) is the abscissa of the drop
		return par[0];
	} else {
		f = par[2] * (x[0] - par[1]) + par[0] / 2.;
		if (f > 0) {
			return f;
		} else {
			return 0;
		}
	}
}

//______________________________________________________________________________
//
// Find range for the track number ID
//______________________________________________________________________________
//

Double_t TMayaHisto::FindRange(Int_t ID, Option_t *o, Double_t xo, Double_t yo) {
	// ID: already find trajectory number
	// Option:
	// +X -> xsens: sens in x (default previous value) become kTRUE
	// +Y -> ysens: sens in y (default previous value) become kTRUE
	// -X -> xsens: sens in x (default previous value) become kFALSE
	// -Y -> ysens: sens in y (default previous value) become kFALSE

	TString opt = o;
	if (opt.Contains("+X"))
		fRangeSensX = kTRUE;
	if (opt.Contains("+Y"))
		fRangeSensY = kTRUE;
	if (opt.Contains("-X"))
		fRangeSensX = kFALSE;
	if (opt.Contains("-Y"))
		fRangeSensY = kFALSE;

	if (GetChargeProjection(ID) == NULL)
		ProjectOnTrack(ID, o, xo, yo);

	if (GetChargeProjection(ID) != NULL) {
		if (GetChargeProjection(ID)->Integral() > 0) {
			TF1 *_fmax;
			TGraphErrors *h = new TGraphErrors(GetChargeProjection(ID));
			Double_t lmax = h->GetXaxis()->GetXmax();
			//      Double_t lmin = h->GetXaxis()->GetXmin() ;
			Double_t emax = GetChargeProjection(ID)->GetMaximum();
			//      Double_t emin = GetChargeProjection(ID)->GetMinimum() ;
			Double_t range = lmax, xmax = lmax;
			Int_t n = (GetChargeProjection(ID)->GetXaxis())->GetNbins();
			if ((n * fSearchDiv) <= 0)
				return -1;
			Double_t dx = (h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin())
					/ (n * fSearchDiv);
			TSpline3 *s = new TSpline3("s", h);
			Double_t dstart = s->Derivative(lmax);
			for (Double_t x = lmax; x > 0; x -= dx) {
				if ((TMath::Sign(dstart, s->Derivative(x)) != dstart)
						&& (h->Eval(x) > emax * fRangeThresRatio)) {
					if ((TMath::Sign(dstart, s->Derivative(x - dx)) != dstart)) {

						xmax = solveBisectD(s, 0, x, x + dx);
						emax = s->Eval(xmax);
						_fmax = new TF1("max", "[0]", h->GetXaxis()->GetXmin(),
								lmax);
						_fmax->SetParameter(0, s->Eval(x));
						((TList*) h->GetListOfFunctions())->Add(_fmax);

						break;
					}
				}
				dstart = s->Derivative(x);
			}

			range = solveBisect(s, emax / 2., xmax, lmax);
			_fmax = new TF1("max", "[0]", h->GetXaxis()->GetXmin(), lmax);
			_fmax->SetParameter(0, s->Eval(range));
			((TList*) h->GetListOfFunctions())->Add(_fmax);

			if (opt.Contains("Fit")) {
				if (strcmp(fRangeFunction->GetName(), "_dfltRangeFunction_")
						== 0) {
					fRangeFunction->SetParameter(fRangeFunction->GetParNumber(
							"base"), emax);
					fRangeFunction->SetParLimits(fRangeFunction->GetParNumber(
							"base"), emax / 2., emax);
					fRangeFunction->SetParameter(fRangeFunction->GetParNumber(
							"slope"), lmax != 0 ? -emax / lmax : 0);
				}
				fRangeFunction->SetParLimits(fRangeFunction->GetParNumber(
						"range"), 0, lmax);
				fRangeFunction->SetParameter(fRangeFunction->GetParNumber(
						"range"), lmax / 2.);
				h->Fit(fRangeFunction, "b");
				range = fRangeFunction->GetParameter("range");
			}
			h->Draw("A*");
			s->Draw("same");

			if (range < lmax && range > 0)
				return range;

			/*
			 h->SetName("g");
			 TFile *f = new TFile("tmp.root","recreate");
			 f->cd();
			 h->Write("g",kOverwrite);
			 f->Close();
			 */
		} else {
			return -1;
		}
	} else {
		return -1;
	}
	return -1;
}

//______________________________________________________________________________
//
// HightLight pads corresponding to a given graphical cut
//______________________________________________________________________________
//
void TMayaHisto::HighLightCut(TCutG*g) {
	Int_t color;
	for (Int_t Row = 1; Row <= GetYaxis()->GetNbins(); Row++) {
		for (Int_t Col = 1; Col <= GetXaxis()->GetNbins(); Col++) {
			color = GetPAD(Col, Row)->GetFillColor();
			if (g == NULL) {
				if (color != GetPAD(Col, Row)->GetLineColor() || color != 0) {
					//	  GetPAD(Col,Row)->SetFillStyle(1000);
					GetPAD(Col, Row)->SetFillColor(color);
				} else {
					GetPAD(Col, Row)->SetFillStyle(0);
				}
			} else if (g != NULL && IsInside(g, Col, Row)) {
				GetPAD(Col, Row)->SetFillStyle(fTCutGPatternStyle);
			}
		}
	}
	gPad->Update();
}

//______________________________________________________________________________
//
// Find x | f(x) = y0 within the [xmin:xmax] range
//______________________________________________________________________________
//

Double_t TMayaHisto::solveBisect(TSpline3 *s, Double_t y0, Double_t xmin,
		Double_t xmax) {
	Double_t xmid = .5* (xmax +xmin), ymin,ymid,ymax;
	ymin = s->Eval(xmin)-y0;
	ymax = s->Eval(xmax)-y0;

	while (TMath::Abs(xmax - xmin) > fRangePrecision) {
		xmid = (xmax + xmin) / 2.;
		ymid = s->Eval(xmid)-y0;
		// std::cout << "}" <<  xmin << " " << xmax << std::endl ;
			if (ymin*ymid < 0) {
				xmax = xmid;
				ymax = ymid;
			} else if (ymid*ymax < 0) {
				xmin = xmid;
				ymin = ymid;
			} else {
				return xmid;
			}
		}
		return xmid;
	}

	//______________________________________________________________________________
	//
	// Find x | f'(x) = y0 within the [xmin:xmax] range
	//______________________________________________________________________________
	//

Double_t TMayaHisto::solveBisectD(TSpline3 *s, Double_t y0, Double_t xmin,
		Double_t xmax) {
	Double_t xmid, ymin, ymid, ymax;
	ymin = s->Derivative(xmin) - y0;
	ymax = s->Derivative(xmax) - y0;
	xmid = (xmax + xmin) / 2.;

	while (TMath::Abs(xmax - xmin) > fRangePrecision) {
		xmid = (xmax + xmin) / 2.;
		ymid = s->Derivative(xmid) - y0;
		//   std::cout << "}" <<  xmin << " " << xmax << std::endl ;
		if (ymin * ymid < 0) {
			xmax = xmid;
			ymax = ymid;
		} else if (ymid * ymax < 0) {
			xmin = xmid;
			ymin = ymid;
		} else {
			return xmid;
		}
	}
	return xmid;
}

//______________________________________________________________________________
/*
 void TMayaHisto::Streamer(TBuffer &R__b)
 {
 // Stream an object of class TMayaHisto.

 if (R__b.IsReading()) {
 UInt_t R__s, R__c;
 Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
 if (R__v > 2) {
 R__b.ReadClassBuffer(TMayaHisto::Class(), this, R__v, R__s, R__c);
 return;
 }
 //====process old versions before automatic schema evolution
 if (R__v < 2) {
 R__b.ReadVersion();
 TH1::Streamer(R__b);
 TArrayF::Streamer(R__b);
 R__b.ReadVersion();
 R__b >> fScalefactor;
 R__b >> fTsumwy;
 R__b >> fTsumwy2;
 R__b >> fTsumwxy;
 } else {
 TH2::Streamer(R__b);
 TArrayF::Streamer(R__b);
 R__b.CheckByteCount(R__s, R__c, TMayaHisto::IsA());
 }
 //====end of old versions

 } else {
 R__b.WriteClassBuffer(TMayaHisto::Class(),this);
 }

 }
 */
//______________________________________________________________________________
TMayaHisto& TMayaHisto::operator=(const TMayaHisto &h1) {
	// Operator =

	if (this != &h1)
		((TMayaHisto&) h1).Copy(*this);
	return *this;
}

//______________________________________________________________________________
TMayaHisto operator*(Float_t c1, TMayaHisto &h1) {
	// Operator *

	TMayaHisto hnew = h1;
	hnew.Scale(c1);
	hnew.SetDirectory(0);
	return hnew;
}

//______________________________________________________________________________
TMayaHisto operator*(TMayaHisto &h1, Float_t c1) {
	// Operator *

	TMayaHisto hnew = h1;
	hnew.Scale(c1);
	hnew.SetDirectory(0);
	return hnew;
}

//______________________________________________________________________________
TMayaHisto operator+(TMayaHisto &h1, TMayaHisto &h2) {
	// Operator +

	TMayaHisto hnew = h1;
	hnew.Add(&h2, 1);
	hnew.SetDirectory(0);
	return hnew;
}

//______________________________________________________________________________
TMayaHisto operator-(TMayaHisto &h1, TMayaHisto &h2) {
	// Operator -

	TMayaHisto hnew = h1;
	hnew.Add(&h2, -1);
	hnew.SetDirectory(0);
	return hnew;
}

//______________________________________________________________________________
TMayaHisto operator*(TMayaHisto &h1, TMayaHisto &h2) {
	// Operator *

	TMayaHisto hnew = h1;
	hnew.Multiply(&h2);
	hnew.SetDirectory(0);
	return hnew;
}

//______________________________________________________________________________
TMayaHisto operator/(TMayaHisto &h1, TMayaHisto &h2) {
	// Operator /

	TMayaHisto hnew = h1;
	hnew.Divide(&h2);
	hnew.SetDirectory(0);
	return hnew;
}

// Local IspellDict: english

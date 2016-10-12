#include "TStyle.h"
#include "TColor.h"
void Style_nptool(){
  //-----------------------------------
  // standardised nptool root style
  // produce minimalist publication ready
  // outlook with nptool color scheme
  //-----------------------------------
  TStyle* style = new TStyle("nptool", "style");
  style->SetTitlePS("nptool");

  //General 
  style->SetFrameFillColor(kWhite);	
  style->SetFrameBorderMode(0);	
  style->SetFrameLineColor(kWhite);
  style->SetStatColor(kBlack);	
  style->SetTitleFillColor(kWhite);  
  style->SetTitleColor(kBlack,"pad");
  style->SetTitleBorderSize(0);
  style->SetTextColor(kBlack);

  // Canvas
  style->SetCanvasPreferGL(true);
  style->SetCanvasColor(kWhite);	
  style->SetCanvasDefH(800);
  style->SetCanvasDefW(800);
  style->SetCanvasBorderMode(0);	
  
  // Pad
  style->SetPadBottomMargin(0.10);
  style->SetPadLeftMargin(0.10);
  style->SetPadTopMargin(0.15);
  style->SetPadRightMargin(0.15);
  style->SetPadBorderMode(0);	
  style->SetPadBorderSize(1);
  style->SetPadColor(kWhite);		
  style->SetPadTickX(1);
  style->SetPadTickY(1);

  // Disable stat, title and fit  
  style->SetOptStat(0);
  style->SetOptTitle(0);
  style->SetOptFit(0);		

  // x axis
  style->SetAxisColor(kBlack,"X");
  style->SetLabelColor(kBlack,"X");
  style->SetTitleXSize(0.04);     
  style->SetTitleXOffset(1.0);     
  style->SetLabelOffset(0.1,"X");   
  style->SetLabelSize(0.04,"X");
  style->SetLabelOffset(0.004,"X");

  // y axis
  style->SetTitleYSize(0.04);  
  style->SetTitleYOffset(1.1);  
  style->SetLabelOffset(0.1,"Y");
  style->SetLabelSize(0.04,"Y");
  style->SetLabelOffset(0.004,"Y");
  style->SetAxisColor(kBlack,"Y");
  style->SetLabelColor(kBlack,"Y");

  // z axis
  style->SetAxisColor(kBlack,"Z");
  style->SetLabelSize(0.04,"Z");
  style->SetLabelOffset(0.004,"Z");
  style->SetLabelColor(kBlack,"Z");

  // Histogramm
  style->SetHistLineColor(kAzure+7);
  style->SetHistFillStyle(1001);
  style->SetHistFillColor(kAzure+7);
    
  // Graph, Line and Marker
  style->SetMarkerStyle(20);
  style->SetMarkerColor(kOrange+7);
  style->SetLineColor(kOrange+7);
  style->SetLineWidth(1);
  style->SetFuncColor(kOrange+7);
  style->SetFuncWidth(2);

  // Create the color gradiant 
  const UInt_t Number = 2;
  Double_t Red[Number]    = { 0,0   };
  Double_t Green[Number]  = { 0,0.8 };
  Double_t Blue[Number]   = { 0,1.00 };

  Double_t Length[Number] = { 0,1.00 };
  Int_t nb=255;
  TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
  style->SetNumberContours(99);
}


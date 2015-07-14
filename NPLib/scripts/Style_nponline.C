#include "TStyle.h"
#include "TColor.h"
void Style_nponline(){
  //-----------------------------------
  // standardised nptool root style
  // produce minimalist publication ready
  // outlook with nptool color scheme
  //-----------------------------------
  TStyle* style_nponline = new TStyle("nponline", "style_nponline");

  //General 
  style_nponline->SetFrameFillColor(kGray+3);	
  style_nponline->SetFrameBorderMode(0);	
  style_nponline->SetFrameLineColor(kGray+3);
  style_nponline->SetStatColor(kGray+3);	
  style_nponline->SetFillColor(kGray+3); 
  style_nponline->SetTitleFillColor(kGray+3);  
  style_nponline->SetTitleColor(kWhite,"pad");
  style_nponline->SetTitleBorderSize(0);
  style_nponline->SetTextColor(kWhite);


  // Canvas
  style_nponline->SetCanvasColor(kGray+3);	
  style_nponline->SetCanvasDefH(800);
  style_nponline->SetCanvasDefW(800);
  style_nponline->SetCanvasBorderMode(0);	
  
  // Pad
  style_nponline->SetPadBottomMargin(0.10);
  style_nponline->SetPadLeftMargin(0.15);
  style_nponline->SetPadTopMargin(0.10);
  style_nponline->SetPadRightMargin(0.15);
  style_nponline->SetPadBorderMode(0);	
  style_nponline->SetPadBorderSize(1);
  style_nponline->SetPadColor(kGray+3);		
  style_nponline->SetPadTickX(1);
  style_nponline->SetPadTickY(1);

  // Desable stat title and fit  
  style_nponline->SetOptStat(0);
  style_nponline->SetOptTitle(1);
  style_nponline->SetOptFit(1);		

  // x axis
  style_nponline->SetAxisColor(kWhite,"X");
  style_nponline->SetLabelColor(kWhite,"X");
  style_nponline->SetTitleXSize(0.06);     
  style_nponline->SetTitleXOffset(1.0);     
  style_nponline->SetLabelOffset(0.1,"X");   
  style_nponline->SetLabelSize(0.06,"X");
  style_nponline->SetLabelOffset(0.006,"X");

  // y axis
  style_nponline->SetTitleYSize(0.06);  
  style_nponline->SetTitleYOffset(1.1);  
  style_nponline->SetLabelOffset(0.1,"Y");
  style_nponline->SetLabelSize(0.06,"Y");
  style_nponline->SetLabelOffset(0.006,"Y");
  style_nponline->SetAxisColor(kWhite,"Y");
  style_nponline->SetLabelColor(kWhite,"Y");

  // z axis
  style_nponline->SetAxisColor(kWhite,"Z");
  style_nponline->SetLabelSize(0.06,"Z");
  style_nponline->SetLabelOffset(0.006,"Z");
  style_nponline->SetLabelColor(kWhite,"Z");


  // Histogramm
  style_nponline->SetHistLineColor(kAzure+7);
  style_nponline->SetHistFillStyle(1001);
  style_nponline->SetHistFillColor(kAzure+7);
  style_nponline->SetMarkerColor(kOrange+7);
  style_nponline->SetMarkerStyle(20);
  style_nponline->SetLineColor(kOrange+7);
  style_nponline->SetLineWidth(1);
  style_nponline->SetFuncColor(kOrange+7);
  style_nponline->SetFuncWidth(2);

  style_nponline->SetTitlePS("nptool");

  const UInt_t Number = 4;
  Double_t Red[Number]    = { 0,0.22,  0.5*0,     0   };
  Double_t Green[Number]  = { 0,0.22,  0.5*0.8,   0.8 };
  Double_t Blue[Number]   = { 0,0.22,  0.5*1.00,  1.00 };

  Double_t Length[Number] = { 0, 0.5,0.5,1.00 };
  Int_t nb=255;
  TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
  style_nponline->SetNumberContours(99);
}

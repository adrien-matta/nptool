#include "TStyle.h"
#include "TColor.h"
void Style_nponline(){
  //-----------------------------------
  // standardised nptool root style
  // produce minimalist publication ready
  // outlook with nptool color scheme
  //-----------------------------------
  TStyle* style = new TStyle("nponline", "style");

  //General 
  style->SetFrameFillColor(kGray+3);	
  style->SetFrameBorderMode(0);	
  style->SetFrameLineColor(kGray+3);
  style->SetStatColor(kWhite);	
  style->SetFillColor(kGray+3); 
  style->SetTitleFillColor(kGray+3);  
  style->SetTitleColor(kWhite,"pad");
  style->SetTitleBorderSize(0);
  style->SetTextColor(kWhite);

style->SetFrameLineColor(kGray+3);
style->SetFrameFillColor(kGray+3); 
  // Canvas
  style->SetCanvasColor(kGray+3);	
  style->SetCanvasDefH(800);
  style->SetCanvasDefW(800);
  style->SetCanvasBorderMode(0);	
  
  // Pad
  style->SetPadBottomMargin(0.10);
  style->SetPadLeftMargin(0.15);
  style->SetPadTopMargin(0.10);
  style->SetPadRightMargin(0.15);
  style->SetPadBorderMode(1);	
  style->SetPadBorderSize(1);
  style->SetPadColor(kGray+3);		
  style->SetPadTickX(1);
  style->SetPadTickY(1);

  // Desable stat title and fit  
  style->SetOptStat(0);
  style->SetOptTitle(1);
  style->SetOptFit(1);		

  // x axis
  style->SetAxisColor(kWhite,"X");
  style->SetLabelColor(kWhite,"X");
  style->SetTitleXSize(0.06);     
  style->SetTitleXOffset(1.0);     
  style->SetLabelOffset(0.1,"X");   
  style->SetLabelSize(0.06,"X");
  style->SetLabelOffset(0.006,"X");

  // y axis
  style->SetTitleYSize(0.06);  
  style->SetTitleYOffset(1.1);  
  style->SetLabelOffset(0.1,"Y");
  style->SetLabelSize(0.06,"Y");
  style->SetLabelOffset(0.006,"Y");
  style->SetAxisColor(kWhite,"Y");
  style->SetLabelColor(kWhite,"Y");

  // z axis
  style->SetAxisColor(kWhite,"Z");
  style->SetLabelSize(0.06,"Z");
  style->SetLabelOffset(0.006,"Z");
  style->SetLabelColor(kWhite,"Z");


  // Histogramm
  style->SetHistLineColor(kAzure+7);
  style->SetHistFillStyle(1001);
  style->SetHistFillColor(kAzure+7);
  style->SetMarkerColor(kOrange+7);
  style->SetMarkerStyle(20);
  style->SetLineColor(kOrange+7);
  style->SetLineWidth(1);
  style->SetFuncColor(kOrange+7);
  style->SetFuncWidth(2);

  style->SetTitlePS("nptool");

/*  const UInt_t Number = 2;
  Double_t Red[Number]    = { 0,0   };
  Double_t Green[Number]  = { 0,0.8 };
  Double_t Blue[Number]   = { 0,1.00 };

  Double_t Length[Number] = { 0,1.00 };
  Int_t nb=255;
  TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
*/
 style->SetNumberContours(99);
 style->SetPalette();
}

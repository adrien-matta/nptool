#include "NPOnline.h"
#include <iostream>

// Root
#include "TRoot.h"
#include "TColor.h"
#include "TSystem.h"
#include "TString.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TASImage.h"
#include "TMessage.h"
ClassImp(NPL::NPOnline);
////////////////////////////////////////////////////////////////////////////////
NPL::NPOnline::NPOnline(){
  m_Sock = 0;
  TString NPLPath = gSystem->Getenv("NPTOOL");
  gROOT->ProcessLine(Form(".x %s/NPLib/scripts/NPToolLogon.C+", NPLPath.Data()));
  gROOT->SetStyle("nptool");
  
  m_BgColor = gROOT->GetColor(kGray+3)->GetPixel();
  m_FgColor = gROOT->GetColor(kAzure+7)->GetPixel();

  m_Main = new TGMainFrame(0,0,0);
  m_Main->SetCleanup(kDeepCleanup);
  m_Main->SetBackgroundColor(m_BgColor);
  m_Main->SetForegroundColor(m_FgColor);


  m_Tab= new TGTab(m_Main,100,100);
  m_Tab->SetBackgroundColor(m_BgColor);
  m_Tab->SetForegroundColor(m_FgColor);
  m_Tab->ChangeSubframesBackground(m_BgColor);  
  AddTab();
  
  // Create a horizonal frame containing two text buttons
  m_Lhorz = new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 30);
  m_Lresize = new TGLayoutHints(kLHintsExpandX|kLHintsExpandY, 0, 0, 0, 30);
  m_Lbut = new TGLayoutHints(kLHintsCenterY, 10, 10, 0, 0);
  m_Lcan = new TGLayoutHints(kLHintsCenterX|kLHintsCenterY,30,30,30,30);
  m_Horz = new TGHorizontalFrame(m_Main, 100, 100);
  
  m_Horz->SetBackgroundColor(m_BgColor);
  m_Horz->SetForegroundColor(m_FgColor);

  // Create "Connect" and "Quit" buttons
  // Add to horizontal frame
  m_Connect = new TGTextButton(m_Horz, "Connect");
  m_Connect->Connect("Clicked()", "NPL::NPOnline", this, "Connect()");
  m_Connect->SetBackgroundColor(m_BgColor);
  m_Connect->SetForegroundColor(m_FgColor);

  m_Quit = new TGTextButton(m_Horz, "Quit");
  m_Quit->SetCommand("gApplication->Terminate()");
  m_Quit->SetBackgroundColor(m_BgColor);
  m_Quit->SetForegroundColor(m_FgColor);

  m_Update = new TGTextButton(m_Horz, "Update");
  m_Update->Connect("Clicked()", "NPL::NPOnline", this, "Update()");
  m_Update->SetBackgroundColor(m_BgColor);
  m_Update->SetForegroundColor(m_FgColor);

  m_Horz->AddFrame(m_Quit, m_Lbut);
  m_Horz->AddFrame(m_Connect, m_Lbut);
  m_Horz->AddFrame(m_Update, m_Lbut);

  m_Main->AddFrame(m_Horz, m_Lhorz);
  m_Main->AddFrame(m_Tab, m_Lresize);

  m_Main->SetWindowName("NPOnline");
  m_Main->MapSubwindows();

  m_Main->Resize(m_Main->GetDefaultSize());
  m_Main->MapWindow();

  m_Main->Layout();
  m_hist=0;
}

////////////////////////////////////////////////////////////////////////////////
NPL::NPOnline::~NPOnline(){
  delete m_Main; 
  delete m_Tab;
  delete m_Lcan;
  delete m_Horz;
  delete m_Lbut;
  delete m_Lhorz;
  delete m_Quit;
  delete m_Connect;
  delete m_Update;
  delete m_Sock;
}

////////////////////////////////////////////////////////////////////////////////
void NPL::NPOnline::Connect(){
  // Connect to SpectraServer
  m_Sock = new TSocket("localhost", 9090);
  if(m_Sock->IsValid()){
    m_Connect->SetState(kButtonDisabled);
    Update();
  }
}
////////////////////////////////////////////////////////////////////////////////
void NPL::NPOnline::Update(){
  if(!m_Sock || !(m_Sock->IsValid())){
    cout << "Spectra server not connected" << endl;
    return;
  }

  TMessage* message;
  m_Sock->Send("RequestSpectra");

  if(m_Sock->Recv(message)<=0){
    cout << "Spectra request failed " << endl;
    return;
  }
  
  m_CanvasList = (TList*) message->ReadObject(message->GetClass());
  
  
  int current_tab = m_Tab->GetCurrent();
  int tabs =  m_Tab->GetNumberOfTabs();
  for(int i = tabs-1 ; i != -1 ; i--){
    m_Tab->RemoveTab(i,false);
  }
 
  for(TCanvas* c = (TCanvas*) m_CanvasList->First() ; c !=0 ; c = (TCanvas*) m_CanvasList->After(c)){
    AddTab(c->GetName(),c);
  }
  if( m_Tab->GetNumberOfTabs()==0)
    AddTab();
 
  m_Main->MapSubwindows();
  m_Main->MapWindow();
  m_Main->Layout();
  m_Tab->SetTab(current_tab);
}


////////////////////////////////////////////////////////////////////////////////
void NPL::NPOnline::AddTab(std::string name,TCanvas* c){
  TGCompositeFrame* tf; 
  TRootEmbeddedCanvas* canvas;
  if(name=="default"){
   tf = m_Tab->AddTab("NPOnline");
  
   canvas= new TRootEmbeddedCanvas("Canvas",tf,1600,800,!kSunkenFrame); 
   canvas->GetCanvas()->SetMargin(0,0,0,0);
   canvas->GetCanvas()->SetFillColor(kGray+3);
   TImage* logo = TASImage::Open("nptoolLogo.png");
   logo->SetConstRatio(true);
   logo->Draw("");
  }
  
  else{
    tf = m_Tab->AddTab(name.c_str());
    canvas= new TRootEmbeddedCanvas("Canvas",tf,1600,800,!kSunkenFrame); 
    if(c)
      canvas->AdoptCanvas(c);
  }
  tf->SetBackgroundColor(m_BgColor);
  tf->SetForegroundColor(m_FgColor);
  tf->AddFrame(canvas,m_Lresize);

}

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

  MakeGui();

  m_Connect->Connect("Clicked()", "NPL::NPOnline", this, "Connect()");
  m_Quit->SetCommand("gApplication->Terminate()");
  m_Update->Connect("Clicked()", "NPL::NPOnline", this, "Update()");
}

////////////////////////////////////////////////////////////////////////////////
void NPL::NPOnline::MakeGui(){
  m_BgColor = gROOT->GetColor(kGray+3)->GetPixel();
  m_FgColor = gROOT->GetColor(kAzure+7)->GetPixel();

  // main frame
  m_Main = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
  m_Main->SetName("nponline");
  m_Main->SetBackgroundColor(m_BgColor);
  m_Main->SetForegroundColor(m_FgColor);

  m_Main->SetLayoutBroken(kTRUE);

  string NPLPath = gSystem->Getenv("NPTOOL");  
  string path_quit = NPLPath+"/NPLib/Core/icons/power.xpm";
  m_Quit = new TGPictureButton(m_Main,gClient->GetPicture(path_quit.c_str()),-1,TGPictureButton::GetDefaultGC()(),kChildFrame);
  m_Quit->SetBackgroundColor(m_BgColor);

  m_Main->AddFrame(m_Quit, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Quit->MoveResize(10,10,32,32);
  
  string path_update = NPLPath+"/NPLib/Core/icons/download.xpm";
  m_Update = new TGPictureButton(m_Main,gClient->GetPicture(path_update.c_str()),-1,TGPictureButton::GetDefaultGC()(),kChildFrame);
  m_Update->SetBackgroundColor(m_BgColor);
  m_Main->AddFrame(m_Update, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Update->MoveResize(45,10,32,32);
 
  string path_connect = NPLPath+"/NPLib/Core/icons/plugin.xpm";
  m_Connect = new TGPictureButton(m_Main,gClient->GetPicture(path_connect.c_str()),-1,TGPictureButton::GetDefaultGC()(),kChildFrame);
  m_Connect->SetBackgroundColor(m_BgColor);

  m_Main->AddFrame(m_Connect, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Connect->MoveResize(80,10,32,32);

  TGFont* ufont;         // will reflect user font changes
  ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

  TGGC  * uGC;           // will reflect user GC changes
  // graphics context changes
  GCValues_t valress;
  valress.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
  gClient->GetColorByName("#000000",valress.fForeground);
  gClient->GetColorByName("#e7e7e7",valress.fBackground);
  valress.fFillStyle = kFillSolid;
  valress.fFont = ufont->GetFontHandle();
  valress.fGraphicsExposures = kFALSE;
  uGC = gClient->GetGC(&valress, kTRUE);
  m_Address = new TGTextEntry(m_Main, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
  m_Address->SetMaxLength(4096);
  m_Address->SetAlignment(kTextLeft);
  m_Address->SetText("localhost");
  m_Address->Resize(200,m_Address->GetDefaultHeight());
  m_Main->AddFrame(m_Address, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Address->MoveResize(120,10,200,20);

  m_Port = new TGNumberEntry(m_Main, (Double_t) 9090,9,-1,(TGNumberFormat::EStyle) 5);
  m_Port->SetName("m_Port");
  m_Main->AddFrame(m_Port, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
  m_Port->MoveResize(330,10,80,30);

  // canvas widget
  TGCanvas* m_ListCanvas = new TGCanvas(m_Main,120,500);
  m_ListCanvas->SetName("m_ListCanvas");

  // canvas viewport
  TGViewPort* fViewPort669 = m_ListCanvas->GetViewPort();

  // list tree
  m_CanvasListTree = new CanvasList(m_Main,m_ListCanvas);
  m_ListTree = m_CanvasListTree->GetListTree();

  fViewPort669->AddFrame(m_ListTree);
  m_ListTree->SetLayoutManager(new TGHorizontalLayout(m_ListTree));
  m_ListTree->MapSubwindows();

  m_ListCanvas->SetContainer(m_ListTree);
  m_ListCanvas->MapSubwindows();
  m_Main->AddFrame(m_ListCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_ListCanvas->MoveResize(10,50,120,500);

  // tab widget
  m_Tab = new TGTab(m_Main,700,500);
  m_Tab->Resize(m_Tab->GetDefaultSize());
  m_Main->AddFrame(m_Tab, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
  m_Tab->MoveResize(144,50,700,500);
  m_CanvasListTree->SetTab(m_Tab);
  //AddTab();
  m_Main->SetMWMHints(kMWMDecorAll,kMWMFuncAll,kMWMInputModeless);
  m_Main->MapSubwindows();

  m_Main->Resize(m_Main->GetDefaultSize());
  m_Main->MapWindow();
  m_Main->Resize(900,600);
}

////////////////////////////////////////////////////////////////////////////////
NPL::NPOnline::~NPOnline(){
  delete m_Main; 
  delete m_ListCanvas;
  delete m_ListTree;
  delete m_Tab;
  delete m_Quit;
  delete m_Connect;
  delete m_Update;
  delete m_Sock;
  delete m_Port;
  delete m_Address;
}

////////////////////////////////////////////////////////////////////////////////
void NPL::NPOnline::Connect(){
  // Connect to SpectraServer
  m_Sock = new TSocket(m_Address->GetDisplayText(),(Int_t) m_Port->GetNumber());
  if(m_Sock->IsValid()){
    m_Connect->SetState(kButtonDisabled);
    Update();
  }
  else{
    cout << "Connection to " << m_Address->GetDisplayText() << " " <<(Int_t) m_Port->GetNumber() << " Failed" << endl;
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
  
  m_CanvasListTree->Clear();
  m_CanvasList = (TList*) message->ReadObject(message->GetClass());

  TGCompositeFrame* tf; 
  TRootEmbeddedCanvas* canvas;
  for(TCanvas* c = (TCanvas*) m_CanvasList->First() ; c !=0 ; c = (TCanvas*) m_CanvasList->After(c)){
    m_CanvasListTree->AddItem(c);

    TGCompositeFrame*  tab =  m_Tab->GetTabContainer(c->GetName());
    if(tab){
      tab->RemoveAll();
      TRootEmbeddedCanvas* canvas = new TRootEmbeddedCanvas("Canvas",tab,700,490,!kSunkenFrame); 
      canvas->AdoptCanvas(c);
      tab->AddFrame(canvas);
      tab->SetLayoutManager(new TGVerticalLayout(tab));
    }
  
  }

  m_Main->MapSubwindows();
  m_Main->MapWindow();
  m_Main->Layout();
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
NPL::CanvasList::CanvasList(TGMainFrame* main, TGCanvas* parent){
  m_popen = gClient->GetPicture("ofolder_t.xpm");
  m_pclose = gClient->GetPicture("folder_t.xpm");
  m_ListTree = new TGListTree(parent,kHorizontalFrame);
  m_ListTree->Connect("DoubleClicked(TGListTreeItem*,Int_t)","NPL::CanvasList",this,"OnDoubleClick(TGListTreeItem*,Int_t)");
  m_Main = main;
}
////////////////////////////////////////////////////////////////////////////////
NPL::CanvasList::~CanvasList(){
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::OnDoubleClick(TGListTreeItem* item, Int_t btn){
  AddTab(item->GetText(),m_Canvas[item->GetText()]);
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::AddItem(TCanvas* c){
  TGListTreeItem*  item  = m_ListTree->AddItem(NULL,c->GetName());
  item->SetPictures(m_popen, m_pclose);
  m_Canvas[c->GetName()]=c;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::Clear(){
  m_Canvas.clear();
  TGListTreeItem* item =  m_ListTree->GetFirstItem() ;
  while(item){
   m_ListTree->DeleteItem(item);
  item =  m_ListTree->GetFirstItem() ;
  }
}
////////////////////////////////////////////////////////////////////////////////
TGListTree* NPL::CanvasList::GetListTree(){
  return m_ListTree;
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::AddTab(std::string name,TCanvas* c){
  // If the tab exist, activate
  if(m_Tab->GetTabTab(name.c_str())){
      m_Tab->SetTab(name.c_str());
    return;
  }

  TGCompositeFrame* tf = m_Tab->AddTab(name.c_str());
  TRootEmbeddedCanvas* canvas = new TRootEmbeddedCanvas("Canvas",tf,700,490,!kSunkenFrame); 
     
  if(c)
    canvas->AdoptCanvas(c);
 
  //  tf->SetBackgroundColor(m_BgColor);
  //  tf->SetForegroundColor(m_FgColor);
  tf->AddFrame(canvas);
  tf->SetLayoutManager(new TGVerticalLayout(tf));
  m_Tab->Resize(m_Tab->GetDefaultSize());
  m_Tab->MoveResize(144,50,700,500);
  m_Tab->SetTab(name.c_str());
  m_Main->MapSubwindows();
  m_Main->MapWindow();
  m_Main->Layout();
}
////////////////////////////////////////////////////////////////////////////////
void NPL::CanvasList::SetTab(TGTab* tab){
  m_Tab=tab;
}

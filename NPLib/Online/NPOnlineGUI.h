#ifndef NPONLINEGUI_H
#define NPONLINEGUI_H
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TApplication.h" 
#include "TGLayout.h"
#include "TGButton.h"
#include "TGTab.h"
#include "TGStatusBar.h"
#include "TGComboBox.h"
#include "TRootEmbeddedCanvas.h"
#include "TH1.h"
#include "TSocket.h"
#include "TGListTree.h"
#include "TGTextEntry.h"
#include "TGTextEdit.h"
#include "TGNumberEntry.h"
#include "TTimer.h"
//#include "TGCanvasContainer.h"
#include "RQ_OBJECT.h"
#include "NPSpectraClient.h"
#include "NPElog.h"
#include<map>
using namespace std;

namespace NPL{
   void ExecuteMacro(string name);
  
  class CanvasList {
    RQ_OBJECT("CanvasList")
    protected:
      TGMainFrame* m_Main;
      TGListTree* m_ListTree;
      TRootEmbeddedCanvas* m_EmbeddedCanvas; 
      TGTab* m_Tab;
      map<string,TCanvas*> m_Canvas;
      const TGPicture* m_popen;     
      const TGPicture* m_pclose;   
      const TGPicture* m_pfolder;   

      Pixel_t m_BgColor;
      Pixel_t m_FgColor;
      vector<TGStatusBar*> m_StatusBar;

    public:
      CanvasList(TGMainFrame* main, TGCanvas* parent, TRootEmbeddedCanvas* canvas);
      virtual ~CanvasList();

      // slots
      void OnDoubleClick(TGListTreeItem* item, Int_t btn);

      // Interface with OnlineGUI
      void SetTab(TGTab* tab);
      void AddItem(TCanvas* c,TGListTreeItem* parent=NULL);
      void Clear();

      // Read in the declared Canvas
      void LoadCanvasList();

      // For Status bar
      void SetStatusText(const char* txt,int pi); 
      void EventInfo(int event,int px,int py,TObject* selected);

           TGListTree* GetListTree();
  };

  class OnlineGUI: public TGMainFrame{
    RQ_OBJECT("OnlineGUI")

    public:
      OnlineGUI(NPL::SpectraClient*);
      ~OnlineGUI();
    
    public: // necessarry to kill the app when closing the windows
      void CloseWindow() {gApplication->Terminate(0);}
    
    private:
      NPL::SpectraClient* m_Client;
    
    public:
      void MakeGui();
      void Connect();
      void Update();
      void AutoUpdate();
      void FitCurrent();
      void FitAll();
      void ApplyRangeCurrent();
      void ApplyRangeAll();
      void ResetCurrent();
      void ResetAll();
      void Eloging();
      void SaveAs();

    private: // Server/Client
      TSocket* m_Sock;
      TList*   m_HistoList;

    private: // ELog Stuff
      NPL::Elog m_Elog;
      std::map<std::string,TGTextEntry*> m_ElogAttributes ;
      TGTextEdit* m_ElogEntry; 
    private: // GUI stuff
      // Main window
      TGMainFrame* m_Main;
      // Menu bar
      TGVerticalFrame* m_ButtonBar;
      // Splitted frame for Tree (l) and Tab (r)
      TGHorizontalFrame* m_Split; 

      // left List Tree
      TGCompositeFrame* m_Left;
      // center View port
      TGCompositeFrame* m_Center;
      TRootEmbeddedCanvas* m_EmbeddedCanvas; 

      // right Tool bar
     
      //Nav
      TGCheckButton* m_CheckLogX;
      TGCheckButton* m_CheckLogY;
      TGCheckButton* m_CheckLogZ;
      TGNumberEntry* m_Xmin;
      TGNumberEntry* m_Xmax;
      TGNumberEntry* m_Ymin;
      TGNumberEntry* m_Ymax;
      TGTextButton* m_ApplyRangeCurrent;
      TGTextButton* m_ApplyRangeAll;
      TGTextButton* m_ResetCurrent;
      TGTextButton* m_ResetAll;

      // Fit
      TGCompositeFrame* m_Right;
      TGTextButton* m_FitCurrent;
      TGTextButton* m_FitAll;
      TGCheckButton* m_BackgroundFit;
      TGPictureButton* m_Eloging;
      
      // SaveAs
      TGPictureButton* m_SaveAs;
      TGTextEntry*     m_SaveAsFileName;
      TGCheckButton*   m_SaveAsColor;
      
      // Server tool bar
      TGTab* m_Tab;
      TGPictureButton* m_Quit;
      TGPictureButton* m_Connect;
      TGPictureButton* m_Update;
      TGPictureButton* m_Clock;
      
      TGNumberEntry* m_TimerEntry;
      TTimer* m_Timer;
      TGStatusBar* m_StatusBar;     

      TGTextEntry* m_Address; 
      TGNumberEntry* m_Port; 
      TGListTree* m_ListTree;
      CanvasList* m_CanvasListTree;

      TGCanvas* m_ListCanvas;
    
    private: // Style
      Pixel_t m_BgColor;
      Pixel_t m_FgColor;
      Pixel_t m_TabBgColor;
      Pixel_t m_TabFgColor;

      TH1* m_hist; 

      ClassDef(NPL::OnlineGUI,1);
  };

}
#endif

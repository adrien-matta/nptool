#ifndef NPONLINE_H
#define NPONLINE_H
#include "TGLayout.h"
#include "TGButton.h"
#include "TGTab.h"
#include "TRootEmbeddedCanvas.h"
#include "TH1.h"
#include "TSocket.h"
#include "TGListTree.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
//#include "TGCanvasContainer.h"
#include "RQ_OBJECT.h"
#include<map>
using namespace std;

namespace NPL{
  class CanvasList {
    RQ_OBJECT("CanvasList")
    protected:
      TGMainFrame* m_Main;
      TGListTree* m_ListTree;
      TGTab* m_Tab;
      map<string,TCanvas*> m_Canvas;
      const TGPicture* m_popen;     
      const TGPicture* m_pclose;    
    

    public:
      CanvasList(TGMainFrame* main, TGCanvas* parent);
      virtual ~CanvasList();

      // slots
      void OnDoubleClick(TGListTreeItem* item, Int_t btn);
      
      // Interface with NPOnline
      void SetTab(TGTab* tab);
      void AddItem(TCanvas* c);
      void Clear();
      TGListTree* GetListTree();
      // Add a new Tab to the interface
      void AddTab(std::string name="default",TCanvas* c=0);
  };

  class NPOnline{
    RQ_OBJECT("NPOnline")

    public:
      NPOnline();
      ~NPOnline();

      void MakeGui();
      void Connect();
      void Update();

        private: // Server client
      TSocket* m_Sock;
      TList* m_CanvasList;

    private: // GUI stuff
      TGMainFrame* m_Main;
      TGTab* m_Tab;
      TGPictureButton* m_Quit;
      TGPictureButton* m_Connect;
      TGPictureButton* m_Update;
      TGTextEntry* m_Address; 
      TGNumberEntry* m_Port; 
      TGListTree* m_ListTree;
      CanvasList* m_CanvasListTree;

      TGCanvas* m_ListCanvas;
      
          private: // Style
      Pixel_t m_BgColor;
      Pixel_t m_FgColor;

      TH1* m_hist; 

      ClassDef(NPOnline,1);
  };

}
#endif

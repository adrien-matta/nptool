#include"RSShellModelCollection.h"
#include"RSShellModelState.h"
#include"RSExperimentalCrossSection.h"
void PlotLevel(string name,int offset,bool LR, RS::ShellModelCollection Collection, vector<int> matched);
vector<TPad*> thePad;
double Emin = -0.2;
double Emax = 2.70;
double ScaleMin = 0;
double ScaleMax = 7;
double LabelSize = 0.04;
int nbr = 2;
unsigned int pad =0 ;

TCanvas* c  = new TCanvas() ;
TCanvas* c2 ;
TPad* tPAD;
int currentPad =0 ;

vector<RS::ShellModelState> SingleFit;
vector<RS::ShellModelState> DoubleFit;
vector<double> dErr;
vector<double> sErr;
vector<int>    dorder;
vector<double> dSMs;
vector<double> sSMs;
vector<double> eSM;
vector<string> ExpFileName;


void SM(){
  RS::ShellModelCollection WBP("WBP");
  WBP.LoadCollectionFromNushell("mg29olapc.lsf");
  RS::ShellModelCollection Exp("Exp");
  Exp.LoadCollectionFromSimpleFile("Level.txt");
  
vector<int> matched;
  matched.resize(100,1);
  PlotLevel("WBP",1,1,WBP,matched);
  PlotLevel("Exp",0,0,Exp,matched);

}
////////////////////////////////////////////////////////////////////////////////
void PlotLevel(string name,int offset,bool LR, RS::ShellModelCollection Collection, vector<int> matched){
  c->cd(1);
  if(name!=""){
    TLatex* t = new TLatex((double) offset/nbr+0.25/nbr,0.01,name.c_str());
    t->SetTextSize(LabelSize+0.01);
    t->Draw();
  }

  unsigned int mysize = Collection.GetNumberOfState();
  for(unsigned int i = 0 ; i < mysize ; i++){
    RS::ShellModelState state = Collection.GetState(i);
    if(Collection.GetStatus(i)==1 && state.GetNumberOfOrbital()>0){

      double start, finnish;
      if(LR){
        start = (double)(offset)/nbr+0.05/nbr;
        finnish = (double)(offset+1)/nbr-0.25/nbr;
      }

      else{
        start = (double)(offset+1)/nbr-0.05/nbr;
        finnish = (double)(offset)/nbr+0.25/nbr;
      }

      double line_length = abs(finnish-start);
      double last = start;

      double position = (state.GetEnergy()-Emin)/(Emax-Emin);
      double labelpos ;

      string final_label;
      if(state.GetOrder()!=0){
        if(state.GetParity()>0)
          final_label = Form("%d^{+}_{%d} %s %.2f" ,(int)state.GetJ() ,state.GetOrder(),state.GetOrbitalString(state.GetMainOrbital(),"SM").c_str(),state.GetEnergy());
        else
          final_label = Form("%d^{-}_{%d} %s %.2f" ,(int)state.GetJ() ,state.GetOrder(),state.GetOrbitalString(state.GetMainOrbital(),"SM").c_str(),state.GetEnergy());
      }
      else{
        if(state.GetParity()>0)
          final_label = Form("%d^{+} %s %.2f" ,(int)state.GetJ() ,state.GetOrbitalString(state.GetMainOrbital(),"SM").c_str(),state.GetEnergy());
        else
          final_label = Form("%d^{-} %s %.2f" ,(int)state.GetJ() ,state.GetOrbitalString(state.GetMainOrbital(),"SM").c_str(),state.GetEnergy());
      }


      if(LR)
        labelpos = start+0.8*line_length; 
      else
        labelpos = finnish-0.13*line_length;

      TLatex* l = new TLatex(labelpos,position-0.003,final_label.c_str());
      l->SetTextSize(LabelSize);
      if(matched.size() ==0 || matched[i]==1)
        l->Draw();

      unsigned int orbsize = state.GetNumberOfOrbital();
      // Where the last one stopped
      int thickness = 4;
      for(unsigned int orb = 0 ; orb < orbsize ; orb++){
        TLine* Level;
        double SF = state.GetOrbitalS(orb);
        int color = kBlack;

        string orbital =  state.GetOrbitalString(orb);
        if(orbital.find("s")!=string::npos){
          color = kRed;
        }
        else if(orbital.find("p")!=string::npos){
          color = kBlue;
        }if(orbital.find("d")!=string::npos){
          color = kGreen+2;
        }if(orbital.find("f")!=string::npos){
          color = kOrange+7;
        }if(orbital.find("g")!=string::npos){
          color = kMagenta+2;
        }


        if(LR){
          Level =
            new TLine(last,position,last+(SF*line_length),position);
          last = last+(SF*line_length);
        }

        else{
          Level =
            new TLine(last,position,last-(SF*line_length),position);
          last = last-(SF*line_length);
        }

        Level->SetLineStyle(1);
        Level->SetLineWidth(thickness--);
        Level->SetLineColor(color);
        Level->Draw();
      }
    }
  }

}


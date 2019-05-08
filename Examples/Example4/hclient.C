void Sync();

int m_Port = 9092;
TSocket *m_Sock;
TCanvas* c1;
TH3F* hXYZ;
TH3F* h3D;

TH2F* h2Dxy;
TH2F* h2Dxz;
TH2F* hXY;
TH2F* hXZ;

TList* m_Histo;

vector<double> p0xy;
vector<double> p1xy;
vector<double> p0xz;
vector<double> p1xz;


vector<TF1*> vfxy;
vector<TF1*> vfxz;


int i;

//////////////////////////////////////////////////////
void hclient()
{
    // Open connection to server
    m_Sock = new TSocket("localhost", m_Port);
    
    //hXYZ = new TH3F("hXYZ","hXYZ",128,0,128,128,0,128,512,0,512);
    hXYZ = new TH3F("hXYZ","hXYZ",128,0,128,128,0,128,128,0,128);
    hXYZ->SetStats(0);
    hXYZ->SetTitle(0);
    hXYZ->GetXaxis()->SetTitle("Pad X");
    hXYZ->GetYaxis()->SetTitle("Pad Y");
    hXYZ->GetZaxis()->SetTitle("Time (a.u.)");
    hXYZ->GetXaxis()->CenterTitle();
    hXYZ->GetYaxis()->CenterTitle();
    hXYZ->GetZaxis()->CenterTitle();
    
    hXYZ->GetXaxis()->SetTitleOffset(1.3);
    hXYZ->GetYaxis()->SetTitleOffset(1.6);
    hXYZ->GetZaxis()->SetTitleOffset(1.3);
    
    h2Dxy = new TH2F("h2Dxy","h2Dxy",128,0,128,128,0,128);
    h2Dxy->SetStats(0);
    h2Dxy->SetTitle(0);
    h2Dxy->GetXaxis()->SetTitle("Pad X");
    h2Dxy->GetXaxis()->CenterTitle();
    h2Dxy->GetYaxis()->SetTitle("Pad Y");
    h2Dxy->GetYaxis()->CenterTitle();
    
    h2Dxz = new TH2F("h2Dxz","h2Dxz",128,0,128,128,0,128);
    h2Dxz->SetStats(0);
    h2Dxz->SetTitle(0);
    h2Dxz->GetXaxis()->SetTitle("Pad X");
    h2Dxz->GetXaxis()->CenterTitle();
    h2Dxz->GetYaxis()->SetTitle("Time (a.u.)");
    h2Dxz->GetYaxis()->CenterTitle();
    
    //h3D = new TH3F("h3D","h3D",128,0,128,128,0,128,512,0,512);
    c1 = new TCanvas("c1","c1",1600,600);
    c1->Divide(3,1);
    
    i=0;
    if(m_Sock->IsValid()){
        cout << "/// Successful connection to spectra server ///" << endl;
        while(1)
            Sync();
    }
    
    return;
}

//////////////////////////////////////////////////////
void Sync()
{
    i++;
    if(!m_Sock || !(m_Sock->IsValid())){
        if(m_Sock){
            m_Sock->Close("force");
            delete m_Sock;
            m_Sock = NULL;
        }
        cout << "/// Sync failed: Connection lost ///" << endl;
    }
    
    TMessage* message=NULL;
    m_Sock->Send("RequestHisto");
    cout << "Sending RequestHisto..." << endl;
    if(m_Sock->Recv(message)<=0){
        if(m_Sock){
            m_Sock->Close("force");
            delete m_Sock;
            m_Sock = NULL;
        }
        cout << "/// Sync failed: message return unreadable ///" << endl;
    }
    
    if(message){
        m_Histo = (TList*) message->ReadObject(message->GetClass());
        if(m_Histo){
            cout << "/// Successful sync of histo list ///" << endl;
            vector<TGraph2D*> vg2d;
            vg2d.clear();
            
            vector<TH3F*> vh3;
            vector<TH2F*> vhxy;
            vector<TH2F*> vhxz;
            vector<TGraph2D*> vg3;
            vh3.clear();
            vhxy.clear();
            vhxz.clear();
            
            
            //cout << "+++++++++++++++++++++++++++++" << endl;
            for(int i=0; i<m_Histo->GetSize(); i++){
                //cout << i << " / " << i%2 << endl;
                
                //if((i%2) == 0) vh3.push_back( (TH3F*) m_Histo->At(i));
                if((i%2) == 0) vg3.push_back( (TGraph2D *) m_Histo->At(i));
                
                else vg2d.push_back( (TGraph2D *) m_Histo->At(i) );
                
            }
            
            for(int i=0; i<vg3.size(); i++){
                //h3D = new TH3F("h3D","h3D",128,0,128,128,0,128,512,0,512);
                h3D = new TH3F("h3D","h3D",128,0,128,128,0,128,128,0,128);
                hXY = new TH2F("hXY","hXY",128,0,128,128,0,128);
                hXZ = new TH2F("hXZ","hXZ",128,0,128,128,0,128);
                
                double *X = vg3[i]->GetX();
                double *Y = vg3[i]->GetY();
                double *Z = vg3[i]->GetZ();
                for(int p=0; p<vg3[i]->GetN(); p++){
                    //cout << i << " / " << p << " / " << X[p] << " / " << Y[p] << " / " << Z[p] <<endl;
                    h3D->Fill(X[p],Y[p],Z[p]);
                    hXY->Fill(X[p],Y[p]);
                    hXZ->Fill(X[p],Z[p]);
                }
                vh3.push_back(h3D);
                vhxy.push_back(hXY);
                vhxz.push_back(hXZ);
            }
            
            /////////////////
            // Track in 3D //
            /////////////////
            c1->cd(1);
            hXYZ->Draw();
            for(int i=0; i<vh3.size(); i++){
                vh3[i]->SetMarkerStyle(8);
                vh3[i]->SetMarkerSize(0.7);
                vh3[i]->SetMarkerColor(kAzure+i);
                vh3[i]->Draw("same");
            }
            
            
            p0xy.clear();
            p1xy.clear();
            p0xz.clear();
            p1xz.clear();
            vfxy.clear();
            vfxz.clear();
            for(int i=0; i<vg2d.size(); i++){
                vg2d[i]->SetLineWidth(4);
                vg2d[i]->SetLineColor(kAzure+i);
                vg2d[i]->Draw("tri1 same");
                
                double axy = (vg2d[i]->GetY()[1] - vg2d[i]->GetY()[0]) / (vg2d[i]->GetX()[1] - vg2d[i]->GetX()[0]);
                p1xy.push_back(axy);
                p0xy.push_back( vg2d[i]->GetY()[0] - axy*vg2d[i]->GetX()[0] );
                
                double axz = (vg2d[i]->GetZ()[1] - vg2d[i]->GetZ()[0]) / (vg2d[i]->GetX()[1] - vg2d[i]->GetX()[0]);
                p1xz.push_back(axz);
                p0xz.push_back( vg2d[i]->GetZ()[0] - axz*vg2d[i]->GetX()[0] );
            }
            
            for(unsigned int i=0; i<p0xy.size(); i++){
                TF1* f1xy = new TF1(Form("fxy%d",i+1),"[0]+[1]*x[0]",0,128);
                f1xy->SetParameter(0,p0xy[i]);
                f1xy->SetParameter(1,p1xy[i]);
                f1xy->SetLineColor(2);
                f1xy->SetLineWidth(3);
                
                vfxy.push_back(f1xy);
                
                TF1* f1xz = new TF1(Form("fxz%d",i+1),"[0]+[1]*x[0]",0,128);
                f1xz->SetParameter(0,p0xz[i]);
                f1xz->SetParameter(1,p1xz[i]);
                f1xz->SetLineColor(2);
                f1xz->SetLineWidth(3);
                
                vfxz.push_back(f1xz);
            }
            
            
            ///////////////////////////
            // Track in the XY plan //
            //////////////////////////
            c1->cd(2);
            h2Dxy->Draw();
            for(int i=0; i<vhxy.size(); i++){
                vhxy[i]->Draw("colsame");
            }
            
            for(int i=0; i<vfxy.size(); i++){
                vfxy[i]->Draw("lsame");
            }
            
            
            
            ///////////////////////////
            // Track in the XZ plan //
            //////////////////////////
            c1->cd(3);
            h2Dxz->Draw();
            for(int i=0; i<vhxz.size(); i++){
                vhxz[i]->Draw("colsame");
            }
            
            for(int i=0; i<vfxz.size(); i++){
                vfxz[i]->Draw("lsame");
            }
            
            TString SaveCanvas = Form("png/canvas_%d.png",i);
            //gPad->SetPhi(110);
            gPad->Update();
            //TString MakeGif = Form("gif/canvas_%d.gif+0.1",i);
            //c1->SaveAs(SaveCanvas);
            c1->Print("gif/canvas.gif+70");
            //c1->Update();
            
        }
        else cout << "/// Sync return a NULL spectra list ///" << endl;
        
        /*m_Histo->First()->Draw();
         m_Histo->Last()->Draw("tri1 same");
         gPad->Update();*/
    }
    
    
}

























void Sync();

int m_Port = 9092;
TSocket *m_Sock;
TCanvas* c1;
TH3F* hXYZ;
TH3F* h3D;

TList* m_Histo;

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
    
    //h3D = new TH3F("h3D","h3D",128,0,128,128,0,128,512,0,512);
    c1 = new TCanvas("c1","c1",600,600);
    
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
            vector<TGraph2D*> vg3;
            vh3.clear();
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
                
                double *X = vg3[i]->GetX();
                double *Y = vg3[i]->GetY();
                double *Z = vg3[i]->GetZ();
                for(int p=0; p<vg3[i]->GetN(); p++){
                    //cout << i << " / " << p << " / " << X[p] << " / " << Y[p] << " / " << Z[p] <<endl;
                    h3D->Fill(X[p],Y[p],Z[p]);
                }
                vh3.push_back(h3D);
            }
            
            
            c1->cd();
            hXYZ->Draw();
            for(int i=0; i<vh3.size(); i++){
                vh3[i]->SetMarkerStyle(8);
                vh3[i]->SetMarkerSize(0.7);
                vh3[i]->SetMarkerColor(kAzure+i);
                vh3[i]->Draw("same");
            }
            
            
            for(int i=0; i<vg2d.size(); i++){
                vg2d[i]->SetLineWidth(4);
                vg2d[i]->SetLineColor(kAzure+i);
                vg2d[i]->Draw("tri1 same");
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

























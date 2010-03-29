{
TFile *file0 = TFile::Open("60Fe_AnalysedData.root");
 
 c1 = new TCanvas();
 c1->Divide(2,2);
 
 c1->cd(1)->SetLogz();
 Analysed_Data->Draw("Si_E[0]:Si_T[0]>>h1(120,360,400,500,0,10000)","TelescopeNumber==1 && SiLi_E < 0","colz");
 c1->Update();

 c1->cd(2)->SetLogz();
 Analysed_Data->Draw("Si_E[0]:Si_T[0]>>h2(120,360,400,500,0,10000)","TelescopeNumber==2 && SiLi_E < 0","colz");
 c1->Update();

 c1->cd(3)->SetLogz();
 Analysed_Data->Draw("Si_E[0]:Si_T[0]>>h3(120,360,400,500,0,10000)","TelescopeNumber==3 && SiLi_E < 0","colz");
 c1->Update();

 c1->cd(4)->SetLogz();
 Analysed_Data->Draw("Si_E[0]:Si_T[0]>>h4(120,360,400,500,0,10000)","TelescopeNumber==4 && SiLi_E < 0","colz");
 c1->Update();


 c2 = new TCanvas();
 Analysed_Data->Draw("Si_E[0]:Si_T[0]>>h0(120,360,400,500,0,10000)","TelescopeNumber!=4 && TelescopeNumber!=3","colz");

}

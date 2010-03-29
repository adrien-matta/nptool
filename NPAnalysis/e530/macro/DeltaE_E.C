{
TFile *file0 = TFile::Open("60Fe_AnalysedData.root");
 
 c1 = new TCanvas();
 c1->Divide(2,2);
 
 c1->cd(1)->SetLogz();
 Analysed_Data->Draw("Si_E:SiLi_E>>h1(1000,0,35000,500,0,12000)","TelescopeNumber==1","colz");
 c1->Update();

 c1->cd(2)->SetLogz();
 Analysed_Data->Draw("Si_E:SiLi_E>>h2(1000,0,35000,500,0,12000)","TelescopeNumber==2","colz");
 c1->Update();

 c1->cd(3)->SetLogz();
 Analysed_Data->Draw("Si_E:SiLi_E>>h3(1000,0,35000,500,0,12000)","TelescopeNumber==3","colz");
 c1->Update();

 c1->cd(4)->SetLogz();
 Analysed_Data->Draw("Si_E:SiLi_E>>h4(1000,0,35000,500,0,12000)","TelescopeNumber==4","colz");
 c1->Update();


 c2 = new TCanvas();
 Analysed_Data->Draw("Si_E[0]:SiLi_E[0]>>h0(1000,0,35000,500,0,12000)","TelescopeNumber!=4","colz");

}

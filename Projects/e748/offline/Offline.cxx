void Offline(){  
  gROOT->Reset(); 
  char command[100];
  gROOT->ProcessLine(command);
  gSystem->Load("GUser.so"); //load and compile GUser class 
  GTape *file = new GTape("/data/muviX/must2/acquisition/run/run_0011.dat.16-06-17_01h18m04s");
  //GTape *file = new GTape("/data/muviX/must2/acquisition/run/run_0008.dat.15-06-17_19h23m54s");

  file->Open();
  GUser * a= new GUser(file); // creat user treatement environement
  //GNetServerRoot *serv = new GNetServerRoot(9094, a);
  a->EventInit("exl"); // even initialisation
  //a->SetSpectraMode(1); // Declare all raw parameters as histograms
  a->SetSpectraMode(0); 
  a->SetTTreeMode(3, "./run11.root");
  a->InitUser();
  //serv->StartServer();
  file->Rewind();
  a->DoRun(); // a->DoRun(2000); do treaments on 2000 first events ( 0 = all);
  a->EndUser(); // must be explicitly called , if it needs
  a->SpeSave("histo.root"); // save all declared histogram
  delete (a); // finish
  gROOT->ProcessLine(".q");
}



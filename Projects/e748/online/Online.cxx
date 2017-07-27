void Online(){  
  // specify good include dir!
  gROOT->Reset(); 
  
  char command[100];
  gROOT->ProcessLine(command);
  gSystem->Load("GUser.so"); //load and compile GUser class 

  GNetClientNarval *net = new GNetClientNarval("ganp207"); //
  net->SetPort(10202);
  net->SetBufferSize(65536);

  GUser * a= new GUser(net); // creat user treatement environement
  GNetServerRoot *serv = new GNetServerRoot(9094, a);
  a->EventInit("must2"); // even initialisation
  //a->SetSpectraMode(1); // Declare all raw parameters as histograms
  a->SetSpectraMode(0); 
  a->InitUser();
  serv->StartServer();
  a->DoRun(); // a->DoRun(2000); do treaments on 2000 first events ( 0 = all);
  net->Close();
  a->EndUser(); // must be explicitly called , if it needs
  a->SpeSave("histo.root"); // save all declared histogram
  delete (a); // finish
  gROOT->ProcessLine(".q");
}



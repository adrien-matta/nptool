void Online(){
  // specify good include dir!
 // gROOT->Reset();
  
//  char command[100];
//  gROOT->ProcessLine(command);
  gSystem->Load("/home/e748/analysis/ganil2root/lib/libG2RCore.so");
  gSystem->Load("./GUser.so"); //load GUser class 

  GNetClientNarval *net = new GNetClientNarval("ganp616");
  net->SetPort(10221);
  net->SetBufferSize(65536);

  GUser* a= new GUser(net); // creat user treatement environement
  GNetServerRoot *serv = new GNetServerRoot(9094, a);
  a->EventInit("e748"); // even initialisation
  //a->SetSpectraMode(2); // Declare all raw parameters as histograms
  a->SetSpectraMode(0); 
  a->InitUser();
  serv->StartServer();
  a->DoRun(); // a->DoRun(2002); do treaments on 2000 first events ( 0 = all);
  net->Close()
  a->EndUser(); // must be explicitly called , if it needs
  //a->SpeSave("histo.root"); // save all declared histogram
  delete (a); // finish
  gROOT->ProcessLine(".q");
}

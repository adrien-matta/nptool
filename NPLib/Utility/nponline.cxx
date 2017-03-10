#include"TApplication.h"
#include "TROOT.h"
#include "TError.h"
#include"NPOnlineGUI.h"
#include"NPSpectraClient.h"
#include<cstdlib>
int main(int argc , char** argv){
  // Root will not issue any BS warning message
  gErrorIgnoreLevel = 10000;

  TApplication* app = new TApplication("NPOnline",0,0);
  NPL::SpectraClient* client = NULL;
  
  if(argc==3)
    client = new NPL::SpectraClient(argv[1],std::atoi(argv[2]));
  else
    client = new NPL::SpectraClient();

  NPL::OnlineGUI* instance = new NPL::OnlineGUI(client);

  app->Run();
  return 0;
}


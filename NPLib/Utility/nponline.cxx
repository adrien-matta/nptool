#include"TApplication.h"
#include"NPOnline.h"

int main(void){
  TApplication* app = new TApplication("NPOnline",0,0);
  NPL::NPOnline* instance = new NPL::NPOnline();
  app->Run();
  return 0;
}


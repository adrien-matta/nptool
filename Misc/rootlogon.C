{
   gROOT->ProcessLine(".include /YourPathToNPTool/NPTool/NPLib/include");
   gSystem->Load("/YourPathToNPTool/NPTool/NPLib/lib/libInitialConditions.so");
   gSystem->Load("/YourPathToNPTool/NPTool/NPLib/lib/libInteractionCoordinates.so");
}

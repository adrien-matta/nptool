#include "ObjectManager.hh"


using namespace std;


int main(int argc,char** argv)
{	
   // command line parsing
   NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);

   // Instantiate RootInput
   string runToReadfileName = myOptionManager->GetRunToReadFile();
   RootInput:: getInstance(runToReadfileName);

   // if input files are not given, use those from TAsciiFile
   if (myOptionManager->IsDefault("EventGenerator")) {
      string name = RootInput::getInstance()->DumpAsciiFile("EventGenerator");
      myOptionManager->SetReactionFile(name);
   }
   if (myOptionManager->IsDefault("DetectorConfiguration")) {
      string name = RootInput::getInstance()->DumpAsciiFile("DetectorConfiguration");
      myOptionManager->SetDetectorFile(name);
   }

   // Instantiate RootOutput
   RootOutput::getInstance("Analysis/Paris_AnalyzedData", "AnalysedTree");

   // get input files from NPOptionManager
   string reactionfileName    = myOptionManager->GetReactionFile();
   string detectorfileName    = myOptionManager->GetDetectorFile();
   string calibrationfileName = myOptionManager->GetCalibrationFile();
   string OutputfileName      = myOptionManager->GetOutputFile();
	
   // Initialize the reaction
   NPL::Reaction* myReaction = new Reaction();
   myReaction->ReadConfigurationFile(reactionfileName);

   // Initialize the detector
   NPA::DetectorManager* myDetector = new DetectorManager;
   myDetector->ReadConfigurationFile(detectorfileName);

   // nominal beam energy
   Double_t BeamEnergyNominal = myReaction->GetBeamEnergy() * MeV;
   cout << BeamEnergyNominal << endl;
   // slow beam at target middle
   //Double_t BeamEnergy = BeamEnergyNominal - BeamTarget.Slow(BeamEnergyNominal, myDetector->GetTargetThickness()/2 * micrometer, 0);
   //cout << BeamEnergy << endl;
   // set energy beam at target middle
   //myReaction->SetBeamEnergy(BeamEnergy);

   // Attach more branch to the output
   double Etot = 0 ; double ExNoStrips = 0 ; double EE = 0 ; double TT = 0 ; double X = 0 ; double Y = 0 ; int det ;
   RootOutput::getInstance()->GetTree()->Branch("TotalE",&Etot,"Etot/D") ;
   //RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergyNoStrips",&ExNoStrips,"ExNoStrips/D") ;
   //RootOutput::getInstance()->GetTree()->Branch("E",&EE,"EE/D") ;
   //RootOutput::getInstance()->GetTree()->Branch("A",&TT,"TT/D") ;
   //RootOutput::getInstance()->GetTree()->Branch("X",&X,"X/D") ;
   //RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D") ;

   // Get GaspardTracker pointer
   //GaspardTracker* GPDTrack = (GaspardTracker*) myDetector->m_Detector["GASPARD"];
   // Get Paris pointer
   Paris* ParisTrack = (Paris*) myDetector->m_Detector["PARIS"];
   // Get Shield pointer
   Shield* ShieldTrack = (Shield*) myDetector->m_Detector["SHIELD"];

   // Get the TChain and treat it
   TChain* chain = RootInput:: getInstance() -> GetChain();

   // Connect TInteractionCoordinates branch
   TInteractionCoordinates *interCoord = 0;
   chain->SetBranchAddress("InteractionCoordinates", &interCoord);
   chain->SetBranchStatus("InteractionCoordinates", 0);
   // Connect TInitialConditions branch
   TInitialConditions *initCond = 0;
   chain->SetBranchAddress("InitialConditions", &initCond);
   chain->SetBranchStatus("InitialConditions", 0);


   // Analysis is here!
   int nentries = chain->GetEntries();
   cout << "Number of entries to be analysed: " << nentries << endl;

   for (int i = 0; i < nentries; i ++) {
      if (i%10000 == 0 && i!=0) cout << "\r" << i << " analyzed events" << flush;

     chain -> GetEntry(i);

      // Treat event
      myDetector->ClearEventPhysics();
      myDetector->BuildPhysicalEvent();


     
      double Einit= initCond->GetICEmittedEnergy(0);

      Ein->Fill(Einit);

      // Get total energy
      double E_Paris = ParisTrack->GetEnergyDeposit();   // Include LaBr & CsIback 
      double E_ParisLaBr = ParisTrack->GetEnergyInDeposit();   // Paris LaBr layer 
      double E_ParisCsI = ParisTrack->GetEnergyOutDeposit();   // Paris Csi layer 
      double E_Shield = ShieldTrack->GetEnergyDeposit(); 
      


       if (E_ParisLaBr >-1000 && E_ParisCsI<0) {
 
	 if(E_ParisLaBr>=(Einit-0.1) && E_ParisLaBr<=(Einit+0.1))     // +- 1% of initial energy
	   {
	     PhPeakEffParisLaBr->Fill(Einit);
	   }
      }

       if (E_ParisCsI >-1000 && E_ParisLaBr<0) {
 
	 if(E_ParisCsI>=(Einit-0.14) && E_ParisCsI<=(Einit+0.14)) 
	   {
	     PhPeakEffParisCsI->Fill(Einit);
	   }
      }


       if (E_Paris > -1000 && E_Shield<0) {
	 Etot=E_Paris;
	 //cout << "Energy Paris=" << Etot << endl;
 
	 // Fill output tree
	 RootOutput::getInstance()->GetTree()->Fill();

	 if(Etot>=(Einit-0.1) && Etot<=(Einit+0.1))
	   {
	     PhPeakEff->Fill(Einit);
	     PhPeakEffNoAB->Fill(Einit); // = No addback between PARIS & Shield
	     PhPeakEffParis->Fill(Einit);
	   }
     }

       
       if (E_Shield > -1000 && E_Paris<0) {
	 Etot=E_Shield;
	 //cout << "Energy Shield =" << Etot << endl;
 
	 // Fill output tree
	 RootOutput::getInstance()->GetTree()->Fill();


	 if(Etot>=(Einit-0.14) && Etot<=(Einit+.14))
	   {
	     PhPeakEff->Fill(Einit);
	     PhPeakEffNoAB->Fill(Einit); // = No addback between PARIS and Shield
	     PhPeakEffShield->Fill(Einit);
	   }
	 }
       
       if (E_Shield > -1000 && E_Paris>-1000) {
	 Etot=E_Shield+E_Paris;  // addback
	 //cout << "Energy Shield+PARIS =" << Etot << endl;
 
	 // Fill output tree
	 RootOutput::getInstance()->GetTree()->Fill();

	 if(Etot>(Einit-0.14) && Etot<(Einit+0.14))PhPeakEff->Fill(Einit);
	 //if(Etot>=(Einit*0.99) && Etot<=(Einit*1.01))PhPeakEff->Fill(Einit);

	 }



    }


   Ein->Write();


   PhPeakEff->Divide(PhPeakEff,Ein,1,0.01);
   PhPeakEff->Write();

   PhPeakEffNoAB->Divide(PhPeakEffNoAB,Ein,1,0.01);
   PhPeakEffNoAB->Write();

   PhPeakEffParis->Divide(PhPeakEffParis,Ein,1,0.01);
   PhPeakEffParis->Write();
   PhPeakEffParisLaBr->Divide(PhPeakEffParisLaBr,Ein,1,0.01);
   PhPeakEffParisLaBr->Write();
   PhPeakEffParisCsI->Divide(PhPeakEffParisCsI,Ein,1,0.01);
   PhPeakEffParisCsI->Write();

   PhPeakEffShield->Divide(PhPeakEffShield,Ein,1,0.01);
   PhPeakEffShield->Write();

   // delete singleton classes
   RootOutput::getInstance()->Destroy();
   RootInput::getInstance()->Destroy();
   NPOptionManager::getInstance()->Destroy();



   return 0;
}


double ThetaCalculation (TVector3 A , TVector3 B)
{
   double Theta = acos( (A.Dot(B)) / (A.Mag()*B.Mag()) );
   return Theta ;
}


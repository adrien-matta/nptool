#include "ObjectManager.hh"

using namespace std;


int main(int argc,char** argv)
{	
   // test if number of arguments is correct
   if (argc != 4) {
      cout << 
         "you need to specify both a Reaction file and a Detector file such as : Analysis myReaction.reaction myDetector.detector runToRead.run" 
           << endl;
      return 0;
   }

   // get arguments
   string reactionfileName  = argv[1];
   string detectorfileName  = argv[2];
   string runToReadfileName = argv[3];

   // Instantiate RootInput and RootOutput singleton classes
   RootInput:: getInstance(runToReadfileName);
   RootOutput::getInstance("Analysis/Gaspard_AnalyzedData", "AnalyzedTree");

   // Initialize the reaction
   NPL::Reaction* myReaction = new Reaction();
   myReaction->ReadConfigurationFile(reactionfileName);

   // Initialize the detector
   DetectorManager* myDetector = new DetectorManager;
   myDetector->ReadConfigurationFile(detectorfileName);

   // Print target thickness
   cout << myDetector->GetTargetThickness() << endl;

   // Attach more branch to the output
   double Ex = 0 ; double ExNoStrips = 0 ; double EE = 0 ; double TT = 0 ; double X = 0 ; double Y = 0 ; int det ;
   RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",&Ex,"Ex/D") ;
   RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergyNoStrips",&ExNoStrips,"ExNoStrips/D") ;
   RootOutput::getInstance()->GetTree()->Branch("E",&EE,"EE/D") ;
   RootOutput::getInstance()->GetTree()->Branch("A",&TT,"TT/D") ;
   RootOutput::getInstance()->GetTree()->Branch("X",&X,"X/D") ;
   RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D") ;

   // Get GaspardTracker pointer
   GaspardTracker* GPDTrack = (GaspardTracker*) myDetector->m_Detector["GASPARD"];

   // Get the TChain and treat it
   TChain* chain = RootInput:: getInstance() -> GetChain();

   // Connect TInteractionCoordinates branch
   TInteractionCoordinates *interCoord = 0;
   chain->SetBranchAddress("InteractionCoordinates", &interCoord);
   chain->SetBranchStatus("InteractionCoordinates", 0);

   // Analysis is here!
   int nentries = chain->GetEntries();
   cout << "Number of entries to be analysed: " << nentries << endl;

   for (int i = 0; i < nentries; i ++) {
      if (i%10000 == 0 && i!=0) cout << "\r" << i << " analysed events" << flush;
      chain -> GetEntry(i);

      // Treat Gaspard event
      myDetector->ClearEventPhysics();
      myDetector->BuildPhysicalEvent();

      // Get total energy
      double E = GPDTrack->GetEnergyDeposit();

      // if there is a hit in the detector array, treat it.
      double Theta, ThetaStrip;
      TVector3 A;
      if (E > -1000) {
         // Get exact scattering angle from TInteractionCoordinates object
         Theta = interCoord->GetDetectedAngleTheta(0) * deg;

         // Get interaction coordinates taking into account the strips
         A = GPDTrack->GetPositionOfInteraction();
         // Calculate scattering angle
         ThetaStrip = ThetaCalculation (A ,TVector3(0,0,1));

         // Correct for energy loss in the target
         E = DeutonTargetCD2.EvaluateInitialEnergy(E, 5.15*micrometer, ThetaStrip);

         // Calculate excitation energy
         if (Theta/deg > 90) {
            ExNoStrips = myReaction->ReconstructRelativistic(E, Theta / rad);
            Ex         = myReaction->ReconstructRelativistic(E, ThetaStrip);
         }
         else {
            Ex         = -200;
            ExNoStrips = -200;
         }
      }
      else {
         Ex         = -100;
         ExNoStrips = -100;
      }

      EE = E ; TT = ThetaStrip/deg;
      if (E>-1000) {
         X = A . X();
         Y = A . Y();
      }
      else {
         X = -1000 ; Y = -1000;
      }

      // Fill output tree
      RootOutput::getInstance()->GetTree()->Fill();
   }

   // delete singleton classes
   RootOutput::getInstance()->Destroy();
   RootInput::getInstance()->Destroy();

   return 0;
}


double ThetaCalculation (TVector3 A , TVector3 B)
{
   double Theta = acos( (A.Dot(B)) / (A.Mag()*B.Mag()) );
   return Theta ;
}


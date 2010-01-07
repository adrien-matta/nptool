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
   NPA::DetectorManager* myDetector = new DetectorManager;
   myDetector->ReadConfigurationFile(detectorfileName);

   // Calculate beam energy at target middle
   // Get nominal beam energy
   Double_t BeamEnergyNominal = myReaction->GetBeamEnergy() * MeV;
   cout << BeamEnergyNominal << endl;
   // Slow beam at target middle
   Double_t BeamEnergy = BeamEnergyNominal - BeamTarget.Slow(BeamEnergyNominal, myDetector->GetTargetThickness()/2 * micrometer, 0);
//   Double_t BeamEnergy = 1293.56 * MeV;
   cout << BeamEnergy << endl;
   // Set energy beam at target middle
   myReaction->SetBeamEnergy(BeamEnergy);

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

   // Get the input TChain and treat it
   TChain* chain = RootInput:: getInstance() -> GetChain();

   // Connect TInitialConditions branch
   TInitialConditions *initCond = 0;
   chain->SetBranchAddress("InitialConditions", &initCond);
   chain->SetBranchStatus("InitialConditions", 1);

   // Connect TInteractionCoordinates branch
   TInteractionCoordinates *interCoord = 0;
   chain->SetBranchAddress("InteractionCoordinates", &interCoord);
   chain->SetBranchStatus("InteractionCoordinates", 0);

   // Analysis is here!
   int nentries = chain->GetEntries();
   cout << "Number of entries to be analysed: " << nentries << endl;

   // Default initialization
   double XTarget = 0;
   double YTarget = 0;
   double BeamTheta = 0;
   double BeamPhi = 0;

   // random generator
   TRandom3 *gene = new TRandom3();

   // Loop on all events
   for (int i = 0; i < nentries; i ++) {
      if (i%10000 == 0 && i!=0) cout << "\r" << i << " analyzed events" << flush;
      chain -> GetEntry(i);

      // Treat Gaspard event
      myDetector->ClearEventPhysics();
      myDetector->BuildPhysicalEvent();

      // Get total energy
      double E = GPDTrack->GetEnergyDeposit();

      // if there is a hit in the detector array, treat it.
      double Theta, ThetaStrip, angle;
      double DetecX, DetecY, DetecZ;
      double r;
      TVector3 A;
      if (E > -1000) {
         // Get exact scattering angle from TInteractionCoordinates object
//         Theta = interCoord->GetDetectedAngleTheta(0) * deg;
         DetecX = interCoord->GetDetectedPositionX(0);
         DetecY = interCoord->GetDetectedPositionY(0);
         DetecZ = interCoord->GetDetectedPositionZ(0);
         TVector3 Detec(DetecX, DetecY, DetecZ);

         // Get interaction position in detector
         // This takes into account the strips
         A = GPDTrack->GetPositionOfInteraction();

         // Get beam interaction coordinates on target (from initial condition)
         XTarget = initCond->GetICPositionX(0);
         YTarget = initCond->GetICPositionY(0);
//         cout << XTarget << "  " << YTarget << endl;
         BeamTheta = initCond->GetICIncidentAngleTheta(0)*deg;
         BeamPhi   = initCond->GetICIncidentAnglePhi(0)*deg;
         TVector3 BeamDirection = TVector3(cos(BeamPhi)*sin(BeamTheta), sin(BeamPhi)*sin(BeamTheta), cos(BeamTheta));
//         cout << BeamDirection.X() << "  " << BeamDirection.Y() << "  " << BeamDirection.Z() << endl;

         // Hit direction taking into account beam position on target
         TVector3 HitDirection = A - TVector3(XTarget, YTarget, 0);
//         cout << "A:            " << A.X() << "  " << A.Y() << "  " << A.Z() << endl;
//         cout << "HitDirection: " << HitDirection.X() << "  " << HitDirection.Y() << "  " << HitDirection.Z() << endl;

         // Calculate scattering angle w.r.t. optical beam axis (do not take into account beam position on target)
//         ThetaStrip = ThetaCalculation(A, TVector3(0,0,1));
//         Theta = ThetaCalculation(Detec, TVector3(0, 0, 1));
         // Calculate scattering angle w.r.t. beam (ideal case)
//         ThetaStrip = ThetaCalculation(HitDirection, BeamDirection);
//         Theta = ThetaCalculation(Detec - TVector3(XTarget, YTarget, 0), BeamDirection);
         // Calculate scattering angle w.r.t. beam (finite spatial resolution)
         double resol = 800;	// in micrometer
         angle = gene->Rndm() * 2*3.14;
         r     = fabs(gene->Gaus(0, resol)) * micrometer;
         ThetaStrip = ThetaCalculation(A     - TVector3(XTarget + r*cos(angle), YTarget + r*sin(angle), 0), BeamDirection);
         Theta      = ThetaCalculation(Detec - TVector3(XTarget + r*cos(angle), YTarget + r*sin(angle), 0), BeamDirection);

         // Correct for energy loss in the target
         E = LightTarget.EvaluateInitialEnergy(E, myDetector->GetTargetThickness()/2 * micrometer, ThetaStrip);

         // Calculate excitation energy
//         if (Theta/deg > 150  && Theta/deg < 180) {
         if (Theta/deg < 45) {
//         if (Theta/deg > 90) {
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


#include "Analysis.h"
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
   RootOutput::getInstance("Analysis/Gaspard_AnalyzedData", "AnalysedTree");

   // get input files from NPOptionManager
   string reactionfileName    = myOptionManager->GetReactionFile();
   string detectorfileName    = myOptionManager->GetDetectorFile();
   string OutputfileName      = myOptionManager->GetOutputFile();

   // Initialize the reaction
   NPL::Reaction* myReaction = new Reaction();
   myReaction->ReadConfigurationFile(reactionfileName);

   // Initialize the detector
   NPA::DetectorManager* myDetector = new DetectorManager;
   myDetector->ReadConfigurationFile(detectorfileName);

   // Calculate beam energy at target middle
   // Target informations
   cout << "/////////// Target information ///////////" << endl;
   cout << "Thickness (um): " << myDetector->GetTargetThickness() << endl;
   // Get nominal beam energy
   Double_t BeamEnergyNominal = myReaction->GetBeamEnergy() * MeV;
   cout << "Nominal beam energy (MeV): " << BeamEnergyNominal << endl;
   // Slow beam at target middle
   Double_t BeamEnergy = BeamTarget.Slow(BeamEnergyNominal, myDetector->GetTargetThickness()/2 , 0);
   cout << "Middle-target beam energy (MeV): " << BeamEnergy << endl << endl;
   // Set energy beam at target middle
   myReaction->SetBeamEnergy(BeamEnergy);

   // Attach more branch to the output
   double Ex = 0 ; double ExNoStrips = 0 ; double EE = 0 ; double TT = 0 ; double X = 0 ; double Y = 0 ; int det ;
   RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",&Ex,"Ex/D") ;
   RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergyNoStrips",&ExNoStrips,"ExNoStrips/D") ;
   RootOutput::getInstance()->GetTree()->Branch("E",&EE,"EE/D") ;
   RootOutput::getInstance()->GetTree()->Branch("A",&TT,"TT/D") ;
   RootOutput::getInstance()->GetTree()->Branch("X",&X,"X/D") ;
   RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D") ;

   // Get GaspardTracker pointer
   GaspardTracker* GPDTrack = (GaspardTracker*) myDetector->GetDetector("GASPARD");

   // Get the input TChain and treat it
   TChain* Chain = RootInput:: getInstance()->GetChain();

   // Connect TInitialConditions branch
   TInitialConditions *initCond = 0;
   Chain->SetBranchAddress("InitialConditions", &initCond);
   Chain->SetBranchStatus("InitialConditions", 1);

   // Connect TInteractionCoordinates branch
   TInteractionCoordinates *interCoord = 0;
   Chain->SetBranchAddress("InteractionCoordinates", &interCoord);
   Chain->SetBranchStatus("InteractionCoordinates", 0);

   // Default initialization
   double XTarget = 0;
   double YTarget = 0;
   double BeamTheta = 0;
   double BeamPhi = 0;

   // random generator
   TRandom3 *gene = new TRandom3();

   // Get number of events to treat
    cout << endl << "///////// Starting Analysis ///////// "<< endl;
   int nentries = Chain->GetEntries();
   cout << " Number of Event to be treated : " << nentries << endl;
   clock_t begin = clock();
   clock_t end = begin;


   // Loop on all events
   for (int i = 0; i < nentries; i ++) {
      if (i%10000 == 0 && i!=0)  {
         cout.precision(5);
         end = clock();
         double TimeElapsed = (end-begin) / CLOCKS_PER_SEC;
         double percent = (double)i/nentries;
         double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
         cout  << "\r Progression:" << percent*100 << " % \t | \t Remaining time : ~" <<  TimeToWait <<"s"<< flush;
      }
      else if (i == nentries-1)  cout << "\r Progression:" << " 100% " <<endl;

      // Get data
      Chain -> GetEntry(i);

      // Treat Gaspard event
      myDetector->ClearEventPhysics();
      myDetector->BuildPhysicalEvent();

      // Get total energy
      double E = GPDTrack->GetEnergyDeposit();

      // if there is a hit in the detector array, treat it.
      double Theta, ThetaStrip, angle, ThetaCM;
      double DetecX, DetecY, DetecZ;
      double r;
      TVector3 A;
      if (E > -1000) {
         // Get c.m. angle
         ThetaCM = initCond->GetThetaCM(0) * deg;

         // Get exact scattering angle from TInteractionCoordinates object
//         interCoord->Dump();
//         cout << i << " mult: " << interCoord->GetDetectedMultiplicity() << endl;
         DetecX = interCoord->GetDetectedPositionX(0);
         DetecY = interCoord->GetDetectedPositionY(0);
         DetecZ = interCoord->GetDetectedPositionZ(0);
         TVector3 Detec(DetecX, DetecY, DetecZ);

         // Get interaction position in detector
         // This takes into account the strips
         A = GPDTrack->GetPositionOfInteraction();

         // Get beam interaction coordinates on target (from initial condition)
         XTarget = initCond->GetIncidentPositionX();
         YTarget = initCond->GetIncidentPositionY();
         BeamTheta = initCond->GetIncidentEmittanceTheta()*deg;
         BeamPhi   = initCond->GetIncidentEmittanceTheta()*deg;
         TVector3 BeamDirection = TVector3(cos(BeamPhi)*sin(BeamTheta), sin(BeamPhi)*sin(BeamTheta), cos(BeamTheta));

         // Hit direction taking into account beam position on target
         TVector3 HitDirection = A - TVector3(XTarget, YTarget, 0);
//         cout << "A:            " << A.X() << "  " << A.Y() << "  " << A.Z() << endl;
//         cout << "HitDirection: " << HitDirection.X() << "  " << HitDirection.Y() << "  " << HitDirection.Z() << endl;

         // Calculate scattering angle w.r.t. optical beam axis (do not take into account beam position on target)
         ThetaStrip = ThetaCalculation(A, TVector3(0,0,1));
         Theta = ThetaCalculation(Detec, TVector3(0, 0, 1));
         // Calculate scattering angle w.r.t. beam (ideal case)
//         ThetaStrip = ThetaCalculation(HitDirection, BeamDirection);
//         Theta = ThetaCalculation(Detec - TVector3(XTarget, YTarget, 0), BeamDirection);
         // Calculate scattering angle w.r.t. beam (finite spatial resolution)
//         double resol = 800;	// in micrometer
//         angle = gene->Rndm() * 2*3.14;
//         r     = fabs(gene->Gaus(0, resol)) * micrometer;
//         ThetaStrip = ThetaCalculation(A     - TVector3(XTarget + r*cos(angle), YTarget + r*sin(angle), 0), BeamDirection);
//         Theta      = ThetaCalculation(Detec - TVector3(XTarget + r*cos(angle), YTarget + r*sin(angle), 0), BeamDirection);
//
         // Correct for energy loss in the target
         E = LightTarget.EvaluateInitialEnergy(E, myDetector->GetTargetThickness()/2 , ThetaStrip);

         // Calculate excitation energy
//         if (Theta/deg > 150  && Theta/deg < 180) {
//         if (Theta/deg < 60 && ThetaCM/deg < 90) {
//         if (Theta/deg > 35 && Theta/deg < 45 && E/MeV < 17) {
//         if (Theta/deg < 45) {
//         if (E/MeV < 38) {		// for (p,t) reaction
         if (Theta/deg > 30) {	// for (d,p) reaction
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
   NPOptionManager::getInstance()->Destroy();

   return 0;
}


double ThetaCalculation (TVector3 A , TVector3 B)
{
   double Theta = acos( (A.Dot(B)) / (A.Mag()*B.Mag()) );
   return Theta ;
}


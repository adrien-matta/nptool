#include "ObjectManager.hh"

using namespace std;


int main(int argc,char** argv)
{	
   // command line parsing
   NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);
   string reactionfileName          = myOptionManager->GetReactionFilePath();
	string detectorfileName          = myOptionManager->GetDetectorFilePath();
	string calibrationfileName       = myOptionManager->GetCalibrationFilePath();
	string runToReadfileName         = myOptionManager->GetRunToReadFilePath();
	string OutputfileName            = myOptionManager->GetOutputFilePath();

   // Instantiate RootInput and RootOutput singleton classes
   RootInput:: getInstance(runToReadfileName);
//   RootOutput::getInstance("Analysis/"+OutputfileName, "AnalyzedTree")	;
   RootOutput::getInstance("Analysis/W1_AnalyzedData", "AnalyzedTree");
 
   // Initialize the reaction
   cout << endl << "/////////// Event generator ///////////" << endl;
   NPL::Reaction* myReaction = new Reaction();
   myReaction->ReadConfigurationFile(reactionfileName);

   // Initialize the detector
   NPA::DetectorManager* myDetector = new DetectorManager;
   myDetector->ReadConfigurationFile(detectorfileName);

   // Calculate beam energy at target middle
   // Target informations
   cout << endl;
   cout << "/////////// Target information ///////////" << endl;
   cout << "Thickness (um): " << myDetector->GetTargetThickness() << endl;
   // Get nominal beam energy
   Double_t BeamEnergyNominal = myReaction->GetBeamEnergy() * MeV;
   cout << "Nominal beam energy (MeV): " << BeamEnergyNominal << endl;
   // Slow beam at target middle
   Double_t BeamEnergy = BeamTarget.Slow(BeamEnergyNominal, myDetector->GetTargetThickness()/2 * micrometer, 0);
   cout << "Middle-target beam energy (MeV): " << BeamEnergy << endl;
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

   // Get TW1Physics pointer
   TW1Physics* W1 = (TW1Physics*) myDetector->m_Detector["W1"];

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

   // random generator
   TRandom3 *gene = new TRandom3();

   // Default initialization
   double XTarget = 0;
   double YTarget = 0;
   double BeamTheta = 0;
   double BeamPhi = 0;

   // Analysis is here!
   int nentries = chain->GetEntries();
   cout << endl;
   cout << "/////////// Loop information ///////////" << endl;
   cout << "Number of entries to be analysed: " << nentries << endl;
   clock_t begin = clock();
   clock_t end = begin;

   // Loop on all events
   for (int i = 0; i < nentries; i ++) {
      if (i%10000 == 0 && i!=0)  {
         cout.precision(5);
         end = clock();
         double TimeElapsed = (end-begin) / CLOCKS_PER_SEC;
         double percent = (double)i / nentries;
         double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
         cout << "\rProgression:" << percent*100 << " % \t | \t Remaining time : ~" <<  TimeToWait << "s" << flush;
      }
      else if (i == nentries-1)  cout << "\rProgression:" << " 100%" << endl;

      // Get raw data
      chain -> GetEntry(i);

      // Treat W1 event
      myDetector->ClearEventPhysics();
      myDetector->BuildPhysicalEvent();

      // Get Target information from TInitialConditions
      XTarget = initCond->GetICPositionX(0);
      YTarget = initCond->GetICPositionY(0);
      BeamTheta = initCond->GetICIncidentAngleTheta(0)*deg;
      BeamPhi   = initCond->GetICIncidentAnglePhi(0)*deg;
      TVector3 BeamDirection = TVector3(cos(BeamPhi)*sin(BeamTheta), sin(BeamPhi)*sin(BeamTheta), cos(BeamTheta));

      // loop on multiplicity event
      for (int hit = 0; hit < W1->GetEventMultiplicity(); hit++) {
         // Get c.m. angle
         double ThetaCM = initCond->GetICEmittedAngleThetaCM(0) * deg;

         // Get exact scattering angle from TInteractionCoordinates object
         double DetecX = interCoord->GetDetectedPositionX(hit);
         double DetecY = interCoord->GetDetectedPositionY(hit);
         double DetecZ = interCoord->GetDetectedPositionZ(hit);
         TVector3 Detec(DetecX, DetecY, DetecZ);

         // Get interaction position in detector
         // This takes into account the strips
         TVector3 A = W1->GetPositionOfInteraction(hit);

         // Hit direction taking into account beam position on target
         TVector3 HitDirection = A - TVector3(XTarget, YTarget, 0);

         // Calculate scattering angle w.r.t. optical beam axis (do not take into account beam position on target)
         double ThetaStrip = ThetaCalculation(A,     TVector3(0,0,1));
         double Theta      = ThetaCalculation(Detec, TVector3(0, 0, 1));

         // Calculate scattering angle w.r.t. real beam direction (ideal case)
//         ThetaStrip = ThetaCalculation(HitDirection, BeamDirection);
//         Theta      = ThetaCalculation(Detec - TVector3(XTarget, YTarget, 0), BeamDirection);
         // Calculate scattering angle w.r.t. beam (finite spatial resolution)
//         double resol = 800;	// in micrometer
//         double angle = gene->Rndm() * 2*3.14;
//         double r     = fabs(gene->Gaus(0, resol)) * micrometer;
//         ThetaStrip = ThetaCalculation(A     - TVector3(XTarget + r*cos(angle), YTarget + r*sin(angle), 0), BeamDirection);
//         Theta      = ThetaCalculation(Detec - TVector3(XTarget + r*cos(angle), YTarget + r*sin(angle), 0), BeamDirection);
//
         // Correct for energy loss in the target
         double E = W1->GetEnergy(hit);
         E = LightTarget.EvaluateInitialEnergy(E, myDetector->GetTargetThickness()/2 * micrometer, ThetaStrip);

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
      }  // end loop on multiplicity event

      // Fill output tree
      RootOutput::getInstance()->GetTree()->Fill();
   }  // end loop on number of events to treat

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


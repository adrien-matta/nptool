#include "Analysis.h"
using namespace std;

#define MM_DEADLAYER 0.4   // Must2 aluminium dead layer in um

double ThetaCalculation (TVector3 A , TVector3 B);
double PhiCalculation (TVector3 A , TVector3 B);

int main(int argc,char** argv)
{	
   // command line parsing
   NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);

   //	Instantiate RootInput
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
   RootOutput::getInstance("Analysis/Must2_AnalysedData", "AnalysedTree");

   // get input files from NPOptionManager
   string reactionfileName    = myOptionManager->GetReactionFile();
   string detectorfileName    = myOptionManager->GetDetectorFile();
   string OutputfileName      = myOptionManager->GetOutputFile();

   // Instantiate some Reaction
   cout << endl << "/////////// Event generator ///////////" << endl;
   NPL::Reaction* myReaction = new Reaction();
   myReaction->ReadConfigurationFile(reactionfileName);
    
   // Instantiate the detector using a file 
   NPA::DetectorManager* myDetector = new DetectorManager();
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
   Double_t BeamEnergy = BeamTargetCD2.Slow(BeamEnergyNominal, myDetector->GetTargetThickness()/2 * micrometer, 0);
   cout << "Middle-target beam energy (MeV): " << BeamEnergy << endl;
   // Set energy beam at target middle
   myReaction->SetBeamEnergy(BeamEnergy);
   myReaction->Print();
      
   // Attach more branch to the output
   const int mult = 2;
   double ELab[mult], ThetaLab[mult], ThetaCM[mult], PhiLab[mult], ExcitationEnergy[mult];
 
   // some initializations
   for (int i = 0; i < mult; i++)  {
	  ELab[i]  = -10000; ThetaLab[i] = -10000; ThetaCM[i] = -10000; ExcitationEnergy[i] = -10000; PhiLab[i] = -10000;
	}

   RootOutput::getInstance()->GetTree()->Branch("ELab",             &ELab,             "ELab[2]/D");
   RootOutput::getInstance()->GetTree()->Branch("ThetaLab",         &ThetaLab,         "ThetaLab[2]/D");
   RootOutput::getInstance()->GetTree()->Branch("ThetaCM",          &ThetaCM,          "ThetaCM[2]/D");
   RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy", &ExcitationEnergy, "ExcitationEnergy[2]/D");
      
   // Get the formed Chained Tree and Treat it
   TChain* Chain = RootInput::getInstance()->GetChain();
      
   // Connect TInitialConditions branch
   TInitialConditions *initCond = 0;
   Chain->SetBranchAddress("InitialConditions", &initCond);
   Chain->SetBranchStatus("InitialConditions", 1);
      
   // Get TMust2Physics pointer
   TMust2Physics *M2 	 = (TMust2Physics*)  myDetector -> GetDetector("MUST2")	;
            
   // define user spectra
   TH2F* DE_E_protons     = new TH2F("DE_E_protons",       "DE_E et cut protons", 1000,    0, 25000, 1000,   0, 25000);
   TH2F* DE_E             = new TH2F("DE_E",               "DE_E",                1000,    0, 25000, 1000,   0, 25000);
   TH2F* E_Theta          = new TH2F("E_Theta",            "E_Theta",               60,  100,   180,  500,   0, 20);
   TH2F* Position_M2      = new TH2F("Must2Positions",     "Must2Positions",      2000, -200,   200, 2000, -200, 200);
   TH2F* Position_M2_tot  = new TH2F("Must2Positions_tot", "Must2Positions_tot",   130,   -1,   129,  130,   -1, 130);
      
   cout << endl << "///////// Starting Analysis ///////// "<< endl;
   int nentries = Chain->GetEntries();
   cout << " Number of Event to be treated : " << nentries << endl;
   clock_t begin = clock();
   clock_t end = begin;


   for (int i = 0; i < nentries; i++) {
	   if (i%10000 == 0 && i!=0) 	{	
	      cout.precision(5);
	      end = clock();										
	      double TimeElapsed = (end-begin) / CLOCKS_PER_SEC;
	      double percent = (double)i/nentries ;
	      double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
	      cout  << "                                                                                                "<< flush;
	      cout	<< "\r Progression:" << percent*100 << " % \t | \t Remaining time : ~" <<  TimeToWait <<"s"<< flush;
      }
      else if (i==nentries-1) 	cout << "\r Progression:" << " 100% " <<endl;
	  
      // Get raw data
      Chain->GetEntry(i);

      // Clear Previous Event
      myDetector->ClearEventPhysics();

      // Build the new event
      myDetector->BuildPhysicalEvent();
	  
      //	Get target interaction position from initial conditions
      double XTarget = initCond->GetIncidentPositionX();
      double YTarget = initCond->GetIncidentPositionY();
      double ZTarget = initCond->GetIncidentPositionZ();
		  
      // Calculate beam direction
      double BeamTheta = initCond->GetIncidentEmittanceTheta()*deg; double BeamPhi = initCond->GetIncidentEmittancePhi()*deg;
      TVector3 BeamDirection = TVector3(cos(BeamPhi)*sin(BeamTheta) , sin(BeamPhi)*sin(BeamTheta) , cos(BeamTheta));
		  
      for (int hit = 0; hit < M2->Si_E.size(); hit++) {
         ExcitationEnergy[hit] = -10000;
         ELab[hit] = -10000 ; ThetaLab[hit] = -10000;

         // Get hit direction
         TVector3 HitDirection = M2->GetPositionOfInteraction(hit) - TVector3(XTarget, YTarget, myDetector->GetTargetZ());
		  
         // Angle between beam and particle
         ThetaLab[hit]  = ThetaCalculation(HitDirection , BeamDirection) * rad;
         PhiLab[hit]    = (M2->GetPositionOfInteraction(hit)).Phi();
		  
         // Angle between particule and z axis (target Normal)
         double ThetaN = ThetaCalculation(HitDirection , TVector3(0,0,1));
         // Angle between particule and Must2 Si surface
         double ThetaMM2Surface = ThetaCalculation(HitDirection, M2->GetTelescopeNormal(hit));

         if ((M2->Si_E[hit] > 0)) {
		      Position_M2->Fill((M2->GetPositionOfInteraction(hit)).X(), (M2->GetPositionOfInteraction(hit)).Y());
		   }
		 
		   if ((M2->Si_E[hit] > 0) && (M2->SiLi_E[hit]) < 0) {
		      ELab[hit] = M2->Si_E[hit] * MeV;
            ELab[hit] = protonStripAl.EvaluateInitialEnergy(ELab[hit], MM_DEADLAYER*micrometer, ThetaMM2Surface) /MeV;
		      ELab[hit] = protonTargetCD2.EvaluateInitialEnergy(ELab[hit], myDetector -> GetTargetThickness() /2 *micrometer , ThetaLab[hit]) /MeV;
		      
		      ExcitationEnergy[hit] = myReaction->ReconstructRelativistic(ELab[hit]/MeV, ThetaLab[hit]) /MeV;
		      
		      ThetaLab[hit]  = ThetaLab[hit] /deg;
		      PhiLab[hit]    = PhiLab[hit] /deg;
		    }
		  
         else if ((M2->Si_E[hit] > 0) && (M2->SiLi_E[hit]) > 0) {
		      Position_M2_tot->Fill(M2->Si_X[hit], M2->Si_Y[hit]);
		      DE_E ->Fill(M2->SiLi_E[hit], M2->Si_E[hit]);
		      
		      ELab[hit]  = M2->SiLi_E[hit]*MeV;
			   ELab[hit]  = protonStripAl.EvaluateInitialEnergy(ELab[hit]/MeV, MM_DEADLAYER*micrometer, ThetaMM2Surface) /MeV;
			   ELab[hit] += M2->Si_E[hit]*MeV;
			   ELab[hit]  = protonStripAl.EvaluateInitialEnergy(ELab[hit]/MeV, MM_DEADLAYER*micrometer, ThetaMM2Surface) /MeV;
		      ELab[hit]  = protonTargetCD2.EvaluateInitialEnergy(ELab[hit]/MeV,myDetector -> GetTargetThickness() /2 *micrometer,ThetaLab[hit]) /MeV;

		      ExcitationEnergy[hit] = myReaction -> ReconstructRelativistic( ELab[hit]/MeV, ThetaLab[hit]) /MeV ;
		      ThetaLab[hit]  = ThetaLab[hit] /deg;
		      PhiLab[hit]    = PhiLab[hit] /deg;
         } 
		  
         E_Theta->Fill(ThetaLab[hit],ELab[hit]/MeV);
		  
      } // end of for loop over hit
	      
      RootOutput::getInstance()->GetTree()->Fill();      
   } // end of for loop over events
      
      
   cout << " A total of " << nentries << " event has been analysed " << endl ;
   cout << endl << " ///////////////////////////////////// "<< endl<< endl ;
   RootOutput::getInstance()->Destroy();
   RootInput::getInstance()->Destroy();
   NPOptionManager::getInstance()->Destroy();
   
   return 0;
}



double ThetaCalculation (TVector3 A , TVector3 B)
{
  double Theta = acos( (A.Dot(B)) / (A.Mag()*B.Mag()) ) ;

  return Theta*rad;
}



double PhiCalculation (TVector3 A, TVector3 B)
{
  double Theta = acos((A.Dot(B)) / (A.Mag()*B.Mag()));
  double R = A.Mag();

  double Phi = 0;
  double test = A.X() / R * sin(Theta);
  
  Phi = acos(test);

  return Phi*rad;
}

#include "Analysis.h"

int main(int argc, char** argv){
	// command line parsing
	NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);

	// Instantiate RootInput
	string runToReadfileName = myOptionManager->GetRunToReadFile();
	RootInput:: getInstance("RunToTreat.txt");
	// Get the formed Chained Tree and Treat it
	TChain* Chain = RootInput:: getInstance()->GetChain();
	// if input files are not given, use those from TAsciiFile
	if (myOptionManager->IsDefault("DetectorConfiguration")) {
		string name = RootInput::getInstance()->DumpAsciiFile("DetectorConfiguration");
		myOptionManager->SetDetectorFile(name);
	}

	// get input files from NPOptionManager
	string detectorfileName    = myOptionManager->GetDetectorFile();
	string reactionfileName    =  myOptionManager->GetReactionFile();
	// string calibrationfileName = myOptionManager->GetCalibrationFile();
	string OutputfileName      = myOptionManager->GetOutputFile();

	// Instantiate RootOutput
	RootOutput::getInstance("Analysis/"+OutputfileName, "AnalysedTree");
	RootOutput::getInstance()->GetFile()->SetCompressionLevel(0);
	// Instantiate the detector using a file
	NPA::DetectorManager* myDetector = new DetectorManager();
	myDetector->ReadConfigurationFile(detectorfileName);
	// Attach new branch
	InitOutputBranch();
	InitInputBranch();

	// Instantiate the Reaction
	NPL::Reaction* P30dpReaction = new Reaction ;
	P30dpReaction -> ReadConfigurationFile(reactionfileName) ;

	//////////////////////////////////////////////////////

	// Get pointer to the different detector

	TTiaraHyballPhysics* TH  = (TTiaraHyballPhysics*) myDetector -> GetDetector("TiaraHyball");
  TTiaraBarrelPhysics* TB  = (TTiaraBarrelPhysics*) myDetector -> GetDetector("TiaraBarrel");

	// intermediate variable
	//TRandom3 Rand = TRandom3();
  TRandom *Rand = new TRandom3();
	double ThetaNormalTarget = 0 ;
	double ThetaTHSurface = 0;
  double ThetaTBSurface = 0;
	double Si_E_TH = 0 ;
  double Si_E_TB = 0 ;
	double Energy = 0;
	double TargetThickness = myDetector->GetTargetThickness()*micrometer;
	
  double XTarget = 0;
	double YTarget =0;
	TVector3 BeamDirection = TVector3(0,0,1);

	// Get number of events to treat
	cout << endl << "///////// Starting Analysis ///////// "<< endl;
	int nentries = Chain->GetEntries();
	cout << " Number of Event to be treated : " << nentries << endl;
	clock_t begin = clock();
	clock_t end = begin;
	cout.precision(5);
	////////////////////////////////////////////////////////////////////////////
	// main loop on entries
	for (int i = 0; i < nentries; i++) {
		if (i%10000 == 0 && i!=0)  {
			cout.precision(5);
			end = clock();
			double TimeElapsed = (end-begin) / CLOCKS_PER_SEC;
			double percent = (double)i/nentries;
			double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
			cout  << "                                                      "<< flush;
			cout  << "\r Progression:"
				<< percent*100 << " % \t | \t Remaining time : ~"
				<<  TimeToWait <<"s | Analysis Rate :"
				<< (double) i/TimeElapsed << flush;
		}
		else if (i == nentries-1)  cout << "\r Progression:" << " 100% " <<endl;

		// Get the raw data
		Chain -> GetEntry(i);
		// Clear previous event
		myDetector->ClearEventPhysics();
		// Build the current event
		myDetector->BuildPhysicalEvent();
		// Reinitiate calculated variable
		ReInitValue();

		// Beam energy is measured using F3 and F2 plastic TOF (time of flight)
		double BeamEnergy = Rand->Gaus(Init->GetIncidentInitialKineticEnergy(),4.5);
		BeamEnergy = P30_CD2.Slow(BeamEnergy,TargetThickness/2.,0);
		P30dpReaction->SetBeamEnergy(BeamEnergy);

		//////////////////////////// LOOP on TiaraHyball + SSSD Hit //////////////////
		for(unsigned int countTiaraHyball = 0 ; countTiaraHyball < TH->Strip_E.size() ; countTiaraHyball++){
			/************************************************/
			//Part 0 : Get the useful Data
			// TiaraHyball

			/************************************************/
			// Part 1 : Impact Angle
			ThetaTHSurface = 0;
			ThetaNormalTarget = 0;
			if(XTarget>-1000 && YTarget>-1000){
				TVector3 BeamImpact(XTarget,YTarget,0);
				TVector3 HitDirection = TH -> GetPositionOfInteraction(countTiaraHyball) - BeamImpact ;
				ThetaLab = HitDirection.Angle( BeamDirection );

				ThetaTHSurface = HitDirection.Angle(TVector3(0,0,-1) );
				ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
			}
			else{
				BeamDirection = TVector3(-1000,-1000,-1000);
				ThetaTHSurface    = -1000  ;
				ThetaNormalTarget = -1000  ;
			}
			/************************************************/

			/************************************************/
			// Part 2 : Impact Energy
			Energy = ELab = 0;
			Si_E_TH = TH->Strip_E[countTiaraHyball];
			Energy = Si_E_TH;

			// Evaluate energy using the thickness 
			ELab = proton_Al.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaTHSurface); 
			// Target Correction
			ELab = proton_CD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget); 
			/************************************************/

			/************************************************/
			// Part 3 : Excitation Energy Calculation
			Ex = P30dpReaction -> ReconstructRelativistic( ELab , ThetaLab );
		  /************************************************/

			/************************************************/
			// Part 4 : Theta CM Calculation
			ThetaCM  = P30dpReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
		  ThetaLab=ThetaLab/deg;
	
      /************************************************/

      /************************************************/
      // Part 5a : Implementing impact matrix for the entire Hyball (all 6 wedges)
      /*TVector3 HyballImpactPosition = TH -> GetPositionOfInteraction(countTiaraHyball);
      int DetectorNumber = TH -> DetectorNumber[countTiaraHyball];
      HyballIMX = HyballImpactPosition.X();
      HyballIMY = HyballImpactPosition.Y();
      HyballIMZ = HyballImpactPosition.Z();

      // Part 5b : Implementing impact matrix for each wedge in the Hyball individually
      if(DetectorNumber==1){
        Wedge1IMX = HyballIMX;
        Wedge1IMY = HyballIMY;
        Wedge1IMZ = HyballIMZ;
        }
      if(DetectorNumber==2){
        Wedge2IMX = HyballIMX;
        Wedge2IMY = HyballIMY;
        Wedge2IMZ = HyballIMZ;
        }
      if(DetectorNumber==3){
        Wedge3IMX = HyballIMX;
        Wedge3IMY = HyballIMY;
        Wedge3IMZ = HyballIMZ;
        }
      if(DetectorNumber==4){
        Wedge4IMX = HyballIMX;
        Wedge4IMY = HyballIMY;
        Wedge4IMZ = HyballIMZ;
        }
      if(DetectorNumber==5){
        Wedge5IMX = HyballIMX;
        Wedge5IMY = HyballIMY;
        Wedge5IMZ = HyballIMZ;
        }
      if(DetectorNumber==6){
        Wedge6IMX = HyballIMX;
        Wedge6IMY = HyballIMY;
        Wedge6IMZ = HyballIMZ;
        } */
      /************************************************/

      /************************************************/
      // Part 6a : Implementing randomised position impact matrix for the entire Hyball (all 6 wedges)
      TVector3 HyballRandomImpactPosition = TH -> GetPositionOfInteraction(countTiaraHyball);
      double Zholder = HyballRandomImpactPosition.Z();
      HyballRandomImpactPosition.SetZ(0.0);
      //DetectorNumber = TH -> DetectorNumber[countTiaraHyball];
      double R = HyballRandomImpactPosition.Mag();
      double Theta = HyballRandomImpactPosition.Theta(); // defines Theta, the inclination coordinate in a spherical coordinate system
      double Phi = HyballRandomImpactPosition.Phi(); // defines Phi, the azimuthal coordinate in a spherical coordinate system
      double RandomNumber1 = Rand->Rndm();
      double DeltaR = ((RandomNumber1 * 6.4)-3.2);
      R = R + DeltaR; // randomises R within a given detector ring
      double RandomNumber2 = Rand->Rndm();
      double DeltaAngle = ((RandomNumber2 * 0.118682389)-0.0593411946);
      Phi = Phi + DeltaAngle; // randomises Phi within a given detector sector
      RandomHyballIMX = R*(sin(Theta))*(cos(Phi)); // defines the final randomised X coordinate by transforming the randomised spherical coordinates
      RandomHyballIMY = R*(sin(Phi))*(sin(Theta)); // defines the final randomised Y coordinate by transforming the randomised spherical coordinates
      RandomHyballIMZ = Zholder;
      /*
      // Part 6b : Implementing randomised position impact matrix for each wedge in the Hyball individually
      if(DetectorNumber==1){
        RandomWedge1IMX = RandomHyballIMX;
        RandomWedge1IMY = RandomHyballIMY;
        RandomWedge1IMZ = RandomHyballIMZ;
        }
      if(DetectorNumber==2){
        RandomWedge2IMX = RandomHyballIMX;
        RandomWedge2IMY = RandomHyballIMY;
        RandomWedge2IMZ = RandomHyballIMZ;
        }
      if(DetectorNumber==3){
        RandomWedge3IMX = RandomHyballIMX;
        RandomWedge3IMY = RandomHyballIMY;
        RandomWedge3IMZ = RandomHyballIMZ;
        } 
      if(DetectorNumber==4){
        RandomWedge4IMX = RandomHyballIMX;
        RandomWedge4IMY = RandomHyballIMY;
        RandomWedge4IMZ = RandomHyballIMZ;
        }
      if(DetectorNumber==5){
        RandomWedge5IMX = RandomHyballIMX;
        RandomWedge5IMY = RandomHyballIMY;
        RandomWedge5IMZ = RandomHyballIMZ;
        }
      if(DetectorNumber==6){
        RandomWedge6IMX = RandomHyballIMX;
        RandomWedge6IMY = RandomHyballIMY;
        RandomWedge6IMZ = RandomHyballIMZ;
        }
      */
      TiaraIMX = RandomHyballIMX;
      TiaraIMY = RandomHyballIMY;
      TiaraIMZ = RandomHyballIMZ;
      /************************************************/
		} // end loop TiaraHyball
		//////////////////////////// LOOP on TiaraBarrel //////////////////
		for(unsigned int countTiaraBarrel = 0 ; countTiaraBarrel < TB->Strip_E.size() ; countTiaraBarrel++){
			/************************************************/
			//Part 0 : Get the useful Data
      //TiaraBarrel

			/************************************************/
			// Part 1 : Impact Angle
			ThetaTBSurface = 0;
			ThetaNormalTarget = 0;
			if(XTarget>-1000 && YTarget>-1000){
				TVector3 BeamImpact(XTarget,YTarget,0);
				TVector3 HitDirection = TB -> GetPositionOfInteraction(countTiaraBarrel) - BeamImpact ;
				ThetaLab = HitDirection.Angle( BeamDirection );

				ThetaTBSurface = HitDirection.Angle(TVector3(0,0,-1) );
				ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
			}
			else{
				BeamDirection = TVector3(-1000,-1000,-1000);
				ThetaTBSurface    = -1000  ;
				ThetaNormalTarget = -1000  ;
			}
			/************************************************/

			/************************************************/
			// Part 2 : Impact Energy
			Energy = ELab = 0;
			Si_E_TB = TB->Strip_E[countTiaraBarrel];
			Energy = Si_E_TB;

			// Evaluate energy using the thickness 
			ELab = proton_Al.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaTBSurface); 
			// Target Correction
			ELab = proton_CD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);
			/************************************************/

			/************************************************/
			// Part 3 : Excitation Energy Calculation
			Ex = P30dpReaction -> ReconstructRelativistic( ELab , ThetaLab );
		  /************************************************/

			/************************************************/
			// Part 4 : Theta CM Calculation
			ThetaCM  = P30dpReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
		  ThetaLab=ThetaLab/deg;
	
      /************************************************/      

      /************************************************/
      // Part 5a : Implementing impact matrix for the Tiara Barrel (all 8 detecting strips)
      /*TVector3 BarrelImpactPosition = TB -> GetPositionOfInteraction(countTiaraBarrel);
      int DetectorNumberB = TB -> DetectorNumber[countTiaraBarrel];
      BarrelIMX = BarrelImpactPosition.X();
      BarrelIMY = BarrelImpactPosition.Y();
      BarrelIMZ = BarrelImpactPosition.Z();

      // Part 5b : Implementing impact matrix for each strip of the octagonal Barrel individually
      if(DetectorNumberB==1){
        //TVector3 Wedge1IM = TH -> GetPositionOfInteraction(countTiaraHyball);
        Strip1IMX = BarrelIMX;
        Strip1IMY = BarrelIMY;
        Strip1IMZ = BarrelIMZ;
        }
      if(DetectorNumberB==2){
        //TVector3 Wedge2IM = TH -> GetPositionOfInteraction(countTiaraHyball);
        Strip2IMX = BarrelIMX;
        Strip2IMY = BarrelIMY;
        Strip2IMZ = BarrelIMZ;
        }
      if(DetectorNumberB==3){
        //TVector3 Wedge3IM = TH -> GetPositionOfInteraction(countTiaraHyball);
        Strip3IMX = BarrelIMX;
        Strip3IMY = BarrelIMY;
        Strip3IMZ = BarrelIMZ;
        }
      if(DetectorNumberB==4){
        //TVector3 Wedge4IM = TH -> GetPositionOfInteraction(countTiaraHyball);
        Strip4IMX = BarrelIMX;
        Strip4IMY = BarrelIMY;
        Strip4IMZ = BarrelIMZ;
        }
      if(DetectorNumberB==5){
        //TVector3 Wedge5IM = TH -> GetPositionOfInteraction(countTiaraHyball);
        Strip5IMX = BarrelIMX;
        Strip5IMY = BarrelIMY;
        Strip5IMZ = BarrelIMZ;
        }
      if(DetectorNumberB==6){
        //TVector3 Wedge6IM = TH -> GetPositionOfInteraction(countTiaraHyball);
        Strip6IMX = BarrelIMX;
        Strip6IMY = BarrelIMY;
        Strip6IMZ = BarrelIMZ;
        }
      if(DetectorNumberB==7){
        //TVector3 Wedge5IM = TH -> GetPositionOfInteraction(countTiaraHyball);
        Strip7IMX = BarrelIMX;
        Strip7IMY = BarrelIMY;
        Strip7IMZ = BarrelIMZ;
        }
      if(DetectorNumberB==8){
        //TVector3 Wedge6IM = TH -> GetPositionOfInteraction(countTiaraHyball);
        Strip8IMX = BarrelIMX;
        Strip8IMY = BarrelIMY;
        Strip8IMZ = BarrelIMZ;
        }*/
      /************************************************/

      /************************************************/
      // Part 6a&b : Implementing randomised position impact matrix for both the entire Barrel (all 8 strips) and each strip making up the octagonal Barrel individually
      TVector3 BarrelRandomImpactPosition = TB -> GetPositionOfInteraction(countTiaraBarrel);
      int DetectorNumberB = TB -> DetectorNumber[countTiaraBarrel];
      if(DetectorNumberB==1){
        RandomBarrelIMX = BarrelRandomImpactPosition.X();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 6.0)-3.0;
        RandomBarrelIMX = RandomBarrelIMX - DeltaX;
        RandomBarrelIMY = BarrelRandomImpactPosition.Y();
        RandomBarrelIMZ = BarrelRandomImpactPosition.Z();
        }
      if(DetectorNumberB==2){
        RandomBarrelIMX = BarrelRandomImpactPosition.X();
        RandomBarrelIMY = BarrelRandomImpactPosition.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 4.242640687)-2.121320344;
        double DeltaY = DeltaX;
        RandomBarrelIMX = RandomBarrelIMX - DeltaX;
        RandomBarrelIMY = RandomBarrelIMY - DeltaY;
        RandomBarrelIMZ = BarrelRandomImpactPosition.Z();
        }
      if(DetectorNumberB==3){
        RandomBarrelIMX = BarrelRandomImpactPosition.X();
        RandomBarrelIMY = BarrelRandomImpactPosition.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaY = (RandomNumber3 * 6.0)-3.0;
        RandomBarrelIMY = RandomBarrelIMY - DeltaY;
        RandomBarrelIMZ = BarrelRandomImpactPosition.Z();
        }
      if(DetectorNumberB==4){
        RandomBarrelIMX = BarrelRandomImpactPosition.X();
        RandomBarrelIMY = BarrelRandomImpactPosition.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 4.242640687)-2.121320344;
        double DeltaY = DeltaX;
        RandomBarrelIMX = RandomBarrelIMX + DeltaX;
        RandomBarrelIMY = RandomBarrelIMY - DeltaY;
        RandomBarrelIMZ = BarrelRandomImpactPosition.Z();
        }
      if(DetectorNumberB==5){
        RandomBarrelIMX = BarrelRandomImpactPosition.X();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 6.0)-3.0;
        RandomBarrelIMX = RandomBarrelIMX + DeltaX;
        RandomBarrelIMY = BarrelRandomImpactPosition.Y();
        RandomBarrelIMZ = BarrelRandomImpactPosition.Z();
        }
      if(DetectorNumberB==6){
        RandomBarrelIMX = BarrelRandomImpactPosition.X();
        RandomBarrelIMY = BarrelRandomImpactPosition.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 4.242640687)-2.121320344;
        double DeltaY = DeltaX;
        RandomBarrelIMX = RandomBarrelIMX + DeltaX;
        RandomBarrelIMY = RandomBarrelIMY + DeltaY;
        RandomBarrelIMZ = BarrelRandomImpactPosition.Z();
        }
      if(DetectorNumberB==7){
        RandomBarrelIMX = BarrelRandomImpactPosition.X();
        RandomBarrelIMY = BarrelRandomImpactPosition.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaY = (RandomNumber3 * 6.0)-3.0;
        RandomBarrelIMY = RandomBarrelIMY + DeltaY;
        RandomBarrelIMZ = BarrelRandomImpactPosition.Z();
        }
      if(DetectorNumberB==8){
        RandomBarrelIMX = BarrelRandomImpactPosition.X();
        RandomBarrelIMY = BarrelRandomImpactPosition.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 4.242640687)-2.121320344;
        double DeltaY = DeltaX;
        RandomBarrelIMX = RandomBarrelIMX - DeltaX;
        RandomBarrelIMY = RandomBarrelIMY + DeltaY;
        RandomBarrelIMZ = BarrelRandomImpactPosition.Z();
        }
      TiaraIMX = RandomBarrelIMX;
      TiaraIMY = RandomBarrelIMY;
      TiaraIMZ = RandomBarrelIMZ;
      /************************************************/
		} // end loop TiaraBarrel
////////////////////////////////////////////////////////////////

		if(ELab>0)
			RootOutput::getInstance()->GetTree()->Fill();
	} // loop over events

	cout << "A total of " << nentries << " event has been analysed " <<endl ;

	RootOutput::getInstance()->Destroy();
	RootInput::getInstance()->Destroy();
	NPOptionManager::getInstance()->Destroy();
	////////////////////////////////////////////////////////////////////////////
	return 0 ;
}
////////////////////////////////////////////////////////////////////////////
void InitOutputBranch() {
	RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
/*	RootOutput::getInstance()->GetTree()->Branch("Ring1Ex",&Ring1Ex,"Ring1Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring2Ex",&Ring2Ex,"Ring2Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring3Ex",&Ring3Ex,"Ring3Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring4Ex",&Ring4Ex,"Ring4Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring5Ex",&Ring5Ex,"Ring5Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring6Ex",&Ring6Ex,"Ring6Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring7Ex",&Ring7Ex,"Ring7Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring8Ex",&Ring8Ex,"Ring8Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring9Ex",&Ring9Ex,"Ring9Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring10Ex",&Ring10Ex,"Ring10Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring11Ex",&Ring11Ex,"Ring11Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring12Ex",&Ring12Ex,"Ring12Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring13Ex",&Ring13Ex,"Ring13Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring14Ex",&Ring14Ex,"Ring14Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring15Ex",&Ring15Ex,"Ring15Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("Ring16Ex",&Ring16Ex,"Ring16Ex/D"); */
	RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
	RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
	RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
/*	RootOutput::getInstance()->GetTree()->Branch("RandomHyballIMX",&RandomHyballIMX,"RandomHyballIMX/D");
	RootOutput::getInstance()->GetTree()->Branch("RandomHyballIMY",&RandomHyballIMY,"RandomHyballIMY/D");
	RootOutput::getInstance()->GetTree()->Branch("RandomHyballIMZ",&RandomHyballIMZ,"RandomHyballIMZ/D");
	RootOutput::getInstance()->GetTree()->Branch("RandomBarrelIMX",&RandomBarrelIMX,"RandomBarrelIMX/D");
	RootOutput::getInstance()->GetTree()->Branch("RandomBarrelIMY",&RandomBarrelIMY,"RandomBarrelIMY/D");
	RootOutput::getInstance()->GetTree()->Branch("RandomBarrelIMZ",&RandomBarrelIMZ,"RandomBarrelIMZ/D"); */
	RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixX",&TiaraIMX,"TiaraImpactMatrixX/D");
	RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixY",&TiaraIMY,"TiaraImpactMatrixY/D");
	RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixZ",&TiaraIMZ,"TiaraImpactMatrixZ/D");
}
/////////////////////////////////////////////////////////////////////////////
void InitInputBranch(){
	RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&Init );
	RootInput:: getInstance()->GetChain()->SetBranchStatus("InitialConditions",true );
	RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
}
/////////////////////////////////////////////////////////////////////////////     
void ReInitValue(){
	Ex = -1000 ;
	ELab = -1000;
	ThetaLab = -1000;
	ThetaCM = -1000;
}
/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////// LOOP on TiaraHyball + SSSD Hit //////////////////////////////////////////
		for(unsigned int countTiaraHyball = 0 ; countTiaraHyball < TH->Strip_E.size() ; countTiaraHyball++){
			/************************************************/

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

			// Part 2 : Impact Energy
			Energy = ELab = 0;
			Si_E_TH = TH->Strip_E[countTiaraHyball];
			Energy = Si_E_TH;

			// Evaluate energy using the thickness 
			ELab = proton_Al.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaTHSurface); 
			// Target Correction
			ELab = proton_CD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget); 

			/************************************************/

			// Part 3 : Excitation Energy Calculation
			Ex = P30dpReaction -> ReconstructRelativistic( ELab , ThetaLab );

			/************************************************/

			// Part 4 : Theta CM Calculation
			ThetaCM  = P30dpReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
		  ThetaLab=ThetaLab/deg;
	
      /************************************************/

      // Part 5 : Implementing impact matrix for the entire Hyball (all 6 wedges)
      /*TVector3 HyballImpactPosition = TH -> GetPositionOfInteraction(countTiaraHyball);
      HyballIMX = HyballImpactPosition.X();
      HyballIMY = HyballImpactPosition.Y();
      HyballIMZ = HyballImpactPosition.Z();*/

      /************************************************/

      // Part 6 : Implementing randomised position impact matrix for the Hyball
      TVector3 HyballRandomImpactPosition = TH -> GetRandomisedPositionOfInteraction(countTiaraHyball);
      TiaraIMX = HyballRandomImpactPosition.X();
      TiaraIMY = HyballRandomImpactPosition.Y();
      TiaraIMZ = HyballRandomImpactPosition.Z();

      /************************************************/

		} // end loop TiaraHyball

////////////////////////////////////////// LOOP on TiaraBarrel /////////////////////////////////////////////////////
		for(unsigned int countTiaraBarrel = 0 ; countTiaraBarrel < TB->Strip_E.size() ; countTiaraBarrel++){
			/************************************************/

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

			// Part 2 : Impact Energy
			Energy = ELab = 0;
			Si_E_TB = TB->Strip_E[countTiaraBarrel];
			Energy = Si_E_TB;

			// Evaluate energy using the thickness 
			ELab = proton_Al.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaTBSurface); 
			// Target Correction
			ELab = proton_CD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);

			/************************************************/

			// Part 3 : Excitation Energy Calculation
			Ex = P30dpReaction -> ReconstructRelativistic( ELab , ThetaLab );

		  /************************************************/

			// Part 4 : Theta CM Calculation
			ThetaCM  = P30dpReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
		  ThetaLab=ThetaLab/deg;
	
      /************************************************/      

      // Part 5 : Implementing impact matrix for the Tiara Barrel (all 8 detecting strips)
      /*TVector3 BarrelImpactPosition = TB -> GetPositionOfInteraction(countTiaraBarrel);
      BarrelIMX = BarrelImpactPosition.X();
      BarrelIMY = BarrelImpactPosition.Y();
      BarrelIMZ = BarrelImpactPosition.Z();*/

      /************************************************/

      // Part 6 : Implementing randomised position impact matrix for both the entire Barrel (all 8 strips) and each strip making up the octagonal Barrel individually
      TVector3 BarrelRandomImpactPosition = TB -> GetRandomisedPositionOfInteraction(countTiaraBarrel);
      TiaraIMX = BarrelRandomImpactPosition.X();
      TiaraIMY = BarrelRandomImpactPosition.Y();
      TiaraIMZ = BarrelRandomImpactPosition.Z();

      /************************************************/

		} // end loop TiaraBarrel
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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

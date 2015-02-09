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
      int RingNumber = TH -> Strip_Ring[countTiaraHyball];
      if(RingNumber==1){
        Ring1Ex = Ex;
        }
      if(RingNumber==2){
        Ring2Ex = Ex;
        }
      if(RingNumber==3){
        Ring3Ex = Ex;
        }
      if(RingNumber==4){
        Ring4Ex = Ex;
        }
      if(RingNumber==5){
        Ring5Ex = Ex;
        }
      if(RingNumber==6){
        Ring6Ex = Ex;
        }
      if(RingNumber==7){
        Ring7Ex = Ex;
        }
      if(RingNumber==8){
        Ring8Ex = Ex;
        }
      if(RingNumber==9){
        Ring9Ex = Ex;
        }
      if(RingNumber==10){
        Ring10Ex = Ex;
        }
      if(RingNumber==11){
        Ring11Ex = Ex;
        }
      if(RingNumber==12){
        Ring12Ex = Ex;
        }
      if(RingNumber==13){
        Ring13Ex = Ex;
        }
      if(RingNumber==14){
        Ring14Ex = Ex;
        }
      if(RingNumber==15){
        Ring15Ex = Ex;
        }
      if(RingNumber==16){
        Ring16Ex = Ex;
        }
		  /************************************************/

			/************************************************/
			// Part 4 : Theta CM Calculation
			ThetaCM  = P30dpReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
		  ThetaLab=ThetaLab/deg;
	
     /************************************************/

      /************************************************/
      // Part 5a : Implementing impact matrix for the entire Hyball (all 6 wedges)
      TVector3 ImpactPosition = TH -> GetPositionOfInteraction(countTiaraHyball);
      int DetectorNumber = TH -> DetectorNumber[countTiaraHyball];
      ImpactMatrixCoordX = ImpactPosition.X();
      ImpactMatrixCoordY = ImpactPosition.Y();
      ImpactMatrixCoordZ = ImpactPosition.Z();

      // Part 5b : Implementing impact matrix for each wedge in the Hyball individually
      if(DetectorNumber==1){
        TVector3 DetectorOnePOI = TH -> GetPositionOfInteraction(countTiaraHyball);
        Detector1CoordX = DetectorOnePOI.X();
        Detector1CoordY = DetectorOnePOI.Y();
        }
      if(DetectorNumber==2){
        TVector3 DetectorTwoPOI = TH -> GetPositionOfInteraction(countTiaraHyball);
        Detector2CoordX = DetectorTwoPOI.X();
        Detector2CoordY = DetectorTwoPOI.Y();
        }
      if(DetectorNumber==3){
        TVector3 DetectorThreePOI = TH -> GetPositionOfInteraction(countTiaraHyball);
        Detector3CoordX = DetectorThreePOI.X();
        Detector3CoordY = DetectorThreePOI.Y();
        }
      if(DetectorNumber==4){
        TVector3 DetectorFourPOI = TH -> GetPositionOfInteraction(countTiaraHyball);
        Detector4CoordX = DetectorFourPOI.X();
        Detector4CoordY = DetectorFourPOI.Y();
        }
      if(DetectorNumber==5){
        TVector3 DetectorFivePOI = TH -> GetPositionOfInteraction(countTiaraHyball);
        Detector5CoordX = DetectorFivePOI.X();
        Detector5CoordY = DetectorFivePOI.Y();
        }
      if(DetectorNumber==6){
        TVector3 DetectorSixPOI = TH -> GetPositionOfInteraction(countTiaraHyball);
        Detector6CoordX = DetectorSixPOI.X();
        Detector6CoordY = DetectorSixPOI.Y();
        }
      /************************************************/

      /************************************************/
      // Part 6a : Implementing randomised position impact matrix for the entire Hyball (all 6 wedges)
      TVector3 ImpactPositionR = TH -> GetPositionOfInteraction(countTiaraHyball); // gets the event's x, y and z coordinates and puts them into a vector entitled ImpactPositionR (for randomised)
      ImpactPositionR.SetZ(0.0);
      DetectorNumber = TH -> DetectorNumber[countTiaraHyball];
      double R = ImpactPositionR.Mag();
      double Theta = ImpactPositionR.Theta(); // defines Theta, the inclination coordinate in a spherical coordinate system
      double Phi = ImpactPositionR.Phi(); // defines Phi, the azimuthal coordinate in a spherical coordinate system
      double RandomNumber1 = Rand->Rndm();
      double DeltaR = ((RandomNumber1 * 6.4)-3.2);
      R = R + DeltaR; // randomises R within a given detector ring
      double RandomNumber2 = Rand->Rndm();
      double DeltaAngle = ((RandomNumber2 * 0.118682389)-0.0593411946);
      Phi = Phi + DeltaAngle; // randomises Phi within a given detector sector
      ImpactMatrixCoordRandomX = R*(sin(Theta))*(cos(Phi)); // defines the final randomised X coordinate by transforming the randomised spherical coordinates
      ImpactMatrixCoordRandomY = R*(sin(Phi))*(sin(Theta)); // defines the final randomised Y coordinate by transforming the randomised spherical coordinates
      
      // Part 6b : Implementing randomised position impact matrix for each wedge in the Hyball individually
      if(DetectorNumber==1){
        Detector1RandomCoordX = ImpactMatrixCoordRandomX;
        Detector1RandomCoordY = ImpactMatrixCoordRandomY;
        }
      if(DetectorNumber==2){
        Detector2RandomCoordX = ImpactMatrixCoordRandomX;
        Detector2RandomCoordY = ImpactMatrixCoordRandomY;
        }
      if(DetectorNumber==3){
        Detector3RandomCoordX = ImpactMatrixCoordRandomX;
        Detector3RandomCoordY = ImpactMatrixCoordRandomY;
        } 
      if(DetectorNumber==4){
        Detector4RandomCoordX = ImpactMatrixCoordRandomX;
        Detector4RandomCoordY = ImpactMatrixCoordRandomY;
        }
      if(DetectorNumber==5){
        Detector5RandomCoordX = ImpactMatrixCoordRandomX;
        Detector5RandomCoordY = ImpactMatrixCoordRandomY;
        }
      if(DetectorNumber==6){
        Detector6RandomCoordX = ImpactMatrixCoordRandomX;
        Detector6RandomCoordY = ImpactMatrixCoordRandomY;
        }
      
      /************************************************/
		} // end loop TiaraHyball*/

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
			ELab = proton_CD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget); // -> have I written these lines correctly? (they look different to the syntax used in Example1) Also, should this be the beam particle or the heavy projectile particle?
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
      TVector3 ImpactPositionB = TB -> GetPositionOfInteraction(countTiaraBarrel);
//      double xcheck = ImpactPositionB.X();
//      double ycheck = ImpactPositionB.Y();
//      cout << "event x value is " << xcheck << endl;
//      int StripNumberB = TB -> Strip_N[countTiaraBarrel];
//      cout << "Barrel resistive strip number is " << StripNumberB << endl;
      int DetectorNumberB = TB -> DetectorNumber[countTiaraBarrel];
//      cout << "Barrel detector number is " << DetectorNumberB << endl;
//      cout << "" << endl;
      ImpactMatrixBCoordX = ImpactPositionB.X();
      ImpactMatrixBCoordY = ImpactPositionB.Y();
      ImpactMatrixBCoordZ = ImpactPositionB.Z();

      // series of if statements to cross check whether all parts of the barrel are being activated

//      if(StripNumberB==1){
//        BStripNumber1X = ImpactPositionB.X();
//       BStripNumber1Y = ImpactPositionB.Y();
//        BStripNumber1Z = ImpactPositionB.Z();
//        }
//      if(StripNumberB==2){
//        BStripNumber2X = ImpactPositionB.X();
//        BStripNumber2Y = ImpactPositionB.Y();
//        BStripNumber2Z = ImpactPositionB.Z();
//        }
//      if(StripNumberB==3){
//        BStripNumber3X = ImpactPositionB.X();
//        BStripNumber3Y = ImpactPositionB.Y();
//        BStripNumber3Z = ImpactPositionB.Z();
//        }
//      if(StripNumberB==4){
//        BStripNumber4X = ImpactPositionB.X();
//        BStripNumber4Y = ImpactPositionB.Y();
//        BStripNumber4Z = ImpactPositionB.Z();
//        }

      // The next lines randomise the above impact matrix stuff. Next week I need to move this to Part 6a) (starting on line 433) whilst I tidy EVERYTHING up.

//      double RandomNumber3 = Rand->Rndm();
//      double DeltaX = (RandomNumber3 * 6.0);
//      ImpactMatrixBCoordX = ImpactMatrixBCoordX - DeltaX; // randomises X within a given barrel resistive strip

      // Part 5b : Implementing impact matrix for each strip making up the octagonal Barrel individually
      if(DetectorNumberB==1){
        BarrelStrip1CoordX = ImpactPositionB.X();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 6.0)-3.0;
        BarrelStrip1CoordX = BarrelStrip1CoordX - DeltaX; // randomises X within a given barrel resistive strip
        BarrelStrip1CoordY = ImpactPositionB.Y();
        BarrelStrip1CoordZ = ImpactPositionB.Z();
        }
      if(DetectorNumberB==2){
        BarrelStrip2CoordX = ImpactPositionB.X();
        BarrelStrip2CoordY = ImpactPositionB.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 4.242640687)-2.121320344;
        double DeltaY = DeltaX;
        BarrelStrip2CoordX = BarrelStrip2CoordX - DeltaX;
        BarrelStrip2CoordY = BarrelStrip2CoordY - DeltaY;
        BarrelStrip2CoordZ = ImpactPositionB.Z();
        }
      if(DetectorNumberB==3){
        BarrelStrip3CoordX = ImpactPositionB.X();
        BarrelStrip3CoordY = ImpactPositionB.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaY = (RandomNumber3 * 6.0)-3.0;
        BarrelStrip3CoordY = BarrelStrip3CoordY - DeltaY;
        BarrelStrip3CoordZ = ImpactPositionB.Z();
        }
      if(DetectorNumberB==4){
        BarrelStrip4CoordX = ImpactPositionB.X();
        BarrelStrip4CoordY = ImpactPositionB.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 4.242640687)-2.121320344;
        double DeltaY = DeltaX;
        BarrelStrip4CoordX = BarrelStrip4CoordX + DeltaX;
        BarrelStrip4CoordY = BarrelStrip4CoordY - DeltaY;
        BarrelStrip4CoordZ = ImpactPositionB.Z();
        }
      if(DetectorNumberB==5){
        BarrelStrip5CoordX = ImpactPositionB.X();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 6.0)-3.0;
        BarrelStrip5CoordX = BarrelStrip5CoordX + DeltaX;
        BarrelStrip5CoordY = ImpactPositionB.Y();
        BarrelStrip5CoordZ = ImpactPositionB.Z();
        }
      if(DetectorNumberB==6){
        BarrelStrip6CoordX = ImpactPositionB.X();
        BarrelStrip6CoordY = ImpactPositionB.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 4.242640687)-2.121320344;
        double DeltaY = DeltaX;
        BarrelStrip6CoordX = BarrelStrip6CoordX + DeltaX;
        BarrelStrip6CoordY = BarrelStrip6CoordY + DeltaY;
        BarrelStrip6CoordZ = ImpactPositionB.Z();
        }
      if(DetectorNumberB==7){
        BarrelStrip7CoordX = ImpactPositionB.X();
        BarrelStrip7CoordY = ImpactPositionB.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaY = (RandomNumber3 * 6.0)-3.0;
        BarrelStrip7CoordY = BarrelStrip7CoordY + DeltaY;
        BarrelStrip7CoordZ = ImpactPositionB.Z();
        }
      if(DetectorNumberB==8){
        BarrelStrip8CoordX = ImpactPositionB.X();
        BarrelStrip8CoordY = ImpactPositionB.Y();
        double RandomNumber3 = Rand->Rndm();
        double DeltaX = (RandomNumber3 * 4.242640687)-2.121320344;
        double DeltaY = DeltaX;
        BarrelStrip8CoordX = BarrelStrip8CoordX - DeltaX;
        BarrelStrip8CoordY = BarrelStrip8CoordY + DeltaY;
        BarrelStrip8CoordZ = ImpactPositionB.Z();
        }
      
      /************************************************/

      /************************************************/
      // Part 6a : Implementing randomised position impact matrix for the entire Hyball (all 6 wedges)
      /*TVector3 ImpactPositionR = TH -> GetPositionOfInteraction(countTiaraHyball); // gets the event's x, y and z coordinates and puts them into a vector entitled ImpactPositionR (for randomised)
      ImpactPositionR.SetZ(0.0);
      DetectorNumber = TH -> DetectorNumber[countTiaraHyball];
      double R = ImpactPositionR.Mag();
      double Theta = ImpactPositionR.Theta(); // defines Theta, the inclination coordinate in a spherical coordinate system
      double Phi = ImpactPositionR.Phi(); // defines Phi, the azimuthal coordinate in a spherical coordinate system
      double RandomNumber1 = Rand->Rndm();
      double DeltaR = ((RandomNumber1 * 6.4)-3.2);
      R = R + DeltaR; // randomises R within a given detector ring
      double RandomNumber2 = Rand->Rndm();
      double DeltaAngle = ((RandomNumber2 * 0.118682389)-0.0593411946);
      Phi = Phi + DeltaAngle; // randomises Phi within a given detector sector
      ImpactMatrixCoordRandomX = R*(sin(Theta))*(cos(Phi)); // defines the final randomised X coordinate by transforming the randomised spherical coordinates
      ImpactMatrixCoordRandomY = R*(sin(Phi))*(sin(Theta)); // defines the final randomised Y coordinate by transforming the randomised spherical coordinates
      
      // Part 6b : Implementing randomised position impact matrix for each wedge in the Hyball individually
      if(DetectorNumber==1){
        Detector1RandomCoordX = ImpactMatrixCoordRandomX;
        Detector1RandomCoordY = ImpactMatrixCoordRandomY;
        }
      if(DetectorNumber==2){
        Detector2RandomCoordX = ImpactMatrixCoordRandomX;
        Detector2RandomCoordY = ImpactMatrixCoordRandomY;
        }
      if(DetectorNumber==3){
        Detector3RandomCoordX = ImpactMatrixCoordRandomX;
        Detector3RandomCoordY = ImpactMatrixCoordRandomY;
        } 
      if(DetectorNumber==4){
        Detector4RandomCoordX = ImpactMatrixCoordRandomX;
        Detector4RandomCoordY = ImpactMatrixCoordRandomY;
        }
      if(DetectorNumber==5){
        Detector5RandomCoordX = ImpactMatrixCoordRandomX;
        Detector5RandomCoordY = ImpactMatrixCoordRandomY;
        }
      if(DetectorNumber==6){
        Detector6RandomCoordX = ImpactMatrixCoordRandomX;
        Detector6RandomCoordY = ImpactMatrixCoordRandomY;
        }
      */
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
	RootOutput::getInstance()->GetTree()->Branch("Ring1Ex",&Ring1Ex,"Ring1Ex/D");
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
	RootOutput::getInstance()->GetTree()->Branch("Ring16Ex",&Ring16Ex,"Ring16Ex/D");
	RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
	RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
	RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
  RootOutput::getInstance()->GetTree()->Branch("ImpactMatrixCoordX",&ImpactMatrixCoordX,"ImpactMatrixCoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("ImpactMatrixCoordY",&ImpactMatrixCoordY,"ImpactMatrixCoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("ImpactMatrixCoordZ",&ImpactMatrixCoordZ,"ImpactMatrixCoordZ/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector1CoordX",&Detector1CoordX,"Detector1CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector1CoordY",&Detector1CoordY,"Detector1CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector2CoordX",&Detector2CoordX,"Detector2CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector2CoordY",&Detector2CoordY,"Detector2CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector3CoordX",&Detector3CoordX,"Detector3CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector3CoordY",&Detector3CoordY,"Detector3CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector4CoordX",&Detector4CoordX,"Detector4CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector4CoordY",&Detector4CoordY,"Detector4CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector5CoordX",&Detector5CoordX,"Detector5CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector5CoordY",&Detector5CoordY,"Detector5CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector6CoordX",&Detector6CoordX,"Detector6CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector6CoordY",&Detector6CoordY,"Detector6CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("ImpactMatrixCoordRandomX",&ImpactMatrixCoordRandomX,"ImpactMatrixCoordRandomX/D");
  RootOutput::getInstance()->GetTree()->Branch("ImpactMatrixCoordRandomY",&ImpactMatrixCoordRandomY,"ImpactMatrixCoordRandomY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector1RandomCoordX",&Detector1RandomCoordX,"Detector1RandomCoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector1RandomCoordY",&Detector1RandomCoordY,"Detector1RandomCoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector2RandomCoordX",&Detector2RandomCoordX,"Detector2RandomCoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector2RandomCoordY",&Detector2RandomCoordY,"Detector2RandomCoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector3RandomCoordX",&Detector3RandomCoordX,"Detector3RandomCoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector3RandomCoordY",&Detector3RandomCoordY,"Detector3RandomCoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector4RandomCoordX",&Detector4RandomCoordX,"Detector4RandomCoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector4RandomCoordY",&Detector4RandomCoordY,"Detector4RandomCoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector5RandomCoordX",&Detector5RandomCoordX,"Detector5RandomCoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector5RandomCoordY",&Detector5RandomCoordY,"Detector5RandomCoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector6RandomCoordX",&Detector6RandomCoordX,"Detector6RandomCoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("Detector6RandomCoordY",&Detector6RandomCoordY,"Detector6RandomCoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("ImpactMatrixBCoordX",&ImpactMatrixBCoordX,"ImpactMatrixBCoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("ImpactMatrixBCoordY",&ImpactMatrixBCoordY,"ImpactMatrixBCoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("ImpactMatrixBCoordZ",&ImpactMatrixBCoordZ,"ImpactMatrixBCoordZ/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip1CoordX",&BarrelStrip1CoordX,"BarrelStrip1CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip1CoordY",&BarrelStrip1CoordY,"BarrelStrip1CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip1CoordZ",&BarrelStrip1CoordZ,"BarrelStrip1CoordZ/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip2CoordX",&BarrelStrip2CoordX,"BarrelStrip2CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip2CoordY",&BarrelStrip2CoordY,"BarrelStrip2CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip2CoordZ",&BarrelStrip2CoordZ,"BarrelStrip2CoordZ/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip3CoordX",&BarrelStrip3CoordX,"BarrelStrip3CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip3CoordY",&BarrelStrip3CoordY,"BarrelStrip3CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip3CoordZ",&BarrelStrip3CoordZ,"BarrelStrip3CoordZ/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip4CoordX",&BarrelStrip4CoordX,"BarrelStrip4CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip4CoordY",&BarrelStrip4CoordY,"BarrelStrip4CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip4CoordZ",&BarrelStrip4CoordZ,"BarrelStrip4CoordZ/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip5CoordX",&BarrelStrip5CoordX,"BarrelStrip5CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip5CoordY",&BarrelStrip5CoordY,"BarrelStrip5CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip5CoordZ",&BarrelStrip5CoordZ,"BarrelStrip5CoordZ/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip6CoordX",&BarrelStrip6CoordX,"BarrelStrip6CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip6CoordY",&BarrelStrip6CoordY,"BarrelStrip6CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip6CoordZ",&BarrelStrip6CoordZ,"BarrelStrip6CoordZ/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip7CoordX",&BarrelStrip7CoordX,"BarrelStrip7CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip7CoordY",&BarrelStrip7CoordY,"BarrelStrip7CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip7CoordZ",&BarrelStrip7CoordZ,"BarrelStrip7CoordZ/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip8CoordX",&BarrelStrip8CoordX,"BarrelStrip8CoordX/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip8CoordY",&BarrelStrip8CoordY,"BarrelStrip8CoordY/D");
  RootOutput::getInstance()->GetTree()->Branch("BarrelStrip8CoordZ",&BarrelStrip8CoordZ,"BarrelStrip8CoordZ/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber1X",&BStripNumber1X,"BStripNumber1X/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber1Y",&BStripNumber1Y,"BStripNumber1Y/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber1Z",&BStripNumber1Z,"BStripNumber1Z/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber2X",&BStripNumber2X,"BStripNumber2X/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber2Y",&BStripNumber2Y,"BStripNumber2Y/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber2Z",&BStripNumber2Z,"BStripNumber2Z/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber3X",&BStripNumber3X,"BStripNumber3X/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber3Y",&BStripNumber3Y,"BStripNumber3Y/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber3Z",&BStripNumber3Z,"BStripNumber3Z/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber4X",&BStripNumber4X,"BStripNumber4X/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber4Y",&BStripNumber4Y,"BStripNumber4Y/D");
  RootOutput::getInstance()->GetTree()->Branch("BStripNumber4Z",&BStripNumber4Z,"BStripNumber4Z/D");
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

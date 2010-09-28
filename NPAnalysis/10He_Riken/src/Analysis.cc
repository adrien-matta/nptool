#include "ObjectManager.hh"

using namespace std;

int main(int argc,char** argv)
{	
	 
	if(argc!=4) 
		{
			cout << 
			"you need to specify both a Reaction file and a Detector file such as : Analysis 	myReaction.reaction myDetector.detector runToRead.run" 
			<< endl;
			return 0;
		} 
	
	string detectorfileName 		= argv[1]	;
	string calibrationfileName 	= argv[2]	;
	string runToReadfileName 		= argv[3]	;
	
	//	First of All instantiate RootInput and Output
	//	Detector will be attached later
	RootInput:: getInstance(runToReadfileName)	;
	RootOutput::getInstance("Analysis/10HeRiken_AnalyzedData", "AnalyzedTree")					;
	
	//	Instantiate some Reaction
	NPL::Reaction*  He10Reaction = new Reaction								;
	He10Reaction	->	ReadConfigurationFile("10He.reaction")	;

	NPL::Reaction*  Li10Reaction = new Reaction								;
	Li10Reaction	->	ReadConfigurationFile("9Li-dp-10Li.reaction")		;

	//	Instantiate the detector using a file 
	NPA::DetectorManager* myDetector = new DetectorManager 			  ;
	myDetector	->	ReadConfigurationFile(detectorfileName)		;

	//	Instantiate the Calibration Manger using a file
	CalibrationManager* myCalibration = CalibrationManager::getInstance(calibrationfileName) ;
	
	//	Attach more branch to the output
	
	double ELab[2], ExcitationEnergy[2]	;
	double ThetaLab[2]	, ThetaCM[2]		;
	double X[2] , Y[2] 									;
	double EventWeight=0;

	// Exitation Energy
	RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",ExcitationEnergy,"ExcitationEnergy[2]/D") ;

	//E lab et Theta lab
	RootOutput::getInstance()->GetTree()->Branch("ThetaCM",ThetaCM,"ThetaCM[2]/D") 	;
	RootOutput::getInstance()->GetTree()->Branch("ELab",ELab,"ELab[2]/D") 					;
	RootOutput::getInstance()->GetTree()->Branch("ThetaLab",ThetaLab,"ThetaLab[2]/D") 					;
	RootOutput::getInstance()->GetTree()->Branch("X",X,"X/D[2]") 										;
	RootOutput::getInstance()->GetTree()->Branch("Y",Y,"Y/D[2]")									 	;
	
	// For phasespace event
	RootOutput::getInstance()->GetTree()->Branch("EventWeight",&EventWeight,"EventWeight/D")									 	;
	
	//	Get the formed Chained Tree and Treat it
	TChain* Chain = RootInput:: getInstance() -> GetChain()	;

	// Open the ThinSi Branch
	Chain->SetBranchStatus("EventWeight",true);
	Chain->SetBranchAddress("EventWeight"	,&EventWeight		);
	
	Chain->SetBranchStatus("InitialConditions",true)	;
	Chain->SetBranchStatus("fIC_*",true)	; 
 		
	TInitialConditions* Init = new TInitialConditions();
	Chain->SetBranchAddress("InitialConditions"	,&Init		);
	
	TInteractionCoordinates* ICoord = new TInteractionCoordinates();
	Chain->SetBranchAddress("InteractionCoordinates"	,&ICoord		);
	
	
	
 	double XTarget=0 ; double YTarget=0; double ZTarget = 0 ; double BeamTheta = 0 ; double BeamPhi = 0 ; double E=-1000;

	// Get Detector Pointer:
	TMust2Physics* M2 		= (TMust2Physics*) 			myDetector -> m_Detector["MUST2"] 	;
	TPlasticPhysics* Pl 	= (TPlasticPhysics*) 		myDetector -> m_Detector["Plastic"] ;
	TSSSDPhysics* ThinSi 	= (TSSSDPhysics*) 			myDetector -> m_Detector["SSSD"] 		;


RootOutput::getInstance()->GetList()->Add(myHist1D);

   TCutG *cut3He_MUST2 = new TCutG("Cut3HeMUST2",11);
   cut3He_MUST2->SetPoint(0,-1.49426,24.2781);
   cut3He_MUST2->SetPoint(1,15.3161,14.3151);
   cut3He_MUST2->SetPoint(2,47.069,7.6732);
   cut3He_MUST2->SetPoint(3,110.575,4.35222);
   cut3He_MUST2->SetPoint(4,308.563,2.25477);
   cut3He_MUST2->SetPoint(5,310.431,1.20604);
   cut3He_MUST2->SetPoint(6,232.917,1.11864);
   cut3He_MUST2->SetPoint(7,89.0948,2.42955);
   cut3He_MUST2->SetPoint(8,32.1264,6.71186);
   cut3He_MUST2->SetPoint(9,1.30747,11.9555);
   cut3He_MUST2->SetPoint(10,-1.49426,24.2781);
   
   TCutG *cut3He_M2_SSSD = new TCutG("Cut3HeM2SSSD",11);
   cut3He_M2_SSSD->SetPoint(0,7.44252,1.45432);
   cut3He_M2_SSSD->SetPoint(1,40.6322,0.684984);
   cut3He_M2_SSSD->SetPoint(2,95.9483,0.406912);
   cut3He_M2_SSSD->SetPoint(3,218.649,0.216896);
   cut3He_M2_SSSD->SetPoint(4,362.471,0.101033);
   cut3He_M2_SSSD->SetPoint(5,355.431,0.0315148);
   cut3He_M2_SSSD->SetPoint(6,100.977,0.0315148);
   cut3He_M2_SSSD->SetPoint(7,25.546,0.314221);
   cut3He_M2_SSSD->SetPoint(8,3.41954,0.995498);
   cut3He_M2_SSSD->SetPoint(9,3.41954,1.40797);
   cut3He_M2_SSSD->SetPoint(10,7.44252,1.45432);

	cout <<  " ///////// Starting Analysis ///////// "<< endl << endl ;	
	int i ,N=Chain -> GetEntries();
	
	cout << " Number of Event to be treated : " << N << endl ;
	clock_t begin=clock();
	clock_t end=begin;
	for ( i = 0 ; i < N ;	 i ++ )
		{
			// Clear local branch
			for(int hh = 0 ; hh <2 ; hh++)
				{
					ELab[hh] = -10000 	; ExcitationEnergy[hh] = -10000 ; ThetaLab[hh] = -10000 ;	
					X[hh]	 = -10000	; Y[hh]					= -10000 ; ThetaCM[hh]  = -10000 ;
				}

			// Minimum code
			if( i%10000 == 0 && i!=0) 	{	
											cout.precision(5);
											end=clock();										
											double TimeElapsed = (end-begin)/CLOCKS_PER_SEC;
											double percent = (double)i/N ;
											double TimeToWait = (TimeElapsed/percent) - TimeElapsed	;					
											cout << "\r Progression:" << percent*100 
												 << " % \t | \t Remaining time : ~" 
												 <<  TimeToWait <<"s"<< flush;
										}	
										
			else if (i==N-1) 	cout << "\r Progression:" 
								 << " 100% " <<endl;
					
			Chain -> GetEntry(i);
			// Clear Previous Event
			myDetector -> ClearEventPhysics()				;
			// Build the new one
			myDetector -> BuildPhysicalEvent()				;
			////
			// Target (from initial condition)
			XTarget = Init->GetICPositionX(0);
			YTarget = Init->GetICPositionY(0);
			ZTarget = Init->GetICPositionZ(0);
			//	XTarget = RandomEngine.Gaus(Init->GetICPositionX(0),1);
			//	YTarget = RandomEngine.Gaus(Init->GetICPositionY(0),1);
			BeamTheta = Init->GetICIncidentAngleTheta(0)*deg ; BeamPhi = Init->GetICIncidentAnglePhi(0)*deg ; 
			TVector3 BeamDirection = TVector3(cos(BeamPhi)*sin(BeamTheta) , sin(BeamPhi)*sin(BeamTheta) , cos(BeamTheta)) ;
			//// 
			
			
			// Must 2 And ThinSi //
			for(int hit = 0; hit < M2 -> Si_E.size() ; hit ++)
				{
					//	Get Hit Direction
					TVector3 HitDirection  = M2 -> GetPositionOfInteraction(hit) - TVector3(XTarget,YTarget,0);
					// Angle between beam and particle
					ThetaLab[hit]  = ThetaCalculation ( HitDirection , BeamDirection   ) ;		
					
					double Xint = ICoord-> GetDetectedPositionX(hit)  - M2 -> GetPositionOfInteraction(hit).X();
					double Yint = ICoord-> GetDetectedPositionY(hit)  - M2 -> GetPositionOfInteraction(hit).Y();
					double Zint = ICoord-> GetDetectedPositionZ(hit)  - M2 -> GetPositionOfInteraction(hit).Z();
					
					
//				cout << Xint << "  " << Yint << " " << Zint << endl;
					
					// Angle between particule and z axis (target Normal)
					double ThetaN = ThetaCalculation ( HitDirection , TVector3(0,0,1) ) ;
					// Angle between particule and Must2 Si surface
					double ThetaMM2Surface = ThetaCalculation ( HitDirection , M2 -> GetTelescopeNormal(hit) );

					if(M2 -> GetPositionOfInteraction(hit).Z() > 0)
						{
							if( M2 -> CsI_E[hit] < 0 && M2 -> Si_E[hit] > 0 )
								{  
										ELab[hit] = M2 -> Si_E[hit]  ; 
										
										ELab[hit]= He3StripAl.EvaluateInitialEnergy(			ELab[hit] 				, // Energy of the detected particle
																																			0.4*micrometer	, // Target Thickness at 0 degree
																																			ThetaMM2Surface		);														
										
										if(ThinSi -> Energy.size() > 0)
											{
												ELab[hit]= He3StripAl.EvaluateInitialEnergy(	ELab[hit] 				, // Energy of the detected particle
																																			0.4*micrometer		, // Target Thickness at 0 degree
																																			ThetaMM2Surface		);
												ELab[hit] += ThinSi-> Energy[hit];
												ELab[hit]= He3StripAl.EvaluateInitialEnergy(	ELab[hit] 				, // Energy of the detected particle
																																			0.4*micrometer		, // Target Thickness at 0 degree
																																			ThetaMM2Surface		);
											} 

										ELab[hit]= He3TargetWind.EvaluateInitialEnergy( 	ELab[hit] 				, // Energy of the detected particle
																																			0*micrometer			, // Target Thickness at 0 degree
																																			ThetaN						);
																			
										ELab[hit]= He3TargetGaz.EvaluateInitialEnergy(		ELab[hit] 				, // Energy of the detected particle
																																			3*micrometer						, // Target Thickness at 0 degree
																																			ThetaN						);
																		 				
									ThetaCM[hit] = He10Reaction -> EnergyLabToThetaCM( ELab[hit] ) /deg 	;
									ExcitationEnergy[hit] = He10Reaction -> ReconstructRelativistic( ELab[hit] , ThetaLab[hit] ) 		;	
									
									if(ThinSi -> Energy.size() > 0 )
								  	if(cut3He_M2_SSSD->IsInside(ThinSi -> Energy[hit], M2 -> Si_E[hit]) )
											myHist1D->Fill(ExcitationEnergy[hit],EventWeight);
									
									X[hit] = HitDirection . X();
									Y[hit] = HitDirection . Y();	
									ThetaLab[hit] = ThetaLab[hit] / deg ;
								}
 
							else if(M2 ->CsI_E[hit] > 0 && M2 -> Si_E[hit] > 0)
								{
					
									ELab[hit]= M2 ->CsI_E[hit] ;
								
									ELab[hit]= He3TargetWind.EvaluateInitialEnergy( 	ELab[hit] 					, // Energy of the detected particle
																																		3*micrometer				, // Target Thickness at 0 degree
																																		ThetaMM2Surface			);
																																	
									ELab[hit]= He3StripAl.EvaluateInitialEnergy(			ELab[hit] 					, // Energy of the detected particle
																																		0.4*micrometer			, // Target Thickness at 0 degree
																																		ThetaMM2Surface			);
									ELab[hit]+= M2 ->Si_E[hit];
									
									ELab[hit]= He3StripAl.EvaluateInitialEnergy(			ELab[hit] 					, // Energy of the detected particle
																																		0.4*micrometer			, // Target Thickness at 0 degree
																																		ThetaMM2Surface			);
								
									if(ThinSi -> Energy.size() > 0)
										{
											ELab[hit]= He3StripAl.EvaluateInitialEnergy(	ELab[hit] 					, // Energy of the detected particle
																																		0.4*micrometer			, // Target Thickness at 0 degree
																																		ThetaMM2Surface			);
											ELab[hit] += ThinSi-> Energy[hit];
											ELab[hit]= He3StripAl.EvaluateInitialEnergy(	ELab[hit] 					, // Energy of the detected particle
																																		0.4*micrometer			, // Target Thickness at 0 degree
																																		ThetaMM2Surface			);
										}
											
									ELab[hit]= He3TargetWind.EvaluateInitialEnergy( 	ELab[hit] 					, // Energy of the detected particle
																																		0*micrometer				, // Target Thickness at 0 degree
																																		ThetaN							);
									
									ELab[hit]= He3TargetGaz.EvaluateInitialEnergy(		ELab[hit] 					, // Energy of the detected particle
																																		3*micrometer							, // Target Thickness at 0 degree
																																		ThetaN							);
																																		
									ThetaCM[hit]= He10Reaction -> EnergyLabToThetaCM( ELab[hit] ) /deg 	;	
									ExcitationEnergy[hit] = He10Reaction -> ReconstructRelativistic( ELab[hit], ThetaLab[hit]) ;	
										
									if( cut3He_MUST2->IsInside(M2 -> Si_E[hit], M2 -> CsI_E[hit]) )
										myHist1D->Fill(ExcitationEnergy[hit],EventWeight);
										
									X[hit] = HitDirection . X();
									Y[hit] = HitDirection . Y();	
									ThetaLab[hit] = ThetaLab[hit] / deg ;
								}	
							
						else {ExcitationEnergy[hit]=-100 ; X[hit] = -100 ; Y[hit]= -100 ; ThetaLab[hit]=-100;ThetaCM[hit]=-100;}
						
						}
						
					/*else if(M2 -> GetPositionOfInteraction(hit).Z()<0)
						{}	*/

				}			
			RootOutput::getInstance()->GetTree()->Fill()	;
		}

	cout << " A total of " << i << " event has been annalysed " << endl ;
	cout << endl << "/////////////////////////////////"<< endl<< endl ;
	myHist1D->Write();
	RootOutput::getInstance()->Destroy();
	return 0	;
}

double ThetaCalculation (TVector3 A , TVector3 B)
	{
		double Theta = acos( (A.Dot(B)) / (A.Mag()*B.Mag()) ) ;
		return Theta ;
	}


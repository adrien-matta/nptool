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
	
	string reactionfileName 	= argv[1]	;
	string detectorfileName 	= argv[2]	;
	string runToReadfileName 	= argv[3]	;
	
	//	First of All instantiate RootInput and Output
	//	Detector will be attached later
	RootInput:: getInstance(runToReadfileName)	;
	RootOutput::getInstance("Analysis/10HeRiken_AnalyzedData", "AnalyzedTree")					;
	
	//	Instantiate a Reaction
	NPL::Reaction* myReaction = new Reaction					;
	myReaction	->	ReadConfigurationFile(reactionfileName)		;

		//	Instantiate the detector using a file 
	DetectorManager* myDetector = new DetectorManager 			;
	myDetector	->	ReadConfigurationFile(detectorfileName)		;
	
	//	Attach more branch to the output
	double ThinSi=-1 ;double Ex = 0 ; double EE = 0 ; double TT = 0 ; double X = 0 ; double Y = 0 ; int det ; double ResolThetaCM=0; double FullTheta = 0 ;
	double ThetaCM=0; double EPl=0; double DEPl=0 ;
	RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",&Ex,"Ex/D") ;
	RootOutput::getInstance()->GetTree()->Branch("E",&EE,"EE/D") ;
	RootOutput::getInstance()->GetTree()->Branch("A",&TT,"TT/D") ;
	RootOutput::getInstance()->GetTree()->Branch("X",&X,"X/D") ;
	RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D") ;
	RootOutput::getInstance()->GetTree()->Branch("ThinSi_E",&ThinSi,"ThinSi/D") ;
	RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D") ;
	RootOutput::getInstance()->GetTree()->Branch("ResolThetaCM",&ResolThetaCM,"ResolThetaCM/D") ;
	RootOutput::getInstance()->GetTree()->Branch("FullTheta",&FullTheta,"FullTheta/D") ;
	RootOutput::getInstance()->GetTree()->Branch("EPl",&EPl,"EPl/D") ;
	RootOutput::getInstance()->GetTree()->Branch("DEPl",&DEPl,"DEPl/D") ;
	
	//	Get the formed Chained Tree and Treat it
	TChain* Chain = RootInput:: getInstance() -> GetChain()	;
 	   
	// Open the ThinSi Branch
	Chain->SetBranchStatus("ThinSiEnergy",true)	;
	Chain->SetBranchStatus("InitialConditions",true)	;
	Chain->SetBranchStatus("fIC_*",true)	; 
 	Chain->SetBranchStatus("fPlastic_*",true);
 		
	TInitialConditions* Init = new TInitialConditions();
	Chain->SetBranchAddress("ThinSiEnergy"		,&ThinSi	);
	Chain->SetBranchAddress("InitialConditions"	,&Init		);
	
	TPlasticData* Plastic = new TPlasticData() ;
	Chain->SetBranchAddress("Plastic",&Plastic	);
	
 double XTarget=0 ; double YTarget=0; double BeamTheta = 0 ; double BeamPhi = 0 ; double E=-1000;

	// Get Must2 Pointer:
	MUST2Array* M2 = (MUST2Array*) myDetector -> m_Detector["MUST2"] ;
	
	int i;
	for ( i = 0 ; i < Chain -> GetEntries() ; i ++ )
		{
			if( i%100000 == 0 && i!=0) {cout << i << " Event annalysed "<<endl;	}					
			Chain -> GetEntry(i);
			
			// Plastic
			if(Plastic->GetEnergySize()==2)
				{
					DEPl=Plastic->GetEnergy(0);
					EPl=Plastic->GetEnergy(1);
				}
			
			else
				{
					EPl	 =0	;
					DEPl =0	;
				}
				
			
	/*		myDetector -> ClearEventPhysics()				;
			myDetector -> BuildPhysicalEvent()				;
			FullTheta = Init->GetICEmittedAngleThetaLabWorldFrame(0);
			E = M2 -> GetEnergyDeposit();
			XTarget = Init->GetICPositionX(0);
			YTarget = Init->GetICPositionY(0);
//			XTarget = RandomEngine.Gaus(Init->GetICPositionX(0),1);
//			YTarget = RandomEngine.Gaus(Init->GetICPositionY(0),1);
			
			TVector3 HitDirection  = M2 -> GetPositionOfInteraction() - TVector3(XTarget,YTarget,0);
			
//			BeamTheta = RandomEngine.Gaus( Init->GetICIncidentAngleTheta(0)*deg , 2*deg ) ;
//			BeamPhi   = RandomEngine.Gaus( Init->GetICIncidentAnglePhi(0)*deg   , 2*deg ) ;
			 
			BeamTheta = Init->GetICIncidentAngleTheta(0)*deg ; BeamPhi = Init->GetICIncidentAnglePhi(0)*deg ; 

			TVector3 BeamDirection = TVector3(cos(BeamPhi)*sin(BeamTheta) , sin(BeamPhi)*sin(BeamTheta) , cos(BeamTheta)) ;	
			// Angle between beam and particle
			double Theta  = ThetaCalculation ( HitDirection , BeamDirection   ) ;				
			// Angle between particule and z axis (target Normal)
			double ThetaN = ThetaCalculation ( HitDirection , TVector3(0,0,1) ) ;
			// ANgle between particule and Must2 Si surface
			double ThetaMM2Surface = ThetaCalculation ( HitDirection , M2 -> GetTelescopeNormal() );
			if(M2 -> GetPositionOfInteraction().Z()>0)
				{
					if(E>-1000 && ThinSi>0 )	
					{
							E= He3StripAl.EvaluateInitialEnergy(	E 					, // Energy of the detected particle
																	2*0.4*micrometer	, // Target Thickness at 0 degree
																	ThetaMM2Surface		);
																	
							E= He3StripSi.EvaluateInitialEnergy(	E 					, // Energy of the detected particle
																	20*micrometer		, // Target Thickness at 0 degree
																	ThetaMM2Surface		);																
						
//							E = E + ThinSi ;
							
							E= He3StripAl.EvaluateInitialEnergy(	E 					, // Energy of the detected particle
																	0.4*micrometer		, // Target Thickness at 0 degree
																	ThetaMM2Surface		);

						//	cout << E << "  " << Eb-E << " " << ThinSi << endl ;

							E= He3TargetWind.EvaluateInitialEnergy( 		E 					, // Energy of the detected particle
																	 		15*micrometer		, // Target Thickness at 0 degree
																			ThetaN				);
																
							E= He3TargetGaz.EvaluateInitialEnergy(			E 					, // Energy of the detected particle
																			1.5*mm				, // Target Thickness at 0 degree
																			ThetaN				);
																			
						ThetaCM = myReaction -> EnergyLabToThetaCM( E , 1 ) /deg 	;
						ResolThetaCM =ThetaCM - Init->GetICEmittedAngleThetaCM(0) 	;
						Ex = myReaction -> ReconstructRelativistic( E , Theta ) 	;	
						X = HitDirection . X();
						Y = HitDirection . Y();	
					}

				else if(E>-1000 )
					{
					if(E>18)//CsI are inside a Mylar foil, plus rear alu strip
					{
						E= He3TargetWind.EvaluateInitialEnergy( E 					, // Energy of the detected particle
																3*micrometer		, // Target Thickness at 0 degree
																ThetaMM2Surface		);
						E= He3StripAl.EvaluateInitialEnergy(	E 					, // Energy of the detected particle
																0.4*micrometer		, // Target Thickness at 0 degree
																ThetaMM2Surface		);
					}
				
					E= He3StripAl.EvaluateInitialEnergy(	E 					, // Energy of the detected particle
															0.4*micrometer		, // Target Thickness at 0 degree
															ThetaMM2Surface		);
				
					E= He3TargetWind.EvaluateInitialEnergy( E 					, // Energy of the detected particle
															15*micrometer		, // Target Thickness at 0 degree
															ThetaN				);
					
					E= He3TargetGaz.EvaluateInitialEnergy(	E 					, // Energy of the detected particle
															1.5*mm				, // Target Thickness at 0 degree
															ThetaN				);
					ThetaCM = myReaction -> EnergyLabToThetaCM( E , 1 ) /deg 	;	
					Ex = myReaction -> ReconstructRelativistic( E, Theta ) ;	
					X = HitDirection . X();
					Y = HitDirection . Y();	
					
					}	
					
				else {Ex=-100 ; X = -100 ; Y = -100 ;}
				
				}
				
			if(M2 -> GetPositionOfInteraction().Z()<0)
				{
					
				if(E>-1000 )
					{
						if(E>18)
							{
								E= protonStripAl.EvaluateInitialEnergy(	E 					, // Energy of the detected particle
																		0.4*micrometer		, // Target Thickness at 0 degree
																		ThetaMM2Surface		);
							}
					
						E= protonStripAl.EvaluateInitialEnergy(		E 					, // Energy of the detected particle
																	0.4*micrometer		, // Target Thickness at 0 degree
																	ThetaMM2Surface		);
					
						E= protonTargetWind.EvaluateInitialEnergy( 	E 					, // Energy of the detected particle
																	15*micrometer		, // Target Thickness at 0 degree
																	ThetaN				);
						
						E= protonTargetGaz.EvaluateInitialEnergy(	E 					, // Energy of the detected particle
																	1.5*mm				, // Target Thickness at 0 degree
																	ThetaN				);
						ThetaCM = myReaction -> EnergyLabToThetaCM( E , 1 ) /deg 	;	
						Ex = myReaction -> ReconstructRelativistic( E, Theta ) ;	
						X = HitDirection . X();
						Y = HitDirection . Y();	
					
					}	
					
				else {Ex=-100 ; X = -100 ; Y = -100 ;}
				
				}	
				
								
			EE = E ; TT = Theta/deg ;
			*/
			RootOutput::getInstance()->GetTree()->Fill()	;
			ThinSi = -1 ;
		}
	cout << "A total of " << i << " event has been annalysed " << endl ;
	
	RootOutput::getInstance()->Destroy();
	return 0	;
}


double ThetaCalculation (TVector3 A , TVector3 B)
	{
		double Theta = acos( (A.Dot(B)) / (A.Mag()*B.Mag()) ) ;
		return Theta*rad ;
	}


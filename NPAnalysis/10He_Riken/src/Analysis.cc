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
	double ThinSi=-1 ;double Ex = 0 ; double EE = 0 ; double TT = 0 ; double X = 0 ; double Y = 0 ; int det ; double ResolTheta=0;
	RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",&Ex,"Ex/D") ;
	RootOutput::getInstance()->GetTree()->Branch("E",&EE,"EE/D") ;
	RootOutput::getInstance()->GetTree()->Branch("A",&TT,"TT/D") ;
	RootOutput::getInstance()->GetTree()->Branch("X",&X,"X/D") ;
	RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D") ;
	RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D") ;
	RootOutput::getInstance()->GetTree()->Branch("ThinSi_E",&ThinSi,"ThinSi/D") ;
	
	//	Get the formed Chained Tree and Treat it
	TChain* Chain = RootInput:: getInstance() -> GetChain()	;
	
	// Open the ThinSi Branch
	Chain->SetBranchStatus("ThinSiEnergy",true)	;
	Chain->SetBranchStatus("InitialConditions",true)	;
	Chain->SetBranchStatus("fIC_*",true)	;

	 TInitialConditions* Init = new TInitialConditions();
	Chain->SetBranchAddress("ThinSiEnergy"		,&ThinSi	);
	Chain->SetBranchAddress("InitialConditions"	,&Init		);
	
	
 double TargetX=0 ; double TargetY=0; double BeamTheta = 0 ; double BeamPhi = 0 ;
double TrueE=0 ; double TrueTheta=0 ;

	// Get Must2 Pointer:
	MUST2Array* M2 = (MUST2Array*) myDetector -> m_Detector["MUST2"] ;
	
	int i;	
	for ( i = 0 ; i < Chain -> GetEntries() ; i ++ )
		{
			if( i%10000 == 0 && i!=0) cout << i << " Event annalysed " << endl ;						
			Chain -> GetEntry(i);
			myDetector -> ClearEventPhysics()				;
			myDetector -> BuildPhysicalEvent()				;
			
			double E = M2 -> GetEnergyDeposit();
			TVector3 HitDirection  = M2 -> GetPositionOfInteraction() - TVector3(Init->GetICPositionX(0),Init->GetICPositionY(0),0);
			
			BeamTheta = Init->GetICIncidentAngleTheta(0)*deg ; BeamPhi = Init->GetICIncidentAnglePhi(0)*deg ; 

//			TVector3 BeamDirection = TVector3(cos(BeamPhi)*sin(BeamTheta) , sin(BeamPhi)*sin(BeamTheta) , cos(BeamTheta)) ;	
			TVector3 BeamDirection = TVector3(0,0,1) ;  BeamDirection.SetTheta(BeamTheta) ; BeamDirection.SetPhi(BeamPhi) ;
			double Theta = ThetaCalculation ( HitDirection , BeamDirection ) ;				

			if(E>-1000 && ThinSi>0 )	
				{
						
						E=E+ThinSi;
						
//						E= He3StripAl.EvaluateInitialEnergy(		E 					, // Energy of the detected particle
//																	2*2*0.4*micrometer	, // One for ThinSi and one for Must
//																	0					);	
							
//						E= He3StripAl.EvaluateInitialEnergy(		E 					, // Energy of the detected particle
//																	2*0.4*micrometer	, // Target Thickness at 0 degree
//																	0					);
//				
//						E= He3TargetWind.EvaluateInitialEnergy( 	E 					, // Energy of the detected particle
//																 	2*15*micrometer		, // Target Thickness at 0 degree
//																	Theta				);
															
//						E= He3TargetGaz.EvaluateInitialEnergy(		E 					, // Energy of the detected particle
//																	3*mm				, // Target Thickness at 0 degree
//																	Theta				);
				
							
					
					
					E=E+ThinSi;
					
					Ex = myReaction -> ReconstructRelativistic( E , Theta ) ;	
					X = HitDirection . X();
					Y = HitDirection . Y();	
				}
				
			else if(ThinSi>0)
				{
				
//					ThinSi= He3StripAl.EvaluateInitialEnergy(	ThinSi 				, // Energy of the detected particle
//																2*0.4*micrometer	, // Target Thickness at 0 degree
//																0					);
//				
//					ThinSi= He3TargetWind.EvaluateInitialEnergy( ThinSi 			, // Energy of the detected particle
//																 2*15*micrometer	, // Target Thickness at 0 degree
//																 Theta				);
//															
//						ThinSi= He3TargetGaz.EvaluateInitialEnergy(	ThinSi 			, // Energy of the detected particle
//																3*mm				, // Target Thickness at 0 degree
//																Theta				);
				 
					E= ThinSi;
					
					Ex = myReaction -> ReconstructRelativistic( E , Theta ) ;	
					X = HitDirection . X();
					Y = HitDirection . Y();	
					
				}
			if(E>-1000 )
				{
				
				
				
//				E= He3StripAl.EvaluateInitialEnergy(	E 					, // Energy of the detected particle
//														2*0.4*micrometer	, 
//														0					);	
//				
//				E= He3TargetWind.EvaluateInitialEnergy( E 					, // Energy of the detected particle
//														2*15*micrometer		, // Target Thickness at 0 degree
//														Theta				);
//				
//				E= He3TargetGaz.EvaluateInitialEnergy(	E 					, // Energy of the detected particle
//														3*mm				, // Target Thickness at 0 degree
//														Theta				);
					
				ResolTheta = Theta/deg - Init->GetICEmittedAngleThetaLabWorldFrame(0);
				Ex = myReaction -> ReconstructRelativistic( E, Theta ) ;	
				X = HitDirection . X();
				Y = HitDirection . Y();	
				
				}	
				
			else {Ex=-100 ; X = -100 ; Y = -100 ;}
								
			EE = E ; TT = Theta/deg ;
			
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


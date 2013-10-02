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
	
	double ELab[2], ExcitationEnergy[2]	;
	double ThetaLab[2]	, ThetaCM[2]	;
	double X[2] , Y[2] 					;
	double EPl1[2], EPl2[2] 			;
	double ThinSi_E						;
	// Exitation Energy
	RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",ExcitationEnergy,"ExcitationEnergy[2]/D") ;

	//E lab et Theta lab
	RootOutput::getInstance()->GetTree()->Branch("ELab",ELab,"ELab[2]/D") ;
	RootOutput::getInstance()->GetTree()->Branch("ThetaLab",ThetaLab,"ThetaLab[2]/D") ;
	RootOutput::getInstance()->GetTree()->Branch("ThinSi_E",&ThinSi_E,"ThinSi_E/D") ;
	
	RootOutput::getInstance()->GetTree()->Branch("X",X,"X/D[2]") ;
	RootOutput::getInstance()->GetTree()->Branch("Y",Y,"Y/D[2]") ;
	
	RootOutput::getInstance()->GetTree()->Branch("ThetaCM",ThetaCM,"ThetaCM[2]/D") ;
	
	RootOutput::getInstance()->GetTree()->Branch("Plastic1_Energy",EPl1,"EPl[2]/D") ;
	RootOutput::getInstance()->GetTree()->Branch("Plastic2_Energy",EPl2,"DEPl[2]/D") ;
	
	//	Get the formed Chained Tree and Treat it
	TChain* Chain = RootInput:: getInstance() -> GetChain()	;
 	   
	// Open the ThinSi Branch
	Chain->SetBranchStatus("ThinSiEnergy",true)	;
	Chain->SetBranchStatus("InitialConditions",true)	;
	Chain->SetBranchStatus("fIC_*",true)	; 
 	Chain->SetBranchStatus("fPlastic_*",true);
 		
	TInitialConditions* Init = new TInitialConditions();
	Chain->SetBranchAddress("ThinSiEnergy"		,&ThinSi_E	);
	Chain->SetBranchAddress("InitialConditions"	,&Init		);
	
	TPlasticData* Plastic = new TPlasticData() ;
	Chain->SetBranchAddress("Plastic",&Plastic	);
	
 	double XTarget=0 ; double YTarget=0; double BeamTheta = 0 ; double BeamPhi = 0 ; double E=-1000;

	// Get Must2 Pointer:
	MUST2Array* M2 = (MUST2Array*) myDetector -> m_Detector["MUST2"] ;
	
	int i;
	for ( i = 0 ; i < Chain -> GetEntries() ; i ++ )
		{
			// Clear local branch
			for(int hh = 0 ; hh <2 ; hh++)
				{
					ELab[hh] = -1 	; ExcitationEnergy[hh] = -1 ; ThetaLab[hh] = -1 ;	
					X[hh]	 = -1000	; Y[hh]					= -1000 ; ThetaCM[hh]  = -1 ;
					EPl1[hh] = 0	; EPl2[hh]				= 0 ;
					ThinSi_E = -1 	;
				}

			// Minimum code
			if( i%100000 == 0 && i!=0) {cout << i << " Event annalysed "<<endl;	}					
			Chain -> GetEntry(i);
			// Clear Previous Event
			myDetector -> ClearEventPhysics()				;
			// Build the new one
			myDetector -> BuildPhysicalEvent()				;
			////
			
			// Target (from initial condition)
			XTarget = Init->GetICPositionX(0);
			YTarget = Init->GetICPositionY(0);
			//	XTarget = RandomEngine.Gaus(Init->GetICPositionX(0),1);
			//	YTarget = RandomEngine.Gaus(Init->GetICPositionY(0),1);
			BeamTheta = Init->GetICIncidentAngleTheta(0)*deg ; BeamPhi = Init->GetICIncidentAnglePhi(0)*deg ; 
			TVector3 BeamDirection = TVector3(cos(BeamPhi)*sin(BeamTheta) , sin(BeamPhi)*sin(BeamTheta) , cos(BeamTheta)) ;
			////
			
			// Must 2 And ThinSi //
			for(int hit = 0; hit < M2 -> GetEventMultiplicity() ; hit ++)
				{
					ELab[hit] = M2 -> GetEnergyDeposit(hit);
					TVector3 HitDirection  = M2 -> GetPositionOfInteraction(hit) - TVector3(XTarget,YTarget,0);
					
					// Angle between beam and particle
					ThetaLab[hit]  = ThetaCalculation ( HitDirection , BeamDirection   ) ;				
					// Angle between particule and z axis (target Normal)
					double ThetaN = ThetaCalculation ( HitDirection , TVector3(0,0,1) ) ;
					// ANgle between particule and Must2 Si surface
					double ThetaMM2Surface = ThetaCalculation ( HitDirection , M2 -> GetTelescopeNormal(hit) );

					if(M2 -> GetPositionOfInteraction(hit).Z()>0)
						{
							if(ELab[hit]>-1000 && ThinSi_E>0 )	
								{
										ELab[hit]= He3StripAl.EvaluateInitialEnergy(	ELab[hit] 					, // Energy of the detected particle
																				2*0.4*micrometer	, // Target Thickness at 0 degree
																				ThetaMM2Surface		);
																				
										ELab[hit]= He3StripSi.EvaluateInitialEnergy(	ELab[hit] 					, // Energy of the detected particle
																				20*micrometer		, // Target Thickness at 0 degree
																				ThetaMM2Surface		);																
										
										ELab[hit]= He3StripAl.EvaluateInitialEnergy(	ELab[hit] 					, // Energy of the detected particle
																				0.4*micrometer		, // Target Thickness at 0 degree
																				ThetaMM2Surface		);

										ELab[hit]= He3TargetWind.EvaluateInitialEnergy( ELab[hit] 					, // Energy of the detected particle
																				15*micrometer		, // Target Thickness at 0 degree
																				ThetaN				);
																			
										ELab[hit]= He3TargetGaz.EvaluateInitialEnergy(	ELab[hit] 					, // Energy of the detected particle
																				1.5*mm				, // Target Thickness at 0 degree
																				ThetaN				);
																						
									ThetaCM[hit] = myReaction -> EnergyLabToThetaCM( ELab[hit] , 1 ) /deg 	;
									ExcitationEnergy[hit] = myReaction -> ReconstructRelativistic( ELab[hit] , ThetaLab[hit] ) 		;	
									X[hit] = HitDirection . X();
									Y[hit] = HitDirection . Y();	
									ThetaLab[hit] = ThetaLab[hit] / deg ;
								}

							else if(ELab[hit]>-1000 )
								{
									if(ELab[hit]>21.66)//CsI are inside a Mylar foil, plus rear alu strip
										{
											ELab[hit]= He3TargetWind.EvaluateInitialEnergy( ELab[hit] 			, // Energy of the detected particle
																						3*micrometer		, // Target Thickness at 0 degree
																						ThetaMM2Surface		);
											ELab[hit]= He3StripAl.EvaluateInitialEnergy(	ELab[hit] 				, // Energy of the detected particle
																						0.4*micrometer		, // Target Thickness at 0 degree
																						ThetaMM2Surface		);
										}
								
									ELab[hit]= He3StripAl.EvaluateInitialEnergy(	ELab[hit] 				, // Energy of the detected particle
																				0.4*micrometer		, // Target Thickness at 0 degree
																				ThetaMM2Surface		);
								
									ELab[hit]= He3TargetWind.EvaluateInitialEnergy( ELab[hit] 			, // Energy of the detected particle
																				15*micrometer		, // Target Thickness at 0 degree
																				ThetaN				);
									
									ELab[hit]= He3TargetGaz.EvaluateInitialEnergy(	ELab[hit] 				, // Energy of the detected particle
																				1.5*mm				, // Target Thickness at 0 degree
																				ThetaN				);
																				
									ThetaCM[hit]= myReaction -> EnergyLabToThetaCM( ELab[hit] , 1 ) /deg 	;	
									ExcitationEnergy[hit] = myReaction -> ReconstructRelativistic( ELab[hit], ThetaLab[hit] ) ;	
									X[hit] = HitDirection . X();
									Y[hit] = HitDirection . Y();	
									ThetaLab[hit] = ThetaLab[hit] / deg ;
								}	
							
						else {ExcitationEnergy[hit]=-100 ; X[hit] = -100 ; Y[hit]= -100 ; ThetaLab[hit]=-100;ThetaCM[hit]=-100;}
						
						}
						
					else if(M2 -> GetPositionOfInteraction(hit).Z()<0)
						{
							
						if(ELab[hit]>-1000 )
							{
								if(ELab[hit]>18)
									{
										ELab[hit]= protonStripAl.EvaluateInitialEnergy(	ELab[hit] 					, // Energy of the detected particle
																				0.4*micrometer		, // Target Thickness at 0 degree
																				ThetaMM2Surface		);
									}
							
								ELab[hit]= protonStripAl.EvaluateInitialEnergy(		ELab[hit]					, // Energy of the detected particle
																			0.4*micrometer		, // Target Thickness at 0 degree
																			ThetaMM2Surface		);
							
								ELab[hit]= protonTargetWind.EvaluateInitialEnergy( 	ELab[hit] 					, // Energy of the detected particle
																			15*micrometer		, // Target Thickness at 0 degree
																			ThetaN				);
								
								ELab[hit]= protonTargetGaz.EvaluateInitialEnergy(	ELab[hit] 					, // Energy of the detected particle
																			1.5*mm				, // Target Thickness at 0 degree
																			ThetaN				);
								ThetaCM[hit] = myReaction -> EnergyLabToThetaCM( ELab[hit] , 1 ) /deg 	;	
								ExcitationEnergy[hit] = myReaction -> ReconstructRelativistic( ELab[hit], ThetaLab[hit] ) ;	
								X[hit] = HitDirection . X();
								Y[hit] = HitDirection . Y();	
								ThetaLab[hit] = ThetaLab[hit] / deg ;
							}	
							
						else {ExcitationEnergy[hit]=-100 ; X[hit] = -100 ; Y[hit] = -100 ;ThetaLab[hit]=-100; ThetaCM[hit]=-100 ;}
						
						}	


				}			
			
			// Plastic
			for(int yy = 0 ; yy < Plastic->GetEnergySize() ; yy++)
				{
						 if(Plastic->GetPlasticNumber(yy)==1) EPl1[yy]=Plastic->GetEnergy(yy);
					else if(Plastic->GetPlasticNumber(yy)==2) EPl2[yy]=Plastic->GetEnergy(yy);
					
				}
			
			RootOutput::getInstance()->GetTree()->Fill()	;
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


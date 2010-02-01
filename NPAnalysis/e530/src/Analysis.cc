#include "ObjectManager.hh"

using namespace std;

int main(int argc,char** argv)
{	
	 
  if(argc!=5) 
    {
      cout << 
	"you need to specify both a Reaction file and a Detector file such as : "<< endl;
      cout << 
	"Analysis 	myReaction.reaction myDetector.detector runToRead.run" 	<< endl;
      return 0;
    } 
	
  string reactionfileName               = argv[1]       ;
  string detectorfileName 		= argv[2]	;
  string calibrationfileName      	= argv[3]	;  
  string runToReadfileName 		= argv[4]	;
	
  //	First of All instantiate RootInput and Output
  //	Detector will be attached later
  RootInput:: getInstance(runToReadfileName)	;
  //RootOutput::getInstance("Analysis/10HeRiken_AnalyzedData", "AnalyzedTree")					;
  RootOutput::getInstance("Analysis/Fe60", "Analysed_Data")					;
	
  //	Instantiate some Reaction

  NPL::Reaction* Fe60Reaction = new Reaction;
  Fe60Reaction    -> ReadConfigurationFile(reactionfileName);

  //	Instantiate the detector using a file 
  NPA::DetectorManager* myDetector = new DetectorManager 			  ;
  myDetector	->	ReadConfigurationFile(detectorfileName)		;

  //	Instantiate the Calibration Manger using a file
  //  CalibrationManager* myCalibration = new CalibrationManager(calibrationfileName) ;
    CalibrationManager* myCalibration = CalibrationManager::getInstance(calibrationfileName);
	
  //	Attach more branch to the output
	
  double ELab[2], ExcitationEnergy[2]	;
  double ThetaLab[2]	, ThetaCM[2]		;
  double X[2] , Y[2] 									;

  // Exitation Energy
  RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",ExcitationEnergy,"ExcitationEnergy[2]/D") ;

  //E lab et Theta lab
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",ThetaCM,"ThetaCM[2]/D") 	;
  RootOutput::getInstance()->GetTree()->Branch("ELab",ELab,"ELab[2]/D") 					;
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",ThetaLab,"ThetaLab[2]/D") 					;
  RootOutput::getInstance()->GetTree()->Branch("X",X,"X/D[2]") 										;
  RootOutput::getInstance()->GetTree()->Branch("Y",Y,"Y/D[2]")									 	;
	
	
  //	Get the formed Chained Tree and Treat it
  TChain* Chain = RootInput:: getInstance() -> GetChain()	;

  // Open the ThinSi Branch
  // Chain->SetBranchStatus("InitialConditions",true)	;
  //  Chain->SetBranchStatus("fIC_*",true)	; 
 		
  //  TInitialConditions* Init = new TInitialConditions();
  //  Chain->SetBranchAddress("InitialConditions"	,&Init		);
	
  double XTarget=0 ; double YTarget=0; double BeamTheta = 0 ; double BeamPhi = 0 ; double E=-1000;

  // Get Detector Pointer:
  TMust2Physics* M2 		= (TMust2Physics*) 	myDetector -> m_Detector["MUST2"] 	;
  //  TPlasticPhysics* Pl 	= (TPlasticPhysics*) 		myDetector -> m_Detector["Plastic"] ;
  //  TSSSDPhysics* ThinSi 	= (TSSSDPhysics*) 		myDetector -> m_Detector["SSSD"] 		;
  cout <<  " ///////// Starting Analysis ///////// "<< endl << endl ;
	
  int i ,N=Chain -> GetEntries();
	
  N = 1000;
  cout << " Number of Event to be treated : " << N << endl ;
	
  clock_t begin=clock();
  clock_t end=begin;
  for ( i = 0 ; i < N ; i ++ )
    {
      // Clear local branch
      for(int hh = 0 ; hh <2 ; hh++)
	{
	  ELab[hh] = -1 	; ExcitationEnergy[hh] = -1   ; ThetaLab[hh] = -1 ;	
	  X[hh]	 = -1000	; Y[hh]		      = -1000 ; ThetaCM[hh]  = -1 ;
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
      myDetector -> ClearEventPhysics()			;
      // Build the new one
      myDetector -> BuildPhysicalEvent()		;
      ////
			
			
      // Target (from initial condition)
      //XTarget = Init->GetICPositionX(0);
      //      YTarget = Init->GetICPositionY(0);
      //	XTarget = RandomEngine.Gaus(Init->GetICPositionX(0),1);
      //	YTarget = RandomEngine.Gaus(Init->GetICPositionY(0),1);
      //      BeamTheta = Init->GetICIncidentAngleTheta(0)*deg ; 
      //      BeamPhi = Init->GetICIncidentAnglePhi(0)*deg ; 
      TVector3 BeamDirection = TVector3(cos(BeamPhi)*sin(BeamTheta) , sin(BeamPhi)*sin(BeamTheta) , cos(BeamTheta)) ;
      ////
			
      // Must 2 And ThinSi //
      //for(int hit = 0; hit < M2 -> GetEventMultiplicity() ; hit ++)
      for(int hit = 0; hit < M2 -> Si_E.size() ; hit ++)
	{
	  ELab[hit] = -1 ; ThetaLab[hit] = -1;
	  //	Get Hit Direction
	  TVector3 HitDirection  = M2 -> GetPositionOfInteraction(hit) - TVector3(XTarget,YTarget,0);
					
	  // Angle between beam and particle
	  ThetaLab[hit]  = ThetaCalculation ( HitDirection , BeamDirection   ) ;				
	  // Angle between particule and z axis (target Normal)
	  double ThetaN = ThetaCalculation ( HitDirection , TVector3(0,0,1) )  ;
	  // ANgle between particule and Must2 Si surface
	  double ThetaMM2Surface = ThetaCalculation ( HitDirection , M2 -> GetTelescopeNormal(hit) );

	  if(M2 -> GetPositionOfInteraction(hit).Z() > 0)
	    {
	      if( M2 -> CsI_E[hit] == 0 && M2 -> Si_E[hit] > 0)
		{  
		  ELab[hit] = M2 -> Si_E[hit]  ; 
			
		  /*							
		  ELab[hit]= He3StripAl.EvaluateInitialEnergy(		ELab[hit] 		, // Energy of the detected particle
									2*0.4*micrometer	, // Target Thickness at 0 degree
									ThetaMM2Surface		);

		  */
		  //	ELab[hit]= He3StripSi.EvaluateInitialEnergy(	ELab[hit] 		, // Energy of the detected particle
		  //							20*micrometer		, // Target Thickness at 0 degree
		  //							ThetaMM2Surface		);																
										
		  //if(ThinSi -> Energy.size() > 0)ELab[hit] += ThinSi-> Energy[hit];
			
		  /*							
		  ELab[hit]= He3StripAl.EvaluateInitialEnergy(		ELab[hit] 		, // Energy of the detected particle
									0.4*micrometer		, // Target Thickness at 0 degree
									ThetaMM2Surface		);

		  ELab[hit]= He3TargetWind.EvaluateInitialEnergy( ELab[hit] 			, // Energy of the detected particle
								  15*micrometer			, // Target Thickness at 0 degree
								  ThetaN			);
																			
		  ELab[hit]= He3TargetGaz.EvaluateInitialEnergy( ELab[hit] 			, // Energy of the detected particle
								 1.5*mm				, // Target Thickness at 0 degree
								 ThetaN				);
		  */			
															 				
		  ThetaCM[hit] = Fe60Reaction -> EnergyLabToThetaCM( ELab[hit] ) /deg 	;
		  ExcitationEnergy[hit] = Fe60Reaction -> ReconstructRelativistic( ELab[hit] , ThetaLab[hit] ) 		;	
		  X[hit] = HitDirection . X();
		  Y[hit] = HitDirection . Y();	
		  ThetaLab[hit] = ThetaLab[hit] / deg ;
		}
 
	      /*
	      else if(M2 ->CsI_E[hit] > 0 && M2 -> Si_E[hit] > 0)
		{
					
		  ELab[hit]= M2 ->CsI_E[hit] ;
								
		  ELab[hit]= He3TargetWind.EvaluateInitialEnergy( ELab[hit] 					, // Energy of the detected particle
								  3*micrometer				, // Target Thickness at 0 degree
								  ThetaMM2Surface			);
																																	
		  ELab[hit]= He3StripAl.EvaluateInitialEnergy(		ELab[hit] 					, // Energy of the detected particle
									0.4*micrometer			, // Target Thickness at 0 degree
									ThetaMM2Surface			);
		  ELab[hit]+= M2 ->Si_E[hit];
									
		  ELab[hit]= He3StripAl.EvaluateInitialEnergy(		ELab[hit] 					, // Energy of the detected particle
									0.4*micrometer			, // Target Thickness at 0 degree
									ThetaMM2Surface			);
								
		  //if(ThinSi -> Energy.size() > 0)ELab[hit] += ThinSi-> Energy[hit];
									
									
		  ELab[hit]= He3StripAl.EvaluateInitialEnergy(		ELab[hit] 					, // Energy of the detected particle
									0.4*micrometer			, // Target Thickness at 0 degree
									ThetaMM2Surface			);
								
		  ELab[hit]= He3TargetWind.EvaluateInitialEnergy( ELab[hit] 					, // Energy of the detected particle
								  15*micrometer				, // Target Thickness at 0 degree
								  ThetaN							);
									
		  ELab[hit]= He3TargetGaz.EvaluateInitialEnergy(	ELab[hit] 					, // Energy of the detected particle
									1.5*mm							, // Target Thickness at 0 degree
									ThetaN							);
																				
		  ThetaCM[hit]= Fe60Reaction -> EnergyLabToThetaCM( ELab[hit] ) /deg 	;	
		  ExcitationEnergy[hit] = Fe60Reaction -> ReconstructRelativistic( ELab[hit], ThetaLab[hit] ) ;	
		  X[hit] = HitDirection . X();
		  Y[hit] = HitDirection . Y();	
		  ThetaLab[hit] = ThetaLab[hit] / deg ;
		}	
	      */					
	      else {ExcitationEnergy[hit]=-100 ; X[hit] = -100 ; Y[hit]= -100 ; ThetaLab[hit]=-100;ThetaCM[hit]=-100;}
						
	    }
						
	  /*else if(M2 -> GetPositionOfInteraction(hit).Z()<0)
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
						
	    }	*/


	}			
			
      RootOutput::getInstance()->GetTree()->Fill()	;
    }

  cout << " A total of " << i << " event has been annalysed " << endl ;
  cout << endl << " ///////////////////////////////////// "<< endl<< endl ;
  RootOutput::getInstance()->Destroy();
  return 0	;
}

double ThetaCalculation (TVector3 A , TVector3 B)
{
  double Theta = acos( (A.Dot(B)) / (A.Mag()*B.Mag()) ) ;
  return Theta*rad ;
}


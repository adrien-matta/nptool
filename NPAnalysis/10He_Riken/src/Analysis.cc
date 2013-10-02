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
	double Ex = 0 ; double EE = 0 ; double TT = 0 ; double X = 0 ; double Y = 0 ; int det ;
	RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",&Ex,"Ex/D") ;
	RootOutput::getInstance()->GetTree()->Branch("E",&EE,"EE/D") ;
	RootOutput::getInstance()->GetTree()->Branch("A",&TT,"TT/D") ;
	RootOutput::getInstance()->GetTree()->Branch("X",&X,"X/D") ;
	RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D") ;
	
	// Get Must2 Pointer:
	MUST2Array* M2 = (MUST2Array*) myDetector -> m_Detector["MUST2"] ;
	//	Get the formed Chained Tree and Treat it
	TChain* Chain = RootInput:: getInstance() -> GetChain()	;
	int i;	
	for ( i = 0 ; i < Chain -> GetEntries() ; i ++ )
		{
			if( i%10000 == 0 && i!=0) cout << i << " Event annalysed " << endl ;						
			Chain -> GetEntry(i);
			
			myDetector -> ClearEventPhysics()				;
			myDetector -> BuildPhysicalEvent()				;
			
			
			double E = M2 -> GetEnergyDeposit();
			TVector3 A = M2 -> GetPositionOfInteraction();
			double Theta = ThetaCalculation ( A , TVector3(0,0,1) ) ;
			if(E>-1000)		Ex = myReaction -> ReconstructRelativistic( E , Theta ) ;
			else Ex = -100 ;
			EE = E ; TT = Theta/deg ;
			if(E>-1000){
			X = A . X();
			Y = A . Y();}
			else{X = -1000 ; Y = -1000;}
			
			RootOutput::getInstance()->GetTree()->Fill()	;
		}
	cout << "A total of " << i << " event has been annalysed " << endl ;
	
	RootOutput::getInstance()->Destroy();
	return 0	;
}


double ThetaCalculation (TVector3 A , TVector3 B)
	{
		double Theta = acos( (A.Dot(B)) / (A.Mag()*B.Mag()) ) ;
		return Theta ;
	}


#include "ObjectManager.hh"

using namespace std;

int main(int argc,char** argv)
{	

	NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv)  ;
	string detectorfileName 		= myOptionManager->GetDetectorFilePath()	      ;
	string reactionfileName 	  = myOptionManager->GetCalibrationFilePath()	    ;
	string calibrationfileName 	= myOptionManager->GetCalibrationFilePath()	    ;
	string runToReadfileName 		= myOptionManager->GetRunToReadFilePath()       ;
	
	//	First of All instantiate RootInput and Output
	//	Detector will be attached later
	RootInput:: getInstance(runToReadfileName)	;
	RootOutput::getInstance("Analysis/Template_AnalyzedData", "AnalyzedTree")					;
	
	//	Instantiate a Reaction
	NPL::Reaction* myReaction = new Reaction					;
	myReaction	->	ReadConfigurationFile(reactionfileName)		;

		//	Instantiate the detector using a file 
	NPA::DetectorManager* myDetector = new DetectorManager 			;
	myDetector	->	ReadConfigurationFile(detectorfileName)		;
	
	//	Instantiate the Calibration Manger using a file
	CalibrationManager* myCalibration = CalibrationManager::getInstance(calibrationfileName) ;
	
	//	Get the formed Chained Tree and Treat it
	TChain* Chain = RootInput:: getInstance() -> GetChain()	;
	int i;	
	for ( i = 0 ; i < Chain -> GetEntries() ; i ++ )
		{
			if( i%10000 == 0 && i!=0) cout << "\r Event Analyzed:" << i << flush;					
			Chain -> GetEntry(i);			
			
			myDetector -> ClearEventPhysics()				;
			myDetector -> BuildPhysicalEvent()				;
			
			/************************************************
						
			Put your code here
			
			************************************************/
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


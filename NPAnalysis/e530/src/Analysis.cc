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
	
  string reactionfileName     = argv[1]       ;
  string detectorfileName 		= argv[2]	;
  string calibrationfileName  = argv[3]	;  
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
	

	
  //	Get the formed Chained Tree and Treat it
  TChain* Chain = RootInput:: getInstance() -> GetChain()	;


	

  TMust2Physics* M2 		= (TMust2Physics*) 	myDetector -> m_Detector["MUST2"] 	;
  cout <<  " ///////// Starting Analysis ///////// "<< endl << endl ;
	
  int i ,N=Chain -> GetEntries();
	
  //N = 1000;
  cout << " Number of Event to be treated : " << N << endl ;
	
  clock_t begin=clock();
  clock_t end=begin;
  for ( i = 0 ; i < N ; i ++ )
    {
      // Minimum code
    	if( i%10000 == 0 && i!=0) 	{	
				cout.precision(5);
				end=clock();										
				double TimeElapsed = (end-begin)/CLOCKS_PER_SEC;
				double percent = (double)i/N ;
				double TimeToWait = (TimeElapsed/percent) - TimeElapsed	;					
				cout	<< "\r Progression:" << percent*100 
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


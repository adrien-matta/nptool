#include "ObjectManager.hh"

using namespace std;

int main(int argc,char** argv)
{	
	 
  if(argc!=5) 
    {
      cout << 
	"you need to specify both a Reaction file and a Detector file such as : "<< endl;
      cout << 
	"Analysis 	myReaction.reaction myDetector.detector myCalib.txt runToRead.run" 	<< endl;
      return 0;
    } 
	
  string reactionfileName     = argv[1]       ;
  string detectorfileName 		= argv[2]	;
  string calibrationfileName  = argv[3]	;  
  string runToTreatFileName 	= argv[4]	;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//	First of All instantiate RootInput and Output
	//	Detector will be attached later
	RootInput:: getInstance(runToTreatFileName)	;
	RootOutput::getInstance("Analysis/60Fe_AnalysedData", "Analysed_Data")	;
	
	//	Instantiate some Reaction
	NPL::Reaction*  e530Reaction = new Reaction								;
	e530Reaction	->	ReadConfigurationFile(reactionfileName)	;

	//	Instantiate the Calibration Manger using a file (WARNING:prior to the detector instantiation)
	CalibrationManager* myCalibration = CalibrationManager::getInstance(calibrationfileName) ;

	//	Instantiate the detector using a file 
	NPA::DetectorManager* myDetector = new DetectorManager 		;
	myDetector	->	ReadConfigurationFile(detectorfileName)		;
	
	//	Ask the detector manager to load the parameter added by the detector in the calibrationfileName
	myCalibration->LoadParameterFromFile() ;
	/////////////////////////////////////////////////////////////////////////////////////////////////////

  //	Attach more branch to the output
  double ELab=0,ThetaLab=0,ExcitationEnergy=0;
	RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D") 							;
	RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D") 	;
	RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",&ExcitationEnergy,"ExcitationEnergy/D") ;
  //	Get the formed Chained Tree and Treat it
  TChain* Chain = RootInput:: getInstance() -> GetChain()	;

  // Connect TTriggerData branch
  
  TTriggerData* TriggerCondition = 0;
  Chain->SetBranchAddress("TRIG", &TriggerCondition);
  Chain->SetBranchStatus("TRIG", 1);
   
  TMust2Physics* M2 		= (TMust2Physics*) 	myDetector -> m_Detector["MUST2"] 	;
  TCATSPhysics * CATS           = (TCATSPhysics* )      myDetector -> m_Detector["CATS"]        ;

  cout <<  " ///////// Starting Analysis ///////// "<< endl << endl ;
	

  int i ,N=Chain -> GetEntries();
	
  //N = 1000;
  cout << " Number of Event to be treated : " << N << endl ;
	
  clock_t begin=clock();
  clock_t end=begin;
  for ( i = 0 ; i < 700 ; i ++ )
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
      
      
      short must2_event = TriggerCondition -> GetTRIG1();
    
      if(must2_event < 16) 
	 {
	   cout << endl << "event number " << i  << " " << "must2_hit " << must2_event <<  endl;
 
	   // Build the new event
	   myDetector -> BuildSimplePhysicalEvent()		;
	   ////

	   M2->Dump();

	   // Must 2
	   for(int hit = 0; hit < M2 -> Si_E.size() ; hit ++)
	     {
	       if((M2 -> TelescopeNumber[hit]) != 4) 
		 {
		   //	Get Hit Direction
		   TVector3 HitDirection  = M2 -> GetPositionOfInteraction(hit) - CATS -> GetPositionOnTarget();
		   //TVector3 HitDirection  = M2 -> GetPositionOfInteraction(hit) - TVector3(0,0,-40);
		   TVector3 BeamDirection = CATS -> GetBeamDirection();
		   
		   ELab = -1 ; ThetaLab = -1;    

		   // if(CUT::cut_protons -> IsInside(M2 -> Si_E[hit], M2 -> SiLi_E[hit])) 
		     {
		       // Angle between beam and particle
		       ThetaLab  = ThetaCalculation ( HitDirection , BeamDirection ) ;
		       //ThetaLab  = ThetaCalculation ( HitDirection , TVector3(0,0,1)   ) ;	
		       
		       if((M2 -> SiLi_E[hit]) !=-10000) ELab = M2 -> Si_E[hit] + M2 -> SiLi_E[hit]	;
		       else                             ELab = M2 -> Si_E[hit];
		     }
		   
		     /* else 
		     {
		       ThetaLab = 0; ELab=0;
		       cout << "pas de protons" << endl;
		       }*/
		   
		   cout << ThetaLab << " " << ELab << endl;
		   
		 }
	       else cout << "Telescope4" << endl;
	       
	       RootOutput::getInstance()->GetTree()->Fill()	;
	     }
	 }
    }
  
cout << " A total of " << i << " event has been analysed " << endl ;
  cout << endl << " ///////////////////////////////////// "<< endl<< endl ;
  RootOutput::getInstance()->Destroy();
  return 0	;
}

double ThetaCalculation (TVector3 A , TVector3 B)
{
  double Theta = acos( (A.Dot(B)) / (A.Mag()*B.Mag()) ) ;
  return Theta*rad ;
}


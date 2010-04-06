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
  
  string reactionfileName     = argv[1] ;
  string detectorfileName     = argv[2]	;
  string calibrationfileName  = argv[3]	;  
  string runToTreatFileName   = argv[4]	;
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////
    //	First of All instantiate RootInput and Output
    //	Detector will be attached later
    RootInput:: getInstance(runToTreatFileName)	;
    RootOutput::getInstance("Analysis/60Fe_AnalysedData", "Analysed_Data")	;
    
    //	Instantiate some Reaction
    NPL::Reaction*  e530Reaction = new Reaction					;
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
      
      double ELab[2],ThetaLab[2],ExcitationEnergy[2];
      double ThetaN[2];
      
      for (int i = 0; i < 2; i++) 
	{
	  ELab[i]  = 0; ThetaLab[i] = 0; ExcitationEnergy[i] = 0;
	  ThetaN[i]= 0;
	}


      RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D") 		;
      RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D") 	;
      RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",&ExcitationEnergy,"ExcitationEnergy/D") ;
      
      //	Get the formed Chained Tree and Treat it
      TChain* Chain = RootInput:: getInstance() -> GetChain()	;
      
      // Connect TTriggerData branch
      
      TTriggerData* TriggerCondition = 0;
      Chain->SetBranchAddress("TRIG", &TriggerCondition);
      Chain->SetBranchStatus("TRIG", 1);
      
      TTacData* TacCondition = 0;
      Chain->SetBranchAddress("TAC", &TacCondition);
      Chain->SetBranchStatus("TAC", 1);
      

      TMust2Physics* M2 	    = (TMust2Physics*)  myDetector -> m_Detector["MUST2"] 	;
      TCATSPhysics * CATS           = (TCATSPhysics* )  myDetector -> m_Detector["CATS"]        ;
            
      TH2F* DE_E_protons  = new TH2F("DE_E_protons","DE_E et cut protons",1000,0,25000,1000,0,25000) ;
      TH2F* DE_E          = new TH2F("DE_E","DE_E               ",1000,0,25000,1000,0,25000) ;
      TH2F* E_Theta       = new TH2F("E_Theta","E_Theta         ",60,100,160,500,0,50) ;
      TH2F* Position_M2   = new TH2F("Must2Positions","Must2Positions",2000,-200,200,2000,-200,200);
      
      TH1F* trigger       = new TH1F("trig","trig",650,0,649);
      TH1F* tac           = new TH1F("tac","tac",200,2800,4000);
     
      int must, evt_tac, evt_ETof, evt_EDE  = 0;
      
      cout <<  " ///////// Starting Analysis ///////// "<< endl << endl ;
      
      int i ,N=Chain -> GetEntries();
      
      //N = 1000;
      cout << " Number of Event to be treated : " << N << endl ;
      
      clock_t begin=clock();
      clock_t end=begin;

      //M2->Dump_Positions();

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
	  
	  UShort_t tac_pl_cats2 = TacCondition -> GetTAC_PL_CATS2();
	  tac -> Fill(tac_pl_cats2);

	  UShort_t tdc_MM1_cats2 = TacCondition -> GetTDC_MM1_CATS2();  
	  UShort_t tdc_MM2_cats2 = TacCondition -> GetTDC_MM2_CATS2();
	  UShort_t tdc_MM3_cats2 = TacCondition -> GetTDC_MM3_CATS2();
	  UShort_t tdc_MM4_cats2 = TacCondition -> GetTDC_MM4_CATS2();
	  //	  cout << tdc_MM1_cats2 << " " << tdc_MM2_cats2 << " " << tdc_MM3_cats2 << " " << tdc_MM4_cats2 << endl;

	  UShort_t must2_event = TriggerCondition -> GetTRIG1();
	  trigger -> Fill(must2_event);
	  
	  
	  if(must2_event < 16) 
	     {
	       //if(tac_pl_cats2 > 6500 && tac_pl_cats2 < 6700) // Jo conditions
	       if(tac_pl_cats2 > 3450 && tac_pl_cats2 < 3650) 
		 {

		   if(3500 < tdc_MM1_cats2 < 5000 || 3500 < tdc_MM2_cats2 < 5000 || 3500 < tdc_MM3_cats2 < 5000 || 3500 < tdc_MM4_cats2 < 5000 )
		     {
		       evt_tac++;
		       must++;
		       
		       // Build the new event
		       myDetector -> BuildSimplePhysicalEvent()		;
		       ////
		       
		       // Must 2
		       
		      
		       if(M2 -> Si_E.size() != M2 -> EventMultiplicity)  cout << "problem with EventMultiplicity !" << endl;
		      		       
		       else 
			 {
			   if(CATS -> GetPositionOnTargetX() > -30 && CATS -> GetPositionOnTargetX() < 30 && CATS -> GetPositionOnTargetY() > -30 && CATS -> GetPositionOnTargetY() < 30)
			     {

			       for(int hit = 0; hit < M2 -> EventMultiplicity ; hit ++)
				 //  for(int hit = 0; hit < M2 -> Si_E.size() ; hit ++)
				 {
				   
				   //	Get Hit Direction
				   TVector3 HitDirection  = M2 -> GetPositionOfInteraction(hit) - CATS -> GetPositionOnTarget();
				   //TVector3 HitDirection  = M2 -> GetPositionOfInteraction(hit) - TVector3(0,0,-40);
				   TVector3 BeamDirection = CATS -> GetBeamDirection();
				   
				   ExcitationEnergy[hit] = -1 ;
				   ELab[hit] = -1 ; ThetaLab[hit] = -1;    
				   
				   
				   // Angle between beam and particle
				   ThetaLab[hit]  = ThetaCalculation ( HitDirection , BeamDirection ) *rad ; 
				   //ThetaLab[hit]  = ThetaCalculation ( HitDirection , TVector3(0,0,1)   ) ;	
				   ThetaN[hit] = ThetaCalculation(HitDirection , M2 -> GetTelescopeNormal(hit))*rad;
				   
				   if((M2->Si_E[hit] > 0))
				     {
				       Position_M2->Fill((M2 -> GetPositionOfInteraction(hit)).X(),(M2 -> GetPositionOfInteraction(hit)).Y());
				     }
				   
				   
				   if((M2-> Si_E[hit] > 0) && (M2 -> MultiplicitySiLi ==0)) 
				     //			       if((M2->Si_E[hit] > 0) && (M2 -> SiLi_E[hit]) < 0) 
				     {
				       ELab[hit] = M2 -> Si_E[hit] * keV;
				       ELab[hit] = protonStripAl.EvaluateInitialEnergy(ELab[hit], 0.5*micrometer, ThetaN[hit]) /MeV;
				       ELab[hit] = protonTargetCD2.EvaluateInitialEnergy(ELab[hit],17.4*micrometer,ThetaLab[hit]) /MeV;
				       
				       ExcitationEnergy[hit] = e530Reaction -> ReconstructRelativistic( ELab[hit]/MeV, ThetaLab[hit]/rad) /MeV;
				       ThetaLab[hit]  = ThetaLab[hit] /deg;
				       
				     }
				   
				   //else if((M2->Si_E[hit] > 0) && (M2 -> MultiplicitySiLi != 0))
				   else if((M2->Si_E[hit] > 0) && (M2 -> SiLi_E[hit]) > 0)
				     {
				       //  if(M2 -> SiLi_E.size() != M2 -> MultiplicitySiLi) Dump()
				       //else
				       {		       
					 DE_E ->Fill(M2 -> SiLi_E[hit],M2 -> Si_E[hit]);
					 //cout << M2 -> SiLi_E[hit] << " " << M2 -> Si_E[hit] << endl;
					 
					 ELab[hit] = M2 -> SiLi_E[hit] *keV;
					 ELab[hit] = protonStripAl.EvaluateInitialEnergy(ELab[hit]/MeV, 0.5*micrometer, ThetaN[hit]) /MeV;
					 
					 ELab[hit] += M2 -> Si_E[hit] *keV;
					 ELab[hit] = protonStripAl.EvaluateInitialEnergy(ELab[hit]/MeV, 0.5*micrometer, ThetaN[hit]) /MeV;
					 
					 ELab[hit] = protonTargetCD2.EvaluateInitialEnergy(ELab[hit]/MeV,17.4*micrometer,ThetaLab[hit]) /MeV;
					 
					 ExcitationEnergy[hit] = e530Reaction -> ReconstructRelativistic( ELab[hit]/MeV, ThetaLab[hit]/rad) /MeV ;
					 ThetaLab[hit]  = ThetaLab[hit] /deg;
				       }
				     }
				   
				   else {ExcitationEnergy[hit]=-100 ; ThetaLab[hit]=-100;}
				   
				   
				   // DE_E_protons   -> Fill(M2 -> SiLi_E[hit],M2 -> Si_E[hit]);
				   E_Theta        -> Fill(ThetaLab[hit],ELab[hit]/MeV);
				   
				   //
				   //  else 
				   //  {
				   //  ThetaLab[hit] = 0; ELab[hit]=0;
				   // cout << "pas de protons" << endl;
				   //  }
				   
				   // cout << "theta lab : " << ThetaLab[hit] << "    ELab[hit] : " << ELab[hit] << endl;
				   
				   
				 } // end of for loop over hit
			     }
			   RootOutput::getInstance()->GetTree()->Fill()	;
			 } 
		       
		       
		       //RootOutput::getInstance()->GetTree()->Fill()	;    // to look at CATS detector only, without must2 events condition
		       
		     }  // tac condition
		 } //td conditions
	     } //must2 events condition
	   	  
	} // end of for loop over events

      
      cout << "must events : " << must << endl;

      DE_E         ->Write("");
      DE_E_protons ->Write("");
      E_Theta      ->Write("");
      trigger      ->Draw("");
      Position_M2->Write("");
      
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


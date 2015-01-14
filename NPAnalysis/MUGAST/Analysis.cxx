#include "Analysis.h"

int main(int argc, char** argv){
  // command line parsing
  NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);

  // Instantiate RootInput
  string runToReadfileName = myOptionManager->GetRunToReadFile();
  RootInput:: getInstance("RunToTreat.txt");
  TChain* Chain = RootInput:: getInstance()->GetChain();
  // if input files are not given, use those from TAsciiFile
  if (myOptionManager->IsDefault("DetectorConfiguration")) {
    string name = RootInput::getInstance()->DumpAsciiFile("DetectorConfiguration");
    myOptionManager->SetDetectorFile(name);
  }

  if (myOptionManager->IsDefault("EventGenerator")) {
    string name = RootInput::getInstance()->DumpAsciiFile("EventGenerator");
    myOptionManager->SetReactionFile(name);
  }

  // get input files from NPOptionManager
  string detectorfileName    = myOptionManager->GetDetectorFile();
  string OutputfileName      = myOptionManager->GetOutputFile();

  // Instantiate RootOutput
  RootOutput::getInstance("Analysis/"+OutputfileName, "ResultTree");
  // RootOutput::getInstance()->GetFile()->SetCompressionLevel(0);
  // Instantiate the detector using a file
  NPA::DetectorManager* myDetector = new DetectorManager();
  myDetector->ReadConfigurationFile(detectorfileName);
  // Attach new branch
  InitOutputBranch();
  InitInputBranch();

  //	Instantiate the Reaction
  NPL::Reaction*  myReaction = new Reaction ;
  myReaction	->	ReadConfigurationFile(myOptionManager->GetReactionFile());

  ////////////////////////////////////////////////////////
  // Get pointer to the different detector

  TMust2Physics* M2  = (TMust2Physics*) myDetector -> GetDetector("MUST2");
  GaspardTracker* GD = (GaspardTracker*)  myDetector -> GetDetector("GASPARD");

  double TargetThickness = 18*micrometer;
  // Beam Energy
  double OriginalBeamEnergy = 4.0* 74; // AMEV
    // intermediate variable
  TRandom3 Rand = TRandom3();
  int DetectorNumber = 0 ;
  double ThetaNormalTarget = 0 ;
  double ThetaM2Surface = 0; 
  double Si_E_M2 = 0 ;
  double SiLi_E_M2 = 0 ; 
  double Energy = 0;
  double E_M2 = 0;
  
  double ThetaGDSurface = 0; 
  double X_GD = 0 ;
  double Y_GD = 0 ;
  double Z_GD = 0 ;
  double Si_E_GD = 0 ;
  double E_GD = 0;
  double Si_X_GD = 0;
  double Si_Y_GD = 0;
 
  TVector3 BeamImpact(0,0,0);

  // Get number of events to treat
  cout << endl << "///////// Starting Analysis ///////// "<< endl;
  int nentries = Chain->GetEntries();
  cout << " Number of Event to be treated : " << nentries << endl;
  clock_t begin = clock();
  clock_t end = begin;
  cout.precision(5);
  //////////////////////////////////////////////////////////////////////////////
  // main loop on entries //
  for (int i = 0 ; i < nentries; i++) {
    if (i%10000 == 0 && i!=0)  {
      end = clock();
      long double TimeElapsed = (long double) (end-begin) / CLOCKS_PER_SEC;
      double percent = (double)i/nentries;
      double TimeToWait = (TimeElapsed/percent) - TimeElapsed;       
      cout  << "                                                      "<< flush;
      cout  << "\r Progression:" 
        << percent*100 << " % \t | \t Remaining time : ~" 
        <<  TimeToWait <<"s |  Analysis Rate : "
        << (double) i/TimeElapsed << flush;
    }
    else if (i == nentries-1)  cout << "\r Progression:" << " 100% " <<endl;
    // Get the raw Data
    Chain -> GetEntry(i);
    // Clear previous Event
    myDetector->ClearEventPhysics();
    // Build the current event
    myDetector->BuildPhysicalEvent();
    // Reinitiate calculated variable
    ReInitValue();
    
    double XTarget = 0;
    double YTarget = 0;
    TVector3 BeamDirection = TVector3(0,0,1);
    double BeamEnergy = BeamCD2.Slow(OriginalBeamEnergy,Rand.Uniform(0,TargetThickness),0);
    myReaction->SetBeamEnergy(273.4);


      //////////////////////////// LOOP on MUST2 //////////////////
    for(unsigned int countMust2 = 0 ; countMust2 < M2->Si_E.size() ; countMust2++){
      /************************************************/
      //Part 0 : Get the usefull Data
      // MUST2
      int TelescopeNumber = M2->TelescopeNumber[countMust2];

      /************************************************/
      // Part 1 : Impact Angle
      ThetaM2Surface = 0;
      ThetaNormalTarget = 0;
      TVector3 HitDirection = M2 -> GetPositionOfInteraction(countMust2) - BeamImpact ;
      ThetaLab = HitDirection.Angle( BeamDirection );

      ThetaM2Surface = HitDirection.Angle(- M2 -> GetTelescopeNormal(countMust2) );
      ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;

      /************************************************/

      /************************************************/
      // Part 2 : Impact Energy
      Energy = ELab = 0;
      Si_E_M2 = M2->Si_E[countMust2];
      SiLi_E_M2= M2->SiLi_E[countMust2];

      // if SiLi
      if(SiLi_E_M2>0 ){
        // The energy in CsI is calculate form dE/dx Table because 
        Energy = SiLi_E_M2;
        Energy = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaM2Surface); 
        Energy+=Si_E_M2;
      }

      else
        Energy = Si_E_M2;

      // Evaluate energy using the thickness 
      ELab = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaM2Surface); 
      // Target Correction
      ELab   = LightCD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);
      /************************************************/

      /************************************************/
      // Part 3 : Excitation Energy Calculation
      Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );
      ThetaLab=ThetaLab/deg;
      
      /************************************************/

      /************************************************/
      // Part 4 : Theta CM Calculation
      ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , 0)/deg;
      /************************************************/
    }//end loop MUST2
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    //////////////////////////// LOOP on GASPARD //////////////////
    if(GD->GetEnergyDeposit()>0){
      /************************************************/
      // Part 1 : Impact Angle
      ThetaGDSurface = 0;
      ThetaNormalTarget = 0;
      if(XTarget>-1000 && YTarget>-1000){
        TVector3 HitDirection = GD -> GetPositionOfInteraction() - BeamImpact ;
        ThetaLab = HitDirection.Angle( BeamDirection );

        ThetaGDSurface = HitDirection.Angle( TVector3(0,0,1) ) ;
        ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
      }

      else{
        BeamDirection = TVector3(-1000,-1000,-1000);
        ThetaGDSurface    = -1000  ;
        ThetaNormalTarget = -1000  ;
      }

      /************************************************/

      /************************************************/
      // Part 2 : Impact Energy
      Energy = ELab = 0;
      Energy = GD->GetEnergyDeposit();
      
      ELab = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaGDSurface); 
      // Target Correction
      ELab   = LightCD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);
      /************************************************/

      /************************************************/
      // Part 3 : Excitation Energy Calculation
      Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );
            /************************************************/

      /************************************************/
      // Part 4 : Theta CM Calculation
      ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;

      cout << ThetaCM << " " << Init->GetThetaCM(0) << endl;
      ThetaLab=ThetaLab/deg;

      /************************************************/
    }//end loop GASPARD

    if(ELab>0)
      RootOutput::getInstance()->GetTree()->Fill();
   }// loop over events

  cout << "A total of " << nentries << " event has been annalysed " << endl ;

  RootOutput::getInstance()->Destroy();
  RootInput::getInstance()->Destroy();
  NPOptionManager::getInstance()->Destroy();
  /////////////////////////////////////////////////////////////////////////////
  return 0 ;
}

////////////////////////////////////////////////////////////////////////////////
void InitOutputBranch() {
  RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
}

////////////////////////////////////////////////////////////////////////////////
void InitInputBranch(){
  RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&Init );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("InitialConditions",true );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
}
////////////////////////////////////////////////////////////////////////////////     
void ReInitValue(){
  Ex = -1000 ;
  ELab = -1000;
  ThetaLab = -1000;
  ThetaCM = -1000;
}


////////////////////////////////////////////////////////////////////////////////

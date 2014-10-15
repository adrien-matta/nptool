#include "Analysis.h"

int main(int argc, char** argv)
{
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
  NPL::Reaction*  He10Reaction = new Reaction ;
  He10Reaction	->	ReadConfigurationFile("10He.reaction") ;

  ////////////////////////////////////////////////////////

  // Get pointer to the different detector

  TMust2Physics* M2  = (TMust2Physics*) myDetector -> GetDetector("MUST2");
  TSSSDPhysics* SSSD = (TSSSDPhysics*)  myDetector -> GetDetector("SSSD");

  // intermediate variable
  TRandom3 Rand = TRandom3();
  int DetectorNumber = 0 ;
  double ThetaNormalTarget = 0 ;
  double ThetaM2Surface = 0; 
  double X_M2 = 0 ;
  double Y_M2 = 0 ;
  double Z_M2 = 0 ;
  double Si_E_M2 = 0 ;
  double CsI_E_M2 = 0 ; 
  double E_SSSD = 0 ;
  double Energy = 0;
  double E_M2 = 0;
  double Si_X_M2 = 0;
  double Si_Y_M2 = 0;
  double ZTarget = 0;
  double TargetThickness = 18*micrometer;
  double ThetaCM = 0 ;
  // Get number of events to treat
  cout << endl << "///////// Starting Analysis ///////// "<< endl;
  int nentries = Chain->GetEntries();
  //      int nentries = 100000;
  cout << " Number of Event to be treated : " << nentries << endl;
  clock_t begin = clock();
  clock_t end = begin;
  cout.precision(5);
  // main loop on entries
  for (int i = 0 ; i < nentries; i++) {
    if (i%10000 == 0 && i!=0)  {
      end = clock();
      long double TimeElapsed = (long double) (end-begin) / CLOCKS_PER_SEC;
      double percent = (double)i/nentries;
      double TimeToWait = (TimeElapsed/percent) - TimeElapsed;       
      cout  << "                                                       "<< flush;
      cout  << "\r Progression:" << percent*100 << " % \t | \t Remaining time : ~" <<  TimeToWait <<"s |  Analysis Rate : "<< (double) i/TimeElapsed << flush;
    }
    else if (i == nentries-1)  cout << "\r Progression:" << " 100% " <<endl;


    // get data
    Chain -> GetEntry(i);
    myDetector->ClearEventPhysics();
    myDetector->BuildPhysicalEvent();
    ReInitValue();

    // Get the Init information on beam positio nand energy
    // and apply by hand the experimental resolution
		double XTarget = Rand.Gaus(Init->GetIncidentPositionX(),1);
    double YTarget = Rand.Gaus(Init->GetIncidentPositionY(),1);
    double BeamEnergy = Rand.Gaus(Init->GetIncidentInitialKineticEnergy(),4.5);
    TVector3 BeamDirection = Init->GetBeamDirection();
    BeamEnergy = Li11CD2.Slow(BeamEnergy,TargetThickness/2.,0);
    He10Reaction->SetBeamEnergy(BeamEnergy);
        
    //////////////////////////// LOOP on MUST2 + SSSD Hit //////////////////
    for(unsigned int countSSSD = 0 ; countSSSD < SSSD->Energy.size() ; countSSSD++){
      for(unsigned int countMust2 = 0 ; countMust2 < M2->Si_E.size() ; countMust2++){
        /************************************************/
        //Part 0 : Get the usefull Data
        // MUST2
        int X = M2->Si_X[countMust2];
        int Y = M2->Si_Y[countMust2];
        int TelescopeNumber = M2->TelescopeNumber[countMust2];
        Si_X_M2 = X ;
        Si_Y_M2 = Y ;
        //SSSD
        int SiNumber = SSSD->DetectorNumber[countSSSD];

        /************************************************/
        if(TelescopeNumber==SiNumber ){
          DetectorNumber = TelescopeNumber ;
          /************************************************/
          // Part 1 : Impact Angle
          ThetaM2Surface = 0;
          ThetaNormalTarget = 0;
          if(XTarget>-1000 && YTarget>-1000){
            TVector3 BeamImpact(XTarget,YTarget,0);
            TVector3 HitDirection = M2 -> GetPositionOfInteraction(countMust2) - BeamImpact ;
            ThetaLab = HitDirection.Angle( BeamDirection );

            ThetaM2Surface = HitDirection.Angle(- M2 -> GetTelescopeNormal(countMust2) );
            ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
            X_M2 = M2 -> GetPositionOfInteraction(countMust2).X() ;
            Y_M2 = M2 -> GetPositionOfInteraction(countMust2).Y() ;
            Z_M2 = M2 -> GetPositionOfInteraction(countMust2).Z() ;
          }

          else{
            BeamDirection = TVector3(-1000,-1000,-1000);
            ThetaM2Surface    = -1000  ;
            ThetaNormalTarget = -1000  ;
          }

          /************************************************/

          /************************************************/

          // Part 2 : Impact Energy
          Energy = ELab = 0;
          Si_E_M2 = M2->Si_E[countMust2];
          CsI_E_M2= M2->CsI_E[countMust2];
          E_SSSD = SSSD->Energy[countSSSD];

          // if CsI
            if(CsI_E_M2>0 ){
              // The energy in CsI is calculate form dE/dx Table bevause calibration for 3He is not good
              Energy = He3Si.EvaluateEnergyFromDeltaE(Si_E_M2,300*micrometer,ThetaM2Surface, 0.01*MeV, 450.*MeV,0.001*MeV ,1000);
              E_M2=CsI_E_M2;
            }

            else
              Energy = Si_E_M2;

            E_M2 += Si_E_M2;

            // Evaluate energy using the thickness 
              ELab   = He3Al.EvaluateInitialEnergy( Energy ,2*0.4*micrometer , ThetaM2Surface); 
              ELab   = He3Si.EvaluateInitialEnergy( ELab ,20*micrometer , ThetaM2Surface);
              ELab   = He3Al.EvaluateInitialEnergy( ELab ,0.4*micrometer , ThetaM2Surface);
              // Target Correction
              ELab   = He3CD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);
          /************************************************/

          /***********M2->Si_E[countMust2];*************************************/
          // Part 3 : Excitation Energy Calculation
          Ex = He10Reaction -> ReconstructRelativistic( ELab , ThetaLab );
          ThetaLab=ThetaLab/deg;
           /************************************************/

          /************************************************/
          // Part 4 : Theta CM Calculation
          ThetaCM  = He10Reaction -> EnergyLabToThetaCM( ELab , 0)/deg;
          /************************************************/
        }
      } //end loop SSSD
    }//end loop MUST2

    if(ELab>0)
      RootOutput::getInstance()->GetTree()->Fill();
  }// loop over events

  cout << "A total of " << nentries << " event has been annalysed " << endl ;

  RootOutput::getInstance()->Destroy();
  RootInput::getInstance()->Destroy();
  NPOptionManager::getInstance()->Destroy();
  /////////////////////////////////////////////////////////////////////
  return 0 ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void InitOutputBranch() {
  RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void InitInputBranch(){
  RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&Init );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("InitialConditions",true );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
}
////////////////////////////////////////////////////////////////////////////////////////////////////     
////////////////////////////////////////////////////////////////////////////////////////////////////
void ReInitValue(){
  Ex = -1000 ;
  ELab = -1000;
  ThetaLab = -1000;
}

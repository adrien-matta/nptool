#include "Analysis.h"
using namespace std;

int main(int argc, char** argv)
{
   // command line parsing
   NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);

   // Instantiate RootInput
   string runToReadfileName = myOptionManager->GetRunToReadFile();
   RootInput:: getInstance(runToReadfileName);

   // if input files are not given, use those from TAsciiFile
   if (myOptionManager->IsDefault("DetectorConfiguration")) {
      string name = RootInput::getInstance()->DumpAsciiFile("DetectorConfiguration");
      myOptionManager->SetDetectorFile(name);
   }

   // get input files from NPOptionManager
   string detectorfileName    = myOptionManager->GetDetectorFile();
   string calibrationfileName = myOptionManager->GetCalibrationFile();
   string OutputfileName      = myOptionManager->GetOutputFile();
   
   // Instantiate RootOutput
   RootOutput::getInstance("Analysis/"+OutputfileName, "AnalysedTree");
   
   // Instantiate the detector using a file
   NPL::DetectorManager* myDetector = new DetectorManager();
   myDetector->ReadConfigurationFile(detectorfileName);
    
   // Get the formed Chained Tree and Treat it
   TChain* Chain = RootInput:: getInstance() -> GetChain();
    
    //	Instantiate some Reaction
    NPL::Reaction*  TransfertReaction = new Reaction								;
    TransfertReaction	->	ReadConfigurationFile("46Ar_pd.reaction")	;
    
    //Get Detector pointer :
    THiraPhysics* Hira 	  = (THiraPhysics*) 			myDetector -> GetDetector("HIRAArray")		;
    
    //TInteractionCoordinate
    TInteractionCoordinates* InteractionCoordinates = new TInteractionCoordinates();
    Chain->SetBranchStatus("InteractionCoordinates",true);
    Chain->SetBranchStatus("fDetected_*",true);
    Chain->SetBranchAddress("InteractionCoordinates",&InteractionCoordinates);
    RootOutput::getInstance()->GetTree()->Branch("InteractionCoordinates","TInteractionCoordinates",&InteractionCoordinates);
    
    //TInitialConditions
    TInitialConditions* InitialConditions = new TInitialConditions();
    Chain->SetBranchStatus("InitialConditions",true);
    Chain->SetBranchStatus("fIC_*",true);
    Chain->SetBranchAddress("InitialConditions",&InitialConditions);
    RootOutput::getInstance()->GetTree()->Branch("InitialConditions","TInitialConditions",&InitialConditions);
    
    RootOutput::getInstance()->GetTree()->Branch( "ThetaLab" , &ThetaLab , "ThetaLab/D" );
    RootOutput::getInstance()->GetTree()->Branch( "PhiLab" , &PhiLab , "PhiLab/D" )  ;
    RootOutput::getInstance()->GetTree()->Branch("ThetaCM", &ThetaCM,"ThetaCM/D") 	;
    RootOutput::getInstance()->GetTree()->Branch( "E_ThinSi" , &E_ThinSi , "E_ThinSi/D" )  ;
    RootOutput::getInstance()->GetTree()->Branch( "E_ThickSi" , &E_ThickSi , "E_ThickSi/D" )  ;
    RootOutput::getInstance()->GetTree()->Branch( "E_CsI" , &E_CsI , "E_CsI/D" )  ;
    RootOutput::getInstance()->GetTree()->Branch( "ELab" , &ELab , "ELab/D" )  ;
    RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy", &ExcitationEnergy,"ExcitationEnergy/D") ;
    RootOutput::getInstance()->GetTree()->Branch( "X" , &X , "X/D" )  ;
    RootOutput::getInstance()->GetTree()->Branch( "Y" , &Y , "Y/D" )  ;
    RootOutput::getInstance()->GetTree()->Branch( "Z" , &Z , "Z/D" )  ;
    RootOutput::getInstance()->GetTree()->Branch( "TelescopeNumber" , &TelescopeNumber , "TelescopeNumber/D" )  ;

   // Get number of events to treat
   cout << endl << "///////// Starting Analysis ///////// "<< endl;
   int nentries = Chain->GetEntries();
   cout << " Number of Event to be treated : " << nentries << endl;
   clock_t begin = clock();
   clock_t end = begin;
    
    double EnergyThreshold = 0.;

   // main loop on entries
   for (int i = 0; i < nentries; i++) {
       if (i%10000 == 0 && i!=0)  {
           cout.precision(5);
           end = clock();
           double TimeElapsed = (end-begin) / CLOCKS_PER_SEC;
           double percent = (double)i/nentries;
           double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
           cout  << "                                                                                                "<< flush;
           cout  << "\r Progression:" << percent*100 << " % \t | \t Remaining time : ~" <<  TimeToWait <<"s"<< flush;
       }
       else if (i == nentries-1)  cout << "\r Progression:" << " 100% " <<endl;

       // get data
       Chain -> GetEntry(i);
       ReInitOuputValue();

       myDetector->ClearEventPhysics();
       myDetector->BuildPhysicalEvent();
       
       if(Hira -> ThickSi_E.size() == 1){
           for(int countHira = 0 ; countHira < Hira->ThickSi_E.size() ; countHira++){
               
               TelescopeNumber      = Hira->TelescopeNumber[countHira];
               
               TargetThickness      = myDetector->GetTargetThickness();
               
               X = Hira->GetPositionOfInteraction(0).X();
               Y = Hira->GetPositionOfInteraction(0).Y();
               Z = Hira->GetPositionOfInteraction(0).Z();
               
               TVector3 PositionOnHira         = TVector3(X,Y,Z);
               TVector3 ZUnit                  = TVector3(0,0,1);
               //TVector3 TelescopeNormal      = -Hira->GetTelescopeNormal(countHira);
               TVector3 TargetNormal            = TVector3(0,0,1);//TVector3(-sin(TargetAngle*deg),0,cos(TargetAngle*deg));
               
               double X_target					= InitialConditions->GetIncidentPositionX();
               double Y_target					= InitialConditions->GetIncidentPositionY();
               double Z_target					= InitialConditions->GetIncidentPositionZ();
               
               TVector3 PositionOnTarget		= TVector3(X_target,Y_target,Z_target);
               //TVector3 PositionOnTarget		= TVector3(0,0,0);
               TVector3 HitDirection			= PositionOnHira - PositionOnTarget;
               TVector3 HitDirectionUnit		= HitDirection.Unit();
               
               //double ThetaNormalHira			= HitDirection.Angle(TelescopeNormal);
               double ThetaNormalTarget         = HitDirection.Angle(TargetNormal);
               
               //double ThetaBeam				= InitialConditions->GetICIncidentAngleTheta(countHira);
               //double PhiBeam					= InitialConditions->GetICIncidentAnglePhi(countHira);
               
               TVector3 BeamDirection			= InitialConditions->GetBeamDirection();
               
               //TVector3(sin(ThetaBeam*deg)*cos(PhiBeam*deg),sin(ThetaBeam*deg)*sin(PhiBeam*deg),cos(ThetaBeam*deg));
               
               double XBeam					= BeamDirection.X();
               double YBeam					= BeamDirection.Y();
               double ZBeam					= BeamDirection.Z();
               
               ThetaLab						= BeamDirection.Angle(HitDirection);
               ThetaLab						= ThetaLab/deg;
               
               PhiLab                       = HitDirection.Phi()/deg;
               
               E_ThickSi                    = Hira->ThickSi_E[countHira];
               E_ThinSi                     = Hira->ThinSi_E[countHira];
               ELab = E_ThinSi + E_ThickSi;
               if(Hira->CsI_E.size() == 1){
                   for(int countCsI =0; countCsI<Hira->CsI_E.size(); countCsI++){
                       E_CsI = Hira->CsI_E[countCsI];
                       if(E_CsI>EnergyThreshold)ELab += E_CsI;
                   }
               }
               else E_CsI = -1000;
               
               ELab	= EL_deuteron_CH2.EvaluateInitialEnergy(ELab,
                                                            (TargetThickness/2)*micrometer,
                                                            ThetaNormalTarget);

            
               // ********************** Angle in the CM frame *****************************
               TransfertReaction -> SetNuclei3(ELab, ThetaLab*deg);
               ThetaCM          = TransfertReaction->GetThetaCM()/deg;
               ExcitationEnergy	= TransfertReaction->GetExcitation4();
           }
       }
       RootOutput::getInstance()->GetTree()->Fill();
    }

   cout << "A total of " << nentries << " event has been annalysed " << endl ;

   RootOutput::getInstance()->Destroy();
   RootInput::getInstance()->Destroy();
   NPOptionManager::getInstance()->Destroy();

   return 0 ;
}


/////////////////////////////////////////////////////////////////////////////////////////
// On reinitialise les valeurs des branches de sorties à chaque entrée dans la boucle :
void ReInitOuputValue()
{
    E_ThinSi    = -1000;
    E_ThickSi   = -1000;
    E_CsI       = -1000;
    ELab        = -1000;
    ThetaLab    = -1000;
    PhiLab      = -1000;
    ThetaCM     = -1000;
    ExcitationEnergy = -1000;
    X           = -1000;
    Y           = -1000;
    Z           = -1000;
    TelescopeNumber = -1;
}


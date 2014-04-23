#include "Analysis.h"
using namespace std;

int main(int argc, char** argv){
  // command line parsing
  NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);

  // Instantiate RootInput
  string runToReadfileName = myOptionManager->GetRunToReadFile();
  RootInput:: getInstance("RunToTreat11Li.txt");
  //    RootInput:: getInstance("RunToTreat11Li_carbonne.txt");

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
  RootOutput::getInstance()->GetFile()->SetCompressionLevel(9);
  // Instantiate the detector using a file
  NPA::DetectorManager* myDetector = new DetectorManager();
  myDetector->ReadConfigurationFile(detectorfileName);
  // Get the formed Chained Tree and Treat it
  TChain* Chain = RootInput:: getInstance() -> GetChain();

  LoadCorrectionValueSSSD();
  // Attach new branch
  InitOutputBranch();
  RootInput::getInstance()->AddFriendChain("RunToTreatData11Li.txt");
  //   RootInput::getInstance()->AddFriendChain("RunToTreatData11Li_carbonne.txt");
  RootInput::getInstance()->AddFriendChain("RunToTreat11Li_T6.txt");
  //   RootInput::getInstance()->AddFriendChain("RunToTreat11Li_T6_carbonne.txt");
  InitInputBranch();

  //	Instantiate the Reaction
  NPL::Reaction*  He10Reaction = new Reaction ;

  He10Reaction	->	ReadConfigurationFile("10He.reaction") ;
  double Mass_10He = 7482.53 + 2.*939.565378; // Mass 8He + 2 * mass neutron
  double MassExcess_10He = (Mass_10He - 10*amu_c2 + 2* electron_mass_c2)*1000.; //keV
  He10Reaction -> 	GetNucleus4()	->	SetMassExcess(MassExcess_10He);
  NPL::Reaction*  He10Reaction2 = new Reaction ;
  He10Reaction2	->	ReadConfigurationFile("10He.reaction") ;
  He10Reaction2 -> 	GetNucleus4()	->	SetMassExcess(MassExcess_10He);
  ////////////////////////////////////////////////////////

  // Get pointer to the different detector
  TMust2Physics* M2  = (TMust2Physics*) myDetector -> GetDetector("MUST2");
  TSSSDPhysics* SSSD = (TSSSDPhysics*)  myDetector -> GetDetector("SSSD");

  LoadCorrectionTimeValue(2);
  // Fast analysis
  ifstream entry_file;
  //ofstream entry_file;
  entry_file.open("entry_file.txt");
  int next_event ;
  entry_file >> next_event ;

  // Get number of events to treat
  int nentries = Chain->GetEntries();
  //    int nentries = 1000000;
  cout << " Number of Event to be treated : " << nentries << endl;
  clock_t begin = clock();
  clock_t end = begin;
  cout.precision(5);

  /////////////////////////// main loop on entries ////////////////////////////
  for (int i = 0 ; i < nentries; i++) {
    if (i%10000 == 0 && i!=0){
      end = clock();
      long double TimeElapsed = (long double) (end-begin) / CLOCKS_PER_SEC;
      double percent = (double)i/nentries;
      double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
      cout  << "                                                        "<< flush;
      cout  << "\r Progression:" << percent*100 << " % \t | \t Remaining time : ~" 
        <<  TimeToWait <<"s |  Analysis Rate : "<< (double) i/TimeElapsed 
        << flush;
    }

    else if (i == nentries-1)  cout << "\r Progression:" << " 100% " <<endl;

    if(i == next_event){
      entry_file >> next_event;
      // get data
      Chain -> GetEntry(i);
      ReInitValue();
      TreatRIPS();

      // Beam Check:
      bool BeamCheck = false;

      // F2Pla vs F3Pla Time
      if( PlaF2_E > 490 && PlaF2_E < 730)
        if( PlaF3_T >33.5 && PlaF3_T < 39.5 )
          BeamCheck = true;

      double BeamTimeOfFlight;
      double BeamTOF_F3_Target;
      BeamEnergy=-1000;
      if(BeamCheck){
        // Calculate beam energy from Time of Flight between F2 and F3
        //BeamTimeOfFlight = a_BeamTof*PlaF3_T +b_BeamTof;

        // second order calibration
        double rawTOF = PlaF3_T-PlaF2_T;
        BeamTimeOfFlight = a_BeamTof + b_BeamTof*rawTOF + c_BeamTof*rawTOF*rawTOF;

        // relativiste formula
        BeamEnergy =  sqrt( 10286.*10286./(1.-pow((1./(299792458.*BeamTimeOfFlight*1e-9/F2_F3_distance)),2)) )- 10286.;
        //cout << BeamTimeOfFlight << " " << BeamEnergy << endl ;

        BeamTOF_F3_Target = 0 ;
        if(BeamEnergy<630){
          // Slow down the beam inside the F3 plastic and PPAC
          BeamEnergy = Li11_Mylar.Slow( BeamEnergy, 2.*2* 12.*micrometer + 3*1.5*2*micrometer, BeamDirection.Angle(TVector3(0,0,1)));
          BeamEnergy = Li11_BC400.Slow( BeamEnergy, 0.3*mm, BeamDirection.Angle(TVector3(0,0,1)));
          BeamEnergy = Li11_Mylar.Slow( BeamEnergy, 2.*2* 12.*micrometer + 3*1.5*2*micrometer, BeamDirection.Angle(TVector3(0,0,1)));

          // Calculate the time of flight of the Beam between F3 and the target
          BeamTOF_F3_Target = 1e-9*(  F3_Target_distance /sqrt( 1-(10286.*10286.)/(BeamEnergy+10286.)/(BeamEnergy+10286.) ) ) / (299792458.);

          // Slow down the beam inside the target

          BeamEnergy = Li11_CD2.Slow( BeamEnergy, TargetThickness/2., BeamDirection.Angle(TVector3(0,0,1)));
          He10Reaction->SetBeamEnergy(BeamEnergy);
        }

        else
          BeamEnergy = -1000;

      }


      int RealMultiplicity = 0;

      for(unsigned int countSSSD = 0 ; countSSSD < SSSD->Energy.size() ; countSSSD++){
        for(unsigned int countMust2 = 0 ; countMust2 < M2->Si_E.size() ; countMust2++){

          int TelescopeNumber = M2->TelescopeNumber[countMust2];
          int SiNumber = SSSD->DetectorNumber[countSSSD];

          if(TelescopeNumber==SiNumber)
            RealMultiplicity++ ;
        }
      }

      /// Rejection Analysis
      if(!BeamCheck) RejectBeam++;
      if(RealMultiplicity!=MaximumMultiplicity) RejectMultiplicity++;
      if(RealMultiplicity == 0 ) Multiplicity0++ ;
      if(RealMultiplicity > 1 ) MultiplicitySup1++ ;
      if(!BeamCheck || RealMultiplicity!=MaximumMultiplicity) RejectTotal++;

      if(BeamCheck &&  RealMultiplicity==MaximumMultiplicity && XTarget>-1000 && YTarget>-1000){
        //entry_file << i << endl ;
        if(T6->Si_E.size()==1){
          T6_SiE = T6->Si_E[0] ;

          if(T6->CsI_E.size()==1){
            T6_CsIE = T6->CsI_E[0];
            T6_CsIN = T6->CsI_N[0];
          }
        }

        RunNumber = RIPSData->GetRunNumber();


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
              //Part1: Time Correction
              if(CorrectionTimeMust2[TelescopeNumber-1][Y-1]!=500 && M2->CsI_E[countMust2]<0 ){
                T_M2 = CorrectionTimeMust2[TelescopeNumber-1][Y-1]+M2->Si_T[countMust2] ;
                T_M2_corr = T_M2+BeamTimeOfFlight+BeamTOF_F3_Target;
              }

              else 
                T_M2=-100;

              /************************************************/

              /************************************************/
              //Part 2 : Impact Angle
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

              //Part 3 : Impact Energy
              Si_E_M2 = M2->Si_E[countMust2];
              CsI_E_M2= M2->CsI_E[countMust2];
              E_SSSD = SSSD->Energy[countSSSD];

              // if CsI
              if(Si_E_M2 < 440 && Si_E_M2>0.01 && ThetaM2Surface!=-1000&& ThetaNormalTarget!=-1000){
                if(CsI_E_M2>0 ){
                  // The energy in CsI is calculate form dE/dx Table bevause calibration for 3He is not good
                  Energy = He3Si.EvaluateEnergyFromDeltaE(Si_E_M2,SiThicknessMUST2[TelescopeNumber-1],ThetaM2Surface, 0.01*MeV, 450.*MeV,0.001*MeV ,1000);
                  CsI_E_M2_calculated = Energy;
                  E_M2=CsI_E_M2;
                }

                else
                  Energy = Si_E_M2;

                E_M2 += Si_E_M2;

                // Evaluate energy using the measured effective thickness (Silicon equivalent)
                if(CorrectionA[DetectorNumber-1][(X-1)][(Y-1)]!=-100 ){
                  E_Corr = NormalisationA*SSSD->Energy[countSSSD]/(CorrectionA[DetectorNumber-1][(X-1)][(Y-1)]/cos(ThetaM2Surface));
                  ELab   = He3Si.EvaluateInitialEnergy( Energy ,CorrectionA[DetectorNumber-1][(X-1)][(Y-1)]*micrometer , ThetaM2Surface);
                  Energy = Energy + E_SSSD;
                  // Target Correction
                  ELab   = He3CD2.EvaluateInitialEnergy( ELab ,TargetThickness/2. - ZTarget, ThetaNormalTarget);
                }

                else {
                  Energy  =  -100;
                  ELab    =-100;
                  E_Corr=-100;
                }

              }
              /************************************************/

              /***********M2->Si_E[countMust2];*************************************/
              //Part 4 : Excitation Energy Calculation
              ExcitationEnergy = He10Reaction -> ReconstructRelativistic( ELab , ThetaLab );
              /************************************************/

              /************************************************/
              //Part 5 : Theta CM Calculation
              ThetaCM  = He10Reaction -> EnergyLabToThetaCM( ELab , 0)/deg;
              ThetaCM2 = He10Reaction -> EnergyLabToThetaCM( ELab , ThetaLab/rad)/deg;
              /************************************************/

              /************************************************/
              // Part 6: Corrected Kinematical information
              if(ELab>0 && ThetaLab>0 && ThetaCM==ThetaCM){
                He10Reaction2 -> SetExcitationHeavy(ExcitationEnergy);
                He10Reaction2 -> SetThetaCM(ThetaCM*deg);
                He10Reaction2 -> KineRelativistic(Corrected_ThetaLab3,Corrected_EnergyLab3 ,Corrected_ThetaLab4,Corrected_EnergyLab4);

                Corrected_ThetaLab3 = Corrected_ThetaLab3/deg; 
                Corrected_ThetaLab4 = Corrected_ThetaLab4/deg; 
                cout << ELab << " " << ThetaLab/deg << " " << Corrected_ThetaLab3 <<" " << Corrected_EnergyLab3 << endl ;
                // Test if we have nan (not a number)
                if(Corrected_ThetaLab3!=Corrected_ThetaLab3){
                  Corrected_ThetaLab3 = -1000;
                  Corrected_EnergyLab3 = -1000;
                  Corrected_ThetaLab4 = -1000;
                  Corrected_EnergyLab4 = -1000;
                }
              }
              /************************************************/
            }
          } //end loop SSSD
        }//end loop MUST2

        /************************************************/
        //Part 6 : Final Check
        if(ELab<0) ThetaLab = -100;
        else       ThetaLab = ThetaLab/deg;

        if(BeamDirection.X()==-1000){
          ReInitValue();
        }
        /************************************************/
        // final file, fill only on interresting event for faster analysis
        else if(ELab>0 && BeamEnergy>0)
          RootOutput::getInstance()->GetTree()->Fill();
      } //beam check
    } // if selected event for fast analysis
  } // loop over events

  cout << "A total of " << nentries << " event has been annalysed " << endl ;

  RootOutput::getInstance()->Destroy();
  RootInput::getInstance()->Destroy();
  NPOptionManager::getInstance()->Destroy();
  /////////////////////////////////////////////////////////////////////
  cout << "Event rejected Beam : " << RejectBeam << endl ;
  cout << "Event rejected Mult : " << RejectMultiplicity << endl ;
  cout << "Event rejected Mult >1 : " << MultiplicitySup1 << endl ;
  cout << "Event rejected Mult =0 : " << Multiplicity0 << endl ;
  cout << "Event rejected Total : " << RejectTotal << endl ;
  /////////////////////////////////////////////////////////////////////
  return 0 ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void InitOutputBranch()
{
  RootOutput::getInstance()->GetTree()->Branch("T_M2",&T_M2,"T_M2/D");
  RootOutput::getInstance()->GetTree()->Branch("T_M2_corr",&T_M2_corr,"T_M2_corr/D");
  RootOutput::getInstance()->GetTree()->Branch("X_M2",&X_M2,"X_M2/D");
  RootOutput::getInstance()->GetTree()->Branch("Y_M2",&Y_M2,"Y_M2/D");
  RootOutput::getInstance()->GetTree()->Branch("Z_M2",&Z_M2 ,"Z_M2/D");

  RootOutput::getInstance()->GetTree()->Branch("ZD1_E",&ZD1_E,"ZD1_E/D");
  RootOutput::getInstance()->GetTree()->Branch("ZD2_E",&ZD2_E ,"ZD2_E/D");
  RootOutput::getInstance()->GetTree()->Branch("ZD1_T",&ZD1_T,"ZD1_T/D");
  RootOutput::getInstance()->GetTree()->Branch("ZD2_T",&ZD2_T ,"ZD2_T/D");
  RootOutput::getInstance()->GetTree()->Branch("BeamEnergy",&BeamEnergy,"BeamEnergy/D");

  RootOutput::getInstance()->GetTree()->Branch("PlaF2_E",&PlaF2_E,"PlaF2_E/D");
  RootOutput::getInstance()->GetTree()->Branch("PlaF2_T",&PlaF2_T,"PlaF2_T/D");
  RootOutput::getInstance()->GetTree()->Branch("PlaF3_E",&PlaF3_E,"PlaF3_E/D");
  RootOutput::getInstance()->GetTree()->Branch("PlaF3_T",&PlaF3_T,"PlaF3_T/D");
  RootOutput::getInstance()->GetTree()->Branch("PlaF3_TL",&PlaF3_TL,"PlaF3_TL/D");

  RootOutput::getInstance()->GetTree()->Branch("PPACT0",&PPACT0,"PPACT0/D");
  RootOutput::getInstance()->GetTree()->Branch("PPACT1",&PPACT1,"PPACT1/D");
  RootOutput::getInstance()->GetTree()->Branch("PPAC0",&PPAC0,"PPAC0/I");
  RootOutput::getInstance()->GetTree()->Branch("PPAC1",&PPAC1,"PPAC1/I");

  RootOutput::getInstance()->GetTree()->Branch("XTarget",&XTarget,"XTarget/D");
  RootOutput::getInstance()->GetTree()->Branch("YTarget",&YTarget,"YTarget/D");
  RootOutput::getInstance()->GetTree()->Branch("Energy",&Energy,"Energy/D");

  RootOutput::getInstance()->GetTree()->Branch("RunNumber", &RunNumber,"RunNumber/I");
  RootOutput::getInstance()->GetTree()->Branch("DetectorNumber", &DetectorNumber,"DetectorNumber/I");
  RootOutput::getInstance()->GetTree()->Branch("Si_E_M2", &Si_E_M2,"Si_E_M2/D");
  RootOutput::getInstance()->GetTree()->Branch("CsI_E_M2", &CsI_E_M2,"CsI_E_M2/D");
  RootOutput::getInstance()->GetTree()->Branch("CsI_E_M2_calculated", &CsI_E_M2_calculated,"CsI_E_M2_calculated/D");
  RootOutput::getInstance()->GetTree()->Branch("Si_X_M2", &Si_X_M2,"Si_X_M2/I");
  RootOutput::getInstance()->GetTree()->Branch("Si_Y_M2", &Si_Y_M2,"Si_Y_M2/I");

  RootOutput::getInstance()->GetTree()->Branch("T6_SiE",&T6_SiE,"T6_SiE/D");
  RootOutput::getInstance()->GetTree()->Branch("T6_CsIE",&T6_CsIE,"T6_CsIE/D");
  RootOutput::getInstance()->GetTree()->Branch("T6_CsNE",&T6_CsIN,"T6_CsIN/I");

  RootOutput::getInstance()->GetTree()->Branch("E_M2",&E_M2,"E_M2/D");
  RootOutput::getInstance()->GetTree()->Branch("E_SSSD",&E_SSSD,"E_SSSD/D");
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLabHeavy",&ThetaLabHeavy,"ThetaLabHeavy/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM2 ",&ThetaCM2,"ThetaCM2/D");
  RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",&ExcitationEnergy,"ExcitationEnergy/D");

  RootOutput::getInstance()->GetTree()->Branch("E_Corr",&E_Corr,"E_Corr/D");

  RootOutput::getInstance()->GetTree()->Branch("Corrected_ThetaLab3",&Corrected_ThetaLab3,"Corrected_ThetaLab3/D");
  RootOutput::getInstance()->GetTree()->Branch("Corrected_EnergyLab3",&Corrected_EnergyLab3,"Corrected_EnergyLab3/D");
  RootOutput::getInstance()->GetTree()->Branch("Corrected_ThetaLab4",&Corrected_ThetaLab4,"Corrected_ThetaLab4/D");
  RootOutput::getInstance()->GetTree()->Branch("Corrected_EnergyLab4",&Corrected_EnergyLab4,"Corrected_EnergyLab4/D");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void InitInputBranch(){
  RootInput:: getInstance()->GetChain()->SetBranchAddress("RIPS",&RIPSData);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("GATCONF",&GATCONF);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("T6Tree.MUST2",&T6);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void LoadCorrectionValueSSSD(){
  // Initialize the Correction Array
  vector<double> BufferCoeffX; BufferCoeffX.resize(128,-100);
  vector< vector<double> > BufferCoeffXY; BufferCoeffXY.resize(128,BufferCoeffX);

  for (unsigned int i = 0; i < 4; ++i){
    CorrectionA.push_back(BufferCoeffXY);
  }

  // Read Value from the file
  TString basename = "./SSSDThickness";

  // Load Time Correction Value
  double Buffer;
  ifstream Correction_file;

  for(int k = 0 ; k < 4 ; k++){
    TString filename = basename + Form("/Si%d_RealThickness.dat", k+1);
    Correction_file.open(filename.Data());

    for(int j = 0 ; j <  128  ; j++){
      for(int i = 0 ; i <  128 ; i++){
        //ignore the first 2s value
        Correction_file >> Buffer >> Buffer ;
        Correction_file >> Buffer ;
        CorrectionA[k][j][i] = Buffer;
      }
    }
    Correction_file.close();
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadCorrectionTimeValue(int i){
  cout << endl << " Load Version "<< i << " of Time Correction" << endl;
  CorrectionTimeVersion = i ;
  string buffer;
  CorrectionTimeMust2.clear();
  TString basename = "./macro/Must2_ver2/Coeff";
  if(i == 1) basename+="_9Li";
  if(i == 2) basename+="_Li11";

  // Load Time Correction Value
  double BufferTime;
  vector<double> LineTimeBuffer;
  ifstream Time_file;
  for(int k = 0 ; k < 4 ; k++){
    LineTimeBuffer.clear();
    TString filename = basename + Form("/Cal_Str_Y_T_MM%d.peak", k+1);
    Time_file.open(filename.Data());
    for(int j = 0 ; j<  128 ; j++){
      Time_file >> buffer >> BufferTime;
      LineTimeBuffer.push_back(BufferTime);
    }

    CorrectionTimeMust2.push_back(LineTimeBuffer);
    Time_file.close();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ReInitValue(){
  T_M2 = -1000 ;
  T_M2_corr = -1000;
  X_M2 = -1000 ;
  Y_M2 = -1000 ;
  Z_M2 = -1000 ;
  ZD1_E = -1000;
  ZD2_E = -1000;
  ZD1_T = -1000;
  ZD2_T = -1000;
  BeamEnergy=-1000;
  XTarget = -1000 ;
  YTarget = -1000 ;
  Energy = -1000 ;
  Si_E_M2 = -1000 ;
  CsI_E_M2 = -1000;
  CsI_E_M2_calculated= -1000;
  Si_X_M2 = -1000 ;
  Si_Y_M2 = -1000 ;
  E_M2 = -1000 ;
  E_SSSD = -1000 ;
  T6_SiE = -1000 ;
  T6_CsIE = -1000 ;
  T6_CsIN = -1000 ;
  ELab = -1000 ;
  ThetaLab = -1000 ;
  ThetaLabHeavy = -1000 ;
  ThetaCM  = -1000 ;
  ThetaCM2  = -1000 ;
  ExcitationEnergy = -1000 ;
  DetectorNumber = -1000 ;
  RunNumber = -1000 ;
  PlaF2_E= -1000 ;
  PlaF2_T= -1000 ;
  PlaF3_E= -1000 ;
  PlaF3_T= -1000 ;
  PlaF3_TL= -1000 ;
  BeamDirection = TVector3(-1000,-1000,-1000);
  PPACT0 = -1000 ;
  PPACT1 = -1000 ;
  PPAC0 = -1000 ;
  PPAC1 = -1000 ;
  ThetaM2Surface=-1000;
  ThetaNormalTarget=-1000;
  E_Corr = -1000;
  Corrected_ThetaLab3=-1000;
  Corrected_EnergyLab3=-1000;
  Corrected_ThetaLab4=-1000;
  Corrected_EnergyLab4=-1000;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void TreatRIPS(){
  // Treat Plastic
  for(int pla = 0 ; pla < RIPSData->GetMultPlastic() ; pla++){
    int plastic_number= RIPSData->GetPlasticN(pla);

    if ( plastic_number== 0){
      PlaF2_E = RIPSData->GetPlasticE(pla);
      PlaF2_T = RIPSData->GetPlasticTR(pla);
    }

    else if (plastic_number == 1){
      PlaF3_E  = RIPSData->GetPlasticE(pla);
      PlaF3_T  = (RIPSData->GetPlasticTR(pla)+RIPSData->GetPlasticTL(pla))/2.;

    }

    else if (plastic_number == 2){
      ZD1_E = RIPSData->GetPlasticE(pla);
      ZD1_T = RIPSData->GetPlasticTR(pla);
    }

    else if (plastic_number == 3){
      ZD2_E = RIPSData->GetPlasticE(pla);
      ZD2_T = RIPSData->GetPlasticTR(pla);
    }

  }

  // Treat PPAC
  double X0=0,Y0=0,Z0=0,Z0X=0,Z0Y=0;
  double X1=0,Y1=0,Z1=0,Z1X=0,Z1Y=0;
  bool check0=false,check1=false;

  ZTarget = 0 ;

  for(int ppac = 0 ; ppac < RIPSData->GetMultPPAC() ; ppac++ ){
    //////////////////////////////////////////////////////////////
    // First group of PPAC outside the reaction chamber
    if(RIPSData->GetPPACN(ppac)==0){ // priority is given to ppac A
      X0 = RIPSData->GetPPACX(ppac) ;
      Y0 = RIPSData->GetPPACY(ppac) ;
      PPACT0 = RIPSData->GetPPACT(ppac) ;
      Z0 = ZPPAC_A - ZTarget;
      Z0X = ZPPAC_A_X - ZTarget;
      Z0Y = ZPPAC_A_Y - ZTarget;
      PPAC0 = 0 ;
      check0=true;
    }

    else if (RIPSData->GetPPACN(ppac)==1 && !check0){
      X0 = RIPSData->GetPPACX(ppac) ;
      Y0 = RIPSData->GetPPACY(ppac) ;
      PPACT0 = RIPSData->GetPPACT(ppac) ;
      Z0 = ZPPAC_B - ZTarget;
      Z0X = ZPPAC_B_X - ZTarget;
      Z0Y = ZPPAC_B_Y - ZTarget;
      PPAC0 = 1 ;
      check0=true;
    }
    //////////////////////////////////////////////////////////////
    // Second group of PPAC (tandem), inside the reaction chamber
    if(RIPSData->GetPPACN(ppac)==3){ // priority is given to ppac TB, more stable
      X1 = RIPSData->GetPPACX(ppac) ;
      Y1 = RIPSData->GetPPACY(ppac) ;
      PPACT1 = RIPSData->GetPPACT(ppac) ;
      Z1 = ZPPAC_TB - ZTarget;
      Z1X = ZPPAC_TB_X - ZTarget;
      Z1Y = ZPPAC_TB_Y - ZTarget;
      PPAC1 = 3;
      check1=true;

      // Mizuki correction
      X1 = X1 + 2.038 - 0.166 * X1 ;
      Y1 = Y1 - 1.98  - 0.098 * Y1 ;
    }

    else if (RIPSData->GetPPACN(ppac)==2 && !check1){
      X1 = RIPSData->GetPPACX(ppac) ;
      Y1 = RIPSData->GetPPACY(ppac) ;
      PPACT1 = RIPSData->GetPPACT(ppac) ;
      Z1 = ZPPAC_TA - ZTarget;
      Z1X = ZPPAC_TA_X - ZTarget;
      Z1Y = ZPPAC_TA_Y - ZTarget;
      PPAC1 = 2 ;
      check1=true;

      // Mizuki correction
      X1 = X1 + 2.47 - 0.1135 * X1 ;
      Y1 = Y1 - 1.05 - 0.0911 * Y1 ;
    }

  }
  /// Build my own position on target
  if(check0 && check1){
    // Reverse X and Y
    X0 = -X0 ; X1 = -X1 ;
    XTarget = X1 + (X1-X0)*( (ZTarget-Z1X)/(Z1X-Z0X) );
    YTarget = Y1 + (Y1-Y0)*( (ZTarget-Z1Y)/(Z1Y-Z0Y) );
    BeamDirection = TVector3(X1-X0,Y1-Y0,Z1-Z0);
  }

  else{
    XTarget = -1000;
    YTarget = -1000;
  }

  ZTarget = Rand.Uniform(-TargetThickness/2.,TargetThickness/2.); // ZTarget is randomly shoot in micrometer unit
  ZTarget = ZTarget/mm ;// convert to mm unit to be consistent with X and Y
}

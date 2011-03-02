#include "Analysis.h"
using namespace std;

int main(int argc, char** argv)
{
   // command line parsing
   NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);

   // Instantiate RootInput
   string runToReadfileName = myOptionManager->GetRunToReadFile();
   RootInput:: getInstance("RunToTreatLi9SSD.txt");
   
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
   RootOutput::getInstance()->GetFile()->SetCompressionLevel(0);
   // Instantiate the detector using a file
   NPA::DetectorManager* myDetector = new DetectorManager();
   myDetector->ReadConfigurationFile(detectorfileName);
   // Get the formed Chained Tree and Treat it
   TChain* Chain = RootInput:: getInstance() -> GetChain();

  
   LoadCorrectionValueSSSD();
   // Attach new branch
   InitOutputBranch();
   RootInput:: getInstance()->AddFriendChain("RunToTreatDataLi9SSD.txt");
   InitInputBranch();
   
   //	Instantiate the Reaction
	NPL::Reaction*  He8Reaction = new Reaction ;
	He8Reaction	->	ReadConfigurationFile("8He.reaction") ;
   
   ////////////////////////////////////////////////////////
   
   // Get pointer to the different detector
   
   TMust2Physics* M2  = (TMust2Physics*) myDetector -> GetDetector("MUST2");
   TSSSDPhysics* SSSD = (TSSSDPhysics*)  myDetector -> GetDetector("SSSD");

   // Get number of events to treat
   cout << endl << "///////// Starting Analysis ///////// "<< endl;
   int nentries = Chain->GetEntries();
   cout << " Number of Event to be treated : " << nentries << endl;
   clock_t begin = clock();
   clock_t end = begin;
   
   // main loop on entries
   for (int i = 0 ; i < nentries; i++) {
      if (i%10000 == 0 && i!=0)  {
         cout.precision(5);
         end = clock();
         double TimeElapsed = (end-begin) / CLOCKS_PER_SEC;
         double percent = (double)i/nentries;
         double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
         cout  << "                                                                                                  "<< flush;
         cout  << "\r Progression:" << percent*100 << " % \t | \t Remaining time : ~" <<  TimeToWait <<"s"<< flush;
      }
      else if (i == nentries-1)  cout << "\r Progression:" << " 100% " <<endl;

      // get data
      Chain -> GetEntry(i);
      TVector3 BeamDirection = TVector3(-1000,-1000,-1000);
      ReInitValue();
      if( M2->Si_E.size() <= MaximumMultiplicity )
         {
            for(unsigned int countMust2 = 0 ; countMust2 < M2->Si_E.size() ; countMust2++){
               for(unsigned int countSSSD = 0 ; countSSSD < SSSD->Energy.size() ; countSSSD++){
      
                  /************************************************/
                  //Part 0 : Get the usefull Data
                  
                  // MUST2
                  int X = M2->Si_X[countMust2];
                  int Y = M2->Si_Y[countMust2];
                  int TelescopeNumber = M2->TelescopeNumber[countMust2];
                  
                  //SSSD
                  int SiNumber = SSSD->DetectorNumber[countSSSD];

                  /************************************************/
                  if(TelescopeNumber==SiNumber)
                     {
                        /************************************************/
                        //Part 1 : Thickness Correction
                        if(CorrectionValidation[TelescopeNumber-1][X-1][Y-1] < Validation && M2->CsI_E[countMust2]<0){
                           Energy_Must2 = M2->Si_E[countMust2] + CorrectionMust2[TelescopeNumber-1][X-1][Y-1];
                           Energy_SSSD = SSSD->Energy[countSSSD] + CorrectionSSSD[TelescopeNumber-1][X-1][Y-1];
                           }
                           
                        /************************************************/

                        /************************************************/
                        //Part 2 : Impact Angle
                        
                        /// Build my own position on target
                           bool PPAC_hit[4]={false,false,false,false};
                           
                           for(int k = 0 ; k <4 ; k++)
                              {
                                 if(PPACF3[k][0]>-1000 && PPACF3[k][1]>-1000) PPAC_hit[k]=true;
                              }
                           
                           // select the two PPAC the more distant:
                           // find the first one:
                           int first = -1;
                           for(int k = 0 ; k <4 ; k++)
                              if(PPAC_hit[k]) {first = k;break;}
                           
                           // find the second one:
                           int second = -1;
                           
                           if(first>-1)
                           for(int k = first ; k<4 ;k++)
                              if(PPAC_hit[k]){second = k;}
                              
                           if(first>-1 && second>-1 && first!=second)
                              {
                                 double X0= PPACF3[first][0];
                                 double X1= PPACF3[second][0];
                                 double Y0= PPACF3[first][1];
                                 double Y1= PPACF3[second][1];
                                 double Z0 = (PPACF3[first][2]+PPACF3[first][2])/2;
                                 double Z1 = (PPACF3[second][2]+PPACF3[second][2])/2;
                                 TVector3 firstVect = TVector3(X0,Y0,Z0);
                                 TVector3 secondVect = TVector3(X1,Y1,Z1);
                                 BeamDirection = secondVect - firstVect;
                                 
	                              double l = sqrt((Z0-Z1)*(Z0-Z1));
	                              double L = - Z1;
	                              double t = (l+L) / l;
	                 
                            	   XTarget = X0 + (X1 - X0) * t ;
                             	   YTarget = Y0 + (Y1 - Y0) * t ;
                              }
                        
                        TVector3 BeamImpact(XTarget,YTarget,ZTarget);
                        
                        TVector3 HitDirection = M2 -> GetPositionOfInteraction(countMust2) - BeamImpact ;
                        ThetaLab = HitDirection.Angle( BeamDirection );
                        double ThetaM2Surface = HitDirection.Angle( M2 -> GetTelescopeNormal(countMust2) );
                        double ThetaNormalTarget = BeamDirection.Angle( TVector3(0,0,1) ) ;

                        /************************************************/
                        
                        /************************************************/
                        //Part 3 : Impact Energy
                        // No CsI
                        
                        if(M2->CsI_E[countMust2]<0){
                           ELab  = M2->Si_E[countMust2];
                           ELab  = He3Al .EvaluateInitialEnergy( ELab , AlThicknessMUST2[TelescopeNumber] , ThetaM2Surface);
                           ELab  = He3Al .EvaluateInitialEnergy( ELab , AlThicknessSSSD[SiNumber]         , ThetaM2Surface);
                           ELab += SSSD->Energy[countSSSD]; 
                           ELab  = He3Al .EvaluateInitialEnergy( ELab , AlThicknessSSSD[SiNumber]         , ThetaM2Surface);
                           ELab  = He3CD2.EvaluateInitialEnergy( ELab , TargetThickness                   , ThetaNormalTarget);
                           
                        }

                        // with CsI
                        if(M2->CsI_E[countMust2]>0){
                        /* Not yet Calibrate, so do nothing */
                        }
                        /************************************************/
                        
                        /************************************************/
                        //Part 4 : Excitation Energy Calculation
                        ExcitationEnergy = He8Reaction -> ReconstructRelativistic( ELab , ThetaLab );	
                        

                        /************************************************/

                        /************************************************/
                        //Part 5 : Theta CM Calculation
                        if(ExcitationEnergy>0)
                           ThetaCM = He8Reaction -> EnergyLabToThetaCM( ELab , ExcitationEnergy)/deg;	
                        
                        ThetaLab = ThetaLab/deg;

                        /************************************************/
                        
                     }
                  
               }// end loop SSSD
            }//end loop MUST2
         }

         /************************************************/
         //Part 6 : Final Check
         
         if(ELab<0) ThetaLab = -100;
         else       ThetaLab = ThetaLab/deg;

         // Check for validity
         if(TMath::IsNaN(XTarget))          XTarget= -110 ;
         if(TMath::IsNaN(YTarget))          YTarget= -110 ;
         if(TMath::IsNaN(Energy_Must2))     Energy_Must2= -110 ;
         if(TMath::IsNaN(Energy_SSSD))      Energy_SSSD= -110 ;
         if(TMath::IsNaN(ELab))             ELab= -110 ;
         if(TMath::IsNaN(ThetaLab))         ThetaLab= -110 ;
         if(TMath::IsNaN(ThetaCM))          ThetaCM= -110 ;
         if(TMath::IsNaN(ExcitationEnergy)) ExcitationEnergy = -110 ;
         
         if(BeamDirection.X()==-1000)
            {
               XTarget= -120 ;
               YTarget= -120 ;
               Energy_Must2= -120 ;
               Energy_SSSD= -120 ;
               ELab= -120 ;
               ThetaLab= -120 ;
               ThetaCM= -120 ;
               ExcitationEnergy = -120 ;
            }
         /************************************************/


      RootOutput::getInstance()->GetTree()->Fill();
   }

   cout << "A total of " << nentries << " event has been annalysed " << endl ;

   RootOutput::getInstance()->Destroy();
   RootInput::getInstance()->Destroy();
   NPOptionManager::getInstance()->Destroy();

   return 0 ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void InitOutputBranch() 
   {
      RootOutput::getInstance()->GetTree()->Branch("XTarget",&XTarget,"XTarget/D");
      RootOutput::getInstance()->GetTree()->Branch("YTarget",&YTarget,"YTarget/D");
      RootOutput::getInstance()->GetTree()->Branch("Energy_Must2",&Energy_Must2,"Energy_Must2/D");
      RootOutput::getInstance()->GetTree()->Branch("Energy_SSSD",&Energy_SSSD,"Energy_SSSD/D");
      RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
	   RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
	   RootOutput::getInstance()->GetTree()->Branch("ThetaCM ",&ThetaCM,"ThetaCM/D");
      RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",&ExcitationEnergy,"ExcitationEnergy/D");	 
   }
   
////////////////////////////////////////////////////////////////////////////////////////////////////
void InitInputBranch()
   {
      RootInput:: getInstance()->GetChain()->SetBranchStatus("t1.SSSD",false);
      RootInput:: getInstance()->GetChain()->SetBranchStatus("t1.MUST2",false);
      
      RootInput:: getInstance()->GetChain()->SetBranchAddress("PPACF3",PPACF3);
      RootInput:: getInstance()->GetChain()->SetBranchAddress("FPPosition",FPPosition);
      RootInput:: getInstance()->GetChain()->SetBranchAddress("FPTime",FPTime);
      RootInput:: getInstance()->GetChain()->SetBranchAddress("FPdE",FPdE);
      RootInput:: getInstance()->GetChain()->SetBranchAddress("RF",RF);
      RootInput:: getInstance()->GetChain()->SetBranchAddress("Pla",Pla);
      RootInput:: getInstance()->GetChain()->SetBranchAddress("GATCONF",&GATCONF);
   }
////////////////////////////////////////////////////////////////////////////////////////////////////   
void LoadCorrectionValueSSSD()
   {
      ////////////////////////////////////////////////////////
      // Load Value for Thickness correction in SSSD
      string buffer;
      double BufferMust2, BufferSSSD;
      vector<double> LineBufferMust2,LineBufferSSSD;
      vector< vector<double> > BufferCorrectionMust2,BufferCorrectionSSSD;
      ifstream correction_file;

      for(int k = 0 ; k < 4 ; k++)
         {   
            BufferCorrectionMust2.clear();
            BufferCorrectionSSSD.clear();
            TString filename = Form("./macro/SSSD_Thickness_Correction/Si%d_ThicknessCorrection.dat", k+1);
            cout << " Loading " << filename << endl ;
            correction_file.open(filename.Data());
            
            for(int i = 0 ; i < 128 ; i++)
               {
                 LineBufferMust2.clear();LineBufferSSSD.clear();
                 for(int j = 0 ; j<  128 ; j++)
                   {
                     correction_file >> buffer >> buffer >> BufferSSSD >> BufferMust2;
                     LineBufferMust2.push_back(BufferMust2);
                     LineBufferSSSD.push_back(BufferSSSD);
                   }

                 BufferCorrectionMust2.push_back(LineBufferMust2);
                 BufferCorrectionSSSD.push_back(LineBufferSSSD);
               }
            correction_file.close();
            CorrectionMust2.push_back(BufferCorrectionMust2);
            CorrectionSSSD.push_back(BufferCorrectionSSSD);
         }
         
      // Load Validation Value
      double BufferVal;
      vector<double> LineBuffer;
      vector< vector<double> > BufferValidation;
      ifstream Validation_file;

      for(int k = 0 ; k < 4 ; k++)
      {   
         BufferValidation.clear();
         TString filename = Form("./macro/SSSD_Thickness_Correction/Si%d_AutoCoherence.dat", k+1);
         cout << " Loading " << filename << endl ;
         Validation_file.open(filename.Data());

         for(int i = 0 ; i < 128 ; i++)
            {
              LineBufferMust2.clear();LineBufferSSSD.clear();
              for(int j = 0 ; j<  128 ; j++)
                {
                  Validation_file >> buffer >> buffer >> BufferVal;
                  LineBuffer.push_back(BufferVal);
                }

              BufferValidation.push_back(LineBuffer);
            }
         correction_file.close();
         CorrectionValidation.push_back(BufferValidation);
      }

   }
////////////////////////////////////////////////////////////////////////////////////////////////////      
void ReInitValue()
   {
   
      XTarget = -1000 ;
      YTarget = -1000 ;
      Energy_Must2 = -100 ;
      Energy_SSSD = -100 ;
	   ELab = -100 ;
	   ThetaLab = -100 ;
	   ThetaCM  = -100 ;
      ExcitationEnergy = -100 ;
   }
	

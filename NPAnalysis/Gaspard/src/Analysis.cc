#include "must.hh"

#include <fstream>
#include "sstream"
#include <string>
#include <cmath>

//include ROOT
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH2.h"
#include "TStyle.h"
#include "TRandom.h"

//NPL
#include "TGaspardTrackerData.h"
#include "TInteractionCoordinates.h"
#include "TInitialConditions.h"
#include "NPReaction.h"
//#include "NPEnergyLossCorrection.h"
using namespace NPL ;

// Use CLHEP System of unit and Physical Constant
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Vector/ThreeVector.h"

//NPA
#include "ConfigurationReader.hh"
#include "ObjectManager.hh"

// prototype	
double ThetaCalculation( double StripX , double StripY ,double StripZ, double PPAC_X, double PPAC_Y, double PPAC_Theta);
double HexaEnergy(double HeavyEnergy ,double ThetaHeavy , double Excitation);
/////////////

int main(int argc,char** argv)
{
   // A Usefull Random Generator
   TRandom rand;

   // Get arguments from command line	
   if (argc != 4) {
      cout << 
		"you need to specify a Reaction file, a Detector file and a ROOT file such as : Analysis myReaction.reaction myDetector.detector myRootFile.root" 
		<< endl;
      return 0;
   }	
   string reactionfileName = argv[1];
   string detectorfileName = argv[2];
   string rootfileName     = argv[3];

   //////////////////////////////////////////////////////////////////////
   ///////////////////////// Load Reaction //////////////////////////////
   //////////////////////////////////////////////////////////////////////
   Reaction* myReaction = ReadReaction(reactionfileName);

   //////////////////////////////////////////////////////////////////////
   ///////////////////////// Load  Detector /////////////////////////////
   //////////////////////////////////////////////////////////////////////
   array* myArray = new array();
   ReadConfiguration(detectorfileName, myArray);
	
   cout << "////////////////////////////////////////////" << endl;
   cout << "Total Number Of telescope : " << myArray->GetNumberOfTelescope() << endl;
   cout << "////////////////////////////////////////////" << endl;

   //////////////////////////////////////////////////////////////////////
   /////////////////////// Load ROOT file ///////////////////////////////
   //////////////////////////////////////////////////////////////////////
   // Open output ROOT file from NPTool simulation run
   string path = getenv("NPTOOL");
   path += "/Outputs/Simulation/" + rootfileName;
   TFile *inFile = new TFile(path.c_str());
   TTree *tree   = (TTree*) inFile->Get("SimulatedTree");

//   TChain* t1 = new TChain("SimulatedTree");
//   t1->Add("../../Outputs/Simulation/mySimul.root");	
   TGaspardTrackerData* EventGPD = new TGaspardTrackerData();
   tree->SetBranchAddress("GASPARD",&EventGPD);
   TInteractionCoordinates* InterCoord = new TInteractionCoordinates();
   tree->SetBranchAddress("InteractionCoordinates",&InterCoord);
   TInitialConditions* InitCond = new TInitialConditions();
   tree->SetBranchAddress("InitialConditions",&InitCond);

   //////////////////////////////////////////////////////////////////////
   /////////////////////// Load ROOT file ///////////////////////////////
   //////////////////////////////////////////////////////////////////////
   for (Int_t i = 0; i < NbElements; i++) {
      TString hname  = Form("hHitPattern%d", i+1);
      TString htitle = Form("HitPattern%d", i+1);
      hHitPattern[i] = new TH2F(hname, htitle, 128, 1, 128, 128, 1, 128);
   }

   //////////////// Analyse Part ////////////////////////////////////////	
   Int_t nentries = (Int_t) tree->GetEntries();
   cout << "There are " << nentries << " entries to process" << endl;
   for (Int_t e = 0; e < nentries; e++) {
      check_light =false	;
      if ((e+1)%10000==0) cout << "Entries number " << e+1 << " on " << tree->GetEntries() << endl ; 
      tree->GetEntry(e);

      // first check that there is an event detected
      if (EventGPD->GetGPDTrkFirstStageFrontEMult() > 0) {
         // calculate multiplicity
         Int_t multXE = EventGPD->GetGPDTrkFirstStageFrontEMult();
         Int_t multYE = EventGPD->GetGPDTrkFirstStageBackEMult();
         Int_t multXT = EventGPD->GetGPDTrkFirstStageFrontTMult();
         Int_t multYT = EventGPD->GetGPDTrkFirstStageBackTMult();
         // case of multiplicity 1 for the strips
         if (multXE==1 && multYE==1 && multXT==1 && multYT==1) {
            // calculate detector number
            Int_t det_ref = EventGPD->GetGPDTrkFirstStageFrontEDetectorNbr(0);
            Int_t detecXE = EventGPD->GetGPDTrkFirstStageFrontEDetectorNbr(0) / det_ref;
            Int_t detecXT = EventGPD->GetGPDTrkFirstStageFrontTDetectorNbr(0) / det_ref;
            Int_t detecYE = EventGPD->GetGPDTrkFirstStageBackEDetectorNbr(0) / det_ref;
            Int_t detecYT = EventGPD->GetGPDTrkFirstStageBackTDetectorNbr(0) / det_ref;
            det_ref -= 1000; // for TGaspardTrackerDummyShape
            // case of same detector
            if (detecXE*detecXT*detecYE*detecYT == 1) {
               // calculate strip number
               Int_t stripXE = EventGPD->GetGPDTrkFirstStageFrontEStripNbr(0);
               Int_t stripXT = EventGPD->GetGPDTrkFirstStageFrontTStripNbr(0);
               Int_t stripYE = EventGPD->GetGPDTrkFirstStageBackEStripNbr(0);
               Int_t stripYT = EventGPD->GetGPDTrkFirstStageBackTStripNbr(0);
               // case of same strips on X and Y
               if (stripXE == stripXT  &&  stripYE == stripYT) { 	// here we have a good strip event
                  // fill hit pattern histograms
                  hHitPattern[det_ref-1]->Fill(stripXE, stripYE);
                  // get energy from strips
                  Double_t EnergyStripX = EventGPD->GetGPDTrkFirstStageFrontEEnergy(0) * MeV;
                  Double_t EnergyStripY = EventGPD->GetGPDTrkFirstStageBackEEnergy(0) * MeV;
                  Double_t EnergyStrip  = 0.5 * (EnergyStripX + EnergyStripY);
//                  Double_t EnergyStrip  = EnergyStripX;
//                  if (EnergyStripY > EnergyStrip) EnergyStrip = EnergyStripY;
                  Double_t TotalEnergy = EnergyStrip;
                  // fill detected energy
                  hEnerX->Fill(EnergyStripX);
                  hEnerY->Fill(EnergyStripY);
                  hEnerXY->Fill(EnergyStripX, EnergyStripY);
                  // calculate multiplicity of 2nd and third stages
                  Int_t mult2E = EventGPD->GetGPDTrkSecondStageEMult();
                  Int_t mult2T = EventGPD->GetGPDTrkSecondStageTMult();
                  Int_t mult3E = EventGPD->GetGPDTrkThirdStageEMult();
                  Int_t mult3T = EventGPD->GetGPDTrkThirdStageTMult();
                  // check if we have a 2nd stage event
                  if (mult2E==1 && mult2T==1) {
                     Double_t EnergySecond = EventGPD->GetGPDTrkSecondStageEEnergy(0);
                     TotalEnergy += EnergySecond;
                  }
                  // check if we have a third stage event
                  if (mult3E==1 && mult3T==1) {
                     Double_t EnergyThird = EventGPD->GetGPDTrkThirdStageEEnergy(0);
                     TotalEnergy += EnergyThird;
                  }
                  // get emitted angle in the beam frame
//                  Double_t Theta = InitCond->GetICEmittedAngleThetaLabIncidentFrame(0) * deg;
                  Double_t Theta = InterCoord->GetDetectedAngleTheta(0) * deg;
                  hTheta->Fill(Theta / deg);
                  hEnerTheta->Fill(Theta / deg, TotalEnergy);
                  // calculate lab angle from strips
                  Double_t xstrip = myArray->GetStripPositionX(det_ref, stripXE, stripYE);
                  Double_t ystrip = myArray->GetStripPositionY(det_ref, stripXE, stripYE);
                  Double_t zstrip = myArray->GetStripPositionZ(det_ref, stripXE, stripYE);
                  Hep3Vector posstrip(xstrip, ystrip, zstrip);
                  Double_t ThetaStrip = posstrip.theta() * rad;
                  cout << Theta/deg << "  " << ThetaStrip/deg << endl;
                  // calculate excitation energy
//                  Ex = myReaction->ReconstructRelativistic(TotalEnergy / MeV, Theta / rad);
                  Ex = myReaction->ReconstructRelativistic(TotalEnergy / MeV, ThetaStrip / rad);
                  // condition on c.m. angle
                  // for (d,p) reaction theta_c.m. < 40 deg
                  Double_t ThetaCM = myReaction->EnergyLabToThetaCM(TotalEnergy / MeV, -500) * rad;
                  hThetaCM->Fill(ThetaCM / deg);
                  hThetaLabCM -> Fill(ThetaCM/deg,Theta/deg);
//                  if (ThetaCM/deg < 20) {
                     hEx->Fill(Ex);
//                  }
               }
               else {
                  cout << "Not same strips" << endl;
               }
            }
            else {
               cout << "Not same detector" << endl;
            }
         }
         else {
            cout << "Multiplicity is not one, it is: " << endl;
            cout << "\tmultXE: " << multXE << endl;
            cout << "\tmultXT: " << multXT << endl;
            cout << "\tmultYE: " << multYE << endl;
            cout << "\tmultYT: " << multYT << endl;
         }
      }
/*			
			PhysicsMUST2->BuildPhysicalEvent(EventMUST2) ;
			
			///////////////////////
			// Put your Analysis Here
			
			if( PhysicsMUST2->TelescopeNumber.size()>0)
				{
				for(Int_t mult = 0 ; mult < PhysicsMUST2->TelescopeNumber.size() ; mult++)
					{
					N=PhysicsMUST2->TelescopeNumber.at(mult);
					  
							Double_t Theta = rad *	ThetaCalculation( 
											Xs=myArray->GetStripPositionX( N , X=PhysicsMUST2->Si_X.at(mult) , Y=PhysicsMUST2->Si_Y.at(mult) ) 	,
											Ys=myArray->GetStripPositionY( N , X=PhysicsMUST2->Si_X.at(mult) , Y=PhysicsMUST2->Si_Y.at(mult) ) 	,
											Zs=myArray->GetStripPositionZ( N , X=PhysicsMUST2->Si_X.at(mult) , Y=PhysicsMUST2->Si_Y.at(mult) ) 	,
											BeamX																				 			 	,
											BeamY																				 		 		,
											BeamTheta																					 	   );
									
									// Rand prevent binning effect due to discret Hit position pattern (strip center)							
									hXY->Fill( (Xs-1)+2*rand.Uniform() ,(Ys-1)+2*rand.Uniform() );
									
									if(N==1)hXY1 -> Fill(X,Y)	;
									if(N==2)hXY2 -> Fill(X,Y)	;
									if(N==3)hXY3 -> Fill(X,Y)	;
									if(N==4)hXY4 -> Fill(X,Y)	;
															
									Double_t Energy = PhysicsMUST2->TotalEnergy.at(mult)*MeV	 		;
									
									//	Correct Energy Loss in the target 
//									Energy = ProtonTarget.EvaluateInitialEnergy(	Energy	 					, 
//		   																			20.618556701*micrometer		, 
//		   																			Theta						);
		   									
		   									
									Ex = myReaction->ReconstructRelativistic( Energy , Theta ) 	; 
									
									ThetaCM = myReaction->EnergyLabToThetaCM(Energy)			;
									hThetaLabCM -> Fill(ThetaCM/deg,Theta/deg)					;
									hThetaCM    -> Fill(ThetaCM/deg)							;
									hEA-> Fill(Theta/deg , Energy)								;
									//hEA-> Fill(InitialTheta , InitialEnergy)					;
									hTheta->Fill(Theta/deg)										;
									check_light = true	;
									
									hEx->Fill(Ex);
*/									
									/*if(N==5 && check_light)
										{
											
										double Hexa = HexaEnergy(PhysicsMUST2->TotalEnergy.at(mult) ,Ex) ;
										cout << Hexa << endl ;
										hEHexa->Fill(Hexa);
										}*/
/*					}
					
				}
				
			PhysicsMUST2->Clear();	
			//////////////////////
			
			PhysicsMUST2->Clear();*/
		}
	
	TFile outfile("./Result/myResult.root","RECREATE")	;
		
        for (Int_t i = 0; i < NbElements; i++) hHitPattern[i]->Write();
	hTheta		-> Write()	;
	hThetaCM 	-> Write()	;
	hThetaLabCM -> Write()	;
	hXY			-> Write()	;
	hXY1		-> Write()	;
	hXY2		-> Write()	;
	hXY3		-> Write()	;
	hXY4		-> Write()	;
	hXY5		-> Write()	;
	hXY6		-> Write()	;
	hEx			-> Write()	;
	hEA			-> Write()	;
	hEHexa		-> Write()	;
        hEnerX->Write();
        hEnerY->Write();
        hEnerXY->Write();
        hEnerTheta->Write();

	outfile.Close()	;	
	
return 0	;
}



double ThetaCalculation( double StripX , double StripY ,double StripZ, double PPAC_X, double PPAC_Y, double PPAC_Theta)
	{
		double TargetZ = -50;//mm
		StripX = StripX-PPAC_X ; StripY = StripY-PPAC_Y ; StripZ=StripZ-TargetZ	;
		double norme = sqrt ( StripX*StripX + StripY*StripY + StripZ*StripZ )	;
		double Theta = acos ( StripZ / norme )									; 
		return (Theta-PPAC_Theta)												; //return Theta in rad
	}


double HexaEnergy(double HeavyEnergy ,double ThetaHeavy , double Excitation)
	{
			/*double M10He  = 9362.73 + Excitation	;
			double Malpha = 3727.42					;
			double Mn     = 939.56536				;
			double M2 = M10He * M10He 				;
			double Ma2 = Malpha * Malpha 			;

			//	HeavyEnergyLab to HeavyEnergyCM
			
			// Initial Lab to CM Momentum
		 	G4double InitialMomentum  = sqrt(EnergyHeavy*EnergyHeavy - Ma2)		;
		 	G4double InitialMomentumX = sin(ThetaHeavy)							;
		 	G4double InitialMomentumY =	sin(ThetaHeavy)							;
		 	G4double InitialMomentumZ =	cos(ThetaHeavy)							;
		 	
		 	// Beta and Gamma CM to Lab									
		 	G4double betaX = (DaughterMomentumX*p - InitialMomentumX*InitialMomentum)/E	;
		 	G4double betaY = (DaughterMomentumY*p - InitialMomentumY*InitialMomentum)/E	;
		 	G4double betaZ = (DaughterMomentumZ*p - InitialMomentumZ*InitialMomentum)/E	;
		 	G4double beta  = sqrt (betaX*betaX + betaY*betaY  + betaZ*betaZ	)			;
		 	G4double beta2 = beta*beta													;
		 	G4double gamma = 1 / ( sqrt(1 - beta2 ) )									;
		 	// Calculate everything for heavy particule
		 	
		 	G4double NewEnergy =
		 	  gamma*E
		 	- betaX*gamma*DaughterMomentumX*p
		 	- betaY*gamma*DaughterMomentumY*p
		 	- betaZ*gamma*DaughterMomentumZ*p;
	
			double HexaEnergy = sqrt( ( (M2 + Ma2)/(2*M10He) - (HeavyEnergy) ) * (2*M10He) );
			
			return(6*Mn-HexaEnergy) ;*/
	}



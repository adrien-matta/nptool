// C++ headers
#include <sstream>
#include <cmath>

//G4 Geometry object
#include "G4Trd.hh"
#include "G4Box.hh"
#include "G4Trap.hh"

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"
#include "G4SubtractionSolid.hh"

// NPTool header
#include "ThinSi.hh"
#include "GeneralScorers.hh"
#include "Must2Scorers.hh"
#include "MUST2Array.hh"
#include "RootOutput.h"

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace THINSI
{
   // Energy and time Resolution
   const G4double ResoTime    = 0      ;
   const G4double ResoEnergy  = 0.022 ;// = 52keV of Resolution   //   Unit is MeV/2.35

   // Geometry
   const G4double DetectorSize   = 70*mm           ;
   const G4double SiliconThickness  = 20*micrometer      ;
   const G4double SiliconSize    = 50*mm           ;
   const G4double AluThickness      = 0.4*micrometer  ;
   const G4int  NumberOfStrip    = 32           ;

   const G4double AluStripFront_PosZ = -0.5*SiliconThickness - 0.5*AluThickness ;
   const G4double Si_PosZ        = 0                                 ;
   const G4double AluStripBack_PosZ  = 0.5*SiliconThickness + 0.5*AluThickness  ;

   const G4double Si_PosX_Shift = 4*mm ;
   const G4double Si_PosY_Shift = 2*mm ;
}

using namespace THINSI ;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ThinSi Specific Method
ThinSi::ThinSi()
{

}

ThinSi::~ThinSi()
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ThinSi::AddTelescope(G4ThreeVector TL         ,
      G4ThreeVector BL        ,
      G4ThreeVector BR        ,
      G4ThreeVector CT        ,
      bool       RightOrLeft)
{
   m_DefinitionType.push_back(true) ;
   m_RightOrLeft.push_back(RightOrLeft)   ;

   m_TL.push_back(TL)               ;
   m_BL.push_back(BL)               ;
   m_BR.push_back(BR)               ;
   m_CT.push_back(CT)               ;

   m_R.push_back(0)              ;
   m_Theta.push_back(0)          ;
   m_Phi.push_back(0)               ;
   m_beta_u.push_back(0)            ;
   m_beta_v.push_back(0)            ;
   m_beta_w.push_back(0)            ;
}

void ThinSi::AddTelescope(G4double R      ,
      G4double Theta ,
      G4double Phi   ,
      G4double beta_u   ,
      G4double beta_v   ,
      G4double beta_w   ,
      bool   RightOrLeft)
{
   G4ThreeVector empty = G4ThreeVector(0, 0, 0)   ;

   m_DefinitionType.push_back(false)   ;
   m_RightOrLeft.push_back(RightOrLeft);

   m_R.push_back(R)              ;
   m_Theta.push_back(Theta)         ;
   m_Phi.push_back(Phi)          ;
   m_beta_u.push_back(beta_u)       ;
   m_beta_v.push_back(beta_v)       ;
   m_beta_w.push_back(beta_w)       ;

   m_TL.push_back(empty)            ;
   m_BL.push_back(empty)            ;
   m_BR.push_back(empty)            ;
   m_CT.push_back(empty)            ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ThinSi::VolumeMaker(G4int            DetNumber      ,
      G4ThreeVector     Det_pos        ,
      G4RotationMatrix*    Det_rot        ,
      G4LogicalVolume*  world       ,
      bool           RightOrLeft)
{
   G4double NbrTelescopes = DetNumber           ;
   G4String DetectorNumber                   ;
   std::ostringstream Number                 ;
   Number << NbrTelescopes                   ;
   DetectorNumber = Number.str()             ;

////////////////////////////////////////////////////////////////
/////////////////Material Definition ///////////////////////////
////////////////////////////////////////////////////////////////
   G4Element* N   = new G4Element("Nitrogen" , "N"  , 7  , 14.01  * g / mole);
   G4Element* O   = new G4Element("Oxigen"   , "O"  , 8  , 16.00  * g / mole);

   G4double a, z, density;
   // Si
   a = 28.0855 * g / mole;
   density = 2.321 * g / cm3;
   G4Material* Silicon = new G4Material("Si", z = 14., a, density);

   // Al
   density = 2.702 * g / cm3;
   a = 26.98 * g / mole;
   G4Material* Al = new G4Material("Al", z = 13., a, density);

   //  Vacuum
   density = 0.000000001 * mg / cm3;
   G4Material* Vacuum = new G4Material("Vacuum", density, 2);
   Vacuum->AddElement(N, .7);
   Vacuum->AddElement(O, .3);

////////////////////////////////////////////////////////////////
/////////General Geometry Parameter Definition /////////////////
////////////////////////////////////////////////////////////////

// Little trick to avoid warning in compilation: Use a PVPlacement "buffer".
// If don't you will have a Warning unused variable 'myPVP'
   G4PVPlacement* PVPBuffer ;

   G4double Si_PosX = 0 ;
   G4double Si_PosY = 0 ;
   if (RightOrLeft)         Si_PosX = -Si_PosX_Shift   ;
   else              Si_PosX =  Si_PosX_Shift   ;

   if (Det_pos.y() > 0)    Si_PosY =  -Si_PosY_Shift  ;
   else              Si_PosY =   Si_PosY_Shift  ;


   G4ThreeVector Shift = G4ThreeVector(Si_PosX , Si_PosY , 0) ;
   Det_pos = Det_pos + Shift ;
/////// Starting Volume Definition ///////
   G4String Name = "ThinSi" + DetectorNumber;

   G4Box* solidThinSi = new G4Box(Name, 0.5*DetectorSize, 0.5*DetectorSize, 0.5*3*mm);

   G4LogicalVolume* logicThinSi =
      new G4LogicalVolume(solidThinSi, Vacuum, Name, 0, 0);

   PVPBuffer =
      new G4PVPlacement(G4Transform3D(*Det_rot, Det_pos)  ,
            logicThinSi                ,
            Name                    ,
            world                   ,
            false                   ,
            0);

   // Frame is made of 4 thick box (2 Horizontal and 2 Vertical)
   G4Box* solidFrameHorizontal = new G4Box(Name + "Frame", 0.5*SiliconSize, 0.5*(DetectorSize - SiliconSize) / 2, 0.5*3*mm)   ;
   G4Box* solidFrameVertical  = new G4Box(Name + "Frame", 0.5*(DetectorSize - SiliconSize) / 2, 0.5*DetectorSize, 0.5*3*mm)   ;

   G4LogicalVolume* logicFrameHorizontal =
      new G4LogicalVolume(solidFrameHorizontal, Al, Name, 0, 0);

   G4LogicalVolume* logicFrameVertical =
      new G4LogicalVolume(solidFrameVertical, Al, Name, 0, 0);

   G4ThreeVector FrameTopPosition      = G4ThreeVector(0 ,  0.5 * SiliconSize + 0.5 * (DetectorSize - SiliconSize) / 2 , 0) ;
   G4ThreeVector FrameBottomPosition   = G4ThreeVector(0 , -0.5 * SiliconSize - 0.5 * (DetectorSize - SiliconSize) / 2 , 0) ;
   G4ThreeVector FrameLeftPosition  = G4ThreeVector(0.5 * SiliconSize + 0.5 * (DetectorSize - SiliconSize) / 2 , 0 , 0) ;
   G4ThreeVector FrameRightPosition = G4ThreeVector(-0.5 * SiliconSize - 0.5 * (DetectorSize - SiliconSize) / 2 , 0 , 0) ;


   PVPBuffer =
      new G4PVPlacement(0                 ,
            FrameTopPosition     ,
            logicFrameHorizontal ,
            Name + "Frame"         ,
            logicThinSi          ,
            false             ,
            0);

   PVPBuffer =
      new G4PVPlacement(0                 ,
            FrameBottomPosition     ,
            logicFrameHorizontal ,
            Name + "Frame"         ,
            logicThinSi          ,
            false             ,
            0);

   PVPBuffer =
      new G4PVPlacement(0                 ,
            FrameLeftPosition    ,
            logicFrameVertical      ,
            Name + "Frame"         ,
            logicThinSi          ,
            false             ,
            0);

   PVPBuffer =
      new G4PVPlacement(0                 ,
            FrameRightPosition      ,
            logicFrameVertical      ,
            Name + "Frame"         ,
            logicThinSi          ,
            false             ,
            0);


   G4ThreeVector posAluFront  = G4ThreeVector(0 ,  0  ,  AluStripFront_PosZ)  ;
   G4ThreeVector posSi        = G4ThreeVector(0 ,  0  ,  0)  ;
   G4ThreeVector posAluBack   = G4ThreeVector(0 ,  0  ,  AluStripBack_PosZ)  ;

   G4Box*   solidAlu  =
      new G4Box("ThinSiAlu", 0.5*SiliconSize, 0.5*SiliconSize, 0.5*AluThickness) ;

   G4LogicalVolume* logicAlu  =
      new G4LogicalVolume(solidAlu, Al, "logicAlu", 0, 0, 0)    ;

   PVPBuffer =
      new G4PVPlacement(0  ,  posAluFront ,  logicAlu ,  Name + "AluFront"   ,  logicThinSi ,  true, 0)  ;

   PVPBuffer =
      new G4PVPlacement(0  ,  posAluBack  ,  logicAlu ,  Name + "AluBack"    ,  logicThinSi ,  true, 0)  ;


   G4Box*   solidSi  =
      new G4Box("ThinSi", 0.5*SiliconSize, 0.5*SiliconSize, 0.5*SiliconThickness)   ;

   G4LogicalVolume* logicSi  =
      new G4LogicalVolume(solidSi, Silicon, "logicSi", 0, 0, 0)           ;

   PVPBuffer =
      new G4PVPlacement(0, posSi, logicSi, Name + "Si", logicThinSi, true, 0)   ;

   //Set Add. Silicon strip sensible
   //instantiate a new scorer
   G4MultiFunctionalDetector* ThinSiScorer = new G4MultiFunctionalDetector("ThinSi" + DetectorNumber);
   //attach it to the Silicon plate
   logicSi ->SetSensitiveDetector(ThinSiScorer);
   //and declare it to the SDManager
   G4SDManager::GetSDMpointer()->AddNewDetector(ThinSiScorer);

   //instantiate primitive scorer
   G4VPrimitiveScorer* ThinSiEnergy       ;

   //create primitive scorer
   ThinSiEnergy = new MUST2::PSStripE("StripEnergy", 0)    ;
   //and register it to the multifunctionnal detector
   ThinSiScorer->RegisterPrimitive(ThinSiEnergy)      ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class


// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void ThinSi::ReadConfiguration(string Path)
{
   ifstream ConfigFile           ;
   ConfigFile.open(Path.c_str()) ;
   string LineBuffer          ;
   string DataBuffer          ;

   G4double TLX , BLX , BRX , CTX , TLY , BLY , BRY , CTY , TLZ , BLZ , BRZ , CTZ   ;
   G4ThreeVector TL , BL , BR , CT                                      ;
   G4double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0                     ;
   bool RightOrLeft = false ;
   bool check_A = false   ;
   bool check_B = false ;
   bool check_C = false   ;
   bool check_D = false ;

   bool check_Theta = false   ;
   bool check_Phi  = false  ;
   bool check_R     = false   ;
   bool check_beta = false  ;
   bool check_side = false  ;
   bool ReadingStatus = false ;

 while (!ConfigFile.eof()) 
 	{
      
	      	getline(ConfigFile, LineBuffer);

			//	If line is a Start Up MUST2 bloc, Reading toggle to true      
	      	if (LineBuffer.compare(0, 6, "ThinSi") == 0) 
		      	{
		        	 G4cout << "///" << G4endl           ;
		       		  G4cout << "Detector found: " << G4endl   ;        
		        	 ReadingStatus = true ;
		        	
			   	}
			
			//	Else don't toggle to Reading Block Status
			else ReadingStatus = false ;
			
			//	Reading Block
			while(ReadingStatus)
				{
						// Pickup Next Word 
					ConfigFile >> DataBuffer ;

					//	Comment Line 
					if (DataBuffer.compare(0, 1, "%") == 0) {/*do nothing */;}

						//	Finding another telescope (safety), toggle out
					else if (DataBuffer.compare(0, 6, "ThinSi") == 0) {
						cout << "WARNING: Another Telescope is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
						ReadingStatus = false ;
					}
			
						 //Position method
			         else if (DataBuffer.compare(0, 3, "A=") == 0) {
			            check_A = true;
			            ConfigFile >> DataBuffer ;
			            TLX = atof(DataBuffer.c_str()) ;
			            TLX = TLX * mm;
			            ConfigFile >> DataBuffer ;
			            TLY = atof(DataBuffer.c_str()) ;
			            TLY = TLY * mm;
			            ConfigFile >> DataBuffer ;
			            TLZ = atof(DataBuffer.c_str()) ;
			            TLZ = TLZ * mm;

			            TL = G4ThreeVector(TLX, TLY, TLZ);
			            cout << "Top Left position : (" << TLX << ";" << TLY << ";" << TLZ << ")" << endl;
			         }
					
					else if (DataBuffer.compare(0, 3, "B=") == 0) {
						check_B = true;
						ConfigFile >> DataBuffer ;
						BLX = atof(DataBuffer.c_str()) ;
						BLX = BLX * mm;
						ConfigFile >> DataBuffer ;
						BLY = atof(DataBuffer.c_str()) ;
						BLY = BLY * mm;
						ConfigFile >> DataBuffer ;
						BLZ = atof(DataBuffer.c_str()) ;
						BLZ = BLZ * mm;

						BL = G4ThreeVector(BLX, BLY, BLZ);
						cout << "Top Right position : (" << BLX << ";" << BLY << ";" << BLZ << ")" << endl;
					}

					else if (DataBuffer.compare(0, 3, "C=") == 0) {
						check_C = true;
						ConfigFile >> DataBuffer ;
						BRX = atof(DataBuffer.c_str()) ;
						BRX = BRX * mm;
						ConfigFile >> DataBuffer ;
						BRY = atof(DataBuffer.c_str()) ;
						BRY = BRY * mm;
						ConfigFile >> DataBuffer ;
						BRZ = atof(DataBuffer.c_str()) ;
						BRZ = BRZ * mm;

						BR = G4ThreeVector(BRX, BRY, BRZ);
						cout << "Bottom Right position : (" << BRX << ";" << BRY << ";" << BRZ << ")" << endl;
					}

					else if (DataBuffer.compare(0, 3, "D=") == 0) {
						check_D = true;
						ConfigFile >> DataBuffer ;
						CTX = atof(DataBuffer.c_str()) ;
						CTX = CTX * mm;
						ConfigFile >> DataBuffer ;
						CTY = atof(DataBuffer.c_str()) ;
						CTY = CTY * mm;
						ConfigFile >> DataBuffer ;
						CTZ = atof(DataBuffer.c_str()) ;
						CTZ = CTZ * mm;

						CT = G4ThreeVector(CTX, CTY, CTZ);
						G4cout << "Center position : (" << CTX << ";" << CTY << ";" << CTZ << ")" << G4endl << G4endl;
					}

										
					//Angle method
					else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
						check_Theta = true;
						ConfigFile >> DataBuffer ;
						Theta = atof(DataBuffer.c_str()) ;
						Theta = Theta * deg;
						cout << "Theta:  " << Theta / deg << endl;
					}

					else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
						check_Phi = true;
						ConfigFile >> DataBuffer ;
						Phi = atof(DataBuffer.c_str()) ;
						Phi = Phi * deg;
						cout << "Phi:  " << Phi / deg << endl;
					}

					else if (DataBuffer.compare(0, 2, "R=") == 0) {
						check_R = true;
						ConfigFile >> DataBuffer ;
						R = atof(DataBuffer.c_str()) ;
						R = R * mm;
						cout << "R:  " << R*mm << endl;
					}


					else if (DataBuffer.compare(0, 5, "BETA=") == 0) {
						check_beta = true;
						ConfigFile >> DataBuffer ;
						beta_u = atof(DataBuffer.c_str()) ;
						beta_u = beta_u * deg   ;
						ConfigFile >> DataBuffer ;
						beta_v = atof(DataBuffer.c_str()) ;
						beta_v = beta_v * deg   ;
						ConfigFile >> DataBuffer ;
						beta_w = atof(DataBuffer.c_str()) ;
						beta_w = beta_w * deg   ;
						G4cout << "Beta:  " << beta_u / deg <<  " " << beta_v / deg << " " << beta_w / deg << G4endl       ;
					}

					else if (DataBuffer.compare(0, 5, "SIDE=") == 0) {
							check_side = true  ;
							ConfigFile >> DataBuffer ;
							if (DataBuffer == "right") RightOrLeft = true  ;
							else               RightOrLeft = false  ;
							G4cout << "Side:  " << DataBuffer << G4endl << G4endl;
						}
			      
			         	///////////////////////////////////////////////////
						//	If no Detector Token and no comment, toggle out
			         else 
			         	{ReadingStatus = false; G4cout << "Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;}
			         
			         	/////////////////////////////////////////////////
			         	//	If All necessary information there, toggle out
			         
			         if ( (check_A && check_B && check_C && check_D) || (check_Theta && check_Phi && check_R && check_beta && check_side) ) 
			         	{ 
					         	ReadingStatus = false; 
					         	
					         	///Add The previously define telescope
			         			//With position method
					         	 if ((check_A && check_B && check_C && check_D) || !(check_Theta && check_Phi && check_R)) {
							            	  AddTelescope(TL      ,
							                  BL    ,
							                  BR    ,
							                  CT    ,
							                  RightOrLeft);
							         }

						         //with angle method
						         else if ((check_Theta && check_Phi && check_R) || !(check_A && check_B && check_C && check_D)) {
							         		  AddTelescope(R       ,
							                  Theta    ,
							                  Phi   ,
							                  beta_u   ,
							                  beta_v   ,
							                  beta_w   ,
							                  RightOrLeft);
							         }
							         
							        //	Reinitialisation of Check Boolean 
							        
									RightOrLeft = false ;
									check_A = false   ;
									check_B = false ;
									check_C = false   ;
									check_D = false ;

									check_Theta = false   ;
									check_Phi  = false  ;
									check_R     = false   ;
									check_beta = false  ;
									check_side = false  ;
									ReadingStatus = false ;
								         
			         	}
			         	
				}
	}

}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void ThinSi::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* Det_rot     = NULL               ;
   G4ThreeVector     Det_pos     = G4ThreeVector(0, 0, 0)  ;
   G4ThreeVector     Det_u    = G4ThreeVector(0, 0, 0)  ;
   G4ThreeVector     Det_v    = G4ThreeVector(0, 0, 0)  ;
   G4ThreeVector     Det_w    = G4ThreeVector(0, 0, 0)  ;

   G4int NumberOfDetector = m_DefinitionType.size()      ;

   for (G4int i = 0 ; i < NumberOfDetector ; i++) {
      // By Point
      if (m_DefinitionType[i]) {
         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing outside
         Det_u = m_TL[i] - m_BL[i]     ;
         Det_u = Det_u.unit()       ;
         Det_v = m_BR[i] - m_BL[i]     ;
         Det_v = Det_v.unit()       ;
         Det_w = Det_u.cross(Det_v)    ;
         Det_w = Det_w.unit()            ;
         // Passage Matrix from Lab Referential to Telescope Referential
         // MUST2
         Det_rot = new G4RotationMatrix(Det_u, Det_v, Det_w);
         // translation to place Telescope
         Det_pos = m_CT[i] ;
      }

      // By Angle
      else {
         G4double Theta = m_Theta[i]   ;
         G4double Phi   = m_Phi[i]  ;
         //This part because if Phi and Theta = 0 equation are false
         if (Theta == 0)      {
            Theta   = 0.0001             ;
         }
         if (Theta == 2*cos(0))   {
            Theta   = 2 * acos(0) - 0.00001  ;
         }
         if (Phi   == 0)         {
            Phi     = 0.0001             ;
         }

         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing outside
         // Phi is angle between X axis and projection in (X,Y) plan
         // Theta is angle between  position vector and z axis
         G4double wX = m_R[i] * sin(Theta / rad) * cos(Phi / rad)   ;
         G4double wY = m_R[i] * sin(Theta / rad) * sin(Phi / rad)   ;
         G4double wZ = m_R[i] * cos(Theta / rad)             ;

         Det_w = G4ThreeVector(wX, wY, wZ)                 ;
         G4ThreeVector CT = Det_w                        ;
         Det_w = Det_w.unit()                         ;

         G4ThreeVector Y = G4ThreeVector(0 , 1 , 0)         ;

         Det_u = Det_w.cross(Y)     ;
         Det_v = Det_w.cross(Det_u) ;

         Det_v = Det_v.unit();
         Det_u = Det_u.unit();
         // Passage Matrix from Lab Referential to Telescope Referential
         // MUST2
         Det_rot = new G4RotationMatrix(Det_u, Det_v, Det_w);
         // Telescope is rotate of Beta angle around Det_v axis.
         Det_rot->rotate(m_beta_u[i], Det_u);
         Det_rot->rotate(m_beta_v[i], Det_v);
         Det_rot->rotate(m_beta_w[i], Det_w);
         // translation to place Telescope
         Det_pos = CT ;
      }



      VolumeMaker(i + 1 , Det_pos , Det_rot , world, m_RightOrLeft[i]);
   }

   delete Det_rot ;
}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void ThinSi::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("ThinSiEnergy", &m_Energy, "ThinSiEnergy/D") ;
}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void ThinSi::ReadSensitive(const G4Event* event)
{
   G4String DetectorNumber    ;
   bool  checkSi  = false  ;

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////// Used to Read Event Map of detector //////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

// Si
   std::map<G4int, G4double*>::iterator Energy_itr     ;
   G4THitsMap<G4double>* EnergyHitMap              ;

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
   G4int HitNumber = 0;
   G4int NumberOfDetector = m_DefinitionType.size()  ;
   for (G4int i = 0 ; i < NumberOfDetector ; i++) {
      G4int k = i + 1;
      ostringstream buffer;
      buffer << k;
      DetectorNumber = buffer.str();

      // Read the Scorer associate to the Silicon Strip
      //Energy
      G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ThinSi" + DetectorNumber + "/StripEnergy")   ;
      EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))                 ;
      Energy_itr = EnergyHitMap->GetMap()->begin()                                                       ;

      // Loop on Strip energy
      for (G4int l = 0 ; l < EnergyHitMap->entries() ; l++) {
         //G4int ETrackID  =   Energy_itr->first      ;
         G4double E     = *(Energy_itr->second)    ;

         if (E > 0) {
            checkSi = true   ;
            m_Energy = RandGauss::shoot(E, ResoEnergy);
         }

         Energy_itr++;
         if (checkSi) {
            HitNumber++ ;
         }
      }
      // clear map for next event
      EnergyHitMap   ->clear()   ;
   }
}

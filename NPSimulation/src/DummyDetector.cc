/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : October 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a simple Dummy Detector :                            *
 *	a simple cylinder of predifined material. user can choose to place it    *
 *  where he want using spherical coordinate and choose between two naterial *
 *	                                                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  This detector respect all the NPTool convention in naming volume,        * 
 *  reading scorers and file. Use it as a basis for your own detector        *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
//G4 Geometry object
#include "G4Tubs.hh"

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

// NPTool header
#include "DummyDetector.hh"
#include "GeneralScorers.hh"
#include "RootOutput.h"
using namespace GENERALSCORERS ;
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace DUMMYDETECTOR
{
   // Energy and time Resolution
   const G4double ResoTime    = 4.2      	;// = 10ns of Resolution   //   Unit is MeV/2.35
   const G4double ResoEnergy  = 5.0   		;// Resolution in %

}

using namespace DUMMYDETECTOR ;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// DUMMYDetector Specific Method
DUMMYDetector::DUMMYDetector()
{
	InitializeMaterial();
	m_Event = new TDUMMYDetectorData() ;
}

DUMMYDetector::~DUMMYDetector()
{
	delete m_MaterialDUMMYDetector_material1		; 
	delete m_MaterialDUMMYDetector_material2	;
	delete m_DUMMYDetectorScorer						;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DUMMYDetector::AddDUMMYDetector(	G4double  R        					,
				        					G4double  Theta    					,
				         			 		G4double  Phi         			,
				         			 		G4double	DUMMYDetectorThickness	,
				         			 		G4double	DUMMYDetectorRadius			,
				         			 		G4String 	Scintillator			)
{

  m_R.push_back(R)              									;
  m_Theta.push_back(Theta)        								;
  m_Phi.push_back(Phi)          									;
  m_DUMMYDetectorThickness.push_back(DUMMYDetectorThickness)	;
 	m_DUMMYDetectorRadius.push_back(DUMMYDetectorRadius)				;
 	m_Scintillator.push_back(Scintillator)					;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class


// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void DUMMYDetector::ReadConfiguration(string Path)
{
	ifstream ConfigFile           ;
	ConfigFile.open(Path.c_str()) ;
	string LineBuffer          ;
	string DataBuffer          ;

	G4double Theta = 0 , Phi = 0 , R = 0 , Thickness = 0 , Radius = 0 ;
	G4String Scintillator ;

	bool check_Theta = false   ;
	bool check_Phi  = false  ;
	bool check_R     = false   ;
	bool check_Thickness = false  		;
	bool check_Radius = false  			;\
	bool check_Scintillator = false		;
	bool ReadingStatus = false ;
	

 while (!ConfigFile.eof()) 
 	{
      
	      	getline(ConfigFile, LineBuffer);

			//	If line is a Start Up DUMMYDetector bloc, Reading toggle to true      
	      	if (LineBuffer.compare(0, 13, "DUMMYDetector") == 0) 
		      	{
		        	 G4cout << "///" << G4endl           ;
		       		  G4cout << "Dummy Module found: " << G4endl   ;        
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
					if (DataBuffer.compare(0, 1, "%") == 0) {	ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

						//	Finding another telescope (safety), toggle out
					else if (DataBuffer.compare(0, 13, "DUMMYDetector") == 0) {
						cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
						ReadingStatus = false ;
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
						cout << "R:  " << R/mm << endl;
					}
					
					else if (DataBuffer.compare(0, 7, "Radius=") == 0) {
						check_Radius = true;
						ConfigFile >> DataBuffer ;
						Radius = atof(DataBuffer.c_str()) ;
						Radius = Radius * mm;
						cout << "DUMMYDetector Radius:  " << Radius/mm << endl;
					}
					
					else if (DataBuffer.compare(0, 10, "Thickness=") == 0) {
						check_Thickness = true;
						ConfigFile >> DataBuffer ;
						Thickness = atof(DataBuffer.c_str()) ;
						Thickness = Thickness * mm;
						cout << "DUMMYDetector Thickness:  " << Thickness/mm << endl;
					}
					
					else if (DataBuffer.compare(0, 13, "Material=") == 0) {
						check_Scintillator = true ;
						ConfigFile >> DataBuffer ;
						Scintillator = DataBuffer ;
						cout << "DUMMYDetector material type:  " << Scintillator << endl;
					}
			      
			     			///////////////////////////////////////////////////
								//	If no Detector Token and no comment, toggle out
			         else 
			         	{ReadingStatus = false; G4cout << "Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;}
			         
			         	/////////////////////////////////////////////////
			         	//	If All necessary information there, toggle out
			         
			         if ( check_Theta && check_Phi && check_R && check_Thickness && check_Radius && check_Scintillator) 
			         	{ 
		         		  AddDUMMYDetector(	R       		,
		                  				Theta    		,
		                  				Phi   			,
		                  				Thickness		,
		                  				Radius			,
		                  				Scintillator		);
					         
					        //	Reinitialisation of Check Boolean 
					        
							check_Theta = false   			;
							check_Phi  = false  			;
							check_R     = false   			;
							check_Thickness = false  		;
							check_Radius = false  			;
							check_Scintillator = false 		;
							ReadingStatus = false 			;	
							cout << "///"<< endl ;	         
			         	}
			         	
				}
	}

}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void DUMMYDetector::ConstructDetector(G4LogicalVolume* world)
{
   	G4ThreeVector Det_pos = G4ThreeVector(0, 0, 0)  ;
	
   for (unsigned short i = 0 ; i < m_R.size() ; i++) 
   	{
         G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] )   ;
         G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] )   ;
         G4double wZ = m_R[i] * cos(m_Theta[i] )             		;

         Det_pos = G4ThreeVector(wX, wY, wZ)                 ;
//         G4LogicalVolume* logicDUMMYDetector = NULL ;
			
		VolumeMaker(Det_pos , i+1, world) ;
    }

}

void DUMMYDetector::VolumeMaker(G4ThreeVector Det_pos, int DetNumber, G4LogicalVolume* world)
	{
		////////////////////////////////////////////////////////////////
		////////////// Starting Volume Definition //////////////////////
		////////////////////////////////////////////////////////////////		
		G4PVPlacement* PVPBuffer ;
		
		// Name of the module
   		std::ostringstream DetectorNumber         			;
   		DetectorNumber << DetNumber                 	  	;
		G4String Name = "DUMMYDetector" + DetectorNumber.str()	;
		
		int i = DetNumber-1;

		G4Material* DUMMYDetectorMaterial ;
		
			 if(m_Scintillator[i] == "material1"    ) DUMMYDetectorMaterial = m_MaterialDUMMYDetector_material1 	;
		else if(m_Scintillator[i] == "material2"  ) DUMMYDetectorMaterial = m_MaterialDUMMYDetector_material2 	;
		else {	
				G4cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl ;
				G4cout << "WARNING: Material Not found, default material set : material1" << endl ; 
				G4cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl ;
				DUMMYDetectorMaterial = m_MaterialDUMMYDetector_material1;
			}
		
		
		// Definition of the volume containing the sensitive detector
		if(m_DUMMYDetectorThickness[i]>0 && m_DUMMYDetectorRadius[i]>0)
			{ 
				G4Tubs* solidDUMMYDetector = new G4Tubs(	Name					, 
			                            								0						,
			                            								m_DUMMYDetectorRadius[i]		,
			                            								m_DUMMYDetectorThickness[i]/2	,
			                            								0*deg					, 
			                            								360*deg					);
		                            		
				G4LogicalVolume* logicDUMMYDetector = new G4LogicalVolume(solidDUMMYDetector, DUMMYDetectorMaterial, Name+ "_Scintillator", 0, 0, 0);
				logicDUMMYDetector->SetSensitiveDetector(m_DUMMYDetectorScorer);
				
				G4VisAttributes* PlastVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9)) ;
   				logicDUMMYDetector->SetVisAttributes(PlastVisAtt) ;
 				
 				
			  
				PVPBuffer = new G4PVPlacement(	0				,
												Det_pos			,
		                                     	logicDUMMYDetector    ,
		                                     	Name  + "_Scintillator"          ,
		                                     	world           ,
		                                     	false           ,
		                                     	0				);	
		                                     	
		       
		                                     	
			}
	}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void DUMMYDetector::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("DUMMYDetector", "TDUMMYDetectorData", &m_Event) ;
}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void DUMMYDetector::ReadSensitive(const G4Event* event)
{
   G4String DetectorNumber 	;
   m_Event->Clear()			;
   
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////// Used to Read Event Map of detector //////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
  
	std::map<G4int, G4int*>::iterator DetectorNumber_itr    ;
	std::map<G4int, G4double*>::iterator Energy_itr     	;
	std::map<G4int, G4double*>::iterator Time_itr    		;
	
	G4THitsMap<G4int>*	  DetectorNumberHitMap				;      
	G4THitsMap<G4double>* EnergyHitMap              		;
	G4THitsMap<G4double>* TimeHitMap             			;
	
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

    // Read the Scorer associate to the Silicon Strip
    
	//Detector Number
	G4int StripDetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("DUMMYDetectorScorer/DUMMYDetectorNumber")  	;
	DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripDetCollectionID))       	;
	DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin()                                               ;

	//Energy
	G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("DUMMYDetectorScorer/Energy")   	;
	EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))           ;
	Energy_itr = EnergyHitMap->GetMap()->begin()                                                          		;

	//Time of Flight
	G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("DUMMYDetectorScorer/Time")    		;
	TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))               ;
	Time_itr = TimeHitMap->GetMap()->begin()                                                              		;

  	G4int sizeN = DetectorNumberHitMap->entries() 	;
    G4int sizeE = EnergyHitMap->entries() 			;
    G4int sizeT = TimeHitMap->entries() 			;

    // Loop on DUMMYDetector Number
    for (G4int l = 0 ; l < sizeN ; l++) {
        G4int N     =      *(DetectorNumber_itr->second)    ;
        G4int NTrackID  =   DetectorNumber_itr->first - N  ;
        
      
        if (N > 0) {
        
	        m_Event->SetDUMMYDetectorNumber(N) ;

			//  Energy
	        Energy_itr = EnergyHitMap->GetMap()->begin();
	        for (G4int h = 0 ; h < sizeE ; h++) {
	            G4int ETrackID  =   Energy_itr->first  - N      ;
	            G4double E     = *(Energy_itr->second)      	;

	            if (ETrackID == NTrackID) {
	                m_Event->SetEnergy(RandGauss::shoot(E, E*ResoEnergy/100./2.35))    ;
	            }
	            
	            Energy_itr++;
	        	}


	        //  Time
	        Time_itr = TimeHitMap->GetMap()->begin();
	        for (G4int h = 0 ; h < sizeT ; h++) {
	            G4int TTrackID  =   Time_itr->first   - N    ;
	            G4double T     = *(Time_itr->second)      ;

	            if (TTrackID == NTrackID) {
	                m_Event->SetTime(RandGauss::shoot(T, ResoTime)) ;
	            }
	            
	            Time_itr++;
	        }

        }

        DetectorNumber_itr++;
    }
    
    // clear map for next event
    TimeHitMap				->clear()	;    
    DetectorNumberHitMap    ->clear()	;
    EnergyHitMap   			->clear() 	; 
   
}

////////////////////////////////////////////////////////////////
void DUMMYDetector::InitializeMaterial()
	{
	
		////////////////////////////////////////////////////////////////
		/////////////////Element  Definition ///////////////////////////
		////////////////////////////////////////////////////////////////
		   G4String symbol               						;
		   G4double density = 0. , a = 0, z = 0   				;
		   G4int ncomponents = 0, natoms = 0, fractionmass = 0	;

			// for DUMMYDetector
		   	G4Element* H   = new G4Element("Hydrogen" 	, symbol = "H"  	, z = 1  , a = 1.01   * g / mole);
		   	G4Element* C   = new G4Element("Carbon"   	, symbol = "C"  	, z = 6  , a = 12.011 * g / mole);
		 		G4Element* Pb  = new G4Element("Lead"   	, symbol = "Pb"  	, z = 82 , a = 207.2  * g / mole);
		////////////////////////////////////////////////////////////////
		/////////////////Material Definition ///////////////////////////
		////////////////////////////////////////////////////////////////


		   // DUMMYDetector BC-400
		   density = 1.032 * g / cm3;
		   m_MaterialDUMMYDetector_material1 = new G4Material("DUMMYDetector_material1", density, ncomponents = 2);
		   m_MaterialDUMMYDetector_material1->AddElement(H , natoms = 10);
		   m_MaterialDUMMYDetector_material1->AddElement(C  , natoms = 9);
		   
		   // DUMMYDetector BC-452 Pb 2%
		   density = 1.05 * g / cm3;
		   m_MaterialDUMMYDetector_material2 = new G4Material("DUMMYDetector_material2", density, ncomponents = 3);
		   m_MaterialDUMMYDetector_material2->AddElement(H  , natoms = 10);
		   m_MaterialDUMMYDetector_material2->AddElement(C  , natoms = 9);
		   m_MaterialDUMMYDetector_material2->AddElement(Pb , fractionmass=2*perCent);

	}

////////////////////////////////////////////////////////////////	
void DUMMYDetector::InitializeScorers() 
	{ 
		m_DUMMYDetectorScorer = new G4MultiFunctionalDetector("DUMMYDetectorScorer") ;
		G4SDManager::GetSDMpointer()->AddNewDetector(m_DUMMYDetectorScorer);
		
		G4VPrimitiveScorer* DetNbr = new PSDetectorNumber("DUMMYDetectorNumber","DUMMYDetector", 0) ;
		G4VPrimitiveScorer* Energy = new PSEnergy("Energy","DUMMYDetector", 0)             		;
		G4VPrimitiveScorer* Time   = new PSTOF("Time","DUMMYDetector", 0)             				;
		 
		//and register it to the multifunctionnal detector
		m_DUMMYDetectorScorer->RegisterPrimitive(DetNbr)             				;
		m_DUMMYDetectorScorer->RegisterPrimitive(Energy)             				;
		m_DUMMYDetectorScorer->RegisterPrimitive(Time)             					;		
		
		
	}
////////////////////////////////////////////////////////////////

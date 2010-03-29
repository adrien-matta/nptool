#ifndef GeneralScorers_h
#define GeneralScorers_h 1
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class old some of the General Scorer, shared by different detector.
    Thos scorer could be a could basis for your own scorer                   *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Those Scorer use TrackID as map index. This way ones can rebuild energy   *
 *  deposit, time of flight or position,... particle by particle for each    *
 *  event.Because standard scorer provide by G4 don't work this way but using* 
 *  a global ID for each event you should not use those scorer with some G4  *
 *  provided ones or being very carefull doing so.                           *s
 *****************************************************************************/
#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace GENERALSCORERS
	{
			//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
			//	This Threshold is used in the above scorer. Any energy deposit under this threshold will not create an entry.
			const double TriggerThreshold = 100*keV ;

			//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
			//	The following function is used in many scorer. following the Detector Volume Nomenclature
			//	DetectorNameX_SubPart_SubPart
			//  where X stand for the detector number.

			int PickUpDetectorNumber(G4Step* aStep, std::string DetName);



			//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
			class PSDetectorNumber : public G4VPrimitiveScorer
				{

				public: // with description
				   PSDetectorNumber(G4String name, G4String VolumeName = "xxx", G4int depth = 0 );
				   virtual ~PSDetectorNumber();

				protected: // with description
				   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

				public:
				   virtual void Initialize(G4HCofThisEvent*);
				   virtual void EndOfEvent(G4HCofThisEvent*);
				   virtual void clear();
				   virtual void DrawAll();
				   virtual void PrintAll();

				private:
					 G4String m_VolumeName; 
				   G4int HCID;
				   G4THitsMap<G4int>* EvtMap;
				   
				};

			//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......			
			class PSEnergy : public G4VPrimitiveScorer
					{

						public: // with description
						   PSEnergy(G4String name, G4String VolumeName, G4int depth);
						   virtual ~PSEnergy();

						protected: // with description
						   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

						public:
						   virtual void Initialize(G4HCofThisEvent*);
						   virtual void EndOfEvent(G4HCofThisEvent*);
						   virtual void clear();
						   virtual void DrawAll();
						   virtual void PrintAll();

						private:
								G4String m_VolumeName;
						   G4int HCID;
						   G4THitsMap<G4double>* EvtMap;
					};

			//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
			class PSTOF : public G4VPrimitiveScorer
					{

						public: // with description
						   PSTOF(G4String name, G4String VolumeName, G4int depth);
						   virtual ~PSTOF();

						protected: // with description
						   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

						public:
						   virtual void Initialize(G4HCofThisEvent*);
						   virtual void EndOfEvent(G4HCofThisEvent*);
						   virtual void clear();
						   virtual void DrawAll();
						   virtual void PrintAll();

						private:
								G4String m_VolumeName;
						   	G4int HCID;
						   	G4THitsMap<G4double>* EvtMap;
					};

			//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
			class PSInteractionCoordinatesX : public G4VPrimitiveScorer
				{
				public: // with description
				   PSInteractionCoordinatesX(G4String name, G4String VolumeName, G4int depth);
				   virtual ~PSInteractionCoordinatesX();

				protected: // with description
				   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

				public:
				   virtual void Initialize(G4HCofThisEvent*);
				   virtual void EndOfEvent(G4HCofThisEvent*);
				   virtual void clear();
				   virtual void DrawAll();
				   virtual void PrintAll();

				private:
						G4String m_VolumeName;
				   G4int HCID;
				   G4THitsMap<G4double>* EvtMap;
				};


			//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
			class PSInteractionCoordinatesY : public G4VPrimitiveScorer
				{
				public: // with description
				   PSInteractionCoordinatesY(G4String name, G4String VolumeName, G4int depth);
				   virtual ~PSInteractionCoordinatesY();

				protected: // with description
				   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

				public:
				   virtual void Initialize(G4HCofThisEvent*);
				   virtual void EndOfEvent(G4HCofThisEvent*);
				   virtual void clear();
				   virtual void DrawAll();
				   virtual void PrintAll();

				private:
						G4String m_VolumeName;
				   G4int HCID;
				   G4THitsMap<G4double>* EvtMap;
				};


			//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
			class PSInteractionCoordinatesZ : public G4VPrimitiveScorer
				{
				public: // with description
				   PSInteractionCoordinatesZ(G4String name, G4String VolumeName, G4int depth);
				   virtual ~PSInteractionCoordinatesZ();

				protected: // with description
				   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

				public:
				   virtual void Initialize(G4HCofThisEvent*);
				   virtual void EndOfEvent(G4HCofThisEvent*);
				   virtual void clear();
				   virtual void DrawAll();
				   virtual void PrintAll();

				private:
						G4String m_VolumeName;
				   G4int HCID;
				   G4THitsMap<G4double>* EvtMap;
				};


			//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
			class PSInteractionCoordinatesAngleTheta : public G4VPrimitiveScorer
				{
					public: // with description
					   PSInteractionCoordinatesAngleTheta(G4String name, G4String VolumeName, G4int depth);
					   virtual ~PSInteractionCoordinatesAngleTheta();

					protected: // with description
					   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

					public:
					   virtual void Initialize(G4HCofThisEvent*);
					   virtual void EndOfEvent(G4HCofThisEvent*);
					   virtual void clear();
					   virtual void DrawAll();
					   virtual void PrintAll();

					private:
								G4String m_VolumeName;
					   G4int HCID;
					   G4THitsMap<G4double>* EvtMap;
				};


			//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
			class PSInteractionCoordinatesAnglePhi : public G4VPrimitiveScorer
				{
					public: // with description
					   PSInteractionCoordinatesAnglePhi(G4String name, G4String VolumeName, G4int depth);
					   virtual ~PSInteractionCoordinatesAnglePhi();

					protected: // with description
					   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

					public:
					   virtual void Initialize(G4HCofThisEvent*);
					   virtual void EndOfEvent(G4HCofThisEvent*);
					   virtual void clear();
					   virtual void DrawAll();
					   virtual void PrintAll();

					private:
						 G4String m_VolumeName;
					   G4int HCID;
					   G4THitsMap<G4double>* EvtMap;
				};

			

	}



#endif

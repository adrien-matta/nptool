
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:              contact address:                            *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "DetectorManager.h"

//   STL
#include <iostream>
#include <fstream>
#include <cstdlib>

//   Detector   
#include "../DetectorList.inc"
#include "GaspardTracker.h"
#include "Hyde2Tracker.h"
#include "TAnnularS1Physics.h"
#include "TCATSPhysics.h"
#include "TCharissaPhysics.h"
#include "TChateauCristalPhysics.h"
#include "TChio_anPhysics.h"
#include "TChio_digPhysics.h"
#include "TExlPhysics.h"
#include "TExogamPhysics.h"
#include "TFatimaPhysics.h"
#include "TLaBr3Physics.h"
#include "TMust2Physics.h"
#include "TParisPhysics.h"
#include "TPlasticPhysics.h"
#include "TS2Physics.h"
#include "TSSSDPhysics.h"
#include "TSharcPhysics.h"
#include "TSiLiPhysics.h"
#include "TSiResPhysics.h"
#include "TSpegPhysics.h"
#include "TTacPhysics.h"
#include "TTiaraBarrelPhysics.h"
#include "TTiaraHyballPhysics.h"
#include "TTigressPhysics.h"
#include "TTrifoilPhysics.h"
#include "TTrifoilPhysics.h"
#include "TW1Physics.h"
#include "RootInput.h"
#include "NPOptionManager.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//   Default Constructor
DetectorManager::DetectorManager(){
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
//   Default Desstructor
DetectorManager::~DetectorManager(){
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//   Read stream at ConfigFile and pick-up Token declaration of Detector
void DetectorManager::ReadConfigurationFile(string Path)   {

  // Instantiate the Calibration Manager
  // All The detector will then add to it their parameter (see AddDetector)
  CalibrationManager::getInstance(NPOptionManager::getInstance()->GetCalibrationFile());

  ////////General Reading needs////////
  string LineBuffer;
  string DataBuffer;

   //////// List of Detector ///////
   bool AnnularS1           = false; 
   bool CATS                = false;
   bool Charissa 	          = false;
   bool ChateauCristal      = false;
   bool EXL                 = false;
   bool Exogam              = false;
   bool GPDTracker          = false;
   bool HYD2Tracker         = false;
   bool IonisationChamber   = false;
   bool LaBr3		            = false;
   bool MUST2               = false;
   bool FatimaDet           = false;
   bool ParisDet            = false;
   bool S2                  = false;
   bool SPEG                = false;
   bool SSSD                = false;
   bool ScintillatorPlastic = false;
   bool Sharc               = false;
   bool SiLi		            = false;
   bool SiRes		            = false;
   bool TAC                 = false;
   bool TiaraBarrel         = false;
   bool TiaraHyball         = false;
   bool Trifoil             = false;
   bool Tigress             = false;
   bool W1                  = false;
   ////////////////////////////////
   bool GeneralTarget       = false;


//////////////////////////////////////////////////////////////////////////////////////////
  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());

  if (ConfigFile.is_open()) {
    cout << endl << "/////////// Detector geometry ///////////" << endl;
    cout << "Configuration file " << Path << " loading " << endl;
  }

  while (!ConfigFile.eof()) {
    // Pick-up next line
    getline(ConfigFile, LineBuffer);
    //Search for comment Symbol: %
    if (LineBuffer.compare(0, 1, "%") == 0) {   /*Do  Nothing*/ ;}

    ////////////////////////////////////////////
    //////  Search for Annular S1 Array   //////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 10, "AnnularS1") == 0 && AnnularS1 == false) {
#ifdef INC_ANNULARS1
      AnnularS1 = true;
      cout << "//////// Annular S1 Array ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TAnnularS1Physics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("AnnularS1", myDetector);
#endif
    }
	  
    ////////////////////////////////////////////
    ////////   Search for CATS Array    ////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 9, "CATSArray") == 0 && CATS == false) {
#ifdef INC_CATS
      CATS = true;
      cout << "//////// CATS Array ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TCATSPhysics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("CATS", myDetector);
#endif
    }

	  ////////////////////////////////////////////
	  ///////  Search for CHARISSA Array    //////
	  ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 13, "CharissaArray") == 0 && Charissa == false) {
#ifdef INC_CHARISSA
		Charissa = true;
		cout << "//////// Charissa Array ////////" << endl << endl;
		
		// Instantiate the new array as a VDetector Object
		VDetector* myDetector = new TCharissaPhysics();
		
		// Read Position of Telescope
		ConfigFile.close();
		myDetector->ReadConfiguration(Path);
		ConfigFile.open(Path.c_str());
		
		// Add array to the VDetector Vector
		AddDetector("Charissa", myDetector);
#endif
    }

    ////////////////////////////////////////////////////
    ////////   Search for Chateau de Cristal    ////////
    ////////////////////////////////////////////////////
    else if (LineBuffer.compare(0, 9, "ChateauCristal") == 14 && ChateauCristal == false) {
#ifdef INC_CHATEAUCRISTAL
      ChateauCristal = true;
      cout << "//////// Chateau de Cristal Array ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TChateauCristalPhysics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("ChateauCristal", myDetector);
#endif
      }
 
    /////////////////////////////////////////
    //////////// Search for EXL  ////////////
    /////////////////////////////////////////
    else if (LineBuffer.compare(0, 3, "EXL") == 0 && EXL == false) {
#ifdef INC_EXL
      EXL = true ;
      cout << "//////// EXL Csi gamma detector ////////" << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TExlPhysics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("EXL", myDetector);
#endif
    }

    //////////////////////////////////////////////
    //////////      Search for Exogam    /////////
    //////////////////////////////////////////////
    else if (LineBuffer.compare(0, 11, "EXOGAMArray") == 0 && Exogam == false) {
#ifdef INC_EXOGAM
      Exogam = true ;
      cout << "//////// Exogam ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TExogamPhysics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("EXOGAM", myDetector);
#endif
    }
    ////////////////////////////////////////////
    ///////////// Search for Fatima ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 6, "Fatima") == 0 && FatimaDet == false) {
#ifdef INC_FATIMA
      FatimaDet = true;
      cout << "//////// FATIMA ////////" << endl << endl;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TFatimaPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());
      
      // Add array to the VDetector Vector
      AddDetector("FATIMA", myDetector);
#endif
    } 
    ////////////////////////////////////////////
    //////////// Search for Gaspard ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 14, "GaspardTracker") == 0 && GPDTracker == false) {
#ifdef INC_GASPARD
      GPDTracker = true ;
      cout << "//////// Gaspard Tracker ////////" << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new GaspardTracker();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("GASPARD", myDetector);
#endif
    }
    ////////////////////////////////////////////
    //////////// Search for Hyde2    ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 12, "Hyde2Tracker") == 0 && HYD2Tracker == false) {
#ifdef INC_HYDE2
      HYD2Tracker = true ;
      cout << "//////// Hyde2 Tracker ////////" << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Hyde2Tracker();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("HYDE2", myDetector);
#endif
    }
    
    ///////////////////////////////////////////////////////
    ///////////// Search for Ionisation Chamber ///////////
    ///////////////////////////////////////////////////////
    else if (LineBuffer.compare(0, 17, "IonisationChamber") == 0 && IonisationChamber == false) {
#ifdef INC_CHIO
      IonisationChamber = true;
      cout << "//////// Ionisation Chamger ////////" << endl << endl;

      /////// Case with analog electronics
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector_an = new TChio_anPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector_an->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("Chio_an", myDetector_an);


      /////// Case with digital electronics
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector_dig = new TChio_digPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector_dig->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("Chio_dig", myDetector_dig);
#endif
    }

    ////////////////////////////////////////////
    ///////////// Search for LaBr3 ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 5, "LaBr3") == 0 && LaBr3 == false) {
#ifdef INC_LABR3
      LaBr3 = true;
      cout << "//////// Plastic ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TLaBr3Physics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("LaBr3", myDetector);
#endif
    }

    ////////////////////////////////////////////
    ////////  Search for MUST2 Array    ////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 10, "MUST2Array") == 0 && MUST2 == false) {
#ifdef INC_MUST2
      MUST2 = true;
      cout << "//////// MUST2 Array ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TMust2Physics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("MUST2", myDetector);
#endif
    }

    ////////////////////////////////////////////
    ///////////// Search for Paris /////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 5, "Paris") == 0 && ParisDet == false) {
#ifdef INC_PARIS
      ParisDet = true;
      cout << "//////// Paris ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TParisPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("PARIS", myDetector);
#endif
    }
     
      ////////////////////////////////////////////
      ////////// Search for S2 (Micron)  /////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 2, "S2") == 0 && S2 == false) {
#ifdef INC_S2
         S2 = true;
         cout << "//////// S2 ////////" << endl;

         // Instantiate the new array as a VDetector Object
         VDetector* myDetector = new TS2Physics();

         // Read Position of Telescope
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // Add array to the VDetector Vector
         AddDetector("S2", myDetector);
#endif
      }

    ////////////////////////////////////////////
    ///////////// Search for SPEG //////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 4, "SPEG") == 0 && SPEG == false) {
#ifdef INC_SPEG
      SPEG = true ;
      cout << "//////// SPEG Spectrometer ////////" << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TSpegPhysics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("SPEG", myDetector);
#endif
    }

    ////////////////////////////////////////////
    //////////      Search for SSSD    /////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 9, "SSSDArray") == 0 && SSSD == false) {
#ifdef INC_SSSD
      SSSD = true ;
      cout << "//////// SSSD ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TSSSDPhysics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("SSSD", myDetector);
#endif
    }

    ////////////////////////////////////////////
    ///////////// Search for Plastic ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 19, "ScintillatorPlastic") == 0 && ScintillatorPlastic == false) {
#ifdef INC_PLASTIC
      ScintillatorPlastic = true;
      cout << "//////// Plastic ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TPlasticPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("Plastic", myDetector);
#endif
    }


    ////////////////////////////////////////////
    /////////      Search for Sharc    /////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 5, "Sharc") == 0 && Sharc == false) {
#ifdef INC_SHARC
      Sharc = true ;
      cout << "//////// Sharc ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TSharcPhysics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("Sharc", myDetector);
#endif
    }
    
    ////////////////////////////////////////////
    ///////////// Search for SiLi ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 4, "SiLi") == 0 && SiLi == false) {
#ifdef INC_SILI
      SiLi = true;
      cout << "//////// Plastic ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TSiLiPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("SiLi", myDetector);
#endif
    } 
    ////////////////////////////////////////////
    ///////////// Search for SiRes ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 5, "SiRes") == 0 && SiRes == false) {
#ifdef INC_SIRES
      SiRes = true;
      cout << "//////// Plastic ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TSiResPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("SiRes", myDetector);
#endif
    }
 
    ////////////////////////////////////////////
    ////////////// Search for TAC //////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 3, "TAC") == 0 && TAC == false) {
#ifdef INC_MISC
      TAC = true ;
      cout << "//////// TAC ////////" << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TTacPhysics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("TAC", myDetector);
#endif
    }

   
    ////////////////////////////////////////////
    ////////// Search for Tiara Hyball /////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 5, "Tiara") == 0 && TiaraHyball == false) {
#ifdef INC_TIARA
      TiaraHyball = true;
      cout << "//////// Tiara Hyball ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TTiaraHyballPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("TiaraHyball", myDetector);
#endif
    }
    
    ////////////////////////////////////////////
    ///////// Search for Tiara Barrel //////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 11, "TiaraBarrel") == 0 && TiaraBarrel == false) {
#ifdef INC_TIARA
      TiaraBarrel = true;
      cout << "//////// Tiara Barrel ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TTiaraBarrelPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("TiaraBarrel", myDetector);
#endif
    }
 
    ////////////////////////////////////////////
    ///////////// Search for Trifoil ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 7, "Trifoil") == 0 && Trifoil == false) {
#ifdef INC_TRIFOIL
      Trifoil = true;
      cout << "//////// Trifoil ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TTrifoilPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("Trifoil", myDetector);
#endif
    }
 
    ////////////////////////////////////////////
    ///////////// Search for Tigress ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 7, "Tigress") == 0 && Tigress == false) {
#ifdef INC_TIGRESS
      Tigress = true;
      cout << "//////// Tigress ////////" << endl << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TTigressPhysics();
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("Tigress", myDetector);
#endif
    }
 
      ////////////////////////////////////////////
      ////////// Search for W1 (Micron)  /////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 2, "W1") == 0 && W1 == false) {
#ifdef INC_W1
      W1 = true;
      cout << "//////// W1 ////////" << endl;

      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new TW1Physics();

      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add array to the VDetector Vector
      AddDetector("W1", myDetector);
#endif
    }

///////////////////////////////////////////////////////////////////////////////
//                           END OF DETECTOR                                 //
///////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////
    //////////// Search for Target /////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 13, "GeneralTarget") == 0 && GeneralTarget == false) {
      GeneralTarget = true ;
      cout << "////////// Target ///////////" << endl;

      // jump one line
      getline(ConfigFile, LineBuffer);
      getline(ConfigFile, LineBuffer);

      bool check_Thickness = false;
      //         bool check_Angle     = false;
      bool check_Radius    = false;
      bool check_Material  = false;
      bool check_X         = false;
      bool check_Y         = false;
      bool check_Z         = false;

      bool ReadingStatusTarget = true;
      while (ReadingStatusTarget) {
        ConfigFile >> DataBuffer;

        // Search for comment Symbol %
        if (DataBuffer.compare(0, 1, "%") == 0) {ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );getline(ConfigFile, LineBuffer);}

        else if (DataBuffer.compare(0, 10, "THICKNESS=") == 0) {
          check_Thickness = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetThickness = atof(DataBuffer.c_str()) * micrometer;
          m_TargetThickness = atof(DataBuffer.c_str());
          cout << "Target Thickness: " << m_TargetThickness << endl;
        }

        else if (DataBuffer.compare(0, 6, "ANGLE=") == 0) {
          //               check_Angle = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetAngle = atof(DataBuffer.c_str()) * deg;
          m_TargetAngle = atof(DataBuffer.c_str());
          cout << "Target Angle: " << m_TargetAngle << endl;
        }

        else if (DataBuffer.compare(0, 7, "RADIUS=") == 0) {
          check_Radius = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetRadius = atof(DataBuffer.c_str()) * mm;
          m_TargetRadius = atof(DataBuffer.c_str());
          cout << "Target Radius: " <<  m_TargetRadius << endl;
        }

        else if (DataBuffer.compare(0, 9, "MATERIAL=") == 0) {
          check_Material = true ;
          ConfigFile >> DataBuffer;
          m_TargetMaterial = DataBuffer;
          cout << "Target Material: " << m_TargetMaterial << endl;
        }

        else if (DataBuffer.compare(0, 2, "X=") == 0) {
          check_X = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetX = atoi(DataBuffer.c_str()) * mm;
          m_TargetX = atoi(DataBuffer.c_str());
          cout << "Target Coordinates (mm): ( " << m_TargetX << " ; ";
        }

        else if (DataBuffer.compare(0, 2, "Y=") == 0) {
          check_Y = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetY = atoi(DataBuffer.c_str()) * mm;
          m_TargetY = atoi(DataBuffer.c_str());
          cout << m_TargetY << " ; ";
        }

        else if (DataBuffer.compare(0, 2, "Z=") == 0) {
          check_Z = true ;
          ConfigFile >> DataBuffer;
          //               m_TargetZ = atoi(DataBuffer.c_str()) * mm;
          m_TargetZ = atoi(DataBuffer.c_str());
          cout  << m_TargetZ << " )" << endl;
        }

        ///////////////////////////////////////////////////
        // If no Target Token and no comments, toggle out
        else {
          ReadingStatusTarget = false; 
          cout << "WARNING : Wrong Token Sequence: Getting out " << endl;
        }

        ///////////////////////////////////////////////////
        // If all Token found toggle out
        if (check_Thickness && check_Radius && check_Material && check_X && check_Y && check_Z)
          ReadingStatusTarget = false;

      }
    }

    //Nothing understandable
    //else ;
  }

  ConfigFile.close();

  // Now that the detector are all added, they can initialise their Branch to the Root I/O
  InitializeRootInput();
  InitializeRootOutput();

  // If Requiered, they can also instiantiate their control histogramm
  if(NPOptionManager::getInstance()->GetGenerateHistoOption())
    InitSpectra();

  // The calibration Manager got all the parameter added, so it can load them from the calibration file
  CalibrationManager::getInstance()->LoadParameterFromFile();

  return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////   
void DetectorManager::BuildPhysicalEvent(){
  ClearEventPhysics();
  map<string,VDetector*>::iterator it;
   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    it->second->BuildPhysicalEvent();
    if(NPOptionManager::getInstance()->GetGenerateHistoOption()){
      it->second->FillSpectra();
      if(NPOptionManager::getInstance()->GetCheckHistoOption())
        it->second->CheckSpectra();
    }  
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::BuildSimplePhysicalEvent(){
  ClearEventPhysics();
  map<string,VDetector*>::iterator it;
  
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
    it->second->BuildSimplePhysicalEvent();
    if(NPOptionManager::getInstance()->GetGenerateHistoOption()){
        it->second->FillSpectra();
      if(NPOptionManager::getInstance()->GetCheckHistoOption())
        it->second->CheckSpectra();
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::InitializeRootInput(){

  if( NPOptionManager::getInstance()->GetDisableAllBranchOption() )
    RootInput::getInstance()->GetChain()->SetBranchStatus ( "*" , false ) ;

  map<string,VDetector*>::iterator it;

  if(NPOptionManager::getInstance()->GetInputPhysicalTreeOption())
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootInputPhysics();

  else // Default Case
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootInputRaw();
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void DetectorManager::InitializeRootOutput(){
  map<string,VDetector*>::iterator it;

  if(!NPOptionManager::getInstance()->GetInputPhysicalTreeOption())
    for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
      it->second->InitializeRootOutput();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::AddDetector(string DetectorName , VDetector* newDetector){
  m_Detector[DetectorName] = newDetector;
  newDetector->AddParameterToCalibrationManager();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
VDetector* DetectorManager::GetDetector(string name){
  map<string,VDetector*>::iterator it;
  it = m_Detector.find(name);
  if ( it!=m_Detector.end() ) return it->second;
  else{
    cout << endl;
    cout << "**********************************       Error       **********************************" << endl;
    cout << " No Detector " << name << " found in the Detector Manager" << endl;
    cout << "***************************************************************************************" << endl;
    cout << endl;
    exit(1);
  }

}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::ClearEventPhysics(){
  map<string,VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->ClearEventPhysics();
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::ClearEventData(){
  map<string,VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it)
    it->second->ClearEventData();
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
void DetectorManager::InitSpectra(){
  map<string,VDetector*>::iterator it;

  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
    it->second->InitSpectra();
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
vector< map< vector<string>, TH1* > > DetectorManager::GetSpectra()
{
   vector< map< vector<string>, TH1* > > myVector;

   map<string,VDetector*>::iterator it;
   // loop on detectors
   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
      myVector.push_back(it->second->GetSpectra());
   }

   return myVector;
}

/////////////////////////////////////////////////////////////////////////////////////////////////   
vector<string> DetectorManager::GetDetectorList(){
  map<string,VDetector*>::iterator it;
  vector<string> DetectorList;
  for (it = m_Detector.begin(); it != m_Detector.end(); ++it) { 
    DetectorList.push_back(it->first);
  }

  return DetectorList;
}


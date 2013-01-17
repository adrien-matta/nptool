#include "DetectorManager.h"

//   STL
#include <iostream>
#include <fstream>
#include <cstdlib>

//   Detector   
#include "DetectorList.inc"
//#include "TExogamPhysics.h"
#include "TMust2Physics.h"
#include "TCATSPhysics.h"
#include "TSSSDPhysics.h"
#include "TPlasticPhysics.h"
#include "TTrifoilPhysics.h"
#include "GaspardTracker.h"
#include "HydeTracker.h"
#include "Paris.h"
#include "TW1Physics.h"
#include "Shield.h"
#include "TSpegPhysics.h"
#include "TExlPhysics.h"
#include "TTacPhysics.h"
#include "NPOptionManager.h"
#include "RootInput.h"
/////////////////////////////////////////////////////////////////////////////////////////////////
//   Default Constructor
DetectorManager::DetectorManager()   
{
}



/////////////////////////////////////////////////////////////////////////////////////////////////   
//   Default Desstructor
DetectorManager::~DetectorManager()
{
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//   Read stream at ConfigFile and pick-up Token declaration of Detector
void DetectorManager::ReadConfigurationFile(string Path)   
{

   // Instantiate the Calibration Manager
   // All The detector will then add to it their parameter (see AddDetector)
   CalibrationManager::getInstance(NPOptionManager::getInstance()->GetCalibrationFile());

   ////////General Reading needs////////
   string LineBuffer;
   string DataBuffer;

   /////////Boolean////////////////////
   bool MUST2               = false;
   bool CATS                = false;
   bool SSSD                = false;
//   bool Exogam		    = false;
   bool ScintillatorPlastic = false;
   bool Trifoil             = false;
   bool GeneralTarget       = false;
   bool GPDTracker          = false;
   bool HYDTracker          = false;
   bool ParisDet            = false;
   bool ShieldDet           = false;
   bool W1                  = false;
   bool SPEG                = false;
   bool EXL                 = false;
   bool TAC                 = false;

   //////////////////////////////////////////////////////////////////////////////////////////
   string GlobalPath = getenv("NPTOOL");
   string StandardPath = GlobalPath + "/Inputs/DetectorConfiguration/" + Path;
   ifstream ConfigFile;
   ConfigFile.open(StandardPath.c_str());

   if (ConfigFile.is_open()) 
    {
      cout << endl << "/////////// Detector geometry ///////////" << endl;
      cout << "Configuration file " << Path << " loading " << endl;
      Path = StandardPath;
    }
  
   else 
    {
      ConfigFile.open( Path.c_str() );
      if(ConfigFile.is_open()) {
      cout << endl << "/////////// Detector geometry ///////////" << endl;
      cout << " Configuration file " << Path << " loading " << endl;
      }
      
      else {cout << "Configuration File " << Path << " not found" << endl;return;}
    }


   while (!ConfigFile.eof()) {
      // Pick-up next line
      getline(ConfigFile, LineBuffer);
      //Search for comment Symbol: %
      if (LineBuffer.compare(0, 1, "%") == 0) {   /*Do  Nothing*/ ;}

      ////////////////////////////////////////////
      //////////// Search for Gaspard ////////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 14, "GaspardTracker") == 0 && GPDTracker == false) {
#ifdef GASPARD
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
      //////////// Search for Hyde    ////////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 11, "HydeTracker") == 0 && HYDTracker == false) {
#ifdef HYDE
         HYDTracker = true ;
         cout << "//////// Hyde Tracker ////////" << endl;

         // Instantiate the new array as a VDetector Object
         VDetector* myDetector = new HydeTracker();

         // Read Position of Telescope
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // Add array to the VDetector Vector
         AddDetector("HYDE", myDetector);
#endif
      }
      ////////////////////////////////////////////
      ///////////// Search for Paris /////////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 5, "Paris") == 0 && ParisDet == false) {
#ifdef PARIS
         ParisDet = true;
         cout << "//////// Paris ////////" << endl << endl;

         // Instantiate the new array as a VDetector Object
         VDetector* myDetector = new Paris();
         // Read Position of Telescope
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // Add array to the VDetector Vector
         AddDetector("PARIS", myDetector);
#endif
      }
      ////////////////////////////////////////////
      ///////////// Search for Paris' Shield /////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 6, "Shield") == 0 && ShieldDet == false) {
#ifdef SHIELD
         ShieldDet = true;
         cout << "//////// Shield ////////" << endl << endl;

         // Instantiate the new array as a VDetector Object
         VDetector* myDetector = new Shield();
         // Read Position of Telescope
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // Add array to the VDetector Vector
         AddDetector("SHIELD", myDetector);
#endif
      }

      ////////////////////////////////////////////
      ////////  Search for MUST2 Array    ////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 10, "MUST2Array") == 0 && MUST2 == false) {
#ifdef MUST2
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
      ////////   Search for CATS Array    ////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 9, "CATSArray") == 0 && CATS == false) {
#ifdef CATS
         MUST2 = true;
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
      ////////// Search for W1 (Micron)  /////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 2, "W1") == 0 && W1 == false) {
#ifdef W1
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

      ////////////////////////////////////////////
      //////////      Search for SSSD    /////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 9, "SSSDArray") == 0 && SSSD == false) {
#ifdef SSSD
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
/*
      //////////////////////////////////////////////
      //////////      Search for Exogam    /////////
      //////////////////////////////////////////////
      else if (LineBuffer.compare(0, 11, "EXOGAMArray") == 0 && Exogam == false) {
#ifdef EXOGAM
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
*/
      ////////////////////////////////////////////
      ///////////// Search for Plastic ///////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 19, "ScintillatorPlastic") == 0 && ScintillatorPlastic == false) {
#ifdef PLASTIC
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
      ///////////// Search for SPEG //////////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 4, "SPEG") == 0 && SPEG == false) {
#ifdef SPEG
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

      ///////////////////////////////////////////////////////////
      //////////// Search for EXL Csi gamma detector ////////////
      ///////////////////////////////////////////////////////////
      else if (LineBuffer.compare(0, 3, "EXL") == 0 && EXL == false) {
#ifdef EXL
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

      ////////////////////////////////////////////
      ////////////// Search for TAC //////////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 3, "TAC") == 0 && TAC == false) {
#ifdef TAC
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
      ///////////// Search for Trifoil ///////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 7, "Trifoil") == 0 && Trifoil == false) {
#ifdef TRIFOIL
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
            if (DataBuffer.compare(0, 1, "%") == 0) {/*Do Nothing*/;}

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

   // The calibration Manager got all the parameter added, so it can load them from the calibration file
   CalibrationManager::getInstance()->LoadParameterFromFile();

   return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////   
void DetectorManager::BuildPhysicalEvent()                  
{
   map<string,VDetector*>::iterator it;

   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
      it->second->BuildPhysicalEvent();
   }
}



/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::BuildSimplePhysicalEvent()
{
   map<string,VDetector*>::iterator it;

   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
      it->second->BuildSimplePhysicalEvent();
   }
}



/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::InitializeRootInput()                  
{

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
void DetectorManager::InitializeRootOutput()   
{
   map<string,VDetector*>::iterator it;

   if(!NPOptionManager::getInstance()->GetInputPhysicalTreeOption())
      for (it = m_Detector.begin(); it != m_Detector.end(); ++it) 
         it->second->InitializeRootOutput();
}



/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::AddDetector(string DetectorName , VDetector* newDetector)
{
   m_Detector[DetectorName] = newDetector;
   newDetector->AddParameterToCalibrationManager();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
VDetector* DetectorManager::GetDetector(string name)
{
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
void DetectorManager::ClearEventPhysics()
{
   map<string,VDetector*>::iterator it;

   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
      it->second->ClearEventPhysics();
   }
}



/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::ClearEventData()
{
   map<string,VDetector*>::iterator it;

   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
      it->second->ClearEventData();
   }
}
         

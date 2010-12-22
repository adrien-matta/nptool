#include "DetectorManager.h"

//	STL
#include <iostream>
#include <fstream>
#include <cstdlib>

//	Detector	
#include "../MUST2/TMust2Physics.h"
#include "../SSSD/TSSSDPhysics.h"
#include "../Plastic/TPlasticPhysics.h"
#include "../GASPARD/GaspardTracker.h"
#include "../Paris/Paris.h"
#include "../Shield/Shield.h"


/////////////////////////////////////////////////////////////////////////////////////////////////
//	Default Constructor
DetectorManager::DetectorManager()	
{
}



/////////////////////////////////////////////////////////////////////////////////////////////////	
//	Default Desstructor
DetectorManager::~DetectorManager()
{
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//	Read stream at ConfigFile and pick-up Token declaration of Detector
void DetectorManager::ReadConfigurationFile(string Path)	
{
   ////////General Reading needs////////
   string LineBuffer;
   string DataBuffer;

   /////////Boolean////////////////////
   bool MUST2               = false;
   bool AddThinSi           = false;
   bool ScintillatorPlastic = false;
   bool GeneralTarget       = false;
   bool GPDTracker          = false;
   bool ParisDet            = false;
   bool ShieldDet           = false;

   //////////////////////////////////////////////////////////////////////////////////////////
   string GlobalPath = getenv("NPTOOL");
   string StandardPath = GlobalPath + "/Inputs/DetectorConfiguration/" + Path;
   ifstream ConfigFile;
   ConfigFile.open(StandardPath.c_str());

   if (ConfigFile.is_open()) 
    {
      cout << "/////////////////////////////" << endl;
      cout << " Configuration file " << Path << " loading " << endl;
      Path = StandardPath;
    }
  
   else 
    {
      ConfigFile.open( Path.c_str() );
      if(ConfigFile.is_open()) {
      cout << "/////////////////////////////" << endl;
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
      }
      ////////////////////////////////////////////
      ///////////// Search for Paris /////////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 5, "Paris") == 0 && ParisDet == false) {
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
      }
      ////////////////////////////////////////////
      ///////////// Search for Paris' Shield /////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 6, "Shield") == 0 && ShieldDet == false) {
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
      }

      ////////////////////////////////////////////
      //////// Search for MUST2 Array  ////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 10, "MUST2Array") == 0 && MUST2 == false) {
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
      }

      ////////////////////////////////////////////
      ////////// Search for ThinSi (SSSD)/////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 9, "AddThinSi") == 0 && AddThinSi == false) {
         AddThinSi = true ;
         cout << "//////// Thin Si ////////" << endl << endl;

         // Instantiate the new array as a VDetector Object
         VDetector* myDetector = new TSSSDPhysics();

         // Read Position of Telescope
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // Add array to the VDetector Vector
         AddDetector("SSSD", myDetector);
      }

      ////////////////////////////////////////////
      ///////////// Search for Plastic ///////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 19, "ScintillatorPlastic") == 0 && ScintillatorPlastic == false) {
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
      }


      ////////////////////////////////////////////
      //////////// Search for Target /////////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 13, "GeneralTarget") == 0 && GeneralTarget == false) {
         GeneralTarget = true ;
         cout << "////////// Target ///////////" << endl << endl;

         // jump one line
         getline(ConfigFile, LineBuffer);
         getline(ConfigFile, LineBuffer);

         bool check_Thickness = false;
         bool check_Angle     = false;
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
               check_Angle = true ;
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

   InitializeRootInput();
   InitializeRootOutput();

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
   map<string,VDetector*>::iterator it;

   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
      it->second->InitializeRootInput();
   }
}



/////////////////////////////////////////////////////////////////////////////////////////////////	
void DetectorManager::InitializeRootOutput()	
{
   map<string,VDetector*>::iterator it;

   for (it = m_Detector.begin(); it != m_Detector.end(); ++it) {
      it->second->InitializeRootOutput();
   }   
}



/////////////////////////////////////////////////////////////////////////////////////////////////
void DetectorManager::AddDetector(string DetectorName , VDetector* newDetector)
{
   m_Detector[DetectorName] = newDetector;
   newDetector->AddParameterToCalibrationManager();
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
			

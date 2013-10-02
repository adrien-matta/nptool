#include "DetectorManager.hh"

//	STL
#include <iostream>
#include <fstream>
#include <cstdlib>

//	Detector	
#include "Must2Array.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//	Default Constructor and Destructor
DetectorManager::DetectorManager()	
	{}
	
/////////////////////////////////////////////////////////////////////////////////////////////////	
DetectorManager::~DetectorManager()
	{}


/////////////////////////////////////////////////////////////////////////////////////////////////
//	Read stream at ConfigFile and pick-up Token declaration of Detector
void	DetectorManager::ReadConfigurationFile(string Path)	
	{
		////////General Reading needs////////
	   string LineBuffer;
	   string DataBuffer;

	   /////////Boolean////////////////////
	   bool MUST2           = false  ;
	   bool AddThinSi       = false  ;
	   bool GeneralTarget   = false  ;
	   bool GPDTracker      = false  ;	// Gaspard Tracker
	   //////////////////////////////////////////////////////////////////////////////////////////
	   // added by Nicolas [07/05/09]
	   string GlobalPath = getenv("NPTOOL");
	   Path = GlobalPath + "/Inputs/DetectorConfiguration/" + Path;
	   ifstream ConfigFile;
	   ConfigFile.open(Path.c_str());

	   if (ConfigFile.is_open())
	   	{
	   		cout << "/////////////////////////////" << endl;
	      	cout << " Configuration file " << Path << " loading " << endl;
	   	}	
	   	
	   else 
	   	{
	      cout << " Error, no configuration file" << Path << " found" << endl;
	      return;
	   	}


	   while (!ConfigFile.eof()) {
	      //Pick-up next line
	      getline(ConfigFile, LineBuffer);
	      //Search for comment Symbol: %
	      if (LineBuffer.compare(0, 1, "%") == 0) {   /*Do  Nothing*/ ;}

	    /*  ////////////////////////////////////////////
	      //////////// Search for Gaspard ////////////
	      ////////////////////////////////////////////
	      else if (LineBuffer.compare(0, 14, "GaspardTracker") == 0 && GPDTracker == false) {
	         GPDTracker = true ;
	         cout << "//////// Gaspard Tracker ////////" << endl   ;

	         // Instantiate the new array as a VDetector Object
	         VDetector* myDetector = new GaspardTracker()                  ;

	         // Read Position of Telescope
	         ConfigFile.close()                                 ;
	         myDetector->ReadConfiguration(Path)                   ;
	         ConfigFile.open(Path.c_str())                      ;

	         // Add array to the VDetector Vector
	         AddDetector(myDetector)                            ;
	      }*/

	      ////////////////////////////////////////////
	      //////// Search for MUST2 Array  ////////
	      ////////////////////////////////////////////
	      else if (LineBuffer.compare(0, 10, "MUST2Array") == 0 && MUST2 == false) {
	         MUST2 = true ;
	         cout << "//////// MUST2 Array ////////" << endl   << endl   ;

	         // Instantiate the new array as a VDetector Object
	         VDetector* myDetector = new MUST2Array()                 ;

	         // Read Position of Telescope
	         ConfigFile.close()                                    ;
	         myDetector->ReadConfiguration(Path)                      ;
	         ConfigFile.open(Path.c_str())                         ;

	         // Add array to the VDetector Vector
	         AddDetector("MUST2" , myDetector)                               ;
	      }

	/*      ////////////////////////////////////////////
	      ////////// Search for Add.ThinSi ///////////
	      ////////////////////////////////////////////
	      else if (LineBuffer.compare(0, 9, "AddThinSi") == 0 && AddThinSi == false) {
	         AddThinSi = true ;
	         cout << "//////// Thin Si ////////" << endl << endl   ;

	         // Instantiate the new array as a VDetector Object
	         VDetector* myDetector = new ThinSi()                  ;

	         // Read Position of Telescope
	         ConfigFile.close()                                 ;
	         myDetector->ReadConfiguration(Path)                   ;
	         ConfigFile.open(Path.c_str())                      ;

	         // Add array to the VDetector Vector
	         AddDetector(myDetector)                            ;
	      }

	      ////////////////////////////////////////////
	      //////////// Search for Target /////////////
	      ////////////////////////////////////////////

	      else if (LineBuffer.compare(0, 13, "GeneralTarget") == 0 && GeneralTarget == false) {
	         GeneralTarget = true ;
	         cout << "////////// Target ///////////" << endl   << endl   ;

	         // Instantiate the new array as a VDetector Objects
	         VDetector* myDetector = new Target()                     ;

	         // Read Position and target specification
	         ConfigFile.close()                                    ;
	         myDetector->ReadConfiguration(Path)                      ;
	         ConfigFile.open(Path.c_str())                         ;

	         m_TargetThickness = ((Target*)myDetector)->GetTargetThickness()   ;
	         m_TargetRadius    = ((Target*)myDetector)->GetTargetRadius()   ;
	         m_TargetX        = ((Target*)myDetector)->GetTargetX()         ;
	         m_TargetY        = ((Target*)myDetector)->GetTargetY()         ;
	         m_TargetZ        = ((Target*)myDetector)->GetTargetZ()         ;

	         // Add target to the VDetector Vector
	         AddDetector(myDetector)                               ;
	      }*/

	      //Nothing understandable
	      //else ;
	   }

		ConfigFile.close();
		
		InitializeRootInput();
		InitializeRootOutput();
		
		return   ;
	}
	
/////////////////////////////////////////////////////////////////////////////////////////////////	

void	DetectorManager::BuildPhysicalEvent()						
	{	
		map<string,VDetector*>::iterator it	;
		
		for( it = m_Detector.begin() ; it != m_Detector.end() ; ++it) 
			{
				it->second->BuildPhysicalEvent()	;	
			}           
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
	
void	DetectorManager::BuildSimplePhysicalEvent()					
	{
		map<string,VDetector*>::iterator it	;
		
		for( it = m_Detector.begin() ; it != m_Detector.end() ; ++it) 
			{
				it->second->BuildSimplePhysicalEvent()	;	
			}    
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
	
void	DetectorManager::InitializeRootInput()						
	{
		map<string,VDetector*>::iterator it	;
		
		for( it = m_Detector.begin() ; it != m_Detector.end() ; ++it) 
			{
				it->second->InitializeRootInput()	;	
			}    
	}
/////////////////////////////////////////////////////////////////////////////////////////////////	

void	DetectorManager::InitializeRootOutput()						
	{
		map<string,VDetector*>::iterator it	;
		
		for( it = m_Detector.begin() ; it != m_Detector.end() ; ++it) 
			{
				it->second->InitializeRootOutput()	;	
			}   
		
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
	
void	DetectorManager::AddDetector(string DetectorName , VDetector* newDetector)	
	{
		m_Detector["MUST2"] = newDetector ;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
void	DetectorManager::ClearEventPhysics()	
	{
		map<string,VDetector*>::iterator it	;
		
		for( it = m_Detector.begin() ; it != m_Detector.end() ; ++it) 
			{
				it->second->ClearEventPhysics()	;	
			}   
		
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
void	DetectorManager::ClearEventData()
	{
		map<string,VDetector*>::iterator it	;
		
		for( it = m_Detector.begin() ; it != m_Detector.end() ; ++it) 
			{
				it->second->ClearEventData()	;	
			}   
		
	}
			

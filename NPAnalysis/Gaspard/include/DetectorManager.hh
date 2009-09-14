#ifndef DetectorManager_h
#define DetectorManager_h

//	NPL
#include "VDetector.h"

//	STL
#include <string>
#include <map>

using namespace std ;
using namespace NPA ;

// This class manage a map of virtual detector

class DetectorManager
	{
		public:
			DetectorManager()		;
			~DetectorManager()		;
			
		public:
			//	Read stream at Path and pick-up Token declaration of Detector
			void	ReadConfigurationFile(string Path)	;
			void	BuildPhysicalEvent()				;
			void	BuildSimplePhysicalEvent()			;
			void	InitializeRootInput()				;
			void	InitializeRootOutput()				;
			void	AddDetector(string,VDetector*)		;
			void	ClearEventPhysics()					;
			void	ClearEventData()					;

		public:	//	The map containning all detectors
			//	Using a Map one can access to any detector using its name
			map<string,VDetector*>	m_Detector	;
				
	};

#endif 

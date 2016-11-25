---
layout: manual 
title: Editing your new NPSimulation classes
permalink: /manual/adding-detectors_part3/
author: Freddy Flavigny
manual_order: 12 
show_in_nav: false 
---

## Simulation classes

When creating a new detector with the nptool-wizard,  a folder with the name of your detector is created in NPSimulation/Detectors  including two files (.hh and .cc) handling the necessary methods to describe the detector geometry and readout of input parameters in the case of an unsegmented plastic scintillator. This third tutorial will guide you to modify this class to handle segmentation and obtain a single sided silicon striped detector.


* * *
name:
: TMSX25

* * *

location:
: NPSimulation/Detectors/MSX25

* * *

files:
: MSX25.h 
: MSX25.cxx

* * *

inherittance:
: NPS::VDetector

* * *


### Class members

The private data members declared in NPSimulation/MSX25/MSX25.hh are the following:
{% highlight C++ %}
  private:
    TnewDetData* m_Event ;

    // Geometry
    // Detector Coordinate 
    vector<double>  m_R; 
    vector<double>  m_Theta;
    vector<double>  m_Phi; 
    
    //   Shape type
    vector<string> m_Shape ;
   
    // Visualisation Attribute
    G4VisAttributes* m_VisSquare;
    G4VisAttributes* m_VisCylinder;

    // Logical Volumes
    G4LogicalVolume* m_SquareDetector;
    G4LogicalVolume* m_CylindricalDetector;
{% endhighlight %}
The m_Event object, from the data class modified in the previous tutorial, will be used here to store the raw physical information of a simulated event. The detector coordinate provided by the user as an input in the file (/Inputs/DetectorConfiguration/MSX25.txt) will be read and stored temporarily in the vectors m_R, m_Theta, m_Phi. The m_Shape attribute defines if the detector is cylindrical or square and is also initialized when reading the input detector file.

Nothing needs to be change here to handle a stripped detector since the format changes in m_Event were made in the previous tutorial.


### Namespace parameters

At the beginning of the MSX25.cc a namespace is defined to handle the physical dimensions and properties of the detector:
{% highlight C++ %}
namespace MSX25_NS{
  const double EnergyThreshold = 0.1*MeV;
  const double ResoTime = 4.5*ns ;
  const double ResoEnergy = 1.0*MeV ;
  const double Radius = 50*mm ; 
  const double Width = 100*mm ;
  const double Thickness = 300*mm ;
  const string Material = "BC400";
}
{% endhighlight %}
These properties are the one that are defined once for all (hard-coded) but not the one that you want to vary frequently and use as a parameter. Here for the turorial we want adapt these properties for a thin single-sided silicon stripped detector:
{% highlight C++ %}
namespace MSX25_NS{
  const double EnergyThreshold = 0.1*MeV;
  const double ResoTime = 0.213*ns;         // Unit is ns/2.35 ,  FWHM = 500 ps
  const double ResoEnergy = 0.017*MeV ;     // Unit is MeV/2.35 , FWHM = 40 keV
  const double Radius = 50*mm ;
  const double Width = 68*mm ;
  const double Thickness = 300*micrometer ;
  const string Material = "Si";
  const G4int  NumberOfStrips = 25 ;
}
{% endhighlight %}

### Methods

Most of the methods in the generated class inherits form the abstract base class NPS :VDetector whose file are located in NPSimulation/Core/NPSVDetector.hh. All these particular methods are generic and called either by the DetectorConstruction or the EventAction class necessary to perform  Geant4 simulation. We now have to implement some of these methods inherrited from NPS::VDetector.  

#### ReadConfiguration
This method reads the geometry input file provided by the user.  It mainly includes reading the position of the detector with respect to the frame origin . Two ways of postionning the detector are available by default : using the center of the detector and spherical coordinates (r,theta,phi) or giving particular position of the detector as input such as the corners of the front face of a telescope (x,y,z). A token based approach is used similarly to the one used in the NPLib classes. For some simple and standard detectors, other options can be provided and read using this method  (ex : detector thickness, simple shape).

If all necessary informations are read, the AddnewDet() method is called and add the coordinate of the detector to a vector (m_R ,m_Theta, m_Phi). If  several detectors of this kind are read, the size of this vector indicates the number of detectors that were created successfully.

The method does not need any change.

#### ConstructDetector
This method takes the world  logical volume as an argument and is used to create the detector (material, logical volume) and initialize its sensitive part. It is called in the DetectorConstruction class (mandatory for Geant4). It includes the important call to G4PVPlacementthat creates the physical volume:

{% highlight C++ %}
new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
                  BuildSquareDetector(),
                  "newDet", world, false, i+1);
{% endhighlight %}

where the BuildSquareDetector() method returns the logical volume built from a given solid geometry: 
{% highlight C++ %}

G4LogicalVolume* newDet::BuildSquareDetector(){
    if(!m_SquareDetector){ 
        G4Box* box = new G4Box("newDet_Box",MSX25_NS::Width*0.5,MSX25_NS::Width*0.5,MSX25_NS::Thickness*0.5);
        G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(MSX25_NS::Material);
        m_SquareDetector = new G4LogicalVolume(box,DetectorMaterial,"logic_newDet_Box",0,0,0);
        m_SquareDetector->SetVisAttributes(m_VisSquare);
        m_SquareDetector->SetSensitiveDetector(m_MSX25Scorer);
    }
    return m_SquareDetector;
}
{% endhighlight %}
Remark: The scorer "m_MSX25Scorer" used in the SetSensitiveDetector() method is defined in the two following methods.

#### InitializeScorers 
This method declares and initialize the scorer that will be used to store the properties of the simulated event  that are of interest for the user (deposited energy, time, detectorID, position, etc.). It also attaches this scorer to the Geant4 sensitive detector manager. For this tutorial,  here is the declaration that has to be used for a silicon stripped detector scorer:

{% highlight C++ %}
#include "SiliconScorers.hh" // To add at beginning of file
...
G4VPrimitiveScorer* StripScorer =
    new  SILICONSCORERS::PS_Silicon_Rectangle("StripScorer",0,
        MSX25_NS::Width,     // Width
        MSX25_NS::Width,     // Length (here same as Width for a square det.)
        MSX25_NS::NumberOfStrips,    // Width segmentation
        1);                         // Length segmentation
  m_MSX25Scorer->RegisterPrimitive(StripScorer);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_MSX25Scorer);
{% endhighlight %}


Different kind of generic scorer classes are available in NPSimulation/Core and can be used :  

1.  CalorimeterScorers: this class handles scorers for unsegmented detectors  mainly used to obtain energy deposit, time  and detector number information. This is the kind of scorer used in the example build from the nptool-wizard.
2.  SiliconScorers : this class handles scorers for stripped silicon detectors of different kind (Rectangle, Annular, Resistive) but are quite generic and could be used for other kind of segmented detector.
3.  PhotoDiodeScorers : this class handles scorer for photon detectors.


To illustrate how hit informaions are stored in a scorer, below is displayed a part of the function ProcessHits() from the SiliconScorer class for a rectangle detector. The longitudinal strip number (m_StripLengthNumber) is calculated using the position of the hit (m_Position) in the sensitive detector, the strip pitch and the size of the detector plane. It is the  stored in the eight element of the scorer Info array.

{% highlight C++ %}
  m_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(m_Position);

  if(m_Axis=="xy"){
    m_StripLengthNumber = (int)((m_Position.x() + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
    m_StripWidthNumber = (int)((m_Position.y() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
  }
  else if(m_Axis=="yz"){
    m_StripLengthNumber = (int)((m_Position.y() + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
    m_StripWidthNumber = (int)((m_Position.z() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
  }
  else if(m_Axis=="xz"){
    m_StripLengthNumber = (int)((m_Position.x() + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
    m_StripWidthNumber = (int)((m_Position.z() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
  }

  //Rare case where particle is close to edge of silicon plan
  if (m_StripLengthNumber > m_NumberOfStripLength) m_StripLengthNumber = m_NumberOfStripLength;
  if (m_StripWidthNumber > m_NumberOfStripWidth) m_StripWidthNumber = m_NumberOfStripWidth;

  Infos[7] = m_DetectorNumber;
  Infos[8] = m_StripLengthNumber;
  Infos[9] = m_StripWidthNumber;
{% endhighlight %}




#### ReadSensitive
This method reads the hit map of the sensitive part of the detector using the chosen scorer (initialized previously)  to access the necessary information for a given event and attach it to the event object from the corresponding NPLib data class. As you can see, the eight element of the scorer Info array is accessed and stored to be attached to the event object:

{% highlight C++ %}
// Strip scorer
G4THitsMap<G4double*>* StripHitMap;
std::map<G4int, G4double**>::iterator Strip_itr;
G4int StripCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("MSX25Scorer/StripScorer");
StripHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(StripCollectionID));

// Loop on the Strip map
for (Strip_itr = StripHitMap->GetMap()->begin() ; Strip_itr != StripHitMap->GetMap()->end() ; Strip_itr++){
    G4double* Info = *(Strip_itr->second);
    double Energy = RandGauss::shoot(Info[0], MSX25_NS::ResoEnergy);
    if(Energy > MSX25_NS::EnergyThreshold){
        double Time     = RandGauss::shoot(Info[1], MSX25_NS::ResoTime);
        int DetNbr      = (int) Info[7];
        int StripNbr    = (int) Info[8];
        m_Event->SetEnergy(DetNbr, StripNbr, Energy)
        m_Event->SetTime(DetNbr, StripNbr, Time);

    }
}
{% endhighlight %}



#### InitializeRootOutput      
This method access the output tree via the RootOutput singleton and connect the data class to a new branch named after the detector.

The methods does not need any change. 

### Compilation

Once all the changes are made in this simulation class, you can compile your work

{% highlight bash %}
> make -jn install
{% endhighlight %}

or if you are using ninja-build
{% highlight bash %}
> ninja install 
{% endhighlight %}


### Execution and test

To test all the changes you made are working fine, simulate and analyze again some events with an alpha source:

{% highlight bash %}
> npsimulation -D MSX25.detector -E alpha.source
> run/beamOn 1000

> npanalysis --last-sim -GH
{% endhighlight %}




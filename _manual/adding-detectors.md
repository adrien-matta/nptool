---
layout: manual 
title: Adding a new detector 
permalink: /manual/adding-detectors/
author: Adrien Matta
manual_order: 10 
show_in_nav: false 
---
THIS PAGE IS UNDER CONSTRUCTION

In this tutorial we will discussed how to add a new detector to the nptool. This is done through an utility, `nptool-wizard`, that wil lcreate for you all the file needed by a detector. You will then have to edit all of those files consistently to create a new working detector, in this tutorial we will modify the class to describe a Single Sided Stripped Silicon detector from Micron Semiconductor, the MSX25.  `nptool-wizard` is a simple bash script that will ask you a few information about the detector you want to create and then will copy and edit the skeleton files to form a new working detector. Straight after this process you will be able to compile and run both simulation and analysis with your new detector. The detector skeleton describe a minimalist detector showing off the main features of the framework. The detector is a simple scintillator, that can be placed anywhere in space and which shape could be rectangular or cylindrical. The physical information recorded by the detector Energy and Time.

## Running nptool-wizard
`nptool-wizard` is added to your path at compilation of the NPLib, so make sure you compiled and install NPLib before following this tutorial. To call the script simply do:
{% highlight bash %}
> nptool-wizard
{% endhighlight %}

You will be prompted by the script to enter your new detector name (for the sake of this tutorial, we will assume you enterred MSX25), your own name and email. Those information are added to the header of each created file, in order to facilitate communication between people maintening the package. If the detector name you choose is already in use by another detector, you will be prompted with an error. You can override this safety features using the -f flag, in that case the detector is created regardless and the preexisting detector folders are overwritten.  

`nptool-wizard` will prompt you with the list of file created and ask you if you want to add all this file to the git repository. For the purpose of this tutorial, say `no`. If you say `yes` a `git add` command will be run on every newly created file, allowing for a fast and easy commit of your new detector. 

## Compilation of the newly created classes
In order to compile de the newly created classes you have to run cmake agoin in NPLib and NPSimulation folders. Doing cmake will add the new directory to the list of file to compile.

{% highlight bash %}
> npl
> cmake ./
> make -jn
> nps
> cmake ./
> make -jn
{% endhighlight %}

And we can now run the simulation:
{% highlight bash %}
> npsimulation -D MSX25.detector -E alpha.source 
{% endhighlight %}

You should be able to see a target surrounded by three green detector: One cylindrical and two rectangular one.

## Detector classes
Four classes are created in the process, three in NPLib will describe the data storage and analysis and one in the simulation will describe the detector geometry and readout.

### The Data class

* * *
name:
: TMSX25Data

* * *

location:
: NPLib/Detectors/MSX25

* * *

files:
: TMSX25Data.h 
: TMSX25Data.cxx

* * *

inherittance:
: TObject (ROOT)

* * *

This class is a very basic container used to store raw data in the output tree of either the simulation or the experiment. The nptool approach is to store the data in std::vector, which offer an easy mechanism to store data of variable size, keeping the size of the output file to a minimum. The vector are declared as private member of the class and accessed/modified using Setter/Getter methods.


The private data member, located in NPLib/Detectors/MSX25/TMSX25Data.h of the default class are the following:
{% highlight C++ %}
  private: 
    // Energy
    vector<UShort_t>   fMSX25_E_DetectorNbr;
    vector<Double_t>   fMSX25_Energy;

    // Time
    vector<UShort_t>   fMSX25_T_DetectorNbr;
    vector<Double_t>   fMSX25_Time;
{% endhighlight %}

This layout allow to store a list of energies and a list of time, with their associated detector number, for each event. Let change this for our MSX25 detector, for which we will need to store the energy, time, detector number and strip number for each event.

{% highlight C++ %}
  private: 
    // Energy
    vector<UShort_t>   fMSX25_E_DetectorNbr;
    vector<UShort_t>   fMSX25_E_StripNbr;
    vector<Double_t>   fMSX25_Energy;

    // Time
    vector<UShort_t>   fMSX25_T_DetectorNbr;
    vector<UShort_t>   fMSX25_T_StripNbr;
    vector<Double_t>   fMSX25_Time;
{% endhighlight %}

We now need to change the Setter and Getter accordingly, you will note that the class is design so all the three vector related to energy : Energy, detector number and strip number are the same size. Same applies to the three time related vector. For this reason it is better practice to use "global" setter that fill the three vector at the same time:
{% highlight C++ %}
public:
    //////////////////////    SETTERS    ////////////////////////
    // Energy
    inline void SetE_DetectorNbr(const UShort_t& DetNbr)
      {fMSX25_E_DetectorNbr.push_back(DetNbr);} //!
    inline void SetE_StripNbr(const UShort_t& StripNbr)
      {fMSX25_E_DetectorNbr.push_back(DetNbr);} //!

    inline void Set_Energy(const Double_t& Energy)
      {fMSX25_Energy.push_back(Energy);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetEnergy(const UShort_t& DetNbr,const USHort_t& StripNbr, const Double_t& Energy) {
      SetE_DetectorNbr(DetNbr);
      SetE_StripNbr(StripNbr);
      Set_Energy(Energy);
    };//!

    // Time
    inline void SetT_DetectorNbr(const UShort_t& DetNbr)
      {fMSX25_T_DetectorNbr.push_back(DetNbr);} //!
    inline void SetT_StripNbr(const UShort_t& DetNbr)
      {fMSX25_T_StripNbr.push_back(DetNbr);} //!
    inline void Set_Time(const Double_t& Time)
      {fMSX25_Time.push_back(Time);}//!
    // Prefer global setter so that all vectors have the same size
    inline void SetTime(const UShort_t& DetNbr, const UShort_t& StripNbr, const Double_t& Time)	{
      SetT_DetectorNbr(DetNbr);
      SetT_StripNbr(StripNbr);
      Set_Time(Time);
    };//!


    //////////////////////    GETTERS    ////////////////////////
    // Energy
    inline UShort_t GetMultEnergy() const
      {return fMSX25_E_DetectorNbr.size();}
    inline UShort_t GetE_DetectorNbr(const unsigned int &i) const 
      {return fMSX25_E_DetectorNbr[i];}//!
    inline UShort_t GetE_StripNbr(const unsigned int &i) const 
      {return fMSX25_E_StripNbr[i];}//!
    inline Double_t Get_Energy(const unsigned int &i) const 
      {return fMSX25_Energy[i];}//!

    // Time
    inline UShort_t GetMultTime() const
      {return fMSX25_T_DetectorNbr.size();}
    inline UShort_t GetT_DetectorNbr(const unsigned int &i) const 
      {return fMSX25_T_DetectorNbr[i];}//!
    inline UShort_t GetT_StripNbr(const unsigned int &i) const 
      {return fMSX25_T_StripNbr[i];}//!
    inline Double_t Get_Time(const unsigned int &i) const 
      {return fMSX25_Time[i];}//!
{% endhighlight %}

A few comments:

1. You will notice that we use inline declaration for the setter and getter. This allow a better optimisation of the code by the compilator, bypassing the call of the function and replacing it by its code. Similarly we pass the argument of the functions by reference to avoid making a local copy at each call. This important because these methods are called for each event analysed or simulated, so typically millions of time.
2. You will notice the use of `\\!` after the functionn declaration. This prevent root from making dictionnaries entries for these methods and pollute the TBrowser view. You can also use this to ask Root not to store a private member in the tree (more about that when talking about the Physics class).

### The Physics class

* * *
name:
: TMSX25Physics

* * *

location:
: NPLib/Detectors/MSX25

* * *

files:
: TMSX25Physics.h 
: TMSX25Physics.cxx

* * *

inherittance:
: NPL::VDetector (NPTool) and TObject (ROOT)

* * *

This class hold the heart of the data analysis. It takes the raw information from the data class and transform it into a meaningfull physical information. This typically involve applying calibration and threshold in a first stage, the option PreTreat, and matching all the data together (Front and back coincidences, Energy and Time coincidences,...) in the mandatory BuildPhysicalEvent method. The class derived from the abstract base classe NPL::VDetector, which source could be found in NPLib/Core/NPVDetector.h . This class define the property of all the Physics class in the framework. An associated manager, the DetectorManager take care of calling the method of each detector at the right time.

First we have to define the physical informations that the class will keep in the output tree. In our case we want to have an energy, a time, a detector number and a strip number. These information are very close to the one in the previous class, however a fundamental difference apply, all our vector will be the same size, corresponding to the multiplicity of the event. In the previous class you could have a size 2 in energy and 1 in time for example (if a TAC was not properly readout for example).


Lets change the data obtained after the BuildPhysicalEvent by:
{% highlight C++ %}
public:
  vector<int>     DetectorNumber; 
  vector<int>     StripNumber; 
  vector<double>  Energy;
  vector<double>  Time; 
{% endhighlight %}


We now have to implement each of the methods inherrited from NPL::VDetector 

#### ReadConfiguration(string)
This method read the detector configuration file and look for the appropriate token. It is by implementing this method that you tell what an input file looks like for your detector. We use a token based approach, allowing for very flexible design. Some detector for instance have variable geometry (thickness or size) and material (scintillator type, target material,...).

#### AddParameterToCalibrationManager 
#### BuildPhysicalEvent  
#### BuildSimplePhysicalEvent
#### InitializeRootInputRaw 
#### InitializeRootInputPhysics   
#### InitializeRootOutput      
#### ClearEventPhysics()
#### ClearEventData()    
#### InitSpectra
#### FillSpectra   
#### CheckSpectra    
#### ClearSpectra
#### WriteSpectra 












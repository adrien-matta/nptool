---
layout: manual 
title: Editing classes created by nptool-wizard in NPLib 
permalink: /manual/adding-detectors_part2/
author: Adrien Matta
manual_order: 11 
show_in_nav: false 
---

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


We now have to implement each of the methods inherrited from NPL::VDetector.  

#### ReadConfiguration
This method read the detector configuration file and look for the appropriate token. It is by implementing this method that you tell what an input file looks like for your detector. We use a token based approach, allowing for very flexible design. Some detector for instance have variable geometry (thickness or size) and material (scintillator type, target material,...).

#### AddParameterToCalibrationManager 
This function is adding the list of token used in the calibration to the calibration manager. The later will then parse all the calibration files provided and look for the token. In our case we want to add new token including the strip number as well as the detector number, we therefore need to modify the for loop to have two counters. The NPL::DetectorManager::AddParameter function take three argument. The first one is the detector name, the second one in the name of the parameter as stored in memory and the third name is the token as written in the file.

{% highlight C++ %}
CalibrationManager* Cal = CalibrationManager::getInstance();  
 for (int i = 0; i < m_NumberOfDetectors; ++i) {  
   for(int j = 0 ; j < 25 ; j++){
    Cal->AddParameter("MSX25", "D"+ NPL::itoa(i+1)+"_ENERGY","MSX25_D"+ NPL::itoa(i+1)+"_STRIP"+ NPL::itoa(j+1)+"_ENERGY");   
    Cal->AddParameter("MSX25", "D"+ NPL::itoa(i+1)+"_TIME","MSX25_D"+ NPL::itoa(i+1)+"_STRIP"+ NPL::itoa(j+1)+"_TIME");   
  }  
 }
{% endhighlight %}

#### BuildPhysicalEvent  
This method is the actual core of the class were the data analysis take place. The stategy to use here all depend on the complexity of your detector but often this method is composed of for loops that call other methods so the code is reasonably readable. The default class come with a PreTreat function, although not mandatory this method is usually present and apply calibration and threshold before doing the actual treatment.

Four our example we will modify the PreTreat first to accomodate the strip detector. 
{% highlight C++ %}
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // Energy
  for (UShort_t i = 0; i < m_EventData->GetMultEnergy(); ++i) {
    if (m_EventData->Get_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("MSX25/ENERGY_D"+NPL::itoa(m_EventData->GetE_DetectorNbr(i))+"_STRIP"+NPL::itoa(m_EventData->GetE_StripNbr(i)),m_EventData->Get_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i), Energy);
      }
    }
  }

  // Time 
  for (UShort_t i = 0; i < m_EventData->GetMultTime(); ++i) {
    Double_t Time= Cal->ApplyCalibration("MSX25/TIME_D"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)+"_STRIP"+NPL::itoa(m_EventData->GetT_StripNbr(i))),m_EventData->Get_Time(i));
    m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i), Time);
  }
{% endhighlight %}

We can now modify the BuildPhysicalEvent method to test for the matching strip number between Energy and Time information.
{% highlight C++ %}
  // apply thresholds and calibration
  PreTreat();

  // match energy and time together
  for (UShort_t e = 0; e < m_PreTreatedData->GetMultEnergy(); e++) {
    for (UShort_t t = 0; t < m_PreTreatedData->GetMultTime(); t++) {
      if (m_PreTreatedData->GetE_DetectorNbr(e) == m_PreTreatedData->GetT_DetectorNbr(t)
          && m_PreTreatedData->GetE_StripNbr(e) == m_PreTreatedData->GetT_StripNbr(t) ) {
        DetectorNumber.push_back(m_PreTreatedData->GetE_DetectorNbr(e));
        StripNumber.push_back(m_PreTreatedData->GetE_StripNbr(e)); 
        Energy.push_back(m_PreTreatedData->Get_Energy(e));
        Time.push_back(m_PreTreatedData->Get_Time(t));
      }
    }
  }
{% endhighlight %}

#### BuildSimplePhysicalEvent
This method is there to allow for simpler analysis, e.g. when used for online analysis. Some detectors require CPU intensive analysis, for example based on fitting histograms (e.g CATS, Trifoil). For these kind of analysis, having a less precise but faster version of the BuildPhysicalEvent is desirable. For most detectors the BuildSimplePhysicalEvent simply call the BuildPhysicalEvent methods.   

#### InitializeRootInputRaw 
This methods is used to connect the m_EventData pointer hold by the Physics Class to the corresponding root branch comming from either real or simulated data. The method is two fold, first it enable the branch in case it has been previously disable, for instance by using the `--disable-branch` flag (for faster analysis). Then the m_EventData pointer is connected to the tree using the SetBranchAddress methods.

The methods does not need any change.

#### InitializeRootInputPhysics   
This method is similar to the first one but for the case where the input tree hold already treated data, i.e. npanalysis has been run with the `--IP` flag. In that case the BuildPhysical method is not called.

The methods does not need any change.

#### InitializeRootOutput      
This method access the output tree via the RootOutput singleton and connect the Physics class to a new branch named after the detector.

The methods does not need any change. 

#### ClearEventPhysics()
This method is a wrapper to the Clear method require by TObject. If you add new private member to the class, like we did by adding the StripNumber vector, you need to clear the new vector in the Clear methods

In our case we need to change the Clear method, called via the ClearEventPhysics methods like so:
{% highlight C++ %}
void TMSX25Physics::Clear() {
  DetectorNumber.clear();
  StripNumber.clear();
  Energy.clear();
  Time.clear();
}
{% endhighlight %}

#### ClearEventData()    
This method is a wrapper for the calling the Clear method of the Raw Data class. It does not require any change.

#### InitSpectra
This methods create the Spectra class and initialise it. It does not require any change. 

#### FillSpectra  
This method is a wrapper for calling the Spectra class method at the right time, i.e. after analysis of the event.
It does not require any change. 

#### CheckSpectra    
This method is a wrapper for calling the Spectra class method at the right time, i.e. after filling of the Spectra.
It does not require any change. 

#### ClearSpectra
This method is a wrapper for calling the Spectra class method at the right time, i.e. after the online programm requested the spectrum to be cleared.
It does not require any change. 

#### WriteSpectra 
This method is a wrapper for calling the Spectra class method at the right time, i.e. at the end of the analysis, to write the spectrum along with the root tree, in the output root file.
It does not require any change. 

NPTool
======

NPTool, which stands for Nuclear Physics Tool, is an open source and freely
distributed data analysis and Monte Carlo simulation package for low-energy
nuclear physics experiments. The NPTool package aims to offer an unified
framework for preparing and analysing complex experiments, making an efficient
use of Geant4 and ROOT toolkits. If you wish to contribute, contact Adrien 
MATTA at a.matta@surrey.ac.uk

## Getting the code
### Using git
The recommended method to obtain the code is to use git. This ensures an easy 
way to access the updated version of the code. First make sure you have git 
installed. If not, use your package manager to get it. Then go to the directory 
where you want to install the NPTool package and do:
````
$ git clone https://github.com/adrien-matta/nptool
````
This will create the nptool folder with the latest version of NPTool.

### Downloading from Git Hub
Alternatively, you can browse the following page https://github.com/adrien-matta/nptool, 
and click the Download ZIP button on the right side of the page. Then, unzip 
the archive at the desire location


## Setup
### Requirements
NPTool components are compiled and installed using the CMake build system, 
so be sure to have a working CMake installation before starting.

In order to compile NPLib, the NPTool core libraries, ROOT 5 (tested with 5.34) 
or 6 with the libMathMore.so library should be installed. 
This is sufficient to compile NPLib and any analysis project. 

In order to compile NPSimulation, a recent installation of Geant4 (tested 
with version 9.6 and 10.1) is needed. If you want to use GDML format in 
NPTool, Geant4 should be installed with GDML support.

### Building NPLib
NPLib is the core of the NPTool package, holding most of the actual code. It is 
made of a collection of stand alone C++ classes that can be used in programs 
and macros. 

The first step is to define some environment variables. Open your .profile / 
.bashrc / .tcshrc file in your home directory and add the following line:

> source /path/to/nptool/nptool.sh

Then, restart your terminal. You should now have all aliases and environment variable properly defined and can now access the NPLib folder by using the command:
````
$ npl 
````
and you should be in the NPLib directory. 
Before the compilation of the libraries you need to run cmake to generate the Makefile. If you give no argument to cmake, all detector will be compile. If you wish to limit the number of detector to be compiled, simply specify the detector folder name (respecting the case). You can specify more than one detector:

All detector compiled:
````
$ cmake ./ 
````
OR Some detector compiled:
````
$ cmake ./ -DETLIST="DetFolder1 DetFolder2"
````

Then you compile the whole NPLib with n threads using :

````
$ make -jn install
````

If you wish to recompile with more detector:

````
$ nptool-cleaner
$ cmake ./ -DETLIST="DetFolder1 DetFolder2 ..."
$ make -jn install
````

If you have google ninja build install then you can alternatively ask cmake to generate the ninja.build file:
````
$ cmake -GNinja ./
$ ninja install
````

Compilation using Ninja is faster than using make.

###Building NPSimulation
This part of the package rely on Geant4 to perform Monte Carlo simulation. You need to first compile and configure correctly NPLib in order for NPSimulation to compile and run. The compilation is done as follow:
````
$ nps
$ cmake ./
$ make -jn install
`````

This will produce the npsimulation executable. For a detail list of the available input flag and their meaning, you can run:
````
$ npsimulation -h
````

To run a standardised test case, you can run the following command:
````
$ npsimulation -D Example1.detector -E Example1.reaction -O Example1
````
This will open the NPSimulation GUI (if you are using Qt) or the prompt terminal. In either case you can generate event using:
````
> run/beamOn/ 10000
> exit
````

This will run the 11Li(d,3He)10He->8He+n+n simulation and produce a root file located in $NPTOOL/Outputs/Simulation/Example1.root. One can have a look at the Example1.detector, located in $NPTOOL/Inputs/DetectorConfiguration, and Example1.reaction, located in $NPTOOL/Inputs/EventGenerator, to see how the input file are formated. They usually are self explenatory using easy to understand token.

You can now try to analyse this simulated tree using the associated NPAnalysis project:
````
$ npp Example1
$ cmake ./
$ make -jn
$ npanalysis -R RunToTreat.txt -O Example1
````
Because the input file are written in the simulation file along the SimulatedTree, npanalysis will automatically use those file as inputs. This will produce the analysed tree located in $NPTOOL/Outputs/Analysis/Example1.root. You can then display the result of the simulation using root:
````
$ root ShowResult.cxx
````

You should be able to see the light particle identification, the light particle kinematical line and the associated excitation energy spectrum fitted by a gaussian.
 
The Example1 input files and NPAnalysis project are simple basis that can be used to start doing your own simulations.

A few Basics: 
npsimulation and npanalysis can be run from any directory. npanalysis look in the current directory for an analysis library to load and use, if none available, it limit hte analysis to building the PhysicsTree.

One can perform quick analysis of the last simulated tree using:
````
npanalysis --last-sim 
````
Any additional flag can be used

One can run npsimulation in batch mode (with no UI) and provide instead a geant4 macro file to run
````
npsimulation -D Example1.detector -E Example1.reaction -B path/to/marcro.mac -O FileName
````

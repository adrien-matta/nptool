NPTool
======

NPTool, which stands for Nuclear Physics Tool, is an open source and freely
distributed data analysis and Monte Carlo simulation package for low-energy
nuclear physics experiments. The NPTool package aims to offer an unified
framework for preparing and analysing complex experiments, making an efficient
use of Geant4 and ROOT toolkits. If you wish to contribute, contact Adrien 
MATTA at a.matta@surrey.ac.uk

# Contents
1. [Getting the code](#getting-the-code)
  1. [Using git](#using-git)
  2. [Downloading from Git Hub](#downloading-from-git-hub)
2. [Setup](#setup)
  1. [Requirements](#requirements)
  2. [Preparing the build](#preparing-the-build)
  3. [Building NPLib](#building-nplib)
  4. [Building NPSimulation](#building-npsimulation)
3. [Benchmarks and Examples](#benchmarks-and-examples)
  1. [Benchmarks](#benchmarks)
  2. [Examples](#examples)
4. [Tricks](#tricks)


## Getting the code
### Using git
The recommended method to obtain the source code is to use git. This is an 
easy way to access the last version of the code. First make sure you have git 
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
or 6 should be installed with the libMathMore.so library. 
This is sufficient to compile NPLib and any analysis project. 

In order to compile NPSimulation, a recent installation of Geant4 (tested 
with version 9.6 and 10.1) is needed. If you want to use GDML format in 
NPTool, Geant4 should be installed with GDML support.

### Preparing the build
To set the needed environment variables, PATH and LD\_LIBRARY\_PATH, and
aliases, source the following script doing:
```
source <pathname>/nptool/nptool.sh
```
where `<pathname>` is the location where you unpacked the NPTool package.
Then, restart your terminal.

You should typically add the previous command line to your .profile, 
.bashrc or .tcshrc file.

### Building NPLib
NPLib is the core of the NPTool package, holding most of the actual code. It is 
made of a collection of stand alone C++ classes that can be used in programs 
and macros. 

First, go to the NPLib folder by using the command:
```
$ npl 
```

In order to prepare the compilation CMake must be run to generate the Makefile.
If no arguments are given to CMake, all detectors will be compiled. If you wish 
to limit the number of detectors to be compiled, specify the detector folder 
name (respecting the case). Note that more than one detector can be specified.

All detectors compiled:
````
$ cmake ./ 
````
__OR__ some detectors compiled:
````
$ cmake ./ -DETLIST="DetFolder1 DetFolder2"
````

Then, the whole NPLib can be compiled with _n_ threads using:

````
$ make -jn install
````

If you wish to recompile in order to get support for more detectors, do:

````
$ nptool-cleaner
$ cmake ./ -DETLIST="DetFolder1 DetFolder2 ..."
$ make -jn install
````

If you have google ninja build installed you can alternatively ask CMake to 
generate the ninja.build file:
````
$ cmake -GNinja ./
$ ninja install
````
Compilation using Ninja is faster than using make.

### Building NPSimulation
This part of the package relies on Geant4 to perform Monte Carlo simulation. 
NPLib needs first to be compiled and configured correctly before NPSimulation 
can be compiled. The compilation is done as follow:
```
$ nps
$ cmake ./
$ make -jn install
```

This will produce the _npsimulation_ executable. For a detailed list of the 
available input flags and their meaning, run the following command:
````
$ npsimulation -h
````

## Benchmarks and Examples
You need to download additional files to be able to run the benchmarks and
the examples. In the $NPTOOL directory, do the following:

```
$ git clone https://github.com/adrien-matta/NPData

```

### Benchmarks
Benchmarks play an important role to check the installation or upgrade integrity
of the NPTool package. They are also useful for comparing CPU performances on
different computer platforms. So far two benchmarks are included in the NPTool
package. The first one (_cats_) analyses experimental data from a beam tracker 
detector using the _npanalysis_ facility, while the second one (_gaspard_) runs 
a silicon array simulation using the _npsimulation_ facility and display some 
basics control spectra. Each benchmark produces figures that can be compared to 
the reference figures provided in NPTool. These two benchmarks cover all the core
functionalities of NPTool's framework.

The first benchmark can be run with the following commands:
```
$ cd $NPTOOL/Benchmarks/cats
$ npanalysis -D benchmark_cats.detector -C calibration.txt -R RunToTreat.txt -O benchmark_cats
```

For the second benchmark do:
```
$ cd $NPTOOL/Benchmarks/gaspard
$ npsimulation -D benchmark_gaspard.detector -E 132Sndp_benchmark.reaction -O benchmark_gaspard -B batch.mac
```

In both cases, the results can be displayed and compared to reference results 
using the following command:
```
$ root -l ShowResult.C
```

### Examples
With respect to benchmarks, examples deal with more complex analysis cases
where several detectors are present. In this context, physical information 
resulting from the combination of information from several detectors can be 
calculated.

As a standardized test case, you can run Example1 by the following command:
````
$ npsimulation -D Example1.detector -E Example1.reaction -O Example1
````
This will open the _npsimulation_ GUI (if you are using Qt) or the prompt 
terminal. In either case events can be generated using the following
command:
````
> run/beamOn/ 10000
> exit
````

This will simulate the <sup>11</sup>Li(d,<sup>3</sup>He)<sup>10</sup>He-><sup>8</sup>He+n+n 
reaction and produce an output ROOT file located in $NPTOOL/Outputs/Simulation/Example1.root. 

The `Example1.detector` file located in $NPTOOL/Inputs/DetectorConfiguration 
and the `Example1.reaction` file located in $NPTOOL/Inputs/EventGenerator are
self explanatory thanks to the use of understandable tokens.

The simulated file can be analysed using the following commands:
````
$ npp Example1
$ cmake ./
$ make -jn
$ npanalysis -R RunToTreat.txt -O Example1
````
This will produce an analysed tree located in the $NPTOOL/Outputs/Analysis/Example1.root file.
Note that since the input files needed by _npsimulation_ are stored in the 
ROOT file, _npanalysis_ use these automatically as inputs.

The results can be displayed using the command:
````
$ root -l ShowResult.C
````

You should be able to see the light particle identification, the light particle kinematical line and the associated excitation energy spectrum fitted by a gaussian.
 
The Example1 input files and NPAnalysis project are simple basis that can be used to start doing your own simulations.


## Tricks
- _npsimulation_ and _npanalysis_ facilities can be run from any directory. 
- _npanalysis_ looks in the current directory for an analysis library 
  (_libNPAnalysis_) to load and use. If not present, the analysis is limited
  to build the PhysicsTree.
- To perform a quick analysis of the last MOnte Carlo simulation, do:
````
npanalysis --last-sim 
````
Any additional flag can be used

- _npsimulation_ can be used in batch mode (with no UI) if the user provides 
a Geant4 macro file specified with the -B flag
````
npsimulation -D Example1.detector -E Example1.reaction -B path/to/macro.mac -O FileName
````

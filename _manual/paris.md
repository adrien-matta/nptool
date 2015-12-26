---
layout: manual
title: PARIS
author: Marc Labiche
permalink: /manual/paris/
manual_order: 103
show_in_nav: false
---


Introduction
============

The PARIS project is a new $$\gamma$$-ray array which aims at addressing
topical nuclear physics research programs with SPIRAL2 beams. We
describe this project here as it is developed within the simulation
GEANT4-based NPTool framework. The design and choice of the scintillator
is still matter of discussion within the collaboration, and several and
independent simulation frameworks are being used to help converging
towards a definitive solution. In NPTool, we consider an array of
phoswich detectors consisting of LaBr$$_{3}$$ and NaI (originally CsI)
scintillators, surrounded by NaI (originally CsI) shields, in an
EXOGAM-like spherical configuration. The modular aspect of the NPTool
framework allows for the PARIS array to be modelled either as a
standalone detector or coupled to other NPTool modelled detectors like
GASPARD and MUST2.The general structure of the PARIS program is based on
the structure of the GASPARD program, thus many details described here
are similar to the GASPARD document.

NPSimulation
============

Specificity of Paris
--------------------

In this released of NPTool, the PARIS $$\gamma$$-ray array consists of
phoswich detectors with squared cross section of 2”$$\times$$2”. The
phoswich first layer is a 2” long LaBr$$_{3}$$ scintillator. The second
layer is a 6” long scintillator of NaI scintillator (see
fig. [fig:phoswich1]). Originally, CsI scintillators were considered
and, for that unique reason, many comments and variable names in the
program still point at CsI scintillators. The user should be aware that
the CsI material has been replaced by NaI following prototype testing
results. All the comments and variable names will be corrected in future
a release.

![fig:Phoswitch](/images/Parismodule1.png "Fig2: A PARIS Phoswitch"){: .center-image}

The phoswich modules are grouped in 3$$\times$$3 clusters (see
fig. [fig:phoswich3by3]) or used as single modules (see
fig. [fig:phoswich1]) . All are positioned to form a spherical
configuration of an adjustable inner radius between 20.8 and 23.5 cm, as
show in fig. [fig:Paris1]. A shield made of extra NaI scintillators
surrounds the cluster and single phoswich to fill the gaps in between,
and give a spherical geometry to the full array (see fig. [fig:Paris2]).
The clusters and single phoswich modules of the Paris detector are
described in the Paris class defined in the Paris.{hh,cc} files. The
geometry of the cluster is defined in the ParisCluster.{hh,cc} files,
and the geometry of a single phoswich module is defined in the
ParisPhoswich.{hh,cc} files. The shields of CsI scintillators are
described in a different class called Shield, and defined in the
Shield.{hh,cc} files. Different shield geometries are used for the
clusters and single phoswich modules. The geometry of the cluster shield
is defined in the files ShieldClParis.{hh,cc}. The geometry of the
single phoswich module shield is defined in the files
ShieldPhParis.{hh,cc}.




![fig:Cluster](/images/Paris3by3.png "Fig1: a PARIS Cluster"){: .center-image}

Since the Paris detector and the NaI shield are registered in the
DetectorConstructor.cc file ,they are available for NPSimulation.

As for other NPTool modelled detectors, in order to manage the different
detector shapes (cluster, single module, cluster shield, single module
shield) of the Paris array, the Paris and Shield classes hold a vector
of ParisModule and ShieldModule objects, respectively, from which are
deriving all the different shapes (ParisCluster,
ParisPhoswich,ShieldClParis, and ShieldPhParis classes).

![fig:FullParis](/images/FullParisNoShield.png "Fig3: Full PARIS Assembly"){: .center-image}

![fig:FullParisShield](/images/FullParis.png "Fig4: Full PARIS Shield Assembly"){: .center-image}

Running the simulation
----------------------

As for other NPTool simulations, to run Paris simulations the following
command line should be executed:

       Simulation -D yyy.detector -E  xxx.reaction

where xxx.reaction is an input file describing the event generator and
yyy.detector is an input file describing the detector geometry. All
these input files are based on keywords and can be found in the
$NPTool/Inputs subdirectories.

### Event Generators

In the distributed version, a source of $$\gamma$$-ray either at rest or
in motion along the beam axis can be used. For an isotropic or a
collimated source at rest, the event generator file isotropic.source in
the Inputs/EventGenerator is available.An example is given in the file
gamma.source. For a source in the move, one can use the example given in
the source.reaction file.

### Detector Configurations

The keywords associated to the detector geometry file are different for
each detector. In case of the Paris detector, an example with all the
kind of detector shapes available at the moment is given below:

       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       Paris
       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       ParisCluster
         X1_Y1=       -84.5 -208   84.5
         X1_Y128=      84.5 -208   84.5
         X128_Y1=     -84.5 -208  -84.5
         X128_Y128=    84.5 -208  -84.5
         VIS=all
       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       ParisPhoswich
         X1_Y1=      -128.6063166   143.3590149   -88.301235
         X1_Y128=    -151.8764696    96.81870993 -111.571389
         X128_Y1=     -88.30122956  143.3590149  -128.606323
         X128_Y128=  -111.5713826    96.81870993  -151.876476
         VIS= all
       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       Shield
       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       ShieldClParis
         X1_Y1=       151.375  153.75   364
         X1_Y128=     151.375   91.5    364
         X128_Y1=     -84.5    153.75   364
         X128_Y128=   -84.5     91.5    364
         VIS= all
       %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       ShieldPhParis
         THETA= 54.73
         PHI=   45.02
         R=    248
         BETA=   0 0 -30
         VIS= all

The Paris and shield detectors are independent. In other words, one can
use Paris detectors with or without the NaI shields.

In order to declare a Paris detector in NPSimulation, the key word
<span>*Paris*</span> should be specified in the geometry file. It should
then be followed by other keywords concerning the different detectors
present. Such keywords available at the moment are:

-   <span>ParisCluster</span>

-   <span>ParisPhoswich</span>

Each keyword corresponds to a detector shape which has its own set of
keywords which is used to position the detector in the world volume. For
the Shield detectors, the keyword <span>*Shield*</span> should be
specified in the geometry file, followed by other keywords associated to
the different detector shield for cluster (Cl) and single phoswich (Ph):

-   <span>ShieldClParis</span>

-   <span>ShieldPhParis</span>

In principle, to position the detectors two possibilities exist. Either
the Cartesian coordinates (x,y,z) of each detector’s corner are
specified with the keywords <span>*X1\_Y1*</span>,
<span>*X1\_Y128*</span>, <span>*X128\_Y1*</span> and
<span>*X128\_Y128*</span>(case of ParisCluster, ParisPhoswich and
ShieldClParis), either the spherical coordinates of the detector’s
centre are specified with the keywords <span>*R*</span>,
<span>*THETA*</span> and <span>*PHI*</span> (case of ShieldPhParis).
While the first solution is very helpful when working with the
mechanical engineers, the second solution is useful when investigating
new geometries. However, up to now, only one solution for each type of
Paris and Shield detectors has been tested, and this is the solution
given above as an example.

Simulation output
-----------------

Again, as in other NPTool simulations, the results of the simulations
are in the ROOT format and the output file is stored in the
$NPTool/Output/Simulation directory. If the PARIS geometry input file
includes the NaI Shield, the output ROOT file contains four classes:

-   <span>TInitialConditions:</span> This class records all the
    information concerning the event generator such as the vertex of
    interaction, the angles of emitted particles in the center of mass
    and laboratory frames...

-   <span>TInteractionCoordinates:</span> Although this class appears in
    the tree, it is not in use with the PARIS array alone. Please, see
    GASPARD documentation for more details.

-   <span>TParisData:</span> This class stores the results of the
    simulation for the cluster and single phoswich detector.
    Independently of the number and shape of the detectors involved in
    the geometry, only <span>*one*</span> class is created for the whole
    PARIS detector. For each event, the energy for each layer of
    scintillators is recorded. The time, detector number and crystal
    number information will be added in future release. At the moment,
    this is equivalent of having a perfect add-back algorithm between
    crystals within each layer.

-   <span>TShieldData:</span> This class stores the results of the
    simulation for Shield detector. Independently of the number and
    shape of the detectors involved in the geometry, only
    <span>*one*</span> class is created for the whole shield detector.
    For each event, the energy is recorded. The time, detector number
    and crystal number information will be added in future release. At
    the moment, this is equivalent of having a perfect add-back
    algorithm between the shield crystals.

NPAnalysis
==========

General
-------

Only a very preliminary analysis code is provided with this release.

Paris
-----

The analysis code for the Paris array is in the
$NPTool/NPAnalysis/Paris directory. For the moment the main feature is
the photopeak efficiency as function of the $$\gamma$$-ray incoming
energy. The photopeak efficiency in each layer of the array is
calculated as well as for the NaI sheild. The full photopeak
efficiencies, with and without add-back correction between the different
layers, are also determined.

### Running the analysis

To run NPAnalysis the following command line should be executed in the
$NPTool/NPAnalysis/Paris directory:

       ./Analysis -D yyy.detector -E xxx.reaction -R RunToTreat.txt

where xxx.reaction is the input file describing the event generator used
in NPSimulation and yyy.detector is the input file describing the
detector geometry used in NPSimulation. All these input files are based
on keywords and can be found in the $NPTool/Inputs subdirectories. The
RunToTreat.txt file contains the name of the files (either from
NPSimulation or from real experiment) which should be analysed. The name
of the tree should also be specified. An example of such a file is given
here:

       TTreeName
               SimulatedTree
       RootFileName
              ../../Outputs/Simulation/myResult.root
       %       ../../Outputs/Simulation/mySimul.root

### Results of the analysis

The results of the analysis are stored in a ROOT file in the
$NPTool/Output/Analysis directory.

### Structure of the analysis

\*\*\*\*\*\*\* to be documented \*\*\*\*\*\*\*\*\*

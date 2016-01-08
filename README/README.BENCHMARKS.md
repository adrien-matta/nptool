README file for running NPTool benchmarks
=========================================

Contents
--------

1. [Why benchmarks?](#why-benchmarks?)
2. [Prerequisities](#prerequisities)
3. [CATS: a multi-wire low-pressure tracking system](#cats:-a-multi-wire-low-pressure-tracking-system)
4. [GASPARD: a silicon array for particle spectroscopy](#gaspard:-a-silicon-array-for-particle-spectroscopy)



## Why benchmarks?
Benchmarks play an important role to check the installation or upgrade integrity
of the NPTool package. They are also useful for comparing CPU performances on
different computer platforms. So far two benchmarks are included in the NPTool
package. The first one analyses experimental data from a beam tracker detector
using the _npanalysis_ facility, while the second one runs a silicon array
simulation using the _npsimulation_ facility and display some basics
control spectra. Each benchmark produces figures that can be compared to the
reference figures provided in NPTool. These two benchmarks cover all the core
functionalities of NPTool's framework.



## Prerequisities
You need to download additional files to be able to run the benchmarks. In the
$NPTOOL directory, do the following:
```
$ git clone https://github.com/adrien-matta/NPData

```



## CATS: a multi-wire low-pressure tracking system
[CATS](http://www.sciencedirect.com/science/article/pii/S0168900299003800
"S. Ottini-Hustache et al., NIMA 431(3), 476-484 (1999)")
consists of two low pressure multi-wire proportional chambers with one plane
of anode wires placed between two cathode planes, respectively segmented into 
28 vertical or horizontal strips. It was designed to provide event-by-event 
particle tracking in experiments with radioactive beams at GANIL.

The benchmark can be run from the $NPTOOL/Benchmarks/cats directory with the 
following command:
```
$  npanalysis -D benchmark_cats.detector -C calibration.txt -R RunToTreat.txt -O benchmark_cats
```

To display the results run the command:
```
$ root -l ShowResults.C
```

Alternatively to the two previous commands, you can run the `benchmark_cats.sh` 
script which will run the analysis and display the results.

Experimental data for the present benchmark comes from the interaction of a
<sup>58</sup>Ni ion beam at 75 MeV/u in a CATS detector. A mask with a known
pattern was inserted in front of it in order to calibrate the position
reconstruction. 
The results that you should obtain are shown in the following Figure, where
the mask pattern including circular holes of different diameters is clearly 
observed.
<img src="../Benchmarks/cats/cats_ref.png" width="384">

Aside from testing the position reconstruction for the CATS detector, this 
benchmark validates the calibration manager facility and the _npanalysis_ 
utility.


### GASPARD: a silicon array for particle spectroscopy
[GASPARD](http://www.sciencedirect.com/science/article/pii/S0168583X13006113
"D. Beaumel, NIMB 317, 661-663 (2013)") aims to be a next generation silicon 
array with a coverage close to 4pi which can be completely integrated 
with new generation gamma-ray detectors such as 
[AGATA](http://iopscience.iop.org/article/10.1088/1742-6596/606/1/012017/pdf
"J. Simpson et al., J. Phys.: Conf. Ser., 606, 012017 (2015)") or
[PARIS](http://www.actaphys.uj.edu.pl/fulltext?series=Reg&vol=40&page=565
"A. Maj et al., Acta Physica Polonica B40, 565 (2009)"). While the physics 
case tackled by GASPARD is broad, it has been designed having in mind the 
study of nuclear structure by direct reactions.

The benchmark can be run from the $NPTOOL/Benchmarks/gaspard directory with the 
following command:
```
$ npsimulation -D benchmark_gaspard.detector -E 132Sndp_benchmark.reaction -O benchmark_gaspard -B batch.mac
```

To display the results run the command:
```
$ root -l ShowResults.C
```

Alternatively to the two previous commands, you can run the `benchmark_gaspard.sh` 
script which will run the Monte Carlo simulation and display the results.



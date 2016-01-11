---
layout: manual 
title: Benchmarks and Examples 
permalink: /manual/benchmarks-examples/
author: Adrien Matta
manual_order: 1 
show_in_nav: false
---
You need to download additional files to be able to run the benchmarks and
the examples. In the $NPTOOL directory, do the following:
{% highlight bash %}
$ git clone https://github.com/adrien-matta/NPData
{% endhighlight %}

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
{% highlight bash %}
$ cd $NPTOOL/Benchmarks/cats
$ npanalysis -D benchmark_cats.detector -C calibration.txt -R RunToTreat.txt -O benchmark_cats
{% endhighlight %}

For the second benchmark do:
{% highlight bash %}
$ cd $NPTOOL/Benchmarks/gaspard
$ npsimulation -D benchmark_gaspard.detector -E 132Sndp_benchmark.reaction -O benchmark_gaspard -B batch.mac
{% endhighlight %}

In both cases, the results can be displayed and compared to reference results
using the following command:
{% highlight bash %}
$ root -l ShowResult.C
{% endhighlight %}
```

### Examples
With respect to benchmarks, examples deal with more complex analysis cases
where several detectors are present. In this context, physical information
resulting from the combination of information from several detectors can be
calculated.

As a standardized test case, you can run Example1 by the following command:
{% highlight bash %}
$ npsimulation -D Example1.detector -E Example1.reaction -O Example1
{% endhighlight %}
This will open the _npsimulation_ GUI (if you are using Qt) or the prompt
terminal. In either case events can be generated using the following
command:
{% highlight bash %}
> run/beamOn/ 10000
> exit
{% endhighlight %}

This will simulate the <sup>11</sup>Li(d,<sup>3</sup>He)<sup>10</sup>He-><sup>8H</sup>e+n+n
reaction and produce an output ROOT file located in $NPTOOL/Outputs/Simulation/Example1.root.

The `Example1.detector` file located in $NPTOOL/Inputs/DetectorConfiguration
and the `Example1.reaction` file located in $NPTOOL/Inputs/EventGenerator are
self explanatory thanks to the use of understandable tokens.

The simulated file can be analysed using the following commands:
{% highlight bash %}
$ npp Example1
$ cmake ./
$ make -jn
$ npanalysis -R RunToTreat.txt -O Example1
{% endhighlight %}
This will produce an analysed tree located in the $NPTOOL/Outputs/Analysis/Example1.root file.
Note that since the input files needed by _npsimulation_ are stored in the
ROOT file, _npanalysis_ use these automatically as inputs.

The results can be displayed using the command:
{% highlight bash %}
$ root -l ShowResults.C
{% endhighlight %}

You should be able to see the light particle identification, the light particle kinematical line and the associated excitation energy spectrum fitted by a gaussian.

The Example1 input files and NPAnalysis project are simple basis that can be used to start doing your own simulations.


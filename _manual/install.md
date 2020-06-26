---
layout: manual 
title: Install 
permalink: /manual/install/
author: Adrien Matta
manual_order: 0
show_in_nav: false
---
## Getting the code

### Using git
The recommended method to obtain the source code is to use git. This is an
easy way to access the last version of the code. First make sure you have git
installed. If not, use your package manager to get it. Then go to the directory
where you want to install the NPTool package and do:
{% highlight bash %}
$ git clone https://gitlab.in2p3.fr/np/nptool
{% endhighlight %}
This will create the nptool folder with the latest version of NPTool.

### Downloading from Git Hub
Alternatively, you can browse the following page https://gitlab.in2p3.fr/np/nptool,
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
{% highlight bash %}
source <pathname>/nptool/nptool.sh
{% endhighlight %}
where `<pathname>` is the location where you unpacked the NPTool package.
Then, restart your terminal.

You should typically add the previous command line to your .profile,
.bashrc or .tcshrc file.

### Building NPLib
NPLib is the core of the NPTool package, holding most of the actual code. It is
made of a collection of stand alone C++ classes that can be used in programs
and macros.

First, go to the NPLib folder by using the command:
{% highlight bash %}
$ npl
{% endhighlight %}

In order to prepare the compilation CMake must be run to generate the Makefile.
If no arguments are given to CMake, all detectors will be compiled. If you wish
to limit the number of detectors to be compiled, specify the detector folder
name (respecting the case). Note that more than one detector can be specified.

All detectors compiled:
{% highlight bash %}
$ cmake ./
{% endhighlight %}
__OR__ some detectors compiled:
{% highlight bash %}
$ cmake ./ -DETLIST="DetFolder1 DetFolder2"
{% endhighlight %}

Then, the whole NPLib can be compiled with _n_ threads using:

{% highlight bash %}
$ make -jn install
{% endhighlight %}

If you wish to recompile in order to get support for more detectors, do:

{% highlight bash %}
$ nptool-cleaner
$ cmake ./ -DETLIST="DetFolder1 DetFolder2 ..."
$ make -jn install
{% endhighlight %}

If you have google ninja build installed you can alternatively ask CMake to
generate the ninja.build file:
{% highlight bash %}
$ cmake -GNinja ./
$ ninja install
{% endhighlight %}
Compilation using Ninja is faster than using make.

### Root configuration
In order for Root to correctly load the nptool headers and library, and so be able to use nptool objects within interactive root macro, you need to add a couple of line to the ~/.rootlogon.C file. You may or may note already have such a file, in any case, a bash scripts is there to either create or modify ~/.rootlogon.C. To run it, simply use the following commands:

{% highlight bash %}
$ npl
$ ./scripts/RootLogon.sh
{% endhighlight %}

You can then test that everything is working correctly by doing:
{% highlight bash %}
$ root 
> NPL::Reaction r("28Si(d,p)29Si@280")
> r.GetKinematicLine3()->Draw("ac")
{% endhighlight %}

This should display a _(d,p)_ kinematic line and no error message display.

### Building NPSimulation
This part of the package relies on Geant4 to perform Monte Carlo simulation.
NPLib needs first to be compiled and configured correctly before NPSimulation
can be compiled. The compilation is done as follow:
{% highlight bash %}
$ nps
$ cmake ./
$ make -jn install
{% endhighlight %}

This will produce the _npsimulation_ executable. For a detailed list of the
available input flags and their meaning, run the following command:
{% highlight bash %}
$ npsimulation -h
{% endhighlight %}


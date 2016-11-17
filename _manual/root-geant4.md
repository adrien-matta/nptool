---
layout: manual
title: "Installing Root and Geant4"
permalink: /manual/root-geant4/
author: Adrien Matta
show_in_nav: false
manual_order: 1000
---

This page is still underdevellopment, for reference ony

## Introduction

This page is given for information only as the nptool collaboration is not part of either Root or Geant4, and does not develop or maintain those packages. Rather it is there to help you going through the install process to get an environment allowing you to use nptool.

## cmake, make

In order to install nptool you will need the cmake utility as well as make and/or ninja. Make is standard on most linux distribution and come as part of the devellopers CLI tool on Mac OS X.

Installing CLI devellopers tools on Mac OS X:
{% highlight console %}
$ xcode-select install 
{% endhighlight %}

If cmake is not present after that: 
{% highlight console %}
$ sudo port install cmake
{% endhighlight %}


## GSL (no longer needed)

nptool required the libmathmore from ROOT, which is build only when the GNU Scientific Library (GSL) is present. This package can be installed easely via package manager:

On Ubuntu and Debian :
{% highlight console %}
$ sudo apt-get install gsl-bin
$ sudo apt-get inatall libgsl0-dev
{% endhighlight %}

On Fedora (> 21) :
{% highlight console %}
$ sudo dnf install gsl
{% endhighlight %}

On Fedora (< 22) :
{% highlight console %}
$ sudo yum install gsl
{% endhighlight %}

On Mac Os X using mac port:
{% highlight console %}
$ sudo port install gsl
{% endhighlight %}

On Mac Os X using homebrew:
{% highlight console %}
$ sudo brew install gsl
{% endhighlight %}

Alternatively, if you do not have sudo credential you can download and compile the source.

## Root

Root sources are available and it is the safest way to install the analysis framework on your system. If however you are in a situation were installing from source is difficult (on virtual machine or to setup test) you can alternatively install the debian package (for both Debian and Ubuntu).

### From source (preferred method)

Installation from source is the safiest and more efficient way of install Root. While the compilation process take some time (up to 4h on old machine), it produce a much more efficient root version for your system. It also allow you to have multiple Root version installed and switch between them, making it easy and safe to upgrade at any time. 

nptool is compatible with both Root 5 and 6. We highly recommend to use the latest recommended version available. Moreover Root 6 offer a lot of improvement that nptool is taking advantage of, making your user experience even better.

Once the source downloaded, one need to decide where to install Root. Some prefere to install in there home directory (not requiring sudo credential), other prefer to install it in /usr/local directory. In any case we recommend to keep the version number in the directory name, as a mean to identify different version.

The cmake installation requier an out-of-source compilation, we will therefore create a temporary folder to hold the build, and remove it at the end.

For newer version of nptool mathmore is no longer required. 

{% highlight console %}
$ cd /path/to/folder
$ tar -xf /path/to/root.xxx.tar.gz
$ mkdir root.xxx_build
$ mkdir root.xxx_install
$ cd root.xxx_build
$ cmake -DCMAKE_INSTALL_PREFIX=/path/to/folder/root.xxx_install -Dmathmore=ON -Dminuit2=ON ../root.xxx
$ make -j2 
$ make install
$ cd ../
$ rm -Rf root.xxx_build
$ rm -Rf root.xxx
$ source /path/to/folder/root.xxx_install/bin/thisroot.sh
{% endhighlight %}

The last line whould be added to your .bashrc/.shrc/.cshrc/.tcshrc/.profile file as well.

### Via debian package

On Debian / Ubuntu
{% highlight console %}
$ sudo add-apt-repository -y "deb http://archive.ubuntu.com/ubuntu/ trusty main universe"
$ sudo apt-get update -y
$ sudo apt-get install libexpat1-dev libexpat1 -y
$ sudo apt-get install root-system -y 
{% endhighlight %}

## Geant4

### From source (preferred method)
The following commands install Geant4 from source. QT and GDML are optional and requiered QT4 or QT5 (package libqt4-dev or qtdeclarative5-dev) and  libxerces-c (package libxerces-c-dev , libxerces-cX.X) to be installed.

We however strongly recommand to use the QT interface of npsimulation, as it provide the best user experience. GDML is required for some detectors which load there geometry from GDML file (e.g. Agata, T-Rex, Miniball) as well as exporting geometry to GDML file.

You also need to compile Geant4 with the OpenGL driver in order to benefit from the visualization. This is usually done automatically as the OpenGL headers are installed on most standard distribution, if not you can try to install the mesa-common-dev package to get the GL/gl.h header file.

{% highlight console %}
$ cd /path/to/folder
$ tar -xf /path/to/geant4.xxx.tar.gz
$ mkdir geant4.xxx_build
$ mkdir geant4.xxx_install
$ cd geant4.xxx_build
$ cmake -DCMAKE_INSTALL_PREFIX=/path/to/folder/geant4.xxx_install -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_GDML=ON ../geant4.xxx
$ make -j2 
$ make install
$ cd ../
$ rm -Rf geant4.xxx_build
$ rm -Rf geant4.xxx
$ source /path/to/folder/geant4.xxx_install/share/Geant4-xxx/geant4make/geant4make.sh
{% endhighlight %}
The last line whould be added to your .bashrc/.shrc/.cshrc/.tcshrc/.profile file as well.

### Precompiled download

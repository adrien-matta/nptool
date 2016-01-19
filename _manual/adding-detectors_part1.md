---
layout: manual 
title: Adding a new detector with nptool-wizard 
permalink: /manual/adding-detectors_part1/
author: Adrien Matta
manual_order: 10 
show_in_nav: false 
---

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



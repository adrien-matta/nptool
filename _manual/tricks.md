---
layout: manual 
title: Tricks
permalink: /manual/tricks/
author: Adrien MATTA
manual_order: 0
show_in_nav: false
---
## A few Basics: 

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

npsimulation and npanalysis can be run from any directory. npanalysis look in the current directory for an analysis library to load and use, if none available, it limits the analysis to building the PhysicsTree.

One can perform quick analysis of the last simulated tree using:

{% highlight bash %}
npanalysis --last-sim 
{% endhighlight %}
Any additional flag can be used

One can run npsimulation in batch mode (with no UI) and provide instead a geant4 macro file to run

{% highlight bash %}
npsimulation -D Example1.detector -E Example1.reaction -B path/to/marcro.mac -O FileName
{% endhighlight %}

You chain simulation, analysis and display of the result in a single command using the AND operator '&&' :
{% highlight bash %}
npsimulation -D Example1.detector -E Example1.reaction -B path/to/marcro.mac -O FileName && npanalysis --last-sim -O FileName && root MyMacro.cxx
{% endhighlight %}




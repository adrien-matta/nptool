---
layout: manual 
title: Tricks
permalink: /manual/tricks/
author: Adrien MATTA
manual_order: 3 
show_in_nav: false
---
## A few basics

- _npsimulation_ and _npanalysis_ facilities can be run from any directory.
- _npanalysis_ looks in the current directory for an analysis library
  (_libNPAnalysis_) to load and use. If not present, the analysis is limited
  to build the PhysicsTree.
- To perform a quick analysis of the last Monte Carlo simulation, do:
{% highlight bash %}
npanalysis --last-sim
{% endhighlight %}

Any additional flag can be used

- _npsimulation_ can be used in batch mode (with no UI) if the user provides
a Geant4 macro file specified with the -B flag
{% highlight bash %}
npsimulation -D Example1.detector -E Example1.reaction -B path/to/macro.mac -O FileName
{% endhighlight %}

- _npsimulation_, _npanalysis_ and the results display can be chained in a 
single command line using the '&&' operator:
{% highlight bash %}
npsimulation -D Example1.detector -E Example1.reaction -B path/to/marcro.mac -O FileName && npanalysis --last-sim -O FileName && root MyMacro.C
{% endhighlight %}


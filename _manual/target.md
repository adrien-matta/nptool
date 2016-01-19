---
layout: manual
title: Target
author: Adrien Matta 
permalink: /manual/target/
manual_order: 100
show_in_nav: false
---

Targets in nptool
==================================
nptool offers a full facility to simulate various types of targets. Having a 
target is mandatory for a simulation to run. The facility allows for solid 
targets (foil or bulk volume) as well as cryogenic targets with variable 
density values and entrance/exit windows. 

There is no limitation of target thickness, although it is the user 
responsability to check that the target volume do not overlap with any other 
volume in the detector set-up.

When _npanalysis_ is used the target position can be recovered and used as the
reference point for every position measurement (e.g. with DSDSD detector).

Description of geometry file for solid target
==================================
The geometry file includes a _GeneralTarget_ token indicating that a 
target is defined. The relevant part of the geometry input file corresponding 
to a standard solid target is given below:
{% highlight text %}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GeneralTarget
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Target
	THICKNESS= 10
	RADIUS=	20
	MATERIAL= CD2
	ANGLE= 0
	X= 0
	Y= 0
	Z= 0
   NBLAYERS= 10
{% endhighlight %}

The thickness of the foil is given in micro-meter. The (X,Y,Z) coordinates of 
the target are given in millimeter and the angle of the target is given in 
degree and describes the rotation around the Y axis. The material of the foil 
is taken from the NPS::MaterialManager. If the material is not found in the 
library, _npsimulation_ will issue a warning and use vacuum as the material.

The NBLAYERS token is optional and indicates the number of layers to be used 
to calculate the beam energy loss in the target in order to obtain the final 
inteaction energy. The default value is 5.


Description of geometry file for cryogenic target
============================
In case of cryogenic targets the relevant part of the geometry input file
is given below:
{% highlight text %}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GeneralTarget
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CryoTarget
	THICKNESS= 3
	RADIUS=	45
	DENSITY= 1
   MATERIAL= D2	
	WINDOWSTHICKNESS= 0
	WINDOWSMATERIAL= Mylar
	X= 0
	Y= 0
	Z= 0			
   NBLAYERS= 10
{% endhighlight %}

The thickness of the target indicates the thickness of cryogenic material 
and is given in micrometer. The windows thickness is also given in micrometer.
Both cryogenic and windows materials are searched in the NPS::MaterialManager.
If not found, vacuum is used as default material after a warning is issued.
Other tokens have same meaning as in the solid target case.

Note that if the density given is 0 then the default density for the considered
material will be used.

__Important note__ the DENSITY token should be placed before the MATERIAL 
token.



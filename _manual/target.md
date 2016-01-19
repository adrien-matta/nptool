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
nptool offer a full facility to simulated various type of target. Having a target is mandatory for a simulation to run. The facility allow for solid target (foil or bulk volume) as well as cryogenic target with variable density of the traget volume and entrance/exit windows. 

There is no limitation of target thickness, althought it is the user responsability to check that the target volume do not interfer with any other volume in the detector set-up.

The target position can be recover from within the analysis and be used as the reference point for every position measurement (e.g. with DSSD detector).

Description of geometry file for solid target
============================

An input files for a standard solid target foil is the following:
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

The thickness of the foil is given in micro-meter. The (X,Y,Z) coordinates of the target are given in millimeter and the Angle of the target is given in degree and describe the rotation around the Y axis. The material of the foil is taken from the NPS::MaterialManager, if the material is not found in the library, the programme will issue a warning and use vacuum as the material.

The NBLAYERS token is optional and indicate the number of layer to be used to calculate the beam energy loss in the target to obtain the final inteaction energy. The default value is 5.

Description of geometry file for cryogenic target
============================
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

The thickness of the target indicate the thickness of cryogenic material and is given in micrometer. The windows thickness is given in millimeter as well. Both cryogenic and windows material are look for in NPS::MaterialManager and if not found the default material vacuum is used after a warning is issued.

NB: if the density given is 0 then the default density for this material is used.
NNB: the density token should be place before the Material token.

The NBLAYERS token is optional and indicate the number of layer to be used to calculate the beam energy loss in the target to obtain the final inteaction energy. The default value is 5.


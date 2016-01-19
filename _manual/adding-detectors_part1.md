---
layout: manual 
title: Adding a new detector with nptool-wizard 
permalink: /manual/adding-detectors/
author: Adrien Matta
manual_order: 10 
show_in_nav: false 
---
This tutorial will present how to add a new detector using the _nptool-wizard_
utility available in the _nptool_ package. `nptool-wizard` is a simple bash 
script that will first ask you a few information about the detector you want 
to create and the author. It will then create the needed files in the NPLib
and NPSimulation directories. These files correspond to the basic case of a
scintillator detector (rectangular or cylindrical) where energy and time
are recorded. For a more elaborated detector, it is recommended to first follow
the present tutorial and then to go to the next one. 


## Running nptool-wizard
`nptool-wizard` is available as soon as you prepare the compilation of NPLib, 
i.e., after the `cmake ./` command is issued. However, as a good practice, it
is recommended to compile __and__ install NPLib as a prerequirity.

Once this is done, you can call the _nptool-wizard_ utility doing:
{% highlight bash %}
$ nptool-wizard
{% endhighlight %}
The script will prompt you to give a name to your new detector. Then it will
ask your name and email which will be added in the header of each created file
so that comunication between people maintaining the package is easier. If the
detector name you have chosen already exist an error will be issued. This 
behaviour can be override using the -f flag, which will create the detector
regardless to the preexisting detector files and folders.

`nptool-wizard` will also give a list of the files created and ask whether you
want to add all these files to the Git repository. For the purpose of this 
tutorial, say `no`. If you say `yes` a `git add` command will be run on every 
newly created file, allowing for a fast and easy commit of your new detector. 


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



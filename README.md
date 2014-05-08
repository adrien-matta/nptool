NPTOOL
======

The NPTool package is based on Root and Geant4. It is aimed to assist experimental nuclear physicist is performing complex data analysis and simulations. NPTool philosophy is to provide an open framework for the community, hence license under GPL2. If you wish to contribute, contact Adrien MATTA at a.matta@surrey.ac.uk

#Getting the code
### Using git
The recommended method to obtain the code is to use git. This insure an easy way to access the update version of the code. First make sure you got git installed, if not, just use your package manager to get it. Then go to the directory where you want to install the nptool folder and type:
````
$ git clone https://github.com/adrien-matta/nptool
````
This will create the nptool folder with the latest version of nptool.

### Downloading from Git Hub
Alternatively you can browse the following page https://github.com/adrien-matta/nptool, and click the Download ZIP button on the right side of the page. Simply unzip the archive at the desire location

#Setup

###Requirements
In order to compile the core libraries NPLib, you will need ROOT to be installed with the libMathMore.so library. This is sufficient to compile NPLib and any analysis project. In order to compile NPSimulation, you will need a recent installation of Geant4.

###Building NPLib
NPLib is the core of the NPTool package, holding most of the actual code. It is made of a collection of stand alone C++ classes that can be used in programs and macro. The first step is to define the variable of environment that the system needs. For this open your .profile / .bashrc / .tcshrc file in your home directory and add the following line:

> source /path/to/nptool/nptool.sh

Restart your terminal. You should now have all aliases and environment variable properly defined and can now access the NPLib folder by using the command:
````
$ NPL
````
and you should be in the NPLib directory. 
Before the compilation of the libraries you need to specify the detector you plan to use by calling the configure script. If you give no argument to configure, all detector will be compile. If you wish to limit the number of detector to be compiled, simply specify the detector name in lowercase (e.g. MUST2 -> must2). You can specify more than one detector:

````
$ ./configure must2 sharc plastic riken
````

You have to performe the configure operation only once, then you compile the whole NPLib with:

````
$ make -jn
````

where you replace n is the number of core to be used in the compilation.

If you wish to compile only one subdirectory, simply add the name of the directory in lower case, e.g. for compiling the Physics directory:

````
$ make -jn physics
````

In order to load the nptool libraries when you launch root, you need to add a few line to your ~/.rootlogon.C file. NPTool can do that safely for you using a script. To launch the script simply type:
````
$ make rootlogon
````
Depending on the existence of the .rootlogon.C file in your home directory, the output of the script might be different. If you already have a .rootlogon.C file that make reference to nptool then the script will leave it unchanged.

###Building NPSimulation
This part of the package rely on Geant4 to perform Monte Carlo simulation. You need to first compile and configure correctly NPLib in order for NPSimulation to compile and run. The compilation is done as follow:
````
$ NPS
$ make -jn
`````
This should produce the NPSimulation executable, you can check this by running the help display
````
$ NPSimulation -h
````


THIS FILE IS UNDER DEVELOPMENT

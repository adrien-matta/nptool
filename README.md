NPTOOL
======

The NPTool package is based on Root and Geant4. It is aimed to assist experimental nuclear physicist is performing complex data analysis and simulations. NPTool phylosophy is to provide an open framework for the community, hence liscence under GPL2. If you wish to contribute, contact Adrien MATTA at a.matta@surrey.ac.uk

#Getting the code

### Using git
The recommanded method to obtain the code is to use git. This insure an easy way to access the update version of the code. First make sure you got git installed, if not, just use your package manager to get it. Then go to the directory where you want to install the nptool folder and type:

>git clone https://github.com/adrien-matta/nptool

this will create the nptool folder with the latest version of nptool.

### Downloading from Git Hub
Alternatively you can browse the following page https://github.com/adrien-matta/nptool, and click the Download ZIP button on the right side of the page. Simply unzip the archive at the desire location

#Setup

###Requierement
In order to compile the core libraries NPLib, you will need ROOT to be installed with the libMathMore.so librairy. This is suffisant to compile NPLib and any analysis project. In order to compile NPSimulation, you will need a recent installation of Geant4.

###Building
The first step is to define the variable of environment that the system needs. For this open your .profile / .bashrc / .tcshrc file in your home directory and add the following line:
>source /path/to/nptool/nptool.sh

Restart your terminal. You should now have all aliases and environment variable properly defined. Type:
NPL

and you shoudl be in the NPLib directory. 
Before the compilation of the libraries you need to specify the detector you plan to use by calling the configure script. If you give no argument to configure, all detector will be compile. If you wish to limit the number of detector to be compiled, simply specify the detector name in lowercase (e.g. MUST2 -> must2). You can specify more than one detector:

>./configure must2 sharc plastic riken

Then you can compile NPLib by calling
>make -jn

where n is the number of core to be used in the compilation.



THIS FILE IS UNDER DEVELLOPMENT

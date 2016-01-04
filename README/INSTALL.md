Installation guide for the NPTool package
=========================================


Contents
--------
   1. Getting the source
     1. Using Git
     2. Using a tarball
   2. Getting ready to build
   3. Build
     1. NPLib
     2. NPSimulation


##  1. Getting the source
### 1.1 Using Git
The recommended method to download the source code is to use git. This ensures 
an easy way to access to the last version of the code. First make sure you have
git installed. Then go to the directory where you want to install the NPTool 
package and type:
   ````
   $ git clone https://github.com/adrien-matta/nptool
   ````
This will create the nptool directory holding the latest NPTool version.

### 1.2 Using a tarball
Alternatively, you can download a tarball at the following page 
https://github.com/adrien-matta/nptool. Click on the Download ZIP button and unzip 
the archive in the directory where you want to install the NPTool package.

      

## 2. Getting ready to build
The NPTool package depends on 4 third-party softwares:
	- Geant4 (http://geant4.web.cern.ch/geant4)
	- ROOT (http://root.cern.ch) with libMathMore support
	- CLHEP (http://www.cern.ch/clhep)
	- GSL (http://www.gnu.org/software/gsl)

Be sure to source the `<pathname>/nptool/nptool.sh` script in order to define
environment variables and aliases. Then, go to the $NPTOOL/NPLib directory:
   ```
   $ npl
   ```


##  3. Build
### 3.1. NPLib
The NPLib directory is the core of the NPTool package where detectors are defined 
in the $NPTOOL/NPLib/Detector directory. 

In order to prepare the compilation for a subset of detectors, do:
   ```
   $ cmake ./ -DETLIST="DetFolder1 DetFolder2"
   ```
If the DETLIST flag is not present, all detectors are taken into account.

Now, compile with *n* threads and install:
   ```
   $ make -jn install
   ```

If you want to add suppport for more detectors, do the following:
   ```
   $ nptool-cleaner
   $ cmake ./ -DETLIST="DetFolder1 DetFolder2 ..."
   $ make -jn install
   ```

If you have google ninja build installed then you can alternatively ask cmake 
to generate the ninja.build file:
   ```
   $ cmake -GNinja ./
   $ ninja install
   ```

### 3.2 NPSimulation
The NPSimulation directory holds all the Monte Carlo related files needed by
Geant4.

To compile this module, do the following:
   ```
   $ nps
   $ cmake ./
   $ make -jn install
   ```

In order to check whether the compilation process was successful or not, do:
   ```
   npsimulation -h
   ```
This will display a list of input flags and their corresponding meaning.

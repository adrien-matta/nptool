#!/usr/bin/env tcsh
  printf "WARNING: Using a shell that does not comply with POSIX is bad practice. Switch to bash !\n"

# endifnd script path
    set rootdir=`dirname $0`       # may be relative path
    set scriptpath=`cd $rootdir && pwd`    # ensure absolute path
#echo $SCRIPTPATH
# export NPTOOL environment variable

setenv NPTOOL $scriptpath
printenv NPTOOL

set NPARCH=`uname`

# mac os x case
if ( "${NPARCH}" == "Darwin" ) then
  setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:${NPTOOL}/NPLib/lib
  setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:${NPTOOL}/NPSimulation/lib
else 
  setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH":$NPTOOL/NPLib/lib"
  setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH":$NPTOOL/NPSimulation/lib"
endif

setenv PATH $PATH":$NPTOOL/NPLib/bin"
setenv PATH $PATH":$NPTOOL/NPSimulation"

alias npt cd `printenv NPTOOL`
alias npl cd `printenv NPTOOL`/NPLib
alias nps cd `printenv NPTOOL`/NPSimulation

alias npa_not_supported npa is now longer supported, use npp instead
alias npa echo $npa_not_supported

alias npp cd `printenv NPTOOL`/Projects

setenv Geant4_DIR $G4LIB
setenv NPLib_DIR $NPTOOL/NPLib

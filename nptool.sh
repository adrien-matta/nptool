#!bin/bash

# source this file to setup your NPTOOL installation
CUR_DIR="$PWD" 
SCRIPTFILE=$0 
if [ "${SCRIPTFILE}" = "-bash" ] ; then  
  SCRIPTFILE=${BASH_ARGV[0]} 
elif [ "${SCRIPTFILE}" = "bash" ] ; then  
  SCRIPTFILE=${BASH_ARGV[0]} 
fi 

SCRIPTPATH="${SCRIPTFILE}" 

export NPTOOL=$(dirname $SCRIPTPATH)
export NPLIB=$NPTOOL/NPLib

NPARCH=$(uname)
# mac os x case
if [ "${NPARCH}" = "Darwin" ] ; 
then
  export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$NPTOOL/NPLib/lib
  export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$NPTOOL/NPSimulation/lib
else 
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NPTOOL/NPLib/lib
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NPTOOL/NPSimulation/lib
fi

export PATH=$PATH:$NPLIB/bin
export PATH=$PATH:$NPTOOL/NPSimulation/bin

alias npt='cd $NPTOOL'  
alias npl='cd $NPTOOL/NPLib'  
alias nps='cd $NPTOOL/NPSimulation'
alias npa='cd $NPTOOL/NPAnalysis'

export Geant4_DIR=$G4LIB
export NPLib_DIR=$NPTOOL/NPLib

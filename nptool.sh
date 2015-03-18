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
else 
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NPTOOL/NPLib/lib
fi

export PATH=$PATH:$NPLIB/bin

alias NPSimulation='$NPTOOL/NPSimulation/bin/$G4SYSTEM/Simulation'
alias NPAnalysis='$NPANA/Analysis'

alias NPT='cd $NPTOOL'  
alias NPL='cd $NPTOOL/NPLib'  
alias NPS='cd $NPTOOL/NPSimulation'
alias NPA='cd $NPTOOL/NPAnalysis'

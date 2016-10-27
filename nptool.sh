#!/bin/bash

CMD="export"
SEP="="

# test if export is supported
export 1>/dev/null 2>/dev/null
if [ "${?}" == 0 ]; then
  CMD="export"
  SEP="="
else
  setenv 1>/dev/null 2>/dev/null
  if [ "${?} == 0" ]; then
  CMD="setenv"
  SEP=" "
  else
  echo "Neither setenv nor export found!"
  fi
fi 

# find script path
if [ -n "$ZSH_VERSION" ]; then
   SCRIPTPATH="$( cd "$( dirname "${(%):-%x}" )" && pwd )"
elif [ -n "$TCSH_VERSION" ]; then
   SCRIPTPATH="$( cd "$( dirname "$0" )" && pwd )"
elif [ -n "$BASH_VERSION" ]; then
   SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
else
   echo "neither bash or zsh is used, abort"
   exit 1
fi

# export NPTOOL environment variable
${CMD} NPTOOL${SEP}$SCRIPTPATH

NPARCH=$(uname)
# mac os x case
if [ "${NPARCH}" = "Darwin" ] ; 
then
  ${CMD} DYLD_LIBRARY_PATH${SEP}$DYLD_LIBRARY_PATH:$NPTOOL/NPLib/lib
  ${CMD} DYLD_LIBRARY_PATH${SEP}$DYLD_LIBRARY_PATH:$NPTOOL/NPSimulation/lib
else 
  ${CMD} LD_LIBRARY_PATH${SEP}$LD_LIBRARY_PATH:$NPTOOL/NPLib/lib
  ${CMD} LD_LIBRARY_PATH${SEP}$LD_LIBRARY_PATH:$NPTOOL/NPSimulation/lib
fi

${CMD} PATH=$PATH:$NPTOOL/NPLib/bin
${CMD} PATH=$PATH:$NPTOOL/NPSimulation/bin

alias npt='cd $NPTOOL'  
alias npl='cd $NPTOOL/NPLib'  
alias nps='cd $NPTOOL/NPSimulation'
${CMD} npa_not_supported='npa is now longer supported, use npp instead'
alias npa='echo $npa_not_supported'

function npp {
  if [[ $1 == *"Example"* ]]
  then
    cd $NPTOOL/Examples/$1 
  else
    cd $NPTOOL/Projects/$1
  fi
}


${CMD} Geant4_DIR${SEP}$G4LIB
${CMD} NPLib_DIR${SEP}$NPTOOL/NPLib

#!/bin/bash

# find script path
if [ -n "$ZSH_VERSION" ]; then
   SCRIPTPATH="$( cd "$( dirname "${(%):-%x}" )" && pwd )"
elif [ -n "$BASH_VERSION" ]; then
   SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
else
   echo "neither bash or zsh is used, abort"
   exit 1
fi

# export NPTOOL environment variable
export NPTOOL=$SCRIPTPATH

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

export PATH=$PATH:$NPTOOL/NPLib/bin
export PATH=$PATH:$NPTOOL/NPSimulation/bin

alias npt='cd $NPTOOL'  
alias npl='cd $NPTOOL/NPLib'  
alias nps='cd $NPTOOL/NPSimulation'
export npa_not_supported='npa is now longer supported, use npp instead'
alias npa='echo $npa_not_supported'

function npp {
  if [[ $1 == *"Example"* ]]
  then
    cd $NPTOOL/Examples/$1 
  else
    cd $NPTOOL/Projects/$1
  fi
}


export Geant4_DIR=$G4LIB
export NPLib_DIR=$NPTOOL/NPLib

# This File content all environment variable needed by NPTool.
# On linux system, include it in your bashrc or tcshrc file.
# using command:  source /path/NPEnvironment

# NPTool Needs
export NPTOOL=/home/Adrien/Desktop/NPTool
export NPLIB=$NPTOOL/NPLib
export NPSIM=$NPTOOL/NPSimulation
export NPANA=$NPTOOL/NPAnalysis
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NPLIB/lib

# alias
alias Simulation='$NPSIM/bin/Linux-g++/Simulation'
alias Analysis='$NPANA/Analysis'

alias NPT='cd $NPTOOL'	
alias NPL='cd $NPLIB'	
alias NPS='cd $NPSIM'	
alias NPA='cd $NPANA'

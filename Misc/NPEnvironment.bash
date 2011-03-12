# This File content all environment variable needed by NPTool.
# On linux system, include it in your bashrc or tcshrc file.

# NPTool Needs
export NPTOOL=/YourPathToNPTool/NPTool
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NPTOOL/NPLib/lib

# alias
alias Simulation='$NPTOOL/NPSimulation/bin/$G4SYSTEM/Simulation'
alias Analysis='$NPANA/Analysis'

alias NPT='cd $NPTOOL'
alias NPL='cd $NPTOOL/NPLib'
alias NPS='cd $NPTOOL/NPSimulation'
alias NPA='cd $NPTOOL/NPAnalysis'
 

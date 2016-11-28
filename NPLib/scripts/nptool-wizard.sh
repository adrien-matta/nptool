#! /bin/bash

# define paths
printf -v pathNPT $NPTOOL
printf -v pathNPL $NPTOOL/NPLib/Detectors/
printf -v pathNPS $NPTOOL/NPSimulation/Detectors/
printf -v pathProjects $NPTOOL/Projects/
printf -v path    $PWD

#check for the force flag
force=0
while (( $# )); do
  if [[ $1 == "-f" ]];then 
    force=1
  fi
  shift
done

month="$(date +'%B')"
year="$(date +'%Y')"
printf  "\r\033[1;32m" 
printf  "********************************************************************************\n"
printf  "********************************* nptool wizard ********************************\n"
printf  "********************************************************************************\n"
printf  "           This utility will create a new detector skeleton\n"
printf  "\033[1;36m-> What is the detector name ? \033[0m\n"
read DetectorName

# Check for existing detector with similar name

if [[ $force == 0 ]]; then
  exist=0
  find Detectors/${DetectorName} > /dev/null 2> /dev/null
  if [[ $? == 0 ]];then 
    printf "\033[1;31m**** ERROR : A detector with name \033[1;36m${DetectorName}\033[1;31m already exist. use \033[1;36m-f\033[1;31m flag to force recreation.****\033[0m\n"
    exit 1
  fi

  lowerName="$(echo $DetectorName | tr '[A-Z]' '[a-z]')"
  find Detectors/${lowerName} > /dev/null 2> /dev/null
  if [[ $? == 0 ]];then 
    printf "\033[1;31m**** ERROR : A detector with similar name \033[1;36m${lowerName}\033[1;31m already exist. use \033[1;36m-f\033[1;31m flag to force recreation.****\033[0m\n"
    exit 1
  fi
 
  upperName="$(echo $DetectorName | tr '[a-z]' '[A-Z]')"
  find Detectors/${upperName} > /dev/null 2> /dev/null
  if [[ $? == 0 ]];then 
    printf "\033[1;31m**** ERROR : A detector with similar name \033[1;36m${upperName}\033[1;31m already exist. use \033[1;36m-f\033[1;31m flag to force recreation.****\033[0m\n"
    exit 1
  fi
fi

printf  "\033[1;36m-> What is your name (firstname and surname)? \033[0m\n"
read -e author 
printf  "\033[1;36m-> What is your email? \033[0m\n"
read email

# Creating the detector files name
DataFile_h="T${DetectorName}Data.h";
DataFile_cxx="T${DetectorName}Data.cxx";
PhysicsFile_h="T${DetectorName}Physics.h";
PhysicsFile_cxx="T${DetectorName}Physics.cxx";
SpectraFile_h="T${DetectorName}Spectra.h";
SpectraFile_cxx="T${DetectorName}Spectra.cxx";
SimFile_h="${DetectorName}.hh";
SimFile_cxx="${DetectorName}.cc";
InputFile="${DetectorName}.detector";

# Create npl folder for new detector
mkdir ${pathNPL}${DetectorName} > /dev/null 2> /dev/null

# Copy template files to new detector directory
cp ${pathNPL}../ressources/DetectorSkeleton/NPLib/* ${pathNPL}${DetectorName}/ > /dev/null 2> /dev/null

# Edit npl file
sed -i '' -e "s/DETECTORNAME/${DetectorName}/g" ${pathNPL}${DetectorName}/*.* > /dev/null 2> /dev/null
sed -i '' -e "s/XAUTHORX/${author}/g" ${pathNPL}${DetectorName}/*.* > /dev/null 2> /dev/null
sed -i '' -e "s/XMAILX/${email}/g" ${pathNPL}${DetectorName}/*.* > /dev/null 2> /dev/null
sed -i '' -e "s/XYEARX/${year}/g" ${pathNPL}${DetectorName}/*.* > /dev/null 2> /dev/null
sed -i '' -e "s/XMONTHX/${month}/g" ${pathNPL}${DetectorName}/*.* > /dev/null 2> /dev/null

# change files name
mv ${pathNPL}${DetectorName}/TDETECTORNAMEData.h ${pathNPL}${DetectorName}/${DataFile_h}
mv ${pathNPL}${DetectorName}/TDETECTORNAMEData.cxx ${pathNPL}${DetectorName}/${DataFile_cxx}
mv ${pathNPL}${DetectorName}/TDETECTORNAMEPhysics.h ${pathNPL}${DetectorName}/${PhysicsFile_h}
mv ${pathNPL}${DetectorName}/TDETECTORNAMEPhysics.cxx ${pathNPL}${DetectorName}/${PhysicsFile_cxx}
mv ${pathNPL}${DetectorName}/TDETECTORNAMESpectra.h ${pathNPL}${DetectorName}/${SpectraFile_h}
mv ${pathNPL}${DetectorName}/TDETECTORNAMESpectra.cxx ${pathNPL}${DetectorName}/${SpectraFile_cxx}

#Create nps folder
  mkdir ${pathNPS}${DetectorName} > /dev/null 2> /dev/null

# Add nps file
  cp ${pathNPL}../ressources/DetectorSkeleton/NPSimulation/*.* ${pathNPS}${DetectorName}/ > /dev/null 2> /dev/null

  sed -i '' -e "s/DETECTORNAME/${DetectorName}/g" ${pathNPS}${DetectorName}/*.* > /dev/null 2> /dev/null
  sed -i '' -e "s/XAUTHORX/${author}/g" ${pathNPS}${DetectorName}/*.* > /dev/null 2> /dev/null
  sed -i '' -e "s/XMAILX/${email}/g" ${pathNPS}${DetectorName}/*.* > /dev/null 2> /dev/null
  sed -i '' -e "s/XYEARX/${year}/g" ${pathNPS}${DetectorName}/*.* > /dev/null 2> /dev/null
  sed -i '' -e "s/XMONTHX/${month}/g" ${pathNPS}${DetectorName}/*.* > /dev/null 2> /dev/null

#change files name
  mv ${pathNPS}${DetectorName}/DETECTORNAME.hh ${pathNPS}${DetectorName}/${SimFile_h}
  mv ${pathNPS}${DetectorName}/DETECTORNAME.cc ${pathNPS}${DetectorName}/${SimFile_cxx}

# Create project folder
  mkdir ${pathProjects}${DetectorName} > /dev/null 2> /dev/null

#Add Project file
  cp ${pathNPL}../ressources/DetectorSkeleton/Projects/*.* ${pathProjects}${DetectorName}/ > /dev/null 2> /dev/null

  sed -i '' -e "s/DETECTORNAME/${DetectorName}/g" ${pathProjects}${DetectorName}/*.* > /dev/null 2> /dev/null

#change files name
  mv ${pathProjects}${DetectorName}/DETECTORNAME.detector ${pathProjects}${DetectorName}/${DetectorName}.detector

# Provide a list of created file 
  printf "\t List of created files to\033[1;35m edit \033[0m:\n" 
  printf "\t NPLib :\n" 
  printf "\t\t -> Folder: Detectors/${DetectorName}\n"
  printf "\t\t -> Data Class: \033[1;35m ${DataFile_h} \033[0m and \033[1;35m ${DataFile_cxx} \033[0m\n" 
  printf "\t\t -> Physics Class: \033[1;35m ${PhysicsFile_h} \033[0m and \033[1;35m ${PhysicsFile_cxx} \033[0m\n" 
  printf "\t\t -> Spectra Class: \033[1;35m ${SpectraFile_h}\033[0m and \033[1;35m${SpectraFile_cxx}\033[0m\n\n" 
  printf "\tNPSimulation :\n" 
  printf "\t\t -> Folder: Detectors/${DetectorName} \n"
  printf "\t\t -> Detector Class: \033[1;35m${SimFile_h}\033[0m and \033[1;35m${SimFile_cxx}\033[0m\n\n" 
  printf "\tProjects :\n" 
  printf "\t\t -> Folder: Detectors/${DetectorName} \n"
  printf "\t\t -> Detector Analysis: \033[1;35m Analysis.h and Analysis.cxx\033[0m\n" 
  printf "\t\t -> Detector input file: \033[1;35m ${InputFile}\033[0m\n\n" 

#Adding file to git
  printf "\033[1;36m-> Do you want to add those files to the nptool repository ? (y/n) \033[0m"
  read gitanswer
  if [[ $gitanswer =~ ^[Yy]$ ]];then 
    git add -f ${pathNPL}${DetectorName}/CMakeLists.txt ${pathNPL}${DetectorName}/${DataFile_h} ${pathNPL}${DetectorName}/${DataFile_cxx} ${pathNPL}${DetectorName}/${PhysicsFile_h} ${pathNPL}${DetectorName}/${PhysicsFile_cxx} ${pathNPL}${DetectorName}/${SpectraFile_h} ${pathNPL}${DetectorName}/${SpectraFile_cxx} ${pathNPS}${DetectorName}/CMakeLists.txt ${pathNPS}${DetectorName}/${SimFile_h} ${pathNPS}${DetectorName}/${SimFile_cxx} ${pathProjects}/${DetectorName}/${InputFile} ${pathProjects}/${DetectorName}/Analysis.h ${pathProjects}/${DetectorName}/Analysis.cxx ${pathProjects}/${DetectorName}/CMakeLists.txt
      printf "\033[1;32m**** Files added, use \033[1;36mgit commit -a \033[1;32m to commit them when ready ****\033[0m\n";
  fi
#Display help message
  printf "\n\033[1;31m**** For help editing those files visit \033[1;36mhttp://nptool.org/manual/adding-detectors \033[1;31m****\033[0m\n\n"



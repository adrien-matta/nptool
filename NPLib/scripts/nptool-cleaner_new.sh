#! /bin/bash

# define paths
printf -v pathNPT $NPTOOL
printf -v pathNPL $NPTOOL/NPLib
printf -v pathNPS $NPTOOL/NPSimulation
printf -v path    $PWD

# check whether path is authorized to run script
current=""
if [ "$path" == "$pathNPL" ] ; then
   current="NPLib"
elif [ "$path" == "$pathNPS" ] ; then
   current="NPSimulation"
else
   printf "\r\033[1;31m"
   printf "ERROR: nptool-cleaner must be used in one of the following directory\n"
   printf "\t- %s\n" "$pathNPL" 
   printf "\t- %s\n" "$pathNPS"
   exit
fi ;

# if authorized path, warn the user and ask whether (s)he wants to proceed or not
printf "\r\033[1;31m"
printf "********************************************************************************\n"
printf "********************************************************************************\n"
printf "**** You are about to clean %s\n" "$current"
printf "****   -> The folder will be reverted to pre-CMake stage\n"
printf "****   -> Only generated files will be suppressed\n"
printf "****   -> %s will not be usable without recompilation\n" "$current"
printf "****   -> Do you want to proceed? (y/n)\n"
printf "\033[0m"

read answer
# yes case
if [ "$answer" == "y" ] || [ "$answer" == "Y" ] ; then
   printf "Proceeding....\n"
# no case
elif [ "$answer" == "n" ] || [ "$answer" == "N" ] ; then
   printf "Aborting....\n"
   exit
else
# other case
   printf "Reply by y (yes) or n (no), nothing done...\n"
fi ;

############################################################
# doing the actual cleaning 
############################################################

############################################################
# specific to NPLib
############################################################
# liblist file
if [ -f ./liblist ] ; then
   rm ./liblist
fi

# include directory
for file in ./include/*
do
   if [ -f $file ] ; then
      rm $file
   fi ;
done

# NPLibTargets.cmake file
if [ -f ./NPLibTargets.cmake ] ; then
   rm ./NPLibTargets.cmake
fi

# NPLibConfig.cmake file
if [ -f ./NPLibConfig.cmake ] ; then
   rm ./NPLibConfig.cmake
fi

# remove Utility executables
if [ -f ./Utility/npanalysis ] ; then
   rm ./Utility/npanalysis
fi
if [ -f ./Utility/nponline ] ; then
   rm ./Utility/nponline
fi
if [ -f ./Utility/nptool-installer ] ; then
   rm ./Utility/nptool-installer
fi
if [ -f ./Utility/nptool-cleaner ] ; then
   rm ./Utility/nptool-cleaner
fi
if [ -f ./Utility/nptool-wizard ] ; then
   rm ./Utility/nptool-wizard
fi

############################################################
# specific to NPLib
############################################################
# remove npsimulation executable 
if [ -f ./npsimulation ] ; then
   rm ./npsimulation
fi

############################################################
# common to NPLib and NPSimulation
############################################################
# ClassList.txt file
if [ -f ./ClassList.txt ] ; then
   rm ./ClassList.txt
fi

# CMakeCache.txt file
if [ -f ./CMakeCache.txt ] ; then
   rm ./CMakeCache.txt
fi

# install_manifest.txt file
if [ -f ./install_manifest.txt ] ; then
   rm ./install_manifest.txt
fi

# bin directory
if [ -d ./bin ] ; then
   rm -rf ./bin
fi ;

# lib directory
if [ -d ./lib ] ; then
   rm -rf ./lib
fi ;

# .so extention
rm -f `find . -maxdepth 3 -type f -name "*.so"`

# .dylib extention
rm -f `find . -maxdepth 3 -type f -name "*.dylib"`

# .rootmap extention
rm -f `find . -maxdepth 3 -type f -name "*.rootmap"`

# .pcm extention
rm -f `find . -maxdepth 3 -type f -name "*.pcm"`

# dictionnary files
rm -f `find . -maxdepth 3 -type f -name "*Dict*"`

# Makefile files
rm -f `find . -maxdepth 3 -type f -name Makefile`

# cmake_install.cmake files
rm -f `find . -maxdepth 3 -type f -name cmake_install.cmake`

# .ninja files
rm -f `find . -maxdepth 3 -type f -name "*.ninja"`

# CMakeFiles directories
rm -rf `find . -maxdepth 3 -type d -name CMakeFiles`


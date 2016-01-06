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
for file in ./bin/*
do
   if [ -f $file ] ; then
   rm $file
fi ;
#rmdir ./bin
done

# include directory
for file in ./include/*
do
   if [ -f $file ] ; then
   rm $file
fi ;
#rmdir ./include
done

# lib directory
for file in ./lib/*
do
   if [ -f $file ] ; then
   rm $file
fi ;
#rmdir ./lib
done

# .so extention
for file in ./*/*.so
do
   if [ -f $file ] ; then
   rm $file
fi ;
done
for file in ./*/*/*.so
do
   if [ -f $file ] ; then
   rm $file
fi ;
done

# .dylib extention
for file in ./*/*.dylib
do
   if [ -f $file ] ; then
   rm $file
fi ;
done
for file in ./*/*/*.dylib
do
   if [ -f $file ] ; then
   rm $file
fi ;
done

# dictionnary files
for file in ./*/*Dict*
do
   if [ -f $file ] ; then
   rm $file
fi ;
done
for file in ./*/*/*Dict*
do
   if [ -f $file ] ; then
   rm $file
fi ;
done

# Makefile files
if [ -f ./Makefile ] ; then
   rm ./Makefile
fi
for file in ./*/Makefile
do
   if [ -f $file ] ; then
   rm $file
fi ;
done
for file in ./*/*/Makefile
do
   if [ -f $file ] ; then
   rm $file
fi ;
done

# cmake_install.cmake files
if [ -f ./cmake_install.cmake ] ; then
   rm ./cmake_install.cmake
fi
for file in ./*/cmake_install.cmake
do
   if [ -f $file ] ; then
   rm $file
fi ;
done
for file in ./*/*/cmake_install.cmake
do
   if [ -f $file ] ; then
   rm $file
fi ;
done

# .ninja files
for file in ./*/.ninja
do
   if [ -f $file ] ; then
   rm $file
fi ;
done
for file in ./*/*/.ninja
do
   if [ -f $file ] ; then
   rm $file
fi ;
done

# CMakeFiles directories
rm -rf `find . -maxdepth 3 -type d -name CMakeFiles`

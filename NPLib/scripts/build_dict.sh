#!/bin/bash

# This script detect the root version and call the command necessary to build 
# a Dictionnary, rootmap and pcm file accordingly

# check rootcint exist
command -v rootcint >/dev/null 2>&1 || { printf >&2 "ERROR : rootcint binary not found.\n * For ROOT install from source check that \$ROOTSYS/bin is in your PATH\n * For Macports ROOT install consider adding /opt/local/bin to your PATH\n * For debian ROOT install consider adding /usr/bin to your PATH\n"; exit 1; }

# Checking that enought argument are given
if [ "$#" -lt 4 ]
  then
    echo "Dictionnary Generation in : $PWD"
    echo "Incorrect argument number : 1.Header 2.DictName 3.Rootmap 4.LibName 5.(optional)LinkDefFile"
    echo "Arguments are : $@"
    exit 1
fi

# Modify the lib name according to the system
lib_ext=.so
system=$(uname -a)
system="${system/%\ */}"
if [ "$system" == "Darwin" ]
  then
    lib_ext=.dylib
fi

lib_name=$4
lib_name="${lib_name%.*}"
lib_name="$lib_name$lib_ext"

# Detect the Root
is_root=$(which root-config)

# Detect the version
if [ $is_root != "" ]; 
  then
    version=$(root-config --version)
  else 
    echo  "root-config not found"
    exit 1
fi

# Get the Major version
version_major="${version%.*}"

# if before version 4, exit
if [ $version_major -lt 5 ]
  then
    echo "Root version too old, min. version 5" 
    exit 1
fi

# Version 5 : generate the dictionnary then the libmap
if [ $version_major -eq 5 ]
  then
    rootcint -f $2 -c -I../Core -I../Physics -I../../Core -I../../Physics -I../TrackReconstruction -I../../TrackReconstruction $1 $5
fi

# Version 6 or more : generate both at once
if [ $version_major -gt 5 ]
  then
   rootcint -f $2 -rmf $3 -rml $lib_name -I../Core -I../Physics -I../../Core -I../../Physics -I../TrackReconstruction -I../../TrackReconstruction $1 $5
fi





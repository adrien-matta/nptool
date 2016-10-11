#!/bin/bash
# Usage
# use -c flag to change copyright date: replace -c 2016 will change all the copyright to the 2009-2016 date
# use -e flag to change contributor email : replace -e old@oldsite.com new@newsite.com will change all the old@oldsite.com to new@newsite.com

# set linux vs mac os sed command
platform='unknown'
unamestr=`uname`
sed_cmd='sed'
if [[ "$unamestr" == 'Linux' ]]; then
  sed_cmd='sed -i'
elif [[ "$unamestr" == 'Darwin' ]]; then
  sed_cmd="sed -i ''"
fi

# update the copyright date
if [ "$1" = "-c" ]; then
  echo "Updating copyright"
  $sed_cmd -e "s/Copyright (C) 2009-[0-9]\+ /Copyright (C) 2009-$2 /g" $NPTOOL/NPLib/*/*/*.cxx 
  $sed_cmd -e "s/Copyright (C) 2009-[0-9]\+ /Copyright (C) 2009-$2 /g" $NPTOOL/NPLib/*/*/*.h 
  $sed_cmd -e "s/Copyright (C) 2009-[0-9]\+ /Copyright (C) 2009-$2 /g" $NPTOOL/NPLib/*/*.cxx 
  $sed_cmd -e "s/Copyright (C) 2009-[0-9]\+ /Copyright (C) 2009-$2 /g" $NPTOOL/NPLib/*/*.h

  $sed_cmd -e "s/Copyright (C) 2009-[0-9]\+ /Copyright (C) 2009-$2 /g" $NPTOOL/NPSimulation/*/*/*.cc 
  $sed_cmd -e "s/Copyright (C) 2009-[0-9]\+ /Copyright (C) 2009-$2 /g" $NPTOOL/NPSimulation/*/*/*.hh 
  $sed_cmd -e "s/Copyright (C) 2009-[0-9]\+ /Copyright (C) 2009-$2 /g" $NPTOOL/NPSimulation/*/*.cc
  $sed_cmd -e "s/Copyright (C) 2009-[0-9]\+ /Copyright (C) 2009-$2 /g" $NPTOOL/NPSimulation/*/*.hh 
  $sed_cmd -e "s/Copyright (C) 2009-[0-9]\+ /Copyright (C) 2009-$2 /g" $NPTOOL/NPSimulation/*.cc 
fi

# update the copyright date
if [ "$1" = "-e" ]; then
  if [[ $2 == *"@"* ]]; then
    if [[ $3 == *"@"* ]]; then
      let size_diff=${#2}-${#3}
      if [[ $size_diff == "-"* ]]; then
        let size=-$size_diff
        printf -v spaces "%${size}s"
        echo "Updating email"
        $sed_cmd -e "s/ ${spaces}contact address: $2/ contact address: $3/g" $NPTOOL/NPLib/*/*/*.cxx 
        $sed_cmd -e "s/ ${spaces}contact address: $2/ contact address: $3/g" $NPTOOL/NPLib/*/*/*.h 
        $sed_cmd -e "s/ ${spaces}contact address: $2/ contact address: $3/g" $NPTOOL/NPLib/*/*.cxx 
        $sed_cmd -e "s/ ${spaces}contact address: $2/ contact address: $3/g" $NPTOOL/NPLib/*/*.h

        $sed_cmd -e "s/ ${spaces}contact address: $2/ contact address: $3/g" $NPTOOL/NPSimulation/*/*/*.cc 
        $sed_cmd -e "s/ ${spaces}contact address: $2/ contact address: $3/g" $NPTOOL/NPSimulation/*/*/*.hh 
        $sed_cmd -e "s/ ${spaces}contact address: $2/ contact address: $3/g" $NPTOOL/NPSimulation/*/*.cc
        $sed_cmd -e "s/ ${spaces}contact address: $2/ contact address: $3/g" $NPTOOL/NPSimulation/*/*.hh 
        $sed_cmd -e "s/ ${spaces}contact address: $2/ contact address: $3/g" $NPTOOL/NPSimulation/*.cc 
      else
        let size=$size_diff
        printf -v spaces "%${size}s"
        echo "Updating email"
        $sed_cmd -e "s/ contact address: $2/ contact address: $3${spaces}/g" test.file 
        $sed_cmd -e "s/ contact address: $2/ contact address: $3${spaces}/g" $NPTOOL/NPLib/*/*/*.cxx 
        $sed_cmd -e "s/ contact address: $2/ contact address: $3${spaces}/g" $NPTOOL/NPLib/*/*/*.h 
        $sed_cmd -e "s/ contact address: $2/ contact address: $3${spaces}/g" $NPTOOL/NPLib/*/*.cxx 
        $sed_cmd -e "s/ contact address: $2/ contact address: $3${spaces}/g" $NPTOOL/NPLib/*/*.h
                                                                         
        $sed_cmd -e "s/ contact address: $2/ contact address: $3${spaces}/g" $NPTOOL/NPSimulation/*/*/*.cc 
        $sed_cmd -e "s/ contact address: $2/ contact address: $3${spaces}/g" $NPTOOL/NPSimulation/*/*/*.hh 
        $sed_cmd -e "s/ contact address: $2/ contact address: $3${spaces}/g" $NPTOOL/NPSimulation/*/*.cc
        $sed_cmd -e "s/ contact address: $2/ contact address: $3${spaces}/g" $NPTOOL/NPSimulation/*/*.hh 
        $sed_cmd -e "s/ contact address: $2/ contact address: $3${spaces}/g" $NPTOOL/NPSimulation/*.cc 

      fi
    else
      echo "New email formating not correct"
    fi
  else
    echo "Old email formating not correct" 
  fi
fi

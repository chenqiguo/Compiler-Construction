#!/bin/bash

for i in `find . -name Makefile` ; do
  if test "$i" == "./Makefile" ; then
    continue ;
  fi
  if test "$i" == "./misc/Makefile" ; then
    continue ;
  fi

  pushd ./ ;
  cd `dirname $i` ; 
  make;
  popd ;
done

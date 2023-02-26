#!/bin/bash

if ! test -f compiler_output ; then
  echo "Test failed" ; 
  echo "File \"compiler_output\" is missing" ;
  exit 1;
fi
if ! test -f output/oracle_output ; then
  echo "Test failed" ; 
  echo "File \"output/oracle_output\" is missing" ;
  exit 1;
fi

outputs=`ls output`;
for i in $outputs ; do
  sort compiler_output > compiler_output_sorted ;
  sort output/$i > oracle_sorted ;
  diffOut=`diff compiler_output_sorted oracle_sorted` ;
  rm *sorted ;
  if test "$diffOut" == "" ; then 
    echo "Test passed!" ; 
    exit 0;
  fi
  echo "$diffOut" > diff/${i}_diff_output ;
done

echo "Test failed" ; 
echo "Output differences can be found in \"diff\"" ;

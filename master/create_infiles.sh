#!/bin/bash

diseasesFile=$1
countriesFile=$2
input_dir=$3
numFilesPerDirectory=$4
numRecordPerFile=$5

mkdir -p $input_dir;

id=0

while read line; do
  mkdir -p $input_dir/$line;

  status=0
  i=1
  while [ $i -le $numFilesPerDirectory ]; do
    dates=$(date -d "$((RANDOM%20+2000))-$((RANDOM%12+1))-$((RANDOM%28+1))" '+%d-%m-%Y')
    touch $input_dir/$line/$dates

    n=1
    while [ $n -le $numRecordPerFile ]; do
      firstName=$(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w 10 | head -n 1)
      lastName=$(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w 10 | head -n 1)
      age=$((0 + RANDOM % 100))
      disease=$(shuf -n 1 $diseasesFile)
      status=$(($RANDOM%2))

      if [ $status -eq 0 ]
      then
        record="${id} ENTER ${firstName} ${lastName} ${disease} ${age}"
      else
        record="${id} EXIT ${firstName} ${lastName} ${disease} ${age}"
      fi

      echo "$record" >> $input_dir/$line/$dates
      id=$(( $id + 1 ))
      n=$(( $n + 1 ))
    done

    i=$(( $i + 1 ))
  done

done < $countriesFile

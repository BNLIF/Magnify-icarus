#!/bin/bash

magnify_source="$(dirname $(readlink -f $BASH_SOURCE))"

rootfile="$1" ; shift
if [[ "$rootfile" =~ :// ]] ; then
    echo "Loading URL $rootfile"
else
    rootfile=$(readlink -f "$rootfile")
fi
tpc="${1:-110}" ; shift
frame="${1:-decon}"
startdir=$(pwd)
rebin="${2:-1}"

echo "Loading | tpc \"$tpc\" | frame \"$frame\" | rebin \"$rebin\" |"

cd $magnify_source/scripts

# echo $rootfile
# echo $frame
# echo $rebin

root -l loadClasses.C Magnify.C'("'"$rootfile"'", "'"$tpc"'", "'"$frame"'", '$rebin')'

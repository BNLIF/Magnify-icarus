#! /bin/bash

magnify_source="$(dirname $(readlink -f $BASH_SOURCE))"

# no pramater specified, or given an "-h" option
if [ "$#" -lt 1 ] || [ "$1" == "-h" ]; then
    echo ""
    echo "NAME"
    echo "      preprocess.sh - wrapper for preparing magnify historgrams for a TPC module"
    echo ""
    echo "SYNOPSIS"
    echo "      ./preprocess.sh /path/and/input.root [/output/directory] [tpc number: 0-3]"
    echo ""
    echo "DESCRIPTION"
    echo "      The output directory and the tpc number are optional."
    echo "      For example, if they were NOT specified explictly, the default"
    echo "      output path is data/, and the tpc number would be 0."
    echo "      To specify them, try this example:"
    echo "          ./preprocess.sh /data1/icarus-sim-check.root /home/user/data/ 1"
    echo ""
    exit 0
fi

rootfile="$1"
rootfile=$(readlink -f "$rootfile") # complete the fullpath
outPath="$magnify_source/data"
tpc=0

if [ "$#" -eq 2 ]; then
	outPath=$(readlink -f "$2") # complete the fullpath
elif [ "$#" -eq 3 ]; then
	outPath=$(readlink -f "$2")
	tpc=${3}
fi

cd $magnify_source/scripts

root -l -b -q preprocess.C+'("'"$rootfile"'", "'"$outPath"'", "orig", "orig", '$tpc', false, "recreate")'
root -l -b -q preprocess.C+'("'"$rootfile"'", "'"$outPath"'", "orig", "raw", '$tpc', true)'
root -l -b -q preprocess.C+'("'"$rootfile"'", "'"$outPath"'", "gauss", "decon", '$tpc')'
root -l -b -q preprocess.C+'("'"$rootfile"'", "'"$outPath"'", "threshold", "threshold", '$tpc')'

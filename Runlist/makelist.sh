#!/bin/bash

DataDir="/usatlas/groups/bnl_local/hzhu/MCgenerator/VBSAna/xAODTruth/Runlist/List"

for file in ${DataDir}/*VBSVVsemi.TRUTH1PLUS.theoryvariation.20180425.v04*list
do
    echo $file
done

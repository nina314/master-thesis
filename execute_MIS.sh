#!/bin/bash

#names=(
#    "short mammalia-voles-plj-trapping"
#    "short reptilia-tortoise-network-bsv"
#    "short aves-weaver-social"
#    "short insecta-ant-colony5"
#    "long ia-facebook-wall-wosn-dir"
#    "long fb-messages"
#    "long ia-escorts-dynamic"
#    "long ia-digg-reply"
#    "long insecta-ant-colony3"
#)
names=("short ant-colony-3" "short ant-colony-4" "short ant-colony-5" "short ant-colony-6")


g++ -std=c++20 -o exe recordRuntimes/recordRuntimesMIS.cpp maximalndependentSet/FastMIS.cpp maximalndependentSet/KboundedMIS.cpp maximalndependentSet/greedyMIS.cpp utils/common.cpp utils/DynamicHeap.cpp

for full_name in "${names[@]}"
do
    part=$(echo "$full_name" | cut -d' ' -f1)
    name=$(echo "$full_name" | cut -d' ' -f2-)
    echo "Executing Incremental MIS for $full_name"
    ./exe "$part" "$name"
done

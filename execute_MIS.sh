#!/bin/bash

names=(
    "short mammalia-voles-plj-trapping"
    "short reptilia-tortoise-network-bsv"
    "short aves-weaver-social"
    "short insecta-ant-colony5"
    "long ia-facebook-wall-wosn-dir.txt"
    "long fb-messages.txt"
    "long ia-escorts-dynamic.txt"
    "long ia-digg-reply.txt"
)

g++ -std=c++20 -o exe recordRuntimes/recordRuntimesMIS.cpp maximalndependentSet/FastMIS.cpp maximalndependentSet/KboundedMIS.cpp maximalndependentSet/greedyMIS.cpp utils/common.cpp utils/DynamicHeap.cpp

for full_name in "${names[@]}"
do
    part=$(echo "$full_name" | cut -d' ' -f1)
    name=$(echo "$full_name" | cut -d' ' -f2-)
    echo "Executing Incremental MIS for $full_name"
    ./exe "$part" "$name"
done

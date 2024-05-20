#!/bin/bash

names=("mammalia-voles-plj-trapping" "reptilia-tortoise-network-bsv" "aves-weaver-social" "insecta-ant-colony5")

for name in "${names[@]}"
do
    echo "Executing Incremental MIS for $name"
    g++ -std=c++20 -o exe recordRuntimes/recordRuntimesMIS.cpp maximalndependentSet/FastMIS.cpp maximalndependentSet/KboundedMIS.cpp maximalndependentSet/greedyMIS.cpp utils/common.cpp utils/DynamicHeap.cpp && ./exe "$name"
done

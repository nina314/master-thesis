#!/bin/bash

names=("mammalia-voles-plj-trapping" "reptilia-tortoise-network-bsv" "aves-weaver-social" "insecta-ant-colony5")

for name in "${names[@]}"
do
    echo "Executing Incremental K center for $name"
    g++ -std=c++20 -o exe4 recordRuntimes/recordRuntimesIncrementalKcenter.cpp staticKcenter/gonzales.cpp staticKcenter/distanceRIndependent.cpp staticKcenter/randomCenters.cpp staticKcenter/baselineGreedy.cpp staticKcenter/bottleneck.cpp dynamicKcenter/IncrementalAlgo.cpp utils/common.cpp maximalndependentSet/FastMIS.cpp dynamicSSSP/Dsource.cpp dynamicSSSP/ScaledEStree.cpp dynamicSSSP/EStree.cpp dynamicKcenter/ModifiedIncrementalAlgo.cpp utils/DynamicHeap.cpp && ./exe4 7 2 1 "$name"
done

for name in "${names[@]}"
do
    echo "Executing Decremental K center for $name"
    g++ -std=c++20 -o exe recordRuntimes/recordRuntimesDecrementalKcenter.cpp staticKcenter/gonzales.cpp staticKcenter/distanceRIndependent.cpp staticKcenter/randomCenters.cpp staticKcenter/baselineGreedy.cpp staticKcenter/bottleneck.cpp dynamicKcenter/DecrementalAlgo.cpp utils/common.cpp maximalndependentSet/FastMIS.cpp dynamicSSSP/Dsource.cpp dynamicSSSP/EStree.cpp dynamicSSSP/ScaledEStree.cpp utils/DynamicHeap.cpp && ./exe 7 2 1 "$name"
done

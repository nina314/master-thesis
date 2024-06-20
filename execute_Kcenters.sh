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

g++ -std=c++20 -o exe_incremental recordRuntimes/recordRuntimesIncrementalKcenterPerK.cpp staticKcenter/gonzales.cpp staticKcenter/distanceRIndependent.cpp staticKcenter/randomCenters.cpp staticKcenter/baselineGreedy.cpp staticKcenter/bottleneck.cpp dynamicKcenter/IncrementalAlgo.cpp utils/common.cpp maximalndependentSet/FastMIS.cpp maximalndependentSet/KboundedMIS.cpp dynamicSSSP/Dsource.cpp dynamicSSSP/ScaledEStree.cpp dynamicSSSP/EStree.cpp dynamicKcenter/ModifiedIncrementalAlgo.cpp utils/DynamicHeap.cpp

g++ -std=c++20 -o exe_decremental recordRuntimes/recordRuntimesDecrementalKcenterPerK.cpp staticKcenter/gonzales.cpp staticKcenter/distanceRIndependent.cpp staticKcenter/randomCenters.cpp staticKcenter/baselineGreedy.cpp staticKcenter/bottleneck.cpp dynamicKcenter/DecrementalAlgo.cpp utils/common.cpp maximalndependentSet/FastMIS.cpp dynamicSSSP/Dsource.cpp dynamicSSSP/EStree.cpp dynamicSSSP/ScaledEStree.cpp utils/DynamicHeap.cpp

g++ -std=c++20 -o exe_full recordRuntimes/recordRuntimesFullKcenterPerK.cpp staticKcenter/gonzales.cpp staticKcenter/distanceRIndependent.cpp staticKcenter/randomCenters.cpp staticKcenter/baselineGreedy.cpp staticKcenter/bottleneck.cpp dynamicKcenter/DecrementalAlgo.cpp utils/common.cpp maximalndependentSet/FastMIS.cpp dynamicSSSP/Dsource.cpp dynamicSSSP/EStree.cpp dynamicSSSP/ScaledEStree.cpp utils/DynamicHeap.cpp

run_programs() {
    local full_name="$1"
    local part=$(echo "$full_name" | cut -d' ' -f1)
    local name=$(echo "$full_name" | cut -d' ' -f2-)

    echo "Executing Incremental K center for $full_name"
    ./exe_incremental 7 2 1 "$part" "$name"

    echo "Executing Decremental K center for $full_name"
    ./exe_decremental 7 2 1 "$part" "$name"

    echo "Executing Full K center for $full_name"
    ./exe_full 7 2 0.5 "$part" "$name"
}

for full_name in "${names[@]}"; do
    run_programs "$full_name"
done

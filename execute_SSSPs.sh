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


g++ -std=c++20 -o exe_incremental recordRuntimes/recordRuntimesIncrementalSSSP.cpp dynamicSSSP/EStree.cpp dynamicSSSP/DecrementalDynamicSSSP.cpp dynamicSSSP/IncrementalDynamicSSSP.cpp dynamicSSSP/ScaledEStree.cpp utils/common.cpp dynamicSSSP/Dsource.cpp dynamicSSSP/D.cpp dynamicSSSP/MonotoneEStree.cpp utils/DynamicHeap.cpp
g++ -std=c++20 -o exe_decremental recordRuntimes/recordRuntimesDecrementalSSSP.cpp dynamicSSSP/EStree.cpp dynamicSSSP/DecrementalDynamicSSSP.cpp dynamicSSSP/IncrementalDynamicSSSP.cpp dynamicSSSP/ScaledEStree.cpp utils/common.cpp dynamicSSSP/Dsource.cpp dynamicSSSP/D.cpp dynamicSSSP/MonotoneEStree.cpp utils/DynamicHeap.cpp

run_programs() {
    local full_name="$1"
    local part=$(echo "$full_name" | cut -d' ' -f1)
    local name=$(echo "$full_name" | cut -d' ' -f2-)

    echo "Executing Incremental SSSP for $full_name"
    ./exe_incremental 40 35 1 "$part" "$name"

#    echo "Executing Decremental SSSP for $full_name"
#    ./exe_decremental 70 35 1 "$part" "$name"
}

for full_name in "${names[@]}"; do
    run_programs "$full_name"
done

#!/bin/bash

names=("mammalia-voles-plj-trapping" "reptilia-tortoise-network-bsv" "aves-weaver-social" "insecta-ant-colony5")

for name in "${names[@]}"
do
    echo "Executing Incremental SSSP for $name"
    g++ -std=c++20 -o exe recordRuntimesIncrementalSSSP.cpp dynamicSSSP/EStree.cpp dynamicSSSP/DecrementalDynamicSSSP.cpp dynamicSSSP/IncrementalDynamicSSSP.cpp dynamicSSSP/ScaledEStree.cpp utils/common.cpp dynamicSSSP/Dsource.cpp dynamicSSSP/D.cpp dynamicSSSP/MonotoneEStree.cpp utils/DynamicHeap.cpp dynamicSSSP/FullyDynamicScaledEStree.cpp && ./exe "$name"  0 35 1 
done

for name in "${names[@]}"
do
    echo "Executing Decremental SSSP for $name"
    g++ -std=c++20 -o exe recordRuntimesDecrementalSSSP.cpp dynamicSSSP/EStree.cpp dynamicSSSP/DecrementalDynamicSSSP.cpp dynamicSSSP/IncrementalDynamicSSSP.cpp dynamicSSSP/ScaledEStree.cpp utils/common.cpp dynamicSSSP/Dsource.cpp dynamicSSSP/D.cpp dynamicSSSP/MonotoneEStree.cpp utils/DynamicHeap.cpp dynamicSSSP/FullyDynamicScaledEStree.cpp && ./exe "$name" 0 35 1 
done

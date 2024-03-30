#include <gtest/gtest.h>
#include "../utils/dynamicHeap.hpp"

TEST(DynamicHeapTest, NoPopping) {
    DynamicHeap dh;
    
    dh.push({1, 1});
    dh.push({2, 2});
    dh.push({3, 3});
    
    dh.changeFirst(5, 1);
    
    EXPECT_EQ(dh.top().first, 2);
    EXPECT_EQ(dh.top().second, 2);
}

TEST(DynamicHeapTest, WithPop) {
    DynamicHeap dh;
    
    dh.push({1, 1});
    dh.push({2, 2});
    dh.push({4, 3});
    
    dh.changeFirst(3, 1);
    
    dh.pop();
    
    EXPECT_EQ(dh.top().first, 3);
    EXPECT_EQ(dh.top().second, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <vector>
#include <cassert>

#include "gtest/gtest.h"

using namespace std;

class Solution {
public:

    int trap(vector<int>& height) {
        int l=0,r=height.size()-1;
        int sum=0;
        int leftM=0,rightM=0;
        while(l<r)
        {
            if(height[l]<height[r])
            {
                if(height[l]>=leftM)
                    leftM=height[l];
                else
                    sum+=leftM-height[l];
                ++l;
            }
            else
            {
                if(height[r]>=rightM)
                    rightM=height[r];
                else
                    sum+=rightM-height[r];
                --r;
            }
        }
        return sum;
    }
};

TEST(traprainwater,tests)
{
    Solution solver;
    {
        vector<int> heights = {0,1,0,2,1,0,1,3,2,1,2,1};
        int traps = 6;
        EXPECT_EQ(solver.trap(heights),traps);
    }
    {
        vector<int> heights = {4,2,0,3,2,5};
        int traps = 9;
        EXPECT_EQ(solver.trap(heights),traps);
    }
}
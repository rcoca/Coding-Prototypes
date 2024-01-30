#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

class Solution
{
public:
    void nextPermutation(vector<int> &nums)
    {
        int i = nums.size() - 1;
        while (i > 0 && nums[i - 1] >= nums[i])
            i--;
        if (i <= 0)
        {
            reverse(nums.begin(), nums.end());
            return;
        }
        int j = nums.size() - 1;
        while (nums[j] <= nums[i - 1])
            j--;
        swap(nums[i - 1], nums[j]);
        j = nums.size() - 1;
        reverse(nums.begin() + i, nums.end());
    }
};

int main()
{
    vector<int> nums = {1, 2, 3};
    vector<int> result = {1, 3, 2};
    Solution solver;
    solver.nextPermutation(nums);
    assert(nums == result);
    nums = {3, 2, 1};
    result = {1, 2, 3};

    solver.nextPermutation(nums);
    assert(nums == result);
    nums = {1, 1, 5};
    result = {1, 5, 1};
    solver.nextPermutation(nums);
    assert(nums == result);
    return 0;
}
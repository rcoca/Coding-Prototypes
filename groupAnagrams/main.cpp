#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <memory>
#include <cassert>
#include "gtest/gtest.h"

using namespace std;
class Solution {
public:
    set<set<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string,vector<string>> anas;
        for(int i=0;i<strs.size();++i)
        {
            string s=strs[i];
            sort(s.begin(),s.end());
            anas[s].push_back(strs[i]);
        }
        set<set<string>> ans;
        for(const auto& kv:anas) ans.insert({kv.second.begin(),kv.second.end()});
        return ans;
    }
};

class MyFixture : public testing::Test {
    static std::shared_ptr<Solution> pGrouper;
 public:
    static void SetUpTestSuite() { pGrouper=std::make_shared<Solution>(); }
    static void TearDownTestSuite() { pGrouper.reset();}
    static std::shared_ptr<Solution> getGrouper(){return pGrouper;}
};
std::shared_ptr<Solution> MyFixture::pGrouper;

TEST(MyFixture,test1)
{
    vector<string> strs {"eat","tea","tan","ate","nat","bat"};
    set<set<string>> anagrams{{"bat"},{"tan","nat"},{"eat","tea","ate"}};
    EXPECT_EQ(MyFixture::getGrouper()->groupAnagrams(strs),anagrams);
}
TEST(MyFixture,test2)
{
    vector<string> strs{""};
    set<set<string>> anagrams{{""}};
    EXPECT_EQ(MyFixture::getGrouper()->groupAnagrams(strs),anagrams);
}
TEST(MyFixture,test3)
{
    vector<string> strs{"a"};
    set<set<string>> anagrams{{"a"}};
    EXPECT_EQ(MyFixture::getGrouper()->groupAnagrams(strs),anagrams);
}

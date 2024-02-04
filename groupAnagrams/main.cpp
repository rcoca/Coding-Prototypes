#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <cassert>


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

int main()
{
    Solution grouper;
    {
        vector<string> strs {"eat","tea","tan","ate","nat","bat"};
        set<set<string>> anagrams{{"bat"},{"tan","nat"},{"eat","tea","ate"}};
        assert(grouper.groupAnagrams(strs)==anagrams);
    }
    {
        vector<string> strs{""};
        set<set<string>> anagrams{{""}};
        assert(grouper.groupAnagrams(strs)==anagrams);
    }
    {
        vector<string> strs{"a"};
        set<set<string>> anagrams{{"a"}};
        assert(grouper.groupAnagrams(strs)==anagrams);
    }
    return 0;
}

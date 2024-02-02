#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxEnvelopes(vector<vector<int>>& envelopes) {
        if(envelopes.empty()) return 0;
        sort(envelopes.begin(),envelopes.end(),
             [](vector<int> const& a,vector<int> const& b){
                 if(a[0] == b[0]) return a[1]>b[1];
                 return a[0]<b[0];
             });        
        auto const &e=envelopes;
        vector<int> ans{e[0][1]};
        for(int i=1;i<e.size();++i){
            if(e[i][1]>ans.back()){
                ans.push_back(e[i][1]);
            }
            else{
                int ix=lower_bound(ans.begin(),ans.end(),e[i][1])-ans.begin();
                ans[ix]=e[i][1];
            }
        }
        return ans.size();
    }
};

int main()
{
    Solution solver;
    vector<vector<int>> input;
    int result;
    input = {{5,4},{6,4},{6,7},{2,3}};
    result = 3;
    assert(solver.maxEnvelopes(input)==result);

    input = {{1,1},{1,1},{1,1}};
    result = 1;
    assert(solver.maxEnvelopes(input)==result);

    return 0;
}

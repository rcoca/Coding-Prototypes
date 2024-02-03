#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <cassert>
#include <optional>

using namespace std;

class Solution {
public:
int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit, vector<tuple<int,int,int,int>>& selected_jobs) {
    map<int, int> times;
    unordered_map<int, vector<pair<int, int>>> jobs;
    for (auto i = 0; i < startTime.size(); ++i) {
        times[startTime[i]] = 0;
        jobs[startTime[i]].push_back({ endTime[i], profit[i] });
    }    
    int max_profit = 0;
    
    for (auto it = rbegin(times); it != rend(times); ++it) {
        optional<tuple<int,int,int,int>> selected;
        for (auto job : jobs[it->first]) {
            auto tmit = times.lower_bound(job.first);
            int current_profit = (tmit == end(times) ? 0 : tmit->second) + job.second;
            if (current_profit > max_profit) {
                max_profit = current_profit;
                selected = {it->first,job.first,job.second,max_profit}; // Store the end time of the selected job
            }            
        }
        if(selected.has_value())
            selected_jobs.push_back(selected.value());
        it->second = max_profit;        
    }
    
    std::reverse(selected_jobs.begin(),selected_jobs.end());
    return max_profit;
} 
};

void printVec(const vector<tuple<int,int,int,int>>& selected_jobs)
{
    for(auto t:selected_jobs)
    {
        cout<<get<0>(t)<<' '<<get<1>(t)<<' '<<get<2>(t)<<' '<<get<3>(t)<<'\n';
    }
    cout<<endl;
}

int main()
{
    vector<tuple<int,int,int,int>> selected_jobs;
    vector<int> startTime {1,2,3,3};
    vector<int> endTime {3,4,5,6};
    vector<int> profit {50,10,40,70};    
    int result = 120;

    Solution solver;
    assert(solver.jobScheduling(startTime,endTime,profit,selected_jobs)==result);
    printVec(selected_jobs);
    selected_jobs.clear();

    startTime = {1,2,3,4,6};
    endTime   = {3,5,10,6,9};
    profit    = {20,20,100,70,60};
    result    = 150;
    assert(solver.jobScheduling(startTime,endTime,profit,selected_jobs)==result);
    printVec(selected_jobs);
    selected_jobs.clear();

    startTime = {1,1,1};
    endTime   = {2,3,4};
    profit    = {5,6,4};
    result    = 6;
    assert(solver.jobScheduling(startTime,endTime,profit,selected_jobs)==result);
    printVec(selected_jobs);
    selected_jobs.clear();

    return 0;
}
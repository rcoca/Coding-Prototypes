#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <memory>
#include <functional>
#include <cassert>

using namespace std;
class Solution {
public:

    int evalRPN(vector<string>& tokens) {
        std::stack<int,std::vector<int>> stack;           
        for(const auto &s:tokens){                             
            if(!isdigit(s[s.size()-1])){
                auto opcode=ops.find(s);
                auto x=stack.top();stack.pop();
                auto y=stack.top();stack.pop();
                int res=opcode->second(y,x);
                stack.push(res);
            }
            else{
                stack.push(std::atoi(s.c_str()));
            }            
        }
        return stack.top();
    }
    private:
    static std::unordered_map<string,std::function<int (int,int)>> ops;

};
    
std::unordered_map<string,std::function<int (int,int)>> Solution::ops{
        {"+",std::plus<int>()},
        {"-",std::minus<int>()},
        {"*",std::multiplies<int>()},
        {"/",std::divides<int>()}};


int main()
{
    vector<string> testData_1{"2","1","+","3","*"};
    int result_1 = 9;
    vector<string> testData_2{"4","13","5","/","+"};
    int result_2 = 6;
    vector<string> testData_3{"10","6","9","3","+","-11","*","/","*","17","+","5","+"};
    int result_3 = 22;

    Solution solver;
    assert(solver.evalRPN(testData_1)==result_1);
    assert(solver.evalRPN(testData_2)==result_2);
    assert(solver.evalRPN(testData_3)==result_3);

    return 0;
}
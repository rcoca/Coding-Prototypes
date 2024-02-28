#include <iostream>
#include <algorithm>
#include <ctime>
#include <ranges>
#include <execution>
#include <vector>
#include <tuple>

template<typename T>
std::ostream& operator<<(std::ostream& os,const std::vector<T>& v)
{
    for(auto i=0;i<v.size();++i) 
    {
        os<<v[i];
        if(i<v.size()-1) os<<' ';
    }
    return os;
}

template<typename ...Ts, size_t ...Is>
std::ostream & print_tuple_impl(std::ostream& os, std::tuple<Ts...> tuple, std::index_sequence<Is...>)
{
    static_assert(sizeof...(Is)==sizeof...(Ts),"Indices must have same number of elements as tuple types!");
    static_assert(sizeof...(Ts)>0, "Cannot insert empty tuple into stream.");
    auto last = sizeof...(Ts) ; // assuming index sequence 0,...,N-1

    os<<"(";
    ((os << std::get<Is>(tuple) << ( (Is != last -1) ? ", " : "")),...);
    return (os<<")");
}

template<typename ...Ts>
std::ostream & operator<<(std::ostream& os, const std::tuple<Ts...> & tuple) {
    return print_tuple_impl(os, tuple, std::index_sequence_for<Ts...>{});
}

int main(int argc, char* argv[])
{
    //just use algorithms
    std::vector<int> v(10);
    std::srand(unsigned(std::time(nullptr)));    
    //try another execution policy for a change
    std::generate(std::execution::par,v.begin(), v.end(), std::rand);
    auto u{v};
    std::cout<<u<<std::endl;
    std::ranges::sort(v);
    std::cout<<v<<std::endl;
    
    // try a filter
    auto third=[](int i){ return i%3==0;};
    for(auto i:v|std::views::filter(third))
    {
        std::cout<<i<<':';
        //divisibility criteria: sum of digits=divisible by 3
        int sumdigits=0;        
        while(i>0) 
        {
            sumdigits+=i%10;
            i/=10;
        }
        std::cout<<sumdigits<<' ';
    }
    std::cout<<std::endl;

    //clang-17 no support for zip view, clang-18 either
    //for(auto elem: std::views::zip(v,u))    
    for(int i=0;i<u.size();++i)
    {
        std::cout<<std::tuple(u[i],v[i])<<' ';
    }
    std::cout<<std::endl;
    return 0;
}



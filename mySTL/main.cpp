#include<iostream>
#include"test.hpp"
#include<string>
#include<vector>
#include"vector.hpp"
#include<list>
#include"list.hpp"
#include<deque>
#include"deque.hpp"
#include<functional>
using std::string;
using std::cout;
using std::cin;
using std::endl;




#include<typeinfo>
#include<algorithm>
#include<vector>
#include<list>
#include<numeric>
#include<string>
constexpr int fun(int a)
{
	if (a == 1)
		return 0;
	return 1;
}
int main(int argc, char** argv)
{
	constexpr int a = fun(1);
	//cout<<testRbt();
	//cout<<testMap();
	//cout << testMultiMap();
	//cout << testSet();
	//cout<<testMultiSet();
	//cout << testStack();
	//cout << testQueue();
	//cout<<dequeTest();
	//cout<<vectorTest();
	//cout << "test finish" << endl;;
	
	return 0;
}
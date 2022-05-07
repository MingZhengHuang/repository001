#pragma once
#include<iostream>
#include<string>
#define TESTNUMBER 20
using std::cin;
using std::cout;
using std::string;
string * getTestData() //获取测试数据(指定种子后调用随机函数从而保证每次获得的测试数据相同)
{
	srand(TESTNUMBER);
	string* datas = new string[TESTNUMBER];
	char buf[10];
	for (int i = 0; i < TESTNUMBER; i++)
	{
		sprintf_s(buf, "%d", rand() % TESTNUMBER);
		datas[i] = buf;
	}
	return datas;
}
template<class  container>
void printContainer(string& des, const container& c)
{
	for (auto it = c.begin(); it != c.end(); it++)
	{
		des += *it;
		des += " ";
	}
	des += "\n";
}
template<class Map>
void printMap(string& des, const Map& mp)
{
	for (auto x : mp)
		des = des + "(" + x.first + "," + x.second + ") ";
	des += "\n";

}

#include<list>
#include"list.hpp"
template<typename List>
void exeList(string &des,List)
{
	string* testData = getTestData();
	//构造
	List l1;
	for (int i = 0; i < TESTNUMBER; i++)
		l1.push_back(testData[i]);
	List l2(l1.begin(), l1.end());
	List l3 = l1;
	List l4(10, "1000");
	printContainer(des, l1);
	// 赋值和交换
	l2 = l1;
	printContainer(des, l2);
	l3.assign(l2.begin(), l2.end());
	printContainer(des, l3);
	l4.assign(10, "1000");
	printContainer(des, l4);
	l1.swap(l4);
	printContainer(des, l1);
	printContainer(des, l4);
	//大小
	l1.resize(10);
	printContainer(des, l1);
	l2.resize(1);
	printContainer(des, l2);
	//插入与删除
	l1.push_back("10");
	l1.push_front("-10");
	printContainer(des, l1);
	l1.pop_back();
	l1.pop_front();
	printContainer(des, l1);
	auto it = l1.begin();
	auto it2 = l1.insert(it, "100");
	l1.insert(it2, "-100");
	l1.insert(it, 10, "123");
	l1.insert(it2, l2.begin(), l2.end());
	l2.erase(l2.begin(), l2.end());
	l1.erase(l1.begin());
	l1.remove("123");
	printContainer(des, l1);
	printContainer(des, l2);
}
int listTest()
{
	string hmzDes,stdDes;
	exeList(hmzDes, hmz::list<string>());
	exeList(stdDes, std::list<string>());
	if (hmzDes == stdDes)
		return 1;
	return 0;
}


#include<vector>
#include"vector.hpp"
template<typename Vector>
void exeVector(string& des, Vector)
{
	string* data = getTestData();
	//构造函数：
	Vector v1;
	for (int i = 0; i < TESTNUMBER; i++)
		v1.push_back(data[i]);
	printContainer(des, v1);

	Vector v2(v1.begin(), v1.end());
	printContainer(des, v2);

	Vector v3(10, "100");
	printContainer(des, v3);

	Vector v4 = v1;
	printContainer(des, v4);

	//赋值:
	v1 = v3;
	printContainer(des, v1);

	v1.assign(v2.begin(),v2.end());
	printContainer(des,v1);

	v1.assign(10, "100");
	printContainer(des, v1);
	//容量和大小
	if (v1.empty())
		des += "empty()\n";
	else
	{
		char buf[20];
		sprintf_s(buf, "%d", v1.capacity());
		des = des + "capacity:" + buf+"\n";
		sprintf_s(buf, "%d", v1.size());
		des = des + "size:" + buf+"\n";
	}
	v1.resize(15);
	printContainer(des, v1);
	v1.resize(20, "123");
	printContainer(des, v1);
	v1.resize(5);
	printContainer(des, v1);

	//插入和删除
	v1.push_back("222");
	printContainer(des, v1);
	v1.pop_back();
	printContainer(des, v1);
	v1.insert(v1.begin(),"0");
	printContainer(des, v1);
	v1.erase(v1.begin());
	printContainer(des,v1);
	v1.erase(v1.begin(), v1.end());
	printContainer(des, v1);
	v1 = v2;
	printContainer(des,v1);
	v1.clear();
	printContainer(des, v1);
	v1.assign(v3.begin(),v3.end());
	printContainer(des, v1);
	v1 = v2;
	printContainer(des, v1);
	des =des+ v1.at(2)+" "+v1[2]+" "+ v1.front()+" " + v1.back() + "\n";
	v1.swap(v3);
	printContainer(des, v1);
	printContainer(des, v3);
	Vector(v1).swap(v1);

	{
		char buf[20];
		sprintf_s(buf, "%d", v1.capacity());
		des = des + "capacity:" + buf + "\n";
		sprintf_s(buf, "%d", v1.size());
		des = des + "size:" + buf + "\n";
	}
	v1.reserve(50);
	{
		char buf[20];
		sprintf_s(buf, "%d", v1.capacity());
		des = des + "capacity:" + buf + "\n";
		sprintf_s(buf, "%d", v1.size());
		des = des + "size:" + buf + "\n";
	}
}
int vectorTest()
{
	string stdDes,hmzDes;
	exeVector(stdDes, std::vector<string>());
	exeVector(hmzDes, hmz::vector<string>());
	cout << stdDes << std::endl<<std::endl<<hmzDes;
	return stdDes==hmzDes;
}

#include<deque>
#include"deque.hpp"

template<class Deque>
void exeDeque(string& des, Deque)
{
	//构造函数
	Deque d1;
	Deque d2(5, "Deque()");
	Deque d3(d2.begin(), d2.end());
	Deque d4(d2);

	//赋值
	Deque d5(5,"=");
	Deque d6, d7, d8;
	d6.assign(d5.begin(), d5.end());
	d7.assign(10, "assign");
	d8 = d5;
	printContainer(des, d5);
	printContainer(des, d6);
	printContainer(des, d7);
	printContainer(des, d8);
	d8.swap(d7);
	printContainer(des, d7);
	printContainer(des, d8);
	//大小
	if (d5.empty())
	{
		des += "empty()\n";
	}
	else
	{
		char buf[10];
		sprintf_s(buf, "%d", d5.size());
		des = des + "size:" + buf+"\n";
	}
	d5.resize(2);
	d6.resize(10, "resize");
	d7.resize(10);
	printContainer(des, d5);
	printContainer(des, d6);
	printContainer(des, d7);
	printContainer(des, d8);

	//插入删除
	des += "******\n";
	printContainer(des, d5);
	d5.push_back("pushb");
	d5.push_front("pushf");
	printContainer(des, d5);
	d5.pop_back();
	d5.pop_front();
	printContainer(des, d5);

}
int dequeTest()
{
	string hmzDes,stdDes;
	exeDeque(stdDes, std::deque<string>());
	exeDeque(hmzDes, hmz::deque<string>());
	cout << hmzDes<<std::endl;
	cout << stdDes <<std::endl;
	return hmzDes==stdDes;
}

#include<stack>
#include<queue>
#include"stack.hpp"
#include"queue.hpp"
#include<string>
using namespace std;
template <class Stack>
void exeStack(string& ret,Stack)
{
	Stack s;
	string* testData = getTestData();
	for (int i = 0; i < TESTNUMBER; i++)
		s.push(testData[i]);
	ret += s.top();
	s.pop();
	if (s.empty())
		ret += "is empty()\n";
	else
	{
		char buf[20];
		sprintf_s(buf, "%d", s.size());
		ret = ret + "size:" + buf + "\n";
	}
	delete []testData;
}
bool testStack()
{
	string hmzDes, stdDes;
	exeStack(hmzDes, hmz::stack<string>());
	exeStack(stdDes, std::stack<string>());
	return hmzDes == stdDes;
}
template<class Queue>
void exeQueue(string &des, Queue)
{
	Queue q;
	string* testData = getTestData();
	for (int i = 0; i < TESTNUMBER; i++)
		q.push(testData[i]);
	des += q.front();
	q.pop();
	if (q.empty())
		des += "empty()\n";
	else
	{
		char buf[20];
		sprintf_s(buf, "size:%d\n", q.size());
		des += buf;
	}
	delete[]testData;

}
bool testQueue()
{
	string hmzDes, stdDes;
	exeQueue(hmzDes, hmz::queue<string>());
	exeQueue(stdDes, std::queue<string>());
	return hmzDes == stdDes;
}
#include"rbt.hpp"
#include"set.hpp"
#include<string>
template<typename RBT>
void exeRbt(string& ret, RBT)
{
	using std::cout;
	using std::endl;
	using std::string;
	RBT tree1;
	tree1.insert_equal("1","1"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("5","5"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("4","4"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("3","3"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("6","6"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("2","2"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("7","7"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("9","9"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("8","8"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("0","0"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("0","0"); tree1.show(); cout << "******************\n";
	tree1.insert_equal("6","6"); tree1.show(); cout << "******************\n";
	//getchar();
	tree1.erase("0"); tree1.show(); cout << "*******************\n";
	tree1.erase("0"); tree1.show(); cout << "*******************\n";
	tree1.erase("1"); tree1.show(); cout << "*******************\n";
	tree1.erase("2"); tree1.show(); cout << "*******************\n";
	tree1.erase("4"); tree1.show(); cout << "*******************\n";
	tree1.erase("6"); tree1.show(); cout << "*******************\n";
	tree1.erase("8"); tree1.show(); cout << "*******************\n";
	tree1.erase("9"); tree1.show(); cout << "*******************\n";
	tree1.erase("7"); tree1.show(); cout << "*******************\n";
	tree1.erase("5"); tree1.show(); cout << "*******************\n";
	tree1.erase("3"); tree1.show(); cout << "*******************\n";

	tree1.clear();
	tree1.insert_unique("1","1");
	tree1.insert_unique("2","2");
	RBT tree2;
	tree2 = tree1;
	tree2.erase("1");
	tree2.show();
	tree2.swap(tree1);
	tree2.show();
	tree1.show();
	cout << "****";
	if (!tree2.empty())
		cout << tree2.size();
	cout << *(tree2.find("2"));
	
}
int testRbt()
{
	string hmz_buf;
	exeRbt(hmz_buf, hmz::rbt<string, string,hmz::identity<string>>());
	return 1;
}


#include<set>
#include"set.hpp"
template <typename Set>

void exeSet(string& ret,Set)
{
	//构造 
	Set s1;
	s1.insert("1");
	s1.insert("5");
	s1.insert("4");
	s1.insert("3");
	s1.insert("6");
	s1.insert("2");
	s1.insert("7");
	s1.insert("9");
	s1.insert("8");
	s1.insert("0");
	s1.insert("0");
	s1.insert("6");
	printContainer(ret, s1);
	Set s2(s1);
	printContainer(ret, s2);
	
	s2.clear();
	printContainer(ret, s2);
	
	//赋值
	Set s3;
	s3= s2;
	printContainer(ret, s3);
	//return;
	//大小
	if (!s1.empty())
	{
		char buf[20];
		sprintf_s(buf, "%d\n", s1.size());
		ret += buf;
	}
	else
		ret += "empty()";
	//return;
	//交换
	Set s4;
	s4.insert("1");
	s1.swap(s4);
	printContainer(ret, s1);
	printContainer(ret, s4);
	//return;
	s1.erase("1");
	printContainer(ret, s1);
	//return;
	s3.erase(s3.begin(), s3.end());
	printContainer(ret, s3);
	s1.insert(s4.begin(), s4.end());
	printContainer(ret, s1);
	s2.insert("1");
	auto it = s2.begin();
	s2.insert(it, "9");
	printContainer(ret, s1);
	ret += *(s2.find("9"));
	std::string s;
	s = "1";
	{
		if (s2.find("8") == s2.end())
			ret += "not found\n";
	}
	{
		char buf[20];
		sprintf_s(buf, "%d\n", s2.count("1"));
		ret += buf;
	}
}

int testSet()
{
	string std_buf,hmz_buf;
	exeSet(std_buf, std::set<string>());
	exeSet(hmz_buf, hmz::set<string>());
	//cout << std_buf << "************" << hmz_buf;
	return std_buf==hmz_buf;
}

int testMultiSet()
{
	string std_buf, hmz_buf;
	exeSet(std_buf, std::multiset<string>());
	exeSet(hmz_buf, hmz::multiset<string>());
	//cout << std_buf << "*********\n" << hmz_buf;
	return std_buf == hmz_buf;
}



#include<map>
#include<string>
#include"map.hpp"
template<typename Map>
void exeMap(string& des, Map)
{
	//构造
	Map mp1;
	mp1.insert(pair<string, string>("1", "2"));
	mp1.insert(pair<string, string>("2", "3"));
	mp1.insert(pair<string, string>("3", "4"));
	mp1.insert(pair<string, string>("4", "5"));
	Map mp2(mp1);
	Map mp3(mp1.begin(), mp1.end());
	printMap(des, mp1);
	printMap(des, mp2);
	printMap(des, mp3);
	//大小
	if (mp1.empty())
	{
		char buf[20];
		sprintf_s(buf,"empty()\n");
		des += buf;
	}
	else
	{
		char buf[30];
		sprintf_s(buf, "size:%d\n", mp1.size());
		des += buf;
	}
	//插入和删除
	mp1.insert(make_pair("5", "6"));
	printMap(des, mp1);
	mp1.insert(mp1.begin(), make_pair("6", "7"));
	printMap(des, mp1);
	mp2.insert(mp1.begin(), mp1.end());
	printMap(des, mp2);
	mp3["8"] = "9";
	printMap(des, mp3);
	mp3.erase("1");
	printMap(des, mp3);
	//mp3.erase(mp3.begin());
	printMap(des, mp3);
	mp3.clear();
	printMap(des, mp3);
	//赋值
	mp3 = mp1;
	printMap(des, mp3);
	mp3.clear();
	printMap(des, mp3);
	//交换
	mp1.swap(mp3);
	printMap(des, mp1);
	printMap(des, mp3);
	//查找
	des = des+mp3["1"]+"\n";
	auto it = mp3.find("1");
	des = des + "("+(*it).first+","+(*it).second+")\n";
	{
		char buf[20];
		sprintf_s(buf, "%d\n", mp3.count("1"));
		des+=buf;
	}
	
}
int testMap()
{
	string std_buf,hmz_buf;
	exeMap(std_buf, std::map<string, string>());
	exeMap(hmz_buf, hmz::map<string, string>());
	std::cout << std_buf << "**********\n" << hmz_buf << endl;
	return std_buf==hmz_buf;
	
}
template<typename Multimap>
void exeMultimap(std::string& des,Multimap)
{
	//构造
	Multimap mp1;
	mp1.insert(pair<string, string>("1", "2"));
	mp1.insert(pair<string, string>("2", "3"));
	mp1.insert(pair<string, string>("3", "4"));
	mp1.insert(pair<string, string>("4", "5"));
	Multimap mp2(mp1);
	Multimap mp3(mp1.begin(), mp1.end());
	printMap(des, mp1);
	printMap(des, mp2);
	printMap(des, mp3);
	//大小
	if (mp1.empty())
	{
		char buf[20];
		sprintf_s(buf, "empty()\n");
		des += buf;
	}
	else
	{
		char buf[30];
		sprintf_s(buf, "size:%d\n", mp1.size());
		des += buf;
	}
	//插入和删除
	mp1.insert(make_pair("5", "6"));
	printMap(des, mp1);
	mp1.insert(mp1.begin(), make_pair("6", "7"));
	printMap(des, mp1);
	mp2.insert(mp1.begin(), mp1.end());
	printMap(des, mp2);
	printMap(des, mp3);
	mp3.erase("1");
	printMap(des, mp3);
	//mp3.erase(mp3.begin());
	printMap(des, mp3);
	mp3.clear();
	printMap(des, mp3);
	//赋值
	mp3 = mp1;
	printMap(des, mp3);
	mp3.clear();
	printMap(des, mp3);
	//交换
	mp1.swap(mp3);
	printMap(des, mp1);
	printMap(des, mp3);
	//查找
	auto it = mp3.find("1");
	des = des + "(" + (*it).first + "," + (*it).second + ")\n";
	{
		char buf[20];
		sprintf_s(buf, "%d\n", mp3.count("1"));
		des += buf;
	}
}
int testMultiMap()
{
	string std_buf, hmz_buf;
	exeMultimap(std_buf, std::multimap<string, string>());
	exeMultimap(hmz_buf, hmz::multimap<string, string>());
	std::cout << std_buf << "**********\n" << hmz_buf << endl;
	return std_buf == hmz_buf;
}
int testPair()
{
	pair<string, int> p("hmz", 666);
	cout << p.first << "," << p.second;

	pair<string, int> p2 = make_pair("wqq", 555);
	cout << p2.first << "," << p2.second;
	return 0;
}
/*
#include<iostream>
#include<functional>
int testFun()
{
	negate<int> neg_fun;
	cout<<neg_fun(50);
	plus<int> plus_fun;
	cout<<plus_fun(20, 10)<<endl;
	minus<int> minus_fun;
	cout<<minus_fun(20, 10)<<endl;
	multiplies<int> mul_fun;
	cout << mul_fun(20, 10)<<endl;
	divides<int> div_fun;
	cout << div_fun(20, 10) << endl;
	modulus<int> mod_fun;
	cout << mod_fun(20, 10) << endl;

	equal_to<int>;
	not_equal_to<int>;
	greater<int>;
	greater_equal<int>;
	less<int>;
	less_equal<int>;

	logical_and<bool> and_fun;
	logical_or<bool> or_fun;
	logical_not<bool> not_fun;

	cout << not_fun(true) << endl;
	cout << and_fun(true, false) << endl;
	return 0;
}
*/
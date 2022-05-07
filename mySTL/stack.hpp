#pragma once
#include"deque.hpp"
#include<iostream>
#include<deque>
namespace hmz
{
	template <class T, class Sequence=deque<T>>
	class stack
	{
	protected:
		Sequence c;
	public:
		bool empty() const { return c.empty(); }
		size_t size() const { return c.size(); }
		T& top() { return c.back(); }
		const T& top()const { return c.back(); }
		void push(const T& x) { c.push_back(x); }
		void pop() { c.pop_back(); }
	};
}
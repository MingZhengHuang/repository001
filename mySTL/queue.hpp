#pragma once
#include"deque.hpp"
namespace hmz
{
	template <class T, class Sequence =deque<T>>
	class queue
	{
	protected:
		Sequence c;
	public:
		bool empty() { return c.empty(); }
		size_t size() { return c.size(); }
		T& front() { return c.front(); }
		const T& front() const { return c.front(); }
		void push(const T& x) { c.push_back(x); }
		void pop() { c.pop_front(); }
	};
}
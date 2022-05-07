#pragma once

#include"rbt.hpp"
#include"allocator.h"
#include<functional>
namespace hmz {

	template<class T>
	class identity
	{
	public:
		const T& operator()(const T& x) const {
			return x;
		}
	};
	
	template<class Key, class Compare = less<Key>, class Alloc = allocator<rbt_node<Key,Key> > >
	class set_base
	{
	protected:
		typedef rbt<Key,Key, identity<Key>, Compare, Alloc > tree_type;
		typedef set_base<Key, Compare, Alloc> self;
		typedef  Key key_type;
		typedef Key data_type;
		typedef Key value_type;
		tree_type tree;
	public:
		typedef  typename tree_type::const_iterator iterator;
	public:
		iterator begin() { return tree.begin(); }
		const iterator begin()const { return tree.begin(); }
		iterator end() { return tree.end(); }
		const iterator end()const { return tree.end(); }
		set_base() {};
		set_base(const set_base& s) { tree = s.tree; };
		//set(iterator beg, iterator end) {tree};
		//赋值比较计算
		//> , >= , < , <= , != , == ,
		self& operator=(self& s) { this->tree = s.tree; return *this; };				//进行赋值
		//大小
		size_t size() const { return tree.size(); };				//计算容器的大小
		bool empty() const { return tree.empty(); };			//判断容器是否为空
		size_t max_size()const { return 1; };			//返回容器能够保存的最大元素个数
		//交换
		void swap(set_base& s) { this->tree.swap(s.tree); };		//交换
		//插入和删除
		virtual iterator insert(const value_type& e) = 0;		//插入e
		virtual iterator insert(iterator& pos, const value_type& e) = 0;		//在pos位置插入e hint
		virtual void insert(const iterator& beg, const iterator& end) = 0;	//插入[beg,end)
		Key erase(const key_type& e) { return tree.erase(e); };			//删除集合中的元素e
		Key erase(const iterator& pos) { return tree.erase(pos); };			//删除指定位置
		void erase(iterator beg, iterator end) { tree.erase(beg, end); };     //删除[beg,end)
		void clear() { tree.clear(); };			//清空集合
		//查找
		iterator find(const key_type& e) { return tree.find(e); };				//找到返回pos，没找到返回end（）；
		size_t count(const key_type& e)const { return tree.count(e); };			//计算集合中元素e的个数；
	};

	template<class Key, class Compare = less<Key>, class Alloc = allocator<rbt_node<Key,Key> > >
	class set :public set_base<Key,Compare,Alloc>
	{
	private:
		typedef set_base<Key, Compare, Alloc> base;
		typedef typename base::iterator iterator;
		typedef typename base::tree_type tree_type;
		typedef typename base::key_type key_type;
		typedef typename base::data_type data_type;
		typedef typename base::value_type value_type;
	public:
		virtual iterator insert(const value_type& e) { return base::tree.insert_unique(identity<value_type>()(e),e); };			//插入e
		virtual iterator insert(iterator& pos, const value_type& e) { return insert(e); };		//在pos位置插入e hint
		virtual void insert(const iterator& beg, const iterator& end) { base::tree.insert_unique(beg, end,0); };	//插入[beg,end)
	};


	template<class Key, class Compare = less<Key>, class Alloc = allocator<rbt_node<Key,Key> > >
	class multiset :public set_base<Key, Compare, Alloc>
	{
	private:
		typedef set_base<Key, Compare, Alloc> base;
		typedef typename base::iterator iterator;
		typedef typename base::tree_type tree_type;
		typedef typename base::key_type key_type;
		typedef typename base::data_type data_type;
		typedef typename base::value_type value_type;
	public:
		virtual iterator insert(const value_type& e) { return base::tree.insert_equal(identity<value_type>()(e),e); };			//插入e
		virtual iterator insert(iterator& pos, const value_type& e) { return insert(e); };		//在pos位置插入e hint
		virtual void insert(const iterator& beg, const iterator& end){ base::tree.insert_equal(beg, end,0); };	//插入[beg,end)
	};
		
}

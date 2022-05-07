#pragma once
#include"rbt.hpp"
#include"allocator.h"
#include<functional>
namespace hmz {

	

	template<class Key,class Data, class Compare = std::less<Key>, class Alloc = allocator<rbt_node<Key,std::pair<Key,Data>>> >
	class map_base
	{
	protected:
		
		typedef map_base<Key,Data,Compare, Alloc> self;
		typedef Key key_type;
		typedef Data data_type;
		typedef typename std::pair<Key,Data> value_type;
		struct get_first
		{
			const Key& operator()(const value_type& v) {
				return v.first;
			}
		};
		typedef rbt<key_type, value_type, get_first, Compare, Alloc > tree_type;
		
		tree_type tree;
	public:
		typedef  typename tree_type::iterator iterator;
	public:
		iterator begin() { return tree.begin(); }
		const iterator begin()const { return tree.begin(); }
		iterator end() { return tree.end(); }
		const iterator end()const { return tree.end(); }
		map_base() {};
		map_base(const map_base& s) { tree = s.tree; };
		map_base(const iterator& beg, const iterator& end)
		{
			tree_type t(beg, end);
			this->tree = t;
		}
		self& operator=(self& s) { this->tree = s.tree; return *this; };				//���и�ֵ
		//��С
		size_t size() const { return tree.size(); };				//���������Ĵ�С
		bool empty() const { return tree.empty(); };			//�ж������Ƿ�Ϊ��
		size_t max_size()const { return 1; };			//���������ܹ���������Ԫ�ظ���
		//����
		void swap(map_base& s) { this->tree.swap(s.tree); };		//����
		//�����ɾ��
		virtual iterator insert(const value_type& e) = 0;		//����e
		virtual iterator insert(const iterator& pos, const value_type& e) = 0;		//��posλ�ò���e hint
		virtual void insert(const iterator& beg, const iterator& end) = 0;	//����[beg,end)
		value_type erase(const key_type& e) { return tree.erase(e); };			//ɾ�������е�Ԫ��e
		value_type erase(const iterator& pos) { return tree.erase(pos); };			//ɾ��ָ��λ��
		void erase(iterator beg, iterator end) { tree.erase(beg, end); };     //ɾ��[beg,end)
		void clear() { tree.clear(); };			//��ռ���
		//����
		iterator find(const key_type& e) { return tree.find(e); };				//�ҵ�����pos��û�ҵ�����end������
		size_t count(const key_type& e)const { return tree.count(e); };			//���㼯����Ԫ��e�ĸ�����
	};

	template<class Key,class Data, class Compare = std::less<Key>, class Alloc = allocator<rbt_node<Key, std::pair<Key,Data>> > >
	class map :public map_base<Key,Data, Compare, Alloc>
	{
	private:
		typedef map_base<Key,Data, Compare, Alloc> base;
		typedef typename base::iterator iterator;
		typedef typename base::tree_type tree_type;
		typedef typename base::key_type key_type;
		typedef typename base::data_type data_type;
		typedef typename base::value_type value_type;
	public:
		map(){};
		map(const map& m) :base(m) {}
		map(const iterator& beg, const iterator& end):base(beg, end) {}
		data_type& operator[](const key_type& key)
		{
			insert(std::make_pair(key, data_type()));
			return (*base::find(key)).second;
		}
		virtual iterator insert(const value_type& e) { return base::tree.insert_unique(base::get_first()(e),e); };			//����e
		virtual iterator insert(const iterator& pos, const value_type& e) { return insert(e); };		//��posλ�ò���e hint
		virtual void insert(const iterator& beg, const iterator& end) { base::tree.insert_unique(beg, end, 0); };	//����[beg,end)
	};


	template<class Key,class Data, class Compare = std::less<Key>, class Alloc = allocator<rbt_node<Key,std::pair<Key,Data>> > >
	class multimap :public map_base<Key,Data, Compare, Alloc>
	{
	private:
		typedef map_base<Key,Data, Compare, Alloc> base;
		typedef typename base::iterator iterator;
		typedef typename base::tree_type tree_type;
		typedef typename base::key_type key_type;
		typedef typename base::data_type data_type;
		typedef typename base::value_type value_type;
	public:
		multimap() {};
		multimap(const multimap& m) :base(m) {}
		multimap(const iterator& beg, const iterator& end) :base(beg, end) {}
		virtual iterator insert(const value_type& e) { return base::tree.insert_equal(base::get_first()(e),e); };			//����e
		virtual iterator insert(const iterator& pos, const value_type& e) { return insert(e); };		//��posλ�ò���e hint
		virtual void insert(const iterator& beg, const iterator& end) { base::tree.insert_equal(beg, end, 0); };	//����[beg,end)
	};

}

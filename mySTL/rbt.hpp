#pragma once
#include<iostream>
#include<functional>
#include"allocator.h"
namespace hmz
{





	template<class K, class V> 
	struct rbt_node
	{
		rbt_node<K, V>* left;
		rbt_node<K, V>* right;
		rbt_node<K, V>* parent;
		int col;
		K key;
		V value;
		rbt_node(const K& k = K(), const V& v = V())
			:left(nullptr)
			, right(nullptr)
			, parent(nullptr)
			, col(colour::red)
			, key(k)
			, value(v)
		{}
		enum colour//������ɫΪö������
		{
			red,
			black
		};
	};





	template<class K, class V, class key_of_value, class compare = std::less<K>, class Alloc = allocator<rbt_node<K, V>> >
	class rbt ;


	//������
	template<class Key, class Value >
	class rbt_iterator
	{
		template<class Key, class Value, class Compare, class Alloc, class Data_of_Value>
		friend class rbt;
	protected:
		typedef rbt_node<Key, Value> node_type;
		typedef rbt_iterator<Key, Value> self;
		node_type* node;
	public:
		~rbt_iterator() { }//���಻��Ϊ�Լ�����ռ�
		rbt_iterator() :node(nullptr) {}
		rbt_iterator(node_type* node) :node(node) {}
		rbt_iterator(const rbt_iterator& it) { node = it.node; }
		self& operator=(const self& it) { this->node = it.node; return *this; }
		Value& operator*() { return node->value; }
		const Value& operator*()const { return node->value; }
		Value* operator->() { return &(operator*()); }
		const Value* operator->()const { return &(operator*()); }
		self& operator--()
		{
			const node_type::colour RED = node_type::colour::red;
			if (node->col == RED && node == node->parent->parent) //����ǰ�ڵ�ָ��head��end������--�൱��ָ��end��
			{
				node = node->right;
				return *this;
			}
			//��������ӣ��������������ֵ
			else if (node->left != nullptr)
			{
				node = node->left;
				while (node->right != nullptr)
					node = node->right;
				return *this;
			}
			//û�����ӡ��ҵ�һ���ڵ㣨which ���������׵��Һ��ӡ����׾���Ҫ�ҵ�ǰ����
			else
			{
				node_type* p = node->parent;
				while (node == p->left)
				{
					node = p;
					p = p->parent;
				}
				node = p;
				return *this;
			}

		}
		rbt_iterator operator--(int) { rbt_iterator tmp = *this; operator--(); return tmp; }
		rbt_iterator& operator++()
		{
			//�����Һ���ʱ�����Ϊ����������Сֵ
			if (node->right != nullptr)
			{
				node = node->right;
				while (node->left != nullptr)
					node = node->left;
				return *this;
			}
			//��û���Һ���ʱ�����ֻ�������ҡ�һֱ�ҵ�һ���ڵ㡣����ڵ����丸�ڵ�����ӡ������ڵ������Ҫ�ҵĺ��
			else
			{
				node_type* p = node->parent;
				while (node == p->right)
				{
					node = p;
					p = p->parent;
				}
				if (node->right != p)//���������ĸ�û���������Ļ���head��rightָ��rootnode��ָ��head����
					node = p;
				return *this;
			}
		}
		rbt_iterator operator++(int) { rbt_iterator tmp = *this; operator++(); return tmp; }
		bool operator!=(const self& it)const  { return this->node != it.node; }
		bool operator==(const self& it)const { return this->node == it.node; }
		
	};





	template<typename K, typename V>
	inline size_t dfs_is_rbt(rbt_node<K, V>* node)//��node�ڵ㵽Ҷ�ӽڵ㾭���ĺ�ɫ�ڵ�������ͬΪ0��
	{
		const rbt_node<K, V>::colour RED = rbt_node<K, V>::colour::red;
		const rbt_node<K, V>::colour BLACK = rbt_node<K, V>::colour::black;
		if (node == nullptr)//����ǿսڵ㣨Ҷ�ӽڵ㣩����ɫ
			return 1;
		if (node->col == RED &&
			(
				(node->left != nullptr && node->left->col == RED) ||
				(node->right != nullptr && node->right->col == RED)))//����������ɫ���ʷ���0
			return 0;

		size_t left_node = dfs_is_rbt(node->left);
		size_t right_node = dfs_is_rbt(node->right);
		if (left_node != right_node)//�ж����ҽڵ㾭���ĺ�ɫ�ڵ��Ƿ���ͬ
			return 0;
		return node->col == BLACK ? left_node + 1 : left_node;
	}

	template<typename K, typename V,typename key_of_value, typename compare, typename Alloc>
	bool is_rbt(const rbt<K, V, key_of_value, compare, Alloc>& tree)//�ж��Ƿ���������������
	{
		rbt_node<K, V>* root = tree.head->parent;
		const rbt_node<K, V>::colour RED = rbt_node<K, V>::colour::red;

		const rbt_node<K, V>::colour BlACK = rbt_node<K, V>::colour::black;
		if (root == nullptr)
			return true;
		if (root->col == RED)
			return false;
		size_t left_black_node = dfs_is_rbt(root->left);
		size_t right_black_node = dfs_is_rbt(root->right);
		return left_black_node == right_black_node;
	}





	//typedef rbt_node<K,V> node_type;
	//�����rbt
	template<class K, class V,class key_of_value, class compare, class Alloc >
	class rbt
	{
		template<typename K, typename V, typename key_of_value, typename compare, typename Alloc >
		friend bool is_rbt(const rbt<K, V, key_of_value, compare, Alloc>& tree);
	protected:
		typedef rbt_node<K,V> node_type;
		typedef rbt<K, V,key_of_value, compare, Alloc> self;
		typedef typename node_type::colour colour;
	public:
		typedef rbt_iterator<K, V> iterator;
		typedef const rbt_iterator<K,V> const_iterator;
	protected:
		node_type* head;//����ָ�����ֵ���Һ���ָ�����ֵ��parentָ��root
	
	public:
		rbt() { create_head(); }//Ĭ�Ϲ���
		rbt(const rbt& r) //��������
		{
			create_head();
			node_type* root = dfs_copy(r.head->parent);
			if (root != nullptr)
			{
				head->parent = root;
				head->left = begin().node;
				head->right = last().node;
			}
		}
		rbt(const iterator& beg, const iterator& end)
		{
			create_head();
			auto it = beg;
			while (it != end)
			{
				insert_equal(key_of_value()(*it),*it);
				++it;
			}
		}
		~rbt()
		{
			deconstruct();
			Alloc().deallocate(head);
		}
		self& operator=(const self& r)
		{
			if (r.head == this->head)
				return *this;
			node_type* root = nullptr;
			deconstruct();
			if (r.head->parent == r.head)
			{
				this->head->parent = this->head;
				this->head->left = this->head;
				this->head->right = this->head;
				return *this;
			}
			root = dfs_copy(r.head->parent);
			root->parent = head;
			head->parent = root;
			head->right = begin().node;
			head->left = last().node;
			return *this;
		}
		size_t size()const {
			iterator it = begin();
			size_t ret = 0;
			while (it != end())
			{
				++ret;
				++it;
			}
			return ret;
		}
		bool empty()const {
			return size() == 0 ? true : false;
		}
		void swap(self& tree) {
			auto* head_copy = head;
			head = tree.head;
			tree.head = head_copy;
		}
		void clear() { erase(begin(), end()); }
		void show() const {
			if (head->parent == head)
				return;
			mid(head->parent);
		};//����
		iterator find(const K& k) {
			if (head->parent == head)
				return end();
			node_type* node = head->parent;
			while (node != nullptr)
			{
				if (k == node->key)
					return iterator(node);
				else if (k < node->key)
					node = node->left;
				else
					node = node->right;
			}
			return end();

		};//����
		const iterator find(const K& k)const {
			node_type* node = head->parent;
			while (node != nullptr)
			{
				if (k == node.key)
					return iterator(node);
				else if (k < node.key)
					node = node->left;
				else
					node = node->right;
			}
			if (node == nullptr)
				return end();
		};
		size_t count(const K& k)const {
			size_t ret = 0;
			auto it = begin();
			while (it != end())
			{
				if (k == key_of_value()(*it)) {
					++ret;
				}
				++it;
			}
			return ret;
		}
		iterator begin()
		{
			if (head->parent == head)
				return iterator(head);
			node_type* p = head->parent;
			while (p != nullptr && p->left != nullptr)
				p = p->left;
			return  iterator(p);
		}//���ص�����
		iterator begin()const
		{
			if (head->parent == head)
				return iterator(head);
			node_type* p = head->parent;
			while (p != nullptr && p->left != nullptr)
				p = p->left;
			return iterator(p);
		}//���ص�����
		iterator end() { return iterator(head); }
		iterator end() const { return iterator(head); }
		iterator last()
		{
			if (head->parent == head)
				return iterator(head);
			node_type* p = head->parent;
			while (p->right != nullptr)
				p = p->right;
			return iterator(p);
		};//����ָ��������������һ���ڵ�
		const iterator last()const
		{
			if (head->parent == head)
				return iterator(head);
			node_type* p = head->parent;
			while (p->right != nullptr)
				p = p->right;
			return iterator(p);
		};//����ָ��������������һ���ڵ�

		iterator insert_unique(const K& k,const V& v)
		{
			node_type* root = head->parent;
			if (root == head)//���Ϊ��,�½ڵ�Ϊ��
			{
				root = Alloc().allocate(1);
				root->key = k;
				root->value = v;
				root->col = BLACK;
				head->parent = root;
				head->left = root;
				head->right = root;
				root->parent = head;
				return iterator(root);
			}
			//�ҵ�����λ�ã�
			node_type* p = root;
			node_type* parent = root;
			do
			{
				parent = p;
				if (compare()(k, p->key))
					p = p->left;
				else if (compare()(p->key, k))
					p = p->right;
				else
					return iterator(p);
			} while (p != nullptr);
			//�����½ڵ�
			p = Alloc().allocate(1);
			p->key = k;
			p->value = v;
			p->parent = parent;
			if (compare()(k, parent->key))
				parent->left = p;
			else
				parent->right = p;
			ajust_after_insert(p);//�������е�������Ҫ�޸�head��
			head->left = begin().node;
			head->right = last().node;
			return iterator(p);
		}//����
		void insert_unique(const iterator& beg, const iterator& end,int flag)
		{
			for (auto it = beg; it != end; ++it)
				insert_unique(key_of_value()(*it),*it);
		}
		iterator insert_equal(const K& k,const V& v)
		{
			node_type* root = head->parent;
			if (root == head)//���Ϊ��,�½ڵ�Ϊ��
			{
				root = Alloc().allocate(1);
				root->key = k;
				root->value = v;
				root->col = BLACK;
				head->parent = root;
				head->left = root;
				head->right = root;
				root->parent = head;
				return iterator(root);
			}
			//�ҵ�����λ�ã�
			node_type* p = root;
			node_type* parent = root;

			do
			{
				parent = p;
				if (compare()(k, p->key))
					p = p->left;
				else
					p = p->right;

			} while (p != nullptr);
			//�����½ڵ�
			p = Alloc().allocate(1);
			p->key = k;
			p->value = v;
			p->parent = parent;
			if (compare()(k, parent->key))
				parent->left = p;
			else
				parent->right = p;
			ajust_after_insert(p);
			head->left = begin().node;
			head->right = last().node;
			return iterator(p);
		}
		void insert_equal(const iterator& beg, const iterator& end,int flag)
		{
			for (auto it = beg; it != end; ++it)
				insert_equal(key_of_value()(*it),*it);
		}
		V erase(const K& k)//ɾ���ҵ��ĵ�һ���ڵ㣨���ܲ�����������ĵ�һ����
		{
			return erase(find(k));
		}//ɾ��
		V erase(const iterator& pos) {
			node_type* node = pos.node;
			if (node == head)
				return head->value;
			V ret = node->value;
			deleteNode(node);
			head->left = begin().node;
			head->right = end().node;
			return ret;
		}
		void erase(const iterator& beg, const iterator& end) {
			auto it = beg;
			while (it != end)
			{
				auto next_it = it;
				++next_it;
				erase(it);
				it = next_it;
			}
		}
	private:
		node_type* creat_node() {
			node_type* node = Alloc().allocate(1);
			node->left = nullptr;
			node->right = nullptr;
			node->parent = nullptr;
			node->col = RED;
			node->key = K();
			node->value = V();
			return node;
		}
		void create_head()
		{
			head = Alloc().allocate(1);
			head->parent = head;
			head->left = head;
			head->right = head;
			head->col = RED;
			
		}
		void deconstruct() {
			if (head->parent == head)
				return;
			dfs_deconstruct(head->parent);
		}
		void dfs_deconstruct(node_type* root)
		{
			if (root == nullptr)
				return;
			dfs_deconstruct(root->left);
			dfs_deconstruct(root->right);
			Alloc().deallocate(root);
		}
		node_type* dfs_copy(const node_type* root)
		{
			if (root == nullptr)
				return nullptr;
			node_type* new_root = Alloc().allocate(1);//�õ���
			new_root->key = root->key;
			new_root->value = root->value;
			new_root->col = root->col;
			auto new_left = dfs_copy(root->left);//�ݹ�õ��������ĸ��ڵ�
			new_root->left = new_left;
			if (new_left != nullptr)//�����������Ϊ�գ���Ҫ�����丸��Ϊ��ǰ�´����ĸ��ڵ�
				new_left->parent = new_root;
			auto new_right = dfs_copy(root->right);//������
			new_root->right = new_right;
			if (new_right != nullptr)
				new_right->parent = new_root;
			return new_root;
		}//�ݹ鿽��
		node_type* ajust_after_insert(node_type* e)
		{
			node_type* root = head->parent;
			while (e->parent != nullptr && e->parent->col == rbt_node<K, V>::colour::red)//������׽ڵ��Ǻ�ɫ�ģ����е���
			{
				//����Ҫ��������������У���Ȼ���ڸ��ף��游�ڵ�
				rbt_node<K, V>* parent = e->parent;
				rbt_node<K, V>* granp = parent->parent;
				if (parent == parent->parent->left)//����������游����ڵ�
				{
					rbt_node<K, V>* uncle = parent->parent->right;
					//�������ɫΪ��,ֻ��Ҫ������ɫ
					if (uncle != nullptr && uncle->col == rbt_node<K, V>::colour::red)
					{
						parent->col = rbt_node<K, V>::colour::black;
						uncle->col = rbt_node<K, V>::colour::black;
						granp->col = rbt_node<K, V>::colour::red;
						if (granp == root)//����游�Ǹ�����Ϊ��,����������
						{
							granp->col = rbt_node<K, V>::colour::black;
							break;
						}
						else
						{
							e = granp;
							continue;
						}
					}
					else //����ת���ٵ�ɫ�����¸�Ϊ�ڣ�������Ϊ�죩
					{
						if (e == parent->left) //LL�ͣ��ҵ���
						{

							LL(granp);//���游Ϊ�����������������µ������ĸ�(parent��Ϊ����granp��Ϊ�ҽڵ㣬e��Ϊ��ڵ�)
							parent->col = rbt_node<K, V>::colour::black;
							granp->col = rbt_node<K, V>::colour::red;
							break;
						}
						else
						{
							LR(granp);//�Ը���Ϊ�����������������游Ϊ��������������eΪ����parentΪ��ڵ㣬granpΪ�ҽڵ㣩
							e->col = rbt_node<K, V>::colour::black;//��ʱeָ���¸���
							granp->col = rbt_node<K, V>::colour::red;
							break;
						}
					}
				}
				else //����������游���ҽڵ�
				{
					rbt_node<K, V>* uncle = granp->left;
					if (uncle != nullptr && uncle->col == rbt_node<K, V>::colour::red)//ֻ��Ҫ������ɫ
					{
						uncle->col = rbt_node<K, V>::colour::black;
						parent->col = rbt_node<K, V>::colour::black;
						granp->col = rbt_node<K, V>::colour::red;
						if (granp == root)
						{
							granp->col = rbt_node<K, V>::colour::black;
							break;
						}
						e = granp;
						continue;
					}
					else //����ת���ٵ�ɫ
					{
						if (e == parent->right)//RR
						{
							RR(granp);//������Ϊ�����游Ϊ����������ǰ�ڵ�Ϊ��������
							parent->col = rbt_node<K, V>::colour::black;
							granp->col = rbt_node<K, V>::colour::red;
							break;
						}
						else
						{
							RL(granp);//(��ǰ�ڵ�Ϊ�����游Ϊ������������Ϊ������)
							e->col = rbt_node<K, V>::colour::black;
							granp->col = rbt_node<K, V>::colour::red;
							break;
						}
					}
				}
			}
			return head->parent;
		};
		node_type* adjust_before_erase(node_type* cur)
		{
			const rbt_node<K, V>::colour RED = rbt_node<K, V>::colour::red;
			const rbt_node<K, V>::colour BLACK = rbt_node<K, V>::colour::black;

			node_type* root = head->parent;
			if (cur->left != nullptr) //��һ�����ӣ�ֻ��Ҫ�Ѻ��ӵ���ɫ��Ϊ��ɫ
			{
				cur->left->col = BLACK;
				return root;
			}
			else if (cur->right != nullptr)
			{
				cur->right->col = BLACK;
				return root;
			}
			rbt_node<K, V>* parent = cur->parent;
			while (cur->col != RED)//���Ҫɾ���Ľڵ����ɫΪ�ڣ���Ҫ��������ɫ����Ҫ
			{
				if (cur == head->parent)
					break;
				parent = cur->parent;
				//ʣ�µ�����ֵܽڵ�϶�����
				if (cur == parent->left)//�����ǰ�ڵ����丸�ڵ����ڵ�
				{
					rbt_node<K, V>* bro = parent->right;
					if (bro->col == RED)//���һ���ֵ�Ϊ�졣��parentΪ�ڵ����������RR��,�ֵܱ�Ϊ��ɫ�����ױ�Ϊ��ɫ���ٶ�cur���з���
					{
						RR(parent);
						bro->col = BLACK;
						parent->col = RED;
						continue;
					}
					else if (bro->right != nullptr && bro->right->col == RED)//����ģ��ֵ�Ϊ�ڣ����Һ���Ϊ�죻 �Ը��������Ѹ�����ɫ���ֵܣ�����ڣ��ֵ��ҽڵ���,��������
					{
						rbt_node<K, V>* bro_right = bro->right;
						RR(parent);
						bro->col = parent->col;
						parent->col = BLACK;
						bro_right->col = BLACK;
						break;
					}
					else if ((bro->left == nullptr || bro->left->col == BLACK) &&
						(bro->right == nullptr || bro->right->col == BLACK))//��������ֵ�Ϊ�ڣ�������Ϊ�ڣ��ֵܱ�죬��������Ϊ�죬����Ϊ�ڣ�����������������Ϊ�ڣ����Ը��ڵ㵱��cur���з�����
					{
						bro->col = RED;
						if (parent->col == RED)
						{
							parent->col = BLACK;
							break;
						}
						else
						{
							cur = parent;
							continue;
						}
					}
					else if ((bro->left != nullptr && bro->left->col == RED) &&
						(bro->right == nullptr || bro->right->col == BLACK))//��������ֵ�Ϊ�ڣ�������Ϊ�죬�Һ���Ϊ�ڻ��; ���ֵ�Ϊ���������ֵܱ�죬�ֵ���ڵ��ڣ���cur���з���
					{
						rbt_node<K, V>* bro_left = bro->left;
						LL(bro);
						bro->col = RED;
						bro_left->col = BLACK;
						continue;
					}
				}
				else //��ǰ�ڵ����丸�ڵ���ҽڵ�
				{
					rbt_node<K, V>* bro = parent->left;
					if (bro->col == RED)//���һ���ֵ�Ϊ�졣��parentΪ�ڵ����������LL��,�ֵܱ�Ϊ��ɫ�����ױ�Ϊ��ɫ���ٶ�cur���з���
					{
						parent->col = RED;
						bro->col = BLACK;
						LL(parent);
						continue;
					}

					else if ((bro->left == nullptr || bro->left->col == BLACK) &&
						(bro->right == nullptr || bro->right->col == BLACK))//��������ֵ�Ϊ�ڣ�������Ϊ�ڣ��ֵܱ�죬��������Ϊ�죬����Ϊ�ڣ�����������������Ϊ�ڣ����Ը��ڵ㵱��cur���з�����
					{
						bro->col = RED;
						if (parent->col == RED)
						{
							parent->col = BLACK;
							break;
						}
						else
						{
							cur = parent;
							continue;
						}
					}
					else if ((bro->left == nullptr || bro->left->col == BLACK) &&
						(bro->right != nullptr && bro->right->col == RED))//��������ֵ�Ϊ�ڣ����Һ���Ϊ�죬����Ϊ�ڻ��; ���ֵ�Ϊ��������RR�����ֵܱ�죬�ֵ��ҽڵ��ڣ���cur���з���
					{
						bro->right->col = BLACK;
						bro->col = RED;
						RR(bro);
						continue;
					}
					else if (bro->left != nullptr && bro->left->col == RED)//����ģ��ֵ�Ϊ�ڣ�������Ϊ�죻 �Ը�������LL�����Ѹ�����ɫ���ֵܣ�����ڣ��ֵ���ڵ���,��������
					{
						bro->col = parent->col;
						parent->col = BLACK;
						bro->left->col = BLACK;
						LL(parent);
						break;
					}
				}
			}
			return head->parent;
		}
		node_type* LL(node_type* r)
		{
			node_type* t = r;
			r = r->left;

			t->left = r->right;
			if (r->right != nullptr)
				r->right->parent = t;
			r->parent = t->parent;
			if (t->parent == head)
				head->parent = r;
			else if (t == t->parent->left)
				t->parent->left = r;
			else
				t->parent->right = r;

			r->right = t;
			t->parent = r;
			return r;
		}
		node_type* RR(node_type* r)
		{
			node_type* t = r;
			r = r->right;

			if (r->left != nullptr)
				r->left->parent = t;
			t->right = r->left;

			r->parent = t->parent;
			if (t->parent != head)
			{
				if (t == t->parent->left)
					t->parent->left = r;
				else
					t->parent->right = r;
			}
			else
				head->parent = r;
			t->parent = r;
			r->left = t;
			return r;
		}
		node_type* LR(node_type* r)
		{

			RR(r->left);
			return LL(r);
		}
		node_type* RL(node_type* r)
		{
			LL(r->right);
			return RR(r);
		}
		void deleteNode(node_type* node)
		{
			node_type* root = head->parent;
			if (node->left != nullptr && node->right != nullptr) //���Һ��Ӷ���
			{
				rbt_node<K, V>* next_node = seccessor(node);//�ҵ����
				//�ú�̽���ֵ���ǵ�Ҫɾ���ڵ����Ϣ
				node->key = next_node->key;
				node->value = next_node->value;
				node = next_node;//nodeָ���̽��
			}
			root = adjust_before_erase(node);//ɾ��֮ǰ�ȵ���
			rbt_node<K, V>* parent = node->parent;
			if (node == root)//(���ڵ� ��
			{
				if (root->left != nullptr)
				{
					root = root->left;
					root->parent = head;
					head->parent = root;
				}
				else if (root->right != nullptr)
				{
					root = root->right;//�µ�rootΪ�亢��
					root->parent = head;
					head->parent = root;
				}
				else
				{
					root = nullptr;
					head->parent = head;
					head->left = head;
					head->right = head;
				}
				Alloc().deallocate(node);
				return;
			}
			if (node->left == nullptr && node->right == nullptr)//�����Ҷ�ӽڵ�
			{
				if (node == parent->left)
					parent->left = nullptr;
				else
					parent->right = nullptr;
				Alloc().deallocate(node);
				return;
			}
			else //�����һ������
			{
				rbt_node<K, V>* son = nullptr;
				if (node->left != nullptr)
				{
					son = node->left;
					son->parent = parent;
					if (node == parent->left)//���parent�������ǿգ�֮ǰ�Ѿ����ǹ��ˣ�
						parent->left = son;
					else
						parent->right = son;
				}
				else
				{
					son = node->right;
					son->parent = parent;
					if (node == parent->left)
						parent->left = son;
					else
						parent->right = son;
				}
				Alloc().deallocate(node);
			}
		}
		void mid(node_type* root) const
		{
			if (root == nullptr)
				return;
			mid(root->left);
			std::cout << "(" << root->key << "," << root->value<< "," << root->col << ")" << std::endl;
			mid(root->right);
		}
		node_type* predecessor(node_type* root)
		{
			if (root == nullptr)
				return nullptr;
			if (root->left != nullptr)
			{
				node_type* p = root->left;
				while (p->right != nullptr)
					p = p->right;
				return p;
			}
			else
			{
				node_type* p = root->parent;
				node_type* cur = root;
				while (p != nullptr && cur == p->left)
				{
					cur = p;
					p = p->parent;
				}
				return p;
			}
		}//Ѱ��ǰ��
		node_type* seccessor(node_type* root)
		{
			if (root == nullptr)
				return nullptr;
			if (root->right != nullptr)
			{
				node_type* p = root->right;
				while (p->left != nullptr)
					p = p->left;
				return p;
			}
			else
			{
				node_type* p = root->parent;
				node_type* cur = root;
				while (p != nullptr && cur == p->right)
				{
					p = p->parent;
					cur = cur->parent;
				}
				return p;
			}
		}//Ѱ�Һ��
	private:
		static const colour RED;
		const static colour BLACK;
	};
	template<class K, class V, class key_of_value, class compare, class Alloc >
	const typename rbt<K, V, key_of_value, compare, Alloc>::colour rbt<K, V, key_of_value, compare, Alloc>::RED =
													 rbt<K, V, key_of_value, compare, Alloc>::colour::red;
	template<class K, class V, class key_of_value, class compare, class Alloc >
	const typename rbt<K, V, key_of_value, compare, Alloc>::colour rbt<K, V, key_of_value, compare, Alloc>::BLACK =
													 rbt<K, V, key_of_value, compare, Alloc>::colour::black;
};
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
		enum colour//定义颜色为枚举类型
		{
			red,
			black
		};
	};





	template<class K, class V, class key_of_value, class compare = std::less<K>, class Alloc = allocator<rbt_node<K, V>> >
	class rbt ;


	//迭代器
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
		~rbt_iterator() { }//该类不会为自己分配空间
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
			if (node->col == RED && node == node->parent->parent) //当当前节点指向head（end（））--相当于指向end；
			{
				node = node->right;
				return *this;
			}
			//如果有左孩子，找左子树的最大值
			else if (node->left != nullptr)
			{
				node = node->left;
				while (node->right != nullptr)
					node = node->right;
				return *this;
			}
			//没有左孩子。找第一个节点（which 他是他父亲的右孩子。父亲就是要找的前驱）
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
			//当有右孩子时，后继为右子树的最小值
			if (node->right != nullptr)
			{
				node = node->right;
				while (node->left != nullptr)
					node = node->left;
				return *this;
			}
			//当没有右孩子时，后继只能向上找。一直找到一个节点。这个节点是其父节点的左孩子。而父节点就是所要找的后继
			else
			{
				node_type* p = node->parent;
				while (node == p->right)
				{
					node = p;
					p = p->parent;
				}
				if (node->right != p)//如果红黑树的根没有右子树的话，head的right指向，rootnode会指向head；；
					node = p;
				return *this;
			}
		}
		rbt_iterator operator++(int) { rbt_iterator tmp = *this; operator++(); return tmp; }
		bool operator!=(const self& it)const  { return this->node != it.node; }
		bool operator==(const self& it)const { return this->node == it.node; }
		
	};





	template<typename K, typename V>
	inline size_t dfs_is_rbt(rbt_node<K, V>* node)//从node节点到叶子节点经过的黑色节点数（不同为0）
	{
		const rbt_node<K, V>::colour RED = rbt_node<K, V>::colour::red;
		const rbt_node<K, V>::colour BLACK = rbt_node<K, V>::colour::black;
		if (node == nullptr)//如果是空节点（叶子节点），黑色
			return 1;
		if (node->col == RED &&
			(
				(node->left != nullptr && node->left->col == RED) ||
				(node->right != nullptr && node->right->col == RED)))//如果不满足红色性质返回0
			return 0;

		size_t left_node = dfs_is_rbt(node->left);
		size_t right_node = dfs_is_rbt(node->right);
		if (left_node != right_node)//判断左右节点经过的黑色节点是否相同
			return 0;
		return node->col == BLACK ? left_node + 1 : left_node;
	}

	template<typename K, typename V,typename key_of_value, typename compare, typename Alloc>
	bool is_rbt(const rbt<K, V, key_of_value, compare, Alloc>& tree)//判断是否满足红黑树的性质
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
	//红黑树rbt
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
		node_type* head;//左孩子指向最大值，右孩子指向最大值，parent指向root
	
	public:
		rbt() { create_head(); }//默认构造
		rbt(const rbt& r) //拷贝构造
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
		};//遍历
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

		};//查找
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
		}//返回迭代器
		iterator begin()const
		{
			if (head->parent == head)
				return iterator(head);
			node_type* p = head->parent;
			while (p != nullptr && p->left != nullptr)
				p = p->left;
			return iterator(p);
		}//返回迭代器
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
		};//返回指向中序遍历的最后一个节点
		const iterator last()const
		{
			if (head->parent == head)
				return iterator(head);
			node_type* p = head->parent;
			while (p->right != nullptr)
				p = p->right;
			return iterator(p);
		};//返回指向中序遍历的最后一个节点

		iterator insert_unique(const K& k,const V& v)
		{
			node_type* root = head->parent;
			if (root == head)//如果为空,新节点为根
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
			//找到插入位置：
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
			//插入新节点
			p = Alloc().allocate(1);
			p->key = k;
			p->value = v;
			p->parent = parent;
			if (compare()(k, parent->key))
				parent->left = p;
			else
				parent->right = p;
			ajust_after_insert(p);//插入后进行调整（需要修改head）
			head->left = begin().node;
			head->right = last().node;
			return iterator(p);
		}//插入
		void insert_unique(const iterator& beg, const iterator& end,int flag)
		{
			for (auto it = beg; it != end; ++it)
				insert_unique(key_of_value()(*it),*it);
		}
		iterator insert_equal(const K& k,const V& v)
		{
			node_type* root = head->parent;
			if (root == head)//如果为空,新节点为根
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
			//找到插入位置：
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
			//插入新节点
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
		V erase(const K& k)//删除找到的第一个节点（可能不是中序遍历的第一个）
		{
			return erase(find(k));
		}//删除
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
			node_type* new_root = Alloc().allocate(1);//得到根
			new_root->key = root->key;
			new_root->value = root->value;
			new_root->col = root->col;
			auto new_left = dfs_copy(root->left);//递归得到左子树的根节点
			new_root->left = new_left;
			if (new_left != nullptr)//如果左子树不为空，需要设置其父亲为当前新创建的根节点
				new_left->parent = new_root;
			auto new_right = dfs_copy(root->right);//右子树
			new_root->right = new_right;
			if (new_right != nullptr)
				new_right->parent = new_root;
			return new_root;
		}//递归拷贝
		node_type* ajust_after_insert(node_type* e)
		{
			node_type* root = head->parent;
			while (e->parent != nullptr && e->parent->col == rbt_node<K, V>::colour::red)//如果父亲节点是红色的，进行调整
			{
				//在需要调整的三个情况中，必然存在父亲，祖父节点
				rbt_node<K, V>* parent = e->parent;
				rbt_node<K, V>* granp = parent->parent;
				if (parent == parent->parent->left)//如果父亲是祖父的左节点
				{
					rbt_node<K, V>* uncle = parent->parent->right;
					//叔叔的颜色为红,只需要调整颜色
					if (uncle != nullptr && uncle->col == rbt_node<K, V>::colour::red)
					{
						parent->col = rbt_node<K, V>::colour::black;
						uncle->col = rbt_node<K, V>::colour::black;
						granp->col = rbt_node<K, V>::colour::red;
						if (granp == root)//如果祖父是根，变为黑,并结束调整
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
					else //先旋转，再调色（把新根为黑，其左右为红）
					{
						if (e == parent->left) //LL型，右单旋
						{

							LL(granp);//以祖父为根进行右旋。返回新的子树的根(parent变为根，granp变为右节点，e变为左节点)
							parent->col = rbt_node<K, V>::colour::black;
							granp->col = rbt_node<K, V>::colour::red;
							break;
						}
						else
						{
							LR(granp);//以父亲为根进行左旋，再以祖父为根进行右旋。（e为根，parent为左节点，granp为右节点）
							e->col = rbt_node<K, V>::colour::black;//此时e指向新根。
							granp->col = rbt_node<K, V>::colour::red;
							break;
						}
					}
				}
				else //如果父亲是祖父的右节点
				{
					rbt_node<K, V>* uncle = granp->left;
					if (uncle != nullptr && uncle->col == rbt_node<K, V>::colour::red)//只需要调整颜色
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
					else //先旋转，再调色
					{
						if (e == parent->right)//RR
						{
							RR(granp);//（父亲为根，祖父为左子树，当前节点为右子树）
							parent->col = rbt_node<K, V>::colour::black;
							granp->col = rbt_node<K, V>::colour::red;
							break;
						}
						else
						{
							RL(granp);//(当前节点为根，祖父为左子树，父亲为右子树)
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
			if (cur->left != nullptr) //有一个孩子，只需要把孩子的颜色变为黑色
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
			while (cur->col != RED)//如果要删除的节点的颜色为黑，需要调整。红色不需要
			{
				if (cur == head->parent)
					break;
				parent = cur->parent;
				//剩下的情况兄弟节点肯定存在
				if (cur == parent->left)//如果当前节点是其父节点的左节点
				{
					rbt_node<K, V>* bro = parent->right;
					if (bro->col == RED)//情况一：兄弟为红。以parent为节点进行左旋（RR）,兄弟变为黑色，父亲变为红色，再对cur进行分析
					{
						RR(parent);
						bro->col = BLACK;
						parent->col = RED;
						continue;
					}
					else if (bro->right != nullptr && bro->right->col == RED)//情况四：兄弟为黑，其右孩子为红； 以父左旋，把父的颜色给兄弟，父变黑，兄弟右节点便黑,结束调整
					{
						rbt_node<K, V>* bro_right = bro->right;
						RR(parent);
						bro->col = parent->col;
						parent->col = BLACK;
						bro_right->col = BLACK;
						break;
					}
					else if ((bro->left == nullptr || bro->left->col == BLACK) &&
						(bro->right == nullptr || bro->right->col == BLACK))//情况二：兄弟为黑，其左右为黑；兄弟变红，（若父亲为红，则父亲为黑，并结束）（若父亲为黑，贼以父节点当作cur进行分析）
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
						(bro->right == nullptr || bro->right->col == BLACK))//情况三：兄弟为黑，其左孩子为红，右孩子为黑或空; 以兄弟为根右旋，兄弟变红，兄弟左节点变黑，对cur进行分析
					{
						rbt_node<K, V>* bro_left = bro->left;
						LL(bro);
						bro->col = RED;
						bro_left->col = BLACK;
						continue;
					}
				}
				else //当前节点是其父节点的右节点
				{
					rbt_node<K, V>* bro = parent->left;
					if (bro->col == RED)//情况一：兄弟为红。以parent为节点进行右旋（LL）,兄弟变为黑色，父亲变为红色，再对cur进行分析
					{
						parent->col = RED;
						bro->col = BLACK;
						LL(parent);
						continue;
					}

					else if ((bro->left == nullptr || bro->left->col == BLACK) &&
						(bro->right == nullptr || bro->right->col == BLACK))//情况二：兄弟为黑，其左右为黑；兄弟变红，（若父亲为红，则父亲为黑，并结束）（若父亲为黑，贼以父节点当作cur进行分析）
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
						(bro->right != nullptr && bro->right->col == RED))//情况三：兄弟为黑，其右孩子为红，左孩子为黑或空; 以兄弟为根左旋（RR），兄弟变红，兄弟右节点变黑，对cur进行分析
					{
						bro->right->col = BLACK;
						bro->col = RED;
						RR(bro);
						continue;
					}
					else if (bro->left != nullptr && bro->left->col == RED)//情况四：兄弟为黑，其左孩子为红； 以父右旋（LL），把父的颜色给兄弟，父变黑，兄弟左节点便黑,结束调整
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
			if (node->left != nullptr && node->right != nullptr) //左右孩子都有
			{
				rbt_node<K, V>* next_node = seccessor(node);//找到后继
				//用后继结点的值覆盖掉要删除节点的信息
				node->key = next_node->key;
				node->value = next_node->value;
				node = next_node;//node指向后继结点
			}
			root = adjust_before_erase(node);//删除之前先调整
			rbt_node<K, V>* parent = node->parent;
			if (node == root)//(根节点 ）
			{
				if (root->left != nullptr)
				{
					root = root->left;
					root->parent = head;
					head->parent = root;
				}
				else if (root->right != nullptr)
				{
					root = root->right;//新的root为其孩子
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
			if (node->left == nullptr && node->right == nullptr)//如果是叶子节点
			{
				if (node == parent->left)
					parent->left = nullptr;
				else
					parent->right = nullptr;
				Alloc().deallocate(node);
				return;
			}
			else //如果有一个孩子
			{
				rbt_node<K, V>* son = nullptr;
				if (node->left != nullptr)
				{
					son = node->left;
					son->parent = parent;
					if (node == parent->left)//如果parent不可能是空（之前已经考虑过了）
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
		}//寻找前驱
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
		}//寻找后继
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
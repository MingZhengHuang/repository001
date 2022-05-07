测试会指定一个rseed，调用rand函数获取测试数据。通过分别调用std::和hmz;:下面的容器对测试数据进行处理，得到的结果应该一样。
//传递参数的时候最好采用const &的形式，因为这样不会调用拷贝构造函数

list:
构造函数：
	list<T> l;	
	list(beg,end);	//构造函数将[beg,end)区间中的元素拷贝给本身(size与capacity相同)
	list(n);
	list(n,elem);	//构造函数将n个elem拷贝给本身,不给elem会有默认值()
	list(const list & l);	//拷贝构造函数
赋值和交换
	assign(beg,end);	//将[beg,end)区间中的数据拷贝赋值给本身 (如果空间不够会按策略分配)
	assign(n,elem);	//将n个elem拷贝赋值给本身
	list & operator=(const list & l);	//重载赋值运算符 (如果空间不够会按策略分配)
	swap(&l);		//将l与本身交换
大小操作：
	size();		//返回容器中元素的个数
	empty();	//判断容器是否为空
	resize(num,elem);	//重新指定容器的长度为num，若容器变长，则以elem值填充新位置(由于存在默认值，所以需要保证存储的类需要有默认构造函数)；
						//如果变短，则末尾超出容器长度的元素被删除//
插入和删除
	push_back(elem);	//在容器尾部加入一个元素
	pop_back();			//删除容器中最后一个元素
	push_front();		//在容器开头插入一个元素
	pop_front();		//在容器开头移除一个元素
	insert(pos,elem);	//在pos位置插入elem元素的拷贝，返回新数据的位置
	insert(pos,n,elem);	//在pos位置插入n个elem数据，无返回值
	insert(pos,beg,end);//在pos位置插入[beg,end)区间的数据，无返回值
	clear();			//移除容器所有数据
	erase(beg,end);		//删除[beg,end)区间的数据，返回下一个数据的位置
	erase(pos);			//删除pos位置的数据，返回下一个数据的位置
	remove(elem);		//删除容器中所有与elem值匹配的元素；
数据存取
	front();			//返回第一个元素
	back();				//返回最后一个元素

listItrator:
	listIterator() {};
	listIterator(const listIterator<T>& n);//拷贝构造函数
	listIterator& operator=(const listIterator<T>& n);//重载赋值运算符
	listIterator& operator++();//重载前置++；
	listIterator  operator++(T);//重载后置++；
	listIterator& operator--();//重载前置--；
	listIterator  operator--(T);//重载后置--；
	bool		  operator!=(const listIterator<T>& L);//
	T			  operator*() const;//
	T*			  operator->() const;



vector: //空间不够1.5倍增长
	构造函数：
	 vector<T> v;					//构造函数
	 vector(beg,end);				//将[beg,end)区间中的元素拷贝给本身  本身不会有多余的空间
	 vector(n,elem);				//构造函数将n个elem拷贝给本身
	 vector(const vector & vec);	//拷贝构造函数
	赋值：
	 vector& operator=(const vector & vec);//重载赋值运算符
	 assign(beg,end);						//将[beg,end)区间的数据拷贝赋值给本身
	 assign(n,elem);						//将n个elem拷贝赋值给本身
	容量大小：
	 empty();						//判断容器是否为空
	 capacity();					//容器的容量
	 size();						//返回容器中元素的个数
	 resize(int num);				//重新制定容器的长度。变长用默认值填充，如果长度小于原来的1.5倍，则增长为1.5倍，如果大于则容量编程指定长度
									// 变短则末尾超出容器长度的元素被删除，但是容量不变
	 resize(int num,elem);			//变长用elem填充
	插入和删除：
	 push_back(elem);								//尾部插入元素elem	
	 pop_back();									//删除最后一个元素
	 insert(const_iterator pos,elem);				//迭代器指向位置pos插入元素elem
	 insert(const_iterator pos,int count,elem);		//迭代器指向位置pos插入count个元素//如果所需空间小于1.5会增长至1.5，否则增长到所需空间
	 erase(const_iterator pot);						//删除迭代器指向元素
	 erase(beg,end);								//删除迭代器从beg到end之间的元素
	 clear();										//删除容器所有元素
	数据存取：
	 at(int);					//返回索引所致的数据
	 operator[];				//返回索引所指的数据
	 front();					//返回第一个
	 back();					//返回最后一个
	互换容器：
	 swap(vec);					//将vec与本身进行交换
	预留空间：
	 reserve(int len);			//容器预留len个元素长度，预留位置不初始化，元素不可访问


deque :
	构造函数： 
		deque<T> deq;//默认构造
		deque(beg,end);//构造函数将[beg,end)区间中的元素拷贝给本身
		deque(n,elem);//构造函数将n个elem拷贝给本身
		deque(const deque& deq);//拷贝构造函数
	赋值操作：
		assign(beg,end);	//将[beg,end)区间中的数据拷贝赋值给本身
		assign(n,elem);		// 将n个elem拷贝赋值给本身
		deque& operato=(const deque & deq);		//重载等号操作符
		swap(deq);			//将deq与本身的元素互换(交换)//直接交换成员变量
	大小操作:
		size();				//返回容器中元素的个数
		empty();			//判断容器是否为空
		resize(num);		//重新指定容器的长度，如果变长，则以默认值填充，
							//如果变短，则末尾超出容器长度的元素被删除
		resize(num,elem);	//以elem填充
	双端插入和删除操作:
		push_back(elem);	//在容器尾部添加一个数据
		push_front(elem);	//在容器头部插入一个数据
		pop_back();			//删除容器最后一个数据
		pop_front();		//删除容器第一个数据
dequeItrator:
	template <class T, class Alloc = allocator<T>,size_t BufSiz=0>//在这里的BufSiz指的是每一缓存区可以存的元素个数。
	class deque;													(0为默认值,如果所存元素大小大于512则为1，否则为512/元素大小）

rbt:
	//根节点是黑色
	//红色节点不会相邻
	//所有的叶子节点（空节点）都是黑色
	//从任一节点到空节点经过的黑色节点相同。
	1.2-3-4 树二节点新增的元素直接合并为三节点
		//红黑树直接添加一个红节点。不调整
	2.2-3-4树 三节点新增一个元素合并为一个四节点
		//红黑树有六种情况，两种（左中右）的不需要调整。
		//根左左，根左右，根右左，根右右
	3.2-3-4树 四节点新增一个元素。四节点中间的节点变为父节点。
		//红黑树新增节点是红色+爷爷节点是黑色， 父节点和叔叔节点为红色，调整为
		//爷爷节点变为红色，父亲和叔叔节点变为黑色，如果爷爷节点为root节点，则调整为黑色

	//插入
		//1.找到插入位置(如果是insert_unique,小于向左找，大于向右找，等于直接返回.找到空位置。
						//如果是insert_equal.等于的时候如果右孩子不为空，替换它的位置。把右孩子的值当作新插入的。如果右孩子没有则找到空位置)
		//2.创建新节点并插入（默认为红色）
		//3.根据红黑树性质进行调整
			//3.1，当没有节点时，将新节点设置为根节点（设置为黑色）
			//3.2，当插入新的节点后，树的高度为2，不需要调整
			//3.3，当插入新的节点后，父亲节点为黑色，不需要调整
			//3.4，当插入新的节点后，父亲节点为红色，需要根据叔叔节点进行调整
				//3.4.1，当叔叔节点为红色。只需要调色（父亲，叔叔，祖父为红），根据祖父是否为根节点进行后续操作
				（将父亲和叔叔变为黑色后，可以满足红色不相邻性质。但造成了以祖父节点为根的子树的黑节点增加所以需要把祖父设置为红色。
					祖父的父节点又可能为红色，所以需要再考虑祖父节点）
					//3.4.1.1，当祖父为根节点，将祖父设置为黑色。
					//3.4.1.2，当祖父不为根节点，将祖父看成新插入的节点递归调整。
				//3.4.2，当叔叔节点为黑色，需要根据当前节点，父亲，祖父的位置关系进行旋转（LL，RR，LR,RL）。然后进行调色（新的子树根为黑色，左右节点为红色）
				//3.4.3，当叔叔节点不存在。操作与3.4.2情况一致。
	//先驱节点。
		//当左孩子不为空时，一直向右查找。
		//找到第一个左父亲（节点是其父亲节点的右节点）
	//删除
		//1.找到真正删除的节点（只有一个孩子或者没有孩子。如果有两个孩子需要用后继节点代替）
		//2.进行调整
			//2.1如果删除节点有一个孩子（只会出现删除节点是黑色。孩子节点是红色，其他情况不满足红黑树性质）；需要把孩子节点变为黑色。
			//2.2如果删除节点没有孩子
				//2.2.1如果删除节点为红色；直接删除
				//2.2.2如果删除节点为黑色
					//2.2.2.1 如果兄弟节点为黑色，兄弟没有红色孩子；将兄弟节点变为红色，
						//如果父亲本来为黑色，把父节点当作下次调整的节点。
						//如果父亲本来是红色，把父节点变为黑色并结束调整
						//（因为会删除当前节点（黑色）。删除后以父亲为根的子树黑色不平衡需要把兄弟变红。
						//如果父亲本来为黑。父亲这颗子树的黑色节点减少了。需要加深父节点。所以把父节点的一度黑当作要删除的节点，这样才能保证把父节点变为黑后整棵树满足性质；
						//如果父亲为红，不满足红色性质。需要把父亲变黑。此时不需要再父亲减色，结束调整。
					//2.2.2.2 如果兄弟节点为黑色，兄弟有同向的红孩子；以父亲为根进行旋转（将兄弟变为新根），兄弟变为父亲颜色。兄弟同向孩子变为黑色，变为黑色
						//（旋转后。父亲从待删除节点的那条路径比另一孩子路径黑色多一。删除后就平衡；）
						//（旋转后，兄弟的同向孩子为根的子树满足性质。）
						//（旋转后，兄弟会变为根。对于子树来说。如果父亲本来为红色，则从新根（兄弟）经过同向孩子的黑色不变，但如果父亲本来为黑。则会少一。应此把同向孩子变为黑，兄弟和父亲原来的颜色一致。
						//（改变兄弟和其同向孩子颜色后，兄弟经过父亲节点路径的黑色节点可能少于同向孩子路径。所以把父亲变为黑色）
					//2.2.2.3 如果兄弟节点为黑，兄弟有红孩子但不是同向的；以兄弟为根进行旋转（将红色节点变为新根，兄弟节点变为其同向孩子，转为2.2.2.2情况）
					//2.2.2.4 如果兄弟节点为红色；以父亲为根进行旋转（将红色兄弟变为新根），兄弟变为黑，父亲变为红
						//（旋转后，从子树根（红色兄弟）父亲另一边路径的黑色减少一，需要将红色兄弟变为黑。这时父亲路径多一，把父亲变为红色）

		//3.删除

set:
	//构造函数
	set c;				//建立一个空的set/multiset
	set c(op);			//以op为排序准则建立一个空集
	set c1(c2);			//建立一个集合c1，并用c2集合初始化
	set c(beg,end);		//用区间[beg,end]建立一个集合c
	//赋值比较计算
	>,>=,<,<=,!=,==,
	c1=c2;				//进行赋值
	//大小
	size();				//计算容器的大小
	empty();			//判断容器是否为空
	max_size();			//返回容器能够保存的最大元素个数
	//交换
	s1.swap(s2);		//交换
	//插入和删除
	insert(e);			//插入e
	insert(pos,e);		//在pos位置插入e
	insert(beg,end);	//插入[beg,end]
	erase(e);			//删除集合中的元素e
	erase(pos);			//删除指定位置
	erase(beg,end);     //删除[beg,edn]
	clear();			//清空集合
	//查找
	find();				//找到返回pos，没找到返回end（）；
	count(e);			//计算集合中元素e的个数；
map:
	
	/*//构造函数
	map c;				//建立一个空的map/multimap
	map c1(c2);			//建立一个集合c1，并用c2初始化
	map c(beg, end);		//用区间[beg,end]建立一个c

	//大小
	size();				//计算容器的大小
	empty();			//判断容器是否为空
	max_size();			//返回容器能够保存的最大元素个数

	
	//插入和删除
	[]
	insert(e);			//插入e
	insert(pos, e);		//在pos位置插入e
	insert(beg, end);	//插入[beg,end]
	erase(e);			//删除集合中的元素e
	erase(pos);			//删除指定位置
	erase(beg, end);     //删除[beg,edn]
	clear();			//清空集合
	
	//交换
	s1.swap(s2);		//交换
	
	//查找
	[]
	find();				//找到返回pos，没找到返回end（）；
	count(e);			//计算集合中元素e的个数；
hashtable:
	//如果元素个数比篮子个数还要多就打散。//增加为原来两倍附近的素数。并重新计算

//算法
  //算法对容器一无所知，需要通过迭代器取得，而迭代器必须能够回答算法的所有提问，才能搭配算法的所有操作
  //迭代器的分类：
	//struct input_iterator_tag{};
	//struct output_iterator_tag{};
	//struct forward_iterator_tag:public input_iterator_tag{};
	//struct bidirectional_iterator_tag: public forward_iterator_tag{};
	//struct random_iterator_tag:public bidirecional_iterator_tag{};
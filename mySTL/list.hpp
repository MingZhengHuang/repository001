#pragma once

#include"allocator.h"
namespace hmz
{

	template <class T>
	struct listNode
	{
		listNode<T>* pre;
		listNode<T>* next;
		T data;
	};
	template <class T, class Alloc = hmz::allocator<listNode<T>>>
	class list;
	template <class T>
	class listIterator
	{
	private:
		listNode<T>* node;
	public:
		listIterator() {};
		listIterator(const listIterator<T>& n);//拷贝构造函数
		listIterator& operator=(const listIterator<T>& n);//重载赋值运算符
		listIterator& operator++();//重载前置++；
		listIterator  operator++(int);//重载后置++；
		listIterator& operator--();//重载前置--；
		listIterator  operator--(int);//重载后置--；
		bool		  operator!=(const listIterator<T>& L);//
		T			  operator*() const;//
		T* operator->() const;

		friend list<T>;//因为这个类是为了list<T>服务的，所以设置了其为友元类
	};
	template <class T>
	listIterator<T>::listIterator(const listIterator<T>& n)
	{
		this->node = n.node;
	}

	template <class T>
	listIterator<T>& listIterator<T>::operator=(const listIterator<T>& n)
	{
		delete node;
		this->node = n.node;
		return *this;
	}
	template <class T>
	listIterator<T>& listIterator<T>::operator++()
	{
		node = node->next;
		return*this;
	}
	template <class T>
	listIterator<T>& listIterator<T>::operator--()
	{
		node = node->pre;
		return*this;
	}
	template <class T>
	listIterator<T> listIterator<T>::operator ++(int)
	{
		listIterator<T> tmp = *this;
		++* this;
		return tmp;
	}
	template <class T>
	listIterator<T> listIterator<T>::operator --(int)
	{
		listIterator<T> tmp = *this;
		--* this;
		return tmp;
	}
	template <class T>
	T listIterator<T>::operator*() const
	{
		return node->data;
	}
	template <class T>
	T* listIterator<T>::operator->() const
	{
		return &(operator *());
	}
	template <class T>
	bool listIterator<T>::operator !=(const listIterator<T>& L)
	{
		return !(this->node == L.node);
	}







	template <class T, class Alloc>//声明在上面
	class list
	{
	public:
		typedef listIterator<T> iterator;
	protected:
		listNode<T>* node;//指向end（什么数据都没有）
	public:
		//构造函数：
		list();
		list(iterator beg, iterator end);						//构造函数将[beg,end)区间中的元素拷贝给本身
		list(size_t n, const T& elem);									//构造函数将n个elem拷贝给本身
		list(size_t n);
		list(const list<T, Alloc>& l);									//拷贝构造函数
		//析构函数
		~list();												//析构函数
		//赋值和交换
		void assign(iterator beg, iterator end);				//将[beg,end)区间中的数据拷贝赋值给本身
		void assign(size_t n, const T& elem);								//将n个elem拷贝赋值给本身
		list<T, Alloc>& operator=(const list<T, Alloc>& L);								//重载赋值运算符
		void swap(list& l);										//将l与本身交换	
		//大小操作：
		unsigned long size();									//返回容器中元素的个数
		bool empty();											//判断容器是否为空
		void resize(size_t num);									//重新指定容器的长度为num，若容器变长，则以默认值填充新位置；
																//如果变短，则末尾超出容器长度的元素被删除
		void resize(size_t num, const T& elem);							//重新指定容器的长度为num，若容器变长，则以elem值填充新位置；
																//如果变短，则末尾超出容器长度的元素被删除
		//插入和删除
		void push_back(const T& elem);									//在容器尾部加入一个元素
		void pop_back();										//删除容器中最后一个元素
		void push_front(const T& elem);								//在容器开头插入一个元素
		void pop_front();										//在容器开头移除一个元素
		iterator insert(iterator pos, const T& elem);					//在pos位置插入elem元素的拷贝，返回新数据的位置
		void insert(iterator pos, size_t n, const T& elem);				//在pos位置插入n个elem数据，无返回值
		void insert(iterator pos, iterator beg, iterator end);	//在pos位置插入[beg,end)区间的数据，无返回值
		void clear();											//移除容器所有数据
		iterator erase(iterator beg, iterator end);				//删除[beg,end)区间的数据，返回下一个数据的位置
		iterator erase(iterator pos);							//删除pos位置的数据，返回下一个数据的位置
		void remove(const T& elem);									//删除容器中所有与elem值匹配的元素；
		//数据存取
		T front();												//返回第一个元素
		T back();												//返回最后一个元素
		//反转和排序
		void reverse();										    //翻转
		void sort();											//排序
		iterator begin() { iterator t; t.node = this->node->next; return t; };
		iterator end() { iterator t; t.node = this->node;	 return t; };
		const iterator begin()const  { iterator t; t.node = this->node->next; return t; };
		const iterator end() const{ iterator t; t.node = this->node;	 return t; };
	};
	//构造函数：
	template <class T, class Alloc>
	list<T, Alloc>::list()
	{
		Alloc allocator;
		this->node = allocator.allocate(1);
		this->node->next = this->node;
		this->node->pre = this->node;
	}
	template <class T, class Alloc>
	list<T, Alloc>::list(iterator beg, iterator end)						//构造函数将[beg,end)区间中的元素拷贝给本身
	{
		Alloc allocator;
		this->node = allocator.allocate(1);
		this->node->next = this->node;
		this->node->pre = this->node;

		while (beg != end)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = *beg;
			listNode<T>* lastNode = this->node->pre;//找到最后一个
			//将新节点加入
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
			beg++;
		}
	}
	template <class T, class Alloc>
	list<T, Alloc>::list(size_t n, const T& elem)									//构造函数将n个elem拷贝给本身
	{
		Alloc allocator;
		this->node = allocator.allocate(1);
		this->node->next = this->node;
		this->node->pre = this->node;

		for (unsigned int i = 0; i < n; i++)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = elem;
			listNode<T>* lastNode = this->node->pre;//找到最后一个
			//将新节点加入
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
		}
	}
	template <class T, class Alloc>
	list<T, Alloc>::list(size_t n)									//构造函数将n个elem拷贝给本身
	{
		Alloc allocator;
		this->node = allocator.allocate(1);
		this->node->next = this->node;
		this->node->pre = this->node;

		for (unsigned int i = 0; i < n; i++)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = T();
			listNode<T>* lastNode = this->node->pre;//找到最后一个
			//将新节点加入
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
		}
	}
	template <class T, class Alloc>
	list<T, Alloc>::list(const list<T, Alloc>& l)									//拷贝构造函数
	{
		Alloc allocator;
		this->node = allocator.allocate(1);
		this->node->next = this->node;
		this->node->pre = this->node;

		listNode<T>* lpNode = l.node->next;//指向L的第一个节点
		while (lpNode != l.node)//遍历L所有的节点
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = lpNode->data;
			listNode<T>* lastNode = this->node->pre;//找到最后一个
			//将新节点加入
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
			lpNode = lpNode->next;
		}
	}

	//析构函数
	template <class T, class Alloc>
	list<T, Alloc>::~list()
	{
		Alloc allocator;
		listNode<T>* pNode = this->node->next;//指向第一个节点
		while (pNode != this->node)
		{
			listNode<T>* tNode = pNode->next;
			allocator.deallocate(pNode);
			pNode = tNode;
		}
		allocator.deallocate(this->node);
	}
	//赋值和交换
	template<class T, class Alloc>
	void list<T, Alloc>::assign(iterator beg, iterator end)				//将[beg,end)区间中的数据拷贝赋值给本身
	{
		Alloc allocator;
		this->clear();//删除原有数据
		while (beg != end)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = *beg;
			listNode<T>* lastNode = this->node->pre;//找到最后一个
			//将新节点加入
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
			beg++;
		}
	}
	template<class T, class Alloc>
	void list<T, Alloc>::assign(size_t n, const T& elem)								//将n个elem拷贝赋值给本身
	{
		Alloc allocator;
		this->clear();

		for (size_t i = 0; i < n; i++)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = elem;
			listNode<T>* lastNode = this->node->pre;//找到最后一个
			//将新节点加入
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
		}
	}
	template<class T, class Alloc>
	list<T, Alloc>& list<T, Alloc>::operator=(const list<T, Alloc>& l)							//重载赋值运算符
	{
		Alloc allocator;
		this->clear();
		listNode<T>* lpNode = l.node->next;//指向L的第一个节点
		while (lpNode != l.node)//遍历L所有的节点
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = lpNode->data;
			listNode<T>* lastNode = this->node->pre;//找到最后一个
			//将新节点加入
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
			lpNode = lpNode->next;
		}
		return *this;
	}
	template<class T, class Alloc>
	void list<T, Alloc>::swap(list& l)										//将l与本身交换	
	{
		//直接交换两个list的node
		listNode<T>* tNode = this->node;
		this->node = l.node;
		l.node = tNode;
	}

	//大小操作：
	template <class T, class Alloc>
	unsigned long list<T, Alloc>::size()									//返回容器中元素的个数
	{
		listNode<T>* pNode = this->node->next;//指向第一个
		unsigned long num = 0;
		while (pNode != this->node)
		{
			num++;
			pNode = pNode->next;
		}
		return num;
	}
	template <class T, class Alloc>
	bool list<T, Alloc>::empty()											//判断容器是否为空
	{
		return this->node->next == this->node;
	}
	template <class T, class Alloc>
	void list<T, Alloc>::resize(size_t num, const T& elem)	//重新指定容器的长度为num，若容器变长，则以elem值填充新位置；如果变短，则末尾超出容器长度的元素被删除
	{
		//找到最后一个，如果此时num为0则直接结束循环；
		listNode<T>* pNode = this->node->next;//指向第一个；
		if (num == 0)
			pNode = this->node;
		while (pNode != this->node)
		{
			num--;
			if (num == 0)
				break;
			pNode = pNode->next;
		}
		Alloc allocator;
		if (num == 0)//说明变短
		{

			listNode<T>* tNode = pNode;
			pNode = pNode->next;//转到下一个节点
			while (pNode != this->node)
			{
				listNode<T>* nNode = pNode->next;//记录当前需要删除节点的下一个节点
				allocator.deallocate(pNode);//删除当前节点
				pNode = nNode;
			}
			tNode->next = this->node;
			this->node->pre = tNode;
		}
		else //变长
		{
			while (num)
			{
				listNode<T>* pNode = allocator.allocate(1);
				pNode->data = elem;
				listNode<T>* lastNode = this->node->pre;//指向最后一个节点
				pNode->next = this->node;
				pNode->pre = lastNode;
				this->node->pre = pNode;
				lastNode->next = pNode;
				num--;
			}
		}
	}
	template <class T, class Alloc>
	void list<T, Alloc>::resize(size_t num)	//重新指定容器的长度为num，若容器变长，则以默认值填充新位置；如果变短，则末尾超出容器长度的元素被删除
	{
		//找到最后一个，如果此时num为0则直接结束循环；
		listNode<T>* pNode = this->node->next;//指向第一个；
		if (num == 0)
			pNode = this->node;
		while (pNode != this->node)
		{
			num--;
			if (num == 0)
				break;
			pNode = pNode->next;
		}
		Alloc allocator;
		if (num == 0)//说明变短
		{

			listNode<T>* tNode = pNode;
			pNode = pNode->next;//转到下一个节点
			while (pNode != this->node)
			{
				listNode<T>* nNode = pNode->next;//记录当前需要删除节点的下一个节点
				allocator.deallocate(pNode);//删除当前节点
				pNode = nNode;
			}
			tNode->next = this->node;
			this->node->pre = tNode;
		}
		else //变长
		{
			while (num)
			{
				listNode<T>* pNode = allocator.allocate(1);
				//listNode<T>* defNode = new listNode<T>();
				//pNode->data = defNode->data;
				//delete	defNode;
				pNode->data = T();
				listNode<T>* lastNode = this->node->pre;//指向最后一个节点
				pNode->next = this->node;
				pNode->pre = lastNode;
				this->node->pre = pNode;
				lastNode->next = pNode;
				num--;
			}
		}
	}

	//插入和删除
	template <class T, class Alloc>
	void list<T, Alloc>::push_back(const T& elem)									//在容器尾部加入一个元素
	{
		Alloc allocator;
		listNode<T>* pNode = allocator.allocate(1);
		pNode->data = elem;
		listNode<T>* endNode = this->node->pre;//最后一个节点
		//将新创建的节点加入
		pNode->next = this->node;
		pNode->pre = endNode;
		endNode->next = pNode;
		this->node->pre = pNode;
	}
	template <class T, class Alloc>
	void  list<T, Alloc>::pop_back()										//删除容器中最后一个元素
	{
		listNode<T>* lastNode = this->node->pre;
		if (lastNode == this->node)
			throw - 1;
		Alloc allocator;
		listNode<T>* preNode = lastNode->pre;
		allocator.deallocate(lastNode);
		this->node->pre = preNode;
		preNode->next = this->node;
	}
	template <class T, class Alloc>
	void list<T, Alloc>::push_front(const T& elem)								//在容器开头插入一个元素
	{
		listNode<T>* firstNode = this->node->next;
		Alloc allocator;
		listNode<T>* pNode = allocator.allocate(1);
		pNode->data = elem;
		pNode->next = firstNode;
		pNode->pre = this->node;
		firstNode->pre = pNode;
		this->node->next = pNode;
	}
	template <class T, class Alloc>
	void list<T, Alloc>::pop_front()										//在容器开头移除一个元素
	{
		Alloc allocator;
		listNode<T>* firstNode = this->node->next;
		if (firstNode == this->node)
			throw - 1;
		listNode<T>* tNode = firstNode->next;//记录被删除节点的下一个节点
		allocator.deallocate(firstNode);
		this->node->next = tNode;
		tNode->pre = this->node;
	}
	template <class T, class Alloc>
	listIterator<T> list<T, Alloc>::insert(iterator pos, const T& elem)			//在pos位置插入elem元素的拷贝，返回新数据的位置
	{
		Alloc allocator;
		listNode<T>* preNode = pos.node->pre;
		listNode<T>* pNode = allocator.allocate(1);
		pNode->data = elem;
		pNode->next = pos.node;
		pNode->pre = preNode;
		preNode->next = pNode;
		pos.node->pre = pNode;
		list<T, Alloc>::iterator retIt;
		retIt.node = pNode;
		return retIt;
	}
	template <class T, class Alloc>
	void list<T, Alloc>::insert(iterator pos, size_t n, const T& elem)				//在pos位置插入n个elem数据，无返回值
	{
		Alloc allocator;
		while (n)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = elem;
			listNode<T>* preNode = pos.node->pre;//指向插入位置的前一个节点
			pNode->next = pos.node;
			pNode->pre = preNode;
			preNode->next = pNode;
			pos.node->pre = pNode;
			n--;
		}
	}
	template <class T, class Alloc>
	void list<T, Alloc>::insert(iterator pos, iterator beg, iterator end)	//在pos位置插入[beg,end)区间的数据，无返回值
	{
		Alloc allocator;
		while (beg != end)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = *beg;
			listNode<T>* preNode = pos.node->pre;//指向插入位置的前一个节点
			pNode->next = pos.node;
			pNode->pre = preNode;
			preNode->next = pNode;
			pos.node->pre = pNode;
			beg++;
		}
	}
	template <class T, class Alloc>
	void list<T, Alloc>::clear()											//移除容器所有数据
	{
		Alloc allocator;
		listNode<T>* pNode = this->node->next;//指向第一个节点
		while (pNode != this->node)
		{
			listNode<T>* tNode = pNode->next;
			allocator.deallocate(pNode);
			pNode = tNode;
		}
		this->node->pre = this->node;
		this->node->next = this->node;
	}
	template <class T, class Alloc>
	listIterator<T> list<T, Alloc>::erase(iterator beg, iterator end)				//删除[beg,end)区间的数据，返回下一个数据的位置
	{
		Alloc allocator;
		listNode<T>* preNode = beg.node->pre;
		listNode<T>* pNode = beg.node;
		listNode<T>* nextNode;
		while (pNode != end.node)
		{
			nextNode = pNode->next;
			allocator.deallocate(pNode);
			pNode = nextNode;
		}
		preNode->next = end.node;
		end.node->pre = preNode;
		return end;
	}
	template <class T, class Alloc>
	listIterator<T> list<T, Alloc>::erase(iterator pos)							//删除pos位置的数据，返回下一个数据的位置
	{
		Alloc allocator;
		listNode<T>* preNode = pos.node->pre;
		listNode<T>* nextNode = pos.node->next;
		allocator.deallocate(pos.node);
		preNode->next = nextNode;
		nextNode->pre = preNode;
		listIterator<T> retIt;
		retIt.node = nextNode;
		return retIt;
	}
	template <class T, class Alloc>
	void list<T, Alloc>::remove(const T& elem)									//删除容器中所有与elem值匹配的元素；*/
	{
		Alloc allocator;
		listNode<T>* preNode=this->node,*pNode=preNode->next;
		while (pNode != this->node)
		{
			listNode<T>* nextNode = pNode->next;
			if (pNode->data == elem)
			{
				allocator.deallocate(pNode);
				preNode->next = nextNode;
				nextNode->pre = preNode;
				pNode = nextNode;
			}
			else
			{
				preNode = pNode;
				pNode = nextNode;
			}
		}

	}
	//数据存取
	template <class T, class Alloc>
	T list<T, Alloc>::front()												//返回第一个元素
	{
		return this->node->next->data;
	}
	template <class T, class Alloc>
	T list<T, Alloc>::back()												//返回最后一个元素
	{
		return this->node->pre->data;
	}
}

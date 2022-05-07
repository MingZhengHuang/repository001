#pragma once
#include<vector>
#include"allocator.h"
namespace hmz
{
	template <class T, class Alloc = allocator<T>,size_t BufSiz=0>
	class deque;

	template <class T,size_t BufSiz>
	class dequeIterator
	{
		template <class Y, class Alloc, size_t BufSize>
		friend class deque;
	private:
		typedef dequeIterator self;
		T* cur;
		T* first;
		T* last;
		T** node;
	private:
		size_t getBufSiz() const
		{
			if (BufSiz == 0)
				return sizeof(T) > 512 ? 1 : 512 / sizeof(T);
			return BufSiz;
		}
	public:
		dequeIterator() {};
		dequeIterator(const dequeIterator<T, BufSiz>& it) { cur = it.cur; first = it.first; last = it.last; node = it.node; };
		T& operator*()const { return *cur; }
		T* operator->()const { return &(operator*()); }
		
		self& operator++()
		{
			++cur;
			if (cur == last)
			{
				++node;
				first = *node;
				last = first + getBufSiz();
				cur = first;
			}
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			++* this;
			return tmp;
		}
		self& operator--() {
			if (cur == first)
			{
				node = node - 1;
				first = *node;
				last = first + getBufSiz();
				cur = last;
			}
			cur--;
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			--* this;
			return tmp;
		}
		self& operator+=(size_t n){
			size_t bufSize = getBufSiz();//获取每个buf的大小
			size_t offset = (size_t)(cur - first) + n; 
			if (offset < bufSize&&offset>=0)//表示在同一buf内
				cur += n;
			else {
				if (offset > 0){
					size_t buf_num = offset / bufSize;//跨越的buf数
					size_t ele_num = offset % bufSize;//在新buf的位置
					node = node + buf_num;
					first = *node;
					last = first + bufSize;
					cur = first + ele_num;
				}
				
			}
			return *this;
		}
		self operator+(size_t n) const { self tmp = *this; return tmp += n; }; 
		self& operator-= (size_t n) 
		{
			size_t bufSize = getBufSiz();//获取每个buf的大小
			size_t offset = (size_t)(cur - first);//当前位置距离开头
			if (n<=offset)//表示在同一buf内
				cur -= n;
			else {
				offset = n-offset;
				size_t buf_num = offset / bufSize + 1;//跨越的buf数
				size_t ele_num = offset % bufSize;//在新buf的位置
				node = node - buf_num;
				first = *node;
				last = first + bufSize;
				cur = last - ele_num;
			}
			return *this;
		}
		self operator-(size_t n) { self tmp = *this; return tmp -= n; }
		size_t operator-(const self& x) const {
			size_t node_offset = (size_t)(this->node - x.node) + 1;		//两个迭代器之间共有多少个buf(包括本身，在同一个为1)
			size_t first_offset = (size_t)(x.cur - x.first);	//减 迭代器cur距离first
			size_t last_offset = (size_t)(this->last - this->cur);	//被减 迭代器 的cur距离last
			return node_offset * getBufSiz() - first_offset - last_offset;
		}
		bool operator ==(const self& x) const {
			return this->first == x.first &&
				this->last == x.last &&
				this->node == x.node &&
				this->cur == x.cur;
		}
		bool operator !=(const self& x)const
		{
			return !operator==(x);
		}
		T& operator[](size_t n)const { return *(*this + n); }
	};

	template<class T,class Alloc,size_t BufSiz>
	class deque
	{
	public:
		typedef dequeIterator<T, BufSiz> iterator;
	protected:
		iterator start;
		iterator finish;
		std::vector<T*> * map;
	private:
		size_t getBufSize()
		{
			if (BufSiz == 0)
				return sizeof(T) > 512 ? 1 : 512 / sizeof(T);
			return BufSiz;
		}
	public:
		iterator begin()const { return start; }
		iterator end()const { return finish; }
		size_t	size() const { return finish - start; } 
		bool empty() const { return finish == start; }
		const T& operator[] (size_t n)const { iterator it = this->begin(); return *(it + n); }
		const T& front() const { return *start; }
		const T& back() const { iterator tmp = finish; tmp--; return *tmp; }
		 T& operator[] (size_t n) { iterator it = this->begin(); return *(it + n); }
		 T& front()  { return *start; }
		 T& back()  { iterator tmp = finish; tmp--; return *tmp; }

		//构造函数：
		deque();	//默认构造
		deque(iterator beg,iterator end);	//构造函数将[beg,end)区间中的元素拷贝给本身
		deque(size_t n,T elem);		//构造函数将n个elem拷贝给本身
		deque(const deque& deq);	//拷贝构造函数
		//赋值操作：
		void assign(iterator beg,iterator end);	//将[beg,end)区间中的数据拷贝赋值给本身
		void assign(size_t n,T elem);		// 将n个elem拷贝赋值给本身
		deque<T, Alloc, BufSiz>& operator= (const deque<T, Alloc, BufSiz> & deq);		//重载等号操作符
		void swap(deque<T, Alloc, BufSiz>& deq);			//将deq与本身的元素互
		//大小操作:
		void resize(size_t num,T elem=T()) ;	//以elem填充
		//双端插入和删除操作:
		void push_back(T elem);	//在容器尾部添加一个数据
		void push_front(T elem);	//在容器头部插入一个数据
		void pop_back();			//删除容器最后一个数据
		void pop_front();		//删除容器第一个数据
	};
	//构造函数：
	template <class T, class Alloc, size_t BufSiz>
	deque<T,Alloc,BufSiz>::deque() //默认构造
	{
		map = new std::vector<T*>;
		(*map).push_back(Alloc().allocate(getBufSize()));
		start.node = &(*map)[0];
		start.first = *start.node;
		start.last = start.first + getBufSize();
		start.cur = start.first;
		finish = start;
	}
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>::deque(iterator beg, iterator end)	//构造函数将[beg,end)区间中的元素拷贝给本身
	{
		size_t num = (size_t)(end - beg);
		size_t buf_num = num / getBufSize()+1;
		size_t ele_num = num % getBufSize();
		map = new std::vector<T*>(buf_num); //map指针指向新分配的map空间
		for (size_t i = 0; i < buf_num; i++)  //每一个节点指向对应分配的buf
			(*map)[i] = Alloc().allocate(getBufSize());
		start.node = &(*map)[0];
		start.first = *start.node;
		start.last = start.first + getBufSize();
		start.cur = start.first;

		finish.node = &(*map).back();
		finish.first = *finish.node;
		finish.last = finish.first + getBufSize();
		finish.cur = finish.first + ele_num;

		iterator it = this->begin();
		while (it != this->end())
		{
			*it = *beg;
			it++;
			beg++;
		}
	}	
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>::deque(size_t n, T elem)		//构造函数将n个elem拷贝给本身
	{
		size_t buf_num = n / getBufSize() + 1;
		size_t ele_num = n % getBufSize();
		map = new std::vector<T*>(buf_num);
		for (size_t i = 0; i < buf_num; i++)
			(*map)[i] = Alloc().allocate(getBufSize());
		start.node = &(*map)[0];
		start.first = *start.node;
		start.last = start.first + getBufSize();
		start.cur = start.first;

		finish.node = &(*map).back();
		finish.first = *finish.node;
		finish.last = finish.first + getBufSize();
		finish.cur = finish.first + ele_num;

		iterator it = begin();
		while (n)
		{
			*it = elem;
			it++;
			n--;
		}
	}
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>::deque(const deque& deq)	//拷贝构造函数
	{
		iterator beg = deq.begin();//得到beg和end 后函数与（beg，end）构造函数相同
		iterator end = deq.end();
		size_t num = (size_t)(end - beg);
		size_t buf_num = num / getBufSize() + 1;
		size_t ele_num = num % getBufSize();
		map = new std::vector<T*>(buf_num); //map指针指向新分配的map空间
		for (size_t i = 0; i < buf_num; i++)  //每一个节点指向对应分配的buf
			(*map)[i] = Alloc().allocate(getBufSize());
		start.node = &(*map)[0];
		start.first = *start.node;
		start.last = start.first + getBufSize();
		start.cur = start.first;

		finish.node = &(*map).back();
		finish.first = *finish.node;
		finish.last = finish.first + getBufSize();
		finish.cur = finish.first + ele_num;

		iterator it = this->begin();
		while (it != this->end())
		{
			*it = *beg;
			it++;
			beg++;
		}
	}
	//赋值操作：
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::assign(iterator beg, iterator end)	//将[beg,end)区间中的数据拷贝赋值给本身
	{
		//回收原有内存
		for (size_t i = 0; i < (*map).size();i++)//回收map所指向内容所指向的空间
			Alloc().deallocate((*map)[i]);
		delete map;
		//创建新空间
		size_t num = (size_t)(end - beg);
		size_t buf_num = num / getBufSize() + 1;
		size_t ele_num = num % getBufSize();
		map = new std::vector<T*>(buf_num); //map指针指向新分配的map空间
		for (size_t i = 0; i < buf_num; i++)  //每一个节点指向对应分配的buf
			(*map)[i] = Alloc().allocate(getBufSize());
		start.node = &(*map)[0];
		start.first = *start.node;
		start.last = start.first + getBufSize();
		start.cur = start.first;

		finish.node = &(*map).back();
		finish.first = *finish.node;
		finish.last = finish.first + getBufSize();
		finish.cur = finish.first + ele_num;

		iterator it = this->begin();
		while (it != this->end())
		{
			*it = *beg;
			it++;
			beg++;
		}
	}
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::assign(size_t n, T elem)		// 将n个elem拷贝赋值给本身
	{
		deque<T, Alloc, BufSiz> tmp(n, elem);
		iterator beg = tmp.begin();
		iterator end = tmp.end();
		assign(beg, end);
	}
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>& deque<T, Alloc, BufSiz>::operator= (const deque<T, Alloc, BufSiz>& deq)		//重载等号操作符
	{
		if (&deq == this)
			return *this;
		iterator beg = deq.begin();
		iterator end = deq.end();
		assign(beg, end);
		return *this;
	}

	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::swap(deque<T, Alloc, BufSiz>& deq)			//将deq与本身的元素互
	{
		iterator start_copy = start;
		iterator finish_copy = finish;
		std::vector<T*>* map_copy = map;
		start = deq.start;
		finish = deq.finish;
		map = deq.map;
		deq.start = start_copy;
		deq.finish = finish_copy;
		deq.map = map_copy;
	}

	//大小操作:
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::resize(size_t num, T elem)		//重新指定容器的长度，如果变长，则以默认值填充，
															//如果变短，则末尾超出容器长度的元素被删除
	{
		size_t old_size = size();
		if (num <= old_size)
		{
			finish -= (old_size-num);
		}
		else
		{ //为了方便，只向后扩展，如果头部还有剩余空间也不会用
			size_t used_capacity = size();
			size_t finish_offset=(size_t)(finish.last-finish.cur);
			size_t available_capacity =finish_offset;
			num -= used_capacity;//得到新增加的个数
			if (available_capacity >= num)//不用再分配空间
			{
				while (num>0)
				{
					*finish = elem;
					finish.cur++;
					num--;
				}
			}
			else
			{
				while (finish.cur != finish.last)//填充剩余空间
				{
					*finish = elem;
					finish.cur++;
					num--;
				}
				//分配剩下所需要的空间
				size_t buf_num=num/getBufSize()+1;//需要分配的buf数
				size_t elem_num = num % getBufSize();//最后一个buf存的elem数
				//分配新空间，并扩充map，让其元素指向新的空间
				for (size_t i = 0; i < buf_num; i++)
				{
					map->push_back(Alloc().allocate(getBufSize()));
				}
				//修改start,finish
				size_t offset = start.cur - start.first;
				start.node = &((*map).front());
				start.first = *start.node;
				start.last = start.first + getBufSize();
				start.cur = start.first + elem_num;

				finish.node = &((*map).back());
				finish.first = *finish.node;
				finish.last = finish.first + getBufSize();
				finish.cur = finish.first + elem_num;
			}
		}
	}
	//双端插入和删除操作:
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_back(T elem)	//在容器尾部添加一个数据
	{
		if (finish.cur != finish.last)//如果还有空间
		{
			*finish.cur = elem;
			finish.cur++;
		}
		else
		{
			(*map).push_back(Alloc().allocate(getBufSize()));
			size_t offset = start.cur - start.first;//得到cur距离first的位置
			start.node = &(*map).front();
			start.first = *start.node;
			start.last = start.first + getBufSize();
			start.cur = start.first + offset;

			finish.node = &(*map).back();
			finish.first = *finish.node;
			finish.last = finish.first + getBufSize();
			finish.cur = finish.first;
			*finish = elem;
			finish.cur++;
		}

	}
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_front(T elem)	//在容器头部插入一个数据
	{
		if (start.cur != start.first)
		{
			start.cur--;
			*start = elem;
		}
		else
		{
			(*map).insert((*map).begin(), Alloc().allocate(getBufSize()));
			start.node = &(*map)[0];
			start.first = *start.node;
			start.last = start.first + getBufSize();
			start.cur = start.last-1;
			*(start.cur) = elem;
			size_t offset = finish.cur - finish.first;//得到cur距离first的位置
			finish.node = &(*map).back();
			finish.first = *finish.node;
			finish.last = finish.first + getBufSize();
			finish.cur = finish.first + offset;
		}
	}

	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::pop_back()			//删除容器最后一个数据
	{
		finish--;
		//如果finish指向了buf的最后说明上一个buf已经空了
		if (finish.cur == finish.last)
		{
			Alloc().deallocate((*map).back());
			(*map).pop_back();
		}
	}
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::pop_front()//删除容器第一个数据
	{
		start++;
		if (start.cur == start.last)
		{
			Alloc().deallocate((*map).front());
			(*map).erase((*map).begin());
		}
	}	
}

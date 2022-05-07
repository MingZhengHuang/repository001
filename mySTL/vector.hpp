#pragma once
#include"allocator.h"
namespace hmz
{
	template <class T, class Alloc = allocator<T>>
	class vector
	{
	public:
		typedef T* iterator;
	protected:
		iterator start;
		iterator finish;
		iterator end_of_storage;
	private:
		void uninitialized_copy(iterator start, iterator end, iterator new_start, iterator new_end);//将start到end的元素复制到new_start,如果空间不够抛出异常-1;
		void copy_backward(iterator position, iterator new_position);//将从positon开始的元素移动到new_position;，如果空间不够，抛出异常-1
		void copy_forward(iterator position, iterator new_position);//将从positon开始的元素移动到new_position;向前
		void insert_aux(iterator position, const T& x);//向position位置插入x;不够会创建新的空间
	public:
		iterator begin() const { return start; }
		iterator end() const { return finish; }
		//构造函数：
		vector(); 					//构造函数
		vector(iterator beg, iterator end);				//将[beg,end)区间中的元素拷贝给本身
		vector(size_t n);
		vector(size_t n, const T& elem);				//构造函数将n个elem拷贝给本身
		vector(const vector& vec);	//拷贝构造函数
		//赋值：
		vector<T>& operator=(const vector<T>& vec);//重载赋值运算符
		void assign(iterator beg, iterator end);						//将[beg,end)区间的数据拷贝赋值给本身
		void assign(size_t n, const T& elem);						//将n个elem拷贝赋值给本身
		//容量大小：
		bool empty() const { return begin() == end(); }						//判断容器是否为空
		size_t capacity() const { return (size_t)(end_of_storage - start); }  //容器的容量
		size_t	size() const { return (size_t)(end() - begin()); }						//返回容器中元素的个数
		//void resize(size_t num);				//重新制定容器的长度。变长用默认值填充，变短则末尾超出容器长度的元素被删除
		void resize(size_t num, const T& elem = T());			//变长用elem填充
		//插入和删除：
		void push_back(const T& elem);								//尾部插入元素elem	
		void pop_back() { finish--; }									//删除最后一个元素
		iterator insert(iterator pos, const T& elem) { insert_aux(pos, elem); return pos; }				//迭代器指向位置pos插入元素elem，并返回新元素的位置
		iterator insert(iterator pos, size_t count, const T& elem);		//迭代器指向位置pos插入count个元素
		void erase(iterator pot);						//删除迭代器指向元素
		void erase(iterator beg, iterator end);								//删除迭代器从beg到end之间的元素
		void clear() { finish = start; };										//删除容器所有元素
		//数据存取：
		T& at(size_t index) { if (index > size()) throw - 1; return *(begin() + index); }					//返回索引所致的数据
		T& operator[](size_t n) { return *(begin() + n); }			//返回索引所指的数据
		T& front() { return *begin(); }					//返回第一个
		T& back() { return *(end() - 1); }					//返回最后一个
		//互换容器：
		void swap(vector<T>& v);				//将vec与本身进行交换
		//预留空间：
		void reserve(size_t len);			//容器预留len个元素长度，预留位置不初始化，元素不可访问
	};
	template <class T, class Alloc>
	void vector<T, Alloc>::copy_backward(iterator position, iterator new_position)//将从positon开始的元素移动到new_position;，如果空间不够，抛出异常-1
	{
		size_t num = (size_t)(finish - position);//需要移动的元素
		size_t gap = (size_t)(new_position - position);//获取移动步长
		if (new_position + num > end_of_storage)//空间不够
			throw - 1;
		for (iterator it = finish - 1; it >= position; it--)
			*(it + gap) = *it;
		finish += gap;
	}
	template <class T, class Alloc>
	void vector<T, Alloc>::copy_forward(iterator position, iterator new_position)//将从positon开始的元素移动到new_position;
	{
		size_t num = (size_t)(finish - position);//需要移动的元素
		size_t gap = (size_t)(position - new_position);//获取移动步长
		for (iterator it = position; it != finish; it++)
			*(it - gap) = *it;
		finish -= gap;
	}
	template <class T, class Alloc>
	void vector<T, Alloc>::uninitialized_copy(iterator start, iterator end, iterator new_start, iterator new_end)//将start到end的元素复制到new_start,如果空间不够抛出异常-1;
	{
		size_t old_len = (size_t)(end - start);
		size_t new_len = (size_t)(new_end - new_start);
		if (old_len > new_len)
			throw - 1;
		for (iterator it = new_start; start != end; it++, start++)
			*it = *start;
	}
	template <class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const T& x)//向position位置插入x;不够会创建新的空间
	{
		if (finish != end_of_storage)//还有存储空间
		{
			//从position位置开始向后移动
			copy_backward(position, position + 1);
			*position = x;
		}
		else
		{
			const size_t old_size = size();
			const size_t len = old_size > 1 ? (size_t)(1.5 * old_size) : old_size + 1;

			iterator new_start = Alloc().allocate(len);
			iterator new_finish = new_start;
			try {
				new_finish = new_start + (size_t)(position - start);
				//将插入位置之前的复制到新空间；
				uninitialized_copy(start, position, new_start, new_finish);
				// 创建一个新元素插入
				*new_finish = x;
				new_finish++;
				//保留new_finish;
				iterator new_finish_copy = new_finish;
				//将剩下的元素复制到新空间
				new_finish += finish - position;
				uninitialized_copy(position, finish, new_finish_copy, new_finish);
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
			catch (...)//失败
			{
				throw - 1;
			}
		}
	}

	//构造//////////
	template <class T, class Alloc>
	vector<T, Alloc>::vector()
	{
		start = 0;
		finish = 0;
		end_of_storage = 0;
	}
	template <class T, class Alloc>
	vector<T, Alloc>::vector(iterator beg, iterator end)				//将[beg,end)区间中的元素拷贝给本身
	{
		size_t num = (size_t)(end - beg);//获取需要分配的空间
		start = Alloc().allocate(num);
		finish = start + num;
		end_of_storage = finish;
		for (iterator it = start; it != finish; it++, beg++)
			*it = *beg;
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(size_t n, const T& elem)				//构造函数将n个elem拷贝给本身
	{
		start = Alloc().allocate(n);
		finish = start + n;
		end_of_storage = finish;
		for (iterator it = start; it != finish; it++)
			*it = elem;
	}
	template <class T, class Alloc>
	vector<T, Alloc>::vector(size_t n)				//构造函数将n个默认元素拷贝给本身
	{
		start = Alloc().allocate(n);
		finish = start + n;
		end_of_storage = finish;
		for (iterator it = start; it != finish; it++)
			*it = T();
	}
	template <class T, class Alloc>
	vector< T, Alloc>::vector(const vector& vec)	//拷贝构造函数*/
	{
		size_t num = (size_t)(vec.finish - vec.start);
		start = Alloc().allocate(num);
		finish = start + num;
		end_of_storage = finish;
		for (iterator it = start, oldIt = vec.begin(); it != finish; it++, oldIt++)
			*it = *oldIt;
	}


	//赋值：
	template <class T, class Alloc>
	vector<T>& vector< T, Alloc>::operator=(const vector<T>& vec)//重载赋值运算符
	{
		size_t new_size, new_capacity, old_size, old_capacity;
		iterator it, it2, new_start;
		new_size = vec.size();
		old_size = size();
		old_capacity = capacity();
		//如果原有空间足够容纳新数据，就直接进行赋值。否则重新分配空间后赋值。
		if (new_size <= old_capacity)
		{
			for (it = start, it2 = vec.begin(); it2 != vec.end(); it2++, it++)
				*it = *it2;
			finish = it;
		}
		else
		{
			if (new_size <= size_t(1.5 * old_capacity))
				new_capacity = (size_t)(1.5 * capacity());
			else
				new_capacity = new_size;
			new_start = Alloc().allocate(new_capacity);
			Alloc().deallocate(start);
			start = new_start;
			finish = start + new_size;
			end_of_storage = start + new_capacity;
			uninitialized_copy(vec.begin(), vec.end(), start, finish);
		}
		return *this;
	}
	template <class T, class Alloc>
	void vector< T, Alloc>::assign(iterator beg, iterator end)						//将[beg,end)区间的数据拷贝赋值给本身
	{
		size_t new_size,new_capacity,old_size,old_capacity;
		iterator it,it2,new_start;
		new_size = (size_t)(end - beg);
		old_size = size();
		old_capacity = capacity();
		//如果原有空间足够容纳新数据，就直接进行赋值。否则重新分配空间后赋值。
		if (new_size <= old_capacity)
		{
			for (it = start, it2 = beg; it2 != end; it2++,it++)
				*it = *it2;
			finish = it;
		}
		else
		{
			if (new_size <= size_t(1.5 * old_capacity))
				new_capacity = (size_t)(1.5 * capacity());
			else
				new_capacity = new_size;
			new_start = Alloc().allocate(new_capacity);
			Alloc().deallocate(start);
			start = new_start;
			finish = start+new_size;
			end_of_storage = start + new_capacity;
			uninitialized_copy(beg, end, start, finish);
		}
			
	}
	template <class T, class Alloc>
	void vector< T, Alloc>::assign(size_t n, const T& elem)						//将n个elem拷贝赋值给本身
	{
		size_t new_capacity,i;
		iterator it;
		if (n <= capacity())
		{
			for (it=begin(),i=0;i<n;i++,it++)
				*it = elem;
			finish = it;
		}
		else
		{
			if (n <= (size_t)(1.5 * capacity()))
				new_capacity = (size_t)(1.5 * capacity());
			else
				new_capacity = n;
			Alloc().deallocate(start);
			start = Alloc().allocate(new_capacity);
			finish = start + n;
			end_of_storage = start + new_capacity;
			for (it = begin(),i = 0; i < n; i++, it++)
				*it = elem;
		}
	}

	template <class T, class Alloc>
	void vector< T, Alloc>::resize(size_t num, const T& elem)			//变长用elem填充
	{
		size_t old_size = (size_t)(finish - start);
		size_t old_capacity = (size_t)(end_of_storage - start),new_capacity;
		iterator new_start,new_finish,it;
		if (old_size >= num)
		{
			finish = start + num;
		}
		else if (old_capacity >= num)
		{
			it = finish;
			finish = start + num;
			while (it != finish)
			{
				*it = elem;
				it++;
			}
		}
		else
		{
			if (num >= 1.5 * old_size)//如果新空间小于原来空间的1.5倍则增加至1.5倍
				new_capacity = (size_t)(1.5 * old_capacity);
			else	//如果所需空间大于原来的1.5倍，则增长至所需空间
				new_capacity = num;
			new_start = Alloc().allocate(new_capacity);
			new_finish = new_start + old_size;
			uninitialized_copy(start, finish, new_start, new_finish);//复制原数据
			//删除原有空间
			Alloc().deallocate(start);
			it = new_finish;
			new_finish = new_start + num;//指向新的结尾
			while (it != new_finish)
			{
				*it = elem;
				it++;
			}
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + new_capacity;
		}
	}

	//插入和删除：
	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(const T& elem)							//尾部插入元素elem	
	{
		//如果还有空间
		if (end_of_storage != finish)
		{
			*finish = elem;
			finish++;
		}
		else
			insert_aux(finish, elem);
	}
	template <class T, class Alloc>
	T* vector<T, Alloc>::insert(iterator pos, size_t count, const T& elem)		//迭代器指向位置pos插入count个元素
	{
		size_t old_capacity, old_size, new_capacity;
		old_capacity = end_of_storage - start;
		old_size = finish - start;
		iterator new_start=start, new_finish=finish, new_position=pos;
		//如果空间够直接加
		//不够需要判断增加的大小
		if (old_capacity >= old_size + count)
		{
			while (count)
			{
				insert_aux(pos, elem);
				count--;
			}
		}
		else
		{
			if (old_size + count <= 1.5 * old_capacity)
				new_capacity = (size_t)(1.5 * old_capacity);
			else
				new_capacity = old_size + count;
			new_start = Alloc().allocate(new_capacity);
			new_finish = new_start + old_size;
			new_position = new_start + (size_t)(pos - start);
			uninitialized_copy(start, finish, new_start, new_finish);//复制之前数据
			Alloc().deallocate(start);
			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_capacity;
			while (count)
			{
				insert_aux(new_position, elem);
				count--;
			}
		}
		return new_position;
	}
	template <class T, class Alloc>
	void vector<T, Alloc>::erase(iterator position)						//删除迭代器指向元素
	{
		copy_forward(position + 1, position);
	}
	template <class T, class Alloc>
	void vector<T, Alloc>::erase(iterator beg, iterator end)								//删除迭代器从beg到end之间的元素
	{
		copy_forward(end, beg);
	}
	//互换容器：
	template <class T, class Alloc>
	void  vector<T, Alloc>::swap(vector<T> & v)				//将vec与本身进行交换
	{
		iterator start_copy, finish_copy, end_of_storage_copy;
		start_copy = start;
		finish_copy = finish;
		end_of_storage_copy = end_of_storage;
		start = v.start;
		finish = v.finish;
		end_of_storage = v.end_of_storage;
		v.start = start_copy;
		v.finish = finish_copy;
		v.end_of_storage = end_of_storage_copy;
	}
	//预留空间：
	template <class T, class Alloc>
	void vector<T, Alloc>::reserve(size_t len)			//容器预留len个元素长度，预留位置不初始化，元素不可访问
	{
		if (len > capacity())
			end_of_storage = start + len;
	}
}
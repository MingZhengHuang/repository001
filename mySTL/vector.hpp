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
		void uninitialized_copy(iterator start, iterator end, iterator new_start, iterator new_end);//��start��end��Ԫ�ظ��Ƶ�new_start,����ռ䲻���׳��쳣-1;
		void copy_backward(iterator position, iterator new_position);//����positon��ʼ��Ԫ���ƶ���new_position;������ռ䲻�����׳��쳣-1
		void copy_forward(iterator position, iterator new_position);//����positon��ʼ��Ԫ���ƶ���new_position;��ǰ
		void insert_aux(iterator position, const T& x);//��positionλ�ò���x;�����ᴴ���µĿռ�
	public:
		iterator begin() const { return start; }
		iterator end() const { return finish; }
		//���캯����
		vector(); 					//���캯��
		vector(iterator beg, iterator end);				//��[beg,end)�����е�Ԫ�ؿ���������
		vector(size_t n);
		vector(size_t n, const T& elem);				//���캯����n��elem����������
		vector(const vector& vec);	//�������캯��
		//��ֵ��
		vector<T>& operator=(const vector<T>& vec);//���ظ�ֵ�����
		void assign(iterator beg, iterator end);						//��[beg,end)��������ݿ�����ֵ������
		void assign(size_t n, const T& elem);						//��n��elem������ֵ������
		//������С��
		bool empty() const { return begin() == end(); }						//�ж������Ƿ�Ϊ��
		size_t capacity() const { return (size_t)(end_of_storage - start); }  //����������
		size_t	size() const { return (size_t)(end() - begin()); }						//����������Ԫ�صĸ���
		//void resize(size_t num);				//�����ƶ������ĳ��ȡ��䳤��Ĭ��ֵ��䣬�����ĩβ�����������ȵ�Ԫ�ر�ɾ��
		void resize(size_t num, const T& elem = T());			//�䳤��elem���
		//�����ɾ����
		void push_back(const T& elem);								//β������Ԫ��elem	
		void pop_back() { finish--; }									//ɾ�����һ��Ԫ��
		iterator insert(iterator pos, const T& elem) { insert_aux(pos, elem); return pos; }				//������ָ��λ��pos����Ԫ��elem����������Ԫ�ص�λ��
		iterator insert(iterator pos, size_t count, const T& elem);		//������ָ��λ��pos����count��Ԫ��
		void erase(iterator pot);						//ɾ��������ָ��Ԫ��
		void erase(iterator beg, iterator end);								//ɾ����������beg��end֮���Ԫ��
		void clear() { finish = start; };										//ɾ����������Ԫ��
		//���ݴ�ȡ��
		T& at(size_t index) { if (index > size()) throw - 1; return *(begin() + index); }					//�����������µ�����
		T& operator[](size_t n) { return *(begin() + n); }			//����������ָ������
		T& front() { return *begin(); }					//���ص�һ��
		T& back() { return *(end() - 1); }					//�������һ��
		//����������
		void swap(vector<T>& v);				//��vec�뱾����н���
		//Ԥ���ռ䣺
		void reserve(size_t len);			//����Ԥ��len��Ԫ�س��ȣ�Ԥ��λ�ò���ʼ����Ԫ�ز��ɷ���
	};
	template <class T, class Alloc>
	void vector<T, Alloc>::copy_backward(iterator position, iterator new_position)//����positon��ʼ��Ԫ���ƶ���new_position;������ռ䲻�����׳��쳣-1
	{
		size_t num = (size_t)(finish - position);//��Ҫ�ƶ���Ԫ��
		size_t gap = (size_t)(new_position - position);//��ȡ�ƶ�����
		if (new_position + num > end_of_storage)//�ռ䲻��
			throw - 1;
		for (iterator it = finish - 1; it >= position; it--)
			*(it + gap) = *it;
		finish += gap;
	}
	template <class T, class Alloc>
	void vector<T, Alloc>::copy_forward(iterator position, iterator new_position)//����positon��ʼ��Ԫ���ƶ���new_position;
	{
		size_t num = (size_t)(finish - position);//��Ҫ�ƶ���Ԫ��
		size_t gap = (size_t)(position - new_position);//��ȡ�ƶ�����
		for (iterator it = position; it != finish; it++)
			*(it - gap) = *it;
		finish -= gap;
	}
	template <class T, class Alloc>
	void vector<T, Alloc>::uninitialized_copy(iterator start, iterator end, iterator new_start, iterator new_end)//��start��end��Ԫ�ظ��Ƶ�new_start,����ռ䲻���׳��쳣-1;
	{
		size_t old_len = (size_t)(end - start);
		size_t new_len = (size_t)(new_end - new_start);
		if (old_len > new_len)
			throw - 1;
		for (iterator it = new_start; start != end; it++, start++)
			*it = *start;
	}
	template <class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const T& x)//��positionλ�ò���x;�����ᴴ���µĿռ�
	{
		if (finish != end_of_storage)//���д洢�ռ�
		{
			//��positionλ�ÿ�ʼ����ƶ�
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
				//������λ��֮ǰ�ĸ��Ƶ��¿ռ䣻
				uninitialized_copy(start, position, new_start, new_finish);
				// ����һ����Ԫ�ز���
				*new_finish = x;
				new_finish++;
				//����new_finish;
				iterator new_finish_copy = new_finish;
				//��ʣ�µ�Ԫ�ظ��Ƶ��¿ռ�
				new_finish += finish - position;
				uninitialized_copy(position, finish, new_finish_copy, new_finish);
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
			catch (...)//ʧ��
			{
				throw - 1;
			}
		}
	}

	//����//////////
	template <class T, class Alloc>
	vector<T, Alloc>::vector()
	{
		start = 0;
		finish = 0;
		end_of_storage = 0;
	}
	template <class T, class Alloc>
	vector<T, Alloc>::vector(iterator beg, iterator end)				//��[beg,end)�����е�Ԫ�ؿ���������
	{
		size_t num = (size_t)(end - beg);//��ȡ��Ҫ����Ŀռ�
		start = Alloc().allocate(num);
		finish = start + num;
		end_of_storage = finish;
		for (iterator it = start; it != finish; it++, beg++)
			*it = *beg;
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(size_t n, const T& elem)				//���캯����n��elem����������
	{
		start = Alloc().allocate(n);
		finish = start + n;
		end_of_storage = finish;
		for (iterator it = start; it != finish; it++)
			*it = elem;
	}
	template <class T, class Alloc>
	vector<T, Alloc>::vector(size_t n)				//���캯����n��Ĭ��Ԫ�ؿ���������
	{
		start = Alloc().allocate(n);
		finish = start + n;
		end_of_storage = finish;
		for (iterator it = start; it != finish; it++)
			*it = T();
	}
	template <class T, class Alloc>
	vector< T, Alloc>::vector(const vector& vec)	//�������캯��*/
	{
		size_t num = (size_t)(vec.finish - vec.start);
		start = Alloc().allocate(num);
		finish = start + num;
		end_of_storage = finish;
		for (iterator it = start, oldIt = vec.begin(); it != finish; it++, oldIt++)
			*it = *oldIt;
	}


	//��ֵ��
	template <class T, class Alloc>
	vector<T>& vector< T, Alloc>::operator=(const vector<T>& vec)//���ظ�ֵ�����
	{
		size_t new_size, new_capacity, old_size, old_capacity;
		iterator it, it2, new_start;
		new_size = vec.size();
		old_size = size();
		old_capacity = capacity();
		//���ԭ�пռ��㹻���������ݣ���ֱ�ӽ��и�ֵ���������·���ռ��ֵ��
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
	void vector< T, Alloc>::assign(iterator beg, iterator end)						//��[beg,end)��������ݿ�����ֵ������
	{
		size_t new_size,new_capacity,old_size,old_capacity;
		iterator it,it2,new_start;
		new_size = (size_t)(end - beg);
		old_size = size();
		old_capacity = capacity();
		//���ԭ�пռ��㹻���������ݣ���ֱ�ӽ��и�ֵ���������·���ռ��ֵ��
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
	void vector< T, Alloc>::assign(size_t n, const T& elem)						//��n��elem������ֵ������
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
	void vector< T, Alloc>::resize(size_t num, const T& elem)			//�䳤��elem���
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
			if (num >= 1.5 * old_size)//����¿ռ�С��ԭ���ռ��1.5����������1.5��
				new_capacity = (size_t)(1.5 * old_capacity);
			else	//�������ռ����ԭ����1.5����������������ռ�
				new_capacity = num;
			new_start = Alloc().allocate(new_capacity);
			new_finish = new_start + old_size;
			uninitialized_copy(start, finish, new_start, new_finish);//����ԭ����
			//ɾ��ԭ�пռ�
			Alloc().deallocate(start);
			it = new_finish;
			new_finish = new_start + num;//ָ���µĽ�β
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

	//�����ɾ����
	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(const T& elem)							//β������Ԫ��elem	
	{
		//������пռ�
		if (end_of_storage != finish)
		{
			*finish = elem;
			finish++;
		}
		else
			insert_aux(finish, elem);
	}
	template <class T, class Alloc>
	T* vector<T, Alloc>::insert(iterator pos, size_t count, const T& elem)		//������ָ��λ��pos����count��Ԫ��
	{
		size_t old_capacity, old_size, new_capacity;
		old_capacity = end_of_storage - start;
		old_size = finish - start;
		iterator new_start=start, new_finish=finish, new_position=pos;
		//����ռ乻ֱ�Ӽ�
		//������Ҫ�ж����ӵĴ�С
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
			uninitialized_copy(start, finish, new_start, new_finish);//����֮ǰ����
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
	void vector<T, Alloc>::erase(iterator position)						//ɾ��������ָ��Ԫ��
	{
		copy_forward(position + 1, position);
	}
	template <class T, class Alloc>
	void vector<T, Alloc>::erase(iterator beg, iterator end)								//ɾ����������beg��end֮���Ԫ��
	{
		copy_forward(end, beg);
	}
	//����������
	template <class T, class Alloc>
	void  vector<T, Alloc>::swap(vector<T> & v)				//��vec�뱾����н���
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
	//Ԥ���ռ䣺
	template <class T, class Alloc>
	void vector<T, Alloc>::reserve(size_t len)			//����Ԥ��len��Ԫ�س��ȣ�Ԥ��λ�ò���ʼ����Ԫ�ز��ɷ���
	{
		if (len > capacity())
			end_of_storage = start + len;
	}
}
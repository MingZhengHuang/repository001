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
			size_t bufSize = getBufSiz();//��ȡÿ��buf�Ĵ�С
			size_t offset = (size_t)(cur - first) + n; 
			if (offset < bufSize&&offset>=0)//��ʾ��ͬһbuf��
				cur += n;
			else {
				if (offset > 0){
					size_t buf_num = offset / bufSize;//��Խ��buf��
					size_t ele_num = offset % bufSize;//����buf��λ��
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
			size_t bufSize = getBufSiz();//��ȡÿ��buf�Ĵ�С
			size_t offset = (size_t)(cur - first);//��ǰλ�þ��뿪ͷ
			if (n<=offset)//��ʾ��ͬһbuf��
				cur -= n;
			else {
				offset = n-offset;
				size_t buf_num = offset / bufSize + 1;//��Խ��buf��
				size_t ele_num = offset % bufSize;//����buf��λ��
				node = node - buf_num;
				first = *node;
				last = first + bufSize;
				cur = last - ele_num;
			}
			return *this;
		}
		self operator-(size_t n) { self tmp = *this; return tmp -= n; }
		size_t operator-(const self& x) const {
			size_t node_offset = (size_t)(this->node - x.node) + 1;		//����������֮�乲�ж��ٸ�buf(����������ͬһ��Ϊ1)
			size_t first_offset = (size_t)(x.cur - x.first);	//�� ������cur����first
			size_t last_offset = (size_t)(this->last - this->cur);	//���� ������ ��cur����last
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

		//���캯����
		deque();	//Ĭ�Ϲ���
		deque(iterator beg,iterator end);	//���캯����[beg,end)�����е�Ԫ�ؿ���������
		deque(size_t n,T elem);		//���캯����n��elem����������
		deque(const deque& deq);	//�������캯��
		//��ֵ������
		void assign(iterator beg,iterator end);	//��[beg,end)�����е����ݿ�����ֵ������
		void assign(size_t n,T elem);		// ��n��elem������ֵ������
		deque<T, Alloc, BufSiz>& operator= (const deque<T, Alloc, BufSiz> & deq);		//���صȺŲ�����
		void swap(deque<T, Alloc, BufSiz>& deq);			//��deq�뱾���Ԫ�ػ�
		//��С����:
		void resize(size_t num,T elem=T()) ;	//��elem���
		//˫�˲����ɾ������:
		void push_back(T elem);	//������β�����һ������
		void push_front(T elem);	//������ͷ������һ������
		void pop_back();			//ɾ���������һ������
		void pop_front();		//ɾ��������һ������
	};
	//���캯����
	template <class T, class Alloc, size_t BufSiz>
	deque<T,Alloc,BufSiz>::deque() //Ĭ�Ϲ���
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
	deque<T, Alloc, BufSiz>::deque(iterator beg, iterator end)	//���캯����[beg,end)�����е�Ԫ�ؿ���������
	{
		size_t num = (size_t)(end - beg);
		size_t buf_num = num / getBufSize()+1;
		size_t ele_num = num % getBufSize();
		map = new std::vector<T*>(buf_num); //mapָ��ָ���·����map�ռ�
		for (size_t i = 0; i < buf_num; i++)  //ÿһ���ڵ�ָ���Ӧ�����buf
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
	deque<T, Alloc, BufSiz>::deque(size_t n, T elem)		//���캯����n��elem����������
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
	deque<T, Alloc, BufSiz>::deque(const deque& deq)	//�������캯��
	{
		iterator beg = deq.begin();//�õ�beg��end �����루beg��end�����캯����ͬ
		iterator end = deq.end();
		size_t num = (size_t)(end - beg);
		size_t buf_num = num / getBufSize() + 1;
		size_t ele_num = num % getBufSize();
		map = new std::vector<T*>(buf_num); //mapָ��ָ���·����map�ռ�
		for (size_t i = 0; i < buf_num; i++)  //ÿһ���ڵ�ָ���Ӧ�����buf
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
	//��ֵ������
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::assign(iterator beg, iterator end)	//��[beg,end)�����е����ݿ�����ֵ������
	{
		//����ԭ���ڴ�
		for (size_t i = 0; i < (*map).size();i++)//����map��ָ��������ָ��Ŀռ�
			Alloc().deallocate((*map)[i]);
		delete map;
		//�����¿ռ�
		size_t num = (size_t)(end - beg);
		size_t buf_num = num / getBufSize() + 1;
		size_t ele_num = num % getBufSize();
		map = new std::vector<T*>(buf_num); //mapָ��ָ���·����map�ռ�
		for (size_t i = 0; i < buf_num; i++)  //ÿһ���ڵ�ָ���Ӧ�����buf
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
	void deque<T, Alloc, BufSiz>::assign(size_t n, T elem)		// ��n��elem������ֵ������
	{
		deque<T, Alloc, BufSiz> tmp(n, elem);
		iterator beg = tmp.begin();
		iterator end = tmp.end();
		assign(beg, end);
	}
	template <class T, class Alloc, size_t BufSiz>
	deque<T, Alloc, BufSiz>& deque<T, Alloc, BufSiz>::operator= (const deque<T, Alloc, BufSiz>& deq)		//���صȺŲ�����
	{
		if (&deq == this)
			return *this;
		iterator beg = deq.begin();
		iterator end = deq.end();
		assign(beg, end);
		return *this;
	}

	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::swap(deque<T, Alloc, BufSiz>& deq)			//��deq�뱾���Ԫ�ػ�
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

	//��С����:
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::resize(size_t num, T elem)		//����ָ�������ĳ��ȣ�����䳤������Ĭ��ֵ��䣬
															//�����̣���ĩβ�����������ȵ�Ԫ�ر�ɾ��
	{
		size_t old_size = size();
		if (num <= old_size)
		{
			finish -= (old_size-num);
		}
		else
		{ //Ϊ�˷��㣬ֻ�����չ�����ͷ������ʣ��ռ�Ҳ������
			size_t used_capacity = size();
			size_t finish_offset=(size_t)(finish.last-finish.cur);
			size_t available_capacity =finish_offset;
			num -= used_capacity;//�õ������ӵĸ���
			if (available_capacity >= num)//�����ٷ���ռ�
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
				while (finish.cur != finish.last)//���ʣ��ռ�
				{
					*finish = elem;
					finish.cur++;
					num--;
				}
				//����ʣ������Ҫ�Ŀռ�
				size_t buf_num=num/getBufSize()+1;//��Ҫ�����buf��
				size_t elem_num = num % getBufSize();//���һ��buf���elem��
				//�����¿ռ䣬������map������Ԫ��ָ���µĿռ�
				for (size_t i = 0; i < buf_num; i++)
				{
					map->push_back(Alloc().allocate(getBufSize()));
				}
				//�޸�start,finish
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
	//˫�˲����ɾ������:
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::push_back(T elem)	//������β�����һ������
	{
		if (finish.cur != finish.last)//������пռ�
		{
			*finish.cur = elem;
			finish.cur++;
		}
		else
		{
			(*map).push_back(Alloc().allocate(getBufSize()));
			size_t offset = start.cur - start.first;//�õ�cur����first��λ��
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
	void deque<T, Alloc, BufSiz>::push_front(T elem)	//������ͷ������һ������
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
			size_t offset = finish.cur - finish.first;//�õ�cur����first��λ��
			finish.node = &(*map).back();
			finish.first = *finish.node;
			finish.last = finish.first + getBufSize();
			finish.cur = finish.first + offset;
		}
	}

	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::pop_back()			//ɾ���������һ������
	{
		finish--;
		//���finishָ����buf�����˵����һ��buf�Ѿ�����
		if (finish.cur == finish.last)
		{
			Alloc().deallocate((*map).back());
			(*map).pop_back();
		}
	}
	template <class T, class Alloc, size_t BufSiz>
	void deque<T, Alloc, BufSiz>::pop_front()//ɾ��������һ������
	{
		start++;
		if (start.cur == start.last)
		{
			Alloc().deallocate((*map).front());
			(*map).erase((*map).begin());
		}
	}	
}

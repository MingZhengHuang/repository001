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
		listIterator(const listIterator<T>& n);//�������캯��
		listIterator& operator=(const listIterator<T>& n);//���ظ�ֵ�����
		listIterator& operator++();//����ǰ��++��
		listIterator  operator++(int);//���غ���++��
		listIterator& operator--();//����ǰ��--��
		listIterator  operator--(int);//���غ���--��
		bool		  operator!=(const listIterator<T>& L);//
		T			  operator*() const;//
		T* operator->() const;

		friend list<T>;//��Ϊ�������Ϊ��list<T>����ģ�������������Ϊ��Ԫ��
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







	template <class T, class Alloc>//����������
	class list
	{
	public:
		typedef listIterator<T> iterator;
	protected:
		listNode<T>* node;//ָ��end��ʲô���ݶ�û�У�
	public:
		//���캯����
		list();
		list(iterator beg, iterator end);						//���캯����[beg,end)�����е�Ԫ�ؿ���������
		list(size_t n, const T& elem);									//���캯����n��elem����������
		list(size_t n);
		list(const list<T, Alloc>& l);									//�������캯��
		//��������
		~list();												//��������
		//��ֵ�ͽ���
		void assign(iterator beg, iterator end);				//��[beg,end)�����е����ݿ�����ֵ������
		void assign(size_t n, const T& elem);								//��n��elem������ֵ������
		list<T, Alloc>& operator=(const list<T, Alloc>& L);								//���ظ�ֵ�����
		void swap(list& l);										//��l�뱾����	
		//��С������
		unsigned long size();									//����������Ԫ�صĸ���
		bool empty();											//�ж������Ƿ�Ϊ��
		void resize(size_t num);									//����ָ�������ĳ���Ϊnum���������䳤������Ĭ��ֵ�����λ�ã�
																//�����̣���ĩβ�����������ȵ�Ԫ�ر�ɾ��
		void resize(size_t num, const T& elem);							//����ָ�������ĳ���Ϊnum���������䳤������elemֵ�����λ�ã�
																//�����̣���ĩβ�����������ȵ�Ԫ�ر�ɾ��
		//�����ɾ��
		void push_back(const T& elem);									//������β������һ��Ԫ��
		void pop_back();										//ɾ�����������һ��Ԫ��
		void push_front(const T& elem);								//��������ͷ����һ��Ԫ��
		void pop_front();										//��������ͷ�Ƴ�һ��Ԫ��
		iterator insert(iterator pos, const T& elem);					//��posλ�ò���elemԪ�صĿ��������������ݵ�λ��
		void insert(iterator pos, size_t n, const T& elem);				//��posλ�ò���n��elem���ݣ��޷���ֵ
		void insert(iterator pos, iterator beg, iterator end);	//��posλ�ò���[beg,end)��������ݣ��޷���ֵ
		void clear();											//�Ƴ�������������
		iterator erase(iterator beg, iterator end);				//ɾ��[beg,end)��������ݣ�������һ�����ݵ�λ��
		iterator erase(iterator pos);							//ɾ��posλ�õ����ݣ�������һ�����ݵ�λ��
		void remove(const T& elem);									//ɾ��������������elemֵƥ���Ԫ�أ�
		//���ݴ�ȡ
		T front();												//���ص�һ��Ԫ��
		T back();												//�������һ��Ԫ��
		//��ת������
		void reverse();										    //��ת
		void sort();											//����
		iterator begin() { iterator t; t.node = this->node->next; return t; };
		iterator end() { iterator t; t.node = this->node;	 return t; };
		const iterator begin()const  { iterator t; t.node = this->node->next; return t; };
		const iterator end() const{ iterator t; t.node = this->node;	 return t; };
	};
	//���캯����
	template <class T, class Alloc>
	list<T, Alloc>::list()
	{
		Alloc allocator;
		this->node = allocator.allocate(1);
		this->node->next = this->node;
		this->node->pre = this->node;
	}
	template <class T, class Alloc>
	list<T, Alloc>::list(iterator beg, iterator end)						//���캯����[beg,end)�����е�Ԫ�ؿ���������
	{
		Alloc allocator;
		this->node = allocator.allocate(1);
		this->node->next = this->node;
		this->node->pre = this->node;

		while (beg != end)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = *beg;
			listNode<T>* lastNode = this->node->pre;//�ҵ����һ��
			//���½ڵ����
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
			beg++;
		}
	}
	template <class T, class Alloc>
	list<T, Alloc>::list(size_t n, const T& elem)									//���캯����n��elem����������
	{
		Alloc allocator;
		this->node = allocator.allocate(1);
		this->node->next = this->node;
		this->node->pre = this->node;

		for (unsigned int i = 0; i < n; i++)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = elem;
			listNode<T>* lastNode = this->node->pre;//�ҵ����һ��
			//���½ڵ����
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
		}
	}
	template <class T, class Alloc>
	list<T, Alloc>::list(size_t n)									//���캯����n��elem����������
	{
		Alloc allocator;
		this->node = allocator.allocate(1);
		this->node->next = this->node;
		this->node->pre = this->node;

		for (unsigned int i = 0; i < n; i++)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = T();
			listNode<T>* lastNode = this->node->pre;//�ҵ����һ��
			//���½ڵ����
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
		}
	}
	template <class T, class Alloc>
	list<T, Alloc>::list(const list<T, Alloc>& l)									//�������캯��
	{
		Alloc allocator;
		this->node = allocator.allocate(1);
		this->node->next = this->node;
		this->node->pre = this->node;

		listNode<T>* lpNode = l.node->next;//ָ��L�ĵ�һ���ڵ�
		while (lpNode != l.node)//����L���еĽڵ�
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = lpNode->data;
			listNode<T>* lastNode = this->node->pre;//�ҵ����һ��
			//���½ڵ����
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
			lpNode = lpNode->next;
		}
	}

	//��������
	template <class T, class Alloc>
	list<T, Alloc>::~list()
	{
		Alloc allocator;
		listNode<T>* pNode = this->node->next;//ָ���һ���ڵ�
		while (pNode != this->node)
		{
			listNode<T>* tNode = pNode->next;
			allocator.deallocate(pNode);
			pNode = tNode;
		}
		allocator.deallocate(this->node);
	}
	//��ֵ�ͽ���
	template<class T, class Alloc>
	void list<T, Alloc>::assign(iterator beg, iterator end)				//��[beg,end)�����е����ݿ�����ֵ������
	{
		Alloc allocator;
		this->clear();//ɾ��ԭ������
		while (beg != end)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = *beg;
			listNode<T>* lastNode = this->node->pre;//�ҵ����һ��
			//���½ڵ����
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
			beg++;
		}
	}
	template<class T, class Alloc>
	void list<T, Alloc>::assign(size_t n, const T& elem)								//��n��elem������ֵ������
	{
		Alloc allocator;
		this->clear();

		for (size_t i = 0; i < n; i++)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = elem;
			listNode<T>* lastNode = this->node->pre;//�ҵ����һ��
			//���½ڵ����
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
		}
	}
	template<class T, class Alloc>
	list<T, Alloc>& list<T, Alloc>::operator=(const list<T, Alloc>& l)							//���ظ�ֵ�����
	{
		Alloc allocator;
		this->clear();
		listNode<T>* lpNode = l.node->next;//ָ��L�ĵ�һ���ڵ�
		while (lpNode != l.node)//����L���еĽڵ�
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = lpNode->data;
			listNode<T>* lastNode = this->node->pre;//�ҵ����һ��
			//���½ڵ����
			pNode->next = this->node;
			pNode->pre = lastNode;
			lastNode->next = pNode;
			this->node->pre = pNode;
			lpNode = lpNode->next;
		}
		return *this;
	}
	template<class T, class Alloc>
	void list<T, Alloc>::swap(list& l)										//��l�뱾����	
	{
		//ֱ�ӽ�������list��node
		listNode<T>* tNode = this->node;
		this->node = l.node;
		l.node = tNode;
	}

	//��С������
	template <class T, class Alloc>
	unsigned long list<T, Alloc>::size()									//����������Ԫ�صĸ���
	{
		listNode<T>* pNode = this->node->next;//ָ���һ��
		unsigned long num = 0;
		while (pNode != this->node)
		{
			num++;
			pNode = pNode->next;
		}
		return num;
	}
	template <class T, class Alloc>
	bool list<T, Alloc>::empty()											//�ж������Ƿ�Ϊ��
	{
		return this->node->next == this->node;
	}
	template <class T, class Alloc>
	void list<T, Alloc>::resize(size_t num, const T& elem)	//����ָ�������ĳ���Ϊnum���������䳤������elemֵ�����λ�ã������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ��
	{
		//�ҵ����һ���������ʱnumΪ0��ֱ�ӽ���ѭ����
		listNode<T>* pNode = this->node->next;//ָ���һ����
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
		if (num == 0)//˵�����
		{

			listNode<T>* tNode = pNode;
			pNode = pNode->next;//ת����һ���ڵ�
			while (pNode != this->node)
			{
				listNode<T>* nNode = pNode->next;//��¼��ǰ��Ҫɾ���ڵ����һ���ڵ�
				allocator.deallocate(pNode);//ɾ����ǰ�ڵ�
				pNode = nNode;
			}
			tNode->next = this->node;
			this->node->pre = tNode;
		}
		else //�䳤
		{
			while (num)
			{
				listNode<T>* pNode = allocator.allocate(1);
				pNode->data = elem;
				listNode<T>* lastNode = this->node->pre;//ָ�����һ���ڵ�
				pNode->next = this->node;
				pNode->pre = lastNode;
				this->node->pre = pNode;
				lastNode->next = pNode;
				num--;
			}
		}
	}
	template <class T, class Alloc>
	void list<T, Alloc>::resize(size_t num)	//����ָ�������ĳ���Ϊnum���������䳤������Ĭ��ֵ�����λ�ã������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ��
	{
		//�ҵ����һ���������ʱnumΪ0��ֱ�ӽ���ѭ����
		listNode<T>* pNode = this->node->next;//ָ���һ����
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
		if (num == 0)//˵�����
		{

			listNode<T>* tNode = pNode;
			pNode = pNode->next;//ת����һ���ڵ�
			while (pNode != this->node)
			{
				listNode<T>* nNode = pNode->next;//��¼��ǰ��Ҫɾ���ڵ����һ���ڵ�
				allocator.deallocate(pNode);//ɾ����ǰ�ڵ�
				pNode = nNode;
			}
			tNode->next = this->node;
			this->node->pre = tNode;
		}
		else //�䳤
		{
			while (num)
			{
				listNode<T>* pNode = allocator.allocate(1);
				//listNode<T>* defNode = new listNode<T>();
				//pNode->data = defNode->data;
				//delete	defNode;
				pNode->data = T();
				listNode<T>* lastNode = this->node->pre;//ָ�����һ���ڵ�
				pNode->next = this->node;
				pNode->pre = lastNode;
				this->node->pre = pNode;
				lastNode->next = pNode;
				num--;
			}
		}
	}

	//�����ɾ��
	template <class T, class Alloc>
	void list<T, Alloc>::push_back(const T& elem)									//������β������һ��Ԫ��
	{
		Alloc allocator;
		listNode<T>* pNode = allocator.allocate(1);
		pNode->data = elem;
		listNode<T>* endNode = this->node->pre;//���һ���ڵ�
		//���´����Ľڵ����
		pNode->next = this->node;
		pNode->pre = endNode;
		endNode->next = pNode;
		this->node->pre = pNode;
	}
	template <class T, class Alloc>
	void  list<T, Alloc>::pop_back()										//ɾ�����������һ��Ԫ��
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
	void list<T, Alloc>::push_front(const T& elem)								//��������ͷ����һ��Ԫ��
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
	void list<T, Alloc>::pop_front()										//��������ͷ�Ƴ�һ��Ԫ��
	{
		Alloc allocator;
		listNode<T>* firstNode = this->node->next;
		if (firstNode == this->node)
			throw - 1;
		listNode<T>* tNode = firstNode->next;//��¼��ɾ���ڵ����һ���ڵ�
		allocator.deallocate(firstNode);
		this->node->next = tNode;
		tNode->pre = this->node;
	}
	template <class T, class Alloc>
	listIterator<T> list<T, Alloc>::insert(iterator pos, const T& elem)			//��posλ�ò���elemԪ�صĿ��������������ݵ�λ��
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
	void list<T, Alloc>::insert(iterator pos, size_t n, const T& elem)				//��posλ�ò���n��elem���ݣ��޷���ֵ
	{
		Alloc allocator;
		while (n)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = elem;
			listNode<T>* preNode = pos.node->pre;//ָ�����λ�õ�ǰһ���ڵ�
			pNode->next = pos.node;
			pNode->pre = preNode;
			preNode->next = pNode;
			pos.node->pre = pNode;
			n--;
		}
	}
	template <class T, class Alloc>
	void list<T, Alloc>::insert(iterator pos, iterator beg, iterator end)	//��posλ�ò���[beg,end)��������ݣ��޷���ֵ
	{
		Alloc allocator;
		while (beg != end)
		{
			listNode<T>* pNode = allocator.allocate(1);
			pNode->data = *beg;
			listNode<T>* preNode = pos.node->pre;//ָ�����λ�õ�ǰһ���ڵ�
			pNode->next = pos.node;
			pNode->pre = preNode;
			preNode->next = pNode;
			pos.node->pre = pNode;
			beg++;
		}
	}
	template <class T, class Alloc>
	void list<T, Alloc>::clear()											//�Ƴ�������������
	{
		Alloc allocator;
		listNode<T>* pNode = this->node->next;//ָ���һ���ڵ�
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
	listIterator<T> list<T, Alloc>::erase(iterator beg, iterator end)				//ɾ��[beg,end)��������ݣ�������һ�����ݵ�λ��
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
	listIterator<T> list<T, Alloc>::erase(iterator pos)							//ɾ��posλ�õ����ݣ�������һ�����ݵ�λ��
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
	void list<T, Alloc>::remove(const T& elem)									//ɾ��������������elemֵƥ���Ԫ�أ�*/
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
	//���ݴ�ȡ
	template <class T, class Alloc>
	T list<T, Alloc>::front()												//���ص�һ��Ԫ��
	{
		return this->node->next->data;
	}
	template <class T, class Alloc>
	T list<T, Alloc>::back()												//�������һ��Ԫ��
	{
		return this->node->pre->data;
	}
}

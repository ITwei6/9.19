#pragma once
#pragma once
using namespace std;
#include <vector>
#include<iostream>


template <class K>
struct defaulthashfunc//Ĭ�ϵķº�������������ģ
{
	size_t operator()(const K& key)
	{
		return (size_t)key;
		//��keyǿ������ת��
		//�����������壺���Ը���Ҳ���Խ���ģ��
	}
};
template <>
//ģ����ػ���������������int��ʱ���Ĭ��ʹ��defaulthashfunc<int>,������������string����ʱ����Ĭ��ʹ��defaulthashfunc<string>
struct defaulthashfunc<string>
{
	size_t operator()(const string& str)
	{
		//Ϊ�˼��ٳ�ͻ�����ǽ��ַ�����ÿ���ַ���ֵ���
		size_t hash = 0;
		for (auto& it : str)
		{
			hash *= 131;
			hash += it;
		}
		return hash;
	}
};




// //һ�����ͺ����ǾͲ�֪�����ݵľ��������ˣ���ô����Ҫ�Ƚϵ�ʱ�򣬾�Ҫдһ���º���������º�������map��set�������ģ�Ȼ��ʵ��������ϣ���еķº�������ȡ�����е�key����key���бȽ�
//��ϣ���
template <class T>
struct HashNode
{
	T _data;
	//�洢��������T���ͣ����ݴ�������ģ�����ȷ��
	HashNode<T>* _next;
	//ָ����һ������ָ��

	HashNode(const T& data)
		:_data(data)
		, _next(nullptr)
	{}
};


//����������⣬��������Ҫ�ù�ϣ����ϣ����Ҫ�е��������໥������ϵ
//����������ǰ���������߱�����������Ҫ���й�ϣ��������Ǵ��ڵģ��ں���

template<class K, class T, class KeyOfT, class Hashfunc>
class Hash_table;
template<class K, class T,class Ref,class Ptr, class KeyOfT, class Hashfunc>
//��ϣ��ĵ���������϶���װһ������ָ�룬������Ҫͨ�������ҵ���һ��Ͱ����Ϊ������Ҫ�������ҵ���һ��Ͱ�������Ͱ���������ô�ҵ���һ����Ϊ�յ�Ͱ�أ�
//��Ҫͨ�������ϣ�����ҵ�Ͱ���������ǻ���Ҫһ��ָ���ϣ���ָ��

struct HSIterator
{
	
	typedef  HashNode<T> Node;
	typedef HSIterator<K, T, T&, T*, KeyOfT, Hashfunc> iterator;
	typedef HSIterator<K, T,Ref,Ptr,KeyOfT, Hashfunc> Self;
	//�ײ��װ��һ�����ָ��
	Node* _node;
	//�ײ㻹��װ��һ����ϣ��ָ��
	const Hash_table <K,T, KeyOfT,Hashfunc>* _pht;
	//������Լ�const��Ϊ���ǲ��Ǹ���pht���ҵ���ϣ�����޸Ĺ�ϣ���������

	//û��ȡ������������Ƕ���ͣ��Ͳ���Ҫ��typename
	//HSIterator(Node* node, Hash_table<K, T, KeyOfT,Hashfunc>* hs)//hs����ͨ������
	//	:_node(node)
	//	, _pht(hs)
	//{}
	//���������Ĺ�ϣ��ָ������ָͨ���������--Ȩ�޵���С
	//����һ����������������const���εĹ�ϣ��ָ���������---Ȩ�޵�ƽ��
	HSIterator(Node* node, const Hash_table<K, T, KeyOfT, Hashfunc>* hs)//hs����ͨ������
		:_node(node)
		, _pht(hs)
	{}
	//��ͨ�Ŀ���������� const������������const����������ͨ��������������ͨ������
	//������д�Ŀ����������ͬʱ֧�������������ö�������ͨ������ʱ������ͨ������������Ҳ�����൱�ڸ�ֵ
	//�����ö�����const������ʱ��Ҳ������ͨ������������������֧������ͨ������ת����const��������
	HSIterator(const iterator& it)
		:_node(it._node)
		,_pht(it._pht)
	{}
	
	Ref& operator*()
	{
		return _node->_data;
	}
	Ptr operator->()
	{
		return &_node->_data;
	}
	Self& operator++()
	{
		if (_node->_next)//��ǰͰû�����꣬��ôֱ������һ������
		{
			_node = _node->_next;
		}
		else//��ǰͰ�ߵ����ˣ���Ҫ����һ����Ϊ�յ�Ͱ
		{
			KeyOfT kof;
			Hashfunc hf;

			size_t hashi = hf(kof(_node->_data)) % _pht->_table.size();
			//��������������ȥ�����˹�ϣ���˽�г�Ա������������Ҫ�õ�������Ϊ��ϣ�����Ԫ
		    //�ҵ���ǰͰ��λ��
			++hashi;
			//����һ��Ͱ��Ϊ��
				while (hashi<_pht->_table.size())
				{
					if (_pht->_table[hashi])
					{
						_node = _pht->_table[hashi];
						return *this;
					}
					else
					{
						++hashi;
					}

			    }
			//�ߵ������˵��Ͱ���߹��ˣ���û�ҵ�
			_node = nullptr;

		}
		return *this;
	}
	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}
	
};



template<class K, class T, class KeyOfT,class Hashfunc = defaulthashfunc<K>>
//��ϣ��
class Hash_table
{
	typedef HashNode<T> Node;

	template<class K, class T, class Ref,class Ptr,class KeyOfT, class Hashfunc>
	friend struct HSIterator;
	//�õ�������Ϊ��ϣ�����Ԫ
	//��ϣ��Ҫ��д������������Ȼ�Զ�������vector���Զ�����Ĭ����������������ĳ�Ա���������ͣ�û��Ĭ�Ϲ��죬
	//������Ҫ�����Լ�����ÿ�����
public:
	typedef HSIterator<K, T,T&,T*, KeyOfT, Hashfunc> iterator;
	typedef HSIterator<K, T, const T&, const T*, KeyOfT, Hashfunc> const_iterator;
	iterator begin()
	{
		//�ҵ�һ��Ͱ
		for (size_t i = 0; i < _table.size(); i++)
		{
			Node* cur = _table[i];
			while (cur)
			{
				if (cur)
				{
					return iterator(cur, this);
				}
			}
		}
		//���û���ҵ�
		return iterator(nullptr, this);
	}
	iterator end()
	{
		return iterator(nullptr, this);
	}
	const_iterator begin()const
	{
		//�ҵ�һ��Ͱ
		for (size_t i = 0; i < _table.size(); i++)
		{
			Node* cur = _table[i];
			while (cur)
			{
				if (cur)
				{
					return const_iterator(cur, this);
				}
			}
		}
		//���û���ҵ�
		return const_iterator(nullptr, this);
	}
	const_iterator end()const//�����const���ε���*this����Ҳ���ǹ�ϣ��const HashTable<K,T,keyoft,hashfunc>
	{
		return const_iterator(nullptr, this);//������this�������γ�����ͨ���͵ģ�Ȩ�޷Ŵ��ˣ�const������ͨ��
	}
	~Hash_table()
	{
		for (size_t i = 0; i < _table.size(); i++)
		{
			Node* cur = _table[i];
			while (cur)
			{
				Node* next = cur->_next;
				//Ҫ�ȱ�������
				delete cur;
				cur = next;
			}
			_table[i] = nullptr;
		}
	}
	Hash_table()
	{
		//����
		_table.resize(10, nullptr);
		//���ȿ���ʮ���ռ䣬ÿ���ռ�ֵΪ��
	}

	pair<iterator,bool> insert(const T& data)
	{

		KeyOfT kt;
		Hashfunc hf;
		//�º�������ʹ����ģ
		//�ڲ���֮ǰ��ȷ��һ�±����Ƿ��Ѿ��������ֵ��������˾Ͳ��ò�����
		iterator it = find(kt(data));
		if (it!=end())
		{
			return make_pair(it,false);
		}

		//�����Լ����������߼�����Ȼvector���Լ����ݣ�������Ҫ���ơ���Ϊ�����꣬�е�key���ͻ�е�ֵ�ֲ���ͻ�ˡ�
	   //��������ݣ���ô��ͻ���˾͸�������һ����
		//���������Ӵ�Լ����1ʱ��Ҫ���ݣ�ƽ��ÿ��Ͱ����һ������

		if (n == _table.size())
		{
			//������ݣ����¿��ռ�
			size_t newSize = _table.size() * 2;
			vector<Node*> newtable;
			newtable.resize(newSize, nullptr);

			//�����ٸ�������ķ������������ã���Ϊ�����ؿ��ռ䣬Ȼ����Ҫ�ͷţ�
			//����Ӧ�ý�ԭ���Ľ���ù���ʹ��
			//�������Ǳ����ɱ����ɱ�Ľ���ù�����ǩ���±���
			for (size_t i = 0; i < _table.size(); i++)
			{
				//���ݺ󣬿ռ�size����ˣ��е����ݾͿ��ܻ�浽��ͬ��Ͱ����
				//�������Ľ��Ҫ���¼���Ž��ĸ�λ��
				Node* cur = _table[i];
				//cur������ܻ������ӵĽ��
				while (cur)
				{
					size_t hashi = hf(kt(cur->_data)) % newtable.size();


					Node* next = cur->_next;
					//ͷ�嵽�±�
					cur->_next = newtable[hashi];
					//ͷ�� ������� �ӵ��������ǰ��԰�
					//��ônext�ͽӵ�newtavle[i]
					newtable[hashi] = cur;

					//�����߽�������
					cur = next;
				}

				//��ǰͰ��Ľ�㱻�ù�󣬾���Ϊ��
				_table[i] = nullptr;


			}
			//����±����������Ҫ�ı���ô��������vector���Ľ������þɱ���±���

			_table.swap(newtable);
		}

		size_t hashi = hf(kt(data)) % _table.size();

		Node* newnode = new Node(data);
		newnode->_next = _table[hashi];
		_table[hashi] = newnode;
		//���½��ͷ�嵽��ϣͰ��
		++n;
		return make_pair(iterator(newnode,this),true);
	}
	iterator find(const K& key)
	{

		Hashfunc hf;
		KeyOfT kt;
		size_t hashi = hf(key) % _table.size();

		Node* cur = _table[hashi];

		while (cur)
		{
			if (kt(cur->_data) == key)
				return iterator(cur,this);
			else
				cur = cur->_next;
		}
		return iterator(nullptr,this);
	}
	void Print()
	{
		KeyOfT kt;
		for (int i = 0; i < _table.size(); i++)
		{
			printf("[%d]", i);

			Node* cur = _table[i];
			while (cur)
			{
				cout << kt(cur->_data) << " ";
				cur = cur->_next;
			}
			cout << endl;
		}

	}
	bool erase(const K& key)
	{

		Hashfunc hf;
		KeyOfT kt;
		//���Ը���find������find�ҵ�keyȻ��ɾ��key�أ�4
		//�����ԣ���Ϊɾ��һ�������Ҫ�ҵ��������ǰ��ͺ����λ�ã�������ֻ��key��λ�ã����Բ���ֱ�Ӹ���find�����Ǹ������еĲ���
		size_t hashi = hf(key) % _table.size();

		Node* cur = _table[hashi];
		Node* prev = nullptr;
		while (cur)
		{
			if (kt(cur->_data) == key)//�ҵ�Ҫɾ���Ľ���
			{

				//��ǰ��Ľ���ָ��ָ������ǰ��

				//����һ�ֿ���cur����Ͱ��ĵ�һ������ô����ͷɾ�ˣ�prev����nullptr
				if (prev == nullptr)
				{
					_table[hashi] = cur->_next;
				}
				else
				{
					prev->_next = cur->_next;
				}

				delete cur;
				--n;
				return true;
			}
			else
			{
				prev = cur;
				//ÿ���ȼ�¼һ��ǰ��Ľ��λ��
				cur = cur->_next;
			}

		}


		return false;

	}
private:
	//�ײ��װ����һ��ָ������
	//�������ָ��ָ��Ķ��ǹ�ϣ���
	vector<Node*> _table;
	//�ײ㻹��װһ����Сn��ʾʵ�ʱ��е����ݸ���
	size_t n = 0;///�������㸺������
};

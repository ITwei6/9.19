#pragma once
#pragma once
using namespace std;
#include <vector>
#include<iostream>


template <class K>
struct defaulthashfunc//默认的仿函数可以让数据模
{
	size_t operator()(const K& key)
	{
		return (size_t)key;
		//将key强行类型转化
		//这样作的意义：可以负数也可以进行模了
	}
};
template <>
//模板的特化，当数据类型是int的时候就默认使用defaulthashfunc<int>,当数据类型是string类型时，就默认使用defaulthashfunc<string>
struct defaulthashfunc<string>
{
	size_t operator()(const string& str)
	{
		//为了减少冲突，我们将字符串的每个字符的值相加
		size_t hash = 0;
		for (auto& it : str)
		{
			hash *= 131;
			hash += it;
		}
		return hash;
	}
};




// //一旦泛型后，我们就不知道数据的具体类型了，那么我们要比较的时候，就要写一个仿函数，这个仿函数根据map和set传过来的，然后实例化出哈希表中的仿函数。获取数据中的key，让key进行比较
//哈希结点
template <class T>
struct HashNode
{
	T _data;
	//存储的数据是T类型，根据传过来的模板参数确定
	HashNode<T>* _next;
	//指向下一个结点的指针

	HashNode(const T& data)
		:_data(data)
		, _next(nullptr)
	{}
};


//这里存在问题，迭代器里要用哈希，哈希里面要有迭代器，相互依赖关系
//我们这里用前置声明告诉编译器，我们要是有哈希表，这个表是存在的，在后面

template<class K, class T, class KeyOfT, class Hashfunc>
class Hash_table;
template<class K, class T,class Ref,class Ptr, class KeyOfT, class Hashfunc>
//哈希表的迭代器里面肯定封装一个结点的指针，但还需要通过表来找到下一个桶，因为我们需要遍历先找到第一个桶，当这个桶遍历完后，怎么找到下一个不为空的桶呢？
//需要通过这个哈希表来找到桶，所以我们还需要一个指向哈希表的指针

struct HSIterator
{
	
	typedef  HashNode<T> Node;
	typedef HSIterator<K, T, T&, T*, KeyOfT, Hashfunc> iterator;
	typedef HSIterator<K, T,Ref,Ptr,KeyOfT, Hashfunc> Self;
	//底层封装着一个结点指针
	Node* _node;
	//底层还封装着一个哈希表指针
	const Hash_table <K,T, KeyOfT,Hashfunc>* _pht;
	//这里可以加const因为我们不是根据pht来找到哈希表来修改哈希表里的内容

	//没有取这个类里面的内嵌类型，就不需要加typename
	//HSIterator(Node* node, Hash_table<K, T, KeyOfT,Hashfunc>* hs)//hs是普通的类型
	//	:_node(node)
	//	, _pht(hs)
	//{}
	//当传过来的哈希表指针是普通指针就走上面--权限的缩小
	//重载一个，当传过来的是const修饰的哈希表指针就走这里---权限的平移
	HSIterator(Node* node, const Hash_table<K, T, KeyOfT, Hashfunc>* hs)//hs是普通的类型
		:_node(node)
		, _pht(hs)
	{}
	//普通的拷贝构造就是 const迭代器拷贝给const迭代器，普通迭代器拷贝给普通迭代器
	//而我们写的拷贝构造可以同时支持两个，当调用对象是普通迭代器时，用普通迭代器拷贝，也就是相当于赋值
	//当调用对象是const迭代器时，也是用普通迭代器拷贝。这样就支持了普通迭代器转化成const迭代器了
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
		if (_node->_next)//当前桶没有走完，那么直接找下一个即可
		{
			_node = _node->_next;
		}
		else//当前桶走到空了，就要找下一个不为空的桶
		{
			KeyOfT kof;
			Hashfunc hf;

			size_t hashi = hf(kof(_node->_data)) % _pht->_table.size();
			//这里我们在外面去调用了哈希表的私有成员，所以我们需要让迭代器成为哈希表的友元
		    //找到当前桶的位置
			++hashi;
			//找下一个桶不为空
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
			//走到这里就说明桶走走光了，还没找到
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
//哈希表
class Hash_table
{
	typedef HashNode<T> Node;

	template<class K, class T, class Ref,class Ptr,class KeyOfT, class Hashfunc>
	friend struct HSIterator;
	//让迭代器成为哈希表的友元
	//哈希需要将写析构函数，虽然自定义类型vector会自动调用默认析构，但它里面的成员是内置类型，没有默认构造，
	//所以需要我们自己析构每个结点
public:
	typedef HSIterator<K, T,T&,T*, KeyOfT, Hashfunc> iterator;
	typedef HSIterator<K, T, const T&, const T*, KeyOfT, Hashfunc> const_iterator;
	iterator begin()
	{
		//找第一个桶
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
		//最后没有找到
		return iterator(nullptr, this);
	}
	iterator end()
	{
		return iterator(nullptr, this);
	}
	const_iterator begin()const
	{
		//找第一个桶
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
		//最后没有找到
		return const_iterator(nullptr, this);
	}
	const_iterator end()const//这里的const修饰的是*this对象。也就是哈希表const HashTable<K,T,keyoft,hashfunc>
	{
		return const_iterator(nullptr, this);//而函数this传给的形成是普通类型的，权限放大了，const传给普通的
	}
	~Hash_table()
	{
		for (size_t i = 0; i < _table.size(); i++)
		{
			Node* cur = _table[i];
			while (cur)
			{
				Node* next = cur->_next;
				//要先保存起来
				delete cur;
				cur = next;
			}
			_table[i] = nullptr;
		}
	}
	Hash_table()
	{
		//构造
		_table.resize(10, nullptr);
		//首先开出十个空间，每个空间值为空
	}

	pair<iterator,bool> insert(const T& data)
	{

		KeyOfT kt;
		Hashfunc hf;
		//仿函数可以使数据模
		//在插入之前，确认一下表里是否已经有了这个值，如果有了就不用插入了
		iterator it = find(kt(data));
		if (it!=end())
		{
			return make_pair(it,false);
		}

		//我们自己控制扩容逻辑，虽然vector会自己扩容，但我们要控制。因为扩容完，有的key会冲突有的值又不冲突了。
	   //如果不扩容，那么冲突多了就根单链表一样了
		//当负载因子大约等于1时就要扩容，平均每个桶里有一个数据

		if (n == _table.size())
		{
			//异地扩容，重新开空间
			size_t newSize = _table.size() * 2;
			vector<Node*> newtable;
			newtable.resize(newSize, nullptr);

			//不能再复用下面的方法，这样不好，因为就又重开空间，然后又要释放，
			//我们应该将原来的结点拿过来使用
			//所以我们遍历旧表，将旧表的结点拿过来，签到新表上
			for (size_t i = 0; i < _table.size(); i++)
			{
				//扩容后，空间size变大了，有的数据就可能会存到不同的桶里了
				//拿下来的结点要重新计算放进哪个位置
				Node* cur = _table[i];
				//cur后面可能还有链接的结点
				while (cur)
				{
					size_t hashi = hf(kt(cur->_data)) % newtable.size();


					Node* next = cur->_next;
					//头插到新表
					cur->_next = newtable[hashi];
					//头插 这个结点的 接到插入结点的前面对吧
					//那么next就接到newtavle[i]
					newtable[hashi] = cur;

					//往后走接着拿走
					cur = next;
				}

				//当前桶里的结点被拿光后，就置为空
				_table[i] = nullptr;


			}
			//这个新表就是我们想要的表，那么我们利用vector、的交换，让旧表和新表交换

			_table.swap(newtable);
		}

		size_t hashi = hf(kt(data)) % _table.size();

		Node* newnode = new Node(data);
		newnode->_next = _table[hashi];
		_table[hashi] = newnode;
		//将新结点头插到哈希桶里
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
		//可以复用find吗？先用find找到key然后删除key呢？4
		//不可以，因为删除一个结点需要找到这个结点的前面和后面的位置，但这里只有key的位置，所以不能直接复用find，但是复用其中的步骤
		size_t hashi = hf(key) % _table.size();

		Node* cur = _table[hashi];
		Node* prev = nullptr;
		while (cur)
		{
			if (kt(cur->_data) == key)//找到要删除的结点后
			{

				//将前面的结点的指针指向后面的前面

				//还有一种可能cur就是桶里的第一个，那么就是头删了，prev就是nullptr
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
				//每次先记录一下前面的结点位置
				cur = cur->_next;
			}

		}


		return false;

	}
private:
	//底层封装的是一个指针数组
	//数组里的指针指向的都是哈希结点
	vector<Node*> _table;
	//底层还封装一个大小n表示实际表中的数据个数
	size_t n = 0;///用来计算负载因子
};

#pragma once
#include"Hash.h"
namespace tao
{



	template<class K>

	class set
	{

		struct setoft
		{
			const K& operator()(const K& key)
			{
				return key;
			}
		};
		
	public:
		typedef typename Hash_table<K, K, setoft>::const_iterator iterator;
		//为了解决set的key不能被修改所以我们让普通迭代器变成const迭代器
		typedef typename Hash_table<K, K, setoft>::const_iterator const_iterator;
		//因为普通迭代器也是const所以我们只用写const的begin和end即可。
		iterator begin()const
		{
			return _ht.begin();
		}
		iterator end()const
		{
			return _ht.end();
		}
		//这里的pair<const_iterator,bool>类型的
		pair<iterator,bool> insert(const K& key)
		{
			//return _ht.insert(key);//set调用的insert传回来的pair<iterator，bool>类型的，pair<iterator,bool>与pair<const_iterator,bool>是两个不同的类型
			//正常的的迭代器拷贝构造我们不需要写，因为迭代器是指针，拷贝构造就是浅拷贝，编译器自动生成的拷贝就可以
			//但是这里我们需要自己写拷贝构造； 因为需要将pair<iteartor,bool>类型，转化成pair<const_iterator,bool>类型,如何转化的呢？

			
			pair<typename Hash_table<K, K, setoft>::iterator, bool> it = _ht.insert(key);
			return pair<iterator, bool>(it.first, it.second);
		}
	private:
		//底层封装一个哈希表，哈希表里存的是K类型
		Hash_table<K, K, setoft> _ht;

	};
};


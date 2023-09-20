#pragma once

#include"Hash.h"
namespace tao
{



	template<class K, class V>

	class map
	{

		struct mapoft
		{
			const K& operator()(const pair<K, V>& kv)
			{
				return kv.first;
			}
		};
		

	public:
		typedef typename Hash_table<K, pair<const K, V>, mapoft>::iterator iterator;
		typedef typename Hash_table<K, pair<const K, V>, mapoft>::const_iterator const_iterator;
		iterator begin()
		{
			return _ht.begin();
		}
		iterator end()
		{
			return _ht.end();
		}
		iterator begin()const
		{
			return _ht.begin();
		}
		iterator end()const
		{
			return _ht.end();
		}
		V& operator[](const K&key)
		{
			pair<iterator, bool> ret = _ht.insert(make_pair(key,V()));
			//�����������pair���ͣ�Ҫ��make_pair����
			return ret.first->second;
		}
		pair<iterator,bool> insert(const pair<K, V>& kv)
		{
			return _ht.insert(kv);
		}
	private:
		//�ײ��װһ����ϣ��,��ϣ�����������pair<K,V>����
		Hash_table<K, pair<const K, V>, mapoft> _ht;

	};
};

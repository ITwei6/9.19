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
		//Ϊ�˽��set��key���ܱ��޸�������������ͨ���������const������
		typedef typename Hash_table<K, K, setoft>::const_iterator const_iterator;
		//��Ϊ��ͨ������Ҳ��const��������ֻ��дconst��begin��end���ɡ�
		iterator begin()const
		{
			return _ht.begin();
		}
		iterator end()const
		{
			return _ht.end();
		}
		//�����pair<const_iterator,bool>���͵�
		pair<iterator,bool> insert(const K& key)
		{
			//return _ht.insert(key);//set���õ�insert��������pair<iterator��bool>���͵ģ�pair<iterator,bool>��pair<const_iterator,bool>��������ͬ������
			//�����ĵĵ����������������ǲ���Ҫд����Ϊ��������ָ�룬�����������ǳ�������������Զ����ɵĿ����Ϳ���
			//��������������Ҫ�Լ�д�������죻 ��Ϊ��Ҫ��pair<iteartor,bool>���ͣ�ת����pair<const_iterator,bool>����,���ת�����أ�

			
			pair<typename Hash_table<K, K, setoft>::iterator, bool> it = _ht.insert(key);
			return pair<iterator, bool>(it.first, it.second);
		}
	private:
		//�ײ��װһ����ϣ����ϣ��������K����
		Hash_table<K, K, setoft> _ht;

	};
};


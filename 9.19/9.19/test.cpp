
#include "Hash.h"
#include "unordered_set.h"
#include "unordered_map.h"

int main()
{
	tao::map<int, int> m;
	m.insert(make_pair(1, 1));
	m.insert(make_pair(2, 2));
	m.insert(make_pair(3, 3));
	m.insert(make_pair(4, 4));

	tao::map<int, int>::iterator it = m.begin();
	while (it != m.end())
	{
		cout << it->first << " " << it->second << endl;
		++it;
	}
	tao::map<string,string> m2;

	m2.insert(make_pair("insert", "����"));
	m2.insert(make_pair("sort", "����"));
	m2.insert(make_pair("weak", "����"));
	m2.insert(make_pair("notion", "�۵�"));
	m2["sss"] = "aaaa";
	for (auto& ch : m2)
	{
		//ch.first += 12;
		//ch.second += 21;
		cout << ch.first << " " << ch.second << endl;
	}
	tao::set<int> s;
	s.insert(1);
	s.insert(100);
	s.insert(111);
	s.insert(21);
	tao::set<int>::iterator its = s.begin();
	while (its != s.end())
	{
		//(*its) += 1;
		cout << *its << " " << endl;
		++its;
	}

}
//1.��һ��ϣ������
// 2.�����ϣ��---> Ҫ��ɷ��ͣ�֧�ֶ�������
// 3.��װmap��set�Ľӿ� insert ,erase,����
// 4.ʵ��map��set�ĵ�������ʵ��֮ǰ��ʵ�ֹ�ϣ��ĵ�����
// 5.�����ϣ��ĵ�����
//6.��װmap��set�ĵ�����������ʵ����ͨ������
//7.�����ϣ��ĵ���������const������
//8.���set��key�����޸������map��key�����޸�value�����޸�����
//9.���map��[]���������set��insert����ֵ
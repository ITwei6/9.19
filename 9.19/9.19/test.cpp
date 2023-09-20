
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

	m2.insert(make_pair("insert", "插入"));
	m2.insert(make_pair("sort", "排序"));
	m2.insert(make_pair("weak", "虚弱"));
	m2.insert(make_pair("notion", "观点"));
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
//1.第一哈希表的完成
// 2.改造哈希表---> 要变成泛型，支持多种数据
// 3.封装map和set的接口 insert ,erase,……
// 4.实现map和set的迭代器，实现之前先实现哈希表的迭代器
// 5.定义哈希表的迭代器
//6.封装map和set的迭代器，基本实现普通迭代器
//7.改造哈希表的迭代器适配const迭代器
//8.解决set的key不能修改问题和map的key可以修改value不能修改问题
//9.解决map的[]运算符重载set的insert返回值
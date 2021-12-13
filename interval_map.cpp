#include <map>
#include <iostream>

template<typename K, typename V>
class interval_map {
public:

	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
		if( !( keyBegin < keyEnd ) )return;
		auto begIt = m_map.lower_bound(keyBegin);
		if(begIt == std::end(m_map))
		{
			if(val!=m_valBegin){
				m_map.insert({keyBegin, val});
				m_map.insert({keyEnd, m_valBegin});
			}
			return;
		}
		auto endIt = m_map.lower_bound(keyEnd);
		if(endIt == std::end(m_map))
		{
			if(val!= m_valBegin)
			{
				endIt = m_map.insert({keyEnd,m_valBegin}).first;
				begIt = m_map.lower_bound(keyBegin);
			}
		}
		V begVal = this->operator[](keyBegin);
		V endVal = this->operator[](keyEnd);
		if(begVal == val) return;
		if(endVal == val) return;
		
		m_map.insert(begIt, {keyBegin,val});
		m_map.insert(endIt, {keyEnd, endVal});

		m_map.erase(begIt++, endIt--);
		
		

	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

template <class K, class V>
void print_map(interval_map<K,V>& map)
{
	for(int i = -1; i < 15; i++)
	   std::cout << i << " " << map[i] << std::endl;

	std::cout << std::endl;
	for(auto i : map.m_map)
		std::cout << i.first <<  " " << i.second << std::endl;
}
#include <string>
#include <time.h>
#include <random>

void o_n_test()
{

	interval_map<uint, std::string> mm("a");
	
	for(uint i = 0; i < (uint)-1; i++)
	{
		auto b = rand()/1000000;
		auto e = rand()/1000000;
		if(e< b){
			auto t = b;
			b = e;
			e = t;
		}
		auto t = clock();
		std::cout << b << " " << e << std::endl;
		mm.assign(b,e, std::to_string(i));
		auto end = clock() - t;
		std:: cout << i << " " << end << std::endl;
	}
}

void map_test(){
	interval_map<int, char> map('a');
	map.assign(1,5,'r');
	map.assign(5,7,'b');
	map.assign(8,12, 'c');
	map.assign(12,15,'f');
	print_map(map);
}

int main()
{

	map_test();
}
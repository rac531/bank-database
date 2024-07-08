/*
Name: Ricardo Cardenas, 5007471477, CS302-1004, ASSIGNMENT 6
Description: Contains the definitions of the hashmap class
and funtions used in main. Each function has its own implementation
described in their headers.
Input: Each function has its own input
Output: Each function has its own output
*/

#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <iostream>

template <class t1, class t2>
class hashMap
{
public:
	hashMap(std::size_t = 5);
	t2& operator[](t1);
private:
	struct keyVal
	{
		keyVal()
		{
			key = t1();
			value = t2();
		}

		t1 key;
		t2 value;
	};

	void resize(std::size_t);

	std::size_t hash1(std::string);
	std::size_t hash2(std::string);

	std::vector<keyVal> table1;
	std::vector<keyVal> table2;

	std::size_t items1;
	std::size_t items2;
};

/*
 * function identifier: contructor for hashMap class. sets the 
   table sizes to passed in parameter.
 * parameters: size_t size
 * return value: none
 */
template <class t1, class t2>
hashMap<t1, t2>::hashMap(std::size_t size)
{
	items1 = 0;
	items2 = 0;

	table1.resize(size);
	table2.resize(size);
}

/*
 * function identifier: overloads the [] operator so that it
   finds a certain element using the key. if found, returns the value.
   if not exsistent, stores it. If collisions occur, it rehashes until
   an element is found. Resizes if necessary
 * parameters: t1 key
 * return value:t2
 */
template <class t1, class t2>
t2& hashMap<t1, t2>::operator[](t1 key)
{
	// determine load factor and resize
	if (((static_cast<double>(items1)/table1.size()) > 0.20) || ((static_cast<double>(items2)/table2.size()) > 0.20)){
		this->resize(table1.size());
	}


	std::size_t index1 = hash1(key) % table1.size();
	std::size_t index2 = hash2(key) % table2.size();
	int i1, i2 = 1;
	bool collision = true;

	while(collision == true){
		// if theres a matching key
		if(table1[index1].key == key){
			return table1[index1].value;
		}

		// if its empty
		if(table1[index1].key == t1()){
			items1++;
			table1[index1].key = key;
			return table1[index1].value;
		}

		// if theres a matching key
		if(table2[index2].key == key){
			return table2[index2].value;
		}

		// if its empty
		if(table2[index2].key == t1()){
			items2++;
			table2[index2].key = key;
			return table2[index2].value;
		}

		i1++;
		i2++;
		//even iterations
		if(i1 % 2 == 0){
			index1 = (index1 + (i1 * hash2(key))) % table1.size();
			index2 = (index2 + (i2 * hash1(key))) % table2.size();
		}
		// odd iterations
		else{
			index1 = (index1 + (i1 * hash1(key))) % table1.size();
			index2 = (index2 + i2 * hash2(key)) % table2.size();
		}
	}
	return table1[index1].value;
}

/*
 * function identifier: takes two tables, creates copies, clears originals,
   resizes originals, and rehashes anything from copies into originals
 * parameters: size_t amount
 * return value: none
 */
template <class t1, class t2>
void hashMap<t1, t2>::resize(std::size_t amount)
{
	std::vector<keyVal> table1Temp = table1;
	std::vector<keyVal> table2Temp = table2;

	table1.clear();
	table2.clear();
	table1.resize(table1Temp.size() * amount);
	table2.resize(table2Temp.size() * amount);

	for(int i = 0; i < table1Temp.size(); i++){
		if(table1Temp[i].key != t1())
			this->operator[](table1Temp[i].key) = table1Temp[i].value;
	}
	for(int i = 0; i < table2Temp.size(); i++){
		if(table2Temp[i].key != t1())
			this->operator[](table2Temp[i].key) = table2Temp[i].value;
	}

}

/*
 * function identifier: takes a string and calculates and returns
   a corresponding size_t value corresponding to it.
 * parameters: string key
 * return value: size_t val
 */
template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash1(std::string key)
{
	std::size_t val;
	for(int i = 0; i < key.size(); i++)
		val = val + (key[i] * pow(10, i));
	return val;
}

/*
 * function identifier: takes a string and calculates and returns
   a corresponding size_t value corresponding to it.
 * parameters: string key
 * return value: size_t val
 */
template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash2(std::string key)
{
	std::size_t val;
	for(int i = 0; i < key.size(); i++)
		val = val + (key[key.size() - 1 - i] * pow(10, i));
	return val;
}
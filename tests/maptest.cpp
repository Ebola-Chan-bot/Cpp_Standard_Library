#include <map>
#include <iostream>


struct Int {
	Int(int x = 0) : val(x) { };
	int val;
};

std::ostream& operator<<(std::ostream& s, Int x)
  { return s << x.val; }


int main(){
	std::map<std::string, double> test;
	std::map<std::string, double>::iterator i, j;
	std::map<std::string, double>::const_iterator k;

	std::cout << "Start of map test" << std::endl;

	std::cout << "Adding a few elements..." << std::endl;


	std::pair<std::string, double> a;
	std::pair<std::string, double> b;
	std::pair<std::map<std::string, double>::iterator, bool> c;

	test["b"] = 2;
	test["k"] = 11;
	test["c"] = 3;
	test["a"] = 1;


	std::cout << "Elements:" << std::endl;
	std::cout << "test[\"b\"] = " << test["b"] << std::endl;
	std::cout << "test[\"a\"] = " << test["a"] << std::endl;
	std::cout << "test[\"k\"] = " << test["k"] << std::endl;


	std::cout << "Reasigning the value of a" << std::endl;
	test["a"] = 65;
	std::cout << "test[\"a\"] = " << test["a"] << std::endl;

	std::cout << "All of the contents of test:" << std::endl;

	i = test.begin();
	std::cout << "First element: " << (*i).first << ": " << (*i).second << std::endl;
	std::cout << "First element: " << i->first << ": " << i->second << std::endl;
	j = test.end();
	while(i != test.end() ){
		std::cout << i->first << ": " << i->second << std::endl;
		++i;
		--i;
		++i;
		i--;
		i++;
	}

	std::cout << "Number of elements: " << test.size() << std::endl;
	std::cout << "Container empty? " << test.empty() << std::endl;

	std::pair<std::string, double> q;

	q.first = "Inserted value";
	q.second = 1.0;


	std::cout << "Finding element \"c\"" << std::endl;
	j = test.find("c");
	if(j == test.end()){
		std::cout << "Not found (bad)\n";
	}else{
		std::cout << "Found value for c: " << j->second << std::endl;
	}

	std::cout << "Count of elements with key \"c\": " << test.count("c") << std::endl;


	std::cout << "Inserting element g: " << std::endl;
	a.first = "g";
	a.second = 6;
	c = test.insert(a);
	std::cout << "Value inserted? " << c.second << std::endl;
	std::cout << "Key: " << c.first->first << ", value: " << c.first->second << std::endl;

	i = test.begin();
	while(i != test.end() ){
		std::cout << i->first << ": " << i->second << std::endl;
		++i;
	}

	j = test.find("g");
	std::cout << "Erasing element \"g\"" << std::endl;
	test.erase(j);

	i=test.begin();
	while(i != test.end() ){
		std::cout << i->first << ": " << i->second << std::endl;
		++i;
	}

	std::cout << "Erasing element \"c\"" << std::endl;
	test.erase("c");

	i=test.begin();
	while(i != test.end() ){
		std::cout << i->first << ": " << i->second << std::endl;
		++i;
	}

	j = test.lower_bound("b");
	std::cout << "This should read 2: " << j->second << std::endl;

	k = test.lower_bound("b");
	std::cout << "This should read 2: " << k->second << std::endl;


	std::cout << "This should read 11: " << test.equal_range("k").first->second << std::endl;



	typedef std::map<char, Int, std::less<char> > maptype;

	maptype map_char_myClass;
	// Store mappings between roman numerals and decimals.
	map_char_myClass['l'] = 50;
	map_char_myClass['x'] = 20; // Deliberate mistake.
	map_char_myClass['v'] = 5;
	map_char_myClass['i'] = 1;
	std::cout << "map_char_myClass['x'] = " << map_char_myClass['x'] << std::endl;
	map_char_myClass['x'] = 10; // Correct mistake.
	std::cout << "map_char_myClass['x'] = " << map_char_myClass['x'] << std::endl;
	std::cout << "map_char_myClass['z'] = " << map_char_myClass['z'] << std::endl; // Note default value is added.
	


	
	return 0;
}


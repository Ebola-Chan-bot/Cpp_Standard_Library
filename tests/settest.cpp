#include <iostream>
#include <algorithm>
#include <set>
#include <vector>

int main(){
	std::cout << "Starting set test\n";
	std::set<int> a;
	std::vector<int> c;
	std::vector<int>::iterator k, l;

	c.clear();

	if(a.empty() == false){
		std::cout << "ERROR - set should be empty\n";
	}else{
		std::cout << "OK - set is empty\n";
	}

	a.clear();

	std::pair<std::set<int>::iterator, bool> p;

	p = a.insert(57);
	p = a.insert(75);
	p = a.insert(11);
	p = a.insert(29);
	p = a.insert(128);
	p = a.insert(103);
	p = a.insert(56);

	std::cout << *(p.first) << " should read 56\n";



	std::set<int>::iterator i, j;

	std::cout << "Element list" << std::endl;
	i = a.begin();
	while(i != a.end() ){
		std::cout << "Element : " << *i << std::endl;
		++i;
	}

	//Deleting element 56
	i = a.find(56);
	std::cout << *i << " should read 56\n";

	a.erase(i);


	std::cout << "Element list (56 should be deleted)\n";
	i = a.begin();
	while(i != a.end() ){
		std::cout << "Element : " << *i << std::endl;
		++i;
	}


	std::cout << "Set algorithms test" << std::endl;
	std::cout << "Testing std::includes\n";

	a.clear();
	a.insert(12);
	a.insert(32);
	a.insert(85);
	a.insert(25);
	a.insert(63);
	a.insert(92);

	std::set<int> b(a);


	b.clear();
	b.insert(12);
	b.insert(32);
	b.insert(92);

	std::cout << "The following should be 1 : ";
	std::cout << std::includes(a.begin(), a.end(), b.begin(), b.end());
	std::cout << std::endl;

	b.clear();
	b.insert(12);
	b.insert(92);
	b.insert(91);

	std::cout << "The following should be 0 : ";
	std::cout << std::includes(a.begin(), a.end(), b.begin(), b.end());
	std::cout << std::endl;

	b.clear();
	std::cout << "The following should be 1 : ";
	std::cout << std::includes(a.begin(), a.end(), b.begin(), b.end());
	std::cout << std::endl;

	b.insert(12);
	std::cout << "The following should be 1 : ";
	std::cout << std::includes(a.begin(), a.end(), b.begin(), b.end());
	std::cout << std::endl;


	b.clear();
	b.insert(1);
	std::cout << "The following should be 0 : ";
	std::cout << std::includes(a.begin(), a.end(), b.begin(), b.end());
	std::cout << std::endl;


	std::cout << "Set union\n";
	a.clear();
	a.insert(10);
	a.insert(12);
	a.insert(32);
	a.insert(52);
	a.insert(72);

	b.clear();
	b.insert(12);
	b.insert(19);
	b.insert(22);
	b.insert(32);
	b.insert(38);
	b.insert(52);
	b.insert(92);

	c.assign(20, 0.0);

	std::cout << "The following two lines should be identical:\n";
	std::cout << "10 12 19 22 32 38 52 72 92 " << std::endl;

	l = std::set_union(a.begin(), a.end(), b.begin(), b.end(), c.begin() );
	k = c.begin();
	while(k != l){
		std::cout << *k << " " ;
		++k;
	}
	std::cout << std::endl;


	std::cout << "Set intersection\n";
	a.clear();
	a.insert(10);
	a.insert(12);
	a.insert(32);
	a.insert(52);
	a.insert(72);

	b.clear();
	b.insert(12);
	b.insert(19);
	b.insert(22);
	b.insert(32);
	b.insert(38);
	b.insert(52);
	b.insert(92);

	c.assign(20, 0.0);
	std::cout << "Size of c: " << c.size() << std::endl;

        std::cout << "The following two lines should be identical:\n";
        std::cout << "12 32 52 " << std::endl;

        l = std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), c.begin() );
        k = c.begin();
        while(k != l){
                std::cout << *k << " " ;
                ++k;
        }
        std::cout << std::endl;



	std::cout << "Set difference\n";
	a.clear();
	a.insert(10);
	a.insert(12);
	a.insert(32);
	a.insert(52);
	a.insert(72);

	b.clear();
	b.insert(12);
	b.insert(19);
	b.insert(22);
	b.insert(32);
	b.insert(38);
	b.insert(52);
	b.insert(92);

	c.assign(20, 0.0);

	std::cout << "The following two lines should be identical:\n";
	std::cout << "10 72 " << std::endl;

	l = std::set_difference(a.begin(), a.end(), b.begin(), b.end(), c.begin() );
	k = c.begin();
	while(k != l){
		std::cout << *k << " " ;
		++k;
	}
	std::cout << std::endl;


	std::cout << "Set symmetric difference\n";
	a.clear();
	a.insert(10);
	a.insert(12);
	a.insert(32);
	a.insert(52);
	a.insert(72);

	b.clear();
	b.insert(12);
	b.insert(19);
	b.insert(22);
	b.insert(32);
	b.insert(38);
	b.insert(52);
	b.insert(92);

	c.assign(20, 0.0);

	std::cout << "The following two lines should be identical:\n";
	std::cout << "10 19 22 38 72 92 " << std::endl;

	l = std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), c.begin() );
	k = c.begin();
	while(k != l){
		std::cout << *k << " " ;
		++k;
	}
	std::cout << std::endl;


	return 0;
}
#include "cbuffer.h"
using namespace std;

bool is_even(int i) { return (i % 2 == 0); }

int main() {
	cbuffer<char> cb(10, 'a');
	for (int i = 0; i < 17; ++i) {
		cb.add_item('b');
	}

	cbuffer<double> cb1(4);

	for (int i = 0; i < 7; ++i) {
		cb1.add_item(3.14);
	}

	cb1.delete_item();
	cb1.delete_item();
	cb1.delete_item();

	cbuffer<double> cb2(cb1);

	cbuffer<unsigned int> cb3(5);

	cb3 = cb;

	cbuffer<unsigned int>::iterator it2 = cb3.begin();
	cbuffer<unsigned int>::iterator it(it2);
	cbuffer<unsigned int>::iterator ite = cb3.end();
	for (; it != ite; ++it) {
		std::cout << *it << std::endl;
	}

	cbuffer<int> cb4(5);
	cbuffer<int> cb5(8);
	cb4 = cb5;
	
	check(cb3, &is_even);
	return 0;
}
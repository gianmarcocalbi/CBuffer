#include "cbuffer2.h"
using namespace std;

int main() {
	cbuffer<int> cb(10);
	/*debug_print_buffer(cb);
	system("pause");
	for (int i = 1; i < 5; i++)
		cb.add_item(i);
	debug_print_buffer(cb);
	system("pause");
	cb.delete_item();
	cb.delete_item();
	cb.delete_item();
	cb.delete_item();
	debug_print_buffer(cb);*/

	for (int i = 0; i < 35; i++)
		cb.add_item(i);

	debug_print_memo_buffer(cb);

	// Iterators
	cout << endl << "Print Iterator" << endl;
	cbuffer<int>::iterator it = cb.begin();
	cbuffer<int>::iterator ite = cb.end();
	for (; it != ite; it++) {
		std::cout << *it << std::endl;
	}

	system("pause");
	return 0;
}
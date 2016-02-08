#include "cbuffer.h"
using namespace std;

int main() {
	//TEST: cbuffer<T> main functions
	cbuffer<int> cb1(19);
	const cbuffer<int> cb(120);
	cb = cb1;
	//cbuffer<double> cb1(18);
	//cbuffer<double> cbc(60);
	for (int i = 0; i < 35; i++) {
		//cb.add_item(i);
	}
	//cb.add_item(0.45);
	//debug_print_memo_buffer(cb);
	//cbc = cb;

	//cb1 = cb;

	//debug_print_memo_buffer(cb);
	//debug_print_memo_buffer(cb1);
	//TEST check()
	
	//debug_print_buffer(cb);
	//check(cb, [](int i) { return (i % 2 == 0); });


	/*debug_print_memo_buffer(cb);
	cbc = cb;
	debug_print_memo_buffer(cbc);*/

	/*cout << "cb2 == cb  : " << (cb2 == cb) << endl;
	cout << "cb2 != cb  : " << (cb2 != cb) << endl;
	cout << "cbc == cbc : " << (cbc == cbc) << endl;
	cout << "cbc != cbc : " << (cbc != cbc) << endl;*/

	/*for (int i = 1; i < 5; i++)
		cb.add_item(i);
	debug_print_buffer(cb);
	system("pause");
	cb.delete_item();
	cb.delete_item();
	cb.delete_item();
	cb.delete_item();
	debug_print_buffer(cb);*/

	/*for (int i = 0; i < 35; i++) {
		cb.add_item(i);
	}

	debug_print_memo_buffer(cb);*/
	
	// TEST: Iterators
	/*cout << endl << "Print Iterator" << endl;
	cbuffer<int>::iterator it = cb.begin();
	cbuffer<int>::iterator ite = cb.end();
	for (; it != ite; it++) {
		std::cout << *it << std::endl;
	}*/

	system("pause");
	return 0;
}
#include "cbuffer.h"
using namespace std;
//template<typename T>
//cbuffer<T>::cbuffer(void) : ptr(0), size(0) {
//	//stuffs
//}
//
//template<typename T>
//cbuffer<T>::cbuffer(size_type sz) : ptr(0), size(sz) {
//	ptr = new T[sz];
//}
//
//template<typename T>
//cbuffer<T>::cbuffer(size_type sz, const T &init) : ptr(0), size(sz) {
//	ptr = new T[sz];
//	for (i = 0; i < size; i++) {
//		this[i] = init; //bisogna implementare l'accesso  all'i-esimo elemento del buffer prima
//	}
//}
//
//template<typename t>
//cbuffer<t>::get_size() {
//	return this->size;
//}
//
//template<typename t>
//&int cbuffer<t>::operator[] (size_type index) {
//	assert(index < size);
//	return ptr[index];
//}

int main() {
	cbuffer<int> cb(3);
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

	for (int i = 0; i < 5; i++)
		cb.add_item(i);

	debug_print_buffer(cb);

	cout << endl << "Print Iterator" << endl;

	cbuffer<int>::iterator it = cb.begin();
	cbuffer<int>::iterator ite = cb.end();

	for (; it != ite; it++) {
		std::cout << *it << std::endl;
	}

	system("pause");
	return 0;
}
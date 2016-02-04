#ifndef CBUFFER_H
#define CBUFFER_H
#include <iostream>
#include <assert.h> //definire NDEBUG in release
#include <algorithm>
using namespace std;

/*
TODO STUFFS

Metodi:
- iteratori end() begin()
[...]

- Aggiungere e implementare Eccezioni
- Rimuovere assert on release
- Costruttore Secondario

*/

template <typename T>
class cbuffer {
public:
	typedef unsigned int size_type; ///< Tipo custom per la size.

private:

	T *ptr;							///< Puntatore all'inizio del buffer in memoria .
	size_type size; 				///< Dimensione del buffer.
	unsigned int items_amount = 0; 	///< Numero di elementi inseriti dalla creazione del buffer.
	size_type head = 0;				///< Indice primo elemento del buffer.
	size_type tail = 0;				///< Indice all'elemento successivo all'ultimo elemento del buffer.
	unsigned int pending_items = 0;	///< Elementi in coda nel buffer.

protected:

public:
	/**
	* Costruttore di default
	*/
	cbuffer(void) : ptr(0), size(0) {
		//stuffs
	}

	/**
	* Costruttore Primario
	* @param size_type sz dimensione in capacità elementi buffer
	*/
	explicit cbuffer(size_type sz) : ptr(0), size(sz) {
		ptr = new T[sz];
		size = sz;
	}

	/**
	* Costruttore Secondario
	* Inizializza ad un valore di default le celle del Buffer
	* blabla
	*/
	// METODO OBSOLETO
	// cbuffer(size_type sz, const T &init);

	/**
	* Copy constructor
	* @param const cbuffer obj oggetto cbuffer da copiare
	*/
	cbuffer(const cbuffer& obj) {
		ptr = new T[obj.size];
		for (size_type i = 0; i < obj.size; ++i) {
			ptr[i] = obj.ptr[i];
		}
		size = obj.size;
	}

	/**
	* operatore=
	* @param const cbuffer other oggetto cbuffer dal quale copiare i dati
	* @return cbuffer
	*/
	cbuffer &operator=(const cbuffer& other) {
		if (this != &other) {
			cbuffer tmp(other);
			std::swap(ptr, tmp.ptr);
			std::swap(size, tmp.size);
			std::swap(items_amount, tmp.items_amount);
			std::swap(head, tmp.head);
			std::swap(tail, tmp.tail);
			std::swap(pending_items, tmp.pending_items);
			//(!) fare una swap per ogni variabile che aggiungerò
		}
		return *this; // concatenazione assegnamenti
	}

	/*
	Operatore Assegnamento Con Casting
	*/

	/**
	* Distruttore
	* (tilde "~" fatta con ALT+126)
	*/
	~cbuffer(void) {
		delete[] ptr; // Deallocazione del puntatore allocato durante l'istanziazione dell'oggetto.
		size = 0; 				
		items_amount = 0;
		head = 0;
		tail = 0;
		pending_items = 0;	
	}

	/**
	* Capacità/Get Size
	* @return size_type size
	*/
	size_type get_size() {
		return size;
	}

	size_type capacity() {
		return get_size();
	}


	/**
	* Numero di Elementi inseriti (dall'inizio dei tempi)
	* @return int items_amount
	*/
	int get_items_amount() {
		return items_amount;
	}


	/**
	* Numero di Elementi Nel Buffer
	* @return int elementi all'interno del buffer
	*/
	int get_pending_items() {
		return pending_items;
	}

	
	/**
	* Inserimento (in coda) di un nuovo elemento
	* @param T item elemento da aggiungere al buffer
	*/
	void add_item(T item) {
		/* Se il buffer è pieno tail punta al primo elemento (ovvero vale 0),
		quindi viene sovrascritto l'elemento più vecchio e quindi cambia anche
		la testa del buffer, altrimenti aggiungo un elemento in coda e sposto 
		la tail di uno. */
		ptr[tail] = item;
		tail = (tail + 1) % size;
		
		// Se buffer pieno sposto la testa
		if (pending_items == size) {
			head = (head + 1) % size;
		} else {
		// Altrimenti il numero degli elementi in coda aumenta di uno
			pending_items++;
		}
		items_amount++; // Aumento il numero di elementi totali mai inseriti nel buffer di uno
	}


	/**
	* Cancellazione elemento (in testa).
	* @return bool indica se è stato eliminato un elemento o meno.
	*/

	//(!) C'è UN PROBLEMA COL DELETE CHE FA ESPLODERE LA MEMORIA
	bool delete_item() {
		/* Se c'è almeno un elemento nel buffer sposto la head di uno in avanti,
		altrimenti non posso eliminare un elemento. */
		if (pending_items > 0) {
			ptr[head] = 0;
			head++;
			pending_items--;
			return true;
		}
		//(!) Notificare all'utente che non ci sono elementi nel buffer.
		return false;	
	}


	/**
	* operatore[] per accedere all'i-esimo elemento del buffer
	* @param size_type index indice dell'elemento a cui accedere
	* @return T tipo templato
	* @throw IndexOutOfBound
	*/
	//cbuffer[0] -> elemento più vecchio
	T &operator[] (size_type index) {
		assert(index < size);
		if (index >= pending_items) {
			//(!) Lanciare eccezione ancora da implementare
			//throw IndexOutOfBound();
		}
		int tmp = (head + index) % size;
		return ptr[tmp];
	}

	


	/**
	* Iterators
	*/
	//cbuffer.begin(); -> iteratore all'elemento più vecchio
	//cbuffer.fine();  -> iteratore all'elemento più giovane

	class iterator; // forward declaration (per gli iteratori)
	
	class iterator {

		friend class cbuffer; //Permette istanziazione ed utilizzo di metodi private della classe cbuffer

		T *ptr; //< puntatore ai dati di cbuffer<T>
		int _size;
		int _beg;

				/**
				Costruttore privato per inizializzare ptr
				cbuffer<T> può chiamarlo grazie la friend
				@param p puntatore ai dati di cbuffer<T>
				*/
		iterator(T* p) : ptr(p) {
			//cout << "Costruttore ptr[]" << endl;
		}

		iterator(T* p, int sz, int beg) : ptr(p), _size(sz), _beg(beg) {
			//cout << "Costruttore ptr[]" << endl;
		}

	public:

		// stuffs
		typedef std::forward_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;

		/**
		Costruttore di default
		*/
		iterator() : ptr(0) {}

		/**
		Copy constructor
		*/
		iterator(const iterator &other) : ptr(other.ptr) {}

		/**
		Operatore di assegnamento
		*/
		iterator& operator=(const iterator &other) {
			ptr = other.ptr;
			return *this;
		}

		/**
		Distruttore
		*/
		~iterator() {}

		/**
		Dereferenziamento
		@return reference al dato puntato
		*/
		T& operator*() const {
			return *ptr;
		}

		/**
		Puntatore
		@return puntatore al dato
		*/
		T* operator->() const {
			return ptr;
		}

		/**
		Confronto iterator/iterator
		@param other iterator da confrontare
		@return true se *this punta allo stesso dato di other
		*/
		bool operator==(const iterator &other) const {
			return (ptr == other.ptr);
		}

		/**
		Confronto iterator/iterator
		@param other iterator da confrontare
		@return true se *this non punta allo stesso dato di other
		*/
		bool operator!=(const iterator &other) const {
			return !(other == *this);
		}

		/**
		Confronto iterator/const_iterator
		@param other const_iterator da confrontare
		@return true se *this punta allo stesso dato di other
		
		bool operator==(const const_iterator &other) const {
			return (ptr == other.ptr);
		}*/

		/**
		Confronto iterator/const_iterator
		@param other const_iterator da confrontare
		@return true se *this punta allo stesso dato di other
		
		bool operator!=(const const_iterator &other) const {
			return !(other == *this);
		}*/

		/**
		Spostamento (prefisso)
		@return iteratore nella nuova posizione
		*/
		iterator& operator++() {
			if ((*ptr) >= _beg + _size) {
				ptr -= _size;
			} else {
				++ptr;
			}
			return *this;
		}

		/**
		Spostamento (postfisso)
		@return iteratore nella vecchia posizione
		*/
		iterator operator++(int) {
			iterator tmp(ptr);
			/*cout << "iterator++ :   ptr = " << (int)ptr << endl;
			cout << "iterator++ :  *ptr = " << *ptr << endl;
			cout << "iterator++ : _size = " << _size << endl;
			cout << "iterator++ :  _beg = " << _beg << endl;*/
			if ((int)ptr >= _beg + _size) {
				ptr -= _size;
			} else {
				++ptr;
			}			
			return tmp;
		}

	}; // fine iterator

	/**
	 * Richiesta iterator
	 * @return un iteratore in lettura/scrittura all'inizio della sequenza dati
	 */
	iterator begin() {
		cout << "begin()" << endl;
		return iterator(ptr + head, (int)size, (int)ptr);
	}

	/**
	 * Richiesta iterator
	 * @return un iteratore in lettura/scrittura alla fine della sequenza dati
	 */
	iterator end() {
		cout << "end()" << endl;
		return iterator(ptr + tail - 1);
	}


	/*
	 * TEMP METHODS
	 * Eliminarli in release!
	 */
	size_type get_head() {
		return head;
	}
	size_type get_tail() {
		return tail;
	}
	T * get_ptr() {
		return ptr;
	}
};

/**
* Funzione Globale check
* @param cbuffer cb
* @param P predicato unario
*
* Funzionamento: per ogni elemento cb[i] nel buffer stampa a console:
* - "[i]: true"  quando P(cb[i]) è vero.
* - "[i]: false" quando P(cb[i]) è falso.
*/
template <typename F, typename T>
void check(cbuffer<T> cb, F p);


/***********************************************
************** DEBUG FUNCTIONS ****************
***********************************************/
template <typename T>
void debug_print_buffer_header(cbuffer<T> &cb) {
	//iteratori
	cout << "Buffer Circolare CBuffer" << endl << "get_size() = " << cb.get_size() << endl;
	cout << "get_pending_items() = " << cb.get_pending_items() << endl << "get_items_amount() = " << cb.get_items_amount() << endl;
	cout << "head = " << cb.get_head() << endl << "tail = " << cb.get_tail() << endl;
}

template <typename T>
void debug_print_buffer(cbuffer<T> &cb) {
	//iteratori
	debug_print_buffer_header(cb);
	cout << "Print ptr[]" << endl;
	for (int i = 0; i < cb.get_pending_items(); i++) {
		cout << "    - [" << i << "] : " << cb.get_ptr()[i] << endl;
	}
}

#endif








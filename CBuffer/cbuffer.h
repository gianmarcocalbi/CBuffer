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
			//fare una swap per ogni variabile che aggiungerò
		}
		return *this; // concatenazione assegnamenti
	}

	/**
	* Distruttore
	* (tilde "~" fatta con ALT+126)
	*/
	~cbuffer(void) {
		delete[] ptr; // Deallocazione del puntatore allocato durante l'istanziazione dell'oggetto.
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


	/*
	TEMP
	*/
	size_type get_head() {
		return head;
	}
	size_type get_tail() {
		return tail;
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
	for (int i = 0; i < cb.get_pending_items(); i++) {
		cout << "[" << i << "] : " << cb[i] << endl;
	}
}

#endif








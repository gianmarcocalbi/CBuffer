/**
* \class cbuffer
*
* \brief Classe Buffer Circolare Progetto C++
*
* Il progetto richiede la realizzazione di una classe cbuffer generica che
* implementa un buffer circolare di elementi di tipo T.
* Il buffer ha una capacità fissa, decisa a costruzione. L'inserimento accoda gli
* elementi finché il buffer non è pieno. Una volta riempito, i nuovi dati vengono
* scritti partendo dall'inizio del buffer, sovrascrivendo i vecchi.
*
* \note Progetto d'esame.
*
* \author (last to touch it) $Author: Gianmarco Calbi 781951 $
*
* \version $Revision: 1.0 $
*
* \date $Date: 2016/02/08 23:00:00 $
*
*/

#ifndef CBUFFER_H
#define CBUFFER_H
#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <assert.h>
using namespace std;


template <typename T>
class cbuffer {
public:
	typedef unsigned int size_type; ///< Tipo custom per la size.

private:

	T *ptr;						///< Puntatore all'inizio del buffer in memoria.
	size_type size; 			///< Dimensione del buffer (lato utente, ovvero quanti oggetti ci possono stare massimo).
	size_type items_amount; 	///< Numero di elementi inseriti dalla creazione del buffer.
	size_type head;				///< Indice primo elemento del buffer.
	size_type tail;				///< Indice all'elemento successivo all'ultimo elemento del buffer.
	size_type pending_items;	///< Elementi in coda nel buffer.

protected:

public:
	/**
	* Costruttore di default.
	*/
	cbuffer(void) : ptr(0), size(0), items_amount(0), head(0), tail(0), pending_items(0) {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::cbuffer(void)" << endl;
		#endif // !NDEBUG

	}


	/**
	* Costruttore Primario.
	* @param sz dimensione in capacità elementi buffer.
	*/
	explicit cbuffer(size_type sz) : ptr(0), size(sz), items_amount(0), head(0), tail(0), pending_items(0) {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::cbuffer(size_type sz)" << endl;
		#endif // !NDEBUG
		try {
			ptr = new T[sz + 1];
			ptr[0] = 0; ///< Contenuto dell'indirizzo di memoria a cui punta tail.
		} catch (const bad_alloc& e) {
			cout << "ERRORE: Impossibile allocare un nuovo cbuffer della dimensione inserita." << endl;
		} catch (...) {
			cout << "ERRORE: Un errore si è verificato durante l'allocazione in memoria del cbuffer." << endl;
		}
	}


	/**
	* Costruttore Secondario.
	* Inizializza ad un valore di default le celle free del Buffer.
	* @param sz dimensione in capacità elementi buffer.
	* @param &init valore di inizializzazione per le celle vuote del buffer.
	*/
	explicit cbuffer(size_type sz, const T &init) : ptr(0), size(sz), items_amount(0), head(0), tail(0), pending_items(0) {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::cbuffer(size_type sz, const T &init)" << endl;
		#endif // !NDEBUG
		try {
			ptr = new T[sz + 1];
			ptr[0] = 0; ///< Contenuto dell'indirizzo di memoria a cui punta tail.
			for (size_type i = 1; i < size; ++i) {
				ptr[i] = init;
			}
		} catch (const bad_alloc& e) {
			cout << "ERRORE: Impossibile allocare un nuovo cbuffer della dimensione inserita." << endl;
		} catch (...) {
			cout << "ERRORE: Un errore si è verificato durante l'allocazione in memoria del cbuffer." << endl;
		}
	}


	/**
	* Copy constructor.
	* Copia solo da cbuffer contenenti items dello stesso tipo.
	* @param other oggetto cbuffer da copiare.
	*/
	cbuffer(const cbuffer& other) {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::cbuffer(const cbuffer& other) /*copy constructor*/" << endl;
		#endif // !NDEBUG
		ptr = new T[other.size + 1];
		for (size_type i = 0; i < other.size + 1; ++i) {
			ptr[i] = other.ptr[i];
		}
		size = other.size;
		items_amount = other.get_items_amount();
		head = other.get_head();
		tail = other.get_tail();
		pending_items = other.get_pending_items();
	}


	/**
	* Operatore "=".
	* Operatore di assegnamento.
	* @param other Oggetto cbuffer dal quale copiare i dati.
	* @return cbuffer.
	*/
	cbuffer &operator=(const cbuffer& other) {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::&operator=(const cbuffer& other)" << endl;
		#endif // !NDEBUG
		if (this != &other) {
			cbuffer tmp(other);
			std::swap(ptr, tmp.ptr);
			std::swap(size, tmp.size);
			std::swap(items_amount, tmp.items_amount);
			std::swap(head, tmp.head);
			std::swap(tail, tmp.tail);
			std::swap(pending_items, tmp.pending_items);
		}
		return *this; // concatenazione assegnamenti
	}


	/**
	* Operatore "=" templato.
	* Operatore di assegnamento tra due cbuffer contenenti item di diverso tipo.
	* cbuffer<T> = cbuffer<Q> con casting.
	* @param &other.
	* @return cbuffer.
	*/
	template<typename Q>
	cbuffer &operator=(const cbuffer<Q> &other) {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::&operator=(const cbuffer<Q>& other)" << endl;
		#endif // !NDEBUG
		cbuffer<T> tmp(other.get_size());

		try {
			for (size_type i = 0; i < other.get_size() + 1; ++i) {
				tmp.ptr[i] = static_cast<T>(other.get_ptr()[i]);
			}
			std::swap(ptr, tmp.ptr);
			std::swap(size, tmp.size);
			items_amount = other.get_items_amount();
			head = other.get_head();
			tail = other.get_tail();
			pending_items = other.get_pending_items();
		} catch (exception& e) {
			cout << "ERRORE: static_cast fallito." << endl;
			throw;
			//e.what();
		}
		return *this; // concatenazione assegnamenti
	}


	/**
	* Distruttore.
	*/
	~cbuffer(void) {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::~cbuffer(void)" << endl;
		#endif // !NDEBUG
		delete[] ptr; // Deallocazione del puntatore allocato durante l'istanziazione dell'oggetto.
		size = 0;
		items_amount = 0;
		head = 0;
		tail = 0;
		pending_items = 0;
	}


	/**
	* Get Size.
	* @return size_type size.
	*/
	size_type get_size() const {
		return size;
	}


	/**
	* Numero di Elementi inseriti (dall'inizio dei tempi).
	* Contatore che viene incrementato ogni volta che un item viene inserito nel buffer.
	* Ovvero conta il numero di item passati per il buffer.
	* @return int items_amount
	*/
	size_type get_items_amount() const {
		return items_amount;
	}


	/**
	* Numero di Elementi Nel Buffer.
	* Numero di elementi che sono ora nel buffer.
	* @return int elementi all'interno del buffer.
	*/
	size_type get_pending_items() const {
		return pending_items;
	}

	
	/**
	* Indice di testa.
	* Indice che indica in che punto della memoria rispetto a ptr è posizionato l'elemento più vecchio del buffer.
	* @return size_type
	*/
	size_type get_head() const {
		return head;
	}
	

	/**
	* Indice di "coda".
	* Indice che indica in che punto della memoria (rispetto a ptr)successivo al punto in cui è posizionato l'elemento più recente del buffer.
	* @return size_type
	*/
	size_type get_tail() const {
		return tail;
	}
	

	/**
	* Puntatore a ptr (solo lettura).
	* @return T* ptr
	*/
	const T * get_ptr() const {
		return ptr;
	}
	

	/**
	* Aggiunta nuovo elemento.
	* Inserimento (in coda) di un nuovo elemento nel buffer circolare.
	* @param item elemento da aggiungere al buffer.
	*/
	void add_item(T item) {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::add_item(T item)" << endl;
		#endif // !NDEBUG
		/* Se il buffer è pieno tail punta al primo elemento (ovvero vale 0),
		quindi viene sovrascritto l'elemento più vecchio e quindi cambia anche
		la testa del buffer, altrimenti aggiungo un elemento in coda e sposto
		la tail di uno. */
		if (size > 0) {
			ptr[tail] = item;
			tail = (tail + 1) % (size + 1);
			ptr[tail] = 0;

			// Se buffer pieno sposto la testa
			if (pending_items == size) {
				head = (head + 1) % (size + 1);
			} else {
				// Altrimenti il numero degli elementi in coda aumenta di uno
				pending_items++;
			}
			items_amount++; // Aumento il numero di elementi totali mai inseriti nel buffer di uno
		}
	}


	/**
	* Cancellazione elemento (in testa).
	* Cancellazione dell'elemento più vecchio (se ne esiste uno) contenuto nel buffer circolare.
	* @return bool indica se è stato eliminato un elemento o meno.
	*/
	bool delete_item() {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::delete_item()" << endl;
		#endif // !NDEBUG
		/* Se c'è almeno un elemento nel buffer sposto la head di uno in avanti,
		altrimenti non posso eliminare un elemento. */
		if (pending_items > 0) {
			ptr[head] = 0;
			head = (head + 1) % (size + 1);
			pending_items--;
			return true;
		}
		cout << "Impossibile eliminare un elemento dal buffer." << endl;
		return false;
	}


	/**
	* Operatore "[]".
	* Operatore per accedere all'i-esimo elemento del buffer.
	* @param index indice dell'elemento a cui accedere.
	* @return T Reference all'i-esimo elemento nel buffer.
	* @throw out_of_range Lanciata se l'indice è fuori dal buffer.
	*/
	//cbuffer[0] --> elemento più vecchio
	T &operator[] (size_type index) {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::&operator[] (size_type index)" << endl;
		#endif // !NDEBUG
		if (index >= pending_items) {
			throw out_of_range("ERRORE: i e' fuori dal range del buffer.");
		} else {
			int tmp = (head + index) % (size + 1);
			return ptr[tmp];
		}
	}


	/**
	* Operatore "[]" const.
	* Operatore (che ammette solo lettura) per accedere all'i-esimo elemento del buffer.
	* @param index Indice dell'elemento a cui accedere.
	* @return T Reference in sola lettura all'i-esimo elemento nel buffer.
	* @throw out_of_range Lanciata se l'indice è fuori dal buffer.
	*/
	const T &operator[] (size_type index) const {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::&operator[] (size_type index)" << endl;
		#endif // !NDEBUG
		if (index >= pending_items) {
			throw out_of_range("ERRORE: 'i' e' fuori dal range del buffer.");
		} else {
			int tmp = (head + index) % (size + 1);
			return ptr[tmp];
		}
	}
	

	/**
	* Operatore "==".
	* Operatore di confronto di uguaglianza tra due cbuffer contenenenti elementi dello stesso tipo.
	* @param &other cbuffer col quale effettuare il confronto.
	* @return bool true se i due cbuffer sono uguali, false altrimenti
	*/
	bool operator==(cbuffer &other) const {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::operator==(cbuffer &other)" << endl;
		#endif // !NDEBUG
		bool b = (size == other.get_size()) &&
			(items_amount == other.get_items_amount()) &&
			(head == other.get_head()) &&
			(tail == other.get_tail()) &&
			(pending_items == other.get_pending_items()) &&
			(ptr == other.get_ptr());
		return b;
	}


	/**
	* Operatore "!=".
	* Operatore di confronto di disuguaglianza tra due cbuffer contenenenti elementi dello stesso tipo.
	* @param &other cbuffer col quale effettuare il confronto.
	* @return bool true se i due cbuffer sono differenti, false altrimenti
	*/
	bool operator!=(cbuffer &other) const {
		#ifndef NDEBUG
		cout << "# cbuffer<T>::operator!=(cbuffer &other)" << endl;
		#endif // !NDEBUG
		bool b = (size == other.get_size()) &&
			(items_amount == other.get_items_amount()) &&
			(head == other.get_head()) &&
			(tail == other.get_tail()) &&
			(pending_items == other.get_pending_items()) &&
			(ptr == other.get_ptr());
		return !b;
	}


	/**
	* Iteratori
	*/
	class const_iterator;	//iteratore costante
	class iterator;			//iteratore non costante

	class iterator {
	private:
		friend class cbuffer<T>;		//Permette a iterator di accedere ai dati/metodi privati di cbuffer.
		friend class const_iterator;	//Permette a iterator di accedere ai dati/metodi privati di const_iterator.

		T *ptr;				///< Puntatore iteratore ai dati di cbuffer.
		size_type cb_size;	///< Size cbuffer.
		T* cb_begin;		///< Puntatore all'inizio del ptr del cbuffer in memoria.


		/**
		* Costruttore privato.
		* @param p Puntatore ai dati di cbuffer<T>.
		*/
		iterator(T* p) : ptr(p), cb_size(0), cb_begin(0) {}


		/**
		* Costruttore privato.
		* Costruttore privato per inizializzare ptr, cb_size, cb_begin.
		* cbuffer<T> può chiamarlo grazie la friendship stabilita con la classe cbuffer.
		* @param p Puntatore ai dati di cbuffer<T>.
		* @param sz Dimensione del buffer sul quale effettuare l'iterazione.
		* @param beg Puntatore all'inizio del buffer in memoria.
		*/
		iterator(T* p, size_type sz, T* beg) : ptr(p), cb_size(sz), cb_begin(beg) {
			#ifndef NDEBUG
			cout << "Costruttore iteratore : iterator(T* p, size_type sz, T* beg)" << endl;
			#endif
		}

	public:
		
		/**
		* Costruttore di default obbligatorio.
		*/
		iterator() : ptr(0), cb_size(0), cb_begin(0) {}

		/**
		* Copy constructor.
		* @param &other.
		*/
		iterator(const iterator &other) : ptr(other.ptr), cb_size(other.cb_size), cb_begin(other.cb_begin) {}

		/**
		* Operatore di assegnamento.
		* @param &other.
		* @return &iterator.
		*/
		iterator& operator=(const iterator &other) {
			ptr = other.ptr;
			cb_size = other.cb_size;
			cb_begin = other.cb_begin;
			return *this;
		}

		/**
		* Distruttore.
		*/
		~iterator() {}

		/**
		* Dereferenziamento dell'iteratore.
		* @return T& Reference al dato puntato.
		*/
		T& operator*() const {
			return *ptr;
		}

		/**
		* Puntatore all'iteratore.
		* @return T* Puntatore al dato.
		*/
		T* operator->() const {
			return ptr;
		}

		/**
		* Operatore "==" iterator/iterator.
		* Confronto di uguaglianza tra due iterator.
		* @param &other iterator col quale effettuare il confronto.
		* @return bool true se *this punta allo stesso dato di other.
		*/
		bool operator==(const iterator &other) const {
			return (ptr == other.ptr);
		}

		/**
		* Operatore "!=" iterator/iterator.
		* Confronto di disuguaglianza tra due iterator.
		* @param &other iterator col quale effettuare il confronto.
		* @return true se *this non punta allo stesso dato di other.
		*/
		bool operator!=(const iterator &other) const {
			return !(other == *this);
		}


		/**
		* Operatore "==" iterator/const_iterator.
		* Confronto di uguaglianza tra un iterator e un const_iterator.
		* @param &other const_iterator col quale effettuare il confronto.
		* @return bool true se *this punta allo stesso dato di other.
		*/
		bool operator==(const const_iterator &other) const {
			return (ptr == other.ptr);
		}

		/**
		* Operatore "!=" iterator/const_iterator.
		* Confronto di disuguaglianza tra un iterator e un const_iterator.
		* @param &other const_iterator col quale effettuare il confronto.
		* @return true se *this non punta allo stesso dato di other.
		*/
		bool operator!=(const const_iterator &other) const {
			return !(other == *this);
		}

		/**
		* Operatore "++"
		* Spostamento (prefisso).
		* @return iterator& iterator nella nuova posizione.
		*/
		iterator& operator++() {
			++ptr;
			if (ptr >= cb_begin + cb_size + 1) {
				ptr -= cb_size + 1;
			}
			return *this;
		}

		/**
		* Operatore "++(int)"
		* Spostamento (postfisso).
		* @return iterator& iterator nella vecchia posizione.
		*/
		iterator operator++(int) {
			iterator tmp(ptr);
			#ifndef NDEBUG
			//cout << "iterator++ :                ptr = " << (int)ptr << endl;
			//cout << "iterator++ :            cb_size = " << cb_size << endl;
			//cout << "iterator++ :             cb_beg = " << (int)cb_begin << endl;
			//cout << "iterator++ : cb_begin + cb_size = " << (int)(cb_begin + cb_size) << endl;
			#endif

			++ptr;
			if (ptr >= cb_begin + cb_size + 1) {
				ptr -= cb_size + 1;
			}
			return tmp;
		}

	};

	class const_iterator {

		friend class cbuffer<T>;	//Permette a const_iterator di accedere ai dati/metodi privati di cbuffer.
		friend class iterator;		//Permette a const_iterator di accedere ai dati/metodi privati di iterator.

		const T *ptr;		///< Puntatore const ai dati di cbuffer<T>
		size_type cb_size;	///< Size cbuffer.
		T* cb_begin;		///< Puntatore all'inizio del ptr del cbuffer in memoria.


		/**
		* Costruttore privato.
		* @param p Puntatore ai dati di cbuffer<T>.
		*/
		const_iterator(const T* p) : ptr(p), cb_size(0), cb_begin(0) {}

		/**
		* Costruttore privato.
		* Costruttore privato per inizializzare ptr, cb_size, cb_begin.
		* cbuffer<T> può chiamarlo grazie la friendship stabilita con la classe cbuffer.
		* @param p Puntatore const ai dati di cbuffer<T>.
		* @param sz Dimensione del buffer sul quale effettuare l'iterazione.
		* @param beg Puntatore all'inizio del buffer in memoria.
		*/
		const_iterator(const T* p, size_type sz, T* beg) : ptr(p), cb_size(sz), cb_begin(beg) {
			#ifndef NDEBUG
			cout << "Costruttore iteratore : iterator(T* p, size_type sz, T* beg)" << endl;
			#endif
		}

	public:

		/**
		* Costruttore di default.
		*/
		const_iterator() : ptr(0), cb_size(0), cb_begin(0) {}

		/**
		* Copy constructor.
		* @param &other.
		*/
		const_iterator(const const_iterator &other) : ptr(other.ptr), cb_size(other.cb_size), cb_begin(other.cb_begin) {}

		/**
		* Operatore di assegnamento.
		* @param &other.
		* @return &const_iterator.
		*/
		const_iterator & operator=(const const_iterator &other) {
			ptr = other.ptr;
			cb_size = other.cb_size;
			cb_begin = other.cb_begin;
			return *this;
		}

		/**
		* Distruttore.
		*/
		~const_iterator() {}

		/**
		* Dereferenziamento dell'iteratore.
		* @return T& Reference al dato puntato.
		*/
		const T& operator*() const {
			return *ptr;
		}

		/**
		* Puntatore all'iteratore.
		* @return T* Puntatore al dato.
		*/
		const T* operator->() const {
			return ptr;
		}

		/**
		* Operatore "==" const_iterator/const_iterator.
		* Confronto di uguaglianza tra due const_iterator.
		* @param &other const_iterator col quale effettuare il confronto.
		* @return bool true se *this punta allo stesso dato di other.
		*/
		bool operator==(const const_iterator &other) const {
			return (ptr == other.ptr);
		}

		/**
		* Operatore "!=" const_iterator/const_iterator.
		* Confronto di disuguaglianza tra due const_iterator.
		* @param &other const_iterator col quale effettuare il confronto
		* @return true se *this non punta allo stesso dato di other
		*/
		bool operator!=(const const_iterator &other) const {
			return !(other == *this);
		}

		/**
		* Operatore "==" const_iterator/iterator.
		* Confronto di uguaglianza tra un const_iterator e un iterator.
		* @param &other iterator col quale effettuare il confronto.
		* @return bool true se *this punta allo stesso dato di other.
		*/
		bool operator==(const iterator &other) const {
			return (ptr == other.ptr);
		}

		/**
		* Operatore "!=" const_iterator/iterator.
		* Confronto di disuguaglianza tra un const_iterator e un iterator.
		* @param &other iterator col quale effettuare il confronto.
		* @return true se *this non punta allo stesso dato di other.
		*/
		bool operator!=(const iterator &other) const {
			return !(other == *this);
		}

		/**
		* Operatore "++"
		* Spostamento (prefisso).
		* @return const_iterator& const_iterator nella nuova posizione.
		*/
		const_iterator & operator++() {
			++ptr;
			if (ptr >= cb_begin + cb_size + 1) {
				ptr -= cb_size + 1;
			}
			return *this;
		}

		/**
		* Operatore "++(int)"
		* Spostamento (postfisso).
		* @return const_iterator& const_iterator nella vecchia posizione.
		*/
		const_iterator operator++(int) {
			const_iterator tmp(ptr);
			++ptr;
			if (ptr >= cb_begin + cb_size + 1) {
				ptr -= cb_size + 1;
			}
			return tmp;
		}

	};

	/**
	* Richiesta iterator di inizio.
	* @return iterator Iteratore in lettura e scrittura all'elemento più vecchio del buffer circolare.
	*/
	iterator begin() {
		#ifndef NDEBUG
		cout << "# richiesta iteratore begin()" << endl;
		#endif
		return iterator(ptr + head, size, ptr);
	}

	/**
	* Richiesta iterator di fine.
	* @return iterator Iteratore in lettura e scrittura all'elemento successivo all'ultimo elemento del buffer circolare.
	*/
	iterator end() {
		#ifndef NDEBUG
		cout << "# richiesta iteratore end()" << endl;
		#endif
		return iterator(ptr + tail);
	}

	/**
	* Richiesta iterator di inizio.
	* @return iterator Iteratore const all'elemento più vecchio del buffer circolare.
	*/
	const_iterator begin() const {
		#ifndef NDEBUG
		cout << "# richiesta iteratore begin()" << endl;
		#endif
		return const_iterator(ptr + head, size, ptr);
	}

	/**
	* Richiesta const_iterator di fine
	* @return iterator Iteratore const all'elemento successivo all'ultimo elemento del buffer circolare.
	*/
	const_iterator end() const {
		#ifndef NDEBUG
		cout << "# richiesta iteratore end()" << endl;
		#endif
		return const_iterator(ptr + tail);
	}
};

/**
* Funzione Globale check.
* Viene applicato il predicato booleano p ad ogni elemento del buffer e il risultato viene stampato in output.
* Funzionamento: per ogni elemento cb[i] nel buffer stampa a console:
* - "[i]: true"  quando P(cb[i]) è vero.
* - "[i]: false" quando P(cb[i]) è falso.
* @param cb Buffer circolare sul quale testare p.
* @param p Predicato booleano unario.
*/
template <typename F, typename T>
void check(cbuffer<T> &cb, F p) {
	#ifndef NDEBUG
	cout << "# check(cbuffer<T> cb, F p) begin" << endl;
	#endif // !NDEBUG

	try {
		string s = "false";
		for (int i = 0; i < cb.get_pending_items(); ++i) {
			if (p(cb[i])) {
				s = "true";
			} else {
				s = "false";
			}
			cout << "[" << i << "]: " << s << endl;
		}
	} catch (exception& e) {
		cout << "Exception check()" << endl;
	}

	#ifndef NDEBUG
	cout << "# check(cbuffer<T> cb, F p) end" << endl;
	#endif // !NDEBUG
}
#endif
#include<iostream>
#include<initializer_list>
using namespace std;

typedef unsigned int uint;

template<typename T>
class DoubleLinkedList
{
	private:
		class Node
		{
			public:
				T value;
				Node* next;
				Node* prev;

				Node(T value) : value{ value }, next{ nullptr }, prev{ nullptr } {}
		};

		Node* _first;
		Node* _last;

		Node* getNodeByIndex(uint index)
		{
			uint size = getSize();
			if (index >= size)
				throw "Out of range";

			Node* current = nullptr;
			if (index <= (uint)size / 2)
			{
				current = _first;
				for (size_t i = 0; i < index; i++)
				{
					current = current->next;
				}
			}
			else
			{
				current = _last;
				for (size_t i = 0; i < size - index - 1; i++)
				{
					current = _last->prev;
				}
			}

			return current;
		}

	public:
		DoubleLinkedList() : _first{ nullptr }, _last{ nullptr } {}

		DoubleLinkedList(initializer_list<T> list): DoubleLinkedList()
		{
			for (auto i : list)
			{
				addBack(i);
			}
		}

		DoubleLinkedList(const DoubleLinkedList& list) : DoubleLinkedList()
		{
			Node* current = list._first;

			while (current)
			{
				addBack(current->value);
				current = current->next;
			}
		}

		DoubleLinkedList(DoubleLinkedList&& list)
		{
			Node* current    = list._first;
			_first           = current;
			_last            = list._last;

			while (current)
			{
				_first->next = current->next;
				_first->prev = current->prev;
				_first       = _first->next;

				current      = current->next;
			}
			_first           = list._first;

			list._first      = nullptr;
			list._last       = nullptr;
		}

		~DoubleLinkedList()
		{
			if (!isEmpty())
			{
				Node* current = _first->next;
				while (current)
				{
					delete _first;
					_first = current;
					current = current->next;
				}

				delete _first;

				_first = nullptr;
				_last = nullptr;
			}
		}

		bool isEmpty() const
		{
			return _first == nullptr;
		}

		uint getSize() const
		{
			uint counter = 0;

			Node* current = _first;
			while (current)
			{
				counter++;
				current = current->next;
			}

			return counter;
		}

		DoubleLinkedList& clear()
		{
			this->~DoubleLinkedList();

			return *this;
		}

		T getLast()
		{
			return _last->value;
		}

		T getFirst()
		{
			return _first->value;
		}

		T getByIndex(uint index)
		{
			return getNodeByIndex(index)->value;
		}

		T setByIndex(uint index, T value)
		{
			Node* node = getNodeByIndex(index);
			T current  = node->value;

			node->value = value;

			return current;
		}

		T deleteByIndex(uint index)
		{
			if (index >= getSize())
				throw "Out of range";

			T value;

			if (index == 0)
				value = deleteForward();

			else if (index == getSize() - 1)
				value = deleteBack();

			else
			{
				Node* node = getNodeByIndex(index);
				value      = node->value;

				node->prev->next = node->next;
				node->next->prev = node->prev;

				delete node;
			}

			return value;
		}

		DoubleLinkedList& insertByIndex(uint index, T value)
		{
			if (index >= getSize())
				throw "Out of range";

			if (index == 0)
				addForward(value);

			else if (index == getSize() - 1)
			{
				Node* node  = getNodeByIndex(index);

				addBack(_last->value);

				node->value = value;
			}

			else
			{
				Node* newNode        = new Node(value);
				Node* nodeDist       = getNodeByIndex(index);

				nodeDist->prev->next = newNode;
				newNode->next        = nodeDist;

				newNode->prev        = nodeDist->prev;
				nodeDist->prev       = newNode;
			}
		}

		DoubleLinkedList& addBack(T value)
		{
			Node* node = new Node(value);
			if (isEmpty())
			{
				_first = node;
				_last  = node;
			}
			else
			{
				_last->next = node;
				node->prev  = _last;
				_last       = node;
			}

			return *this;
		}

		DoubleLinkedList& addForward(T value)
		{
			Node* node = new Node(value);
			if (isEmpty())
			{
				_first = node;
				_last  = node;
			}
			else
			{
				node->next   = _first;
				_first->prev = node;
				_first       = node;
			}

			return *this;
		}

		T deleteBack()
		{
			if (isEmpty())
				throw "Out of range";

			Node* current = _last;
			T value       = current->value;

			_last = current->prev;
			_last->next = nullptr;

			delete current;

			return value;
		}

		T deleteForward()
		{
			if (isEmpty())
				throw "Out of range";

			Node* current = _first;
			T value       = current->value;

			_first = current->next;
			_first->prev = nullptr;

			delete current;

			return value;
		}

		T& operator[](uint index)
		{
			return getNodeByIndex(index)->value;
		}
		
		uint operator()()
		{
			return getSize();
		}

		T operator()(uint index)
		{
			return deleteByIndex(index);
		}

		T operator()(uint index, T value)
		{
			return setByIndex(index, value);
		}

		friend ostream& operator<<(ostream& out, const DoubleLinkedList& list)
		{
			Node* current = list._first;
			while (current)
			{
				out << current->value << '\t';
				current = current->next;
			}
			out << endl;

			return out;
		}
};

int main()
{
	DoubleLinkedList<int> list;

	list.addBack(4);
	list.addBack(8);
	list.addBack(16);
	list.addBack(32);
	list.addBack(64);

	cout << list << endl;

	return 0;
}

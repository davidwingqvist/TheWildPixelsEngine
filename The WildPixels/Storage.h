#pragma once
#include <algorithm>
namespace storage
{
	// Node containing parent and child pointer, also contains data.
	template <class T> class Node
	{
	public:
		Node* parent = nullptr;
		Node* child = nullptr;
		T data;

		Node(T in)
		{
			this->data = in;
			this->child = nullptr;
			this->parent = nullptr;
		}
	};

	// Double-Linked list storage
	template <class T> class Link
	{
	public:

		Link();
		~Link();


		T &operator[] (unsigned int index);

		// remove data at index position.
		void remove(unsigned int index);

		// push data at the top of the list. r-value
		void push(T &&data);

		// push data at the top of the list. l-value
		void push(T data);

		// push data at the top of the list. r-value
		void push(const T&& data);

		// insert data at index position in list.
		void insert(T data, unsigned int index);

		// pop the top node.
		void pop();

		// return the item at index position.
		T peek(unsigned int index);

		T peek_bottom() const;
		T peek_top() const;

		// return amount of items in list.
		unsigned int size() const;

		bool is_empty() const;

		// set index position to the top node.
		void set_top(unsigned int index);

		// swap positions between first and second input index.
		void swap(unsigned int first, unsigned int second);

		// Removes all items in linked list.
		void reset();

	private:

		// is the bottom node.
		Node<T>* root;

		// is actually "top" from the users perspective
		Node<T>* bottom;

		unsigned int amount;

		// Hopefully will make access operations faster.
		struct NodePointer
		{
			// Pointer to the node.
			Node<T>* point;
			// Index spot in link chain.
			size_t spot;
		}*current;
	};
	template<class T>
	inline Link<T>::Link()
	{
		root = nullptr;
		amount = 0;
		bottom = root;
		this->current = new NodePointer();
		this->current->point = root;
		this->current->spot = 0;
	}
	template<class T>
	inline Link<T>::~Link()
	{
		delete this->current;
		// Clear list.
		if (root == nullptr)
			return;

		if (root->child == nullptr)
		{
			
			if (std::is_pointer<T>::value)
			{
				delete root->data;
			}
			delete root;
			return;
		}

		while (root)
		{
			Node<T>* terminator = root;
			if (std::is_pointer<T>::value)
			{
				delete root->data;
			}
			root = root->child;
			delete terminator;

			if (root->child == nullptr)
			{
				Node<T>* terminator = root;
				if (std::is_pointer<T>::value)
				{
					delete root->data;
				}
				delete terminator;
				break;
			}
		}
	}
	template<class T>
	inline T& Link<T>::operator[](unsigned int index)
	{
		/*
			Worst-case O(n), average O(n), best-case O(1)
		*/

		// Special cases.
		if (index == amount - 1)
			return bottom->data;

		int x, y;
		x = (unsigned int)current->spot - index;
		y = amount - index;

		// Is current->spot closer to the index than bottom and top?
		// start from *current* pos
		if (std::abs(x) < std::abs(y) && std::abs(x) <= (int)index)
		{
			if (index > current->spot)
			{
				// Work yourself up.
				while (current->spot < index)
				{
					current->point = current->point->child;
					current->spot++;
				}

				return current->point->data;
			}
			else if (index < current->spot)
			{
				// Work yourself down.
				while (current->spot > index)
				{
					current->point = current->point->parent;
					current->spot--;
				}

				return current->point->data;
			}
			else // current is already pointing towards indiced spot.
				return current->point->data;
		}

		// start from top.
		if (index > (unsigned int)(amount * 0.5))
		{
			unsigned int spot = amount - index;
			Node<T>* tmp = bottom;
			this->current->spot = amount;
			this->current->point = bottom;

			for (unsigned int i = 0; i < spot; i++)
			{
				if (tmp->parent == nullptr)
				{
					return tmp->data;
				}
				this->current->spot--;

				tmp = tmp->parent;
				this->current->point = tmp;
			}

			return tmp->data;
		}

		// start from root.
		Node<T>* tmp = root;
		this->current->spot = 0;
		this->current->point = root;
		for (unsigned int i = 0; i < index; i++)
		{
			if (tmp->child == nullptr)
			{
				return tmp->data;
			}
			this->current->spot++;

			tmp = tmp->child;
			this->current->point = tmp;
		}

		return tmp->data;
	}
	template<class T>
	inline void Link<T>::remove(unsigned int index)
	{
		/*
			Worst-case O(n), Best-case O(1), todo... make "O(n/2)"
		*/

		/*
			----Strat for O(n/2)----
			Check the index and amount. Is index above (amount / 2 ) start from the bottom and
			work yourself up to the index position.
		*/
		if (index > amount || index < 0 || root == nullptr)
			return;

		if (index > (unsigned int)(amount * 0.5))
		{
			unsigned int spot = amount - index;
		}

		// remove root
		if (index == 0)
		{
			Node<T>* tmp_root = root;
			root = root->child;
			delete tmp_root;
			amount--;
			return;
		}

		// remove top
		else if (index == (amount - 1))
		{
			Node<T>* destroy = bottom->parent;
			delete bottom;
			bottom = destroy;
			bottom->child = nullptr;
			amount--;
			return;
		}

		Node<T>* tmp = root;
		for (unsigned int i = 0; i < index; i++)
		{
			tmp = tmp->child;
		}

		Node<T>* swap = tmp;
		tmp->child->parent = swap->parent;
		tmp->parent->child = swap->child;
		delete swap;
		amount--;
	}
	template<class T>
	inline void Link<T>::push(T &&data)
	{
		/*
			Average O(1)
		*/

		// Special case for root == nullptr
		if (root == nullptr)
		{
			root = new Node<T>(data);
			bottom = root;
			this->current->point = root;
			this->current->spot = 0;
			amount++;
			return;
		}
		else
		{
			// Special case for root != nullptr and bottom = nullptr
			if (amount <= 1)
			{
				bottom = new Node<T>(data);
				bottom->parent = root;
				root->child = bottom;
				//this->current->point = bottom;
				//this->current->spot = amount;
				amount++;
				return;
			}
			else
			{
				Node<T>* tmp = bottom;
				bottom->child = new Node<T>(data);
				bottom = bottom->child;
				//this->current->point = tmp;
				//this->current->spot = amount;
				bottom->parent = tmp;
				amount++;
			}
		}

		data = nullptr;
	}
	template<class T>
	inline void Link<T>::push(T data)
	{
		/*
			Average O(1)
		*/

		// Special case for root == nullptr
		if (root == nullptr)
		{
			root = new Node<T>(data);
			bottom = root;
			this->current->point = root;
			this->current->spot = 0;
			amount++;
			return;
		}
		else
		{
			// Special case for root != nullptr and bottom = nullptr
			if (amount <= 1)
			{
				bottom = new Node<T>(data);
				bottom->parent = root;
				root->child = bottom;
				//this->current->point = bottom;
				//this->current->spot = amount;
				amount++;
				return;
			}
			else
			{
				Node<T>* tmp = bottom;
				bottom->child = new Node<T>(data);
				bottom = bottom->child;
				//this->current->point = tmp;
				//this->current->spot = amount;
				bottom->parent = tmp;
				amount++;
			}
		}
	}
	template<class T>
	inline void Link<T>::insert(T data, unsigned int index)
	{

	}
	template<class T>
	inline void Link<T>::pop()
	{
		/*
			Average O(1)
		*/
		if (root == nullptr || bottom == nullptr || amount <= 0)
			return;

		Node<T>* temp = bottom;
		if (bottom != root)
		{
			bottom = bottom->parent;
			delete temp;
			bottom->child = nullptr;
			amount--;
		}
		else
		{
			bottom = nullptr;
			delete root;
			root = nullptr;
			amount = 0;
		}
	}
	template<class T>
	inline T Link<T>::peek(unsigned int index)
	{
		/*
			Worst-case O(n), average O(n), best-case O(1)
		*/

		// Special cases.
		if (index == amount - 1)
			return bottom->data;

		int x, y;
		x = (unsigned int)current->spot - index;
		y = amount - index;

		// Is current->spot closer to the index than bottom and top?
		// start from *current* pos
		if (std::abs(x) < std::abs(y) && std::abs(x) <= (int)index)
		{
			if (index > current->spot)
			{
				// Work yourself up.
				while (current->spot < index)
				{
					current->point = current->point->child;
					current->spot++;
				}

				return current->point->data;
			}
			else if (index < current->spot)
			{
				// Work yourself down.
				while (current->spot > index)
				{
					current->point = current->point->parent;
					current->spot--;
				}

				return current->point->data;
			}
			else // current is already pointing towards indiced spot.
				return current->point->data;
		}

		// start from top.
		if (index > (unsigned int)(amount * 0.5))
		{
			unsigned int spot = amount - index;
			Node<T>* tmp = bottom;
			this->current->spot = amount;
			this->current->point = bottom;

			for (unsigned int i = 0; i < spot; i++)
			{
				if (tmp->parent == nullptr)
				{
					return tmp->data;
				}
				this->current->spot--;

				tmp = tmp->parent;
				this->current->point = tmp;
			}

			return tmp->data;
		}

		// start from root.
		Node<T>* tmp = root;
		this->current->spot = 0;
		this->current->point = root;
		for (unsigned int i = 0; i < index; i++)
		{
			if (tmp->child == nullptr)
			{
				return tmp->data;
			}
			this->current->spot++;

			tmp = tmp->child;
			this->current->point = tmp;
		}

		return tmp->data;
	}
	template<class T>
	inline T Link<T>::peek_bottom() const
	{
		return root->data;
	}
	template<class T>
	inline T Link<T>::peek_top() const
	{
		return bottom->data;
	}
	template<class T>
	inline unsigned int Link<T>::size() const
	{
		return amount;
	}
	template<class T>
	inline bool Link<T>::is_empty() const
	{
		if (amount)
			return false;
		else
			return true;
	}
	template<class T>
	inline void Link<T>::set_top(unsigned int index)
	{
		/*
			Worst-case O(n)
		*/
		if (index > amount || index < 0)
			return;

		Node<T>* swap = root;
		for (unsigned int i = 0; i < index; i++)
		{
			swap = swap->child;
		}

		// Safety net
		if (swap == nullptr || bottom == nullptr)
			return;

		// Swap data between pointers.
		std::swap(swap->data, bottom->data);
	}
	template<class T>
	inline void Link<T>::swap(unsigned int first, unsigned int second)
	{
		/*
			Worst-case O(n)
		*/

		Node<T>* swap = root;
		Node<T>* swap2 = root;
		for (unsigned int i = 0; i < amount; i++)
		{
			if (i < first)
			{
				swap = swap->child;
			}

			if (i < second)
			{
				swap2 = swap2->child;
			}
		}

		std::swap(swap->data, swap2->data);
	}
	template<class T>
	inline void Link<T>::reset()
	{
		/*
			Average O(n)
		*/
		for (unsigned int i = 0; i < amount; i++)
		{
			Node<T>* terminator = bottom;
			bottom = bottom->parent;
			delete terminator;
		}

		root = nullptr;
		bottom = nullptr;
		amount = 0;
	}
}
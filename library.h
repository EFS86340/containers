#ifndef LIST_LIBRARY_H
#define LIST_LIBRARY_H

#include <cstddef>

/*
 * this is a stl list implementation review, sgi stl version implementation.
 * Fri Jul 14 2017
 */



template<typename T>
struct _list_node {
    typedef void*	void_pointer;
    void_pointer	prev;
    void_pointer next;
    //_list_node<T>*	prev;
    //_list_node<T>* 	next;
    T	data;
};

struct input_iterator_tag   {   };
struct forward_iterator_tag :   public input_iterator_tag   {   };
struct bidirectional_iterator_tag : public  forward_iterator_tag {  };

// iterators
template<typename T, typename Ref, typename Ptr>
struct _list_iterator {
    typedef	_list_iterator<T, T&, T*>	iterator;
    typedef _list_iterator<T, Ref, Ptr> self;

    typedef	bidirectional_iterator_tag	iterator_category;
    typedef	T	value_type;
    typedef	Ptr	pointer;
    typedef	Ref	reference;
    typedef	_list_node<T>*	link_type;
    typedef	size_t	size_type;
    typedef	ptrdiff_t	difference_type;

    /*
     * explaination fot ptrdiff_t:
     * 	if two pointers minus, then their result becomes a distance, the corresponding type
     * 	we define it as ptrdiff_t, and it's stored as long int.
     */

    link_type node;

    //constructor

    _list_iterator(link_type x) : node(x) { }
    _list_iterator(const iterator &x) : node(x.node) { }
    _list_iterator() { }

    bool operator==(const self &x) const {	return node == x.node;	}

    bool operator!=(const self &x) const {	return node != x.node; 	}

    reference operator*() const {	return (*node).data;	}

    pointer	operator->() const	{	return &(operator*());	}

    self& operator++() {
        node = (link_type)((*node).next);
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--() {
        node = (link_type)((*node).prev);
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }

};



//	end of class 	_list_iterator

template<typename T, typename Alloc>
class simple_alloc {
public:
    static T *allocate(size_t n) {
        return 0 == n? 0 : (T*)Alloc::allocate(n * sizeof(T));
    }

    static T *allocate(void) {
        return (T*)Alloc::allocate(sizeof(T));
    }

    static void deallocate(T *p, size_t n) {
        if(0 != n)  Alloc::deallocate(p, n * sizeof(T));
    }

    static void deallocate(T *p) {
        Alloc::deallocate(p, sizeof(T));
    }
};


template<typename T, typename Alloc = alloc>
class list {
protected:
    typedef _list_node<T> list_node;
    typedef _list_iterator<T, T &, T *> iterator;
    typedef T &reference;
public:
    typedef list_node *link_type;
    typedef simple_alloc<list_node, Alloc> list_node_allocator;

protected:
    link_type node;
    // whole circle list

    link_type get_node() { return list_node_allocator::allocate(); }

    void put_node(link_type p) { list_node_allocator::deallocate(p); }

    link_type create_node(const T &x) {
        link_type p = get_node();
        construct(&p->data, x);        // whole scale function
        return p;
    }

    void destroy_node(link_type p) {
        destroy(&p->data);
        put_node(p);
    }

    void empty_initialize() {
        node = get_node();
        node->next = node;
        node->prev = node;
    }

public:
    //constructor
    list() { empty_initialize(); }

    iterator begin() { return (link_type) ((*node).next); }

    iterator end() { return node; }

    bool empty() const { return node->next == node; }

    size_t size() const {
        size_t result = 0;
        distance(begin(), end(), result);    // whole scale function
        return result;
    }

    reference front() { return *begin(); }

    reference back() { return *(--end()); }    // does reference here not defined?

    iterator insert(iterator position, const T &x) {
        link_type tmp = create_node(x);
        tmp->next = position.node;
        tmp->prev = position.node->prev;
        (link_type(position.node->prev))->next = tmp;
        position.node->prev = tmp;
        return tmp;
    }

    iterator erase(iterator position) {
        link_type prev_node = (link_type)position.node->prev;
        link_type next_node = (link_type)position.node->next;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(position.node);
        return iterator(next_node);
    }

    void transfer(iterator position, iterator first, iterator last) {
        if(position != last) {
            (*(link_type((*last.node).prev))).next = position.node;
            (*(link_type((*first.node).prev))).next = last.node;
            (*(link_type((*position.node).prev))).next = first.node;
            link_type tmp = (link_type)((*position.node).prev);
            (*position.node).prev = (*last.node).prev;
            (*last.node).prev = (*first.node).prev;
            (*first.node).prev = tmp;
        }
    }

    void push_front(const T &x) {   insert(begin(), x); }
    void push_back(const T &x) {    insert(end(), x);   }

    void clear();
    void remove(const T &value);
    void unique();
};

template <class T, class Alloc>
void list<T, Alloc>::clear() {
    link_type cur = (link_type)node->next;
    while(cur != node) {
        link_type tmp = cur;
        cur = (link_type) cur->next;
        destroy_node(tmp);
    }

    node->next = node;
    node->prev = node;
};

template <class T, class Alloc>
void list<T, Alloc>::remove(const T &value) {
    iterator first = begin();
    iterator last = end();
    while(first != last) {
        iterator next = first;
        ++next;
        if(*first == value)
            erase(first);
        first = last;
    }
}

template <class T, class Alloc>
void list<T, Alloc>::unique() {
    iterator first = begin();
    iterator last = end();
    if(first == last) return;
    iterator next = first;
    while(++next != last) {
        if(*first == *next)
            erase(next);
        else
            first = next;
        next = first;
    }
}

#endif
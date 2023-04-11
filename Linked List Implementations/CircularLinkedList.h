#ifndef CIRCULARLINKEDLIST_H_
#define CIRCULARLINKEDLIST_H_
#include"TemporaryEmployee.h"
#define max 20 // define max

struct NodeA{ // node struct for array based
	TemporaryEmployee tEmp;
	int next;
};

class CircularLinkedList {
public:
	struct NodeA list[max];
	int avail = 0;
	int last = max-1;

	void insertLast(TemporaryEmployee emp); // insert at back
	void deleteAfter(int p); // delete after p

	bool searchSameNumber(int num); // search same number
	TemporaryEmployee* findByNumber(int num); // find by number
	void printEmployee(int num); // print employee

	CircularLinkedList();
	virtual ~CircularLinkedList();
};

#endif

#ifndef DOUBLELINKEDLIST_H_
#define DOUBLELINKEDLIST_H_
#include"PermanentEmployee.h"

struct Node { // struct for dynamically allocated linked list node
    PermanentEmployee employee;
    Node* next;
    Node* prev;
};

class DoubleLinkedList {
public:

	Node* head; // head

	DoubleLinkedList();
	virtual ~DoubleLinkedList();

	void insertFront(struct Node** head, PermanentEmployee newEmp); // push

	void deleteByNumber(struct Node** head, int num); // delete by number

	bool searchSameNumber(int num); // search same number

	PermanentEmployee* findByNumber(int num); // find by number

	void printEmployee(int num); // print employee
};

#endif

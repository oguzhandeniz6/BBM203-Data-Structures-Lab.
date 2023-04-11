#include <iostream>
#include "DoubleLinkedList.h"

DoubleLinkedList::DoubleLinkedList() {head = NULL;}

DoubleLinkedList::~DoubleLinkedList() {}

void DoubleLinkedList::insertFront(Node **head, PermanentEmployee newEmp)
{
	struct Node* newNode = new Node; // create a node

	newNode->employee = newEmp;

	newNode->next = *head;
	newNode->prev = NULL;

	if ((*head) != NULL) // insert at front
	{
		(*head)->prev = newNode;
	}

	(*head) = newNode; // change head

}

void DoubleLinkedList::deleteByNumber(Node** head, int num)
{
	Node* tempNode;
	tempNode = this->head;
	while(tempNode != NULL)
	{
		if(tempNode->employee.eNumber == num)
		{
			break;
		}
		else
		{
			tempNode = tempNode->next;
		}
	}

	if(*head == NULL || tempNode == NULL) // Base case
	{
		return;
	}

	if(*head == tempNode) // If deleted node is head
	{
		*head = tempNode->next;
	}

	if(tempNode->next != NULL) // Change next only if not last node
	{
		tempNode->next->prev = tempNode->prev;
	}

	if(tempNode->prev != NULL) // Change prev only if not first node
	{
		tempNode->prev->next = tempNode->next;
	}

	delete(tempNode); // Free the memory

	return;

}

bool DoubleLinkedList::searchSameNumber(int num)
{
	bool returnValue = false; // return value

	Node* tempNode;
	tempNode = head;
	while(tempNode != NULL) // iterating over list
	{
		if(tempNode->employee.eNumber == num) // if numbers are equal
		{
			returnValue = true;
		}
		tempNode = tempNode->next;
	}

	return returnValue;
}

PermanentEmployee* DoubleLinkedList::findByNumber(int num)
{
	PermanentEmployee* x; // return value

	Node* tempNode;
	tempNode = head;
	while(tempNode != NULL) // iterating over list
	{
		if(tempNode->employee.eNumber == num) // if numbers are equal
		{
			x = &tempNode->employee;
		}
		tempNode = tempNode->next;
	}

	return x;
}

void DoubleLinkedList::printEmployee(int num)
{
	PermanentEmployee* x = DoubleLinkedList::findByNumber(num); // find by number

	cout << "Employee Number: " << x->eNumber << endl;
	cout << "Employee Type: Permanent" << endl;
	cout << "Employee Name: " << x->eName << endl;
	cout << "Employee Surname: " << x->eSurname << endl;
	cout << "Employee Title: " << x->eTitle << endl;
	cout << "Employee Salary Coefficience: " << x->salaryCo << endl;
	cout << "Employee's Date of Birth:" << x->birth.day << "-" << x->birth.month << "-" << x->birth.year << endl;
	cout << "Employee's Date of Appointment: " << x->join.day << "-" << x->birth.month << "-" << x->join.year << endl;
	cout << "Employee's Length of Service in Other Institutions: " << x->exp << " days" << endl;
}


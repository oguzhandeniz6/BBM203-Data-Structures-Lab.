#include "CircularLinkedList.h"

CircularLinkedList::CircularLinkedList() { // construct list
	for(int i = 0; i < max-1; i++)
	{
		list[i].next = i+1;
	}
	list[max-1].next = 0;
}

CircularLinkedList::~CircularLinkedList() {}

void CircularLinkedList::insertLast(TemporaryEmployee emp)
{
	if ((avail == -1 ) || (last == -1)) // base case
	{
		cout << "Error" << endl;
		return;
	}
	int x = avail; // available node
	avail = list[avail].next;

	list[x].tEmp = emp;
	list[x].next = list[last].next;
	list[last].next = x;

	last = x; // last node
}

void CircularLinkedList::deleteAfter(int p)
{
	int q;

	if((p == -1) || (list[p].next == -1)) // base case
	{
		cout << "Error" << endl;
		return;
	}

	q = list[p].next;
	list[p].next = list[q].next;
	list[q].next = avail;
	avail = q;
}

bool CircularLinkedList::searchSameNumber(int num)
{
	bool returnValue = false; // return value

	for(int i = 0; i < max; i++)
	{
		if(list[i].tEmp.eNumber == num) // if numbers are equal
		{
			returnValue = true;
			break;
		}
	}

	return returnValue;
}

TemporaryEmployee* CircularLinkedList::findByNumber(int num)
{
	TemporaryEmployee* returnEmp; // return employee

	for(int i = 0; i < max; i++)
	{
		if(list[i].tEmp.eNumber == num) // if numbers are equal
		{
			returnEmp = &list[i].tEmp;
			break;
		}
	}

	return returnEmp;
}

void CircularLinkedList::printEmployee(int num)
{
	TemporaryEmployee* x = CircularLinkedList::findByNumber(num); // find employee by number

	cout << "Employee Number: " << x->eNumber << endl;
	cout << "Employee Type: Temporary" << endl;
	cout << "Employee Name: " << x->eName << endl;
	cout << "Employee Surname: " << x->eSurname << endl;
	cout << "Employee Title: " << x->eTitle << endl;
	cout << "Employee Salary Coefficience: " << x->salaryCo << endl;
	cout << "Employee's Date of Birth:" << x->birth.day << "-" << x->birth.month << "-" << x->birth.year << endl;
	cout << "Employee's Date of Appointment: " << x->join.day << "-" << x->birth.month << "-" << x->join.year << endl;
	cout << "Employee's Length of Service in Other Institutions: " << x->exp << " days" << endl;}

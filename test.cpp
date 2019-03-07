#include <bits/stdc++.h>

using namespace std;

struct Node { 
  int data; 
  struct Node *next; 
}; 

int main() {

	Node* head = new Node();
	head->data = 1;
	head->next = new Node();
	head->next->data = 2;

	uintptr_t pointer;

	// cout << head->data << endl;

	printf("%p\n", head);
	printf("%p\n", head->next);

	// cout << head->next->data << endl;

}
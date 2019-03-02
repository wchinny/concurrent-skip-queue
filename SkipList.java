import java.util.*;
import java.io.*;

class Node<T extends Comparable<T>> {

	T value;
	ArrayList<Node<T>> nextPointers;
	int height;

	Node(int height) {								// Constructor for Node
		this.nextPointers = new ArrayList<>(height);
		this.height = height;
		for(int i = 0; i < height; i++) {
			this.nextPointers.add(null);
		}
	}

	Node(T data, int height) {						// Another construct for node that also takes data
		this.nextPointers = new ArrayList<>();
		this.value = data;
		this.height = height;
		for(int i = 0; i < height; i++) {
			this.nextPointers.add(null);
		}
	}

	public T value() {			// Returns value of the node
		return this.value;
	}

	public int height() {		// Returns height of the node
		return this.height;
	}

	public Node<T> next(int level) {				// Returns the reference to the next node in the specified level
		if(level < 0 || level > this.height - 1) {
			return null;
		}

		return this.nextPointers.get(level);
	}

	// SUGGESTED METHODS //

	public void setNext(int level, Node<T> node) {	// Set the next reference at a specified level to point to a node
		this.nextPointers.set(level, node);
	}

	public void grow() {				// Grows the node by height 1
		this.height++;
		this.nextPointers.add(null);
	}

	public void maybeGrow() {			// Grows the node with a 50 percent probability
		double prob = 0.5;
		if(Math.random() < prob) {
			this.height++;
			this.nextPointers.add(null);
		}
	}

	public void trim(int height) { 		// Trims the node until it reaches a specified height
		if(this.value != null && this.height == 1) {
			return;
		}
		if(height >= this.height) {
			return;
		}
		for(int i = this.height-1; i >= height; i--) {	// Removes the top pointers until the height reaches the desired height
			this.nextPointers.remove(i);
			this.height--;
		}
		return;
	}
}

public class SkipList<T extends Comparable<T>> {

	Node<T> head;
	int height;
	int size;

	SkipList() { 					// Constructor to create a skiplist
		this.head = new Node<>(1);
		this.height = 1;
		this.size = 0;
	}

	SkipList(int height) {				// Another constructor to create a skiplist of a specific height
		this.head = new Node<>(height);
		this.height = height;
		this.size = 0;
	}

	public int size() {		// Returns the size of the skiplist
		return this.size; 
	}

	public int height() {	// Returns the height of the skiplist
		return this.height;
	}

	public Node<T> head() {	// Returns the reference to the head of the skiplist
		return this.head; 
	}

	public void insert(T data) {	// Inserts a value to the skiplist, in sorted order, with an optimal height
		Node<T> curr;
		int newsize = this.size+1;

		if(this.size == 0) {
			newsize++;
		}

		int log = (int)Math.ceil(lg(newsize));		
		int maxHeight = Math.max(log, this.height);			// Find the max suitable height for the node to be inserted in the list
		int randHeight = generateRandomHeight(maxHeight);	// Randomize a height that is withing constraints of a suitable height
		int newheight = this.height+1;				

		if(log > this.height) {								// Check if inserting the new node would require growing the skip list.
			growSkipList();
		}

		insert(data, randHeight);							// Use the insert method with the random generated height
		return;
	}

	public void insert(T data, int height) {			// Insert a node with a value into the list in O(log(n)) time

		Node<T> curr = this.head;
		Node<T> temp = new Node<T>(data, height);
		ArrayList<Node<T>> path = new ArrayList<>();	// ArrayList to store the traversal of the list, to keep track 
														// of references to be updated
		this.size++;

		for(int i = 0; i < this.height; i++) {			// Initializes the path ArrayList to store null references first
			path.add(null);
		}

		for(int i = this.height - 1; i >= 0; i--) {		// BST-style traversal of the skiplist
			if(curr.next(i) == null) {
				path.set(i, curr);
			}
			else if(data.compareTo(curr.next(i).value()) < 0) {
				path.set(i, curr);
			}
			else if(data.equals(curr.next(i).value())) {
				path.set(i, curr);
			}
			else {
				curr = curr.next(i);
				i++;
			}
		}

		for(int i = temp.height()-1; i >= 0; i--) {			// Updates references to point to the new node, and for the new node
				temp.setNext(i, path.get(i).next(i));		// to point to its next node
				path.get(i).setNext(i, temp);
		}
	}

	public void delete(T data) {							// Deletes a node (if exists) from the list in O(log(n)) time

		Node<T> curr = this.head;
		Node<T> want = null;								// This will be used to check if our node-to-be-deleted exists in the list
		ArrayList<Node<T>> path = new ArrayList<>();
		Node<T> temp = new Node<T>(data, height);

		for(int i = 0; i < this.height; i++) {				// Initialization for the path ArrayList
			path.add(null);
		}

		for(int i = this.height - 1; i >= 0; i--) {			// BST-style traversal of the skiplist
			if(curr.next(i) == null) {
				path.set(i, curr);
			}
			else if(data.compareTo(curr.next(i).value()) < 0) {
				path.set(i, curr);
			}
			else if(data.equals(curr.next(i).value())) {	// If we find our node-to-be-deleted, we store it
				path.set(i, curr);
				want = curr.next(i);
			}
			else {
				curr = curr.next(i);
				i++;
			}
		}

		if(want == null) {	// If we couldn't find our node-to-be-deleted, just return since there isn't anything to delete
			return;
		}

		for(int i = want.height()-1; i >= 0; i--) {		// Update references so we don't lose the references that our deleted node was pointing to.
			path.get(i).setNext(i, want.next(i));		// *Poof* we lost the references to our deleted node, so the Garbage Collector will get rid of it for us.
		}

		this.size--;	// Decrement the size of the skip list

		int newsize = this.size;
		int log = (int)Math.ceil(lg(newsize));

		if(newsize == 1 || newsize == 0) {
			log = 1;
		}

		while(log < this.height) {		// If our height is sub-optimal, we trim the list until the height becomes optimal
			trimSkipList();
		}
		return;
	}

	public boolean contains(T data) {			// Checks if the list contains a particular value in O(log(n)) time

		Node<T> curr = this.head;		

		for(int i = this.height - 1; i >= 0; i--) {		// BST-style traversal of the list
			if(curr.next(i) == null) {
			}
			else if(data.compareTo(curr.next(i).value()) < 0) {
			}
			else if(data.equals(curr.next(i).value())) { 
				return true;		// If we find our node, return true
			}	
			else {
				curr = curr.next(i);
				i++;
			}
		}
		return false; 	// If we couldn't find it, return false
	}

	public Node<T> get(T data) {		// Returns a reference to a specified node (if exists) in O(log(n)) time

		Node<T> curr = this.head;
		Node<T> want = null;

		for(int i = this.height - 1; i >= 0; i--) {				// BST-style traversal
			if(curr.next(i) == null) {
			}
			else if(data.compareTo(curr.next(i).value()) < 0) {
			}
			else if(data.equals(curr.next(i).value())) {
				want = curr.next(i);
				return want;		// If we find our node, return the reference to it
			}
			else {
				curr = curr.next(i);
				i++;
			}
		}

		return want; 	// If at this point we couldn't find our node, return null ('want' was initialized to null)
	}
	
	private static int generateRandomHeight(int maxHeight) {	// Generates a random height (within constraints) for a node.

		int level = 1;
		double prob = 0.5;

		while(Math.random() < prob) {	// Pretty much flipping a coin until you get an unwanted side.
			level++;
			if(level > maxHeight) {
				level = maxHeight;
				break;
			}
		}
		return level;
	}

	private void growSkipList() {	// Grows the skiplist by height 1.

		Node<T> curr = this.head().next(this.height);

		if(curr == null) {			// If there is nothing else at max height except the head, just grow the head.
			this.head.grow();
			this.height++;
			return;
		}

		do {						// Grow the top level nodes of the list by height 1
			curr.maybeGrow();
			curr = curr.next(this.height);
		} while(curr.next(this.height) != null);

		this.head.grow();			// Then grow the head
		this.height++;
	}
	

	private void trimSkipList() {	// Trims the skip list by height 1.

		Node<T> curr = this.head().next(this.height-1);
		Node<T> temp;

		if(curr == null) {					// If there is nothing at max height, just trim the head.
			this.head.trim(this.height-1);
			this.height--;
			return;
		}

		for(;;) {								// Trims all of the max-height nodes of the list by height 1
			temp = curr.next(this.height-1);
			curr.trim(this.height-1);
			if(temp == null) {
				break;
			}
			curr = temp;
		}

		this.head.trim(this.height-1);		// Then trim the head.
		this.height--;
	}

	public void print() {
        int curr_level = this.head.height();
        for(int i = curr_level - 1; i >= 0; i--) {
            Node<T> curr = this.head;
            while(curr != null) {
                System.out.print(((curr.value() == null) ? "null" : curr.value()) + " -> ");
                curr = curr.next(i);
            }
            System.out.println();
        }
        System.out.println("----");
    }

	private double lg(int a) {			// A base-2 logarithm function
		return logb(a, 2);
	} 

	private double logb(double a, double b) {	// A logartihm base converter using a property of logarithms
		return Math.log(a) / Math.log(b);
	}

	public static void main(String[] args) {

		SkipList<Integer> s = new SkipList<>();

		for(int i = 0; i < 100000; i++) {
			s.insert(i);
		}

		s.print();

		System.out.println(s.height());

	}
}






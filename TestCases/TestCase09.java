// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase09.java
// =========================
// Check that the delete() method does not change the height of a skip list if
// the value to be deleted is not actually in the skip list. This is very
// similar to TestCase08.java.


import java.io.*;
import java.util.*;

public class TestCase09
{
	// Check that the skiplist's size and height are as expected. If not, fail whale.
	private static void checkList(SkipList<Integer> s, int expectedSize, int expectedHeight)
	{
		if (s.size() != expectedSize || s.height() != expectedHeight)
		{
			System.out.println("fail whale :(");
			System.exit(1);
		}
	}

	public static void main(String [] args)
	{
		int size, height;
		ArrayList<Integer> heights = new ArrayList<Integer>();
		SkipList<Integer> skiplist = new SkipList<Integer>();

		skiplist.insert(RNG.getUniqueRandomInteger());
		checkList(skiplist, size = 1, height = 1);

		skiplist.insert(RNG.getUniqueRandomInteger());
		checkList(skiplist, size = 2, height = 1);

		skiplist.insert(RNG.getUniqueRandomInteger());
		checkList(skiplist, size = 3, height = 2);

		skiplist.insert(RNG.getUniqueRandomInteger());
		checkList(skiplist, size = 4, height = 2);

		skiplist.insert(RNG.getUniqueRandomInteger());
		checkList(skiplist, size = 5, height = 3);

		// Loop through skip list and get the heights of all the nodes.
		Node<Integer> temp = skiplist.head().next(0);
		for (int i = 0; i < 5; i++)
		{
			heights.add(temp.height());
			temp = temp.next(0);
		}

		// Call delete() on an element that is not in the skip list. Then check
		// that things are okay.
		skiplist.delete(RNG.getRandomUnusedInteger());
		checkList(skiplist, size = 5, height = 3);

		// Loop through the list to ensure that all the nodes' heights are
		// preserved.
		boolean success = true;
		temp = skiplist.head().next(0);
		for (int i = 0; i < 5; i++)
		{
			success &= (temp.height() == heights.get(i));
			temp = temp.next(0);
		}

		System.out.println(success ? "Hooray!" : "fail whale :(");
	}
}

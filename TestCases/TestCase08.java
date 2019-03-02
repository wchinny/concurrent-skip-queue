// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase08.java
// =========================
// Check that the delete() method changes the overall height of a skip list when
// appropriate.


import java.io.*;
import java.util.*;

public class TestCase08
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
		int size, height, r;
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

		skiplist.insert(r = RNG.getUniqueRandomInteger());
		checkList(skiplist, size = 5, height = 3);

		// Loop through skip list and get the heights of all the nodes.
		Node<Integer> temp = skiplist.head().next(0);
		for (int i = 0; i < 5; i++)
		{
			if (temp.value() != r)
				heights.add(temp.height());

			temp = temp.next(0);
		}

		// Delete element. Check that things are okay.
		skiplist.delete(r);
		checkList(skiplist, size = 4, height = 2);

		// Loop through the list to ensure that all the tallest nodes got
		// truncated.
		boolean success = true;
		temp = skiplist.head().next(0);
		for (int i = 0; i < 4; i++)
		{
			success &= (temp.height() == Math.min(heights.get(i), 2));
			temp = temp.next(0);
		}

		System.out.println(success ? "Hooray!" : "fail whale :(");
	}
}

// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase11.java
// =========================
// When a skip list is created with a manually specified initial height,
// attempting to delete an element that is *not* in the skip list should not
// reduce its height.


import java.io.*;
import java.util.*;

public class TestCase11
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
		SkipList<Integer> skiplist = new SkipList<Integer>(5);

		// Insert three random values into the skip list.
		skiplist.insert(RNG.getUniqueRandomInteger());
		checkList(skiplist, size = 1, height = 5);

		skiplist.insert(RNG.getUniqueRandomInteger());
		checkList(skiplist, size = 2, height = 5);

		skiplist.insert(RNG.getUniqueRandomInteger());
		checkList(skiplist, size = 3, height = 5);

		// Now delete one of those values at random.
		skiplist.delete(RNG.getRandomUnusedInteger());
		checkList(skiplist, size = 3, height = 5);

		System.out.println("Hooray!");
	}
}

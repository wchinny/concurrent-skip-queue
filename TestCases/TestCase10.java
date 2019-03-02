// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase10.java
// =========================
// When a skip list is created with a manually specified initial height,
// successfully deleting an element from the skip list should truncate the whole
// list's height back to the more conventional ceiling(log_2(n)).


import java.io.*;
import java.util.*;

public class TestCase10
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
		skiplist.delete(RNG.getRandomUsedInteger());
		checkList(skiplist, size = 2, height = 1);

		System.out.println("Hooray!");
	}
}

// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase13.java
// =========================
// When deletion causes a skip list to have a single element remaining, the
// height should only be reduced to 1 (not 0).


import java.io.*;
import java.util.*;

public class TestCase13
{
	private static void failwhale()
	{
		System.out.println("fail whale :(");
		System.exit(1);
	}

	// Check that the skiplist's size and height are as expected. If not, fail whale.
	private static void checkList(SkipList<Integer> s, int expectedSize, int expectedHeight)
	{
		if (s.size() != expectedSize || s.height() != expectedHeight)
			failwhale();
	}

	public static void main(String [] args)
	{
		int size, height;
		SkipList<Integer> skiplist = new SkipList<Integer>();

		// Insert three elements into the skip list.
		skiplist.insert(10);
		checkList(skiplist, size = 1, height = 1);

		skiplist.insert(30);
		checkList(skiplist, size = 2, height = 1);

		skiplist.insert(20);
		checkList(skiplist, size = 3, height = 2);

		// Now check the height of the list as elements are deleted.
		skiplist.delete(30);
		checkList(skiplist, size = 2, height = 1);

		skiplist.delete(10);
		checkList(skiplist, size = 1, height = 1);

		// The height of an empty list may be 0 or 1. It's your choice; either way
		// is fine.
		skiplist.delete(20);
		if (skiplist.size() != 0) failwhale();
		if (skiplist.height() != 0 && skiplist.height() != 1) failwhale();

		System.out.println("Hooray!");
	}
}

// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase12.java
// =========================
// A skip list with a single node should have a height of 1 (not 0). (This is
// also tested in TestCase07.java. This test case merely isolates that
// expectation.)


import java.io.*;
import java.util.*;

public class TestCase12
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
		SkipList<Integer> skiplist = new SkipList<Integer>();

		// Insert into the skip list and check its height.
		skiplist.insert(RNG.getUniqueRandomInteger());
		checkList(skiplist, size = 1, height = 1);

		System.out.println("Hooray!");
	}
}

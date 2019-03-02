// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase07.java
// =========================
// Ensure that the skip list height is growing as expected, and that when an
// insertion causes the skip list's height to grow, it is possible for the new
// node's height to be equal to the new height of the skip list.
//
// Note that a skip list with a single node should have a height of 1 (not 0).


import java.io.*;
import java.util.*;

public class TestCase07
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
		boolean isTall = false, isShort = false;
		RNG.setMaxRandomValue(1000);

		// It is improbably that 40 iterations of this would not result in having
		// seen the last node reach height 3 at some point,  as well as a lesser
		// height at some point.
		for (int i = 0; i < 40; i++)
		{
			// 'size' and 'height' variables are used to improve readability of
			// checkList() method calls.
			int size, height, r;
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

			// Search skip list for the most recently inserted value, r.
			Node<Integer> temp = skiplist.head().next(0);
			while (temp.value() != r)
				temp = temp.next(0);

			// Check whether r has height 3. This should be possible, although it won't
			// always happen.
			if (temp.height() == 3)
				isTall = true;
			else
				isShort = true;

			if (isTall && isShort)
			{
				System.out.println("Hooray!");
				return;
			}
		}

		System.out.println("fail whale :(");
	}
}

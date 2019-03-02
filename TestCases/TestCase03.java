// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase03.java
// =========================
// This test case manually checks that the height of the skip list is growing as
// expected.


import java.io.*;
import java.util.*;

public class TestCase03
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
		int size, height, initHeight = 3;

		SkipList<Integer> skiplist = new SkipList<Integer>(initHeight);
		checkList(skiplist, size = 0, height = 3);

		// In case the following for-loops are too difficult to decipher, here's
		// what's going on:

		// When i = 0, this adds your 1st and 2nd elements, and expects the height
		// to be the max of 1 (which is i + 1) and 3 (which is the initial height).

		// When i = 1, this adds your 3rd and 4th elements, and expects the height
		// to be the max of 2 (which is i + 1) and 3 (which is the initial height).

		// When i = 2, this adds your 5th, 6th, 7th, and 8th elements, and expects
		// the height to be the max of 3 (which is i + 1) and 3 (which is the
		// initial height).

		// ... and so on.

		int [] powers = new int[] {0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

		for (int i = 0; i < powers.length - 1; i++)
		{
			for (size = powers[i] + 1; size <= powers[i+1]; size++)
			{
				skiplist.insert(RNG.getUniqueRandomInteger());
				checkList(skiplist, size, height = Math.max(initHeight, i + 1));
			}
		}

		System.out.println("Hooray!");


		/*
		// Another way to write the above test case would be:

		for (size = 1; size <= 8; size++)
		{
			skiplist.add(uniqueRandomInteger());
			checkList(skiplist, size, height = 3);
		}

		for (size = 9; size <= 16; size++)
		{
			skiplist.add(uniqueRandomInteger());
			checkList(skiplist, size, height = 4);
		}

		for (size = 17; size <= 32; size++)
		{
			skiplist.add(uniqueRandomInteger());
			checkList(skiplist, size, height = 5);
		}

		for (size = 33; size <= 64; size++)
		{
			skiplist.add(uniqueRandomInteger());
			checkList(skiplist, size, height = 6);
		}

		// ... and so on.

		System.out.println("Hooray!");
		*/

	}
}

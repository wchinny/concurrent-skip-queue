// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase04.java
// =========================
// This test case inserts a lot of unique random elements and then checks that
// approximately half of them have height < 1 (which also means approximately
// half of them have height >= 1).
//
// In this test case, no duplicates are inserted into the skip list.


import java.io.*;
import java.util.*;

public class TestCase04
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
		// Probabilistically speaking, this test should succeed within 10 attempts.
		for (int i = 0; i < 10; i++)
		{
			RNG.clear();
			int numElements = 1000;

			// Insert random values into a skiplist.
			SkipList<Integer> skiplist = new SkipList<Integer>();
			for (int j = 0; j < numElements; j++)
				skiplist.insert(RNG.getUniqueRandomInteger());

			// Skim level 1 (the next-to-bottom level) and count the nodes there.
			Node<Integer> temp = skiplist.head().next(1);
			int count = 0;
			while (temp != null)
			{
				count++;
				temp = temp.next(1);
			}

			if (Math.abs(numElements / 2 - count) <= 10)
			{
				System.out.println("Hooray!");
				return;
			}
		}

		System.out.println("fail whale :(");
	}
}

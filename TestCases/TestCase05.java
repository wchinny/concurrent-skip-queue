// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase05.java
// =========================
// This test case inserts a lot of random elements and then checks that
// approximately half of them have height < 1 (which also means approximately
// half of them have height >= 1).
//
// This is identical to TestCase04, except that it does not ensure that the
// elements being inserted into the list are unique.


import java.io.*;
import java.util.*;

public class TestCase05
{
	public static void main(String [] args)
	{
		// Probabilistically speaking, this test should succeed within 10 attempts.
		for (int i = 0; i < 10; i++)
		{
			int numElements = 1000;
			int maxValue = 10000;

			// Insert random values into a skiplist.
			SkipList<Integer> skiplist = new SkipList<Integer>();
			for (int j = 0; j < numElements; j++)
				skiplist.insert((int)(Math.random() * maxValue));

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

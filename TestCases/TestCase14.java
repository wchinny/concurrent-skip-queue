// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase14.java
// =========================
// This test case manually checks the structure of a small skip list after
// performing a few insertions and deletions.


import java.io.*;
import java.util.*;

public class TestCase14
{
	private static boolean referenceCheck(SkipList<Integer> s, int level, int [] values)
	{
		Node<Integer> temp = s.head();

		for (int i = 0; i < values.length; i++)
		{
			temp = temp.next(level);
			if (temp.value().compareTo(values[i]) != 0)
				return false;
		}

		// One final check to ensure this is the end of the list on this level.
		if (temp.next(level) == null)
		{
			System.out.println("Reference check: PASS");
			return true;
		}

		System.out.println("Reference check: fail whale :(");
		return false;
	}

	// Check that the skiplist's size and height are as expected. If not, fail whale.
	private static boolean checkList(SkipList<Integer> s, int expectedSize, int expectedHeight)
	{
		boolean success = true;

		if (s.size() != expectedSize)
		{
			System.out.println("List size: fail whale :(");
			success = false;
		}

		if (s.height() != expectedHeight)
		{
			System.out.println("List height: fail whale :(");
			success = false;
		}

		return success;
	}

	public static void main(String [] args)
	{
		SkipList<Integer> s = new SkipList<Integer>(5);

		s.insert(10, 2);
		s.insert(15, 3);
		s.insert(20, 2);
		s.insert(25, 4);
		s.insert(25, 1);
		s.insert(30, 5);

		int level, size, height;
		boolean success = true;

		// Verify each level of the skip list is connected as indicated.
		success &= referenceCheck(s, level = 0, new int [] {10, 15, 20, 25, 25, 30});
		success &= referenceCheck(s, level = 1, new int [] {10, 15, 20, 25, 30});
		success &= referenceCheck(s, level = 2, new int [] {15, 25, 30});
		success &= referenceCheck(s, level = 3, new int [] {25, 30});
		success &= referenceCheck(s, level = 4, new int [] {30});
		success &= referenceCheck(s, level = 5, new int [] {});

		// Verify the heights of the nodes. This also helps test the order in
		// which duplicate values are inserted into the skip list.
		int [] newHeights = new int[] {5, 2, 3, 2, 1, 4, 5};
		Node<Integer> temp = s.head();
		for (int i = 0; i < newHeights.length; i++)
		{
			success &= (temp.height() == newHeights[i]);
			temp = temp.next(0);
		}

		// Delete 25. This should delete the first instance, which at the point
		// should be the one with height 1.
		s.delete(25);
		success &= checkList(s, size = 5, height = 3);

		// Verify each level of the skip list is connected as indicated.
		success &= referenceCheck(s, level = 0, new int [] {10, 15, 20, 25, 30});
		success &= referenceCheck(s, level = 1, new int [] {10, 15, 20, 25, 30});
		success &= referenceCheck(s, level = 2, new int [] {15, 25, 30});
		success &= referenceCheck(s, level = 3, new int [] {});
		success &= referenceCheck(s, level = 4, new int [] {});
		success &= referenceCheck(s, level = 5, new int [] {});

		// Verify the heights of the remaining nodes are as expected.
		newHeights = new int[] {3, 2, 3, 2, 3, 3};
		temp = s.head();
		for (int i = 0; i < newHeights.length; i++)
		{
			success &= (temp.height() == newHeights[i]);
			temp = temp.next(0);
		}

		System.out.println(success ? "Hooray!" : "fail whale :(");
	}
}

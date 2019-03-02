// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase01.java
// =========================
// This test case manually checks the structure of a small skip list.


import java.io.*;
import java.util.*;

public class TestCase01
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

	public static void main(String [] args)
	{
		SkipList<Integer> s = new SkipList<Integer>(3);

		s.insert(10, 1);
		s.insert(20, 1);
		s.insert(3, 2);
		s.insert(15, 1);
		s.insert(5, 1);

		// Verify each level of the skip list is connected as indicated.

		int level;
		boolean success = true;

		success &= referenceCheck(s, level = 0, new int [] {3, 5, 10, 15, 20});
		success &= referenceCheck(s, level = 1, new int [] {3});
		success &= referenceCheck(s, level = 2, new int [] {});
		success &= referenceCheck(s, level = 20, new int [] {});

		System.out.println(success ? "Hooray!" : "fail whale :(");
	}
}

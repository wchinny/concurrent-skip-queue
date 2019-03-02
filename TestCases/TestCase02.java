// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase02.java
// =========================
// This test case manually checks the structure of a small skip list.
//
// This is similar to TestCase01, except that the heights of the nodes being
// inserted are not manually specified in this test case.


import java.io.*;
import java.util.*;

public class TestCase02
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
		SkipList<Integer> s = new SkipList<Integer>();

		s.insert(10);
		s.insert(20);
		s.insert(3);
		s.insert(15);
		s.insert(5);

		// Verify the bottom level of the skip list is connected as indicated.
		// Because the heights of the nodes are not specified manually as the
		// values are being inserted, this is the only level of the skip list
		// whose structure can be predetermined (other than null references for
		// levels that exceed the height of the skip list).

		int level;
		boolean success = true;

		success &= referenceCheck(s, level = 0, new int [] {3, 5, 10, 15, 20});
		success &= referenceCheck(s, level = 4, new int [] {});
		success &= (s.size() == 5);
		success &= (s.height() == 3);

		System.out.println(success ? "Hooray!" : "fail whale :(");
	}
}

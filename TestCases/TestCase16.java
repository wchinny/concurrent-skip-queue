// Sean Szumlanski
// COP 3503, Fall 2017

// =========================
// SkipList: TestCase16.java
// =========================
// A brief test case to help ensure you've implemented the difficultyRating()
// and hoursSpent() methods correctly.


import java.io.*;
import java.util.*;

public class TestCase16
{
	private static void failwhale()
	{
		System.out.println("fail whale :(");
		System.exit(1);
	}

	public static void main(String [] args)
	{
		double difficulty = SkipList.difficultyRating();
		if (difficulty < 1.0 || difficulty > 5.0) failwhale();

		double hours = SkipList.hoursSpent();
		if (hours <= 0.0) failwhale();

		System.out.println("Hooray!");
	}
}

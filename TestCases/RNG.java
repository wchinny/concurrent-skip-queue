// Sean Szumlanski
// COP 3503, Fall 2017

// ==================
// SkipList: RNG.java
// ==================
// Random number generator support class for test cases. This beast keeps track
// of the random numbers it has generated and can ensure no duplicates are
// generated. It can also randomly select a previously generated integer.


import java.util.*;

class RNG
{
	// These are used to generate unique random integers.
	private static int maxRandomValue = Integer.MAX_VALUE;
	private static HashSet<Integer> usedIntegers = new HashSet<Integer>();

	// Returns a random integer on the specified range
	public static int getUniqueRandomInteger()
	{
		if (usedIntegers.size() == maxRandomValue)
		{
			System.out.println("Whoa, buck-oh! You've generated enough random integers for one day!");
			System.exit(1);
		}

		int r = (int)(Math.random() * maxRandomValue) + 1;

		while (usedIntegers.contains(r))
			r = (int)(Math.random() * maxRandomValue) + 1;

		usedIntegers.add(r);
		return r;
	}

	public static void setMaxRandomValue(int max)
	{
		maxRandomValue = max;
	}

	public static void clear()
	{
		usedIntegers.clear();
	}

	// Select at random an integer that has already been used.
	public static int getRandomUsedInteger()
	{
		if (usedIntegers.size() == 0)
			return -1;

		int i = 0, index = (int)(Math.random() * usedIntegers.size());

		for (int used : usedIntegers)
		{
			if (i == index)
				return used;
			++i;
		}

		// This line should be unreachable.
		return -1;
	}

	// Select at random an integer that has not already been used.
	public static int getRandomUnusedInteger()
	{
		int r = (int)(Math.random() * maxRandomValue) + 1;

		while (usedIntegers.contains(r))
			r = (int)(Math.random() * maxRandomValue) + 1;

		return r;
	}
}

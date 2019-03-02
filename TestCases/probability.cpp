#include <bits/stdc++.h>

using namespace std;

int main() {
	int count = 0;
	srand(time(NULL));
	for(int i = 0; i < 100; i++) {
		int num = rand() % 100 + 1;
		if(num < 50) {
			count++;
		}
	}
	cout << count << endl;
}
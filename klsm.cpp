#include <bits/stdc++.h>

using namespace std;

struct Item {
public:
	int key;
	char c;
	bool logical_deleted;
};

struct Block {
public:
	int filled;
	int level;

	Item** items = nullptr;

	Block(int l) {
		filled = 0;
		level = l;
		vector<Item*> item;
	}

	void append(Item* item) {
		if (!item->logical_deleted) {
			items[filled++] = item;
		}
	}

	Block* copy(int l) {
		Block* nb = new Block(l);
		for(int i = 0; i < filled; ++i) {
			nb->append(items[i]);
		}
		return nb;
	}

	void merge_in(Block* b1, Block* b2);

	Block* shrink() {
		int f = filled;

		while(f > 0 && !(items[f - 1]->logical_deleted)) {
			--f;
		}

		int l = level;

		while(l > 0 && f <= (int)pow(2.0, l-1)) {
			--l;
		}

		if(l < level) {
			return copy(l)->shrink();
		}

		filled = f;
		return this;
	}
};

int main() {
	Block *b = new Block(3);
}
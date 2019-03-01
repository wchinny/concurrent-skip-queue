#include <bits/stdc++.h>

using namespace std;

struct Item {
	int key;
	char c;
	bool logical_deleted;
};

struct Block {
	int filled;
	int level;

	Item** items = nullptr;

	Block(int l) {
		filled = 0;
		level = l;
		vector<Item*> items;
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

struct BlockArray {
	vector<Block*> blocks;
	vector<int> k_pivots;
	int size;

	void insert(Block* block);
	bool consolidate();
	void calculate_pivots();
	BlockArray* copy();

	Item* find_min() {

		srand(69);
		int total = 0;

		for(int i = 0; i < size; ++i) {
			total += blocks[i]->filled - k_pivots[i];
		}

		int r = rand() % (total);

		for(int i = 0; i < size; ++i) {

			int range = blocks[i]->filled - k_pivots[i];
			if(range <= r) {
				r -= range;
			}
			else {
				if(r != range - 1) {
					Item* item = blocks[i]->items[k_pivots[i] + r];
					if(!(item->logical_deleted)) {
						return item;
					}
				}
				return blocks[i]->items[blocks[i]->filled - 1];
			}

		}

	}

	void insert(Block *block) {
		blocks.push_back(block);
	}

};

int main() {
	Block *b = new Block(3);
}
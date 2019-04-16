#include "LockFreeSkiplist.h"

int randomLevel(int _maxHeight) {
	int selected = (int)((rand() % _maxHeight)); //Not rand() % _maxHeight + 1 "Array index out of bounds!" 
	return selected;
}

bool SharedSkipList::add(int v) {

	int topLayer = randomLevel(MaxHeight);

	SharedNode** preds = new SharedNode*[MaxHeight];
	SharedNode** succs = new SharedNode*[MaxHeight];

	int highestLocked = -1;

	while(true) {
		int lFound = findNode(v, preds, succs);
		
		if(lFound != -1) {
			SharedNode* nodeFound = succs[lFound];
			
			//Wait until the node is fully linked. Should not be an issue for sequential!
			if(!nodeFound->marked) {
				while(!nodeFound->fullyLinked) {
					;
				}

				return false;
			}
			
			continue;
		}

		highestLocked = -1;
		
		SharedNode *pred, *succ, *prevPred = NULL;
		bool valid = true;

		for(int layer = 0; valid && (layer <= topLayer); layer++) {
			pred = preds[layer];
			succ = succs[layer];
			
			if(pred != prevPred) {
				pred->lock.lock();
				highestLocked = layer;
				prevPred = pred;
			}

			valid = !pred->marked && !succ->marked && pred->nexts[layer] == succ;
		}

		if(!valid){

			for(int i = 0; i < MaxHeight; i++) {
				preds[i]->lock.unlock();
			}

			continue;
		} 

		SharedNode* newNode = new SharedNode(v, topLayer);

		for(int layer = 0; layer <= topLayer; layer++) 
		{
			newNode->nexts[layer] = succs[layer];
			preds[layer]->nexts[layer] = newNode;
		}

		newNode->fullyLinked = true;

		for(int i = 0; i < MaxHeight; i++) {
			preds[i]->lock.unlock();
		}
	
		return true;
	}

}

int SharedSkipList::findNode(int v, SharedNode* preds [], SharedNode* succs []) {

	int lFound = -1;
	SharedNode* pred = LSentinel;

	for(int layer = MaxHeight - 1; layer >= 0; layer--) {
		SharedNode* curr = pred->nexts[layer];
		while(v > curr->key) {
			pred = curr;
			curr = pred->nexts[layer];
		}
		if(lFound == -1 && v == curr->key) {
			lFound = layer;
		}
		preds[layer] = pred;
		succs[layer] = curr;
	}
	return lFound;
}

bool SharedSkipList::remove(int v) {

	SharedNode* nodeToDelete = NULL;
	bool isMarked = false;
	int topLayer = -1;

	SharedNode** preds = new SharedNode*[MaxHeight];
	SharedNode** succs = new SharedNode*[MaxHeight];
	
	while(true) {
		int lFound = findNode(v, preds, succs);
	
		if(isMarked || (lFound != -1 && okToDelete(succs[lFound], lFound))) {
			if(!isMarked) {
				nodeToDelete = succs[lFound];
				topLayer = nodeToDelete->topLayer;
				nodeToDelete->lock.lock();

				if(nodeToDelete->marked) {
					nodeToDelete->lock.unlock();
					return false;
				}

				nodeToDelete->marked = true;
				isMarked = true;
			}

			int highestLocked = -1;
				
			SharedNode *pred, *succ, *prevPred = NULL;
			bool valid = true;

			for(int layer = 0; valid && (layer <= topLayer); layer++) {
				pred = preds[layer];
				succ = succs[layer];

				if(pred != prevPred) {
					pred->lock.lock();
					highestLocked = layer;
					prevPred = pred;
				}
				valid = !pred->marked && pred->nexts[layer] == succ;
			}

			if(!valid) {
				// for(int i = MaxHeight-1; i >= highestLocked; i--) 
				// 	preds[i]->lock.unlock();
				for(int i = 0; i < MaxHeight; i++) {
					preds[i]->lock.unlock();
				}
				
				continue;
			}

			for(int layer = topLayer; layer >= 0; layer--) 
				preds[layer]->nexts[layer] = nodeToDelete->nexts[layer];
			
			nodeToDelete->lock.unlock();

			// for(int i = MaxHeight-1; i >= highestLocked; i--) //Check if this is needed here!
			// 	preds[i]->lock.unlock();
			for(int i = 0; i < MaxHeight; i++) {
					preds[i]->lock.unlock();
			}
			
			return true;
		}

		else return false;
	}
}

bool SharedSkipList::okToDelete(SharedNode* candidate, int lFound) {
	return (candidate->fullyLinked && candidate->topLayer == lFound && !candidate->marked);
}

bool SharedSkipList::contains(int v) {
	SharedNode** preds = new SharedNode*[MaxHeight];
	SharedNode** succs = new SharedNode*[MaxHeight];
	int lFound = SharedSkipList::findNode(v, preds, succs);
	return(lFound != -1 && succs[lFound]->fullyLinked && !succs[lFound]->marked);
}

void SharedSkipList::print() {

    ofstream myfile;
    
    myfile.open("outputLockFreeSkipList.txt");

    int curr_level = LSentinel->topLayer;

    myfile << "Sentinel Node Height: " << curr_level << endl;

    for(int i = curr_level - 1; i >= 0; i--) {
        SharedNode *curr = LSentinel->nexts[i];

        while(curr != NULL) {
			if(curr->key != INT_MAX - 1)
				myfile << curr->key << "->";
			else
				myfile << "+∞" << "->";	//Updated with actual infinity symbol :)		

            curr = curr->nexts[i];
        }
        myfile << endl;
    }
    myfile.close(); 

}

// void batch_add(SkipList* s)
// {

// 	for(int i = 0; i < 10000; i++)
// 	{
// 		s->add(i);
// 	}
// }

// int main() 
// {
// 	SkipList* s = new SkipList(20);

// 	vector<thread> v;
// 	int c = 0;

// 	for(int i = 1; i < 4; i++)
// 	{
// 		v.emplace_back(thread(batch_add, s));
// 	}

// 	for(auto &t : v)
// 	{
// 		t.join();
// 	}

// 	s->print();

// 	return 0;

// }
#include <iostream>
#include <fstream>
#include <cmath>
#include <queue>

struct Node {
	int keys[11];
	struct Node *child[11];
	struct Node *father;
	bool is_leaf;
	int N;
	friend class Tree;
	struct Node(int degree, struct Node *parent, bool is_leaf) {

		for (int i = 0; i < degree; i++) {
			this->keys[i] = -1;
			this->child[i] = nullptr;
		}
		child[degree] = nullptr;
		this->is_leaf = is_leaf;
		this->father = parent;
		this->N = 0;
	}
};

class Tree {

public:
	Tree();
	Tree(const int& degree1);
	~Tree();
	struct Node* LeftBro(struct Node *node, int val = -1);
	struct Node* RightBro(struct Node *node, int val = -1);
	struct Node* SearchKey1(int val, int *position);
	void OutputTree();
	bool SearchKey(int val);
	bool TransformTree();
	void UpdateHeight();
	void UpdateNumberOfKeys();
	void UpdateIsLeaf(const bool& exp, const int& ord);
	bool InsertKey(int val);
	bool DeleteKey(int val);
	void SplitRoot(struct Node *node1, struct Node *node2, int value, int position);
	void Split(struct Node *pom, int value, int position);
	bool GetIsLeaf(const int& ord) const;
	int GetMinKeys() const;
	int GetDegree() const;
	int GetHeight() const;
	int GetNumberOfKeys() const;
	void ShiftArraysRight(int start_pos, struct Node* pom);
	void ShiftArraysRightRoot(int start_pos);
	int FindFatherKey(struct Node *pom);
	void AsignSplitNodes(int split, struct Node* node1, struct Node* node2, struct Node* main_node);
	struct Node * root;
	struct Node *FindSuccessor(struct Node *node, int position);
	void DeleteFromLeaf(struct Node *leaf, int position, struct Node *left, struct Node *right);
	void LeftShiftArrays(struct Node *node, int position);
	void MergeRight(struct Node *node, struct Node* right, int val);
	int degree;
	int height;
	int num_keys;
	int min_keys;
	int split_pos;
};

Tree::Tree() {

}

Tree::Tree(const int& degree1) {
	this->degree = degree1;
	this->min_keys = ceil(degree1 / 2.0) - 1;
	this->split_pos = (degree1 - 1) / 2;
	this->height = 0;
	this->num_keys = 0;
	this->root = nullptr;
}

Tree::~Tree() {

}

void Tree::UpdateHeight() {
	this->height++;
}

void Tree::UpdateNumberOfKeys() {
	this->num_keys++;
}

void Tree::UpdateIsLeaf(const bool& exp, const int& ord) {
	this->root->is_leaf = exp;
}

bool Tree::GetIsLeaf(const int& ord) const {
	return this->root->is_leaf;
}

void Tree::ShiftArraysRightRoot(int start_pos) {
	if (this->root->child[0] != nullptr) {
		for (int i = this->root->N + 1; i > start_pos; i--) {
			this->root->child[i] = this->root->child[i - 1];
		}
	}
	for (int i = this->root->N; i > start_pos; i--) {
		this->root->keys[i] = this->root->keys[i - 1];
	}
}

void Tree::ShiftArraysRight(int start_pos, struct Node* pom) {
	for (int i = pom->N + 1; i > start_pos; i--) {
		pom->child[i] = pom->child[i - 1];
	}
	for (int i = pom->N; i > start_pos; i--) {
		pom->keys[i] = pom->keys[i - 1];
	}
}

int Tree::FindFatherKey(struct Node* pom) {
	struct Node *parent = pom->father;
	int i = 0;
	while (pom->keys[0] > parent->keys[i] && i < parent->N) {
		i++;
	}
	if (pom->keys[0] > parent->keys[parent->N - 1]) return -parent->keys[i - 1];
	return parent->keys[i];
}

void Tree::AsignSplitNodes(int split, struct Node* node1, struct Node* node2, struct Node* main_node) {

	// Asign keys left from the split pos to left son
	for (int i = 0; i < split; i++) {
		node1->keys[i] = main_node->keys[i];
	}
	node1->N = (main_node->N - 1) / 2;

	//Asign pointers left from the split pos to left son
	for (int i = 0; i < split + 1; i++) {
		node1->child[i] = main_node->child[i];
		//node1->child[i]->father = main_node;
	}

	//Asign keys right from the split pos to right son
	for (int i = split + 1; i < main_node->N; i++) {
		node2->keys[i - (split + 1)] = main_node->keys[i];
	}
	node2->N = (main_node->N) / 2;

	//Asign pointers right from the split post to right son
	for (int i = split + 1; i < main_node->N + 1; i++) {
		node2->child[i - (split + 1)] = main_node->child[i];
		//node2->child[i - (split + 1)] = main_node;
	}

}

void Tree::SplitRoot(struct Node *help1, struct Node* help2, int value, int position) {

	//1. CASE - Is root node a leaf?
	if (this->root->is_leaf) {
		//INSERT KEY IN A ROOT NODE
		ShiftArraysRightRoot(position);
		this->root->keys[position] = value;
		this->root->N++;

		int split = this->split_pos;

		this->root->child[position + 1] = nullptr;
		struct Node * node1 = new struct Node(degree, this->root, true);
		struct Node * node2 = new struct Node(degree, this->root, true);
		int split_node = this->root->keys[split];

		AsignSplitNodes(split, node1, node2, this->root);

		//Asign root to point to node1 and node2
		struct Node *help_node = new struct Node(degree, nullptr, false);
		help_node->keys[0] = split_node;
		help_node->child[0] = node1;
		help_node->child[1] = node2;
		help_node->N = 1;
		delete this->root;
		this->root = help_node;
		node1->father = this->root;
		node2->father = this->root;
	}

	//2. CASE - ROOT NODE IS NOT A LEAF
	else {
		//this->root
		int split = this->split_pos;

		struct Node * node1 = new struct Node(degree, this->root, false);
		struct Node * node2 = new struct Node(degree, this->root, false);
		int split_node = this->root->keys[split];

		AsignSplitNodes(split, node1, node2, this->root);

		//Asign root to point to node1 and node2
		struct Node *help_node = new struct Node(degree, nullptr, false);
		help_node->keys[0] = split_node;
		help_node->child[0] = node1;
		help_node->child[1] = node2;
		help_node->N = 1;
		delete this->root;
		this->root = help_node;
		node1->father = node2->father = this->root;
		for (int i = 0; i < node1->N + 1; i++) {
			node1->child[i]->father = node1;
		}
		for (int i = 0; i < node2->N + 1; i++) {
			node2->child[i]->father = node2;
		}
	}
	this->UpdateHeight();
}

void Tree::Split(struct Node *pom, int value, int position) {

	while (true) {
		if (pom != root) {

			//1. CASE - POM IS A LEAF AND HE IS NOT A ROOT
			if (pom->is_leaf) {
				struct Node * parent = pom->father;

				//Shift all keys and pointers one position right to free the position for insertion
				ShiftArraysRight(position, pom);

				pom->child[position + 1] = nullptr;
				pom->keys[position] = value;
				pom->N++;

				int split = this->split_pos;
				int split_node = pom->keys[split];

				struct Node * node1 = new struct Node(degree, pom, pom->is_leaf);
				struct Node * node2 = new struct Node(degree, pom, pom->is_leaf);
				AsignSplitNodes(split, node1, node2, pom);

				int i = 0;
				while (split_node > parent->keys[i] && i < parent->N) {
					i++;
				}

				// If father of the leaf is root then we insert in root
				if (parent == root) {
					ShiftArraysRight(i, root);
					root->child[i + 1] = node2;
					root->child[i] = node1;
					root->keys[i] = split_node;
					root->N++;
					node1->father = node2->father = this->root;
					delete pom;
					pom = root;
				}

				// If parent of the leaf is not root then we just insert in parent
				else {
					ShiftArraysRight(i, parent);
					parent->child[i + 1] = node2;
					parent->child[i] = node1;
					parent->keys[i] = split_node;
					parent->N++;
					node1->father = node2->father = parent;
					delete pom;
					pom = parent;
				}
			}

			//Here we have already inserted value in the node and i just split it and insert split val in parent
			else {

				// Here we set value that will be sent up in the parent
				int split = this->split_pos;
				int split_node = pom->keys[split];

				struct Node *parent = pom->father;

				struct Node *node1 = new struct Node(degree, pom->father, false);
				struct Node *node2 = new struct Node(degree, pom->father, false);
				AsignSplitNodes(split, node1, node2, pom);

				int i = 0;
				while (split_node > parent->keys[i] && i < parent->N) {
					i++;
				}

				// If father of the leaf is root then we insert in root
				if (parent == root) {
					ShiftArraysRight(i, root);
					root->child[i + 1] = node2;
					root->child[i] = node1;
					root->keys[i] = split_node;
					root->N++;
					node1->father = node2->father = this->root;
					for (int i = 0; i < node1->N + 1; i++) {
						node1->child[i]->father = node1;
					}
					for (int i = 0; i < node2->N + 1; i++) {
						node2->child[i]->father = node2;
					}
					delete pom;
					pom = root;
				}

				// If parent of the leaf is not root then we just insert in parent
				else {
					ShiftArraysRight(i, parent);
					parent->child[i + 1] = node2;
					parent->child[i] = node1;
					parent->keys[i] = split_node;
					parent->N++;
					node1->father = node2->father = parent;
					for (int i = 0; i < node1->N + 1; i++) {
						node1->child[i]->father = node1;
					}
					for (int i = 0; i < node2->N + 1; i++) {
						node2->child[i]->father = node2;
					}
					delete pom;
					pom = parent;
				}
			}
		}

		//If we are in the root
		else {
			SplitRoot(nullptr, nullptr, 0, 0);
			pom = root;
		}
		if (pom->N != this->degree) {
			break;
		}
	}
}

bool Tree::InsertKey(int val) {
	bool OK = true;
	int ord = 0;
	bool found = false;
	bool found1 = false;
	if (this->root == nullptr) {
		root = new struct Node(this->degree, nullptr, true);
		root->keys[0] = val;
		root->N = 1;
	}
	else {
		struct Node * father = nullptr;
		Tree * copy = this;
		struct Node * pom = copy->root;
		while (!found) {

			while (val > pom->keys[ord] && ord < pom->N) {
				ord++;
			}

			//1. CASE - DID WE FOUND THAT KEY?
			if (val == pom->keys[ord]) {
				found1 = true;
				found = true;
			}

			//2. BIG CASE - IS IT A LEAF?
			else if (pom->is_leaf) {
				int cnt = 0;

				//2.1 CASE - IS IT NOT A FULL NODE?
				if (pom->N < degree - 1) {

					//2.1.1 CASE - IS HIS POSITION THE RIGHT MOST?
					if (pom->N == ord) {
						pom->keys[ord] = val;
						pom->child[ord] = nullptr;
						pom->child[ord + 1] = nullptr;
						pom->father = father;
						pom->N++;
						found = true;
					}

					//2.1.2 CASE - IS HIS POSITION NOT THE RIGHT MOST? -> MOVE KEYS
					else {
						for (int i = pom->N; i > ord; i--)
							pom->keys[i] = pom->keys[i - 1];
						pom->keys[ord] = val;
						found = true;
						pom->N++;
					}
				}

				//2.2 CASE - IS IT A FULL NODE?
				else {

					//2.2.1 CASE - IS IT A ROOT NODE?
					if (pom == this->root) {

						SplitRoot(nullptr, nullptr, val, ord);
						found = true;
					}

					//2.2.2 CASE - IS IT NOT A ROOT NODE?
					else {
						Split(pom, val, ord);
						found = true;
					}
				}
			}

			//3. CASE - IS IT NOT A LEAF NODE?
			else {
				father = pom;
				pom = pom->child[ord];
			}
			ord = 0;
		}
	}


	if (found1) {
		OK = false;
	}
	else {
		this->UpdateNumberOfKeys();
	}
	return OK;
}

void Tree::OutputTree() {
	std::queue <struct Node*> queue;
	struct Node* help = this->root;
	int prev = 1;
	int curr = 0;
	int level = 0;
	int key;
	queue.push(help);
	while (!queue.empty()) {
		std::cout << "Level " << level << ": ";

		//This while is for one level
		while (prev != 0) {
			help = queue.front();
			queue.pop();

			//Finding a key that is father of current node only if it's not zero level
			if (level != 0 && help != nullptr) {
				key = FindFatherKey(help);
				if (key < 0) std::cout << "( " << -key << " right son):";
				else std::cout << "(" << key << "): ";
			}

			//Here i am printing keys of current node
			for (int i = 0; i < help->N; i++) {
				std::cout << help->keys[i] << " ";
			}

			//Here i am pushing on queue those nodes who have point to something
			for (int i = 0; i < help->N + 1; i++) {
				if (help->child[i] != nullptr) {
					queue.push(help->child[i]);

					//Updating how many nodes are in next level
					curr++;
				}
			}
			std::cout << "| ";

			// Trying to count how many
			prev--;
		}
		std::cout << std::endl;

		//Previous is now current
		prev = curr;

		//Current is again 0
		curr = 0;

		//Updating level
		level++;
	}
}

bool Tree::SearchKey(int val) {
	struct Node * node = this->root;
	bool found = false;
	while (!found && node != nullptr) {
		int i = 0;
		while (val > node->keys[i] && i < node->N)
			i++;
		if (node->keys[i] == val) {
			found = true;
		}
		else {
			node = node->child[i];
		}
	}
	return found;
}

struct Node* Tree::SearchKey1(int val, int *position) {
	struct Node * node = this->root;
	bool found = false;
	int i = 0;
	while (!found) {
		while (val > node->keys[i] && i < node->N)
			i++;
		if (node->keys[i] == val) {
			found = true;
		}
		else {
			node = node->child[i];
		}
		i = 0;
	}
	*position = i;
	return node;
}

struct Node* Tree::LeftBro(struct Node *node, int val) {
	struct Node *parent = node->father;
	int value = node->keys[0];
	if (val != -1) value = val;
	int i = 0;
	while (value > parent->keys[i] && i < parent->N)
		i++;

	//There is no left bro
	if (i == 0)
		return nullptr;

	//Child[i] is node that is parameter of this function
	return parent->child[i - 1];
}

struct Node* Tree::RightBro(struct Node *node, int val) {
	struct Node *parent = node->father;
	int value = node->keys[0];
	if (val != -1) value = val;
	int i = 0;
	while (value > parent->keys[i] && i < parent->N)
		i++;

	//There is no right bro
	if (i == parent->N)
		return nullptr;

	//Child[i] is node that is parameter of this function
	return parent->child[i + 1];
}

struct Node* Tree::FindSuccessor(struct Node *node, int position) {
	struct Node *succ = node;
	succ = succ->child[position + 1];
	while (succ->child[0] != nullptr) {
		succ = succ->child[0];
	}
	return succ;
}

void Tree::LeftShiftArrays(struct Node *node, int position) {
	for (int i = position; i < node->N; i++) {
		node->keys[i] = node->keys[i + 1];
	}
	for (int i = position + 1; i < node->N + 1; i++) {
		node->child[i] = node->child[i + 1];
	}
	node->N--;
}

void Tree::MergeRight(struct Node *node, struct Node* right, int val) {
	struct Node * parent = node->father;
	int i = 0;
	while (val > parent->keys[i] && parent->N > i)
		i++;
	val = parent->keys[i];
	int before = node->N;
	//Update number of keys in node(1 from parent and N from right bro)
	node->N += (1 + right->N);

	//Take keys and pointers from parent and right bro
	node->keys[before] = parent->keys[i];
	for (int i = before + 1; i < node->N; i++) {
		node->keys[i] = right->keys[i - (before + 1)];
	}
	for (int i = before + 1; i < node->N + 1; i++) {
		node->child[i] = right->child[i - (before + 1)];
		if (node->child[i] != nullptr)
			node->child[i]->father = node;
	}
	for (int i = 0; i < before + 1; i++) {
		if (node->child[i] != nullptr)
			node->child[i]->father = node;
	}

	LeftShiftArrays(parent, i);
	parent->child[i] = node;
	delete right;
	if (parent->N == 0 && parent == this->root) {
		delete parent;
		node->father = nullptr;
		this->root = node;
		this->height--;
	}
	node->father = parent;
	node = node->father;

	while (node) {
		if (node->N >= this->min_keys)
			break;
		right = RightBro(node, val);
		struct Node *left = LeftBro(node, val);
		if (right) {
			//BORROWING FROM RIGHT BROTHER
			if (right->N - 1 >= this->min_keys) {
				parent = node->father;
				int i = 0;
				while (val > parent->keys[i] && i < parent->N)
					i++;
				node->N++;
				node->keys[node->N - 1] = parent->keys[i];
				node->child[node->N] = right->child[0];
				for (int i = 0; i < node->N + 1; i++) {
					if (node->child[i] != nullptr)
						node->child[i]->father = node;
				}
				parent->keys[i] = right->keys[0];
				LeftShiftArrays(right, 0);
				parent->child[i] = node;
				parent->child[i + 1] = right;
				node->father = right->father = parent;
			}

			//BORROWING FROM LEFT BROTHER
			else if (left->N - 1 >= this->min_keys){
				parent = node->father;
				int i = 0;
				while (val > parent->keys[i] && i < parent->N)
					i++;
				i--;
				node->N++;
				//In deletation node we take from parent and then putting in the parent key from left bro
				ShiftArraysRight(0, node);
				node->keys[0] = parent->keys[i];
				node->child[0] = left->child[left->N];
				for (int i = 0; i < node->N + 1; i++) {
					if (node->child[i] != nullptr)
						node->child[i]->father = node;
				}
				parent->keys[i] = left->keys[left->N - 1];
				//Taking biggest key in left bro
				LeftShiftArrays(left, left->N - 1);
				parent->child[i] = node;
				parent->child[i - 1] = left;
				node->father = left->father = parent;
			}

			//RIGHT MERGE
			else {

			}
		}
		else {

			//BORROWING FROM LEFT BROTHER
			if (left->N - 1 >= this->min_keys) {
				parent = node->father;
				int i = 0;
				while (val > parent->keys[i] && i < parent->N)
					i++;
				i--;
				node->N++;
				//In deletation node we take from parent and then putting in the parent key from left bro
				ShiftArraysRight(0, node);
				node->keys[0] = parent->keys[i];
				node->child[0] = left->child[left->N];
				for (int i = 0; i < node->N + 1; i++) {
					if (node->child[i] != nullptr)
						node->child[i]->father = node;
				}
				parent->keys[i] = left->keys[left->N - 1];
				//Taking biggest key in left bro
				LeftShiftArrays(left, left->N - 1);
			}

			//LEFT MERGE
			else {

			}
		}
		node = node->father;
	}
}

void Tree::DeleteFromLeaf(struct Node *leaf, int position, struct Node *left, struct Node *right) {

	//IF THERE IS ENOUGH KEYS LEFT AFTER DELETATION
	if (leaf->N - 1 >= this->min_keys) {
		LeftShiftArrays(leaf, position);
	}

	//IF HE NEEDS TO BORROW OR MERGE
	else {
		struct Node *par = leaf->father;
		//HE HAS RIGHT BRO
		if (right) {

			//HE CAN BORROW FROM RIGHT BROTHER
			if (right->N - 1 >= this->min_keys) {
				struct Node *parent = leaf->father;
				int i = 0;
				while (leaf->keys[position] > parent->keys[i] && i < parent->N)
					i++;

				//In deletation node we take from parent and then putting in the parent key from left bro
				leaf->keys[position] = parent->keys[i];
				parent->keys[i] = right->keys[0];

				//We need to delete first key in right bro
				LeftShiftArrays(right, 0);
			}

			//HE CAN'T BORROW FROM RIGHT BRO
			else {

				//HE CAN'T BORROW FROM RIGHT BROTHER AND HE HAS NOT LEFT BRO OR CAN'T BORROW FROM HIM----> MERGEEE WITH RIGHT BRO
				if (!left || left->N - 1 < this->min_keys) {
					int val = leaf->keys[position];
					LeftShiftArrays(leaf, position);
					MergeRight(leaf, right, val);
				}

				//HE CAN BORROW FROM LEFT BRO 
				else if (left->N - 1 >= this->min_keys) {
					struct Node *parent = leaf->father;
					int i = 0;
					while (leaf->keys[position] > parent->keys[i] && i < parent->N)
						i++;
					i--;

					//In deletation node we take from parent and then putting in the parent key from left bro
					leaf->keys[position] = parent->keys[i];
					parent->keys[i] = left->keys[left->N - 1];
					//Taking biggest key in left bro
					LeftShiftArrays(left, left->N - 1);
				}
			}
		}

		//HE HAS NO RIGHT BROTHER
		else {

			//CAN BORROW FROM LEFT BRO
			if (left->N - 1 >= this->min_keys) {
				struct Node *parent = leaf->father;
				int i = 0;
				while (leaf->keys[position] > parent->keys[i] && i < parent->N)
					i++;
				i--;

				//In deletation node we take from parent and then putting in the parent key from left bro
				leaf->keys[position] = parent->keys[i];
				parent->keys[i] = left->keys[left->N - 1];
				//Taking biggest key in left bro
				LeftShiftArrays(left, left->N - 1);
			}

			//MERGE WITH LEFT BRO
			else {
				int val = leaf->keys[position];
				LeftShiftArrays(leaf, position);
				MergeRight(left, leaf, val);
			}
		}

	}
}

bool Tree::DeleteKey(int val) {
	bool OK = SearchKey(val);
	if (OK) {
		int position;
		struct Node *node = SearchKey1(val, &position);
		struct Node *help = node;
		int succ_pos;

		//1. CASE - WE ARE IN INTERNAL NODE AND BORROWING FROM LEAF
		if (!node->is_leaf) {
			struct Node *succ = FindSuccessor(node, position);

			//succ key is located on the first position in node
			succ_pos = 0;

			//simple replacement
			node->keys[position] = succ->keys[succ_pos];

			//now succ_pos is my deletation position and deletation node is help
			position = succ_pos;
			help = succ;
		}

		//Just in case we need them
		struct Node *right = RightBro(help);
		struct Node *left = LeftBro(help);

		//I surely delete key from the leaf
		DeleteFromLeaf(help, position, left, right);


	}

	return OK;
}

int Tree::GetMinKeys() const {
	return this->min_keys;
}

int Tree::GetDegree() const {
	return this->degree;
}

int Tree::GetHeight() const {
	return this->height;
}

int Tree::GetNumberOfKeys() const {
	return this->num_keys;
}

bool Tree::TransformTree() {

	return false;
}



void MakeMenu() {
	std::cout << "Choose one option(by entering a number):" << std::endl << "1. Make a tree by entering degree of tree:" << std::endl <<
		"2. Get height and number of keys in a given tree:" << std::endl << "3. Find a key in a given tree:" << std::endl << "4. Output the tree:" << std::endl
		<< "5. Insert a key in a given tree:" << std::endl << "6. Delete a key from a given tree:" << std::endl << "Transform the tree:" << std::endl <<
		"8. Delete tree object" << std::endl <<"9. Exit program" << std::endl;
}

int main() {

	Tree * tree = nullptr;
	unsigned option;
	MakeMenu();
	std::cin >> option;
	while (option != 9) {
		
		//MAKE A TREE BY ENTERING A DEGREE
		if (option == 1) {
			if (!tree) {
				std::cout << "Enter degree of B tree:" << std::endl;
				int tree_degree;
				std::cin >> tree_degree;
				Tree* copy = new Tree(tree_degree);
				tree = copy;
			}
			else {
				std::cout << "Tree is already created" << std::endl;
			}
		}

		//GET HEIGHT AND NUMBER OF KEYS IN A TREE
		else if (option == 2) {
			if (tree)
				std::cout << "Height of the tree is " << tree->GetHeight() << " and it has " << tree->GetNumberOfKeys() << " keys." << std::endl;
			else
				std::cout << "Tree is not created." << std::endl;
		}

		//FIND A KEY IN A GIVEN TREE
		else if (option == 3) {
			if (tree) {
				int key;
				std::cout << "Enter key that you are searching for:" << std::endl;
				std::cin >> key;
				if (tree->SearchKey(key))
					std::cout << "Key was found." << std::endl;
				else
					std::cout << "Key was not found." << std::endl;
			}
			else
				std::cout << "Tree is not created." << std::endl;
		}

		//OUTPUT THE TREE
		else if (option == 4) {
			if (tree) {
				tree->OutputTree();
			}
			else 
				std::cout << "Tree is not created." << std::endl;
		}

		//INSERT A KEY
		else if (option == 5) {
			std::cout << "Enter a value of key that you want to insert:" << std::endl;
			int key;
			std::cin >> key;
			if (tree->InsertKey(key)) {
				std::cout << "Insertion was successful." << std::endl;
			}
			else {
				std::cout << "Insertion was unsuccessful." << std::endl;
			}
		}

		//DELETE A KEY
		else if (option == 6) {
			std::cout << "Enter a value of key that you want to delete:" << std::endl;
			int key;
			std::cin >> key;
			if (tree->DeleteKey(key)) {
				std::cout << "Deletation was successful." << std::endl;
			}
			else {
				std::cout << "Deletation was unsuccessful." << std::endl;
			}
		}
		
		//TRANSFORM A TREE
		else if (option == 7) {
			if (tree->TransformTree()) {
				std::cout << "Transformation was successful." << std::endl;
			}
			else {
				std::cout << "Transformation was unsuccessful." << std::endl;
			}
		}

		//DELETE A TREE OBJECT
		else if (option == 8) {
			if (tree) {
				tree->~Tree();
			}
			else {
				std::cout << "Tree is not created." << std::endl;
			}
		}

		else if (option > 8 && option < 1) {
			std::cout << "You have choosen option that doesn't exist." << std::endl;
		}
		MakeMenu();
		std::cin >> option;
	}


	return 0;
}
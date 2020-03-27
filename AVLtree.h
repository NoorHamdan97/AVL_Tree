
#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <iostream>
#include <cstdio>
#include <iostream>
#include <cstdlib>

typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;

template<class T>
class TNode {
public:
	T data;
	int key;
	int BF;
	int height;
	TNode *right;
	TNode *left;
	TNode *father;
	TNode(T &data = NULL,int k=0) :
	data(data),key(k), BF(0), height(0), right(NULL), left(NULL), father(NULL) {
	}
	~TNode()=default;
	int getBF() {return BF;}

	T &getData() {return data;}

	int getHeight() {return height;}

	void calcHeight() {
		height = 1
				+ (getNodeHeight(right) > getNodeHeight(left) ?
						getNodeHeight(right) : getNodeHeight(left));
	}

	void calcBF() {
		BF = getNodeHeight(left) - getNodeHeight(right);
	}
};

template<class T>
int getNodeHeight(TNode<T> *node) {
	if (!node) {
		return -1;
	}
	return node->getHeight();
}

template<class T>
int getNodeBF(TNode<T> *node) {
	if (!node) {
		return 0;
	}
	return node->getBF();
}

/////////////////////////////////


template<class T>
class Tree {

	TNode<T> *root;
	int size;
public:
	Tree(TNode<T> *root = NULL) :
			root(root), size(0) {
	}

	~Tree() {
		freeNodes((this->root));
	}

	void freeNodes(TNode<T> *&r) {
		if (r == NULL)
			return;
		freeNodes(r->left);
		freeNodes(r->right);
		delete r;
		r = NULL;
		return;
	}

	///////////////////////////////
	
	int getSize() {
		return size;
	}

	///////////////////////////////
	
	TNode<T> *getRoot() {
		return root;
	}

	///////////////////////////////

	int max(int a, int b) {
		if (a > b)
			return a;
		else
			return b;
	}
	void updateBF(TNode<T>* node) {
		int R, L;
		if (!node->right) {
			R = -1;
		} else {
			R = node->right->height;
		}
		if (!node->left) {
			L = -1;
		} else {
			L = node->left->height;
		}
		node->BF = L - R;

	}

	void updateHeightAux(TNode<T>* node) {
		if (node == NULL)
			return;
		int rightH, leftH;
		if (node->right != NULL) {
			rightH = node->right->height;
		} else {
			rightH = -1;
		}
		if (node->left != NULL) {
			leftH = node->left->height;
		} else {
			leftH = -1;
		}
		node->height = max(rightH, leftH) + 1;
		updateBF(node);

	}
	void updateHeight(TNode<T>* node) {
		if (node == NULL)
			return;
		updateHeightAux(node);
		if (node->BF > 1 || node->BF < -1)
			checkRotate(node);
		updateHeight(node->father);
	}
	void checkRotate(TNode<T>* curr) {
		if (!curr)
			return;
		int pBF = curr->getBF();
		int rBF = getNodeBF(curr->right);
		int lBF = getNodeBF(curr->left);

		if (pBF == 2 && lBF >= 0)
			LL(curr);
		else if (pBF == 2 && lBF == -1)
			LR(curr);
		else if (pBF == -2 && rBF <= 0)
			RR(curr);
		else if (pBF == -2 && rBF == 1)
			RL(curr);
	}

	////////////////////////////////////////////
	StatusType Find(int key, T* value){
		if (value==NULL){return INVALID_INPUT;}
		TNode<T>* tmp =find(key);
		if (tmp==NULL){
			return FAILURE;
		}
		*value=tmp->data;
		return SUCCESS;
	}




	TNode<T> *find(int k) {
		TNode<T>* temp = root;
		while (temp) {
			if (temp->key < k) {
				if (temp->right == NULL) {
					return NULL;
				}
				temp = temp->right;
				continue;
			}
			if (temp->key > k) {
				if (temp->left == NULL) {
					return NULL;
				}
				temp = temp->left;
				continue;
			}
			return temp;
		}
		return NULL;
	}

	TNode<T> *FindInInsert(int k) {
		TNode<T>* temp = root;
		while (temp) {
			if (temp->key < k) {
				if (temp->right == NULL) {
					return temp;
				}
				temp = temp->right;
				continue;
			}
			if (temp->key > k) {
				if (temp->left == NULL) {
					return temp;
				}
				temp = temp->left;
				continue;
			}
			return temp;
		}
		return NULL;
	}

	///////////////////////////////

	StatusType Add (int key, T* value, TNode<T>* pnode){
		if (pnode==NULL){return INVALID_INPUT;}
		T* tmp= value;
		if (this->Find(key,tmp)==SUCCESS){return FAILURE;}
		this->insert(*(value),key);
		pnode= this->find(key);
		return SUCCESS;
	}

	StatusType insert(T &newNode,int k) {
		if (find(k) != NULL) {
			return FAILURE;
		}
		TNode<T> *temp = new TNode<T>(newNode,k);
		if (root == NULL) {
			root = temp;
			size = size + 1;
			return SUCCESS;
		} else {
			TNode<T>* tmpRight = FindInInsert(k);
			if (tmpRight->key < k) {
				tmpRight->right = temp;
				temp->father = tmpRight;
				size = size + 1;

			} else {
				tmpRight->left = temp;
				temp->father = tmpRight;
				size = size + 1;

			}
			updateHeight(tmpRight);
		}
		return SUCCESS;
	}

	///////////////////////////////
	void balance(TNode<T> *&node) {
		if (node == NULL)
			return;
		if (node->right != NULL) {
			TNode<T> *temp1 = node->right;
			temp1->calcHeight();
			temp1->calcBF();

		}
		if (node->left != NULL) {
			TNode<T> *temp2 = node->left;
			temp2->calcHeight();
			temp2->calcBF();
		}

		node->calcHeight();
		node->calcBF();

	}

	///////////////////////////////
	void chooseTurn(TNode<T> *&curr) {
		if (!curr)
			return;
		int pBF = curr->getBF();
		int rBF = getNodeBF(curr->right);
		int lBF = getNodeBF(curr->left);

		if (pBF == 2 && lBF >= 0)
			LL(curr);
		else if (pBF == 2 && lBF == -1)
			LR(curr);
		else if (pBF == -2 && rBF <= 0)
			RR(curr);
		else if (pBF == -2 && rBF == 1)
			RL(curr);
	}

	///////////////////////////////
	void LL(TNode<T> *node) {

		TNode<T>* temp = node->left;
		if (node->father == NULL) {
			root = temp;
		} else {
			if (node->father->left == node) {
				node->father->left = temp;
			} else {
				node->father->right = temp;
			}
		}
		temp->father = node->father;
		node->left = temp->right;
		if (temp->right != NULL) {
			temp->right->father = node;
		}
		temp->right = node;
		node->father = temp;
		updateHeightAux(node);
		updateHeightAux(temp);

	}

	///////////////////////////////
	void RR(TNode<T> *node) {

		TNode<T>* temp = node->right;
		if (node->father == NULL) {
			root = temp;
		} else {
			if (node->father->left == node) {
				node->father->left = temp;
			} else {
				node->father->right = temp;
			}
		}
		temp->father = node->father;
		node->right = temp->left;
		if (temp->left != NULL) {
			temp->left->father = node;
		}
		temp->left = node;
		node->father = temp;
		updateHeightAux(node);
		updateHeightAux(temp);
	}

	////////////////////////////////
	void LR(TNode<T> *curr) {
		RR(curr->left);
		LL(curr);
	}

	/////////////////////////////


	void RL(TNode<T> *curr) {
		LL(curr->right);
		RR(curr);
	}

	//////////////////////////////////////////////////


	TNode<T> ** inorderFunction() {
		if (root == NULL)
			return NULL;
		TNode<T> **array = new TNode<T>*[this->size];
		TNode<T> *tmp = root;
		int index = 0;
		inorderAux(array, tmp, &index);
		return array;
	}

	void inorderAux( TNode<T> **array, TNode<T> *p, int *index) {
		if (p == NULL)
			return;
		inorderAux(array, p->left, index);
		array[*index] = p;
		(*index)++;
		inorderAux(array, p->right, index);
	}

	///////////////////////////////////////////////////

	StatusType Delete(int k){
		return this->remove(k);
	}

	StatusType DeleteByPointer(TNode<T>* node) {
		if (node == NULL)
			return INVALID_INPUT;
		return this->Delete(node->key);

	}

	StatusType remove(int k) {
		TNode<T> *tmp = find(k);
		if (tmp == NULL) {
			return FAILURE;
		} else if (tmp->left == NULL && tmp->right == NULL) {
			NoSon(tmp);
		} else if (tmp->left == NULL && tmp->right != NULL) {
			RightSon(tmp);
		} else if (tmp->left != NULL && tmp->right == NULL) {
			LeftSon(tmp);
		} else if (tmp->left != NULL && tmp->right != NULL) {
			TwoSons(tmp);
		}
		size = size - 1;
		return SUCCESS;
	}


	/////////////////////////////////////////////////////


	void TwoSons(TNode<T>* node) {

		TNode<T>* nextMin = node->right;
		while (nextMin->left != NULL) {
			nextMin = nextMin->left;
		}

		int H = node->height;
		int B = node->BF;
		node->height = nextMin->height;
		node->BF = nextMin->BF;
		nextMin->height = H;
		nextMin->BF = B;
		TNode<T>* RightNext = nextMin->right;
		TNode<T>* NextFather = nextMin->father;

		if (node->father == NULL) {

			nextMin->father = NULL;
			root = nextMin;
			if (node->right == nextMin) {
				nextMin->left = node->left;
				nextMin->left->father = nextMin;
				nextMin->right = node;
				node->father = nextMin;
				node->left = NULL;
				node->right = RightNext;
				if (RightNext != NULL) {
					RightNext->father = node;
				}

			} else {
				nextMin->left = node->left;
				nextMin->left->father = nextMin;
				nextMin->right = node->right;
				node->right->father = nextMin;
				node->left = NULL;
				node->father = NextFather;
				NextFather->left = node;
				node->right = RightNext;
				if (RightNext != NULL) {
					RightNext->father = node;
				}

			}

		} else {

			nextMin->father=node->father;
			if(node->father->left == node){
				node->father->left=nextMin;
			}else{
				node->father->right=nextMin;
			}

			if (node->right == nextMin) {
				nextMin->left = node->left;
				nextMin->left->father = nextMin;
				nextMin->right = node;
				node->father = nextMin;
				node->left = NULL;
				node->right = RightNext;
				if (RightNext != NULL) {
					RightNext->father = node;
				}

			} else {
				nextMin->left = node->left;
				nextMin->left->father = nextMin;
				nextMin->right = node->right;
				node->right->father = nextMin;
				node->left = NULL;
				node->father = NextFather;
				NextFather->left = node;
				node->right = RightNext;
				if (RightNext != NULL) {
					RightNext->father = node;
				}

			}

		}

		if (RightNext != NULL) {
			RightSon(node);
		} else {
			NoSon(node);
		}

	}

///////////////////////////////////////////////////////////////////////////////


	void LeftSon(TNode<T>* node) {

		TNode<T>* LeftNode = node->left;
		if (node->father == NULL) {
			LeftNode->father = NULL;
			root = LeftNode;
			delete node;
		} else {
			if (node->father->left == node) {
				node->father->left = LeftNode;
				LeftNode->father = node->father;
				delete node;
				updateHeight(LeftNode->father);

			} else {
				node->father->right = LeftNode;
				LeftNode->father = node->father;
				delete node;
				updateHeight(LeftNode->father);

			}

		}

	}

///////////////////////////////////////////////////////////////////////////////


	void RightSon(TNode<T>* node) {

		TNode<T>* RightNode = node->right;
		if (node->father == NULL) {
			RightNode->father = NULL;
			root = RightNode;
			delete node;
		} else {
			if (node->father->left == node) {
				node->father->left = RightNode;
				RightNode->father = node->father;
				delete node;
				updateHeight(RightNode->father);

			} else {
				node->father->right = RightNode;
				RightNode->father = node->father;
				delete node;
				updateHeight(RightNode->father);

			}

		}

	}

/////////////////////////////////////////////////////


	void NoSon(TNode<T>* node) {

		if (node == root && node->father == NULL) {
			root = NULL;
			delete node;
			return;
		}
		TNode<T>* parent = node->father;

		if (!parent) {
			delete node;
			return;
		}
		if (parent->right == node) {
			parent->right = NULL;
		}
		if (parent->left == node) {
			parent->left = NULL;
		}
		delete node;
		updateHeight(parent);

	}

///////////////////////////////////////////////////////////////////////////////


StatusType Size(int * n) {
	if (n == NULL){
		return INVALID_INPUT;
	}
	*n = this->getSize();
	return SUCCESS;
}
}
;

#endif /* AVLTREE_H_ */

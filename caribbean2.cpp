/*
 * =========================================================================================
 * Name        : caribbean2.cpp
 * Author      : Nguyen Hoang Minh
 * Email       : nghoangminh1505@gmail.com
 * Copyright   : Faculty of Computer Science and Engineering - HCMC University of Technology
 * Description : Initial code for Assignment 2 - Data structures and Algorithms - Oct 2015
 * =========================================================================================
 */

//REMEMBER: Do not include any other library. Otherwise you will get 0 mark. :P

#ifndef _DEFS_H_
#include "defs.h"
#define _DEFS_H_
#endif

struct queue
{
	int key;
	queue *next;
};

void adventure(eventList*, pirateTree*&);
int getHeight(nodeTree*);
bool addNode(nodeTree*);
void delNode(int);
int getCount(nodeTree*);
int extractEvent(int);
void addShip(int);
void monsterAttack(int);
void monsterAttack2(int);
void Bermuda();
void Poseiden();
void deleteMyTree(nodeTree*&);
void soulLand();
void deadManNoTell();
void strangeWedding();
void driverGuy(int);
void Hades();
int _pow(int x, int y);

pirateTree *myTree;
bool isAVL = 0, isS9 = 0, isHalt = 0 , isRiverCrossed = 0, isS12 = 0;
bool isSoulLand = 0, isPosiden = 0, isBermuda = 0, isSteeled = 0, isJack = 1, isBlackBeard = 1;
int eventNum, prev11;

void adventure(eventList* pEvent, pirateTree*& aTree)
{
	int eventCode, backEvent = 0;
	eventList *backup = pEvent;
	myTree = aTree;
	eventNum = 0;
	prev11 = 0;
	while (pEvent != NULL)
	{
		eventCode = (pEvent->nEventCode > 0)? pEvent->nEventCode: (pEvent->nEventCode * -1);
		eventNum++;
		switch (extractEvent(pEvent->nEventCode))
		{
		case 1:
			if (!isSoulLand && !isRiverCrossed) addShip(eventCode);
			break;
		case 2:
			if (!isPosiden) monsterAttack(eventCode);
			break;
		case 3:
			if (!isSoulLand && !isRiverCrossed &&
				((!isBermuda && !isPosiden) || (isPosiden && isSteeled))) Bermuda();
			break;
		case 4:
			if (!isSteeled) Poseiden();
			break;
		case 5:
			if (isSteeled) monsterAttack2(eventCode);
			break;
		case 6:
			if (isSoulLand || isRiverCrossed) break;
			if (isSteeled) soulLand(); else deleteMyTree(myTree->root);
			break;
		case 7:
			if (isSoulLand && isJack) deadManNoTell();
			break;
		case 8:
			if (isSoulLand) strangeWedding();
			break;
		case 9:
			if (isS9 && !isRiverCrossed) driverGuy(eventCode);
			break;
		case 10:
			if (!isJack && isRiverCrossed && prev11 == 0)
			{
				Hades();
				prev11 = eventNum;
			}
			break;
		case 11:
			if (isJack && prev11 > 0)
			{
				backEvent = (999 % prev11) + 1;
				isS12 = 1;
				eventNum = 0;
				pEvent = backup;
				isAVL = 0; isS9 = 0; isRiverCrossed = 0;
				isSoulLand = 0; isPosiden = 0; isBermuda = 0; isSteeled = 0;
				isBlackBeard = 1;
				prev11 = 0;
				deleteMyTree(myTree->root);
				continue;
			}
			break;
		}
		if (myTree->root == NULL) break;
		if (isHalt) break;

		if (isS12 && eventNum == backEvent)
		{
			if (!isJack)
			{
				deleteMyTree(myTree->root);
			}
			break;
		}

		pEvent = pEvent->pNext;
	}
}

int extractEvent(int eventCode)
{
	//eventCode = (eventCode > 0)? eventCode: (eventCode * -1);
	if (eventCode == 777) return 3;
	if (eventCode == -11111) return 4;
	if (eventCode == 5) return 6;
	if (eventCode == 6) return 7;
	if (eventCode == 7) return 8;
	if (eventCode == 13) return 10;
	if (eventCode == 999) return 11;

	if (eventCode / 10000 == 1) return 1;
	if (eventCode / 10000 == -2) return 2;
	if (eventCode / 10000 == -3) return 5;
	if (eventCode / 10 == 8) return 9;

	return 0;
}

void addShip(int code)
{
	int level = code % 10;
	if (level < 2 || level > 7) return;
	int key = code / 10 % 1000;

	nodeTree *ship = new nodeTree;
	ship->balance = 0;
	ship->exp = 0;
	ship->key = key;
	ship->level = level;
	ship->pLeft = NULL;
	ship->pRight = NULL;

	while (!addNode(ship))
	{
		ship->key--;
		if (ship->key < 0) break;
	}
	if (ship->key < 0) delete ship;
}

void monsterAttack(int code)
{
	int levelO = code % 10;
	int keyO = code / 10 % 1000;
	nodeTree *root = myTree->root;
	nodeTree *attackShip = myTree->root;

	while (root != NULL)
	{
		if (root->key < keyO)
		{
			attackShip = root;
			root = root->pRight;
		}
		else root = root->pLeft;
	}

	if (attackShip->level <= levelO) delNode(attackShip->key);
	else
	{
		int delta = 1000 / (((attackShip->level - levelO) + 1) * (attackShip->level + 1));
		if (attackShip->level == 7)
		{
			attackShip->exp = (100 < (attackShip->exp + delta))? 100 : (attackShip->exp + delta);
		}
		else
		{
			attackShip->exp += delta;
			while (attackShip->exp >= 100)
			{
				attackShip->level += 1;
				attackShip->exp -= 100;
			}
		}
		//cout << attackShip->exp << "\n";
	}

}

nodeTree* deleteLeafRNL(nodeTree *root)
{
	if (root == NULL) return NULL;

	if (root->pLeft == NULL && root->pRight == NULL)
	{
		delete root;
		return NULL;
	}

	root->pRight = deleteLeafRNL(root->pRight);
	root->pLeft = deleteLeafRNL(root->pLeft);

	return root;
}

void Bermuda()
{
	myTree->root = deleteLeafRNL(myTree->root);
	isBermuda = 1;
}

void deleteLevel(nodeTree *root, int level, int rLevel)
{
	if (root == NULL) return;
	if (level == rLevel)
	{
		delNode(root->key);
	}
	else
	{
		deleteLevel(root->pLeft, level + 1, rLevel);
		deleteLevel(root->pRight, level + 1, rLevel);
	}
}

bool sidaShip(nodeTree *root)
{
	if (root == NULL) return 0;
	if (root->key % 111 == 0) return 1;
	bool sidaLeft = sidaShip(root->pLeft);
	bool sidaRight = sidaShip(root->pRight);
	return sidaLeft | sidaRight;
}

void Poseiden()
{
	int height = getHeight(myTree->root);
	isPosiden = 1;
	if (sidaShip(myTree->root))
	{
		int n = (height - 2) > 0? (height - 2):0;
		deleteLevel(myTree->root, 0, n);
	}
	else isSteeled = 1;
}

void findNearestKey(nodeTree* root, nodeTree*& retNode, int retKey, int key)
{
	if (root == NULL) return;
	int absKey = root->key - key;
	if (absKey < 0) absKey *= -1;

	if (absKey < retKey || (absKey == retKey && retNode->key > root->key))
	{
		retKey = absKey;
		retNode = root;
	}

	findNearestKey(root->pLeft, retNode, retKey, key);
	findNearestKey(root->pRight, retNode, retKey, key);
}

void monsterAttack2(int code)
{
	int levelO = code % 10;
	int keyO = code / 10 % 1000;
	nodeTree *attackShip = myTree->root;
	int cnt = 3;
	while (cnt)
	{
		findNearestKey(myTree->root, attackShip, 1000, keyO);

		int absKey = keyO - attackShip->key;
		if (absKey < 0) absKey *= -1;

		if (attackShip->level <= levelO)
		{
			keyO += 3*absKey;
			while (keyO > 999) keyO -= 1000;

			delNode(attackShip->key);
		}
		else
		{
			int delta = 1000 / (((attackShip->level - levelO) + 1) * (attackShip->level + 1));
			if (attackShip->level == 7)
			{
				attackShip->exp = (100 < (attackShip->exp + delta))? 100 : (attackShip->exp + delta);
			}
			else
			{
				attackShip->exp += delta;
				while (attackShip->exp >= 100)
				{
					attackShip->level += 1;
					attackShip->exp -= 100;
				}

			}
			cnt--;
			keyO -= 3*absKey;
			while (keyO < 0) keyO += 1000;
			//cout << attackShip->exp << "\n";
		}
		if (myTree->root == NULL) return;
	}
}

void insertNLR(nodeTree* root)
{
	if (root == NULL) return;
	nodeTree *node = new nodeTree;
	node->balance = 0;
	node->exp = root->exp;
	node->key = root->key;
	node->level = root->level;
	node->pLeft = NULL;
	node->pRight = NULL;

	addNode(node);
	insertNLR(root->pLeft);
	insertNLR(root->pRight);

	delete root;
}

void incLevel(nodeTree *root)
{
	if (root == NULL) return;
	if (root->level < 7) root->level++;
	incLevel(root->pLeft);
	incLevel(root->pRight);
}

void soulLand()
{
	nodeTree *oldRoot = myTree->root;
	myTree->root = new nodeTree();
	myTree->root->balance = 0;
	myTree->root->exp = 0;
	myTree->root->key = 777;
	myTree->root->level = 7;
	myTree->root->pLeft = NULL;
	myTree->root->pRight = NULL;

	insertNLR(oldRoot);
	isSoulLand = 1;
	isJack = 1;
	if (_pow(2, getHeight(myTree->root)) - 1 == getCount(myTree->root))
		incLevel(myTree->root);
}

void listLNR(nodeTree *root, queue *&rear)
{
	if (root == NULL) return;
	listLNR(root->pLeft, rear);
	rear->next = new queue;
	rear = rear->next;
	rear->next = NULL;
	rear->key = root->key;
	listLNR(root->pRight, rear);
}

void deadManNoTell()
{
	//Find choosen Node
	nodeTree *choosenNode = NULL;
	struct ptrQueue
	{
		nodeTree *ptr;
		ptrQueue *next;
	};
	ptrQueue *pfront = new ptrQueue;
	ptrQueue *prear = pfront;
	//pfront->next = prear;
	prear->next = NULL;

	int cnt = 1;
	int j = getHeight(myTree->root);
	j = (j > 4)? j/2 : 1;
	prear->ptr = myTree->root;

	if (j == 1) choosenNode = myTree->root;

	while (j!= 1 && pfront != NULL)
	{
		nodeTree *node = pfront->ptr;
		ptrQueue *del = pfront;


		if (node->pLeft != NULL)
		{
			cnt++;
			if (cnt == j)
			{
				choosenNode = node->pLeft;
				break;
			}
			prear->next = new ptrQueue;
			prear = prear->next;
			prear->ptr = node->pLeft;
			prear->next = NULL;
		}
		if (node->pRight != NULL)
		{
			cnt++;
			if (cnt == j)
			{
				choosenNode = node->pRight;
				break;
			}
			prear->next = new ptrQueue;
			prear = prear->next;
			prear->ptr = node->pRight;
			prear->next = NULL;
		}

		pfront = pfront->next;
		delete del;
	}

	while (pfront != NULL)
	{
		ptrQueue *del = pfront;
		pfront = pfront->next;
		delete del;
	}

	queue *front = NULL;
	queue *rear = NULL;

	//Find branch
	bool flag = 0;
	for (int i = 0; i <= 1; i++)
	{
		front = new queue;
		front->next = NULL;
		rear = front;

		int way = i;
		cnt = 0;
		nodeTree *node = choosenNode;
		while (node != NULL)
		{
			rear->next = new queue;
			rear = rear->next;
			rear->next = NULL;
			rear->key = node->key;
			cnt++;
			if (cnt >= 4 && node->pLeft == NULL && node->pRight == NULL) flag = 1;

			if (way == 0) node = node->pLeft;
			else node = node->pRight;
			way = !(way);
		}
		if (flag) break;
		while (front != NULL)
		{
			queue *del = front;
			front = front->next;
			delete del;
		}
	}

	if (!flag)
	{
		front = new queue;
		front->next = NULL;
		rear = front;
		listLNR(myTree->root->pLeft, rear);
	}

	//delete branch
	while (front->next != NULL)
	{
		queue *del = front;
		if (front->next->key == 777) isJack = 0;
		if (front->next->key == 888 && isS9)
		{
			isBlackBeard = 0;
			isAVL = 0;
		}
		delNode(front->next->key);
		front = front->next;
		delete del;
	}
	delete front;
}

void insertRNL(nodeTree* root)
{
	if (root == NULL) return;
	insertRNL(root->pRight);
	nodeTree *node = new nodeTree;
	node->balance = 0;
	node->exp = root->exp;
	node->key = root->key;
	node->level = root->level;
	node->pLeft = NULL;
	node->pRight = NULL;

	addNode(node);
	insertRNL(root->pLeft);


	delete root;
}

void outsideNodeLeft(nodeTree* root, queue *&rear)
{
	if (root == NULL) return;
	if (root->pLeft == NULL && root->pRight == NULL) return;
	outsideNodeLeft(root->pLeft, rear);

	rear->next = new queue;
	rear = rear->next;
	rear->next = NULL;
	rear->key = root->key;
}

void outsideNodeRight(nodeTree* root, queue *&rear)
{
	if (root == NULL) return;
	if (root->pLeft == NULL && root->pRight == NULL) return;

	rear->next = new queue;
	rear = rear->next;
	rear->next = NULL;
	rear->key = root->key;

	outsideNodeRight(root->pRight, rear);
}

void strangeWedding()
{
	isS9 = 1;
	nodeTree *oldRoot = myTree->root;
	myTree->root = new nodeTree();
	myTree->root->balance = 0;
	myTree->root->exp = 0;
	myTree->root->key = 888;
	myTree->root->level = 7;
	myTree->root->pLeft = NULL;
	myTree->root->pRight = NULL;

	isAVL = 1;

	if (!isBlackBeard)
	{
		isAVL = 0;
		myTree->root = NULL;
	}

	insertRNL(oldRoot);

	queue *front = new queue;
	queue *rear = front;
	rear->next = NULL;

	outsideNodeLeft(myTree->root->pLeft, rear);

	while (front->next != NULL)
	{
		queue *del = front;
		if (front->next->key == 777) isJack = 0;
		if (front->next->key == 888)
		{
			isBlackBeard = 0;
			isAVL = 0;
		}
		delNode(front->next->key);
		front = front->next;
		delete del;
	}
	delete front;

	front = new queue;
	rear = front;
	rear->next = NULL;

	outsideNodeRight(myTree->root->pRight, rear);

	while (front->next != NULL)
	{
		queue *del = front;
		if (front->next->key == 777) isJack = 0;
		if (front->next->key == 888)
		{
			isBlackBeard = 0;
			isAVL = 0;
		}
		delNode(front->next->key);
		front = front->next;
		delete del;
	}
	delete front;
}

void listLNRleaf(nodeTree *root, queue *&rear)
{
	if (root == NULL) return;
	listLNRleaf(root->pLeft, rear);
	if (root->pLeft == NULL && root->pRight == NULL)
	{
		rear->next = new queue;
		rear = rear->next;
		rear->next = NULL;
		rear->key = root->key;
	}
	listLNRleaf(root->pRight, rear);
}

void driverGuy(int code)
{
	int x = code%10;

	if (isJack)
	{
		isHalt = 1;
		return;
	}

	queue *front = new queue;
	queue *rear = front;
	rear->next = NULL;

	if (getCount(myTree->root) > (3 * x))
	{
		listLNR(myTree->root->pRight, rear);
	}
	else
	{
		isRiverCrossed = 1;
		isSoulLand = 0;
		listLNRleaf(myTree->root->pLeft, rear);
	}

	while (front->next != NULL)
	{
		queue *del = front;
		if (front->next->key == 888)
		{
			isBlackBeard = 0;
			isAVL = 0;
		}
		delNode(front->next->key);
		front = front->next;
		delete del;
	}
	delete front;
}

nodeTree* makeTrace(nodeTree *root, int prev)
{
	if (root == NULL) return NULL;
	nodeTree* traceRoot = new nodeTree;

	traceRoot->key = prev + root->key;
	traceRoot->pLeft = makeTrace(root->pLeft, traceRoot->key);
	traceRoot->pRight = makeTrace(root->pRight, traceRoot->key);

	return traceRoot;
}

int processTraceKey(nodeTree *root)
{
	if (root == NULL) return 0;
	if (root->pLeft == NULL && root->pRight == NULL) return root->key;

	int left = processTraceKey(root->pLeft);
	int right = processTraceKey(root->pRight);

	if ((left % 13 == 0 && right % 13 == 0) || (left % 13 != 0 && right % 13 != 0)
			|| left == 0 || right == 0)
	{
		root->key = (left > right)? left : right;
	}
	else if (left % 13 == 0) root->key = left;
	else if (right % 13 == 0)root->key = right;

	return root->key;
}

void Tracing(nodeTree *root, nodeTree *traceRoot, queue *&rear, int key, bool &same)
{
	if (root == NULL) return;
	Tracing(root->pLeft, traceRoot->pLeft, rear, key, same);
	if (traceRoot->key == key)
	{
		rear->next = new queue;
		rear = rear->next;
		rear->next = NULL;
		rear->key = root->key;
		if (traceRoot->pLeft != NULL && traceRoot->pRight != NULL)
			if ((traceRoot->pLeft->key == traceRoot->pRight->key)
					&& ( traceRoot->pLeft->key == key)) same = 1;
	}
	Tracing(root->pRight, traceRoot->pRight, rear, key, same);
}

void Hades()
{
	nodeTree *traceRoot = NULL;

	traceRoot = makeTrace(myTree->root, 0);
	traceRoot->key = processTraceKey(traceRoot);

	if (traceRoot->key % 13 == 0)
	{
		isJack = 1;
		queue *front = new queue;
		queue *rear = front;
		rear->next = NULL;
		bool same = 0;
		Tracing(myTree->root, traceRoot, rear, traceRoot->key, same);
		if (same == 0)
		{
			while (front != NULL)
			{
				queue *del = front;
				front = front->next;
				delete del;
			}
			int key = traceRoot->key;
			front = new queue;
			rear = new queue;
			front->next = rear;
			rear->next = NULL;
			rear->key = myTree->root->key;

			nodeTree *root = myTree->root;
			while (traceRoot->pLeft != NULL || traceRoot->pRight != NULL)
			{
				if (traceRoot->pLeft != NULL)
					if (traceRoot->pLeft->key == key)
					{
						traceRoot = traceRoot->pLeft;
						root = root->pLeft;
						rear->next = new queue;
						rear = rear->next;
						rear->next=NULL;
						rear->key = root->key;
						continue;
					}

				if (traceRoot->pRight != NULL)
					if (traceRoot->pRight->key == key)
					{
						traceRoot = traceRoot->pRight;
						root = root->pRight;
						rear->next = new queue;
						rear = rear->next;
						rear->next=NULL;
						rear->key = root->key;
						continue;
					}
			}
		}

		while (front->next != NULL)
		{
			queue *del = front;
			if (front->next->key == 888)
			{
				isBlackBeard = 0;
				isAVL = 0;
			}
			//num = front->next->ptr->key;
			delNode(front->next->key);
			front = front->next;
			delete del;
		}
		delete front;
	}
	else
	{
		deleteMyTree(myTree->root);
	}

	deleteMyTree(traceRoot);
}

/*----------------------------------------------------------*/
/*----------------------------------------------------------*/
/*----------------------------------------------------------*/

void AVLrotateLeft(nodeTree *&root)
{
	nodeTree *tmp = root->pRight;
	root->pRight = tmp->pLeft;
	tmp->pLeft = root;
	root = tmp;
}

void AVLrotateRight(nodeTree *&root)
{
	nodeTree *tmp = root->pLeft;
	root->pLeft = tmp->pRight;
	tmp->pRight = root;
	root = tmp;
}

void AVLleftBalance(nodeTree *&root, bool &taller)
{
	nodeTree *&leftTree = root->pLeft;
	switch (leftTree->balance)
	{
	case 0:
		leftTree->balance = 1;
		taller = false;
		AVLrotateRight(root);
		break;
	case -1:
		root->balance = 0;
		leftTree->balance = 0;
		AVLrotateRight(root);
		break;
	case 1:
		nodeTree *rightTree = leftTree->pRight;
		switch (rightTree->balance)
		{
		case 0:
			root->balance = 0;
			leftTree->balance = 0;
			break;
		case -1:
			root->balance = 1;
			leftTree->balance = 0;
			break;
		case 1:
			root->balance = 0;
			leftTree->balance = -1;
			break;
		}
		rightTree->balance = 0;
		AVLrotateLeft(leftTree);
		AVLrotateRight(root);
		break;
	}
}

void AVLrightBalance(nodeTree *&root, bool &taller)
{
	nodeTree *&rightTree = root->pRight;
	switch (rightTree->balance)
	{
	case 0:
		rightTree->balance = -1;
		taller = false;
		AVLrotateLeft(root);
		break;
	case 1:
		root->balance = 0;
		rightTree->balance = 0;
		AVLrotateLeft(root);
		break;
	case -1:
		nodeTree *leftTree = rightTree->pLeft;
		switch (leftTree->balance)
		{
		case 0:
			root->balance = 0;
			rightTree->balance = 0;
			break;
		case -1:
			root->balance = 0;
			rightTree->balance = 1;
			break;
		case 1:
			root->balance = -1;
			rightTree->balance = 0;
			break;
		}
		leftTree->balance = 0;
		AVLrotateRight(rightTree);
		AVLrotateLeft(root);
		break;
	}
}

bool AVLinsert(nodeTree *&root, nodeTree *node, bool &taller)
{
	bool ret = 1;
	if (root == NULL)
	{
		root = node;
		taller = true;
		return 1;
	}
	else if (node->key == root->key)
	{
		taller = false;
		ret = 0;
	}
	else if (node->key < root->key)
	{
		ret = AVLinsert(root->pLeft, node, taller);
		if (taller == true)
		{
			switch (root->balance)
			{
			case -1:
				AVLleftBalance(root, taller);
				taller = false;
				break;
			case 0:
				root->balance = -1;
				break;
			case 1:
				root->balance = 0;
				taller = false;
				break;
			}
		}
	}
	else
	{
		ret = AVLinsert(root->pRight, node, taller);
		if (taller == true)
			switch (root->balance)
			{
			case -1:
				root->balance = 0;
				taller = false;
				break;
			case 0:
				root->balance = 1;
				break;
			case 1:
				AVLrightBalance(root, taller);
				taller = false;
				break;
			}
	}

	return ret;
}

nodeTree* AVLremove(nodeTree *root, int key, bool &shorter)
{
	if (root == NULL)
	{
		shorter = false;
	}
	else if (root->key == key)
	{
		nodeTree *temp = root;
		if (root->pRight == NULL)
		{
			root = root->pLeft;
			shorter = true;
			delete temp;
		}
		else if (root->pLeft == NULL)
		{
			root = root->pRight;
			shorter = true;
			delete temp;
		}
		else
		{
			temp = root->pRight;
			while (temp->pLeft != NULL) temp = temp->pLeft;

			root->key = temp->key;
			root->exp = temp->exp;
			//root->balance = temp->balance;
			root->level = temp->level;

			root->pRight = AVLremove(root->pRight, temp->key, shorter);
			if(shorter == true)
			{
				switch(root->balance)
				{
					case 0:
						root->balance = -1;
						shorter = false;
						break;
					case -1:
						AVLleftBalance(root, shorter);
						break;
					case 1:
						root->balance = 0;
						break;
				}
			}
		}
	}
	else if (key < root->key)
	{
		root->pLeft = AVLremove(root->pLeft, key, shorter);
		if(shorter == true)
			switch(root->balance)
			{
			case 0:
				root->balance = 1;
				shorter = false;
				break;
			case -1:
				root->balance = 0;
				break;
			case 1:
				AVLrightBalance(root, shorter);
				break;
			}
	}
	else
	{
		root->pRight = AVLremove(root->pRight, key, shorter);
		if(shorter == true)
			switch(root->balance)
			{
			case 0:
				root->balance = -1;
				shorter = false;
				break;
			case 1:
				root->balance = 0;
				break;
			case -1:
				AVLleftBalance(root, shorter);
				break;
			}
	}
	return root;
}

bool BSTinsert(nodeTree *&root, nodeTree* node)
{
	if (root == NULL)
	{
		root = node;
		return 1;
	}
	if (node->key < root->key) return BSTinsert(root->pLeft, node);
	if (node->key > root->key) return BSTinsert(root->pRight, node);
	return 0;
}

bool addNode(nodeTree *node)
{
	if (!isAVL)
	{
		return BSTinsert(myTree->root, node);
	}
	else
	{
		bool taller;
		return AVLinsert(myTree->root, node, taller);
	}
}

nodeTree* BSTremove(nodeTree *root, int key)
{
	if (root == NULL) return root;
	if (key < root->key) root->pLeft = BSTremove(root->pLeft, key);
	else if (key > root->key) root->pRight = BSTremove(root->pRight, key);
	else
	{
        if (root->pLeft == NULL)
        {
            nodeTree *temp = root->pRight;
            delete root;
            return temp;
        }
        else if (root->pRight == NULL)
        {
            nodeTree *temp = root->pLeft;
            delete root;
            return temp;
        }

        nodeTree *temp = root->pRight;
        while (temp->pLeft != NULL) temp = temp->pLeft;

        root->key = temp->key;
        root->exp = temp->exp;
        root->balance = temp->balance;
        root->level = temp->level;

        root->pRight = BSTremove(root->pRight, temp->key);
	}

	return root;
}

void delNode(int key)
{
	if (!isAVL)
	{
		myTree->root = BSTremove(myTree->root, key);
	}
	else
	{
		bool shorter;
		myTree->root = AVLremove(myTree->root, key, shorter);
	}
}

int getHeight(nodeTree* root)
{
	if (root == NULL) return 0;
	int leftHeight = getHeight(root->pLeft);
	int rightHeight = getHeight(root->pRight);
	return (leftHeight > rightHeight)? leftHeight + 1 : rightHeight + 1;
}

void deleteMyTree (nodeTree *&root)
{
	if (!root) return;
	nodeTree *pDel = root;
	deleteMyTree(root->pLeft);
	deleteMyTree(root->pRight);
	delete pDel; root = NULL;
}

int getCount(nodeTree* root)
{
	if (root == NULL) return 0;
	return 1 + getCount(root->pLeft) + getCount(root->pRight);
}

int _pow(int x, int y)
{
	if (y == 0) return 1;
	return x * pow(x,y - 1);
}

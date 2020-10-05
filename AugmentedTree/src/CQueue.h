#pragma once
#include "TTree.h"

template <typename T>
class CQueue : public TTree<T>
{
	/**
	 * @author Evripidis Pavlidis
	 * @since 2020-5-12
	 */

public:
	CQueue() {};
	CQueue(T* p);
	CQueue(Node<T>* root, int height, Node<T>* minNode, Node<T>* maxNode);
	~CQueue() {
		std::cout << "CQueue Destructor Called." << std::endl;
	};

	void ShallowCopy(CQueue<T>* CQ2);

	CQueue<T>* Split(T valueToSplit, bool returnFlag);
	void SplitAtLeaf(Node<T>* splittingNode, T valueToSplit, CQueue<T>* q_lesseq, CQueue<T>* q_bigger);     // maybe returns void
	void SplitAtInternal(Node<T>* splittingNode, T valueToSplit, CQueue<T>* q_lesseq, CQueue<T>* q_bigger); // maybe returns void
};

template<typename T>
inline CQueue<T>::CQueue(T* p) {
	this->Insert(p);
}

template<typename T>
inline CQueue<T>::CQueue(Node<T>* root, int height, Node<T>* minNode, Node<T>* maxNode)
{
	this->root = root;
	this->height = height;
	this->minNode = minNode;
	this->maxNode = maxNode;
}

template<typename T>
inline void CQueue<T>::ShallowCopy(CQueue<T>* CQ2)
{
	if (!CQ2) return;
	this->root = CQ2->GetRoot();
	this->height = CQ2->GetHeight();
	this->minNode = CQ2->GetMinNode();
	this->maxNode = CQ2->GetMaxNode();
}

template <typename T>
inline CQueue<T>* CQueue<T>::Split(T p, bool returnFlag) {
	// returnFlag true = Left SubTree (Smaller/Equal to p)
	// returnFlag false = Right SubTree (Bigger than p)
	
	CQueue<T>* q_lesseq = new CQueue();
	CQueue<T>* q_bigger = new CQueue();

	if (this->root == nullptr) return q_lesseq;

	if (p < *this->minNode->getLVal()) {
		if (returnFlag) return q_lesseq;
		else {
			q_bigger->ShallowCopy(this);
			this->ShallowCopy(q_lesseq);
			return q_bigger;
		}
	}
	if (p > *this->maxNode->getMaxVal()) {
		if (returnFlag) {
			q_lesseq->ShallowCopy(this);
			this->ShallowCopy(q_bigger);
			return q_lesseq;
		}
		else return q_bigger;
	}

	Node<T>* itr = this->root;
	while (!itr->getIsLeaf()) {
		Node<T>* LMax = itr->getLMax();
		Node<T>* MMax = itr->getMMax();
		Node<T>* RMax = itr->getRMax();

		if (RMax) {
			if (*RMax->getMaxVal() == p) {
				SplitAtInternal(itr, p, q_lesseq, q_bigger);
				break;
			}
			if (*RMax->getMaxVal() < p) {
				itr = itr->getRCh();
				continue;
			}
		}
		if (MMax) {
			if (*MMax->getMaxVal() == p) {
				SplitAtInternal(itr, p, q_lesseq, q_bigger);
				break;
			}
			if (*MMax->getMaxVal() < p){
				itr = itr->getM2Ch();
				continue;
			}
		}
		if (LMax) {
			if (*LMax->getMaxVal() == p) {
				SplitAtInternal(itr, p, q_lesseq, q_bigger);
				break;
			}
			if (*LMax->getMaxVal() < p) {
				if (itr->getNumVal() == 1)
					itr = itr->getRCh();
				else 
					itr = itr->getM1Ch();
				continue;
			}
			else itr = itr->getLCh();
		}
	}

	if (itr->getIsLeaf()) {
		T* LVal = itr->getLVal();
		T* MVal = itr->getMVal();
		T* RVal = itr->getRVal();

		if (p < *LVal) {
			SplitAtLeaf(itr, *itr->getChainLeft()->getMaxVal(), q_lesseq, q_bigger);
		}
		else if (p == *LVal) {
			SplitAtLeaf(itr, *LVal, q_lesseq, q_bigger);
		}
		else if (p == *RVal) {
			SplitAtLeaf(itr, *RVal, q_lesseq, q_bigger);
		}
		else {
			if (itr->getNumVal() == 2) {
				if (p < *RVal)
					SplitAtLeaf(itr, *LVal, q_lesseq, q_bigger);
			}
			else {
				if (p == *MVal) {
					SplitAtLeaf(itr, *MVal, q_lesseq, q_bigger);
				}
				else if (p < *MVal) {
					SplitAtLeaf(itr, *LVal, q_lesseq, q_bigger);
				}
				else {
					SplitAtLeaf(itr, *MVal, q_lesseq, q_bigger);
				}
			}
		}
	}

	// TAKE CARE OF MEMORY CLEARS !!

	if (returnFlag) {
		delete this;
		this->ShallowCopy(q_bigger);
		return q_lesseq;
	}
	else {
		delete this;
		this->ShallowCopy(q_lesseq);
		return q_bigger;
	}
	// BEFORE I RETURN I HAVE TO DELETE / FREE THE APPROPRIATE MEMORY
}

template<typename T>
inline void CQueue<T>::SplitAtLeaf(Node<T>* splittingNode, T valueToSplit, CQueue<T>* q_lesseq, CQueue<T>* q_bigger)
{
	Node<T>* father = splittingNode->getFather();

	if (valueToSplit == *splittingNode->getLVal()) {
		Node<T>* newLeft = new Node<T>(splittingNode->getLVal());
		Node<T>* newRight = nullptr;
		
		q_lesseq->SetRoot(newLeft);
		q_lesseq->SetMinNode(newLeft);
		q_lesseq->SetMaxNode(newLeft);
		q_lesseq->SetHeight(0);

		if (splittingNode->getMVal()) {
			newRight = new Node<T>(splittingNode->getMVal(), splittingNode->getRVal());
		}
		else if (splittingNode->getRVal()) {
			newRight = new Node<T>(splittingNode->getRVal());
		}

		if (newRight) {
			q_bigger->SetRoot(newRight);
			q_bigger->SetMinNode(newRight);
			q_bigger->SetMaxNode(newRight);
			q_bigger->SetHeight(0);
		}
	}
	else if (valueToSplit == *splittingNode->getRVal()) {
		splittingNode->setChainRight(nullptr);
		splittingNode->setFather(nullptr);
		father->removeChild(splittingNode);

		q_lesseq->SetRoot(splittingNode);
		q_lesseq->SetMinNode(splittingNode);
		q_lesseq->SetMaxNode(splittingNode);
		q_lesseq->SetHeight(0);
	}
	else {
		Node<T>* newLeft = new Node<T>(splittingNode->getLVal(), splittingNode->getMVal());
		Node<T>* newRight = new Node<T>(splittingNode->getRVal());

		q_lesseq->SetRoot(newLeft);
		q_lesseq->SetMinNode(newLeft);
		q_lesseq->SetMaxNode(newLeft);
		q_lesseq->SetHeight(0);

		q_bigger->SetRoot(newRight);
		q_bigger->SetMinNode(newRight);
		q_bigger->SetMaxNode(newRight);
		q_bigger->SetHeight(0);

	}
	return SplitAtInternal(father, valueToSplit, q_lesseq, q_bigger);
}

template<typename T>
inline void CQueue<T>::SplitAtInternal(Node<T>* splittingNode, T valueToSplit, CQueue<T>* q_lesseq, CQueue<T>* q_bigger)
{
	if (!splittingNode) return ;

	Node<T>* father = splittingNode->getFather();

	if (splittingNode->getNumVal() == 1) {

		if (valueToSplit < *splittingNode->getLMax()->getMaxVal()) {
			Node<T>* leftCh = splittingNode->getLCh();
			Node<T>* rightCh = splittingNode->getRCh();
			if (leftCh->getHeight() == q_bigger->GetHeight()) {
				splittingNode->setLCh(q_bigger);
				q_bigger->SetRoot(splittingNode);
				q_bigger->SetHeight(q_bigger->GetHeight() + 1);
				// minNode stays same
				// maxNode -> RCh subtree maxNode 
				// TODO find maxNode
				// TODO sever splitting node from tree
				// TODO fix Chaining.
				// TODO delete leftCh subtree.

			}
			else {

				// CHECK AGAIN !!!

				CQueue<T> tmp;
				tmp.SetRoot(rightCh);
				tmp.SetHeight(rightCh->getHeight());
				Node<T>* maxItr, * minItr;
				while (minItr->getLCh()) {
				// TODO fix right side chaining.
					maxItr->setChainLeft(nullptr);
					
					minItr = minItr->getLCh();
					maxItr = maxItr->getRCh();
				}
				tmp.SetMaxNode(maxItr);
				tmp.SetMinNode(minItr);

				// TODO glue two trees together.

				// sever RCh from tree

				rightCh->setFather(nullptr);
				splittingNode->setRCh(nullptr);

			}
		}
		else if (valueToSplit == *splittingNode->getLMax()->getMaxVal()) {

			// CHECK AGAIN !!!

			Node<T>* ql_root = splittingNode->getLCh();
			q_lesseq->SetRoot(ql_root);
			q_lesseq->SetMinNode(ql_root);
			q_lesseq->SetMaxNode(ql_root);
			q_lesseq->SetHeight(ql_root->getHeight());
			
			Node<T>* qr_root = splittingNode->getRCh();
			q_bigger->SetRoot(qr_root);
			q_bigger->SetMinNode(qr_root);
			q_bigger->SetMaxNode(qr_root);
			q_bigger->SetHeight(qr_root ->getHeight());

			Node<T>* left_itr = ql_root;
			Node<T>* right_itr = qr_root;

			while (left_itr) {
				left_itr->setChainRight(nullptr);
				right_itr->setChainLeft(nullptr);

				left_itr = left_itr->getRCh();
				right_itr = right_itr->getLCh();
			}

			splittingNode->setLCh(nullptr);
			splittingNode->setRCh(nullptr);
		}
		else {

		}

	}


	return ;

}
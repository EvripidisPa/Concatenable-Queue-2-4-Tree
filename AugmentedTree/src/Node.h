#pragma once
#include <vector>

template<typename T>
class Node
{
	/**
	 * @author Evripidis Pavlidis
	 * @since 2020-5-12
	 */

protected:
	// values 
	T* l = nullptr;
	T* m = nullptr;
	T* r = nullptr;

	// predecessor nodes

	Node* l_max = nullptr;
	Node* m_max = nullptr;
	Node* r_max = nullptr;

	// children nodes
	Node* l_ch = nullptr;
	Node* m1_ch = nullptr;
	Node* m2_ch = nullptr;
	Node* r_ch = nullptr;

	// father node 
	Node* fatherNode = nullptr;
	Node* chainLeft = nullptr;
	Node* chainRight = nullptr;
	bool isLeaf;

public:
	// constructors 

	Node(T* single_value);

	Node(T* first_value, T* second_value);

	Node(Node* l_predecessor_node, Node* leftChild, Node* rightChild);


	// setters + getters

	T* getLVal();
	T* getMVal();
	T* getRVal();

	Node* getLMax();
	Node* getMMax();
	Node* getRMax();

	T* getMaxVal();
	int getHeight();

	int getNumVal();
	int getNumCh();
	std::vector<T> getAllVal();
	std::vector<Node*> getAllCh();
	
	Node* getLCh();
	Node* getM1Ch();
	Node* getM2Ch();
	Node* getRCh();
	
	Node* getLSibling();
	Node* getRSibling();
	
	Node* getFather();
	Node* getChainLeft();
	Node* getChainRight();
	bool getIsLeaf();

	void setLVal(T*);
	void setMVal(T*);
	void setRVal(T*);

	void setLMax(Node*);
	void setMMax(Node*);
	void setRMax(Node*);
	
	void ValueCopy(const Node& node2);
	
	void setLCh(Node*);
	void setM1Ch(Node*);
	void setM2Ch(Node*);
	void setRCh(Node*);
	
	void setFather(Node<T>*);
	void setChainLeft(Node<T>*);
	void setChainRight(Node<T>*);

	void clear();
	void removeVal(T*);
	void removeChild(Node<T>*);
};

template <typename T>
inline Node<T>::Node(T* single_value) {
	// leaf node
	this->l = single_value;
	this->l_max = this;
	this->isLeaf = true;
}

template<typename T>
inline Node<T>::Node(T* first_value, T* second_value)
{	
	// leaf node with 2 values
	this->l = first_value;
	this->r = second_value;
	this->l_max = this;
	this->isLeaf = true;
}

template<typename T>
inline Node<T>::Node(Node* l_predecessor_node, Node* leftChild, Node* rightChild) {
	// internal node
	this->l_max = l_predecessor_node;
	this->setLCh(leftChild);
	this->setRCh(rightChild);
	this->isLeaf = false;
}

// setters + getters

template<typename T>
inline T* Node<T>::getLVal() {
	return this->l;
}

template<typename T>
inline T* Node<T>::getMVal() {
	return this->m;
}

template<typename T>
inline T* Node<T>::getRVal() {
	return this->r;
}

template<typename T>
inline Node<T>* Node<T>::getLMax() {
	return this->l_max;
}

template<typename T>
inline Node<T>* Node<T>::getMMax() {
	return this->m_max;
}

template<typename T>
inline Node<T>* Node<T>::getRMax() {
	return this->r_max;
}

template<typename T>
inline T* Node<T>::getMaxVal()
{
	return (this->r)? this->r : this->l;
}

template<typename T>
inline int Node<T>::getHeight()
{
	Node<T>* itr = this;
	if (!itr) return -1;

	int height = -1;
	while (itr) {
		++height;
		itr = itr->getLCh();
	}
	return height;
}

template<typename T>
inline int Node<T>::getNumVal() {
	int count = 0;
	if (this->isLeaf) {
		if (this->l)
			++count;
		if (this->r)
			++count;
		if (this->m)
			++count;
	}
	else {
		if (this->l_max)
			++count;
		if (this->r_max)
			++count;
		if (this->m_max)
			++count;
	}
	return count;
}

template<typename T>
inline int Node<T>::getNumCh()
{
	int count = 0;
	if (l_ch)
		++count;
	if (m1_ch)
		++count;
	if (m2_ch)
		++count;
	if (r_ch)
		++count;
	return count;
}

template<typename T>
inline std::vector<T> Node<T>::getAllVal()
{
	std::vector<T> out;
	if (this->l) out.push_back(*l);
	if (this->m) out.push_back(*m);
	if (this->r) out.push_back(*r);
	return out;
}

template<typename T>
inline std::vector<Node<T>*> Node<T>::getAllCh() {
	std::vector<Node<T>*> out;
	if (this->l_ch) out.push_back(l_ch);
	if (this->m1_ch) out.push_back(m1_ch);
	if (this->m2_ch) out.push_back(m2_ch);
	if (this->r_ch) out.push_back(r_ch);
	return out;
}

template<typename T>
inline Node<T>* Node<T>::getLCh() {
	return this->l_ch;
}

template<typename T>
inline Node<T>* Node<T>::getM1Ch() {
	return this->m1_ch;
}

template<typename T>
inline Node<T>* Node<T>::getM2Ch() {
	return this->m2_ch;
}

template<typename T>
inline Node<T>* Node<T>::getRCh() {
	return this->r_ch;
}

template<typename T>
inline Node<T>* Node<T>::getLSibling() {

	Node<T>* fatherNode = this->getFather();
	Node<T>* fathersLeftCh = fatherNode->getLCh();
	Node<T>* fathersM1Ch = fatherNode->getM1Ch();
	Node<T>* fathersM2Ch = fatherNode->getM2Ch();
	Node<T>* fathersRightCh = fatherNode->getRCh();

	if (this == fathersLeftCh) return nullptr;
	if (this == fathersM1Ch) return fathersLeftCh;
	if (this == fathersM2Ch) return fathersM1Ch;
	if (fathersM2Ch != nullptr) return fathersM2Ch;
	else if (fathersM1Ch != nullptr) return fathersM1Ch;
	else return fathersLeftCh;
}

template<typename T>
inline Node<T>* Node<T>::getRSibling() {
	Node<T>* fatherNode = this->getFather();
	Node<T>* fathersLeftCh = fatherNode->getLCh();
	Node<T>* fathersM1Ch = fatherNode->getM1Ch();
	Node<T>* fathersM2Ch = fatherNode->getM2Ch();
	Node<T>* fathersRightCh = fatherNode->getRCh();
	if (this == fathersRightCh) return nullptr;
	if (this == fathersM2Ch) return fathersRightCh;
	if (this == fathersM1Ch) {
		if (fathersM2Ch != nullptr) return fathersM2Ch;
		else return fathersRightCh;
	}
	if (fathersM1Ch != nullptr) return fathersM1Ch;
	else return fathersRightCh;
}

template<typename T>
inline Node<T>* Node<T>::getFather() {
	return this->fatherNode;
}

template<typename T>
inline Node<T>* Node<T>::getChainLeft()
{
	return this->chainLeft;
}

template<typename T>
inline Node<T>* Node<T>::getChainRight()
{
	return this->chainRight;
}

template<typename T>
inline bool Node<T>::getIsLeaf()
{
	return this->isLeaf;
}

template<typename T>
inline void Node<T>::setLVal(T* p) {
	this->l = p;
}

template<typename T>
inline void Node<T>::setMVal(T* p) {
	this->m = p;
}

template<typename T>
inline void Node<T>::setRVal(T* p) {
	this->r = p;
}

template<typename T>
inline void Node<T>::setLMax(Node* n)
{
	this->l_max = n;
}

template<typename T>
inline void Node<T>::setMMax(Node* n)
{
	this->m_max = n;
}

template<typename T>
inline void Node<T>::setRMax(Node* n)
{
	this->r_max = n;
}

template<typename T>
inline void Node<T>::ValueCopy(const Node& other)
{
	this->l = other.l;
	this->m = other.m;
	this->r = other.r;

	this->l_max = other.l_max;
	this->m_max = other.m_max;
	this->r_max = other.r_max;

	this->isLeaf = other.isLeaf;
}

template<typename T>
inline void Node<T>::setLCh(Node* n) {
	this->l_ch = n;
	if(n) n->setFather(this);
}

template<typename T>
inline void Node<T>::setM1Ch(Node* n) {
	this->m1_ch = n;
	if(n) n->setFather(this);
}

template<typename T>
inline void Node<T>::setM2Ch(Node* n) {
	this->m2_ch = n;
	if (n) n->setFather(this);
}

template<typename T>
inline void Node<T>::setRCh(Node* n) {
	this->r_ch = n;
	if (n) n->setFather(this);
}

template<typename T>
inline void Node<T>::setFather(Node<T>* n) {
	this->fatherNode = n;
}

template<typename T>
inline void Node<T>::setChainLeft(Node<T>* n)
{
	this->chainLeft = n;
	n->setChainRight(this);
}

template<typename T>
inline void Node<T>::setChainRight(Node<T>* n)
{
	this->chainRight = n;
}

template<typename T>
void Node<T>::clear() {
	this->l = nullptr;
	this->m = nullptr;
	this->r = nullptr;
	this->l_ch = nullptr;
	this->m1_ch = nullptr;
	this->m2_ch = nullptr;
	this->r_ch = nullptr;
}

template<typename T>
inline void Node<T>::removeVal(T* p)
{
	if (l != nullptr) if (*p == *l) l = nullptr;
	if (m != nullptr) if (*p == *m) m = nullptr;
	if (r != nullptr) if (*p == *r) r = nullptr;
}

template<typename T>
inline void Node<T>::removeChild(Node<T>* n)
{
	if (n == this->l_ch) this->l_ch = nullptr;
	if (n == this->r_ch) this->r_ch = nullptr;
	if (n == this->m1_ch) this->m1_ch = nullptr;
	if (n == this->m2_ch) this->m2_ch = nullptr;
}
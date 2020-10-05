#define BOOST_TEST_MODULE _AugmentedTreeTests
#include <boost/test/included/unit_test.hpp>
#include <random>
#include <algorithm>
#include "CQueue.h"
#include "Point_2D.h"

BOOST_AUTO_TEST_CASE(NodeHeight) {
	TTree<int> Tree;
	std::vector<int> list;
	for (int i = 0; i < 40; i += 2) {
		list.push_back(i + 1);
	}

	for (int i = 0; i < 20; ++i) {
		Tree.Insert(&list[i]);
	}

	int treeHeight = Tree.GetHeight();

	Node<int>* itr = Tree.GetRoot();
	itr = itr->getLCh();
	itr = itr->getRCh();
	itr = itr->getLCh();
	assert(itr->getHeight() == treeHeight - 3);
}

BOOST_AUTO_TEST_CASE(TTreeInsert)
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_real_distribution<double> distribution(-200, 200);

	TTree<int> tt;
	std::vector<int> list;
	std::vector<int> ordered_list;
	std::vector<int> check;

	for (int i = 0; i < 100; i += 2) {
		list.push_back(i + 1);
		ordered_list.push_back(i + 1);
	}

	std::shuffle(list.begin(), list.end(), g);

	for (int i = 0; i < 50; ++i) {
		tt.Insert(&list[i]);
	}

	tt.GetTreeTraversal(tt.GetRoot(), check);
	assert(check == ordered_list);

	assert(1 == 1);

	TTree<Point_2D> t2;
	std::vector<Point_2D*> point_list;
	std::vector<Point_2D> point_ordered_list;
	std::vector<Point_2D> point_check;

	for (int i = 0; i < 100; i++) {
		point_list.push_back(new Point_2D(0.0 + distribution(g), 1.0 * distribution(g)));
	}

	for (int i = 0; i < 100; i++) {
		point_ordered_list.push_back(*point_list[i]);
		t2.Insert(point_list[i]);
	}

	std::sort(point_ordered_list.begin(), point_ordered_list.end());
	t2.GetTreeTraversal(t2.GetRoot(), point_check);
	assert(point_check == point_ordered_list);

	for (int i = 0; i < 100; i++) {
		delete point_list[i];
	}
}

BOOST_AUTO_TEST_CASE(TTreeInsertDuplicates)
{
	TTree<int> tt;
	int* i = new int(5);
	for (int i = 0; i < 50; ++i) {
		tt.Insert(&i);
	}

	std::vector<int> check;
	tt.GetTreeTraversal(tt.GetRoot(), check);
	assert(check.size() == 1);

	TTree<Point_2D> t2;
	Point_2D* p = new Point_2D(5, 4);
	for (int i = 0; i < 50; ++i) {
		t2.Insert(p);
	}

	std::vector<Point_2D> check2;
	t2.GetTreeTraversal(t2.GetRoot(), check2);
	assert(check2.size() == 1);

	delete i;
	delete p;
}

BOOST_AUTO_TEST_CASE(TTreeDelete)
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_real_distribution<double> distribution(-200, 200);

	TTree<int> tt;
	std::vector<int> list;
	std::vector<int> ordered_list;

	for (int i = 0; i < 100; ++i) {
		list.push_back(i + 1);
		if (i % 2 == 0) {
			ordered_list.push_back(i + 1);
		}
	}

	std::shuffle(list.begin(), list.end(), g);

	for (int i = 0; i < 100; ++i) {
		tt.Insert(&list[i]);
	}

	for (int i = 100; i >= 0; i -= 2) {
		tt.Delete(i);
	}

	std::vector<int> check;
	tt.GetTreeTraversal(tt.GetRoot(), check);
	assert(check == ordered_list);
}

BOOST_AUTO_TEST_CASE(TTreeMinMaxNodes_LevelPointers)
{
	std::random_device rd;
	std::mt19937 g(rd());

	TTree<int> tt;
	std::vector<int> list;

	for (int i = 0; i < 100; ++i) {
		list.push_back(i + 1);
	}

	std::shuffle(list.begin(), list.end(), g);

	for (int i = 0; i < 100; ++i) {
		tt.Insert(&list[i]);
	}

	for (int i = 100; i >= 0; i -= 2) {
		tt.Delete(i);
	}

	Node<int> *min, *max, *itr, *func_minNode, * func_maxNode;
	itr = tt.GetRoot();

	while (!itr->getIsLeaf()) {
		itr = itr->getLCh();
	}
	min = itr;
	while (itr->getChainRight()) {
		itr = itr->getChainRight();
	}
	max = itr;

	func_minNode = tt.GetMinNode();
	func_maxNode = tt.GetMaxNode();

	assert(min == func_minNode && max == func_maxNode);
}

BOOST_AUTO_TEST_CASE(CQBaseTest)
{
	int* testInt = new int(3);
	Node<int>* testNode = new Node<int>(testInt);

	CQueue<int>* defConstrTestCQ = new CQueue<int>();
	CQueue<int>* singleValConstrTestCQ = new CQueue<int>(testInt);
	CQueue<int>* fullConstrTestCQ = new CQueue<int>(testNode, 0, testNode, testNode);
	
	delete defConstrTestCQ;
	delete singleValConstrTestCQ;
	delete fullConstrTestCQ;
	delete testInt;
}

BOOST_AUTO_TEST_CASE(CQSplitFuncTest)
{
	CQueue<int>* Cq = new CQueue<int>();
	std::vector<int> v = { 2,4,5,6,7,9,10,11,12,15,16,20,25 };
	for (std::size_t i = 0; i < v.size(); ++i) {
		Cq->Insert(&v[i]);
	}

//
//	/*segment dividers - wont have to travel all the way to the leaf to know where is it*/
//	Cq->Split(4, true);
//	Cq->Split(9, true);
//	Cq->Split(15, true);
//	Cq->Split(6, true);
//	Cq->Split(11, true);
//
//	/* x not present in tree */
//	Cq->Split(3,true);
//	// must find x = 2
//	Cq->Split(8,true);
//	// must split at x = 7
//	Cq->Split(13,true);
//	// must split at x = 12
//	Cq->Split(17, true);
//	Cq->Split(21, true);
//
//	/* x present in tree not a segment divider - travel all the way to the leaf */
//	Cq->Split(2, true);
//	Cq->Split(12, true);
//	Cq->Split(16, true);
//	Cq->Split(20, true);
//	Cq->Split(25, true);
//
}
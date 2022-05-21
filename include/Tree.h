#ifndef TREE_BASE
#define TREE_BASE
#include "struct.h"
#include "MeTPerson.h"
#include <stdio.h>
/*
  key per node: [M-1, 2M-1]
  child per node: [M, 2M]
 */

//class MeTPerson;

class Tree{
protected:	
	btree_nodes *roots;
	
	FILE *pfile;
	
	virtual btree_nodes *btree_create()=0; // create root

	virtual btree_nodes *btree_nodes_new()=0; // allocate a new btree node

	virtual int btree_split_child(btree_nodes *parent, int pos, btree_nodes *child)=0; // split child if num of key in child exceed 2M-1

	virtual void btree_insert_nonfull(btree_nodes *node, MeTPerson *target)=0;

	virtual void btree_merge_child(btree_nodes *root, int pos, btree_nodes *y, btree_nodes *z)=0;

	virtual void btree_delete_nonone(btree_nodes *root, MeTPerson *target)=0;

	virtual MeTPerson * btree_search_successor(btree_nodes *root)=0;

	virtual MeTPerson * btree_search_predecessor(btree_nodes *root)=0;

	virtual void btree_shift_to_left_child(btree_nodes *root, int pos, btree_nodes *y, btree_nodes *z)=0;

	virtual void btree_shift_to_right_child(btree_nodes *root, int pos, btree_nodes *y, btree_nodes *z)=0;

	virtual btree_nodes* btree_insert(btree_nodes *root, MeTPerson *target)=0;

	virtual btree_nodes *btree_delete(btree_nodes *root, MeTPerson *target)=0;

	virtual void btree_inorder_print(btree_nodes *root)=0;

	virtual void btree_level_display(btree_nodes *root)=0;

  	virtual void Save(btree_nodes *root)=0;
  	
  	int btree_nodes_num;  // number of btree_nodes

public:

	Tree(void){
		btree_nodes_num=0;
	};

	virtual ~Tree(void){
		btree_nodes_num=0;
		delete roots;
	};

	void insert(MeTPerson *target){
		roots = btree_insert(roots, target);
		Save(roots); 
	};

	void level_display(){
		btree_level_display(roots);
	};
	
	void del(MeTPerson *target){
		roots = btree_delete(roots, target);

		Save(roots); 
	};
	
	void inorder_print(){
		btree_inorder_print(roots);
	};
	
	void NodeNum_print(){
		printf("%d\n", btree_nodes_num);
	};
};

#endif
#ifndef TREE_BASE
#define TREE_BASE
#include "struct.h"
#include "MeTPerson.h"
#include <stdio.h>
/*
  key per node: [M-1, 2M-1]
  child per node: [M, 2M]
 */

class Tree{
protected:	
	btree_node *roots;
	
	FILE *pfile;
	
	virtual btree_node *btree_create()=0; // create root

	virtual btree_node *btree_node_new()=0; // allocate a new btree node

	virtual int btree_split_child(btree_node *parent, int pos, btree_node *child)=0; // split child if num of key in child exceed 2M-1

	virtual void btree_insert_nonfull(btree_node *node, MeTPerson *target)=0;

	virtual void btree_merge_child(btree_node *root, int pos, btree_node *y, btree_node *z)=0;

	virtual void btree_delete_nonone(btree_node *root, MeTPerson *target)=0;

	virtual MeTPerson * btree_search_successor(btree_node *root)=0;

	virtual MeTPerson * btree_search_predecessor(btree_node *root)=0;

	virtual void btree_shift_to_left_child(btree_node *root, int pos, btree_node *y, btree_node *z)=0;

	virtual void btree_shift_to_right_child(btree_node *root, int pos, btree_node *y, btree_node *z)=0;

	virtual btree_node* btree_insert(btree_node *root, MeTPerson *target)=0;

	virtual btree_node *btree_delete(btree_node *root, MeTPerson *target)=0;

	virtual void btree_inorder_print(btree_node *root)=0;

	virtual void btree_level_display(btree_node *root)=0;

  	virtual void Save(btree_node *root)=0;
  	
  	int btree_node_num;  // number of btree_node

public:

	Tree(void){
		btree_node_num=0;
	};

	virtual ~Tree(void){
		btree_node_num=0;
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
		printf("%d\n", btree_node_num);
	};
};

#endif
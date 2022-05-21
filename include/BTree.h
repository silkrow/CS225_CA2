#ifndef B_TREE
#define B_TREE
#include "struct.h"
#include "Tree.h"
//#include "MeTPerson.h"
#include "PersonDB.h"

class MeTPerson;

class BTree :
	public Tree
{
protected:
	virtual btree_nodes* btree_create();
	virtual btree_nodes* btree_nodes_new();
	virtual int btree_split_child(btree_nodes *parent, int pos, btree_nodes *child);
	virtual void btree_insert_nonfull(btree_nodes *node, MeTPerson *target);
	virtual void btree_merge_child(btree_nodes *root, int pos, btree_nodes *y, btree_nodes *z);
	virtual void btree_delete_nonone(btree_nodes *root, MeTPerson *target);
	virtual MeTPerson * btree_search_successor(btree_nodes *root);
	virtual MeTPerson * btree_search_predecessor(btree_nodes *root);
	virtual void btree_shift_to_left_child(btree_nodes *root, int pos, btree_nodes *y, btree_nodes *z);	
	virtual void btree_shift_to_right_child(btree_nodes *root, int pos, btree_nodes *y, btree_nodes *z);
	virtual btree_nodes* btree_insert(btree_nodes *root, MeTPerson *target);
	virtual btree_nodes *btree_delete(btree_nodes *root, MeTPerson *target);
	virtual void btree_inorder_print(btree_nodes *root);
	virtual void btree_level_display(btree_nodes *root);
	virtual void Save(btree_nodes *root);
public:
	BTree(void);
	~BTree(void);	
};

#endif
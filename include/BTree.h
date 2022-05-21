#ifndef B_TREE
#define B_TREE
#include "struct.h"
#include "Tree.h"
#include "MeTPerson.h"
#include "PersonDB.h"

class BTree :
	public Tree
{
protected:
	virtual btree_node* btree_create();
	virtual btree_node* btree_node_new();
	virtual int btree_split_child(btree_node *parent, int pos, btree_node *child);
	virtual void btree_insert_nonfull(btree_node *node, MeTPerson *target);
	virtual void btree_merge_child(btree_node *root, int pos, btree_node *y, btree_node *z);
	virtual void btree_delete_nonone(btree_node *root, MeTPerson *target);
	virtual MeTPerson * btree_search_successor(btree_node *root);
	virtual MeTPerson * btree_search_predecessor(btree_node *root);
	virtual void btree_shift_to_left_child(btree_node *root, int pos, btree_node *y, btree_node *z);	
	virtual void btree_shift_to_right_child(btree_node *root, int pos, btree_node *y, btree_node *z);
	virtual btree_node* btree_insert(btree_node *root, MeTPerson *target);
	virtual btree_node *btree_delete(btree_node *root, MeTPerson *target);
	virtual void btree_inorder_print(btree_node *root);
	virtual void btree_level_display(btree_node *root);
	virtual void Save(btree_node *root);
public:
	BTree(void);
	~BTree(void);	
};

#endif
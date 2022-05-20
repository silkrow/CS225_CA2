#ifndef STRU_TREE
#define STRU_TREE

#define M 2 // degree

// Btree's node
#include "MeTPerson.h"
#include "IntNode.h"

typedef struct btree_nodes{
	MeTPerson *k[2 * M - 1]; // METsystem
	struct btree_nodes *p[2 * M];
	int num; // total number of children
	bool is_leaf; // 1-leaf
	IntNode* block; // if it is leaf, its corresponding block
	struct btree_nodes *prev;  // use one struct just for simple, B+Tree used
	struct btree_nodes *next;  // for B+Tree 
	
} btree_node;

// !!
typedef struct StorageNode{
	btree_node bnode;
	int index[M];   // index_set 
}storage_node;

// !!
typedef struct StorageStruct{
	storage_node *snode;
	int len;
}storage_struct;

#endif
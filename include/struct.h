#ifndef STRU_TREE
#define STRU_TREE

#define M 2 // degree

// Btree's node
typedef struct btree_nodes{
	int k[2 * M - 1];
	struct btree_nodes *p[2 * M];
	int num;
	bool is_leaf;
	struct btree_nodes *prev;  // use one struct just for simple,��2������B+Tree used
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
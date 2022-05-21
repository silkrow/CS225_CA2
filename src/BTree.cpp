#include "BTree.h"
#include "struct.h"
#include "PersonDB.h"
#include "MeTPerson.h"
#include <stdio.h>
#include <stdlib.h>


btree_nodes* BTree::btree_nodes_new(){ // create a b-tree node
	btree_nodes* node = (btree_nodes *)malloc(sizeof(btree_nodes));
	if(NULL == node) return NULL; // fail to malloc
	for(int i = 0; i < 2 * M - 1; i++){
		node->k[i] = NULL;
	} // initialize keys

	for(int i = 0; i < 2 * M; i++){
		node->p[i] = NULL;
	} // initialize p

	node->num = 0;
	node->is_leaf = true;     
	return node;
}

btree_nodes* BTree::btree_create(){ // create a b-tree
	btree_nodes *node = btree_nodes_new(); // create the root
	if(NULL == node) return NULL; // fail to malloc
	return node; // return root
}

int BTree::btree_split_child(btree_nodes *parent, int pos, btree_nodes *child){
	btree_nodes *new_child = btree_nodes_new();
	if(NULL == new_child) return -1; // fail to malloc

	new_child->is_leaf = child->is_leaf;
	new_child->num = M - 1; // key per node: [M-1, 2M-1]

	for(int i = 0; i < M - 1; i++) {
		new_child->k[i] = child->k[i+M]; // copy the last few nodes
	}

	if(false == new_child->is_leaf) { // not leaf -- has child
		for(int i = 0; i < M; i++) {
			new_child->p[i] = child->p[i+M];
		}
	}

	child->num = M - 1; // split

	// insert the new child to parent
	for(int i = parent->num; i > pos; i--) {
		parent->p[i + 1] = parent->p[i];
	}
	parent->p[pos + 1] = new_child;

	// update the keys of parent
	for(int i = parent->num - 1; i >= pos; i--) {
		parent->k[i+1] = parent->k[i];
	}
	parent->k[pos] = child->k[M-1];

	parent->num += 1;
	
	return 0; // successfully
}

void BTree::btree_insert_nonfull(btree_nodes *node, MeTPerson *target){
	if(1 == node->is_leaf){ // insert now
		int pos = node->num; // add new child to node
		//while(pos >= 1 && target < node->k[pos-1]) {
		while(pos >= 1 && target->cmpBTree(node->k[pos - 1]) == 0) {	
			node->k[pos] = node->k[pos-1];
			pos--;
		}

		node->k[pos] = target;
		node->num += 1;
		btree_nodes_num += 1;
		return;
		
	} else {
		int pos = node->num;
		while(pos > 0 && target->cmpBTree(node->k[pos - 1]) == 0) {
			pos--;
		}

		if(2 * M - 1 == node->p[pos]->num) {
			btree_split_child(node, pos, node->p[pos]);
			//if(target > node->k[pos]) {
			if(1 == target->cmpBTree(node->k[pos])){
				pos++;
			}
		}

		btree_insert_nonfull(node->p[pos], target);
	}
}

btree_nodes* BTree::btree_insert(btree_nodes *root, MeTPerson *target){
	if(NULL == root) return NULL;

	if(2 * M - 1 == root->num) { // root is full
		btree_nodes* node = btree_nodes_new();
		if(NULL == node) {
			return root;
		}
		node->is_leaf = 0;
		node->p[0] = root; // node is the new root
		btree_split_child(node, 0, root);
		btree_insert_nonfull(node, target);
		return node; // new root
	} else {
		btree_insert_nonfull(root, target);    
		return root;
	}
}

void BTree::btree_merge_child(btree_nodes *root, int pos, btree_nodes *y, btree_nodes *z){
	y->num = 2 * M - 1;
	for(int i = M; i < 2 * M - 1; i++) {
		y->k[i] = z->k[i-M];
	}
	y->k[M-1] = root->k[pos];

	if(false == z->is_leaf) {
		for(int i = M; i < 2 * M; i++) {
			y->p[i] = z->p[i-M];
		}
	}
	for(int j = pos + 1; j < root->num; j++) {
		root->k[j-1] = root->k[j];
		root->p[j] = root->p[j+1];
	}

	root->num -= 1;
	free(z);
}


btree_nodes* BTree::btree_delete(btree_nodes* root, MeTPerson *target){
	if(1 == root->num) {
		btree_nodes *y = root->p[0];
		btree_nodes *z = root->p[1];
		if(NULL != y && NULL != z && M - 1 == y->num && M - 1 == z->num) {
			btree_merge_child(root, 0, y, z);
			free(root);
			btree_delete_nonone(y, target);
			return y;
		} else {
			btree_delete_nonone(root, target);
			return root;
		}
	} else {
		btree_delete_nonone(root, target);	
		return root;
	}
}

void BTree::btree_delete_nonone(btree_nodes *root, MeTPerson *target){
	if(true == root->is_leaf) {
		int i = 0;
		//while(i < root->num && target > root->k[i]) i++;
		while(i < root->num && target->cmpBTree(root->k[i])) i++;
		if(target == root->k[i]){
			for(int j = i + 1; j < 2 * M - 1; j++) {
				root->k[j-1] = root->k[j];
			}
			root->num -= 1;
			
			btree_nodes_num-=1;
			
		} else{ // cannot find even in leaf
			printf("target not found\n");
		}
	} else {
		int i = 0;
		btree_nodes *y = NULL, *z = NULL;
		while(i < root->num && target->cmpBTree(root->k[i])) i++;
		if(i < root->num && target == root->k[i]) {
			y = root->p[i];
			z = root->p[i+1];
			if(y->num > M - 1) {
				MeTPerson *pre = btree_search_predecessor(y);
				root->k[i] = pre;
				btree_delete_nonone(y, pre);
			} else if(z->num > M - 1) {
				MeTPerson *nxt = btree_search_successor(z);
				root->k[i] = nxt;
				btree_delete_nonone(z, nxt);
			} else {
				btree_merge_child(root, i, y, z);
				btree_delete(y, target);
			}
		} else {
			y = root->p[i];
			if(i < root->num) {
				z = root->p[i+1];
			}
			btree_nodes *p = NULL;
			if(i > 0) {
				p = root->p[i-1];
			}

			if(y->num == M - 1) {
				if(i > 0 && p->num > M - 1) {
					btree_shift_to_right_child(root, i-1, p, y);
				} else if(i < root->num && z->num > M - 1) {
					btree_shift_to_left_child(root, i, y, z);
				} else if(i > 0) {
					btree_merge_child(root, i-1, p, y); // note
					y = p;
				} else {
					btree_merge_child(root, i, y, z);
				}
				btree_delete_nonone(y, target);
			} else {
				btree_delete_nonone(y, target);
			}
		}

	}
}

MeTPerson * BTree::btree_search_predecessor(btree_nodes *root){
	btree_nodes *y = root;
	while(false == y->is_leaf) {
		y = y->p[y->num];
	}
	return y->k[y->num-1];
}

MeTPerson * BTree::btree_search_successor(btree_nodes *root){
	btree_nodes *z = root;
	while(false == z->is_leaf) {
		z = z->p[0];
	}
	return z->k[0];
}

void BTree::btree_shift_to_right_child(btree_nodes *root, int pos, btree_nodes *y, btree_nodes *z){
	z->num += 1;
	for(int i = z->num -1; i > 0; i--) {
		z->k[i] = z->k[i-1];
	}
	z->k[0]= root->k[pos];
	root->k[pos] = y->k[y->num-1];

	if(false == z->is_leaf) {
		for(int i = z->num; i > 0; i--) {
			z->p[i] = z->p[i-1];
		}
		z->p[0] = y->p[y->num];
	}

	y->num -= 1;
}

void BTree::btree_shift_to_left_child(btree_nodes *root, int pos, btree_nodes *y, btree_nodes *z){
	y->num += 1;
	y->k[y->num-1] = root->k[pos];
	root->k[pos] = z->k[0];

	for(int j = 1; j < z->num; j++){
		z->k[j-1] = z->k[j];
	}

	if(false == z->is_leaf){
		y->p[y->num] = z->p[0];
		for(int j = 1; j <= z->num; j++){
			z->p[j-1] = z->p[j];
		}
	} 

	z->num -= 1;
}

void BTree::btree_inorder_print(btree_nodes *root) {
	if(NULL != root) {
		btree_inorder_print(root->p[0]);
		for(int i = 0; i < root->num; i++) {
			printf("%d ", root->k[i]->pid);
			btree_inorder_print(root->p[i+1]);
		}
	}
}

void BTree::btree_level_display(btree_nodes *root) {
	// just for simplicty, can't exceed 200 nodes in the tree
	btree_nodes *queue[200] = {NULL};
	int front = 0;
	int rear = 0;

	queue[rear++] = root;

	while(front < rear) {
		btree_nodes *node = queue[front++];

		printf("[");
		for(int i = 0; i < node->num; i++) {
			printf("%d ", node->k[i]->pid);
		}
		printf("]");

		for(int i = 0; i <= node->num; i++) {
			if(NULL != node->p[i]) {
				queue[rear++] = node->p[i];               
			}
		}
	}
	printf("\n");
}

void BTree::Save(btree_nodes *root) {
	
}

BTree::BTree(void){	
	roots = btree_create();
}

BTree::~BTree(void){
}

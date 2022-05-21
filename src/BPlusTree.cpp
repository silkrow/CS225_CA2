#include "BPlusTree.h"
#include "PersonDB.h"
#include "BPNode.h"
#include "MeTPerson.h"
#include <iostream>
#include <vector>
#include <queue>
using namespace std;



/*
*    @brief Find the location of given _key in given _node. 
*    @param _node: Given node
*    @param _key: Key we want to locate in _node
*    @return Index of _key in _node. If _key is not in _node, return the nearest and smaller index
*/
//template<typename MeTPerson*, typename MeTPerson*>
inline int BPTree::keyIndex(Node *tmp, MeTPerson* key){
    int loc = -1;
    int size = tmp->key.size();
    /*for(int i = 0; i < size; i++){
        if(key == tmp->key[i]) return i;
    }*/
    //while(tmp->key[loc + 1] <= key){
    while(key->cmpMeTPer(tmp->key[loc + 1]) == 1){ 
        loc++;
        if(loc == size - 1) break;
    }
    return loc;
}

/*
*    @brief Find the location of given _key in leaf node. 
*    @param _key: Key we want to locate.
*    @return A pair of leaf and index of given _key. If _key not in B+ tree, the index is the nearest and smaller key than given _key.
*/
//template<typename MeTPerson*, typename MeTPerson*>
inline pair<Node*, int> BPTree::keyIndexInLeaf(MeTPerson* key){
    if(root == NULL){
        Node* ret = NULL;
        return make_pair(ret, 0);
    }
    Node *node = root;
    while(true){ 
        int loc = keyIndex(node, key);
        if(node->leaf){ 
            return make_pair(node, loc);
        } else{ 
            node = node->ptr2node[loc + 1];
        }
    }
}

/*
*    @brief Find the location of given _key in given _node. 
*    @param _node: Given node
*    @param _key: Key we want to locate in _node
*    @return Index of _key in _node. If _key is not in _node, return the nearest and smaller index
*/
//template<typename MeTPerson*, typename MeTPerson*>
inline int BPTree::eqkeyIndex(Node *tmp, MeTPerson* key){
    int size = tmp->key.size();
    for(int i = 0; i < size; i++){
        if(key == tmp->key[i]) return i;
    }
    return -1;
}

/*
*    @brief Find the location of given _key in leaf node. 
*    @param _key: Key we want to locate.
*    @return A pair of leaf and index of given _key. If _key not in B+ tree, the index is the nearest and smaller key than given _key.
*/
//template<typename MeTPerson*, typename MeTPerson*>
inline pair<Node*, int> BPTree::eqkeyIndexInLeaf(MeTPerson* key){
    if(root == NULL){
        Node* ret = NULL;
        return make_pair(ret, 0);
    }
    Node *node = root;
    while(true){ 
        int loc = keyIndex(node, key);
        if(node->leaf){
            return make_pair(node, loc);
        } else{
            node = node->ptr2node[loc + 1];
        }
    }
}

/*
*    @brief Split leaf node when oversize.
*    @param _leaf: Leaf we want to split.
*    @return The new leaf we created after split.
*/
//template<typename MeTPerson*, typename MeTPerson*>
Node* BPTree::splitLeaf(Node* _leaf){
    Node *new_leaf = new Node(LEAF);
    new_leaf->next = _leaf->next;

    if (new_leaf->next != NULL) new_leaf->next->last = new_leaf;
    _leaf->next = new_leaf;
    new_leaf->last = _leaf;
    new_leaf->parent = _leaf->parent;
    int mid = _leaf->key.size() / 2;
    for(int i = mid; i < _leaf->key.size();i++){
        new_leaf->key.push_back(_leaf->key[i]);
    }
    //new_leaf->key.assign(_leaf->key.begin() + mid, _leaf->key.end());
    //new_leaf->ptr2val.assign(_leaf->ptr2val.begin() + mid, _leaf->ptr2val.end());
    for(int i = mid; i < _leaf->ptr2val.size();i++){
        new_leaf->ptr2val.push_back(_leaf->ptr2val[i]);
    }
    _leaf->key.erase(_leaf->key.begin() + mid, _leaf->key.end());
    _leaf->ptr2val.erase(_leaf->ptr2val.begin() + mid, _leaf->ptr2val.end());
    return new_leaf;
}

/*
*    @brief Split non-leaf node when oversize.
*    @param _node: Node we want to split.
*    @return The new node we created after split.
*/
//template<typename MeTPerson*, typename MeTPerson*>
pair<Node*, MeTPerson*> BPTree::splitNode(Node* _node){
    Node *new_node = new Node();
    new_node->parent = _node->parent;
    int mid = (_node->key.size() + 1) / 2 - 1;
    MeTPerson* push_key = _node->key[mid];
    for(int i = mid + 1; i < _node->key.size();i++){
        new_node->key.push_back(_node->key[i]);
    }
    //new_node->key.assign(_node->key.begin() + mid + 1, _node->key.end());
    //new_node->ptr2node.assign(_node->ptr2node.begin() + mid + 1, _node->ptr2node.end());
    for(int i = mid + 1; i < _node->ptr2node.size();i++){
        new_node->ptr2node.push_back(_node->ptr2node[i]);
    }
    _node->key.erase(_node->key.begin() + mid, _node->key.end());
    _node->ptr2node.erase(_node->ptr2node.begin() + mid + 1, _node->ptr2node.end());
    //for(Node* each : new_node->ptr2node)
    //    each->parent = new_node;
    for(int i = 0; i < new_node->ptr2node.size(); i++)
        new_node->ptr2node[i]->parent = new_node;
    return make_pair(new_node, push_key);
}

/*
*    @brief Create index for given _new_node using _index as index. The index will be inserted to _new_node's parent.
*    @param _new_node: Node we want to create index for.
*    @param _index: Index of our new node. For leaf node, it should be the first key.
*    @return void
*/
//template<typename MeTPerson*, typename MeTPerson*>
void BPTree::createIndex(Node* _new_node, MeTPerson* _index){
    Node *node = _new_node->parent;
    int loc = keyIndex(node, _index);
    node->key.insert(node->key.begin() + loc + 1, _index);
    node->ptr2node.insert(node->ptr2node.begin() + loc + 2, _new_node);
    if(node->key.size() > order){
        pair<Node*, MeTPerson*> pair = splitNode(node);
        Node *new_node = pair.first;
        MeTPerson* push_key = pair.second;
        if(node == root){
            Node *new_root = new Node();
            new_root->key.push_back(push_key);
            new_root->ptr2node.push_back(node);
            new_root->ptr2node.push_back(new_node);
            root = new_root;
            node->parent = root;
            new_node->parent = root;
        } else{
            createIndex(new_node, push_key);
        }
    }
}

Node::Node(bool _leaf) : leaf(_leaf), parent(nullptr), next(nullptr), last(nullptr) {}

BPTree::BPTree() : root(nullptr), sz(0) {}

/*
*    @brief Insert (key, value) to B+ tree
*    @param _key: Key we want to insert
*    @param _val: Value we want to insert
*    @return void
*/
//template<typename MeTPerson*, typename MeTPerson*>
void BPTree::insert(MeTPerson* _key){ 
    if(root == nullptr){ 
        root = new Node(LEAF);
        root->key.push_back(_key);
        root->ptr2val.push_back(_key);
        //root->ptr2val.emplace_back(new MeTPerson*(_val));
        root->ptr2node.push_back(nullptr);
        /*for DB*/ 
        PDB = new PerDB();
        //PDB->insertPer(_key, PDB->topNode); 
        PDB->topNode->insertB(_key);
        sz++;
        return;
    }
    pair<Node*, int> pair = keyIndexInLeaf(_key);
    Node *leaf = pair.first;
    int loc = pair.second;
    if(loc != -1 && leaf->key[loc] == _key){
        //cout << "Key " << _key << " with value " << *(leaf->ptr2val[loc]) << " is already in B+ tree, overwrite it with new val " << _val << endl;
        //*(leaf->ptr2val[loc]) = _key;
        return;
    }
    sz++;
    leaf->key.insert(leaf->key.begin() + loc + 1, _key);     leaf->ptr2val.insert(leaf->ptr2val.begin() + loc + 1, _key);
    // for DB
    // !! tree cannot be empty
    // which means at least one node else in the leafs 
    if(-1 != loc){
        //PDB->insertPer(_key, leaf->key[loc]->dbblock);
        leaf->key[loc]->dbblock->insertB(_key);
    }
    else{ 
        int endk = leaf->key.size();
        if(loc != endk - 2){ 
            //PDB->insertPer(_key, leaf->key[loc + 2]->dbblock); 
            leaf->key[loc + 2]->dbblock->insertB(_key); 
            //PDB->topNode->insertB(_key);
        }
        else if(NULL != leaf->next){ 
            Node* llf = leaf->next;
            bool flag = 1;
            while(llf->key.size() == 0){ 
                if(NULL == llf->next){
                    flag = 0;
                    break;
                }
                llf = llf->next;
            }
            //if(flag) PDB->insertPer(_key, llf->key[0]->dbblock);
            if(flag) llf->key[0]->dbblock->insertB(_key);
        }
        else{ 
            Node* llf = leaf->last;
            bool flag = 1;
            while(llf->key.size() == 0){ 
                if(NULL == llf->last){
                    flag = 0;
                    break;
                }
                llf = llf->last;
            }
            if(flag){ 
                int ll =  leaf->last->key.size() - 1;
                //PDB->insertPer(_key, leaf->last->key[ll]->dbblock);
                leaf->last->key[ll]->dbblock->insertB(_key);
            }
        }
    } 
    if(leaf->key.size() > order){   
        Node *new_leaf = splitLeaf(leaf); 
        if(leaf == root){
            Node *new_root = new Node();
            new_root->key.push_back(new_leaf->key[0]); 
            new_root->ptr2node.push_back(leaf);
            new_root->ptr2node.push_back(new_leaf);
            root = new_root;
            leaf->parent = root;
            new_leaf->parent = root;
        } else{  
            createIndex(new_leaf, new_leaf->key[0]);
        }
    } 
}

/*
*    @brief Delete _key from B+ tree
*    @param _key: Key we want to delete
*    @return void
*/
//template<typename MeTPerson*, typename MeTPerson*>
void BPTree::erase(MeTPerson* _key){
    pair<Node*, int> pair = eqkeyIndexInLeaf(_key);
    Node *leaf = pair.first;
    int loc = pair.second;
    if(loc == -1){
        printf("no such key in btree\n");
        return;
    }
    sz--;
    PDB->deletePer(_key, _key->dbblock);
    for(int i = loc; i < leaf->key.size() - 1; i++){
        leaf->key[i] = leaf->key[i + 1];
        leaf->ptr2val[i] = leaf->ptr2val[i + 1];
    }
    return;
}

/*
*    @brief Find the value ptr of given key in B+ tree
*    @param _key: Key we want to find
*    @return A ptr to value. If key is not in B+ tree then return nullptr
*/
//template<typename MeTPerson*, typename MeTPerson*>
MeTPerson* BPTree::find(MeTPerson* _key){
    pair<Node*, int> pair = keyIndexInLeaf(_key);
    Node *leaf = pair.first;
    int loc = pair.second;
    if(loc == -1 || leaf->key[loc] != _key){
        //std::cout << "Key " << _key << " is not in B+ tree" << std::endl;
        return nullptr;
    } else{
        return leaf->ptr2val[loc];
    }
}

MeTPerson* BPTree::poptop(int num){
    return this->PDB->popTopn(num);
}


//template<typename MeTPerson*, typename MeTPerson*>
void BPTree::display(){
    if(root == nullptr){
        cout << "B+ tree is empty!" << endl;
        return;
    }
    queue<Node*> q;
    q.push(root);
	int layer = 0;
    while(!q.empty()){
		cout << "layer " << layer++ <<endl;
        int q_size = q.size();
        while(q_size--){
            Node *node = q.front();
            q.pop();
            int key_size = node->key.size();
            if(node->leaf){
               // for(auto each : node->ptr2val)
               //    cout << *each << " ";
               for(int i = 0; i < node->ptr2val.size(); i++){
                   printf("%d ",node->ptr2val[i]->pid);
               }
            } else{
                /*for(auto each : node->key)
                  cout << each << " ";
                for(auto each : node->ptr2node)
                    q.push(each);*/
                for(int i = 0; i < key_size; i++){
                    printf("%d ",node->key[i]->pid);}
				for(int i = 0; i < node->ptr2node.size();i++){
                    q.push(node->ptr2node[i]);
                }
            }
            cout << "| ";
        }
        cout << endl;
    }
}

//template<typename MeTPerson*, typename MeTPerson*>
/*void BPTree::scan(){
    if(root == nullptr){
        cout << "B+ tree is empty!" << endl;
        return;
    }
    Node *node = root;
    while(!node->leaf){
        node = node->ptr2node[0];
    }
    while(node != nullptr){
        for(auto each : node->ptr2val)
            cout << *each << " ";
        node = node->next;
    }
    cout << endl;
}
*/

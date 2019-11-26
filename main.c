/*Designed and programmed by - Cem Gulec - 150117828
 *
 * Constructed tree:
 *                18
 *         /              \
 *       12                36
 *      /  \              /  \
 *     8    16          24    72
 *    /    /  \        /     /
 *   4   14    17     20    54
 *  /     \             \
 * 2       15            21
 *
 * */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct bst_tree {
    int data;
    int left_size;
    int right_size;
    int total_size;
    int depth;
    int topology_num;
    int ord;
    struct bst_tree *left, *right;
};
typedef struct bst_tree node;

node * root = NULL;
int numArray[20], i=0, num, decision;
FILE *fptr;

/*prototypes*/
node * insert(node *, int);
node * delete(node *, int);
void print_lnr(node *);
node * getMin(node *);
node * getMax(node *);
int getMaxDepth(node *);
int getSize(node *);
int getDifTopology(int);
int calculateCatalan(int);
int binomialCoeF(int,int);
unsigned int factorial(unsigned int);
node * traceNode (node *, int );
int checkTerm(int);
int checkDepth(node *, int, int);
void deleteByTheUserEntered();
void tracePathByTheUserEntered();
void splitString(char []);
void assignDepths(node *);
void assignSize(node *);
void assignSizeLR(node *);
void assignTopology(node *);
void assignOrd(node *);

void main()
{
    char str[200];
    printf("Enter your sequence of numbers: ");
    gets(str);
    splitString(str);

    /* Printing nodes of tree in LNR fashion */
    assignDepths(root);
    assignSize(root);
    assignSizeLR(root);
    assignTopology(root);
    assignOrd(root);
    printf("LNR display\n");
    print_lnr(root);
    printf("\nmax depth level: %d\n",getMaxDepth(root));
    printf("\nsize of the root: %d\n",getSize(traceNode(root,36)));
    printf("\ndepth level: %d\n",checkDepth(root,36,1));
    printf("\nPress (1) to delete a node\nPress(2) to trace path of a nodepress\n(-1) to exit\nDecision: ");
    scanf("%d",&decision);

    while(decision != -1){
        switch (decision){
            case 1:
                deleteByTheUserEntered();
                break;
            case 2:
                tracePathByTheUserEntered();
                break;
            case -1:
                exit(-1);
        }

        printf("\nPress (1) to delete a node\nPress(2) to trace path of a nodepress\n(-1) to exit\nDecision: ");
        scanf("%d",&decision);

    }
    fclose(fptr);
}

//inserting a new node into tree
node * insert(node * tree, int val){
    //creating a node in order to place the value
    if( tree == NULL ){
        node *tmp = (node *)malloc(sizeof(node));
        tmp->data = val;
        numArray[i++]=val;
        tmp->left = tmp->right = NULL;
        tree = tmp;
        tmp = NULL;
    }
        //looking for the right place to put node, iterating through its left, middle and right branches
    else if( val < tree->data ){
        tree->left = insert(tree->left,val);
    }
    else if( val >= tree->data ){
            tree->right =  insert(tree->right,val);

    }else{}

    return tree;
}

//deleting a node
node * delete(node * tree, int val){
    // base case
    if (tree == NULL) return tree;

    // If the key to be deleted is smaller than the root's key,
    // then it lies in left subtree
    if (val < tree->data)
        tree->left = delete(tree->left, val);

        // If the key to be deleted is greater than the root's key,
        // then it lies in right subtree
    else if (val > tree->data)
        tree->right = delete(tree->right, val);

        // if key is same as root's key, then This is the node
        // to be deleted
    else
    {
        // node with only one child or no child
        if (tree->left == NULL)
        {
            node *temp = tree->right;
            free(tree);
            return temp;
        }
        else if (tree->right == NULL)
        {
            node *temp = tree->left;
            free(tree);
            return temp;
        }

        // node with two children: Get the inorder successor (smallest
        // in the right subtree)
        node* temp = getMin(tree->right);

        // Copy the inorder successor's content to this node
        tree->data = temp->data;

        // Delete the inorder successor
        tree->right = delete(tree->right, temp->data);
    }
    return tree;
}

//print all the nodes in LNR fashion
void print_lnr(node * tree){
    if (tree){
        print_lnr(tree->left);
        printf("%d - d_l:%d - #left:%d - #right:%d - #total_size:%d - #topology:%d - ord:%d\n",tree->data, tree->depth,
                tree->left_size, tree->right_size, tree->total_size, tree->topology_num, tree->ord);
        print_lnr(tree->right);
    }
}

//get left most node by iterating recursively
node * getMin(node *tree){
    node *currentNode = tree;
    while (currentNode->left != NULL)
        currentNode = currentNode->left;

    return currentNode;
}

//get right most node by iterating recursively
node * getMax(node *tree){
    node *currentNode = tree;
    while (currentNode->right != NULL)
        currentNode = currentNode->right;

    return currentNode;
}

//a function to find out max depth level
int getMaxDepth(node *tree){
    if (tree==NULL)
        return 0;
    else
    {
        /* compute the depth of each subtree */
        int left_depth = getMaxDepth(tree->left);
        int right_depth = getMaxDepth(tree->right);

        /* use the larger one */
        if (left_depth > right_depth)
            return(left_depth+1);
        else return(right_depth+1);
    }
}

//a function to learn size of a node
int getSize(node * tree){
    if (tree==NULL)
        return 0;
    else
    {
        int left_size = getSize(tree->left);
        int right_size = getSize(tree->right);
        return (left_size + right_size +1);
    }
}

//a function to calculate number of topologies of a given node
int getDifTopology(int node_size){
    //node *tmp = traceNode();
    int count = calculateCatalan(node_size);

    return count;


}

int calculateCatalan(int node_size){
    double c = binomialCoeF(2*node_size, node_size);
    double temp = c / (node_size+1);

    return floor(temp);
}

int binomialCoeF(int node_size, int k){
    int index, res = 1;

    //C(n, k) = C(n, n-k)
    if (k > (node_size-k))
        k = node_size-k;

    // Calculate value of [n*(n-1)*---*(n-k+1)] /
    // [k*(k-1)*---*1]
    for (index=0; index<k; index++){
        res *= (node_size-index);
        res = floor(res/(index+1));
    }

    return res;
}

unsigned int factorial(unsigned int n){
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}

//trace a node and return it
node * traceNode (node *tree, int val){
    if (tree == NULL || tree->data == val)
        return tree;

    if (tree->data < val)
        return traceNode(tree->right, val);

    return traceNode(tree->left, val);
}

//this function checks if the term is exist
int checkTerm(int val){

    int i;

    for(i=0; i<20; i++){
        if(numArray[i] == val)
            return 1;
    }
    return 0;
}

//return the level of wanted node
int checkDepth(node *tree, int val, int depthLevel){
    //if not a single node exists
    if(tree == NULL)
        return 0;

    //if found the node
    if(tree->data == val)
        return depthLevel;

    //if not found recursively look the other branches
    return checkDepth(tree->left, val, depthLevel+1)
           | checkDepth(tree->right, val, depthLevel+1);
}

void deleteByTheUserEntered(){
    //get the value which is wanted to be deleted
    printf("write the number to delete: ");
    scanf("%d",&num);

    //if it does exist
    if(checkTerm(num) == 1){
        //assign the last form into root
        root = delete(root, num);
        printf("\ndeleted\n**re-construct tree by the new formation**\nlnr display\n");

        print_lnr(root);
    }
        //if does not exist
    else
        printf("does not exist\n\n");
}

void tracePathByTheUserEntered(){
    //select the node to trace path
    printf("write the number to trace path: ");
    scanf("%d",&num);

    //if selected value does exist, look through its path and depth information
    if(checkTerm(num) == 1)
        printf("found in the depth level: %d",checkDepth(root,num,1));
    else
        printf("does not exist\n\n");
}

void splitString(char string[]){

    int val;
    char * pch;
    pch = strtok (string," ,.-");
    while (pch != NULL)
    {
        val = atoi(pch);
        root = insert(root, val);
        pch = strtok (NULL, " ,.-");
    }
}

void assignDepths(node *currentPtr){
    if (currentPtr){
        assignDepths(currentPtr->left);
        currentPtr->depth = checkDepth(root,currentPtr->data,1);
        assignDepths(currentPtr->right);
    }
}

void assignSize(node *currentPtr){
    if (currentPtr){
        assignSize(currentPtr->left);
        currentPtr->total_size = getSize(traceNode(root,currentPtr->data));
        assignSize(currentPtr->right);
    }
}

void assignSizeLR(node *currentPtr){
    if(currentPtr && currentPtr->left  && currentPtr->right){
        assignSizeLR(currentPtr->left);
        currentPtr->left_size = currentPtr->left->total_size;
        currentPtr->right_size = currentPtr->right->total_size;
        assignSizeLR(currentPtr->right);
    }
    else if(currentPtr && currentPtr->left && currentPtr->right == NULL){
        assignSizeLR(currentPtr->left);
        currentPtr->left_size = currentPtr->left->total_size;
        currentPtr->right_size = 0;
        assignSizeLR(currentPtr->right);
    }
    else if(currentPtr && currentPtr->left == NULL && currentPtr->right){
        assignSizeLR(currentPtr->left);
        currentPtr->left_size = 0;
        currentPtr->right_size = currentPtr->right->total_size;
        assignSizeLR(currentPtr->right);
    }
    else if(currentPtr && currentPtr->left == NULL && currentPtr->right == NULL){
        assignSizeLR(currentPtr->left);
        currentPtr->left_size = 0;
        currentPtr->right_size = 0;
        assignSizeLR(currentPtr->right);
    }
}

void assignTopology(node *currentPtr){
    if (currentPtr){
        assignTopology(currentPtr->left);
        currentPtr->topology_num = getDifTopology(traceNode(root,currentPtr->data)->total_size-1);
        assignTopology(currentPtr->right);
    }
}

void assignOrd(node *tree){

    int index1,index2;
    int nodes[] = {18, 12, 8, 4, 2, 16, 14, 15, 17, 36, 24, 20, 21, 72, 54};
    int maxDepth = getMaxDepth(root);

    for (index1=maxDepth; index1>0; index1--)
        for(index2=0 ; index2<tree->total_size; index2++){
            node * temp = traceNode(tree,nodes[index2]);
            if (temp->left == NULL && temp->right == NULL)
                temp->ord = 1;

            else if (temp->left && temp->right){
                temp->ord = temp->left->ord * temp->right->ord * ((factorial(temp->left_size + temp->right_size)) / (factorial(temp->left_size)*factorial(temp->right_size)));
            }
            else if (temp->left || temp->right)
                temp->ord = temp->topology_num;
        }
}
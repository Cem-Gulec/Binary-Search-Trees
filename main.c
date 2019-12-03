/*Designed and programmed by - Cem Gulec - 150117828
 *
 * An example constructed tree:
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
 * TODO: output düzenlenecek
 * TODO: kodlar biraz daha düzenlenebilir
 * TODO: yorumlama daha fazla eklenebilir
 * TODO: main içi düzenle
 * TODO: tracepath için bir seçenek sun
 * TODO: 180bin sequence'in printlenmesi
 * TODO: data rapor
 * TODO: getdiftopology sileyim mi
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
    unsigned long long int topology_num;
    unsigned long long int ord;
    struct bst_tree *left, *right;
};
typedef struct bst_tree node;

node * root = NULL;
int numArray[50], i=0, num, decision;
FILE *fptr;

/*prototypes*/
node * insert(node *, int);
node * delete(node *, int);
void print_lnr(node *);
node * getMin(node *);
node * getMax(node *);
int getMaxDepth(node *);
int getSize(node *);
unsigned long long int getDifTopology(int);
int calculateCatalan(int);
int binomialCoeF(int,int);
unsigned long long int factorial(unsigned int);
node * traceNode (node *, int );
int checkTerm(int);
int checkDepth(node *, int, int);
void checkDepthByTheUserEntered();
void getSizeByTheUserEntered();
void insertByTheUserEntered();
void tracePathByTheUserEntered();
void splitString(char []);
void assignDepths(node *);
void assignSize(node *);
void assignSizeLR(node *);
void assignOrd(node *);

void main()
{
    int insert_number;
    char str[200];
    printf("Enter your sequence of numbers: ");
    gets(str);
    //while splitting the sequence of numbers given
    //it also adds elements to the tree
    splitString(str);

    assignDepths(root);
    assignSize(root);
    assignSizeLR(root);
    assignOrd(root);

    /* Printing nodes of tree in LNR fashion */
    printf("LNR display\n");
    print_lnr(root);
    printf("\nMax depth level: %d\n",getMaxDepth(root));
    printf("Min element of the tree: %d  |  Max element of the tree: %d\n", getMin(root)->data,getMax(root)->data);
    printf("Size of the root: %d  |  # of_left_nodes: %d  |  # of_right_nodes: %d\n",root->total_size, root->left_size, root->right_size);
    printf("\nPress (1) to insert a node\nPress (2) to trace path of a node\n"
           "Press (3) to get size of a node\nPress (4) check depth of a node\n      (-1) to exit\nDecision: ");
    scanf("%d",&decision);

    while(decision != -1){
        switch (decision){
            //insert another node
            case 1:
                insertByTheUserEntered();
                break;
            //trace path of a node
            case 2:
                tracePathByTheUserEntered();
                break;
            //track size information of a node
            case 3:
                getSizeByTheUserEntered();
                break;
            case 4:
                checkDepthByTheUserEntered();
                break;
            //press -1 to exit loop else continue asking
            case -1:
                exit(-1);
        }

        printf("\nPress (1) to insert a node\nPress (2) to trace path of a node\n"
               "Press (3) to get size of a node\nPress (4) check depth of a node\n      (-1) to exit\nDecision: ");
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
        printf("%d -> d_l:%d - #left:%d - #right:%d - #total_size:%d - ord:%llu\n",tree->data, tree->depth,
                tree->left_size, tree->right_size, tree->total_size, tree->ord);
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
    else{
        //compute the depth of each subtree
        int left_depth = getMaxDepth(tree->left);
        int right_depth = getMaxDepth(tree->right);

        //use the larger one
        if (left_depth > right_depth)
            return(left_depth+1);
        else return(right_depth+1);
    }
}

//a function to learn size of a node
int getSize(node * tree){
    if (tree==NULL)
        return 0;
    else{
        int left_size = getSize(tree->left);
        int right_size = getSize(tree->right);
        return (left_size + right_size +1);
    }
}

//a function to calculate number of topologies of a given node
unsigned long long int getDifTopology(int node_size){
    unsigned long long int count = calculateCatalan(node_size);

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

unsigned long long int factorial(unsigned int n){
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
//if it exists return 1
int checkTerm(int val){
    int i;

    for(i=0; i<50; i++){
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

void checkDepthByTheUserEntered(){
    int number,depth;
    printf("Which node's depth information: ");
    scanf("%d",&number);

    if(checkTerm(number) == 1){
        depth = checkDepth(root,number,1);
        printf("\n\t\t\tDepth level of \"%d\": %d\n",number,depth);
    }
    else
        printf("\n\t\t\t**It doesn't exist in the tree**\n\t\t\tReturning back to selection panel\n\n");
}

void getSizeByTheUserEntered(){
    int number,size;
    printf("Which node's size information: ");
    scanf("%d",&number);

    if(checkTerm(number) == 1){
        size = getSize(traceNode(root,number));
        printf("\n\t\t\tSize of the root: %d | # of_left_nodes: %d | # of_right_nodes: %d\n\n",size, traceNode(root,number)->left_size, traceNode(root,number)->right_size);
    }
    else
        printf("\n\t\t\t**It doesn't exist in the tree**\n\t\t\tReturning back to selection panel\n\n");
}

void insertByTheUserEntered(){
    int insertion_number;
    printf("Number to be inserted: ");
    scanf("%d",&insertion_number);

    if(checkTerm(insertion_number) == 0){
        root = insert(root,insertion_number);
        assignDepths(root);
        assignSize(root);
        assignSizeLR(root);
        assignOrd(root);
        printf("\n\t\t\t**Number has been inserted**\n\t\t\tRe-constructing tree by the new formation\n\t\t\tLNR display\n\n");
        print_lnr(root);
        printf("\nMax depth level: %d\n",getMaxDepth(root));
        printf("Min element of the tree: %d  |  Max element of the tree: %d\n", getMin(root)->data,getMax(root)->data);
        printf("Size of the root: %d  |  # of_left_nodes: %d  |  # of_right_nodes: %d\n",root->total_size, root->left_size, root->right_size);
    }
    else
        printf("\n\t\t\t**It already exists in tree**\n\t\t\tReturning back to selection panel\n\n");
}

void tracePathByTheUserEntered(){
    //select the node to trace path
    printf("Write number to be traced: ");
    scanf("%d",&num);

    //if selected value does exist, look through its path and depth information
    if(checkTerm(num) == 1)
        printf("\n\t\t\tFound in the depth level: %d\n",checkDepth(root,num,1));
    else
        printf("\n\t\t\t**The number does not exist\n\t\t\tReturning back to selection panel\n\n");
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

void assignOrd(node *tree){
    int index1,index2;
    int maxDepth = getMaxDepth(root);

    for (index1=maxDepth; index1>0; index1--)
        for(index2=0 ; index2<tree->total_size; index2++){
            node * temp = traceNode(tree,numArray[index2]);
            if(temp->depth == index1){
                if (temp->left == NULL && temp->right == NULL)
                    temp->ord = 1;

                else if (temp->left && temp->right){
                    unsigned long long int fact_value = factorial(temp->left_size + temp->right_size) / (factorial(temp->left_size)*factorial(temp->right_size));
                    unsigned long long int temp_val= temp->left->ord * temp->right->ord * fact_value;
                    temp->ord = temp_val;
                }
                else if (temp->left || temp->right)
                    temp->ord = 1;
            }
            else
                continue;
        }
}
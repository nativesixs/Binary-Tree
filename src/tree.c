#include "tree.h"
#include "mymalloc.h"
void print_postorder(TreeNode* node, TreeNodeProc proc);
void print_inorder(TreeNode* node, TreeNodeProc proc);
void print_preorder(TreeNode* node, TreeNodeProc proc);

bool Tree_Init(Tree* const root) {
    if (root) {
        root->root = NULL;
        root->itemsCount = 0;
        return true;
    }
    return false;
}


void clear(TreeNode*node){
    if (node != NULL){
        clear(node->left);
        clear(node->right);
     myFree(node);

    }

}

void Tree_Clear(Tree* const root) {
    if (root != NULL){
   clear(root->root);
   root->root=NULL;
   root->itemsCount=0;
    }
}


TreeNode* _tree_create_node(const Data_t* const data) {
    // alokace pameti
    TreeNode* node = myMalloc(sizeof(TreeNode));
    if(node != NULL){
        node->data = *data;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

bool Tree_Insert(Tree* const root, const Data_t data) {
    //kontrola ukazatele
    if (root) {
        //kontrola existence rootu
        if (!root->root) {
            //create root
            root->root = _tree_create_node(&data);
            //kontrola creatu
            if (root->root == NULL) {
                return false;
            }
        } else {
            TreeNode* node = root->root;
            TreeNode* parent = NULL;
            int result = 0;

            //pruchod bintree
            while (node) {
                parent = node;
                //porovnani input x current
                result = Data_Cmp(&data, &node->data);

                //posunuti v bintree na relevantniho potomka
                if (result == 0) {
                    return false;
                } else if (result < 0) {
                    node = node->left;
                } else {
                    node = node->right;
                }
            }

            //kontrola ulozeni l x r node ?
            if (result < 0) {
                //create new node -> save as left
                parent->left = _tree_create_node(&data);
                if (parent->left == NULL) {
                    return false;
                }
            } else {
                //create new node -> save as right
                parent->right = _tree_create_node(&data);
                if (parent->right == NULL) {
                    return false;
                }
            }
        }
        //++ prvky bintree
        root->itemsCount++;
        return true;
    }

    return false;
}



TreeNode * FindMin(TreeNode *root){
    if(root==NULL){
        printf("strom je prazndy\n");
        return NULL;
    }
    while(root->left !=NULL){
        root=root->left;
    }
    return root;
}

void Tree_Delete(Tree* const root, const Data_t data) {
if(root==NULL) return;
TreeNode *uk = root->root, **pred=NULL;

int cmp;
while(uk!=NULL){
    cmp = Data_Cmp(&data,&uk->data);
    if(cmp==0){
        root->itemsCount--;
        if(uk->left==NULL&&uk->right==NULL){
            myFree(uk);
            *pred = NULL;
            return;
        }else if(uk->left!=NULL&&uk->right!=NULL){
            TreeNode * nejpravejsi = uk->left, *predchozi = NULL;
            while(nejpravejsi->right!=NULL){
                predchozi=nejpravejsi;
                nejpravejsi=nejpravejsi->right;
            }
            uk->data=nejpravejsi->data;
            if(predchozi==NULL){
                uk->left=nejpravejsi->left;
            }else{
                predchozi->right=nejpravejsi->left;
            }
            myFree(nejpravejsi);
            return;
        }else{
            if(uk->left!=NULL) *pred=uk->left;
            else *pred=uk->right;
            myFree(uk);
            return;
        }
    }else{
        if(cmp<0){
            pred=&uk->left;
            uk=uk->left;
        }
        else if(cmp>0){
            pred=&uk->right;
            uk=uk->right;
        }
    }
}
if(pred==NULL)
    return;

}

const Data_t* Tree_Get_Data(const TreeNode* const node) {
  return &(node->data);
}

TreeNode* Tree_Find_Node(Tree root, const Data_t data) {
    TreeNode*uk =root.root;
    int cmp;
    while(uk!=NULL){
        cmp= Data_Cmp(&data,&uk->data);
        if(cmp<0)uk=uk->left;
        else if(cmp>0) uk=uk->right;
        else return uk;
    }
    return NULL;
}

size_t Tree_Get_Count(Tree root) {
    return root.itemsCount;
}

void Tree_Process(Tree root, TreeNodeProc proc, TreeProcessMode mode) {
switch(mode){
case procPREORDER:
    print_preorder(root.root,proc);
break;
case procINORDER:
    print_inorder(root.root,proc);
    break;
case procPOSTORDER:
    print_postorder(root.root,proc);
break;
    }

}


void print_preorder(TreeNode* node, TreeNodeProc proc) {
    if (node != NULL){

        proc(node);                     //root
         print_preorder(node->left,proc); //left
         print_preorder(node->right,proc); //right
    }
}

void print_inorder(TreeNode* node, TreeNodeProc proc) {
    if (node != NULL){
print_inorder(node->left,proc);  //left
 proc(node);                    //root
print_inorder(node->right,proc); //right
    }
}

void print_postorder(TreeNode* node, TreeNodeProc proc) {
    if (node != NULL){
print_postorder(node->left,proc);   //left
print_postorder(node->right,proc);  //right
proc(node);                         //root
    }
}

void Tree_Print(TreeNode* node, TreeNode* previous, int spaces) {
    //kontrola predani ukazatelu
    if (node == NULL || previous == NULL) return;

    //vypis bintree
    spaces += 6;
    Tree_Print(node->right, node, spaces);
    printf("\n");
    for (int i = 6; i < spaces; i++) printf(" ");
    if (previous == node) {
        printf("---%s", node->data.name);
    } else if (node == previous->left) {
        printf("|\n");
        for (int i = 6; i < spaces; i++) printf(" ");
        printf("---%s", node->data.name);
    } else {
        printf("---%s\n", node->data.name);
        for (int i = 6; i < spaces; i++) printf(" ");
        printf("|");
    }
    Tree_Print(node->left, node, spaces);
}

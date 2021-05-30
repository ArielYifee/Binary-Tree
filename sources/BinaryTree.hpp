#pragma once
#include <stack>
#include <unordered_set>

namespace ariel{

    template <typename T>
    class BinaryTree{

    public:
        BinaryTree(){
            root = nullptr;
        }
        ~BinaryTree(){
            removeTree();
        }
        BinaryTree(const BinaryTree& otherTree){   
            removeTree();
            copyTree(otherTree);
        }

        BinaryTree(BinaryTree&& otherTree)noexcept{
            root = otherTree.root;
            otherTree.root = nullptr;
        }

        BinaryTree& operator=(const BinaryTree& otherTree){
            if(this != &otherTree){
                removeTree();
                copyTree(otherTree);
            }
            return *this;
        }

        BinaryTree &operator=(BinaryTree&& otherTree) noexcept{
            root = otherTree.root;
            otherTree.root = nullptr;
            return *this;
        }

        BinaryTree& add_root(T value){
            if(root == nullptr){
                root = new Node(value);
            }else{
                root->nValue = value;
            }
            return *this;
        }
        BinaryTree& add_left(T parent, T child){
            Node* temp = findNode(parent);
            if(temp == nullptr){
                throw std::invalid_argument("Parent not found");
            }
            if(temp->left == nullptr){
                temp->left = new Node(child);
            }else{
                (temp->left)->nValue = child;
            }
            return *this;
        }
        BinaryTree& add_right(T parent, T child){
            Node* temp = findNode(parent);
            if(temp == nullptr){
                throw std::invalid_argument("Parent not found");
            }
            if(temp->right == nullptr){
                temp->right = new Node(child);
            }else{
                (temp->right)->nValue = child;
            }
            return *this;
        }
        friend std::ostream& operator<<(std::ostream &out, const ariel::BinaryTree<T>& binaryTree){
            for(auto it : binaryTree){
                out << it << " ";
            }
            return out;
        }

    private:

        typedef struct Node{
            T nValue;
            Node *left;
            Node *right;
            Node(const T& v, Node *leftN = nullptr, Node *rightN = nullptr) : left(leftN), right(rightN), nValue(v){}
        }Node;

        Node* root;

        Node* findNode(T value){
            if(root){
                for (auto it = begin_inorder(); it != end_inorder(); ++it){
                    if (*it == value){
                        return it.getNode();
                    }
                }
            }
            return nullptr;
        }
        void removeTree(){
            if(!root){
                for (auto it = begin_inorder(); it != end_inorder();++it){
                    Node *temp = it.getNode();
                    delete temp;
                }
            }
        }
        void recCopy(Node* node, Node* otherNode){
            if(otherNode->right){
                node->right = new Node(otherNode->right->nValue);
                recCopy(node->right, otherNode->right);
            }
            if(otherNode->left){
                node->left = new Node(otherNode->left->nValue);
                recCopy(node->left, otherNode->left);
            }
        }
        void copyTree(const BinaryTree& otherTree){
            if(otherTree.root){
                root = new Node(otherTree.root->nValue);
                recCopy(root, otherTree.root);
            }
        }

    public:
        /*
        I used the algorithm of geeksforgeeks for Preorder Tree Traversal without Recursion
        https://www.geeksforgeeks.org/iterative-preorder-traversal/    
        */
        class preorder_iterator{
            public:
                preorder_iterator(Node* ptr = nullptr){
                    if(ptr != nullptr){
                        nodes.push(ptr);
                        currNodePtr = nodes.top();
                    }else{
                        currNodePtr = nullptr;
                    }
                }

		        T& operator*() const {
		        	return currNodePtr->nValue;
		        }

		        T* operator->() const {
		        	return &(currNodePtr->nValue);
		        }

		        preorder_iterator& operator++() {
                    if(!nodes.empty()){
                    Node* temp = currNodePtr;
                    nodes.pop();
                    if(temp->right != nullptr){
                        nodes.push(temp->right);
                    }
                    if(temp->left != nullptr){
                        nodes.push(temp->left);
                    }
                    if(nodes.empty()){
                        currNodePtr = nullptr;
                    }else{
                        currNodePtr = nodes.top();
                    }
                    }else{
                        currNodePtr = nullptr;
                    }
		        	return *this;
		        }

		        preorder_iterator operator++(int){
		        	preorder_iterator tmp= *this;
		        	if(!nodes.empty()){
                    Node* temp = currNodePtr;
                    nodes.pop();
                    if(temp->right != nullptr){
                        nodes.push(temp->right);
                    }
                    if(temp->left != nullptr){
                        nodes.push(temp->left);
                    }
                    if(nodes.empty()){
                        currNodePtr = nullptr;
                    }else{
                        currNodePtr = nodes.top();
                    }
                    }else{
                        currNodePtr = nullptr;
                    }
		        	return tmp;
		        }

		        bool operator==(const preorder_iterator& rhs) const {
		        	return (currNodePtr == rhs.currNodePtr);
		        }

		        bool operator!=(const preorder_iterator& rhs) const {
		        	return (currNodePtr != rhs.currNodePtr);
		        }

            private:
                Node* currNodePtr;
                std::stack<Node*> nodes;

        }; // end of preorder_iterator class

        /*
        I used the algorithm of geeksforgeeks for Inorder Tree Traversal without Recursion
        https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
        */

        class inorder_iterator{
            public:
                inorder_iterator(Node* ptr = nullptr) {
                    while(ptr != nullptr){
                        nodes.push(ptr);
                        ptr = ptr->left;
                    }
                    if(nodes.empty()){
                        currNodePtr = nullptr;
                    }else{
                        currNodePtr = nodes.top();
                        nodes.pop();
                    }
                }

		        T& operator*() const {
		        	return currNodePtr->nValue;
		        }

		        T* operator->() const {
		        	return &(currNodePtr->nValue);
		        }

		        inorder_iterator& operator++() {
                    if(!nodes.empty() || currNodePtr != nullptr){
                        if(currNodePtr->right != nullptr){
                            currNodePtr = currNodePtr->right;
                            while(currNodePtr != nullptr){
                                nodes.push(currNodePtr);
                                currNodePtr = currNodePtr->left;
                            }
                            currNodePtr = nodes.top();
                            nodes.pop();
                        }else{
                            if(nodes.empty()){
                                currNodePtr = nullptr;
                            }else{
                            currNodePtr = nodes.top();
                            nodes.pop();
                            }
                        }
                    }else{
                        currNodePtr = nullptr;
                    }
		        	return *this;
		        }

		        inorder_iterator operator++(int) {
		        	inorder_iterator tmp= *this;
                    if(!nodes.empty() || currNodePtr != nullptr){
                        if(currNodePtr->right != nullptr){
                            currNodePtr = currNodePtr->right;
                            while(currNodePtr != nullptr){
                                nodes.push(currNodePtr);
                                currNodePtr = currNodePtr->left;
                            }
                            currNodePtr = nodes.top();
                            nodes.pop();
                        }else{
                            if(nodes.empty()){
                                currNodePtr = nullptr;
                            }else{
                            currNodePtr = nodes.top();
                            nodes.pop();
                            }
                        }
                    }else{
                        currNodePtr = nullptr;
                    }
		        	return tmp;
		        }

		        bool operator==(const inorder_iterator& rhs) const {
		        	return (currNodePtr == rhs.currNodePtr);
		        }

		        bool operator!=(const inorder_iterator& rhs) const {
		        	return (currNodePtr != rhs.currNodePtr);
		        }
                Node* getNode(){
                    return currNodePtr;
                }
                
            private:
                Node* currNodePtr;
                std::stack<Node*> nodes;


        }; // end of inorder_iterator class

        /*
        I used the algorithm of geeksforgeeks for Postorder Tree Traversal without Recursion
        https://www.geeksforgeeks.org/postorder-traversal-binary-tree-without-recursion-without-stack/ 
        */
        class postorder_iterator{
            public:
                postorder_iterator(Node* ptr = nullptr){
                    if(ptr == nullptr){
                        currNodePtr = nullptr;
                    }else{
                    temp = ptr;
                        while (temp && visited.find(temp) == visited.end()) {
                            if (temp->left != nullptr && visited.find(temp->left) == visited.end()){
                                temp = temp->left;
                            }
                            else if (temp->right && visited.find(temp->right) == visited.end()){
                                temp = temp->right;
                            }
                            else {
                                currNodePtr = temp;
                                visited.insert(temp);
                                temp = ptr;
                                break;
                            }
                        }
                    }
                }

		        T& operator*() const {
		        	return currNodePtr->nValue;
		        }

		        T* operator->() const {
		        	return &(currNodePtr->nValue);
		        }

		        postorder_iterator& operator++() {
                        Node* ptr = temp;
                        // if(currNodePtr != temp){
                        if(temp && visited.find(temp) == visited.end()){
                        while (temp != nullptr && visited.find(temp) == visited.end()) {
                            if (temp->left != nullptr && visited.find(temp->left) == visited.end()){
                                temp = temp->left;
                            }
                            // Visited right subtree
                            else if (temp->right && visited.find(temp->right) == visited.end()){
                                temp = temp->right;
                            }
                            else {
                                // if(temp == ptr){

                                // }
                                currNodePtr = temp;
                                visited.insert(temp);
                                temp = ptr;
                                break;   
                            }
                        }
                        }else{
                            currNodePtr = nullptr;
                        }
		        	return *this;
		        }

		        postorder_iterator operator++(int) {
		        	postorder_iterator tmp= *this;
                    Node* ptr = temp;
                        // if(currNodePtr != temp){
                        if(temp && visited.find(temp) == visited.end()){
                        while (temp != nullptr && visited.find(temp) == visited.end()) {
                            if (temp->left != nullptr && visited.find(temp->left) == visited.end()){
                                temp = temp->left;
                            }
                            // Visited right subtree
                            else if (temp->right && visited.find(temp->right) == visited.end()){
                                temp = temp->right;
                            }
                            else {
                                // if(temp == ptr){

                                // }
                                currNodePtr = temp;
                                visited.insert(temp);
                                temp = ptr;
                                break;   
                            }
                        }
                        }else{
                            currNodePtr = nullptr;
                        }
		        	return tmp;
		        }

		        bool operator==(const postorder_iterator& rhs) const {
		        	return (currNodePtr == rhs.currNodePtr);
		        }

		        bool operator!=(const postorder_iterator& rhs) const {
                    if(currNodePtr == nullptr && rhs.currNodePtr == nullptr){
                   }
		        	return (currNodePtr != rhs.currNodePtr);
		        }
                
            private:
                Node* currNodePtr;
                Node* temp;
                std::unordered_set<Node*> visited;

        }; // end of postorder_iterator class

        preorder_iterator begin_preorder() const{
            return preorder_iterator{root};
        }
        preorder_iterator end_preorder() const{
            return preorder_iterator{};
        }
        inorder_iterator begin_inorder() const{
            return inorder_iterator{root};
        }
        inorder_iterator end_inorder() const{
            return inorder_iterator{nullptr};
        }
        inorder_iterator begin() const{
            return inorder_iterator{root};
        }
        inorder_iterator end() const{
            return inorder_iterator{};
        }
        postorder_iterator begin_postorder() const{
            return postorder_iterator{root};
        }
        postorder_iterator end_postorder() const{
            return postorder_iterator{nullptr};
        }

    };//end of BinaryTree class
}; // end of namespace
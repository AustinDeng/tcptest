class Solution {
public:
    int cnt = 0;
    int pathSum(TreeNode* root, int targetSum) {
        if(!root) {
            return 0;
        }
        dfs(root, targetSum, root->val);

        return cnt;
    }

    void dfs(TreeNode* root, int targetSum, int curr) {
        
        if(curr == targetSum) {
            cnt++;
        }

        if(root->left) {
            dfs(root->left, targetSum, curr+root->left->val);
            dfs(root->left, targetSum, root->left->val);
        }
            
        if(root->right) {
            dfs(root->right, targetSum, curr+root->right->val);
            dfs(root->right, targetSum, root->right->val)
        }
        
        return;
    }

};
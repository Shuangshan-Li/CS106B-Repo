    def buildTree(self, inorder: List[int], postorder: List[int]) -> TreeNode:
        self.table = {}
        for i, num in enumerate(inorder):
            self.table[num] = i
        return self.build(inorder, postorder, 0, len(inorder) - 1)
    def build(self, inorder, postorder, leftBound, rightBound):
        if leftBound > rightBound: return None
        nodeVal = postorder.pop()
        node = TreeNode(nodeVal)
        mid = self.table[nodeVal]
        node.right = self.build(inorder, postorder, mid + 1, rightBound)
        node.left = self.build(inorder, postorder, leftBound, mid - 1)
        return node
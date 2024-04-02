/**
 * @file        tripletree_private.h
 * @description student declarations of private functions
 *              for TripleTree, CPSC 221 PA3
 *
 *              THIS FILE WILL BE SUBMITTED.
 *
 *              Simply declare your function prototypes here.
 *              No other scaffolding is necessary.
 */

 // begin your declarations below

/**
 * Helper function to recursively copy another tree to the current one
 * @param node - the root of the other tree
*/
Node* CopyTree(Node* node);

/**
 * Helper function to recursively travel the tree and count number of leaves
 * @param node - the root of the tree
*/
int NumLeavesCounter(const Node* node) const;

/**
 * Helper function to mirror the tree
 * @param node - the root of the tree
*/
void FlipHorizontal(Node* node);

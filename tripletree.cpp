/**
 * @file        tripletree.cpp
 * @description Student-implemented functions of a ternary tree for CPSC 221 PA3.
 *
 *              THIS FILE WILL BE SUBMITTED FOR GRADING
 */

#include "tripletree.h"
 /**
      * Constructor that builds a TripleTree out of the given PNG.
      *
      * The TripleTree represents the subimage from (0,0) to (w-1, h-1) where
      * w-1 and h-1 are the largest valid image coordinates of the original PNG.
      * Every node corresponds to a rectangle of pixels in the original PNG,
      * represented by an (x,y) pair for the upper left corner of the
      * square and two integers for the number of pixels on the width and
      * height dimensions of the rectangular region the node defines.
      *
      * The node's three children correspond to a partition
      * of the node's rectangular region into three approximately equal-size strips.
      *
      * If the rectangular region is taller than it is wide, the region is divided
      * into horizontal strips:
      *  +-------+
      *  |   A   |
      *  |       |
      *  +-------+
      *  |   B   |
      *  |       |    (Split tall)
      *  +-------+
      *  |   C   |
      *  |       |
      *  +-------+
      *
      * If the rectangular region is wider than it is tall, the region is divided
      * into vertical strips:
      *  +---------+---------+---------+
      *  |    A    |    B    |    C    |
      *  |         |         |         |    (Split wide)
      *  +---------+---------+---------+
      *
      * Your regions are not guaranteed to have dimensions exactly divisible by 3.
      * If the dimensions of your rectangular region are 3p x q or q x 3p where 3p
      * represents the length of the long side, then your rectangular regions will
      * each have dimensions p x q (or q x p)
      *
      * If the dimensions are (3p+1) x q, subregion B gets the extra pixel of size
      * while subregions A and C have dimensions p x q.
      *
      * If the dimensions are (3p+2) x q, subregions A and C each get an extra pixel
      * of size, while subregion B has dimensions p x q.
      *
      * If the region to be divided is a square, then apply the Split wide behaviour.
      *
      * Every leaf in the constructed tree corresponds to a pixel in the PNG.
      *
      * @param imIn - the input image used to construct the tree
      */
TripleTree::TripleTree(PNG& imIn) {
    root = BuildNode(imIn, pair<unsigned int, unsigned int>(0, 0), imIn.width(), imIn.height());
}

/**
 * Render returns a PNG image consisting of the pixels
 * stored in the tree. It may be used on pruned trees. Draws
 * every leaf node's rectangle onto a PNG canvas using the
 * average color stored in the node.
 *
 * You may want a recursive helper function for this.
 */
PNG TripleTree::Render() const {
    // replace the line below with your implementation
    PNG image; // Initialize with appropriate dimensions if not default constructed.
    if (root) {
        image.resize(root->width, root->height);
        RenderHelper(root, image);
    }
    return image;
}

/*
 * Prune function trims subtrees as high as possible in the tree.
 * A subtree is pruned (cleared) if all of its leaves are within
 * tol of the average color stored in the root of the subtree.
 * Pruning criteria should be evaluated on the original tree, not
 * on a pruned subtree. (we only expect that trees would be pruned once.)
 *
 * You may want a recursive helper function for this.
 *
 * @param tol - maximum allowable RGBA color distance to qualify for pruning
 */
void TripleTree::Prune(double tol) {
    // add your implementation below
	PruneHelper(root, tol);
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be mirrored horizontally (flipped over a vertical axis).
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::FlipHorizontal() {
    FlipHorizontal(root);
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be rotated 90 degrees counter-clockwise.
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::RotateCCW() {
    // add your implementation below
	
}

/*
 * Returns the number of leaf nodes in the tree.
 *
 * You may want a recursive helper function for this.
 */
int TripleTree::NumLeaves() const {
    return NumLeavesCounter(root);
}

/**
     * Destroys all dynamically allocated memory associated with the
     * current TripleTree object. To be completed for PA3.
     * You may want a recursive helper function for this one.
     */
void TripleTree::Clear() {
    // add your implementation below
	ClearHelper(root);
}

/**
 * Copies the parameter other TripleTree into the current TripleTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other - The TripleTree to be copied.
 */
void TripleTree::Copy(const TripleTree& other) {
    root = CopyTree(other.root);
}

/**
 * Private helper function for the constructor. Recursively builds
 * the tree according to the specification of the constructor.
 * @param im - reference image used for construction
 * @param ul - upper left point of node to be built's rectangle.
 * @param w - width of node to be built's rectangle.
 * @param h - height of node to be built's rectangle.
 */
Node* TripleTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
    if (w == 0 || h == 0) // base case for when we divide 1 pixel by 3
        return nullptr;

    Node* node = new Node(ul, w, h);
    if (w == 1 && h == 1) { // 'base case' for recursion when calculating avg colour
        node->avg = *im.getPixel(ul.first, ul.second);
        return node;
    }
    
    // unsigned int avgR, avgG, avgB;
    // double avgA;
    int maxSide = std::max(w, h);
    int heightcase = maxSide % 3;
    int heightA = maxSide / 3;
    int heightB = maxSide / 3;
    int heightC = heightA;
    switch (heightcase) {
        case 0: // case 1: divisible by 3
            break;
        case 1: // case 2: 3p + 1 so middle gets extra
            heightB++;
            break;
        case 2: // case 3: 3p + 2 so side two gets extra
            heightA++;
            heightC = heightA;
            break;
    }
        
    if (w >= h) {
        node->A = BuildNode(im, make_pair(ul.first, ul.second), heightA, h);
        node->B = BuildNode(im, make_pair(ul.first + heightA, ul.second), heightB, h);
        node->C = BuildNode(im, make_pair(ul.first + heightA + heightB, ul.second), heightC, h);
    } else {
        node->A = BuildNode(im, make_pair(ul.first, ul.second), w, heightA);
        node->B = BuildNode(im, make_pair(ul.first, ul.second + heightA), w, heightB);
        node->C = BuildNode(im, make_pair(ul.first, ul.second + heightA + heightB), w, heightC);

    }

        

    // idea is to weigh the average colour of all three children by its area and calculate the average based on that
    
    node->avg = AverageColour(node);
    return node;
}

/* ===== IF YOU HAVE DEFINED PRIVATE MEMBER FUNCTIONS IN tripletree_private.h, IMPLEMENT THEM HERE ====== */

RGBAPixel TripleTree::AverageColour(Node* node) {

    int totalRed = 0, totalGreen = 0, totalBlue = 0, totalAlpha = 0, totalArea = 0;

    // Helper lambda to add child node's contribution to totals
    auto addChildContribution = [&](Node* child) {
        if (child != nullptr) {
            int area = child->width * child->height;
            totalRed += child->avg.r * area;
            totalGreen += child->avg.g * area;
            totalBlue += child->avg.b * area;
            totalAlpha += child->avg.a * area;
            totalArea += area;
        }
    };

    // Add contributions of A, B, and C children
    addChildContribution(node->A);
    addChildContribution(node->B);
    addChildContribution(node->C);

    // Calculate the total area of the current node
    if (totalArea == 0) { // Prevent division by zero
        return RGBAPixel(0, 0, 0, 0);
    }

    // Calculate average color components
    int avgRed = totalRed / totalArea;
    int avgGreen = totalGreen / totalArea;
    int avgBlue = totalBlue / totalArea;
    int avgAlpha = totalAlpha / totalArea;

    return RGBAPixel(avgRed, avgGreen, avgBlue, avgAlpha);
}

/**
 * Helper function to recursively copy another tree to the current one
 * @param node - the root of the other tree
*/
Node* TripleTree::CopyTree(Node* node) {
    if (node == nullptr)
        return nullptr;
    
    Node* newNode = new Node(node->upperleft, node->width, node->height);
    newNode->avg = node->avg;
    newNode->A = CopyTree(node->A);
    newNode->B = CopyTree(node->B);
    newNode->C = CopyTree(node->C);

    return newNode;
}

/**
 * Helper function to recursively travel the tree and count number of leaves
 * @param node - the root of the tree
*/
int TripleTree::NumLeavesCounter(const Node* node) const {
    if (node == nullptr)
        return 0;
    else if (node->A == nullptr && node->B == nullptr && node->C == nullptr)
        return 1;
    
    int result = 0;
    result += NumLeavesCounter(node->A);
    result += NumLeavesCounter(node->B);
    result += NumLeavesCounter(node->C);

    return result;
}

/**
 * Helper function to mirror the tree
 * @param node - the root of the tree
*/
void TripleTree::FlipHorizontal(Node* node) {
    if (node->A != nullptr && node->C != nullptr) { // because image has to be a rectangle so tree is symmetrical
        FlipHorizontal(node->A);
        FlipHorizontal(node->C);
        
        Node* temp = node->A;
        node->A = node->C;
        node->C = temp;
    }
    if (node->B != nullptr)
        FlipHorizontal(node->B);
}

void TripleTree::RenderHelper(Node* node, PNG& image) const {
    if (!node) return; // Base case: node is null.
    if (!node->A && !node->B && !node->C) { // Leaf node check.
        // Paint the rectangle with node's average color.
        for (unsigned int x = node->upperleft.first; x < node->upperleft.first + node->width; ++x) {
            for (unsigned int y = node->upperleft.second; y < node->upperleft.second + node->height; ++y) {
                RGBAPixel* pixel = image.getPixel(x, y);
                *pixel = node->avg;
            }
        }
    } else {
        // Recurse for children.
        RenderHelper(node->A, image);
        RenderHelper(node->B, image);
        RenderHelper(node->C, image);
    }
}

void TripleTree::PruneHelper(Node*& node, double tol) {
    if (!node) return; // Base case.
    if (!node->A && !node->B && !node->C) return; // Leaf node.

    if (ShouldPrune(node, node->avg, tol)) {
        Clear();
    } else {
        PruneHelper(node->A, tol);
        PruneHelper(node->B, tol);
        PruneHelper(node->C, tol);
    }
}

bool TripleTree::ShouldPrune(Node* node, RGBAPixel avg, double tol) const {
    if (!node) return true;
    if (!node->A && !node->B && !node->C) { // Leaf node.
        return node->avg.distanceTo(avg) <= tol;
    }
    return ShouldPrune(node->A, avg, tol) &&
           ShouldPrune(node->B, avg, tol) &&
           ShouldPrune(node->C, avg, tol);
}

void TripleTree::FlipHorizontalHelper(Node* node) {
    if (!node || (!node->A && !node->B && !node->C)) return; // Null or leaf node.
    std::swap(node->A, node->C); // Swap the horizontal children.
    FlipHorizontalHelper(node->A);
    FlipHorizontalHelper(node->B);
    FlipHorizontalHelper(node->C);
}

int TripleTree::NumLeavesHelper(Node* node) const {
    if (!node) return 0; // Base case.
    if (!node->A && !node->B && !node->C) return 1; // Leaf node.
    return NumLeavesHelper(node->A) + NumLeavesHelper(node->B) + NumLeavesHelper(node->C); // Sum of leaves in children.
}

void TripleTree::ClearHelper(Node*& node) {
    if (node == nullptr) return;
    ClearHelper(node->A);
    ClearHelper(node->B);
    ClearHelper(node->C);
    delete node; // Free the current node after its children are dealt with.
    node = nullptr; // Avoid dangling pointers by setting the deleted node's pointer to nullptr.
}

Node* TripleTree::CopyHelper(Node* otherNode) {
    if (otherNode == nullptr) return nullptr;
    Node* newNode = new Node(otherNode->upperleft, otherNode->width, otherNode->height);
    newNode->avg = otherNode->avg;
    newNode->A = CopyHelper(otherNode->A);
    newNode->B = CopyHelper(otherNode->B);
    newNode->C = CopyHelper(otherNode->C);
    return newNode;
}
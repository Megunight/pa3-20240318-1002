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
    // add your implementation below
	FlipHorizontalHelper(root);
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
    // replace the line below with your implementation
    return NumLeavesHelper(root);
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
    // add your implementation below
	root = CopyHelper(other.root);
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
    node->avg = AverageColour(im, ul, w, h);

    if (h > w) {
        int heightcase = h % 3;
        int heightA, heightB, heightC;
        switch (heightcase) {
            case 0: // case 1: divisible by 3
                heightA = h / 3;
                heightB = h / 3;
                heightC = h / 3;
                break;
            case 1: // case 2: 3p + 1 so middle gets extra
                heightA = h / 3;
                heightB = (h / 3);
                if (heightB != 0) // for terminating correctly when we only have 1 pixel
                    heightB++;
                heightC = h / 3;
                break;
            case 2: // case 3: 3p + 2 so side two gets extra
                heightA = (h / 3) + 1;
                heightB = h / 3;
                heightC = (h / 3) + 1;
                break;
        }
        
        node->A = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second), w, heightA);
        node->B = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + heightA), w, heightB);
        node->C = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + heightB), w, heightC);
    } else {
        int widthcase = w % 3;
        int widthA, widthB, widthC;
        switch (widthcase) {
            case 0: // case 1: divisible by 3
                widthA = w / 3;
                widthB = w / 3;
                widthC = w / 3;
                break;
            case 1: // case 2: 3p + 1 so middle gets extra
                widthA = w / 3;
                widthB = (w / 3);
                if (widthB != 0) // same as above
                    widthB++;
                widthC = w / 3;
                break;
            case 2: // case 3: 3p + 2 so side two gets extra
                widthA = (w / 3) + 1;
                widthB = w / 3;
                widthC = (w / 3) + 1;
                break;
        }
        
        node->A = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second), widthA, h);
        node->B = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + widthA, ul.second), widthB, h);
        node->C = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + widthB, ul.second), widthC, h);
    }

    return node;
}

/* ===== IF YOU HAVE DEFINED PRIVATE MEMBER FUNCTIONS IN tripletree_private.h, IMPLEMENT THEM HERE ====== */

/**
 * Given an area of an image, find the average colour and return an RGBAPixel
 * @param im - reference image used for construction
 * @param ul - upper left point of node to be built's rectangle.
 * @param w - width of node to be built's rectangle.
 * @param h - height of node to be built's rectangle.
 */
RGBAPixel TripleTree::AverageColour(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
    unsigned long long totalRed = 0, totalGreen = 0, totalBlue = 0, totalAlpha = 0;
    unsigned long long pixelCount = 0; // Use unsigned long long for large images to prevent overflow

    for (unsigned int x = ul.first; x < ul.first + w && x < im.width(); x++) {
        for (unsigned int y = ul.second; y < ul.second + h && y < im.height(); y++) {
            RGBAPixel* pixel = im.getPixel(x, y);
            totalRed += pixel->r;
            totalGreen += pixel->g;
            totalBlue += pixel->b;
            totalAlpha += pixel->a;
            pixelCount++;
        }
    }

    // Check to prevent division by zero
    if (pixelCount == 0) {
        return RGBAPixel(); // Return default RGBAPixel if no pixels are in the specified region
    }

    // Calculate average values
    unsigned int avgRed = static_cast<unsigned int>(totalRed / pixelCount);
    unsigned int avgGreen = static_cast<unsigned int>(totalGreen / pixelCount);
    unsigned int avgBlue = static_cast<unsigned int>(totalBlue / pixelCount);
    unsigned int avgAlpha = static_cast<unsigned int>(totalAlpha / pixelCount);

    return RGBAPixel(avgRed, avgGreen, avgBlue, avgAlpha);
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
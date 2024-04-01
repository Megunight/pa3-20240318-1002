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
    return PNG();
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
    return -1;
}

/**
     * Destroys all dynamically allocated memory associated with the
     * current TripleTree object. To be completed for PA3.
     * You may want a recursive helper function for this one.
     */
void TripleTree::Clear() {
    // add your implementation below
	
}

/**
 * Copies the parameter other TripleTree into the current TripleTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other - The TripleTree to be copied.
 */
void TripleTree::Copy(const TripleTree& other) {
    // add your implementation below
	
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
    if (w == 1 && h == 1) // 'base case' for recursion when calculating avg colour
        node->avg = *(im.getPixel(ul.first, ul.second));
    
    unsigned int avgR, avgG, avgB;
    double avgA;

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

        if (w > 1 && h > 2) {
            avgR = ((node->A->avg.r * (w * heightA)) + (node->B->avg.r * (w * heightB)) + (node->C->avg.r * (w * heightC)))
                / (w * (heightA + heightB + heightC));
            avgG = ((node->A->avg.g * (w * heightA)) + (node->B->avg.g * (w * heightB)) + (node->C->avg.g * (w * heightC)))
                / (w * (heightA + heightB + heightC));
            avgB = ((node->A->avg.b * (w * heightA)) + (node->B->avg.b * (w * heightB)) + (node->C->avg.b * (w * heightC)))
                / (w * (heightA + heightB + heightC));
            avgA = ((node->A->avg.a * (w * heightA)) + (node->B->avg.a * (w * heightB)) + (node->C->avg.a * (w * heightC)))
                / (w * (heightA + heightB + heightC));
            node->avg = RGBAPixel(avgR, avgG, avgB, avgA);
        } else if (w != 1 && h != 1) {
            avgR = ((node->A->avg.r * (w * heightA)) + (node->C->avg.r * (w * heightC)))
                / (w * (heightA + heightC));
            avgG = ((node->A->avg.g * (w * heightA)) + (node->C->avg.g * (w * heightC)))
                / (w * (heightA + heightC));
            avgB = ((node->A->avg.b * (w * heightA)) + (node->C->avg.b * (w * heightC)))
                / (w * (heightA + heightC));
            avgA = ((node->A->avg.a * (w * heightA)) + (node->C->avg.a * (w * heightC)))
                / (w * (heightA + heightC));
            node->avg = RGBAPixel(avgR, avgG, avgB, avgA);
        }
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

        if (w > 2 && h > 1) {
            avgR = ((node->A->avg.r * (widthA * h)) + (node->B->avg.r * (widthB * h)) + (node->C->avg.r * (widthC * h)))
                / (h * (widthA + widthB + widthC));
            avgG = ((node->A->avg.g * (widthA * h)) + (node->B->avg.g * (widthB * h)) + (node->C->avg.g * (widthC * h)))
                / (h * (widthA + widthB + widthC));
            avgB = ((node->A->avg.b * (widthA * h)) + (node->B->avg.b * (widthB * h)) + (node->C->avg.b * (widthC * h)))
                / (h * (widthA + widthB + widthC));
            avgA = ((node->A->avg.a * (widthA * h)) + (node->B->avg.a * (widthB * h)) + (node->C->avg.a * (widthC * h)))
                / (h * (widthA + widthB + widthC));
            node->avg = RGBAPixel(avgR, avgG, avgB, avgA);
        } else if (w != 1 && h != 1) {
            avgR = ((node->A->avg.r * (widthA * h)) + (node->C->avg.r * (widthC * h)))
                / (h * (widthA + widthC));
            avgG = ((node->A->avg.g * (widthA * h)) + (node->C->avg.g * (widthC * h)))
                / (h * (widthA + widthC));
            avgB = ((node->A->avg.b * (widthA * h)) + (node->C->avg.b * (widthC * h)))
                / (h * (widthA + widthC));
            avgA = ((node->A->avg.a * (widthA * h)) + (node->C->avg.a * (widthC * h)))
                / (h * (widthA + widthC));
            node->avg = RGBAPixel(avgR, avgG, avgB, avgA);
        }
    }

    // idea is to weigh the average colour of all three children by its area and calculate the average based on that
    

    return node;
}

/* ===== IF YOU HAVE DEFINED PRIVATE MEMBER FUNCTIONS IN tripletree_private.h, IMPLEMENT THEM HERE ====== */
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
 * Given an area of an image, find the average colour and return an RGBAPixel
 * @param im - reference image used for construction
 * @param ul - upper left point of node to be built's rectangle.
 * @param w - width of node to be built's rectangle.
 * @param h - height of node to be built's rectangle.
 */
RGBAPixel AverageColour(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h);

/**
 * @param node 
*/
void RenderHelper(Node* node, PNG& image) const;

/**
 * 
*/
void PruneHelper(Node*& node, double tol);

/**
 * 
*/
bool ShouldPrune(Node* node, RGBAPixel avg, double tol) const;

/**
 * 
*/
void FlipHorizontalHelper(Node* node);

/**
 * 
*/
int NumLeavesHelper(Node* node) const;

/**
 * 
*/
void ClearHelper(Node*& node);

Node* CopyHelper(Node* otherNode);
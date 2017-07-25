//
// Created by wang on 7/25/17.
//

#ifndef LIST_MAP_H
#define LIST_MAP_H

/*
 *  a self stl-like associative container written by wx.
 *
 */


/*
 * from The Annotated STL Sources:
 *  in normal case, the inner structure of associative container
 *  implemented with balanced binary tree including AVL-tree which of
 *  widely use, RB-tree, AA-tree gains better efficiency.
 */


/*
 *  about red-black tree:
 *      1. a node is either red or black
 *      2. the root node is black
 *      3. if a node is red, then its children are black
 *      4. paths from any node to NULL conclude same numbers of nodes
 *  so, according to rule 4, new node must be red(but the leaf node
 *  can be black), and to rule 3, new node's parent must be black.
 */



#endif //LIST_MAP_H

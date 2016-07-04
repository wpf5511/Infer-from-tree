//
// Created by 王鹏飞 on 16/6/23.
//

#ifndef INFER_FROM_TREE_PATTERNTREE_H
#define INFER_FROM_TREE_PATTERNTREE_H

#include <vector>
#include "GenericTree.h"

class PatternNode {
public:
    std::string lexeme;
    std::string pos;
    std::string dependency;

    std::vector<PatternNode*> Pchildren;

    PatternNode* parent;

};


#endif //INFER_FROM_TREE_PATTERNTREE_H

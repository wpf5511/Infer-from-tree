//
// Created by 王鹏飞 on 16/6/23.
//

#ifndef INFER_FROM_TREE_TEMPLATETREE_H
#define INFER_FROM_TREE_TEMPLATETREE_H

#include "PatternTree.h"
#include "ZparTree.h"
#include <map>


typedef std::tuple<std::string,std::string> s2tuple;


class TemplateNode{
public:

    //int n_args;

    std::vector<TemplateNode*> Args;

    TemplateNode* parent;

    std::vector<TemplateNode*> Others;

    std::vector<TemplateNode*> Coors;

    TemplateNode(const ZparNode& znode);

    bool is_arg(TemplateNode* cnode);

    int get_id();

private:

    int id;//index which denote appearance order in a sentence
    std::string lexeme;
    std::string pos;
    int parent_id;
    std::string dependency;

    static std::map<s2tuple,bool> arg_map;


};


class TemplateTree{
public:
    TemplateNode *root;

    void Convert_from_Zpar(ZparTree ztree);

    bool is_coor(TemplateNode* cnode);

private:

    static std::map<std::string,int> coor_map;
};


#endif //INFER_FROM_TREE_TEMPLATETREE_H

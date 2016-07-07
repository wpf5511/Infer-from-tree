//
// Created by 王鹏飞 on 16/6/23.
//

#ifndef INFER_FROM_TREE_TEMPLATETREE_H
#define INFER_FROM_TREE_TEMPLATETREE_H

#include "ZparTree.h"
#include <map>


//typedef std::tuple<std::string,std::string> s2tuple;


struct judge_arg{
    std::string cpos;
    std::string ppos;
    std::string dependency;
    bool necessary;

    judge_arg(std::string s1,std::string s2,std::string s3,bool flag){
        cpos = s1;
        ppos = s2;
        dependency = s3;
        necessary = flag;
    }

    bool operator <(const judge_arg& other){
        if(cpos!=other.cpos){
            return cpos<other.cpos;
        }
        else if(ppos!=other.ppos){
            return ppos<other.ppos;
        }
        else if(dependency!=other.dependency){
            return dependency<other.dependency;
        }
        else{
            return necessary<other.necessary;
        }
    }

};

class TemplateNode{
public:

    //int n_args;

    std::vector<TemplateNode*> Args;

    TemplateNode* parent;

    std::vector<TemplateNode*> Children;

    std::vector<TemplateNode*> Coors;

    std::vector<TemplateNode*> Preps;

    TemplateNode(const ZparNode& znode);

    bool is_arg();

    int get_Id();

private:

    int id;//index which denote appearance order in a sentence
    std::string lexeme;
    std::string pos;
    int parent_id;
    std::string dependency;

    static std::map<judge_arg,bool> arg_map;


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

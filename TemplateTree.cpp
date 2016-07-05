//
// Created by 王鹏飞 on 16/6/23.
//

#include "TemplateTree.h"
#include <queue>

std::map<judge_arg,bool> TemplateNode::arg_map{
        {judge_arg("NT","NR","", false), false},
        {judge_arg("NT","VV","", false), false},
        {judge_arg("NT","NN","", false), false},
        {judge_arg("NT","VC","SBJ", true), true},
        {judge_arg("NT","VE","VMOD", true), false},

        {judge_arg("NR","NR","", false), false},
        {judge_arg("NR","VV","", false), true},
        {judge_arg("NR","NN","NMOD", true), false},
        {judge_arg("NR","VC","SBJ", true), true},
        {judge_arg("NR","VE","", false), true},


        {judge_arg("NN","NR","NMOD", true), false},
        {judge_arg("NN","VC","", false), true},
        {judge_arg("NN","VV","", false), true},
        {judge_arg("NN","LB","SBJ", true), true},
        {judge_arg("NN","NN","NMOD", true), false},
        {judge_arg("NN","M","SBJ", true), true},
        {judge_arg("NN","VA","", false), true},
        {judge_arg("NN","VE","", false), true}

};

std::map<std::string,int> TemplateTree::coor_map;

int TemplateNode::get_Id() {
    return id;
}

void TemplateTree::Convert_from_Zpar(ZparTree ztree) {

    ZparNode root_node = ztree.get_Node(ztree.root_id);

    root = new TemplateNode(root_node);

    root->parent = nullptr;

    std::queue<TemplateNode*> TNode_queue;

    TNode_queue.push(root);

    while(!TNode_queue.empty()){

        TemplateNode* current_TNode = TNode_queue.front();

        TNode_queue.pop();

        int current_id = current_TNode->get_Id();

        std::vector<int> children_id = ztree.get_children(current_id);


        for(int i=0;i<children_id.size();i++){

            ZparNode child_node = ztree.get_Node(children_id[i]);

            TemplateNode *child_TNode = new TemplateNode(child_node);

            child_TNode->parent = current_TNode;

            if(child_TNode->is_arg()){               //is arguments
                current_TNode->Args.push_back(child_TNode);
            }else{                                  // not arguments
                current_TNode->Others.push_back(child_TNode);
            }

            TNode_queue.push(child_TNode);
        }

    }

}


bool TemplateNode::is_arg() {
    std::string cpos = this->pos;

    TemplateNode *pnode = this->parent;

    std::string ppos = pnode->pos;

    std::string dependency = this->dependency;

    judge_arg judge_node1 = judge_arg(cpos,ppos,dependency, true);
    judge_arg judge_node2 = judge_arg(cpos,ppos,"", false);

    bool res = arg_map[judge_node1]||arg_map[judge_node2];


    return res;

}

TemplateNode::TemplateNode(const ZparNode& znode) {

    this->id = znode.id;
    this->lexeme = znode.lexeme;
    this->pos = znode.pos;
    this->parent_id = znode.parent_id;
    this->dependency = znode.dependency;

}
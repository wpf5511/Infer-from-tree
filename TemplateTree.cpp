//
// Created by 王鹏飞 on 16/6/23.
//

#include "TemplateTree.h"
#include <tuple>
#include <queue>

std::map<s2tuple,bool> TemplateNode::arg_map{
        {std::make_tuple("vv","nn"),1}
};

std::map<std::string,int> TemplateTree::coor_map;

int ::TemplateNode::get_id() {
    return id;
}

void::TemplateTree::Convert_from_Zpar(ZparTree ztree) {

    ZparNode root_node = ztree.get_Node(ztree.root_id);

    root = new TemplateNode(root_node);

    root->parent = nullptr;

    std::queue<TemplateNode*> TNode_queue;

    TNode_queue.push(root);

    while(!TNode_queue.empty()){

        TemplateNode* current_TNode = TNode_queue.front();

        TNode_queue.pop();

        int current_id = current_TNode->get_id();

        std::vector<int> children_id = ztree.get_children(current_id);


        for(int i=0;i<children_id.size();i++){

            ZparNode child_node = ztree.get_Node(children_id[i]);

            TemplateNode *child_TNode = new TemplateNode(child_node);

            child_TNode->parent = current_TNode;

            current_TNode->Others.push_back(child_TNode);

            TNode_queue.push(child_TNode);
        }

    }

}


bool TemplateNode::is_arg(TemplateNode *cnode) {
    std::string cpos = cnode->pos;

    TemplateNode *pnode = cnode->parent;

    std::string ppos = pnode->pos;

    s2tuple pc_pos = std::make_tuple(ppos,cpos);

    bool res = arg_map[pc_pos];

    return res;

}

TemplateNode::TemplateNode(const ZparNode& znode) {

    this->id = znode.id;
    this->lexeme = znode.lexeme;
    this->pos = znode.pos;
    this->parent_id = znode.parent_id;
    this->dependency = znode.dependency;

}
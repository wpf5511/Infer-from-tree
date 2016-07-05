//
// Created by 王鹏飞 on 16/6/23.
//

#include "ZparTree.h"
#include <sstream>
#include <iostream>

//ZparNode method

int ZparNode::get_parent() {
    return this->parent_id;
}

int ZparNode::get_id(){
    return this->id;
}

ZparNode::ZparNode(std::string lexeme, std::string pos, int parent_id, std::string dependency) {

    this->lexeme = lexeme;
    this->pos = pos;
    this->parent_id =parent_id;
    this->dependency = dependency;
}

ZparNode::ZparNode(const ZparNode &node) {

    this->id = node.id;
    this->lexeme = node.lexeme;
    this->pos = node.pos;
    this->parent_id = node.parent_id;
    this->dependency = node.dependency;
}



std::string ZparNode::get_pos() {
    return this->pos;
}

std::string ZparNode::get_dependency() {
    return this->dependency;
}

std::string ZparNode::get_lexeme() {
    return this->lexeme;
}




//ZparTree method

void ZparTree::add_node(ZparNode node2) {

    int j = this->nodes.size();

    node2.id = j;


    if(node2.parent_id==-1){
        root_id=j;
    }

    this->nodes.push_back(node2);

}

void ZparTree::set_children_array() {

    for(int i=0;i<nodes.size();i++){
        if(nodes[i].get_parent()!=-1){
            children_array[nodes[i].get_parent()].push_back(i);
        }
    }

}

std::vector<int> ZparTree::get_children(int id) {

    return children_array[id];  //at 处理单独成句的会出现异常

}


ZparNode ZparTree::get_Node(int id) {
    return nodes[id];
}

std::string ZparTree::to_sentence() {
    std::ostringstream out_sen;

    for(int i=0;i<nodes.size();i++){
        out_sen<<nodes[i].get_lexeme()<<" ";
    }
    return out_sen.str();
}

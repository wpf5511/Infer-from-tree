//
// Created by 王鹏飞 on 16/7/14.
//

#include "InferenceGraph.h"

int main(){
    InferenceGraph inferenceGraph;

    ifstream Zparfile("/Users/wangpf/Downloads/reference-4.txt",ios::in);

    string line;
    string lexeme,pos,parent_id,dependency;

    ZparTree *zparTree=new ZparTree();

    while(getline(Zparfile,line)){
        if(line.empty()){
            zparTree->set_children_array();
            zpars.push_back(*zparTree);
            delete zparTree;
            zparTree = new ZparTree();
        }
        else{
            istringstream is(line);
            is>>lexeme>>pos>>parent_id>>dependency;

            zparTree->add_node(ZparNode(lexeme,pos,stoi(parent_id),dependency));
        }
    }
}
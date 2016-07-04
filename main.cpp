#include <iostream>
#include <fstream>
#include <sstream>
#include "ZparTree.h"
using namespace std;

void search_zpartree(ZparTree ztree){

    int root_id = ztree.root_id;

    ZparNode root_Node = ztree.get_Node(root_id);

    vector<int>root_children=ztree.get_children(root_id);

    //ofstream root_print("/Users/wangpf/Downloads/root-print",ios::app);

    //root_print<<root_Node.lexeme<<"\t"<<root_Node.pos<<"\t"<<root_Node.parent_id<<"\t"<<root_Node.dependency<<"\n";

    ofstream   be_children("/Users/wangpf/Downloads/NT_parent",ios::app);


   // if(root_Node.pos=="VA"){

        //cout<<ztree.to_sentence()<<endl;

        for(int i=0;i<root_children.size();i++){

            ZparNode node = ztree.get_Node(root_children[i]);

            if(node.pos=="NT"){
                be_children<<root_Node.lexeme<<"\t"<<root_Node.pos<<"\t"<<node.parent_id<<"\t"<<node.lexeme<<"\t"<<node.pos<<"\t"<<node.id<<"\t"<<node.dependency<<endl;
            }

        }
       // cout<<"----------------"<<"\n";
    //}


}
int main() {

   ifstream Zparfile("/Users/wangpf/Downloads/reference-44.txt",ios::in);

    string line;
    string lexeme,pos,parent_id,dependency;

    std::vector<ZparTree> zpars;

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

    cout<<zpars.size()<<endl;

    for_each(zpars.begin(),zpars.end(),search_zpartree);
}
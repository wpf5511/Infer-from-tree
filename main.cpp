#include <iostream>
#include <fstream>
#include <sstream>
#include "InferenceGraph.h"
using namespace std;

void search_zpartree(ZparTree ztree){

    int root_id = ztree.root_id;

    ZparNode root_Node = ztree.get_Node(root_id);

    vector<int>root_children=ztree.get_children(root_id);

    //ofstream root_print("/Users/wangpf/Downloads/root-print",ios::app);

    //root_print<<root_Node.lexeme<<"\t"<<root_Node.pos<<"\t"<<root_Node.parent_id<<"\t"<<root_Node.dependency<<"\n";

    ofstream   be_children("/Users/wangpf/Downloads/NR_children",ios::app);


    if(root_Node.pos=="NR"){

        //be_children<<ztree.to_sentence()<<endl;

        for(int i=0;i<root_children.size();i++){

            ZparNode node = ztree.get_Node(root_children[i]);

                be_children<<root_Node.lexeme<<"\t"<<root_Node.pos<<"\t"<<node.parent_id<<"\t"<<node.lexeme<<"\t"<<node.pos<<"\t"<<node.id<<"\t"<<node.dependency<<endl;

        }
       // cout<<"----------------"<<"\n";
    }
}

void search_zpartree1(ZparTree ztree){

    int root_id = ztree.root_id;

    ZparNode root_Node = ztree.get_Node(root_id);

    vector<int>root_children=ztree.get_children(root_id);

    ofstream   be_children("/Users/wangpf/Downloads/VE_parent",ios::app);


        //cout<<ztree.to_sentence()<<endl;

        for(int i=0;i<root_children.size();i++){

            ZparNode node = ztree.get_Node(root_children[i]);

            if(node.pos=="VE"){
                be_children<<root_Node.lexeme<<"\t"<<root_Node.pos<<"\t"<<node.parent_id<<"\t"<<node.lexeme<<"\t"<<node.pos<<"\t"<<node.id<<"\t"<<node.dependency<<endl;
            }

        }
        // cout<<"----------------"<<"\n";

}

void search_zpartree2(ZparTree ztree){

    ofstream   be_children("/Users/wangpf/Downloads/LC_allparent",ios::app);

    for(int i=0;i<ztree.nodes.size();i++){

        ZparNode znode = ztree.get_Node(i);
        if(znode.pos=="LC"){
            int parent_id = znode.parent_id;
            if(parent_id!=-1)
            {
                ZparNode pnode = ztree.get_Node(parent_id);
                be_children<<pnode.lexeme<<"\t"<<pnode.pos<<"\t"<<znode.parent_id<<"\t"<<znode.lexeme<<"\t"<<znode.pos<<"\t"<<znode.id<<"\t"<<znode.dependency<<endl;
            }
        }
    }
    be_children.close();
}

void search_zpartree3(ZparTree ztree){

    ofstream be_children("/Users/wangpf/Downloads/LC_allchildren",ios::app);

    for(int i=0;i<ztree.nodes.size();i++) {

        ZparNode pnode = ztree.get_Node(i);

        if (pnode.pos == "LC") {

        vector<int> children_id = ztree.get_children(i);

           // if(children_id.size()>=2){
        for (int j = 0; j < children_id.size(); j++) {
                ZparNode cnode = ztree.get_Node(children_id[j]);
                 be_children << pnode.lexeme << "\t" << pnode.pos << "\t" << cnode.parent_id << "\t" << cnode.lexeme <<
                 "\t" << cnode.pos << "\t" << cnode.id << "\t" << cnode.dependency << endl;
               /* if(cnode.pos=="VV"){
                    be_children<<ztree.to_sentence()<<endl;
                    break;
                }*/
            }
                //be_children<<ztree.to_sentence()<<endl;
            //be_children<<"-"<<"\t"<<"-"<<"\t"<<"-2"<<"\t"<<"-"<<"\t"<<"-"<<"\t"
            //<<"-2"<<"\t"<<"-"<<endl;

      //  }

        }
    }
    be_children.close();
}

int main() {

   ifstream Zparfile("/Users/wangpf/Downloads/reference-4.txt",ios::in);

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


    //for_each(zpars.begin(),zpars.end(),search_zpartree3);

    InferenceGraph inferenceGraph;



    inferenceGraph.Convert_from_Zpar(zpars[0]);

    inferenceGraph.PrintEdge();




}
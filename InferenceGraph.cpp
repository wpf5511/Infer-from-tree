//
// Created by 王鹏飞 on 16/7/13.
//

#include "InferenceGraph.h"
#include <iostream>
#include <vector>

std::map<judge_pre,bool> InferenceGraph::pre_map{


        {judge_pre("VC","NT","SBJ", true, true), true},


        {judge_pre("VV","NR","", false, true), true},

        {judge_pre("VC","NR","SBJ", true, true), true},
        {judge_pre("VE","NR","", false, true), true},



        {judge_pre("VC","NN","", false, true), true},
        {judge_pre("VV","NN","", false, true), true},
        {judge_pre("LB","NN","SBJ", true, true), true},//需要检验

        {judge_pre("M","NN","SBJ", true, true), true},//需要检验
        {judge_pre("VA","NN","", false, true), true},
        {judge_pre("VE","NN","", false, true), true},

        {judge_pre("VV","VA","SBJ", true, true), true},//有vv的情况注意处理情态
        {judge_pre("VV","VA","OBJ", true, true), true},

        {judge_pre("VV","VC","OBJ", true, true), true},

        {judge_pre("VV","VE","SBJ", true, true), true},
        {judge_pre("VV","VE","OBJ", true, true), true},
        {judge_pre("VC","VE","SBJ", true, true), true},

        {judge_pre("VV","VV","SBJ", true, true), true},
        {judge_pre("VV","VV","OBJ", true, true), true},
        {judge_pre("VC","VV","SBJ", true, true), true},


        {judge_pre("NN","NN","NMOD", true, false), true}, //false means down
        {judge_pre("NR","NN","NMOD", true, false), true}, // false means down
        {judge_pre("JJ","NN","NMOD", true, false), true}, //false means down
        {judge_pre("VV","NN","NMOD", true, false), true}, // false means down

        {judge_pre("NR","NR","NMOD", true, false), true}, //false means down
        {judge_pre("NN","NR","NMOD", true, false), true},//false means down
        {judge_pre("VA","NN","NMOD", true, false),true}

};

std::map<judge_coor,bool> InferenceGraph::coor_map{
        {judge_coor("VA","VA","COOR"), true},
        {judge_coor("VA","VA","VMOD"), true},
        {judge_coor("VV","VA","VMOD"), true},

        {judge_coor("VV","VC","VMOD"), true},
        {judge_coor("VC","VC","VMOD"), true},

        {judge_coor("VV","VE","VMOD"), true},

        {judge_coor("VV","VV","COOR"), true},
        {judge_coor("VV","VV","VMOD"), true}  //todo NMOD并列的处理

};

void InferenceGraph::ProcessDependencyNode(ZparNode znode,ZparTree& ztree){


    judge_process_predicate(znode,ztree);


    std::vector<int> children_id = ztree.get_children(znode.id);

    for (int i = 0; i < children_id.size(); i++){

        int child_id = children_id[i];

        ProcessDependencyNode(ztree.get_Node(child_id),ztree);

    }

}

void InferenceGraph::CreateVertices(ZparTree ztree) {

    int ztree_size = ztree.nodes.size();

    VertexNum = ztree_size;

    adjList = new Vertex[VertexNum+10];  //以防添加节点的情况

    for(int i=0;i<ztree.nodes.size();i++){

        ZparNode znode = ztree.get_Node(i);

        adjList[i] = Vertex(znode);
    }
}

//判断是不是predicate  参数当前node 和 相邻node,以及当前node的位置,true means up
bool InferenceGraph::is_predicate(ZparNode cur_node, ZparNode nb_node, bool direction) {

    std::string cur_pos = cur_node.pos;

    std::string cur_dependency = cur_node.dependency;

    std::string nb_pos = nb_node.pos;

    std::string nb_dependency = nb_node.dependency;

    if(direction == true){

        judge_pre judge_with_dep = judge_pre(cur_pos,nb_pos,nb_dependency, true, true);
        judge_pre judge_without = judge_pre(cur_pos,nb_pos,"", false, true);

        bool res =pre_map[judge_with_dep]||pre_map[judge_without];

        return  res;

    } else{
        judge_pre judge_with_dep = judge_pre(cur_pos,nb_pos,cur_dependency, true, false);
        judge_pre judge_without = judge_pre(cur_pos,nb_pos,"", false, false);

        bool res = pre_map[judge_with_dep]||pre_map[judge_without];
        return  res;

    }

}

void InferenceGraph::add_DepEdge(ZparNode from,ZparNode to){



    Dep_Edge *dep_edge = new Dep_Edge(to.id,to.dependency);

    Vertex &from_vertex = adjList[from.id];  //parent vertex

    //添加dependency边
    dep_edge->next=from_vertex.first_dep;
    from_vertex.first_dep = dep_edge;

}

void InferenceGraph::add_DepEdge(ZparNode from, ZparNode to,
                                 std::string Dep_relation, bool has_more,int col_nodeid) {

    Dep_Edge *dep_edge= new Dep_Edge(to.id,Dep_relation,has_more,col_nodeid);

    Vertex &from_vertex = adjList[from.id];  //parent vertex

    //添加dependency边
    dep_edge->next=from_vertex.first_dep;

    from_vertex.first_dep = dep_edge;

}

void InferenceGraph::add_DepEdge(int from_id, int to_id, std::string Dep_relation,bool has_more,int collapse_id) {
    Dep_Edge *dep_edge=new Dep_Edge(to_id,Dep_relation);
    Vertex &from_vertex = adjList[from_id];

    //添加dependency边
    dep_edge->next=from_vertex.first_dep;
    from_vertex.first_dep = dep_edge;
}

void InferenceGraph::add_PaEdge(ZparNode from, ZparNode to) {

    Pa_Edge *pa_edge=new Pa_Edge(to.id,"argument");
    Vertex &cur_vertex = adjList[from.id];  //current vertex


    pa_edge->next = cur_vertex.first_arg;
    cur_vertex.first_arg = pa_edge;
}

void InferenceGraph::add_PaEdge(int from_id,int to_id) {

    Pa_Edge *pa_edge=new Pa_Edge(to_id,"argument");
    Vertex &cur_vertex = adjList[from_id];  //current vertex


    pa_edge->next = cur_vertex.first_arg;
    cur_vertex.first_arg = pa_edge;
}

void InferenceGraph::add_AffEdge(ZparNode from,ZparNode to) {

    Aff_Edge *aff_edge=new Aff_Edge(to.id,"affix");

    Vertex &cur_vertex = adjList[from.id]; //current vertex

    aff_edge->next = cur_vertex.first_aff;
    cur_vertex.first_aff = aff_edge;
}

bool InferenceGraph::is_coor(ZparNode from, ZparNode to) {

    std::string cur_pos = from.pos;

    std::string child_pos = to.pos;

    std::string inter_dependency = to.dependency;

    judge_coor judgeCoor = judge_coor(cur_pos,child_pos,inter_dependency);


    return  coor_map[judgeCoor];

}


bool InferenceGraph::is_affix(ZparNode cur_node,ZparNode parent_node) {

    /*std::string cur_pos = cur_node.pos;

    if(cur_pos=="nt"){
        return true;
    }*/
    return true;
}

void InferenceGraph::addcollapse_node(int topid, int middleid, int bottomid) {

    auto top_vertex= adjList[topid];

    Dep_Edge *bottom_edge= nullptr;

    Dep_Edge *middle_edge= nullptr;

    auto p = top_vertex.first_dep;
    while (p!= nullptr){
        if(p->adjvex==bottomid){
            bottom_edge = p;
        }
        if(p->adjvex==middleid){
            middle_edge = p;
        }
        p = p->next;
    }

    if(middle_edge!= nullptr){

        auto middle_colids = middle_edge->collapse_nodeids;
        for(int i=1;i<middle_colids.size();i++){
            bottom_edge->collapse_nodeids.push_back(middle_colids[i]);
        }

    }


}

bool InferenceGraph::handle_prep(ZparNode znode,ZparTree& ztree) {

    std::string cur_pos = znode.pos;

    if(cur_pos!="P"){
        return false;
    } else{
        int cur_id = znode.id;

        int parent_id = znode.parent_id;

        std::vector<int>children_id = ztree.get_children(cur_id);

        for(int i=0;i<children_id.size();i++){
            ZparNode& prep_child = ztree.get_Node(children_id[i]);
            if(prep_can_collapse(prep_child)){
                ZparNode prep_parent = ztree.get_Node(parent_id);

                add_DepEdge(prep_parent,prep_child,znode.dependency, true,znode.id);

                //addcollapse_node(prep_parent.id,znode.id,prep_child);

                /*bool is_pred = is_predicate(prep_parent,prep_child, true);

                if(is_pred){
                    add_PaEdge(prep_parent,prep_child);
                }*/
                prep_child.parent_id = parent_id;
                prep_child.dependency = znode.dependency;

                return true;
            }
        }
    }
}

bool InferenceGraph::handle_lc(ZparNode znode,ZparTree& ztree){
    std::string cur_pos = znode.pos;
    if(cur_pos!="LC"){
        return false;
    } else{

        int cur_id = znode.id;

        int parent_id = znode.parent_id;

        std::vector<int>children_id = ztree.get_children(cur_id);

        for(int i=0;i<children_id.size();i++){
            ZparNode& lc_child = ztree.get_Node(children_id[i]);
            if(lc_can_collapse(lc_child)){
                ZparNode lc_parent = ztree.get_Node(parent_id);

                add_DepEdge(lc_parent,lc_child,znode.dependency, true,znode.id);

                addcollapse_node(lc_parent.id,znode.id,lc_child.id);

               /* bool is_pred = is_predicate(lc_parent,lc_child, true);

                if(is_pred){
                    add_PaEdge(lc_parent,lc_child);
                }*/
                lc_child.parent_id = parent_id;

                return true;
            }
        }
    }
}

bool InferenceGraph::handle_dec(ZparNode znode,ZparTree& ztree) {
    //znode is the dec node
    std::string cur_pos = znode.pos;

    if(cur_pos!="DEC"){
        return false;
    } else{
        int cur_id = znode.id;

        int parent_id = znode.parent_id;  // cause cur is a child , parent exist

        ZparNode dec_parent = ztree.get_Node(parent_id);

        std::string dec_parent_pos = dec_parent.pos;

        //添加节点并坍塌"的"
        if(dec_parent_pos=="VA"||dec_parent_pos=="VC"||dec_parent_pos=="VE"||dec_parent_pos=="VV"){

            int added_id = VertexNum;  int added_position = znode.id+1;//在DEC节点之后

            adjList[VertexNum++]=Vertex(added_id,added_position);

            //add first edge from "的"parent to new added node
            add_DepEdge(parent_id,added_id,znode.dependency);

            std::vector<int>children_id = ztree.get_children(cur_id);

            for(int i=0;i<children_id.size();i++){

                ZparNode &dec_child = ztree.get_Node(children_id[i]);

                if(dec_child.dependency=="DEC"){
                    add_DepEdge(added_id,dec_child.id,"NMOD", true,znode.id);//assume the added node is noun and child as a modifier

                    //处理新节点的pre-arg关系
                    Process_added_node(added_id,dec_parent.id,dec_child.id);

                    dec_child.parent_id = added_id;
                    //dec_child.dependency = "NMOD";// suppose is a noun

                    return true;
                }
            }


        } else if(dec_parent_pos=="NN"||dec_parent_pos=="NR"){ //只坍塌节点
            std::vector<int>children_id = ztree.get_children(cur_id);

            for(int i=0;i<children_id.size();i++){
                ZparNode & dec_child = ztree.get_Node(children_id[i]);

                if(dec_child.dependency=="DEC"){
                    add_DepEdge(dec_parent,dec_child,znode.dependency, true,znode.id);

                    dec_child.parent_id = parent_id;
                    dec_child.dependency = znode.dependency;

                    return true;
                }
            }
        }
    }
}

//暂定新添加的节点是arg,上下两个一元模版,增加pre-arg边,指向added_node
void InferenceGraph::Process_added_node(int added_id,int parent_id,int child_id){
    //suppose added node is noun ,and also cause dec parent is verb,and for dec_child which has "DEC" relation is also verb
    //so I assume the added node is a argument
    add_PaEdge(parent_id,added_id);
     add_PaEdge(child_id,added_id);
}

bool  InferenceGraph::prep_can_collapse(ZparNode prep_child) {
    std::string pos = prep_child.pos;
    std::string dependency = prep_child.dependency;
    if(pos=="NN"||pos=="NR"||pos=="VV"||pos=="LC"){
        if(dependency=="POBJ"){
            return true;
        }
    }
    return false;
}

bool  InferenceGraph::lc_can_collapse(ZparNode lc_child) {
    std::string pos = lc_child.pos;
    std::string dependency = lc_child.dependency;
    if(pos=="NN"||pos=="VV"){
        if(dependency=="LC"){
            return true;
        }
    }
    return false;
}





bool InferenceGraph::judge_process_predicate(ZparNode znode, ZparTree& ztree) {

    int parent_id = znode.parent_id;

    int node_id = znode.id;

    if(parent_id!=-1&&parent_id<ztree.nodes.size()){

        ZparNode pnode = ztree.get_Node(parent_id);

        bool is_pred = is_predicate(znode,pnode, false);

        //添加pre-arg边
        if(is_pred){
           add_PaEdge(znode,pnode);
        }
    }

    std::vector<int> children_id = ztree.get_children(node_id);

    bool is_down_pred = false;
    std::vector<std::tuple<ZparNode,ZparNode>> aff_edge;

    for(int i=0;i<children_id.size();i++){

        int child_id = children_id[i];

        ZparNode child_node = ztree.get_Node(child_id);

        //handle preposition
        bool handle_prep_res = handle_prep(child_node,ztree);//判断child_node是不是介词,后边接有宾语可坍塌的情况

        bool handle_lc_res = handle_lc(child_node,ztree);

        bool handle_dec_res =  handle_dec(child_node,ztree);//判断child_node 是不是dec情况.

        int true_znodeid = child_node.parent_id; //if doesn't collapse should be the same with znode.id

        bool same_node = (true_znodeid==znode.id);// 如果不相等,说明childnode是介词的宾语,znode是介词

        if(!same_node){

            znode = ztree.get_Node(true_znodeid);

        }

        if(!handle_prep_res&&same_node&&!handle_dec_res&&!handle_lc_res){
            //添加dependency边
            add_DepEdge(znode,child_node);// dependency from parent to child
        }

        bool is_pred = is_predicate(znode,child_node, true);


        //添加pre-arg边
        if(is_pred){
            add_PaEdge(znode,child_node);
            is_down_pred = true;
        }
        else{

            //判断子节点是不是并列
            bool is_coo = is_coor(znode,child_node);

            //并列处理
            if(is_coo){
                //handle coor
            } else{
                bool is_aff = is_affix(child_node,znode);
                auto zc_tuple = std::make_tuple(znode,child_node);
                if(is_aff&!handle_prep_res&&!handle_dec_res&&!handle_lc_res){
                    //添加pre-aff边
                    //add_AffEdge(znode,child_node);
                    aff_edge.push_back(zc_tuple);
                }
            }
        }
    }
    if(is_down_pred){
        for(int i=0;i<aff_edge.size();i++){
            add_AffEdge(std::get<0>(aff_edge[i]),std::get<1>(aff_edge[i]));
        }
    }
}

void InferenceGraph::Convert_from_Zpar(ZparTree ztree) {

    ZparNode root_node = ztree.get_Node(ztree.root_id);

    CreateVertices(ztree);

    ProcessDependencyNode(root_node,ztree);

}

void InferenceGraph::PrintEdge() {
    for(int i=0;i<VertexNum;i++){
        auto vertex = adjList[i];
        auto p1=vertex.first_arg;
        auto p2=vertex.first_aff;

        while (p1!= nullptr){
            std::cout<<adjList[i].lexeme<<"--->"<<adjList[p1->adjvex].lexeme<<"   "<<p1->Pa_relation<<std::endl;
            p1=p1->next;
        }
        while (p2!= nullptr){
            std::cout<<adjList[i].lexeme<<"--->"<<adjList[p2->adjvex].lexeme<<"   "<<p2->Aff_relation<<std::endl;
            p2=p2->next;
        }
    }
}

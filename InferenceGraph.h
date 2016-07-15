//
// Created by 王鹏飞 on 16/7/13.
//

#ifndef INFER_FROM_TREE_INFERENCEGRAPH_H
#define INFER_FROM_TREE_INFERENCEGRAPH_H

#include <vector>
#include <string>
#include "ZparTree.h"

struct Dep_Edge{

    int adjvex;
    std::string Dep_relation;//depdency relation

    bool has_vertex_info;

    int collapse_nodeid;

    Dep_Edge *next;

    Dep_Edge(int target,std::string dependency,bool has_info= false,int col_id=-1){

        this->adjvex = target;
        this->Dep_relation = dependency;
        this->collapse_nodeid = col_id;
        this->has_vertex_info = false;
        next = nullptr;
    }
};

struct Pa_Edge{

    int adjvex;
    std::string Pa_relation;
    Pa_Edge *next;

    Pa_Edge(int target,std::string dependency){

        this->adjvex = target;
        this->Pa_relation = dependency;
        next = nullptr;
    }


};

struct Aff_Edge{

    int adjvex;
    std::string Aff_relation;
    Aff_Edge *next;

    Aff_Edge(int target,std::string dependency){

        this->adjvex = target;
        this->Aff_relation = dependency;
        next = nullptr;
    }

};

struct Vertex{

    int id;

    int sentence_position; //词在句子中的位置
    std::string lexeme;
    std::string pos;
    bool can_be_pre;
    bool can_be_arg;
    bool is_empty_node;


    Dep_Edge* first_dep;
    Pa_Edge* first_arg;
    Aff_Edge* first_aff;

    Vertex(int Id,int sentence_position){
        this->id = Id;
        this->sentence_position = sentence_position;
        is_empty_node = true;
        first_dep = nullptr;
        first_arg = nullptr;
        first_aff = nullptr;
    }

    Vertex(){
        id = -1;
        first_dep = nullptr;
        first_arg = nullptr;
        first_aff = nullptr;
    }

    Vertex(ZparNode znode){
        this->id  = znode.id;
        this->sentence_position = znode.id;
        this->lexeme = znode.lexeme;
        this->pos = znode.pos;
        can_be_arg = false;
        can_be_pre = false;
        is_empty_node = false;


        this->first_dep= nullptr;   //initialize_edge
        this->first_arg = nullptr;
        this->first_aff = nullptr;
    }



    bool operator <(const Vertex& other){
        return this->id<other.id;
    }
};


struct judge_pre{
    std::string cpos; //current postag
    std::string npos; // neighbot postag
    std::string dependency; //dependency between two node
    bool necessary;   // is dependency field necessary
    bool direction;// true means current at up and false means current at down

    judge_pre(std::string cpos,std::string npos,std::string dependency,bool necessary,bool direction)
    {
        this->cpos = cpos;
        this->npos = npos;
        this->dependency = dependency;
        this->direction = direction;
        this->necessary = necessary;
    }

    bool operator <(const judge_pre& other)const{
        if(cpos!=other.cpos){
            return cpos<other.cpos;
        }
        else if(npos!=other.npos){
            return npos<other.npos;
        }
        else if(dependency!=other.dependency){
            return dependency<other.dependency;
        }
        else if(necessary!=other.necessary){
            return necessary<other.necessary;
        }
        else{
            return this->direction<other.direction;
        }
    }

};

struct judge_coor{
    std::string cur_pos;
    std::string child_pos;
    std::string dependency;

    judge_coor(std::string cur_pos,std::string child_pos,std::string dependency){
        this->cur_pos = cur_pos;
        this->child_pos = child_pos;
        this->dependency = dependency;
    }

    bool operator <(const judge_coor& other)const{
        if(cur_pos!=other.cur_pos){
            return cur_pos<other.cur_pos;
        }
        else if(child_pos!=other.child_pos){
            return child_pos<other.child_pos;
        }
        else{
            return dependency<other.dependency;
        }
    }

};




class InferenceGraph {


    Vertex* adjList;

    int VertexNum;

    int EdgeNum;

    static std::map<judge_pre,bool> pre_map;
    static std::map<judge_coor,bool> coor_map;



public:

    void Convert_from_Zpar(ZparTree ztree);

    void CreateVertices(ZparTree ztree);

    void AddVertex();

    bool is_predicate(ZparNode cur_node,ZparNode nb_node,bool direction);

    bool is_coor(ZparNode from,ZparNode to);

    bool is_affix(ZparNode cur_node,ZparNode parent_node);

    void add_DepEdge(ZparNode from,ZparNode to);

    void add_DepEdge(ZparNode from,ZparNode to,std::string Dep_relation,bool has_more= false,int collapse_id=-1);

    void add_DepEdge(int from_id,int to_id,std::string Dep_relation,bool has_more= false,int collapse_id=-1);

    void add_PaEdge(ZparNode from,ZparNode to);

    void add_PaEdge(int from_id,int to_id);

    void add_AffEdge(ZparNode from,ZparNode to);

    bool handle_prep(ZparNode znode,ZparTree ztree);

    bool handle_dec(ZparNode znode,ZparTree ztree);

    bool can_collapse(ZparNode prep_child);

    void Process_added_node(int added_id,int parent_id,int child_id);

    void ProcessDependencyNode(ZparNode znode,ZparTree ztree);

    bool judge_process_predicate(ZparNode znode, ZparTree ztree);

    void PrintEdge();



};


#endif //INFER_FROM_TREE_INFERENCEGRAPH_H

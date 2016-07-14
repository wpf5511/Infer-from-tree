//
// Created by 王鹏飞 on 16/6/23.
//

#ifndef INFER_FROM_TREE_PATTERNTREE_H
#define INFER_FROM_TREE_PATTERNTREE_H

#include <vector>


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
    TemplateNode(const ZparNode& znode);

public:
    int id;
    std::string lexeme;
    std::string pos;
};

class ArgumentNode{
public:
    ArgumentNode(const ZparNode& znode);

    ArgumentNode(int id,std::string lexeme,std::string pos);

public:
    int id;
    std::string lexeme;
    std::string pos;
};

class CommonNode{
public:
    CommonNode(const ZparNode& znode);

    std::string get_pos();

public:
    int id;
    std::string lexeme;
    std::string pos;
};

struct Edge{

    int adjvex;
    std::string relation;
    Edge* next;

    Edge(int adjvex,std::string dependency){

        this->adjvex = adjvex;
        this->relation = dependency;
        next = nullptr;
    }
};

struct Vertex{
    int id;
    Edge* first_edge;

    Vertex(int Id){
        id = Id;
        first_edge = nullptr;
    }

    Vertex(){
        first_edge = nullptr;
    }

    bool operator <(const Vertex& other){
        return this.id<other.id;
    }
};

struct Position{
    int type;
    int index;

    Position(int i,int j){
        type = i;
        index = j;
    }
};

class ALGraph{

    std::vector<TemplateNode> templatenodes;

    std::vector<ArgumentNode> argumentnodes;

    std::vector<CommonNode>  commonnodes;

    Vertex* adjList;

    Vertex* reverse_adjList;

    int VertexNum;

    int EdgeNum;

    static std::map<judge_arg,bool> arg_map;

    static std::map<judge_pre,bool> pre_map;



public:
    void Convert_from_Zpar(ZparTree ztree);

    void collapse_prep();

    void  handle_dec();

    bool is_arg(ZparNode znode);

    bool is_pre(ZparNode znode);

    std::string get_node_pos(int node_id);

    std::map<int,Position> vertex_index;

};
#endif //INFER_FROM_TREE_PATTERNTREE_H

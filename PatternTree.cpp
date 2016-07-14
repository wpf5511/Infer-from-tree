//
// Created by 王鹏飞 on 16/6/23.
//

#include "PatternTree.h"

std::map<judge_arg,bool> ALGraph::arg_map{
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

/*void ALGraph::Convert_from_Zpar(ZparTree ztree) {


    std::queue<int> id_queue;

    id_queue.push(ztree.root_id);

    while(!id_queue.empty()){

        int  node_id = id_queue.front();

        id_queue.pop();

        ZparNode znode = ztree.get_Node(node_id);

        if(node_id==ztree.root_id){


            TemplateNode  templateNode = TemplateNode(znode);

            templatenodes.push_back(templateNode);

            Position p=Position(0,0);
        }else
        {
            bool is_argument=is_arg(znode,ztree);

            if(is_argument){
                ArgumentNode argumentNode = ArgumentNode(znode);
                argumentnodes.push_back(argumentNode);
                Position p = Position(1,argumentnodes.size()-1);
            }else{
                bool is_predicate = is_pre(znode,ztree);
                if(is_predicate){
                   TemplateNode templateNode = TemplateNode(znode);
                   templatenodes.push_back(templateNode);
                   Position p = Position(0,templatenodes.size()-1);
                } else{
                    CommonNode commonNode = CommonNode(znode);
                    commonnodes.push_back(commonNode);
                    Position p = Position(2,commonnodes.size()-1);
                }
            }
        }
        Vertex vertex = Vertex(node_id);

        vertex_index.insert({node_id,p});

        vertexes.insert(vertex);

        std::vector<int> children_id = ztree.get_children(node_id);

        for(int i=0;i<children_id.size();i++){

            ZparNode child_node = ztree.get_Node(children_id[i]);

            int child_id = children_id[i];

            std::string dependency = child_node.dependency;

            Edge* edge_node = new Edge(child_id,dependency);

            // insert edge node
            edge_node->next = vertex.first_edge;
            vertex.first_edge = edge_node;

            id_queue.push(children_id[i]);
        }

    }

}*/

void ALGraph::Convert_from_Zpar(ZparTree ztree) {

     VertexNum = ztree.nodes.size();

     adjList = new Vertex[VertexNum+10];   //以防以后添加节点,如处理"...的"的情况.

     reverse_adjList = new Vertex[VertexNum+10];

    std::queue<int> id_queue;

    id_queue.push(ztree.root_id);

    while(!id_queue.empty()){

        int  node_id = id_queue.front();

        id_queue.pop();

        ZparNode znode = ztree.get_Node(node_id);

        if(node_id==ztree.root_id){


            TemplateNode  templateNode = TemplateNode(znode);

            templatenodes.push_back(templateNode);

            Position p=Position(0,0);
            Vertex vertex = Vertex(node_id);

            vertex_index.insert({node_id,p});

            adjList[node_id]=vertex;
            reverse_adjList[node_id]=vertex;
        }

        std::vector<int> children_id = ztree.get_children(node_id);

        for(int i=0;i<children_id.size();i++){

            ZparNode child_node = ztree.get_Node(children_id[i]);

            bool is_argument=is_arg(child_node,ztree);

            if(is_argument){
                ArgumentNode argumentNode = ArgumentNode(child_node);
                Position p = Position(1,argumentnodes.size());
                argumentnodes.push_back(argumentNode);
            }else{
                bool is_predicate = is_pre(child_node,ztree);
                if(is_predicate){
                    TemplateNode templateNode = TemplateNode(child_node);
                    Position p = Position(0,templatenodes.size());
                    templatenodes.push_back(templateNode);
                } else{
                    CommonNode commonNode = CommonNode(child_node);
                    Position p = Position(2,commonnodes.size());
                    commonnodes.push_back(commonNode);
                }
            }

            Vertex vertex = Vertex(children_id[i]);

            vertex_index.insert({children_id[i],p});


            adjList[children_id[i]]=vertex;

            reverse_adjList[children_id[i]]=vertex;


            int child_id = children_id[i];

            std::string dependency = child_node.dependency;


            Vertex &iter = adjList[node_id];

            Vertex &r_iter = reverse_adjList[child_id];

            /*if(is_predicate&&is_arg(znode)){
                Edge *edge_node = new Edge(node_id, dependency);

                // insert edge node
                edge_node->next = vertex.first_edge;
                vertex.first_edge = edge_node;
            }*/
            //else {
                // insert edge node to adjList
                Edge *edge_node = new Edge(child_id, dependency);
                edge_node->next = iter.first_edge;
                iter.first_edge = edge_node;
            //}

                // insert edge node to reverse_adjList
                Edge *reverse_edge = new Edge(node_id,dependency);
                reverse_edge->next = r_iter.first_edge;
                r_iter.first_edge = reverse_edge;

            id_queue.push(children_id[i]);
        }

    }

}


bool ALGraph::is_arg(ZparNode znode,ZparTree ztree) {
    std::string cpos = znode.pos;

    int parent_id = znode.parent_id;

    ZparNode pnode = ztree.get_Node(parent_id);


    std::string ppos = pnode.pos;

    std::string dependency = znode.dependency;

    judge_arg judge_node1 = judge_arg(cpos,ppos,dependency, true);
    judge_arg judge_node2 = judge_arg(cpos,ppos,"", false);

    bool res = arg_map[judge_node1]||arg_map[judge_node2];


    return res;

}

bool  ALGraph::is_pre(ZparNode znode,ZparTree ztree) {

    std::string pos = znode.pos;

    if(pos=="VA"||pos=="VC"||pos=="VE"||pos=="VV"||pos="JJ"){
        return true;
    }
    else{
        return false;
    }

}

void ALGraph::collapse_prep() {

    std::vector<int> prep_list;
    for(int i=0;i<commonnodes.size();i++){
        CommonNode commonNode = commonnodes[i];

        if(commonNode.get_pos()=="P"){
            //获得介词节点的邻接表
            auto prep_vertex = adjList[commonNode.id];
            Edge *edge = prep_vertex.first_edge;
            Edge *previous = nullptr;
            while(edge!= nullptr){
                if(edge->relation=="POBJ"){
                    int child_id = edge->adjvex;
                    std::string node_pos = get_node_pos(edge->adjvex);
                    if(node_pos=="NN"||node_pos=="NR"||node_pos=="LC"||node_pos=="VV"){
                        //若果介词有宾语,满足坍塌条件,再去找介词的parent
                        auto r_prep_vertex = reverse_adjList[commonNode.id];
                        Edge *r_edge = r_prep_vertex.first_edge;

                        int parent_id = r_edge->adjvex;

                        //逆邻接表,一个点只有一条逆邻接边,有逆邻接边进行坍塌
                        if(r_edge!= nullptr){
                            //删除逆邻接边
                            r_prep_vertex.first_edge= nullptr;
                            delete r_edge;

                            //删除邻接边
                            previous->next = edge->next;
                            delete edge;

                            //删除后仍要继续前进
                            edge = previous;

                            Edge * added_edge = new Edge(node_id,commonNode.lexeme);

                            Edge * r_added_edge = new Edge(parent_id,commonNode.lexeme);

                            //添加一条边到parent
                            r_prep_vertex.first_edge = added_edge;

                            //添加一条逆边到child
                            Vertex &r_child_vertex = reverse_adjList[child_id];

                            r_added_edge->next = r_child_vertex.first_edge;

                            r_child_vertex.first_edge = r_added_edge;

                        }
                    }
                }
                previous = edge;
                edge = edge->next;
            }
        }
    }
}

void ALGraph::handle_dec() {

    //find dec节点;
    for(int i=0;i<commonnodes.size();i++){
        CommonNode commonNode = commonnodes[i];
        if(commonNode.get_pos()=="DEC"){
            auto r_dec_vertex = reverse_adjList[commonNode.id];

            Edge *edge = r_dec_vertex.first_edge;

            //逆邻接表不为空,有parent节点
            if(edge!= nullptr){
                int parent_id = edge->adjvex;

                auto  &parent_vertex = reverse_adjList[parent_id];

                auto  &r_parent_vertex = adjList[parent_id];


                Edge *pedge = parent_vertex.first_edge;

                if(pedge!= nullptr) {
                    int pparent_id = pedge->adjvex;

                    //删除pparent的邻接边
                    auto &pparent_vertex = adjList[pparent_id];
                    Edge *ppedge = pparent_vertex.first_edge;
                    Edge *pp_previous = nullptr;

                    if (ppedge != nullptr)
                    {
                        while (ppedge != nullptr) {
                        if (ppedge->adjvex == parent_id) {
                            pp_previous->next = ppedge->next;

                            //删除后break
                            break;
                        }
                        pp_previous = ppedge;
                        ppedge = ppedge->next;
                        }

                         //删除parent的逆邻接边  TODO:假设parent只是一个的情况
                        delete pedge;
                        parent_vertex.first_edge = nullptr;

                        //添加节点
                        int added_id = VertexNum;
                        adjList[VertexNum++] = Vertex(added_id);   //添加一个新节点,节点的id为节点数目(顺序添加的最后一个),TODO:id在句子中的位置还没有记录
                        reverse_adjList[added_id] = Vertex(added_id);

                        //这个节点应该是名词,变元节点
                        ArgumentNode added_node = ArgumentNode(added_id,"empty node","empty pos");

                        Position added_pos = Position(1,argumentnodes.size());

                        argumentnodes.push_back(added_node);

                        //添加新节点的两条边

                        Edge * added_edge = new Edge(parent_id,"ATT");
                        Edge * added_r_edge = new Edge(pparent_id,"")
                        adjList[added_id].first_edge=

                    }

                }
            }
        }

    }

}

std::string ALGraph::get_node_pos(int node_id) {
    Position node_position = vertex_index[node_id];

    if(node_position.type==0){
        return templatenodes[node_position.index].pos;
    }
    if(node_position.type==1){
        return argumentnodes[node_position.index].pos;
    }
    if(node_position.type==2){
        return commonnodes[node_position.index].pos;
    }
}


TemplateNode::TemplateNode(const ZparNode &znode) {

    this->id = znode.id;
    this->lexeme = znode.lexeme;
    this->pos = znode.pos;

}

ArgumentNode::ArgumentNode(const ZparNode &znode) {

    this->id = znode.id;
    this->lexeme = znode.lexeme;
    this->pos = znode.pos;

}

CommonNode::CommonNode(const ZparNode &znode) {

    this->id = znode.id;
    this->lexeme = znode.lexeme;
    this->pos = znode.pos;
}

std::string CommonNode::get_pos() {
    return this->pos;
}

ArgumentNode::ArgumentNode(int id, std::string lexeme, std::string pos) {
    this->id = id;
    this->lexeme = lexeme;
    this->pos = pos;
}
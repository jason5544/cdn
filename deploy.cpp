#include "deploy.h"
#include <iostream>
#include <vector>
#include <stack>
#include <utility>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

// graph中的弧
struct Arc
{
    int tail;
    int branwith;
    int unitCost;

    Arc(int tail, int branwith, int unitCost)
    {
        this->tail = tail;
        this->branwith = branwith;
        this->unitCost = unitCost;
    }

    Arc()
    {   }
};

// 消费节点
struct ConsumeNode
{
    int netNode;
    int demand;
};

vector<vector<Arc> > graph;                 // 网络图
vector<ConsumeNode> consumes;               // 消费节点
int nodeNum, linkNum, consumeNodeNum;       // 节点数，边数，消费节点数
int sever_cost;

// 构建图
void constructGraph(char * topo[])
{
    sscanf(topo[0], "%d %d %d", &nodeNum, &linkNum, &consumeNodeNum);
    cout << nodeNum << " " << linkNum << " " << consumeNodeNum << endl;

    sscanf(topo[2], "%d", &sever_cost);
    cout << sever_cost << endl;

    graph.resize(nodeNum);

    int head, tail, brandwith, unitCost;
    for (int i = 4; i < linkNum + 4; i++)
    {
        sscanf(topo[i], "%d %d %d %d", &head, &tail, &brandwith, &unitCost);
        cout << head << " " << tail << " " << brandwith << " " << unitCost << endl;
        graph[head].push_back(Arc(tail, brandwith, unitCost));
        graph[tail].push_back(Arc(head, brandwith, unitCost));
    }

    consumes.resize(consumeNodeNum);
    int num, netNode, demand; 
    for (int i = linkNum + 5; i < linkNum + consumeNodeNum + 5; i++)
    {
        sscanf(topo[i], "%d %d %d", &num, &netNode, &demand);
        cout << num << " " << netNode << " " << demand << endl;
        consumes[num].netNode = netNode;
        consumes[num].demand = demand;
    }

}

//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
    constructGraph(topo);



	// 需要输出的内容
	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);

}

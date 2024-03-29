#include "deploy.h"
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <list>

using namespace std;
int sumFlow;
const int MAXN = 1010;
const int MAXM = 1000200;
const int INF = 1000000000;

struct Edge
{
    int u;
    int v;
    int cap;
    int cost;
    int next;
    int originCap;
    int flow;
    // int consumes;
}edge[MAXM<<2];

int NE;
int head[MAXN], dist[MAXN], pp[MAXN];
bool vis[MAXN];
bool setSever[MAXN];
bool stackVisit[MAXN];
int nodeOutFlow[MAXN];
int consu[MAXN];
int path;
string topo_string;



vector<list<int> > topo_list;

int nodeNum, linkNum, consumeNodeNum;       // 节点数，边数，消费节点数
int sever_cost;
int source;
int target;
int sever_count;

void init()
{
    NE=0;
    memset(head,-1,sizeof(head));
}

void addedge(int u,int v,int cap,int cost)
{
    edge[NE].u=u;
    edge[NE].v=v;
    edge[NE].cap=cap;
    edge[NE].originCap = cap;
    edge[NE].cost=cost;
    edge[NE].next=head[u];
    head[u]=NE++;
    edge[NE].u=v;
    edge[NE].v=u;
    edge[NE].cap=0;
    edge[NE].cost=-cost;
    edge[NE].next=head[v];
    head[v]=NE++;
}

bool SPFA(int s,int t,int n)
{
    int i,u,v;
    queue<int>qu;
    memset(vis,false,sizeof(vis));
    memset(pp,-1,sizeof(pp));
    for(i=0;i<=n;i++)
        dist[i]=INF;
    vis[s]=true;
    dist[s]=0;
    qu.push(s);
    while(!qu.empty())
    {
        u=qu.front();
        qu.pop();
        vis[u]=false;
        for(i=head[u];i!=-1;i=edge[i].next)
        {
           // cout << "i = " << i << endl;
            v=edge[i].v;
            if(edge[i].cap&&dist[v]>dist[u]+edge[i].cost)
            {
                dist[v]=dist[u]+edge[i].cost;
                pp[v]=i;
                if(!vis[v])
                {
                    qu.push(v);
                    vis[v]=true;
                }
            }
        }
    }

    if(dist[t]==INF)
        return false;
    return true;
}

int MCMF(int s,int t,int n)
{

    int flow=0; // 总流量
    int i,minflow,mincost;
    mincost=0;
    while(SPFA(s,t,n))
    {
        minflow=INF+1;
        for(i=pp[t];i!=-1;i=pp[edge[i].u])
        {
            if(edge[i].cap<minflow)
            {
                minflow=edge[i].cap;

            }
            //cout << "edge:" << edge[i].u << " "<<edge[i].v << " edge.cap = " << edge[i].cap << " ";
        }
        //cout << endl;

        flow+=minflow;

        int pre_i;
        for(i=pp[t];i!=-1;i=pp[edge[i].u])
        {
            edge[i].cap-=minflow;
            edge[i^1].cap+=minflow;
            pre_i = i;
        }

        int realDist = dist[t] - edge[pre_i].cost;
        cout << "edge[pre_i] = " << edge[pre_i].u << endl;

        mincost += realDist*minflow;
        // if(!setSever[edge[pre_i].v])
        // {
        //     mincost = mincost  + sever_cost;
        //     setSever[edge[pre_i].v] = true;
        // }

        cout << "dist = " << dist[t] << endl;
        cout << "realDist = " << realDist << endl;
        cout << "minflow = " << minflow << endl;
        cout << "mincost = "  << mincost << endl;
    }

    sumFlow=flow; // 最大流
    return mincost;
}

struct HeapNode
{
    int node;
    int outflow;
    HeapNode(int node, int outflow)
    {
        this->node = node;
        this->outflow = outflow;
    }
    HeapNode()
    {   }

    bool operator<(const HeapNode &node ) const
    {
        return this->outflow < node.outflow;
    }
};


void constructGraph(char * topo[])
{
    init();
    memset(nodeOutFlow, 0, sizeof(nodeOutFlow));
    sscanf(topo[0], "%d %d %d", &nodeNum, &linkNum, &consumeNodeNum);
    //cout << nodeNum << " " << linkNum << " " << consumeNodeNum << endl;

    source =  nodeNum;
    target =  nodeNum + 1;

    sscanf(topo[2], "%d", &sever_cost);
    //cout << sever_cost << endl;

    // graph.resize(nodeNum);

    int head, tail, brandwith, unitCost;
    for (int i = 4; i < linkNum + 4; i++)
    {
        sscanf(topo[i], "%d %d %d %d", &head, &tail, &brandwith, &unitCost);
         //cout << head << " " << tail << " " << brandwith << " " << unitCost << endl;
        // graph[head].push_back(Arc(tail, brandwith, unitCost));
        // graph[tail].push_back(Arc(head, brandwith, unitCost));
        nodeOutFlow[i] += brandwith;

        addedge(head, tail, brandwith, unitCost);
        addedge(tail, head, brandwith, unitCost);
    }

    // consumes.resize(consumeNodeNum);
    int num, netNode, demand;
    for (int i = linkNum + 5; i < linkNum + consumeNodeNum + 5; i++)
    {
        sscanf(topo[i], "%d %d %d", &num, &netNode, &demand);
        // cout << num << " " << netNode << " " << demand << endl;

        addedge(netNode, target, demand, 0);
        consu[netNode] = num;
    }

    priority_queue<HeapNode> myHeap;

    for (int i = 0; i < nodeNum; i++)
    {
        myHeap.push(HeapNode(i, nodeOutFlow[i]));
    }



    for (int i = 0; i < nodeNum; i++)
    {
        // HeapNode node;
        // node = myHeap.top();
        // myHeap.pop();
        addedge(source, i, INF, sever_cost);
        // cout << "outflow = " << node.outflow << endl;
        // addedge(source, node.node, INF, node.outflow);
    }



    // addedge(source, 43, INF, sever_cost);
    // addedge(source, 22, INF, sever_cost);
    // addedge(source, 7, INF, sever_cost);
    // addedge(source, 37, INF, sever_cost);
    // addedge(source, 13, INF, sever_cost);
    // addedge(source, 15, INF, sever_cost);
    // addedge(source, 38, INF, sever_cost);
}


struct STNode
{
    int node;
    int edge;
    STNode(int node, int edge)
    {
        this->node = node;
        this->edge = edge;
    }

    STNode()
    {   }
};


void backTrackPath(int s, int t, int n)
 {
    topo_list.resize(5000);
    memset(setSever, false, sizeof(setSever));
    memset(stackVisit, false, sizeof(stackVisit));
    list<STNode> st;
    int curNode;
    int curMinFlow;
    int curMinFlowSit;

    st.push_back(STNode(s,-1));
    stackVisit[s] = true;

    while (!st.empty())
    {
        curNode = st.back().node;
        // cout << "curNode = " << curNode << endl;
        if (curNode == t)
        {
            cout << "find a path:" << path <<  endl;
            cout << "size = " << st.size() << endl;
            curMinFlow = 0;

            if(setSever[edge[st.back().edge].u] == false)
            {
                cout << "sever sit = "<< edge[st.back().edge].u << endl;
                sever_count++;
                setSever[edge[st.back().edge].u] = true;
                cout << "sever_count = " << sever_count << endl;
            }


            for (list<STNode>::iterator iter = st.begin(); iter != st.end(); iter++)
            {
                cout << iter->node << " ";
                // cout << iter->node << endl;
                if (iter->node == source)
                {
                    ;
                } 
                else if (iter->node == target)
                {
                    topo_list[path].push_back(consu[topo_list[path].back()]);
                }
                else
                {
                    topo_list[path].push_back(iter->node);
                }


                if (iter->edge != -1 && curMinFlow < edge[iter->edge].flow)
                {
                    curMinFlow = edge[iter->edge].flow;
                }
            }
            cout << endl;
            topo_list[path].push_back(curMinFlow);
            path++;

            for (list<STNode>::iterator iter = st.begin(); iter != st.end(); iter++)
            {
                edge[iter->edge].flow -= curMinFlow;
                // if (iter->edge != -1 && edge[iter->edge].flow == 0)
                // {
                //     for (list<STNode>::iterator iter2 = iter; iter2 != st.end(); iter2++)
                //         stackVisit[iter2->node] = false;
                //     st.erase(iter, st.end());
                //     break;
                // }
            }

            st.clear();
            memset(stackVisit, false, sizeof(stackVisit));
            st.push_back(STNode(s, -1));
            stackVisit[s] = true;
            curNode = st.back().node;
            // cout << "curNode = " << curNode << "st.size = " << st.size() << endl;
        }

        bool flag = false;
        for (int i = head[curNode]; i != -1; i = edge[i].next)
        {
            //if(edge[i].flow)
               // cout << edge[i].u << " "<< edge[i].v << " " << edge[i].flow << endl;

            if (edge[i].flow && !stackVisit[edge[i].v])
            {
                //cout << edge[i].u << " "<< edge[i].v << " " << edge[i].flow << endl;
                flag = true;
               // cout << "edge[i].v  = " <<edge[i].v << endl;
                st.push_back(STNode(edge[i].v, i));
                stackVisit[edge[i].v] = true;
                break;
            }
        }

        if (!flag)
        {
            //stackVisit[curNode] = false;
            //cout << "pop = " <<st.back().node << endl;
            st.pop_back();
        }
    }
}



//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
    constructGraph(topo);

    int ans = MCMF(source, target, nodeNum + 2);

    for (int i = 0; i < NE; i += 2)
    {
        edge[i].flow = edge[i].originCap - edge[i].cap;
        if (edge[i].flow)
            cout << edge[i].u << " "<< edge[i].v << " " <<edge[i].flow << endl;
    }
    cout << "sever_cost = " << sever_cost << endl;


    backTrackPath(source, target, nodeNum + 2);
    cout << "all cost = " << ans + sever_cost * sever_count << endl;
	// 需要输出的内容

	char * topo_file = (char *)malloc(5000 * sizeof(char));
    // memset(topo_file, '\0', 5000*sizeof(char));


    cout << path << endl;
    sprintf(topo_file, "%d\n\n", path);

    for (int i = 0; i < path; i++)
    {
        for (list<int>::iterator iter = topo_list[i].begin(); iter != topo_list[i].end(); iter++)
        {
            sprintf(topo_file, "%s%d ", topo_file,*iter);
            cout << *iter << " ";
        }
        sprintf(topo_file, "%s\n", topo_file);
        cout << endl;
    }



	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);

}

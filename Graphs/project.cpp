#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <algorithm>

using namespace std;

using Graph = vector<vector<int>>;
enum Color {
    WHITE,
    GREY,
    BLACK
};

void dfs(int i, const Graph& graph, vector<bool>& visited, vector<int>& order, int n) {

    stack<pair<int, Color>> dfsStack;  // Stack to store vertices and their colors
    vector<int> orderFlag(n, 0);

    if (!visited[i]) {
        dfsStack.push({i, WHITE});  // Inicializa a branco

        while (!dfsStack.empty()) {
            int currentVertex = dfsStack.top().first;
            Color currentColor = dfsStack.top().second;

            visited[currentVertex] = true;  //visita vertice i
            if (currentColor == WHITE) {
                dfsStack.pop();
                dfsStack.push({currentVertex, GREY});  //Coloca i a grey
                for (int neighbor : graph[currentVertex]) {
                    if (!visited[neighbor]) {
                        dfsStack.push({neighbor, WHITE}); //inicializa neighbor a white
                    }
                }
            } 
            else if (currentColor == GREY) {
                // turn vetix to black = (pop())
                if (orderFlag[currentVertex]==0) {
                    order.push_back(currentVertex);
                    orderFlag[currentVertex]=1;
                }
                dfsStack.pop();
            }
        }
    }
}
vector<int> getFinishingTimes(const Graph& graph) {
    int n = graph.size();
    vector<bool> visited(n, false);
    vector<int> order;

    for (int i = 1; i <n; ++i) {
        if (!visited[i]) {
            dfs(i, graph, visited, order, n);
        }
    }
    reverse(order.begin(), order.end());

    return order;
}


int findLongestPath(const Graph& graph, int start, vector<int>& memo, vector<int>& visited,
                     vector<int>& membersSCC, vector<int>& flagSCC, int n){

    stack<pair<int, Color>> dfsStack;  // Stack to store vertices and their colors
    int maxPath = 0;
    membersSCC.push_back(start);
    dfsStack.push({start, WHITE});  // Inicializa a 
    
    while(!dfsStack.empty()){

        int currentVertex = dfsStack.top().first;
        Color currentColor = dfsStack.top().second;

        visited[currentVertex] = 1;  //visita vertice i, global
        flagSCC[currentVertex]= 1;   //visita vertice i, reset apos terminar SCC

        if (currentColor == WHITE){
            dfsStack.pop();
            dfsStack.push({currentVertex, GREY});  //Coloca i a grey
        
            for (int neighbor : graph[currentVertex]) {
                if (visited[neighbor]==1 && flagSCC[neighbor]==0) {
                    maxPath= max(maxPath, memo[neighbor]+1);   //Caso em que acontece Salto

                } else if (visited[neighbor]==0) {
                    membersSCC.push_back(neighbor);            //Caso em que é da mesma SCC
                    dfsStack.push({neighbor, WHITE});       //PUSH neighbor a white
                }
            }
        }
        else if(currentColor == GREY){
            dfsStack.pop();
        }
    }
    int i=0;
    int size=(int)membersSCC.size();
    for(i=0;i<size; i++){
        memo[membersSCC[i]]=maxPath;
    }
    return maxPath;
}

int longestPathInDAG(const Graph& graph, const vector<int>& order) {
    int n = graph.size();
    int maxPath = 0;
    int m= order.size();
    vector<int> memo(n+1, 0);
    vector<int> visited(n,0);  // Track visited vertices within the current traversal

    for (int i = 0; i<m; i++){
        int start = order[i];
        if (visited[start]==0){
            vector<int> membersSCC;
            vector<int> flagSCC(n+1, 0);
            maxPath = max(maxPath, findLongestPath(graph, start, memo, visited, membersSCC, flagSCC, n));
        }
    }

    return maxPath;
}

int main() {
    int people, relations, i=0;
    if (scanf("%d %d", &people, &relations) != 2) {
        return 1;
    }
    Graph graph(people+1);
    Graph graphTranspost(people+1);

    for(i=0; i<relations; i++){
        int person1 ,person2;
        if(scanf("%d %d", &person1,&person2) !=2){
            return 1;
        }
        graph[person1].push_back(person2);
        graphTranspost[person2].push_back(person1);
    }

    vector<int> order = getFinishingTimes(graph);
    int maxPath = longestPathInDAG(graphTranspost, order);

    cout << maxPath << endl;

    return 0;
}

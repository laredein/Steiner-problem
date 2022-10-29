#include <bits/stdc++.h>
using namespace std;
void solveHeuristic(string s);
void solveAprox(string s);
vector<int> path(int u, int v, vector<vector<int>>& next)
{
    vector<int> path;
    path.push_back(u);
    while (u != v) 
	{
        u = next[u][v];
        path.push_back(u);
    }
    return path;
}
 
 
int main()
{
	//cout << "test number from 1 to 39: ";
	//int n; cin >> n;
	//solveAprox("tests/input" + to_string(n) + ".txt");
	int n = 3;
	solveHeuristic("tests/input" + to_string(n) + ".txt");
}


void solveAprox(string s)
{
	fstream test;
	test.open(s, ios::in);
	int n, m; test >> n >> m;
	vector<vector<int>> graph(n, vector<int> (n, 99999));
	vector<int> marked;
	vector<bool> ismarked(n, false);
//extract graph
	for (int i = 0; i < m; i++)
	{
		int x, y, w; test >> x >> y >> w;
		graph[x][y] = w;
		graph[y][x] = w;
	}
	for (int i = 0; i < n; i++)
		graph[i][i] = 0;
	int markednumber; test >> markednumber;
	for (int i = 0; i < markednumber; i++)
	{
		int v; test >> v; v--;
		marked.push_back(v);
		ismarked[v] = true;
	}
	sort(marked.begin(), marked.end());
		
		
//Floyd-Warshall algorithm for shortest distances
	vector<vector<int>> next(n, vector<int>(n, -1));
	vector<vector<int>> dist(n, vector<int>(n));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			dist[i][j] = graph[i][j];
			if (graph[i][j] == 99999)
				next[i][j] = -1;
			else
				next[i][j] = j;		
		}
	}
	
	for (int k = 0; k < n; k++) 
	{
	    for (int i = 0; i < n; i++) 
		{
	        for (int j = 0; j < n; j++) 
			{
	            if (dist[i][j] > (dist[i][k] + dist[k][j]) && (dist[k][j] != 99999 && dist[i][k] != 99999))
	            {
	            	dist[i][j] = dist[i][k] + dist[k][j];
	            	next[i][j] = next[i][k];
				}
	        }
	    }
	}
	
	
//Prim's algorithm, not optimal version as I could have used priority queue rather than just an array, but it won't affect big O complexity because of Floyd-Warshall's O(n ^ 3)
	vector<vector<bool>> steiner(n, vector<bool>(n, false));
		
	vector<bool> intree(markednumber, false);
	//start from marked vertex
	intree[marked[0]] = true;
	int treesize = 1;
	
	while (treesize < markednumber)
	{
		int oldv = -1, newv = -1;
		for (int i = 0; i < n; i++)
		{
			if (intree[i] == false)
				continue;
			for (int j = 0; j < n; j++)
			{
				if (intree[j] == true || i == j || ismarked[j] == false)
					continue;
				if (oldv == -1 && newv == -1)
				{
					oldv = i;
					newv = j;
				}
				else if (dist[oldv][newv] > dist[i][j])
				{
					oldv = i;
					newv = j;
				}
			}
		}
		
		vector<int> pathvect = path(oldv, newv, next);
		intree[pathvect[0]] = true;
		for (int i = 1; i < pathvect.size(); i++)
		{
			steiner[pathvect[i - 1]][pathvect[i]] = true;
			steiner[pathvect[i]][pathvect[i - 1]] = true;
			intree[pathvect[i]] = true;	
		}
		
		treesize++;
	}

	//calculate sum
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (steiner[i][j])
				sum += dist[i][j];
		}
	}

	cout << sum << endl;
	test.close();
	
}



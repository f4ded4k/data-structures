class Directed_Graph {
private:
	stack<int> kosas, tarjans;
	vector<int> topo;
	int time = 1;
	int cidx = 0;
	unordered_map<int, int> con;
	bool iscyclic;

	void cycledfs(int curr, int prev) {
		if (!iscyclic) return;
		visited[curr] = true;
		for (auto& next : adj[curr]) {
			if (next != prev) {
				if (visited[next]) { iscyclic = false; return; }
				cycledfs(next, curr);
			}
		}
		if (!iscyclic) return;
	}

	void topodfs(int curr) {
		visited[curr] = true;
		for (auto& next : adj[curr])
			if (!visited[next])
				topodfs(next);
		topo.push_back(curr);
	}

	void orderingdfs(int node) {
		for (auto& nextnode : adj[node]) {
			if (!visited[nextnode]) {
				visited[nextnode] = true;
				orderingdfs(nextnode);
			}
		}
		kosas.lazy_push(node);
	}

	void findingdfs(int node, bool* vis) {
		for (auto& nextnode : adj[node]) {
			if (!vis[nextnode]) {
				vis[nextnode] = true;
				findingdfs(nextnode, vis);
			}
		}
	}

	void finaldfs(int curr, int* disc, int* low, bool* stacked) {
		disc[curr] = low[curr] = ++time;
		tarjans.lazy_push(curr);
		stacked[curr] = true;

		for (auto& next : adj[curr]) {
			if (disc[next] == 0) {
				finaldfs(next, disc, low, stacked);
				low[curr] = min(low[curr], low[next]);
			}
			else if (stacked[next])
				low[curr] = min(low[curr], disc[next]);
		}

		if (low[curr] == disc[curr]) {
			while (tarjans.top() != curr) {
				stacked[tarjans.top()] = false;
				con[tarjans.top()] = cidx;
				//cout << tarjans.top() << " ";
				tarjans.pop();
			}
			heads.push_back(curr);
			stacked[tarjans.top()] = false;
			con[tarjans.top()] = cidx;
			tarjans.pop();
			cidx++;
			//cout << curr << endl;
		}
	}

public:
	int V, E;
	vector<int> *adj;
	vector<int> heads;
	bool *visited;

	Directed_Graph(int V) {
		this->V = V;
		adj = new vector<int>[V]();
		visited = new bool[V]();
		E = 0;
	}

	void kosa() {
		resetvisited();
		for (int i = 0; i < V; i++)
			if (!visited[i]) {
				visited[i] = true;
				orderingdfs(i);
			}
		Directed_Graph revg = getreverse();
		resetvisited();
		while (!kosas.empty()) {
			int node = kosas.top(); kosas.pop();
			if (!visited[node]) {
				//cout << node << endl;
				visited[node] = true;
				revg.findingdfs(node, visited);
			}
		}
		resetvisited();
	}

	void tarjan() {
		int *disc = new int[V]();
		int *low = new int[V]();
		bool *stacked = new bool[V]();
		for (int i = 0; i < V; i++) {
			if (disc[i] == 0)
				finaldfs(i, disc, low, stacked);
		}
	}

	void resetvisited() {
		memset(visited, false, V);
	}

	Directed_Graph getcondensed() {
		if (!cidx) tarjan();
		Directed_Graph g(cidx);
		for (int curr = 0; curr < V; curr++) {
			for (auto& next : adj[curr]) {
				if (con[curr] != con[next])
					g.addedge(con[curr], con[next]);
			}
		}
		return g;
	}

	Directed_Graph getreverse() {
		Directed_Graph g(V);
		for (int i = 0; i < V; i++) {
			for (auto& node : adj[i]) {
				g.adj[node].push_back(i);
			}
		}
		return g;
	}

	void addedge(int u, int v) {
		adj[u].push_back(v);
		E++;
	}

	vector<int> toposort() {
		topo.clear();
		resetvisited();
		for (int i = 0; i < V; i++)
			if (!visited[i])
				topodfs(i);
		resetvisited();
		reverse(topo.begin(), topo.end());
		return topo;
	}

	bool iscyclic() {
		iscyclic = true;
		resetvisited();
		for (int i = 0; i < V; i++)
			if (!visited[i])
				cycledfs(i, -1);
		resetvisited();
		return iscyclic;
	}
};
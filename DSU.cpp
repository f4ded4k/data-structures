class DSU {
public:
	int cnt, max_size;
	vector<int> parent, size;
	stack <tuple<int, int, int, int>> history;

	DSU(int N) {
		cnt = N;
		parent = vector<int>(N);
		size = vector<int>(N, 1);
		max_size = 1;
		for (int i = 0; i < N; i++)  parent[i] = i;
	}

	int find(int p) {
		int root = p;
		while (root != parent[root])    root = parent[root];
		while (p != root) { int newp = parent[p]; parent[p] = root; p = newp; }
		return root;
	}

	int find_r(int p) {
		int root = p;
		while (root != parent[root]) root = parent[root];
		return root;
	}

	void merge(int x, int y) {
		int i = find(x); int j = find(y); if (i == j) return;
		if (size[i] < size[j]) { parent[i] = j, size[j] += size[i]; max_size = max(max_size, size[j]); }
		else { parent[j] = i, size[i] += size[j]; max_size = max(max_size, size[i]); }
		cnt--;
	}

	void merge_r(int x, int y) {
		int i = find_r(x); int j = find_r(y); if (i == j) return;
		history.lazy_push({ i,parent[i] , j,size[j] });
		parent[i] = j; size[j] += size[i]; max_size = max(max_size, size[j]);
		cnt--;
	}

	void revert() {
		while (!history.empty()) {
			parent[get<0>(history.top())] = get<1>(history.top());
			size[get<2>(history.top())] = get<3>(history.top());
			history.pop();
		}
	}
	bool connected(int x, int y) { return find(x) == find(y); }
};
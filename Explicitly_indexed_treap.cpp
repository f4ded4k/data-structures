template <typename T = int, typename No_overflow = long long>
class Treap {
public:

	class Node {
	public:
		T val;
		int priority;
		Node *l, *r;
		int sz;
		No_overflow sm;

		Node(T val = 0) : val(val), sz(1), l(nullptr), r(nullptr), priority(rand()), sm(val) {}

		void update() {
			sz = 1, sm = val;
			if (l) sz += l->sz, sm += l->sm;
			if (r) sz += r->sz, sm += r->sm;
		}

		~Node() { delete l, delete r; }
	};

	Node* root;

	Treap() {
		root = nullptr;
		srand(123454321);
	}

	Treap(vector<T> &v) {
		root = nullptr;
		srand(123454321);
		for (auto& x : v) insert(x);
	}

	void split(Node *t, T val, Node *&l, Node *&r) {
		if (!t) { l = r = nullptr; return; }
		if (t->val < val) {
			split(t->r, val, l, r);
			t->r = l;
			l = t;
		}
		else {
			split(t->l, val, l, r);
			t->l = r;
			r = t;
		}
		t->update();
	}

	Node* merge(Node *l, Node *r) {
		if (!l || !r) return l ? l : r;
		if (l->priority < r->priority) {
			l->r = merge(l->r, r);
			l->update();
			return l;
		}
		r->l = merge(l, r->l);
		r->update();
		return r;
	}

	bool insert(T val) {
		if (find(root, val)) return false;
		Node *l, *r;
		split(root, val, l, r);
		root = merge(merge(l, new Node(val)), r);
		return true;
	}

	int count(T val) { return find(root, val); }

	bool find(T val) { return find(root, val); }
	bool find(Node *t, T val) {
		if (!t) return false;
		if (t->val == val) return true;
		return find(t->val < val ? t->r : t->l, val);
	}

	void erase(T val) {
		Node *l, *r, *t;
		split(root, val, l, t);
		split(t, val + 1, t, r);
		delete t;
		root = merge(l, r);
	}

	int valueof(int idx) { return valueof(root, idx); }
	int valueof(Node *t, int idx) {
		int lsz = getcount(t->l);
		if (idx == lsz) return t->val;
		if (idx < lsz) return valueof(t->l, idx);
		return valueof(t->r, idx - lsz - 1);
	}

	int indexof(T val) { return indexof(root, val); }
	int indexof(Node *t, T val) {
		int lsz = getcount(t->l);
		if (t->val == val) return lsz;
		if (t->val < val)return lsz + 1 + indexof(t->r, val);
		return indexof(t->l, val);
	}

	No_overflow nth_sum(int n) {
		T val = nth_element(root, n);
		Node *l, *r;
		split(root, val + 1, l, r);
		No_overflow result = l ? l->sm : 0;
		root = merge(l, r);
		return result;
	}

	T nth_element(int n) { return nth_element(root, n); }
	T nth_element(Node *t, int n) {
		if (n == getcount(t->l)) return t->val;
		if (n < getcount(t->l)) return nth_element(t->l, n);
		return nth_element(t->r, n - getcount(t->l) - 1);
	}

	T getmin() { return root ? getmin(root) : -1; }
	T getmin(Node *t) {
		if (!t->l) return t->val;
		return getmin(t->l);
	}

	T getmax() { return root ? getmax(root) : -1; }
	T getmax(Node *t) {
		if (!t->r) return t->val;
		return getmax(t->r);
	}

	void print() { print(root); cout << endl; }
	void print(Node* t) {
		if (!t) return;
		print(t->l);
		cout << t->val << " ";
		print(t->r);
	}

	int getcount(Node *t) {
		return t ? t->sz : 0;
	}

	int size() { return root ? root->sz : 0; }
};
template<typename T = int, typename No_overflow = long long>
class Treap {
	public:
		class Node {
		public:
			T val;
			int priority;
			Node *l, *r;
			bool rev;
			int sz;
			T mini, maxi, delta;
			No_overflow sm;

			Node(T val = 0) : val(val), sz(1), l(nullptr), r(nullptr),
				priority(rand()), sm(val), rev(false), mini(val), maxi(val), delta(0) {}

			void update() {
				sz = 1, sm = val, mini = val, maxi = val;
				if (l) sz += l->sz, sm += l->sm, mini = min(mini, l->mini), maxi = max(maxi, l->maxi);
				if (r) sz += r->sz, sm += r->sm, mini = min(mini, r->mini), maxi = max(maxi, r->maxi);
			}

			void push() {
				if (rev) {
					rev = false;
					swap(l, r);
					if (l) l->rev ^= true;
					if (r) r->rev ^= true;
				}
				if (delta) {
					val += delta;
					if (l) l->delta += delta;
					if (r) r->delta += delta;
					delta = 0;
				}
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
			for (int i = 0; i < v.size(); i++) insert(v[i], i);
		}

		void split(Node *t, int ind, Node *&l, Node *&r, int add = 0) {
			if (!t) { l = r = nullptr; return; }
			t->push();
			if (add + getcount(t->l) < ind) {
				split(t->r, ind, l, r, add + 1 + getcount(t->l));
				t->r = l;
				l = t;
			}
			else {
				split(t->l, ind, l, r, add);
				t->l = r;
				r = t;
			}
			t->update();
		}

		Node* merge(Node *l, Node *r) {
			if (l) l->push();
			if (r) r->push();
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

		void reverse(int l, int r) { reverse(root, l, r); }
		void reverse(Node *t, int l, int r) {
			Node *t1, *t2, *t3;
			split(t, l, t1, t2);
			split(t2, r - l + 1, t2, t3);
			t2->rev ^= true;
			root = merge(merge(t1, t2), t3);
		}

		void addition(T delta, int ind) { addition(root, delta, ind, ind); }
		void addition(T delta, int l, int r) { adition(root, delta, l, r); }
		void addition(Node *t, T delta, int l, int r) {
			Node *t1, *t2, *t3;
			split(t, l, t1, t2);
			split(t2, r - l + 1, t2, t3);
			t2->delta += delta;
			root = merge(merge(t1, t2), t3);
		}

		No_overflow getsum(int l, int r) { return getsum(root, l, r); }
		No_overflow getsum(Node *t, int l, int r) {
			Node *t1, *t2, *t3;
			split(t, l, t1, t2);
			split(t2, r - l + 1, t2, t3);
			No_overflow result = t2->sm;
			root = merge(merge(t1, t2), t3);
			return result;
		}

		T getmin(int l, int r) { return getmin(root, l, r); }
		T getmin(Node *t, int l, int r) {
			Node *t1, *t2, *t3;
			split(t, l, t1, t2);
			split(t2, r - l + 1, t2, t3);
			T result = t2->mini;
			root = merge(merge(t1, t2), t3);
			return result;
		}

		T getmax(int l, int r) { return getmax(root, l, r); }
		T getmax(Node *t, int l, int r) {
			Node *t1, *t2, *t3;
			split(t, l, t1, t2);
			split(t2, r - l + 1, t2, t3);
			T result = t2->maxi;
			root = merge(merge(t1, t2), t3);
			return result;
		}

		void insert(T val, int ind) {
			Node *l, *r;
			split(root, ind, l, r);
			root = merge(merge(l, new Node(val)), r);
		}

		void erase(int ind) {
			Node *l, *r, *t;
			split(root, ind, l, t);
			split(t, ind + 1, t, r);
			delete t;
			root = merge(l, r);
		}

		T valueof(int idx) { return valueof(root, idx); }
		T valueof(Node *t, int idx) {
			int lsz = getcount(t->l);
			if (idx == lsz) return t->val;
			if (idx < lsz) return valueof(t->l, idx);
			return valueof(t->r, idx - lsz - 1);
		}

		void print() { print(root); cout << endl; }
		void print(Node* t) {
			if (!t) return;
			t->push();
			print(t->l);
			cout << t->val << " ";
			print(t->r);
		}

		T getmin() {
			return root ? root->mini : -1;
		}

		T getmax() {
			return root ? root->maxi : -1;
		}

		int getcount(Node *t) {
			return t ? t->sz : 0;
		}

		int size() { return root ? root->sz : 0; }
	};
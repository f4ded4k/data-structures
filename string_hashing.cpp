template <typename T = unsigned long long>
class String_Hasher {
public:
	string & s;
	T m, p;
	vector<T> preh, expop, modinvexpop;

	String_Hasher(string &s, T p = 31, T m = 1000000123) : s(s), m(m), p(p) {
		preh.resize(s.size() + 1);
		expop.resize(s.size() + 1);
		modinvexpop.resize(s.size() + 1);
		preprocess();
		fillprefixes();
	}

	void fillprefixes() {
		preh[0] = 0;
		preh[1] = (expop[0] * (s[0] - 'a' + 1)) % m;
		for (int i = 2; i <= s.size(); i++)
			preh[i] = (preh[i - 1] + (expop[i - 1] * (s[i - 1] - 'a' + 1))) % m;
	}

	T operator()(int i, int j) {
		return (((preh[j + 1] - preh[i] + m) % m) * modinvexpop[i + 1]) % m;
	}

	void preprocess() {
		expop[0] = 1;
		modinvexpop[0] = 1;
		expop[1] = p % m;
		modinvexpop[1] = binaryexpo(p, m - 2, m);
		for (int i = 2; i <= s.size(); i++)
			expop[i] = (expop[i - 1] * p) % m,
			modinvexpop[i] = (modinvexpop[i - 1] * modinvexpop[1]) % m;
	}

	T binaryexpo(T base, T exp, const T mod) {
		T ans = 1;
		base = base % mod;
		while (exp > 0) {
			if (exp & 1) {
				ans = (ans * base) % mod;
			}
			exp >>= 1;
			base = (base * base) % mod;
		}
		return ans % mod;
	}
};

template <typename T = unsigned long long>
class Extreme_Hasher {
public:
	string & s;
	String_Hasher<T> *h1, *h2;

	Extreme_Hasher(string &st, T p1 = 31, T p2 = 53, T m1 = 1000000009, T m2 = 1000000123) : s(st) {
		h1 = new String_Hasher<T>(s, p1, m1);
		h2 = new String_Hasher<T>(s, p2, m2);
	}

	T operator()(int i, int j) {
		return ((*h1)(i, j) << 32) + ((*h2)(i, j));
	}
};
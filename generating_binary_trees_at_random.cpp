// http://www.cs.otago.ac.nz/staffpriv/mike/Papers/RandomGeneration/RandomBinaryTrees.pdf
class Node {
 public:
  unique_ptr<Node> left, right;
};
class RandomBinaryTree {
 public:
  int n;
  mt19937 rng;

  RandomBinaryTree(int _n) : n(_n) {
    rng.seed(chrono::steady_clock::now().time_since_epoch().count());
  }

  unique_ptr<Node> Sample(int n) {
    vector<int> L(2 * n);
    iota(L.begin(), L.end(), 0);
    shuffle(L.begin(), L.end(), rng);

    unordered_set<int> hash(L.begin(), L.begin() + n);

    string x(2 * n, ')');
    for (int i = 0; i < 2 * n; ++i)
      if (hash.count(i)) x[i] = '(';

    x = ToWellFormedWord(x, 0, 2 * n - 1);
    CreateBinaryTree(x);
  }

 private:
  string ToWellFormedWord(string& s, int l, int r) {
    if (l > r) return "";
    int partial_sum = 0;
    bool is_well_formed = true;
    for (int i = l; i <= r; ++i) {
      partial_sum += (s[i] == '(' ? 1 : -1);
      is_well_formed = is_well_formed && (partial_sum >= 0);
      if (!partial_sum) {
        if (is_well_formed) return s.substr(l, i - l + 1) + f(s, i + 1, r);
        string inv_s(i - l - 1, '(');
        for (int j = l + 1; j <= i - 1; ++j)
          if (s[j] == '(') inv_s[j - l - 1] = ')';
        return "(" + f(s, i + 1, r) + ")" + inv_s;
      }
    }
    assert(false);
  }

  unique_ptr<Node> CreateNode(vector<int>& rb, int l, int r) {
    if (l > r) return nullptr;
    unique_ptr<Node> root = make_unique<Node>();
    root->left = CreateNode(rb, l + 1, rb[l] - 1);
    root->right = CreateNode(rb, rb[l] + 1, r);
    return root;
  }

  unique_ptr<Node> CreateBinaryTree(string& s) {
    vector<int> rb(s.size(), -1);
    stack<int> st;
    for (size_t i = 0; i < s.size(); ++i) {
      if (s[i] == '(') {
        st.push(i);
      } else {
        rb[st.top()] = i;
        st.pop();
      }
    }
    unique_ptr<Node> root = CreateNode(rb, 0, rb.size() - 1);
    return root;
  }
};

#include <bits/stdc++.h>
using namespace std;

class Client {
private:
  unordered_set<int> likes;
  unordered_set<int> dislikes;
  int id;

public:
  Client(int id) { this->id = id; }

  void set_likes(const unordered_set<int> &likes) {
    this->likes = unordered_set<int>(likes);
  }

  void set_dislikes(const unordered_set<int> &dislikes) {
    this->dislikes = unordered_set<int>(dislikes);
  }

  unordered_set<int> &get_likes() { return this->likes; }

  unordered_set<int> &get_dislikes() { return this->dislikes; }

  int get_id() { return this->id; }

  bool is_compatible(Client &other) {
    unordered_set<int> d = other.get_dislikes(), l = other.get_likes();
    for (int ing : this->likes) {
        if (d.find(ing) != d.end()) return false;
    }
    for (int ing : this->dislikes) {
        if (l.find(ing) != l.end()) return false;
    }
    return true;
  }
};

vector<Client> clients;
unordered_map<string, int> ing2id;
vector<string> id2ing = vector<string>();
int ing_idx = 0;

unordered_set<int> read_input(int field_count) {
  string name;
  unordered_set<int> elems = unordered_set<int>(field_count);
  unordered_map<string, int>::iterator it;
  for (int idx = 0; idx < field_count; ++idx) {
    cin >> name;
    it = ing2id.find(name);
    if (it == ing2id.end()) {
      ing2id.emplace(name, ing_idx);
      id2ing.push_back(name);
      elems.insert(ing_idx++);
    } else
      elems.insert(it->second);
  }

  return elems;
}

vector<vector<Client*>> build_graph() {
    vector<vector<Client*>> g = vector<vector<Client*>>(clients.size());
    for (int i = 0; i < (int) clients.size(); ++i) {
        for (int j = i + 1; j < (int) clients.size(); ++j) {
            if (!clients[i].is_compatible(clients[j])) {
                g[i].push_back(&clients[j]);
                g[j].push_back(&clients[i]);
            }
        }
    }
    return g;
}

vector<Client*> get_independent_set(const vector<vector<Client*>> &g) {
    vector<Client*> ans = vector<Client*>();
    vector<int> nodes = vector<int>(clients.size());
    iota(nodes.begin(), nodes.end(), 0);
    vector<int>::iterator random_node;

    while (!nodes.empty()) {
        random_node = nodes.begin();
        advance(random_node, rand() % nodes.size());
        ans.push_back(&clients[*random_node]);
        for (Client *c : g[*random_node]) {
            nodes.erase(nodes.begin() + c->get_id());
        }
        nodes.erase(random_node);
    }

    return ans;
}

unordered_set<int> solve() {
  unordered_set<int> pizza;
  vector<vector<Client*>> graph = build_graph();

  vector<Client*> independent_set = get_independent_set(graph);
  for (Client *c : independent_set) {
      for (int ing : c->get_likes()) pizza.insert(ing);
  }

  return pizza;
}

int compute_score(const unordered_set<int> &pizza) {
  int score = 0;
  for (Client c : clients) {
    bool comes = true;
    for (int ing : c.get_likes()) {
      if (pizza.find(ing) == pizza.end()) {
        comes = false;
        break;
      }
    }
    if (comes) {
      for (int ing : c.get_dislikes()) {
        if (pizza.find(ing) != pizza.end()) {
          comes = false;
          break;
        }
      }
    }
    if (comes)
      score++;
  }

  return score;
}

void output_solution(const unordered_set<int> &solution) {
  cout << solution.size();
  for (int ing : solution) {
    cout << " " << id2ing[ing];
  }
  cout << endl;
}

int main(int argc, char **argv) {
#ifdef _LOCAL
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <FILENAME> <NUMERIC SEED>" << endl;
    return 1;
  }
  freopen(argv[1], "r", stdin);
  srand(stoi(argv[2]));
#endif

  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);
  cerr.tie(0);

  int client_amount, elem_amount;

  cin >> client_amount;
  clients = vector<Client>(client_amount, -1);

  for (int client_idx = 0; client_idx < client_amount; ++client_idx) {
    clients[client_idx] = Client(client_idx);

    cin >> elem_amount;
    clients[client_idx].set_likes(read_input(elem_amount));

    cin >> elem_amount;
    clients[client_idx].set_dislikes(read_input(elem_amount));
  }

  unordered_set<int> solution = solve();
  output_solution(solution);
  cerr << "Score: " << compute_score(solution) << endl;

  return 0;
}

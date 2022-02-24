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
vector<vector<Client*>> graph;
unordered_map<string, int> ing2id;
vector<string> id2ing = vector<string>();
mt19937 rng;
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

unordered_set<int> get_random_independent_set() {
    unordered_set<int> ans = unordered_set<int>();
    unordered_set<int> nodes = unordered_set<int>(clients.size());
    for (int i = 0; i < (int)clients.size(); ++i) nodes.insert(i);

    while (!nodes.empty()) {
        auto it = next(nodes.begin(), rng() % nodes.size());
        ans.insert(clients[*it].get_id());
        for (Client *c : graph[*it]) {
            nodes.erase(c->get_id());
        }
        nodes.erase(*it);
    }

    return ans;
}

void update_randomly_independent_set(unordered_set<int> &ans) {
    unordered_set<int> nodes = unordered_set<int>(clients.size());
    for (int i = 0; i < (int)clients.size(); ++i) nodes.insert(i);
    for (int i : ans) {
        nodes.erase(i);
        for (Client *c : graph[i]) nodes.erase(c->get_id());
    }

    while (!nodes.empty()) {
        auto it = next(nodes.begin(), rng() % nodes.size());
        ans.insert(clients[*it].get_id());
        for (Client *c : graph[*it]) {
            nodes.erase(c->get_id());
        }
        nodes.erase(*it);
    }
}

unordered_set<int> make_pizza(const unordered_set<int> &independent_set) {
  unordered_set<int> pizza;
  for (int id : independent_set) {
      for (int ing : clients[id].get_likes()) pizza.insert(ing);
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

unordered_set<int> refine_solution(unordered_set<int> &solution, int iters) {
    unordered_set<int> best_solution = unordered_set<int>(solution);
    unordered_set<int> c_solution, eliminated;
    uniform_int_distribution<> d(25,50);
    int c_score, best_score = compute_score(make_pizza(best_solution));
    cerr << "First score: " << best_score << endl;
    long long p;

    for (int i = 0; i < iters; ++i) {
        // Delete d(rng)% of nodes from best solution
        p = llround(d(rng) * best_solution.size() / 100);
        sample(best_solution.begin(), best_solution.end(), inserter(eliminated, eliminated.begin()), p, rng);
        c_solution = unordered_set<int>(best_solution);
        for (int id : eliminated) c_solution.erase(id);
        update_randomly_independent_set(c_solution);
        c_score = compute_score(make_pizza(c_solution));
        if (c_score > best_score) {
            cerr << "[Increase: " << c_score - best_score << "]" << endl;
            best_score = c_score;
            best_solution = c_solution;
        }
        eliminated.clear();
    }

    return best_solution;
}

void output_solution(const unordered_set<int> &solution) {
  cout << solution.size();
  for (int ing : solution) {
    cout << " " << id2ing[ing];
  }
  cout << endl;
}

int main(int argc, char **argv) {
  int iters = 1000;

#ifdef _LOCAL
  if (argc != 4) {
    cerr << "Usage: " << argv[0] << " <FILENAME> <ITERS> <NUMERIC SEED>" << endl;
    return 1;
  }
  freopen(argv[1], "r", stdin);
  rng = mt19937{random_device{}()};
  rng.seed(stoi(argv[3]));
  iters = stoi(argv[2]);
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

  graph = build_graph();
  unordered_set<int> solution = get_random_independent_set();

  solution = refine_solution(solution, iters);

  unordered_set<int> pizza = make_pizza(solution);
  output_solution(pizza);
  cerr << "Score: " << compute_score(pizza) << endl;

  return 0;
}

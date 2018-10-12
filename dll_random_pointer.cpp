#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

struct ListNode {
  ListNode* prev;
  ListNode* next;
  ListNode* rand;
  std::string data;
};

class ListRand {
 public:
  void Serialize(std::fstream& f);
  void Deserialize(std::fstream& f);

 public:
  ListNode* head_;
  ListNode* tail_;
  int count_;
};

void ListRand::Serialize(std::fstream& f) {
  f.open("list.txt", std::fstream::out);

  std::map<ListNode*, int> map;
  auto node = head_;
  for (int i = 0; i < count_; i++) {
    map[node] = i;
    node = node->next;
  }

  node = head_;
  while (node) {
    // Write random node's index
    // 0 stands for nullptr random node
    if (node->rand) {
      f << map.find(node->rand)->second + 1;
    } else {
      f << 0;
    }
    f << ";";
    f << node->data;
    if (node->next) {
      f << std::endl;
    }
    node = node->next;
  }

  f.close();
}

void ListRand::Deserialize(std::fstream& f) {
  f.open("list.txt", std::fstream::in);

  // Create nodes
  int count = 0;
  std::string line;
  std::map<int, ListNode*> map;
  std::vector<int> rand_keys;

  ListNode* temp = nullptr;

  while(std::getline(f, line)) {
    int rand_key = std::stoi(line.substr(0, line.find_first_of(';')));
    rand_keys.push_back(rand_key);
    std::string data = line.substr(line.find_first_of(';') + 1);

    auto node = new ListNode;
    node->prev = temp;
    if (temp) temp->next = node;
    node->data = data;
    temp = node;

    map[count] = node;

    count++;
  }

  count_ = count;

  head_ = map[0];
  tail_ = map[count_ - 1];
  tail_->next = nullptr;

  // Assign rands
  for (int i = 0; i < count_; i++) {
    map[i]->rand = rand_keys[i] == 0 ? nullptr : map[rand_keys[i] - 1];
  }

  f.close();
}

void print_list(const ListRand& list) {
  auto node = list.head_;
  while (node != nullptr) {
    std::cout << "data=" << node->data;
    if (node->rand) std::cout << " : random node data=" << node->rand->data;
    std::cout << std::endl;
    node = node->next;
  }
}

void release_list(const ListRand& list) {
  auto node = list.head_;
  while (node != nullptr) {
    auto next = node->next;
    delete node;
    node = next;
  }
}

int main() {
  // Some random nodes.
  auto node_1 = new ListNode;
  auto node_2 = new ListNode;
  auto node_3 = new ListNode;
  auto node_4 = new ListNode;
  auto node_5 = new ListNode;

  node_1->prev = nullptr;
  node_1->next = node_2;
  node_1->rand = nullptr;
  node_1->data = "abc";

  node_2->prev = node_1;
  node_2->next = node_3;
  node_2->rand = node_3;
  node_2->data = "def";

  node_3->prev = node_2;
  node_3->next = node_4;
  node_3->rand = node_1;
  node_3->data = "ghi";

  node_4->prev = node_3;
  node_4->next = node_5;
  node_4->rand = node_1;
  node_4->data = "jkl";

  node_5->prev = node_4;
  node_5->next = nullptr;
  node_5->rand = node_3;
  node_5->data = "mno";

  std::fstream f;

  ListRand list_out;
  list_out.count_ = 5;
  list_out.head_ = node_1;
  list_out.tail_ = node_5;
  list_out.Serialize(f);

  ListRand list_in;
  list_in.Deserialize(f);
  print_list(list_in);

  release_list(list_out);
  release_list(list_in);

  while (true) {
  }
}
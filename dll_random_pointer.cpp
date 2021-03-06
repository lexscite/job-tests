﻿// Doubly linked list with random pointer serialization and
// deserialization. Algorithm complexity < O(N^2).

#include <fstream>
#include <iostream>
#include <map>
#include <string>

struct ListNode {
 public:
  // Constructor can be ommited but you'll need to assign
  // every default value by hand in order for them not to
  // become filled with random values from memory.
  ListNode() : data(), prev(), next(), rand() {}

 public:
  std::string data;
  ListNode* prev;
  ListNode* next;
  ListNode* rand;
};

class ListRand {
 public:
  // Constructor can be ommited but you'll need to assign
  // every default value by hand in order for them not to
  // become filled with random values from memory.
  ListRand() : count_(), head_(), tail_() {}

  void Serialize(std::fstream& fs) const;
  void Deserialize(std::fstream& fs);

 public:
  int count_;
  ListNode* head_;
  ListNode* tail_;
};

void ListRand::Serialize(std::fstream& fs) const {
  using namespace std;
  if (count_ != 0) {
    map<ListNode*, int> nodes;
    ListNode* current;

    current = head_;
    for (int i = 0; i < count_; i++) {
      nodes[current] = i;
      current = current->next;
    }

    current = head_;
    for (int i = 0; i < count_; i++) {
      int r = current->rand ? nodes[current->rand] : -1;
      fs << r << "," << current->data << endl;
      current = current->next;
    }
  }
}

void ListRand::Deserialize(std::fstream& fs) {
  using namespace std;
  map<int, ListNode*> nodes;
  ListNode* prev = nullptr;
  string line;
  int i = 0;
  bool failed = false;

  while (getline(fs, line)) {
    int r = 0;
    // Can throw invalid_arguments and out_of_range.
    // Abort parsing if that happens.
    try {
      r = stoi(line.substr(0, line.find_first_of(',')));
    } catch (exception e) {
      cerr << "Error: " << e.what() << ". Possibly wrong file format" << endl;
      failed = true;
      break;
    }

    string data = line.substr(line.find_first_of(',') + 1);

    if (!nodes[i]) nodes[i] = new ListNode;
    if (!nodes[r] && r != -1) nodes[r] = new ListNode;
    if (prev) prev->next = nodes[i];

    nodes[i]->data = data;
    nodes[i]->prev = prev;
    nodes[i]->rand = nodes[r];

    prev = nodes[i];
    i++;
  }

  if (!failed) {
    if (i != 0) {
      count_ = i;
      head_ = nodes[0];
      tail_ = prev;
    } else {
      // Release already allocated memory when bad things happened
      for (auto node : nodes) {
        delete node.second;
      }
    }
  }
}

std::ostream& operator<<(std::ostream& os, const ListNode* node) {
  using namespace std;
  os << "    Data: " << node->data << endl;
  if (node->rand) os << "    Rand: " << node->rand->data << endl;
  return os;
}

std::ostream& operator<<(std::ostream& os, const ListRand& list) {
  using namespace std;
  ListNode* current = list.head_;
  os << "List (" << (list.count_) << "):" << endl;
  for (int i = 0; i < list.count_; i++) {
    os << "  Node " << i + 1 << ":" << endl
       << current;
    current = current->next;
  }
  os << endl;
  return os;
}

std::fstream& operator<<(std::fstream& fs, const ListRand& list) {
  list.Serialize(fs);
  return fs;
}

std::fstream& operator>>(std::fstream& fs, ListRand& list) {
  list.Deserialize(fs);
  return fs;
}

ListRand CreateList() {
  // Some random nodes
  ListNode* node_1 = new ListNode;
  ListNode* node_2 = new ListNode;
  ListNode* node_3 = new ListNode;
  ListNode* node_4 = new ListNode;

  node_1->data = "Apple";
  node_1->prev = nullptr;
  node_1->next = node_2;
  node_1->rand = node_3;

  node_2->data = "Balloon";
  node_2->prev = node_1;
  node_2->next = node_3;
  node_2->rand = nullptr;

  node_3->data = "Crayon";
  node_3->prev = node_2;
  node_3->next = node_4;
  node_3->rand = node_1;

  node_4->data = "Drum";
  node_4->prev = node_3;
  node_4->next = nullptr;
  node_4->rand = node_1;

  ListRand list;
  list.head_ = node_1;
  list.tail_ = node_4;
  list.count_ = 4;

  return list;
}

void ReleaseList(ListRand& list) {
  if (list.count_ == 0)
    return;

  ListNode* current = list.head_;
  ListNode* next = nullptr;

  while (current) {
    next = current->next;
    delete current;
    current = next;
  }

  delete next;
}

int main() {
  using namespace std;
  string filename = "list.txt";
  fstream fs;

  ListRand list_1 = CreateList();
  fs.open(filename, fstream::out);
  if (fs.fail()) {
    cerr << "Error: failed to open file \"" << filename << "\"" << endl;
  } else {
    // Serialize list
    fs << list_1;
    cout << list_1;
    fs.close();
  }

  ListRand list_2;
  fs.open(filename, fstream::in);
  if (fs.fail()) {
    cerr << "Error: failed to open file \"" << filename << "\"" << endl;
  } else {
    // Deserialize list
    fs >> list_2;
    cout << list_2;
    fs.close();
  }

  ReleaseList(list_1);
  ReleaseList(list_2);

  system("pause");

  return 0;
}
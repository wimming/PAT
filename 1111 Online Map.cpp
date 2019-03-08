#include <iostream>
#include <vector>
#include <map>
#include <stack>

using namespace std;

class Node {
public:
    int v, length, time;
    Node * next;
    Node(int _v, int _length, int _time) : v(_v), length(_length), time(_time) { next = NULL; }
};

int n, m, source, destination;
vector<Node*> adjacency_list;

int find_path(int source, int destination, int flag, stack<int> & path) {

    // my understand for dijkstra : 1. select min node; 2. involve this node; 3. refresh all node;
    map<int, pair<int, int> > involved, uninvolved;  // record current min dist from source for each node : node index => current min dist
    map<int, int> previous;  // record previous node for current nnode : current node => previous node
    for (int i = 0; i < n; ++i) {
        uninvolved.insert(pair<int, pair<int, int> >(i, pair<int, int>(-1, -1)));
        previous.insert(pair<int, int>(i, -1));
    }
    uninvolved[source] = pair<int, int>(0, 0);

    for (int i = 0; i < n; ++i) {  // or while !uninvolved.empty()
        // step1
        map<int, pair<int, int> >::iterator min_itr = uninvolved.begin();
        int min = min_itr->second.first;
        int min_v;
        for (map<int, pair<int, int> >::iterator itr = uninvolved.begin(); itr != uninvolved.end(); ++itr) {
            if (itr->second.first != -1) {
                if (min == -1 || itr->second.first < min
                    /********************************************
                    most important, a second compare when the first is equal. */
                    || itr->second.first == min && itr->second.second < min_itr->second.second
                    /*******************************************/
                ) {
                    min = itr->second.first;
                    min_itr = itr;
                }
            }
        }

        // step2
        min_v = min_itr->first;
        involved.insert(pair<int, pair<int, int> >(min_itr->first, pair<int, int>(min_itr->second.first, min_itr->second.second)));
        uninvolved.erase(min_itr->first);

        // step3
        Node * p = adjacency_list[min_v];
        while (p != NULL) {
        	uninvolved.find(p->v);
            if (uninvolved.find(p->v) != uninvolved.end()) {
                int dist = (flag == 0 ? p->length : p->time);
                int dist2 = (flag == 0 ? p->time : 1);

                if (uninvolved[p->v].first == -1 || involved[min_v].first+dist < uninvolved[p->v].first
                    /********************************************
                    most important, a second compare when the first is equal. */
                    || involved[min_v].first+dist == uninvolved[p->v].first && involved[min_v].second+dist2 < uninvolved[p->v].second
                    /*******************************************/
                ) {
                    uninvolved[p->v].first = involved[min_v].first+dist;
                    uninvolved[p->v].second = involved[min_v].second+dist2;

                    previous[p->v] = min_v;
                }
            }

            p = p->next;  // ++p cause fatal error
        }
    }

    int pre = destination;
    while (pre != source) {
        path.push(pre);
        pre = previous[pre];
    }
    path.push(pre);

    return involved[destination].first;
}

void print_path(stack<int> & path) {
    cout << path.top();
    path.pop();
    while (!path.empty()) {
        cout << " -> " << path.top();
        path.pop();
    }
    cout << endl;  // Distance = D: source -> v1 -> ... -> destination
}

bool is_equal(stack<int> s1, stack<int> s2) {
	while (!s1.empty() && !s2.empty()) {
		if (s1.top() != s2.top()) return false;
		s1.pop();
		s2.pop();
	}
	return s1.empty() && s2.empty();
}

int main() {
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        adjacency_list.push_back(NULL);
    }
    for (int i = 0; i < m; ++i) {
        int v1, v2, one_way, length, time;
        cin >> v1 >> v2 >> one_way >> length >> time;
        if (one_way == 1) {
            Node * node = new Node(v2, length, time);
            node->next = adjacency_list[v1];
            adjacency_list[v1] = node;
        }
        else {
            Node * node1 = new Node(v2, length, time);
            Node * node2 = new Node(v1, length, time);
            node1->next = adjacency_list[v1];
            node2->next = adjacency_list[v2];
            adjacency_list[v1] = node1;
            adjacency_list[v2] = node2;
        }
    }
    cin >> source >> destination;

    stack<int> length_path, time_path;
    int d, t;

	d = find_path(source, destination, 0, length_path);
	t = find_path(source, destination, 1, time_path);

	if (!is_equal(length_path, time_path)) {
	    cout << "Distance = " << d << ": ";
	    print_path(length_path);
	    cout << "Time = " << t << ": ";
	    print_path(time_path);
	}
	else {
	    cout << "Distance = " << d << "; ";
	    cout << "Time = " << t << ": ";
	    print_path(length_path);
	}

}

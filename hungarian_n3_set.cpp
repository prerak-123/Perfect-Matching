#include<bits/stdc++.h>
using namespace std;

// Using stl set

const int INFTY = 10000000;
// maximum weight saturating matching

class Compare{
    public:
        bool operator()(tuple<int, int>* a, tuple<int, int>* b ){
            return (get<0>(*a) < get<0>(*b));
        }
};
bool DijkstrAugment(const vector<vector<tuple<int,int>>>& M,
                    vector<int>& matching_b, vector<int>& matching_a,
                    vector<int> &A, vector<int> &B,
                    int A_index, int m, int n){

    vector<int> distance(m+n, INFTY);
    vector<int> predecessor(m+n, -1);
    vector<int> marked_indices;

    //initialize source
    distance[A_index] = 0;
    bool flag = false;
    int match = -1;
    int match_dist = -1;


    vector<tuple<int, int>> pointers(m+n);
    for(int i = 0; i < m+n; i++){
        pointers[i] = {distance[i], i};
    }
    set<tuple<int, int>> queue(pointers.begin(), pointers.end());

    while(queue.size() != 0){
        auto it = queue.begin();
        int extract = get<1>(*it);
        int dist_extract = get<0>(*it);
        queue.erase(queue.begin());
        // cout << "Extracted: " << extract  << " " << dist_extract << endl;
        
        if(dist_extract == INFTY){
            break;
        }
        
        marked_indices.push_back(extract);

        // if it is an A vertex
        if(extract < m){
            for(int j = 0; j < M[extract].size(); j++){
                int new_dist = get<1>(M[extract][j]);
                int b_index  = get<0>(M[extract][j]);
                new_dist =  A[extract] + B[b_index] - new_dist;
                if(distance[extract] + new_dist < distance[m + b_index]){
                    // cout << m + b_index << " " << distance[extract] + new_dist << endl;
                    distance[m + b_index] = distance[extract] + new_dist;
                    
                    predecessor[m + b_index] = extract;
                    queue.erase(pointers[m + b_index]);
                    pointers[m + b_index] = {distance[m + b_index] ,m + b_index};
                    queue.insert(pointers[m + b_index]);
                }
            }

        }
        // if it is a  B vertex

        else{
            int b_index = extract - m;
            int a_index = matching_b[b_index];
            if(a_index != -1){
                if( distance[extract] < distance[a_index]){
                    distance[a_index] = distance[extract];
                    predecessor[a_index] = extract;
                    queue.erase(pointers[a_index]);
                    pointers[a_index] = {distance[a_index] , a_index};
                    queue.insert(pointers[a_index]);
                }
            }
            else{
                flag = true;
                match = extract;
                match_dist = distance[extract];
                break;
            }
        }
    }

    if(!flag){
        // cout << "No saturating matching exists!" << endl;
        // for(int i = 0; i < m+n; i++){
        //     delete(pointers[i]);
        // }
        return false;
    }
    else{
        // cout << "Found match, augmenting" << endl;
    }

    // Augment and Update
    int updateval = match_dist;
    int augmenter = match;
    // for(int i = m; i < m+n; i++){
    //     if(matching_b[i - m] == -1){
    //         if(distance[i] <= updateval){
    //             updateval = distance[i];
    //             augmenter = i;
    //         }
    //     }
    // }
    // cout << updateval << endl;
    // cout << augmenter << endl;
    //Update
    for(int i = 0; i < marked_indices.size(); i++){
        int index = marked_indices[i];
        if(distance[index] <= updateval){
            if(index < m){
                A[index] -= updateval - distance[index];
            }
            else{
                int b_index = index - m;
                B[b_index] += updateval - distance[index];
            }
        }
    }

    //Augment
    while(augmenter != -1){
        matching_b[augmenter - m] = predecessor[augmenter];
        matching_a[predecessor[augmenter]] = augmenter - m;
        augmenter = predecessor[predecessor[augmenter]];
    }

    // for(int i = 0; i < m+n; i++){
    //     delete(pointers[i]);
    // }

    return true;

}




int main(){
    int m, n;
    cin >> m >> n;
    
    // adjacency list - weight of edge and index
    // consists of a-index --> b-index
    vector<vector<tuple<int, int>>> M;

    for(int i = 0; i < m; i++){
        int i_degree;
        cin >> i_degree;
        vector<tuple<int, int>> adj;

        while(i_degree--){
            int node, weight;
            cin >> node >> weight;
            adj.push_back({node, weight});
        }


        M.push_back(adj);
    }

    vector<int> weights(n);
    for(int i = 0; i < n; i++){
        cin >> weights[i];
    }

    // cout << "Input" << endl;
    vector<int> A(m);
    // matching b contains index of a and weight of edge
    vector<int> matching_b(n,-1); // set value = -1 if not matched, else index
    vector<int> matching_a(m, -1);
    vector<int> B(n, 0);

    for(int i = 0; i < m; i++){
        for(int j = 0; j < M[i].size(); j++){
            A[i] = max(A[i], get<1>(M[i][j]));
        }
    }
    // for(int i = 0; i < A.size(); i++){
    //     cout << A[i] << " ";
    // }
    // cout << endl;
    // for(int i = 0; i < B.size(); i++){
    //     cout << B[i] << " ";
    // }

    for(int i = 0; i < m; i++){
        // cout << "Matching " << i << endl;
        if (!DijkstrAugment(M, matching_b, matching_a, A, B, i, m, n)){
            cout << "No A-Saturating Matching Exists"<<endl;
            return 0;
        }
        // for(int i = 0; i < A.size(); i++){
        //     cout << A[i] << " ";
        // }
        // cout<< endl;
        // for(int i = 0; i < B.size(); i++){
        //     cout << B[i] << " ";
        // }

    }
    int sum = 0;

    cout << "Maximum Weight A-saturating Matching: "<< endl;
    for(int i = 0; i < matching_a.size(); i++){
        cout << i << " : " << matching_a[i] << endl;
    }
    // cout << "Weight of the Matching = " << <<endl;

}

#include<bits/stdc++.h>
using namespace std;

// using bruteforce dijkstra O(n^2)

const int INFTY = 10000000;
// maximum weight saturating matching

bool DijkstrAugment(const vector<vector<tuple<int,int>>>& M,
                    vector<int>& matching_b, vector<int>& matching_a,
                    vector<int> &A, vector<int> &B,
                    int A_index, int m, int n){

    //O(E + n^2) dijkstra implementation, not using priorityqueue/heaps
    // first m are A vertices, next n are B vertices

    vector<int> marked(m+n, false);
    vector<int> distance(m+n, INFTY);
    vector<int> predecessor(m+n, -1);
    vector<int> marked_indices;

    //initialize source
    distance[A_index] = 0;
    bool flag = false;
    int match = -1;
    int match_dist = -1;

    for(int i = 0; i < m+n; i++){

        int extract = -1;
        for(int j = 0; j < m+n; j++){
            if(!marked[j] && (distance[j] < ((extract == -1) ? INFTY + 1 : distance[extract]))){
                extract = j;
            }
        }
        // cout << "Extracted: " << extract << endl;
        if(distance[extract] == INFTY){
            break;
        }
        
        marked[extract] = true;
        marked_indices.push_back(extract);

        // if it is an A vertex
        if(extract < m){
            for(int j = 0; j < M[extract].size(); j++){
                int new_dist = get<1>(M[extract][j]);
                int b_index  = get<0>(M[extract][j]);
                new_dist =  A[extract] + B[b_index] - new_dist;
                if(distance[extract] + new_dist < distance[m + b_index]){
                    distance[m + b_index] = distance[extract] + new_dist;
                    predecessor[m + b_index] = extract;
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
                }
            }
            else{
                flag = true;
                match = extract;
                match_dist = distance[extract];
                break;
            }
        }

        // for(int j= 0; j < m+n; j++){
        //     cout << distance[j] << " ";
        // }
        // cout << endl;
    }

    if(!flag){
        //cout << "No saturating matching exists!" << endl;
        return false;
    }
    // else{
    //     cout << "Found match, augmenting" << endl;
    // }

    // Augment and Update
    int updateval = INFTY;
    int augmenter = -1;
    for(int i = m; i < m+n; i++){
        if(matching_b[i - m] == -1){
            if(distance[i] <= updateval){
                updateval = distance[i];
                augmenter = i;
            }
        }
    }
    // cout << updateval << endl;
    // cout << augmenter << endl;
    //Update
    for(int i = 0; i < marked_indices.size(); i++){
        int index = marked_indices[i];
        if(marked[index] && distance[index] <= updateval){
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

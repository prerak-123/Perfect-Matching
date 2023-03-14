#include<bits/stdc++.h>
using namespace std;
// O(n^4) implementation of Hungarian Method
const int INFTY = 10000000;

int main(){

    int m, n;
    cin >> m >> n;
    
    vector<vector<int>> M(m);


    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            int w;
            cin >> w;
            M[i].push_back(w);
        }
    }

    cout << "Input Done" << endl;
    vector<int> A(m);
    vector<int> matching(m,-1); // set value = -1 if not matched, else index
    vector<int> B(n, 0);

    for(int i = 0; i < m; i++){
        A[i] = *max_element(M[i].begin(), M[i].end());
    }

    bool flag_1 = false, flag_2 = false;
    // flag_1 = found augmenting path to unmatched B vertex
    // flag_2 = found augmenting path to 0 weight vertex

    for (int i = 0; i < m; i++){
        cout << "matching vertex " << i << endl; 
        queue<int> bfs_queue;
        vector<bool> visited_a(m, false);
        vector<int> predecessor_a(m, -1);
        flag_1 = false;
        flag_2 = false;

        
    
        //bfs queue only holds A vertices
        while (matching[i] == -1 && A[i] != 0 ){
            bfs_queue.push(i);

            while ((flag_1 && flag_2) == false && bfs_queue.size() > 0){
                
                int row = bfs_queue.front();
                bfs_queue.pop(); 
                visited_a[row] = true;

                // break if flag_2 holds
                if(A[row] == 0){
                    flag_2 = true;
                    // Augment();

                    //augment bot up
                    int augmenter = row;
                    int temp = matching[augmenter];
                    int temp_2 = -1;

                    while(augmenter != i){           
                        matching[augmenter] = temp_2;
                        temp_2 = temp;
                        temp = matching[predecessor_a[augmenter]];
                        augmenter = predecessor_a[augmenter];
                    }
                    matching[augmenter] = temp_2;
                    break;
                }

                bool other_tight_exists = false;

                for(int j = 0; j < n; j++){
                    
                    if (A[row] + B[j] == M[row][j] && j != matching[row]){
                        
                        auto it = find(matching.begin(), matching.end(), j);

                        if ( it == matching.end()){
                            other_tight_exists = true;
                            flag_1 = true;
                            // Augment();

                            //augment bot up
                            int augmenter = row;
                            int temp = matching[augmenter];
                            int temp_2 = j;

                            while(augmenter != i){           
                                matching[augmenter] = temp_2;
                                temp_2 = temp;
                                temp = matching[predecessor_a[augmenter]];
                                augmenter = predecessor_a[augmenter];
                            }
                            matching[augmenter] = temp_2;

                            break;
                        }

                        else if ( !visited_a[it - matching.begin()] ){
                            other_tight_exists = true;
                            bfs_queue.push(it - matching.begin());
                            predecessor_a[it - matching.begin()] = row;
                            
                        }
                    }

                    visited_a[row] == true;
                }

                if(!other_tight_exists){
                        //adjust weights seems O(n^2) actually
                    //Optimize();

                    int cost = INFTY;
                    for(int l = 0; l < m; l++){

                        if(visited_a[l]){
                            int new_cost = INFTY;
                            for(int k = 0; k < n; k++){
                                if(A[l] + B[k] - M[l][k] == 0){
                                    new_cost = min(new_cost, A[l]);
                                }
                                else{
                                    new_cost = min(new_cost, min(A[l] + B[k] - M[l][k], A[l]));
                                } 
                                // change this line to allow matchings or perfect matchings
                            }
                            cost = min(cost, new_cost); 
                        }                     
                        
                    }
                    cout << "Optimizing dual by " << cost << endl;
                    vector<bool> cost_reassign_b(n, false);
                    for(int l = 0; l < m; l++){
                        if(visited_a[l]){
                            for(int k = 0; k < n; k++){
                                if(A[l] + B[k] - M[l][k] == 0 && !cost_reassign_b[k]){
                                    cost_reassign_b[k] = true;
                                    B[k] += cost;
                                }
                            }
                            A[l] -= cost;
                        }
                    }




                    // clear queue, clear predecessor, clear visited and repeat search
                    
                    bfs_queue = queue<int>();                
                    for(int k = 0; k < m; k++){
                        visited_a[k] = false;
                        predecessor_a[k] = -1;
                    }
                    break;
                }
            }
        }

    }

    int sum = 0;
    for(int i = 0; i < m; i++){
        cout << i << " -- " << matching[i] << endl;
        sum += matching[i] != -1 ? M[i][matching[i]] : 0;
    }
    cout << sum;
}

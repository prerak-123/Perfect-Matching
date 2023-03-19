#include<iostream>
#include<vector>
#include<algorithm>

struct Job{
    int start;
    int end;
    int matchedDay;
    bool toVisit;
    int parent;
    bool fixed;

    std::vector<int> adjacencyList;

    Job(){
        matchedDay = -1;
        toVisit = false;
        parent = -1;
        fixed = false;
    }
};

struct Day{
    int day;
    std::vector<int> adjacencyList;
    int matchedJob;
    bool toVisit;
    int parent;
    long long unsigned int weight;
    bool fixed;

    Day(){
        toVisit = false;
        matchedJob = -1;
        parent = -1;
        fixed = false;
    }

    bool operator < (Day const &day2){
        return this->weight < day2.weight;
    } 
};

bool augmentMatching(Job* jobs, Day* days, int dayIndex){
    std::vector<std::pair<bool, int>> BFSQueue;
    days[dayIndex].toVisit = true;
    BFSQueue.push_back({true, dayIndex});
    int queueIndex = 0;
    bool success = false;
    while(true){
        bool inDays = BFSQueue[queueIndex].first;
        int currNode = BFSQueue[queueIndex].second;

        if(inDays){
            for (auto job: days[currNode].adjacencyList){
                if(jobs[job].matchedDay == -1){
                    success = true;
                    
                    days[currNode].matchedJob = job;
                    jobs[job].matchedDay = currNode;

                    while(days[currNode].parent != -1){
                        int jobToAugment = days[currNode].parent;
                        int dayToAugment = jobs[jobToAugment].parent;

                        days[dayToAugment].matchedJob = jobToAugment;
                        jobs[jobToAugment].matchedDay = dayToAugment;
                        currNode = dayToAugment;
                    }
                    break;
                }

                else if(!jobs[job].toVisit && !jobs[job].fixed){
                    BFSQueue.push_back({false, job});
                    jobs[job].toVisit = true;
                    jobs[job].parent = currNode;
                }
            }
        }
        else{
            int matchedVertex = jobs[currNode].matchedDay;
            if(!days[matchedVertex].toVisit && !days[matchedVertex].fixed){
                BFSQueue.push_back({true, matchedVertex});
                days[matchedVertex].toVisit = true;
                days[matchedVertex].parent = currNode;
            }
        }

        queueIndex++;
        if(queueIndex == BFSQueue.size() || success){
            break;
        }
    }

    for (auto i: BFSQueue){
        if(i.first){
            days[i.second].parent = -1;
            days[i.second].toVisit = false;
            if(!success){
                days[i.second].fixed = true;
            }
        }

        else{
            jobs[i.second].parent = -1;
            jobs[i.second].toVisit = false;
            if(!success){
                jobs[i.second].fixed = true;
            }
        }
    }
    
    return success;
}

int main(){
    int numJobs, numDays;
    std::cin >> numJobs >> numDays;

    Job jobs[numJobs];
    Day days[numDays];

    for(int i = 0; i < numJobs; i++){
        int start, end;
        std::cin >> start >> end;
        jobs[i].start = start - 1;
        jobs[i].end = end - 1;
    }

    for(int i = 0; i < numDays; i++){
        int weight;
        std::cin >> weight;
        days[i].day = i;
        days[i].weight = weight;
    }

    std::sort(days, days + numDays);

    for (int dayIndex = 0; dayIndex < numDays; dayIndex++){
        for(int job = 0; job < numJobs; job++){
            if(days[dayIndex].day >= jobs[job].start && days[dayIndex].day <= jobs[job].end){
                jobs[job].adjacencyList.push_back(dayIndex);
                days[dayIndex].adjacencyList.push_back(job);
            }
        }
    }

    long long unsigned int totalCost = 0;
    int jobsDone = 0;

    for(int dayIndex = 0; dayIndex < numDays; dayIndex++){  
        if(augmentMatching(jobs, days, dayIndex)){
            totalCost += days[dayIndex].weight;
            jobsDone++;
        }

        if(jobsDone == numJobs){
            std::cout << "1\n" << totalCost << std::endl;
            return 0;
        }
    }

    std::cout << "0\n" << totalCost << std::endl;
    return 0;

}
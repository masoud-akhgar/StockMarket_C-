#include "main.hpp"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <list>
#include <typeinfo>
#include <queue>
#include <sstream>
#include<algorithm>
using namespace std;

bool sellAgressiveFlag=true;    // if agressive offer is selling - use it in operator function
int clockTime=0;

int charToNumber(char c){
    if(c<48 || c>57)return 0;
    return int(c)-48;
}

struct NodeOffer{
    int id;
    char side;      // either S or B
    char symbol;    // either A or T (or W)
    int volume;
    float priceInteger;
    int timeTurn;
    bool operator<(const NodeOffer& s2){
        float thisPrice=s2.priceInteger;
        float otherPrice = priceInteger;
        if (thisPrice == otherPrice) return s2.timeTurn > timeTurn;
        if(sellAgressiveFlag) return thisPrice < otherPrice ;    // more pricy, more priority
        else return thisPrice > otherPrice;                     // otherwise
    }
};
bool cmprFun(const NodeOffer& s2,const float& val){
    float thisPrice=s2.priceInteger;
    if(sellAgressiveFlag)return thisPrice > val ; 
    else return thisPrice < val ; 
}
bool cmprFun2(const float& val,const NodeOffer& s2){
    float thisPrice=s2.priceInteger;
    if(sellAgressiveFlag)return thisPrice < val ; 
    else return thisPrice > val ; 
}
bool cmprFunTime(const int& valtime, const NodeOffer& s2){
    int thisTurn=s2.timeTurn;
    return thisTurn > valtime ; 
}
vector<NodeOffer> stock;    // First, tried to implement by priority queue, however, we need access to elements

void pull(int argid){
    for(int j=0; j < stock.size() ; j++ ) 
        if( stock[j].id == argid ){
            stock.erase(remove_if(stock.begin(), stock.end(), [argid](const NodeOffer& i){return i.id==argid;}), stock.end());
            break;
        }
    return;
}

vector<int> collectPotentialRecords(NodeOffer aggressiveOffer){
    vector<int> potentialRecordsIndex;
    potentialRecordsIndex.clear();
    // we need index of Stock elements, so we need to implement for loops
    if(aggressiveOffer.symbol == 'A'){ // look for APPL symbol records
        if( aggressiveOffer.side == 'S' ){ // look for buy records
            // collect a list of potential candidates
            for (int i = 0; i < stock.size(); i++){
                NodeOffer it = stock[i];
                if(it.side== 'S' || it.symbol != 'A' )continue;
                if(aggressiveOffer.priceInteger <= it.priceInteger){
                    potentialRecordsIndex.push_back(i);
                }
            }
        }else{              // look for sell records
            // collect a list of potential candidates
            for (int i = 0; i < stock.size(); i++){
                NodeOffer it = stock[i];
                if(it.side== 'B' || it.symbol != 'A' )continue;
                if(aggressiveOffer.priceInteger >= it.priceInteger){
                    potentialRecordsIndex.push_back(i);
                }
            }
        }
    }
    else if(aggressiveOffer.symbol == 'T'){                   // look for APPL symbol records
        if( aggressiveOffer.side == 'S' ){ // look for buy records
            // collect a list of potential candidates
            for (int i = 0; i < stock.size(); i++){
                NodeOffer it = stock[i];
                if(it.side== 'S' || it.symbol != 'T' )continue;
                if(aggressiveOffer.priceInteger <= it.priceInteger){
                    potentialRecordsIndex.push_back(i);
                }
            }
        }else{              // look for sell records
            // collect a list of potential candidates
            for (int i = 0; i < stock.size(); i++){
                NodeOffer it = stock[i];
                if(it.side== 'B' || it.symbol != 'T' )continue;
                if(aggressiveOffer.priceInteger >= it.priceInteger){
                    potentialRecordsIndex.push_back(i);
                }
            }
        }
    }if(aggressiveOffer.symbol == 'W'){
        if( aggressiveOffer.side == 'S' ){ // look for buy records
            // collect a list of potential candidates
            for (int i = 0; i < stock.size(); i++){
                NodeOffer it = stock[i];
                if(it.side== 'S' || it.symbol != 'W' )continue;
                if(aggressiveOffer.priceInteger <= it.priceInteger){
                    potentialRecordsIndex.push_back(i);
                }
            }
        }else{              // look for sell records
            // collect a list of potential candidates
            for (int i = 0; i < stock.size(); i++){
                NodeOffer it = stock[i];
                if(it.side== 'B' || it.symbol != 'W' )continue;
                if(aggressiveOffer.priceInteger >= it.priceInteger){
                    potentialRecordsIndex.push_back(i);
                }
            }
        }
    }
    return potentialRecordsIndex;
}

pair<vector<NodeOffer>::iterator,vector<NodeOffer>::iterator> findRangePrice(NodeOffer aggressiveOffer, vector<NodeOffer> *destRep){
    vector<NodeOffer>::iterator it_begin = lower_bound(destRep->begin(),destRep->end(), aggressiveOffer.priceInteger, cmprFun);
    vector<NodeOffer>::iterator it_end = upper_bound(it_begin,destRep->end(), aggressiveOffer.priceInteger, cmprFun2);
    return make_pair(it_begin, it_end);
}

void pushPriorityBased(NodeOffer aggressiveOffer, vector<NodeOffer> *destRep){// First, tried to implement  by priority queue, however, we need access to elements
    
    if(destRep->size()!=0){
        pair<vector<NodeOffer>::iterator,vector<NodeOffer>::iterator> it_pair = findRangePrice(aggressiveOffer, destRep);
        vector<NodeOffer>::iterator it_begin = upper_bound(it_pair.first,it_pair.second, aggressiveOffer.timeTurn, cmprFunTime);
        destRep->insert(it_begin, aggressiveOffer);
    }
    else destRep->push_back(aggressiveOffer);
}

vector<std::string> checkMatching(NodeOffer aggressiveOffer){
    // sort direction
    if( aggressiveOffer.side == 'B' ){
        sellAgressiveFlag=false;
        vector<NodeOffer> filteredNodes;
        for (int i=0;i<stock.size();i++)pushPriorityBased(stock[i],&filteredNodes);
        stock = filteredNodes;
    }
    else{
        sellAgressiveFlag=true;
        vector<NodeOffer> filteredNodes;
        for (int i=0;i<stock.size();i++)pushPriorityBased(stock[i],&filteredNodes);
        stock = filteredNodes;
    } 
    
    vector<int> potentialRecordsIndex = collectPotentialRecords(aggressiveOffer); // having the indexes in stock
    
    
    vector<std::string> output;
    output.clear();
    for (int it = 0; it < potentialRecordsIndex.size(); it++){
        std::stringstream sstm;
        int id = potentialRecordsIndex[it];
        
        int dealVolume=min(stock[id].volume, aggressiveOffer.volume);
        stock[id].volume -= dealVolume;
        aggressiveOffer.volume -= dealVolume;
        
        if(aggressiveOffer.symbol=='A')
            sstm<<"AAPL,"<<stock[id].priceInteger<<","<<dealVolume<<","<<aggressiveOffer.id<<","<<stock[id].id;
        else if(aggressiveOffer.symbol=='T')sstm<<"TSLA,"<<stock[id].priceInteger<<","<<dealVolume<<","<<aggressiveOffer.id<<","<<stock[id].id;
        else sstm<<"WEBB,"<<stock[id].priceInteger<<","<<dealVolume<<","<<aggressiveOffer.id<<","<<stock[id].id;
        output.push_back(sstm.str());
        if(aggressiveOffer.volume ==0)break;
    }
    for(const int& id_p : potentialRecordsIndex ){
        if( stock[id_p].volume == 0 ){
            pull(stock[id_p].id);
        }         
    }
    if(aggressiveOffer.volume !=0)pushPriorityBased(aggressiveOffer, &stock);
    else pull(aggressiveOffer.id);
    return output;
}

vector<std::string> waitingOffers(){
    vector<std::string> output;
    output.clear();   
    char symbArr[3] = {'A','T','W'};
    char sideArr[3] = {'S','B'};
    
    
    
    for( char& symb : symbArr){
        vector<pair<float, int>> forSalePairs,forBuyPairs;
        forSalePairs.clear();forBuyPairs.clear();   // just in case
        for( char& sideEL : sideArr){
            
            vector<NodeOffer> filteredNodes;
            filteredNodes.clear();  // just in case
            copy_if(stock.begin(), stock.end(), back_inserter(filteredNodes), [sideEL,symb](const NodeOffer& i){return i.side==sideEL && i.symbol==symb && i.volume!=0;}); 
            if(filteredNodes.size()==0)continue;
            
            // tune sort order:
            if(sideEL=='S')sellAgressiveFlag=false;
            else sellAgressiveFlag=true;
            sort(filteredNodes.begin(),filteredNodes.end());    //sort based on price and time(id)// more price, less index

            int volumeSum=0;float priceTemp=0.0;
            for ( int it = 0 ; it < filteredNodes.size() ; it++ ){
                if(priceTemp == filteredNodes[it].priceInteger)volumeSum += filteredNodes[it].volume;
                else {
                    // fill two side of the buy/sell queue
                    if(priceTemp!=0 && sideEL=='S')forSalePairs.push_back(make_pair(priceTemp,volumeSum));
                    if(priceTemp!=0 && sideEL=='B')forBuyPairs.push_back(make_pair(priceTemp,volumeSum));
                    priceTemp = filteredNodes[it].priceInteger;
                    volumeSum=filteredNodes[it].volume;
                }
            }
            if(priceTemp!=0 && sideEL=='B')forBuyPairs.push_back(make_pair(priceTemp,volumeSum));
            if(priceTemp!=0 && sideEL=='S')forSalePairs.push_back(make_pair(priceTemp,volumeSum));
        }
        
        int waitingSize = forSalePairs.size() + forBuyPairs.size();
        if(symb=='A' && waitingSize!=0)output.push_back("===AAPL===");else
        if(symb=='T' && waitingSize!=0)output.push_back("===TSLA===");else
        if(symb=='W' && waitingSize!=0)output.push_back("===WEBB===");
        
        for(int i=0,j=0; i<forSalePairs.size()&&j<forBuyPairs.size();i++,j++){
            std::stringstream sstm;
            sstm<<forBuyPairs[j].first<<","<<forBuyPairs[j].second<<","<<forSalePairs[i].first<<","<<forSalePairs[i].second; 
            output.push_back(sstm.str());
        }
        for(int i=forBuyPairs.size(); i<forSalePairs.size();i++){
            std::stringstream sstm;
            sstm<<","<<","<<forSalePairs[i].first<<","<<forSalePairs[i].second; 
            output.push_back(sstm.str());
        }
        for(int j=forSalePairs.size(); j<forBuyPairs.size();j++){
            std::stringstream sstm;
            sstm<<forBuyPairs[j].first<<","<<forBuyPairs[j].second<<","<<","; 
            output.push_back(sstm.str());
        }
    }
    return output;
}

vector<std::string> insertFunction(char* pch){
    NodeOffer temp;
    pch = strtok (NULL, " ,.-");
    temp.id=atoi(pch);
    pch = strtok (NULL, " ,.-");
    temp.symbol=pch[0];
    pch = strtok (NULL, " ,.-");
    temp.side=pch[0];
    pch = strtok (NULL, " ,-");
    temp.priceInteger=stof(pch);
    pch = strtok (NULL, " ,.-");
    temp.volume=atoi(pch);
    temp.timeTurn = clockTime++;
    
    vector<std::string> outputList = checkMatching(temp);
    return outputList;
}

vector<std::string> amendFunction(char* pch){//,<price>,<volume>
    pch = strtok (NULL, " ,.-");
    int nodeId=atoi(pch);
    pch = strtok (NULL, " ,-");
    float nodePrice=stof(pch);
    pch = strtok (NULL, " ,.-");
    int nodeVolume=atoi(pch);
    vector<std::string> output;
    output.clear(); //just in case
    
    for(int j = 0 ; j < stock.size() ; j++){
        if(stock[j].id == nodeId){
            if(nodePrice != stock[j].priceInteger){
                stock[j].volume=nodeVolume;
                stock[j].timeTurn=clockTime++;
                if((nodePrice < stock[j].priceInteger && stock[j].side=='S') || (nodePrice > stock[j].priceInteger && stock[j].side=='B')){     // if updating the cost brings a matching possiblity
                stock[j].priceInteger=nodePrice;
                output = checkMatching(stock[j]) ;      // then check matching       
                }else stock[j].priceInteger=nodePrice;
            }else{
                if(nodeVolume <= stock[j].volume){  // no time penalty
                    stock[j].volume=nodeVolume;
                }
                else{
                    stock[j].volume=nodeVolume;
                    stock[j].timeTurn=clockTime++;
                    NodeOffer n1;
                    memcpy(&n1, &stock[j], sizeof(stock[j]));
                    pull(stock[j].id);
                    pushPriorityBased(n1, &stock);
                    
                }
            }
            break;
        }
    }
    return output;
}

vector<string> run(vector<string> const& input) {
    vector<std::string> output;
    stock.clear();
    for(const string& term : input){
        char *inpstr = const_cast<char*>(term.c_str());
        char * pch;
        pch = strtok (inpstr,",");
        if ( strcmp(pch,"INSERT") == 0 ){
            vector<std::string> tempVec = insertFunction(inpstr);
            output.insert(output.end(), tempVec.begin(), tempVec.end());
        }
        else if ( strcmp(pch,"PULL") == 0 ){
            pch = strtok (NULL, " ,.-");
            pull(atoi(pch));
        }else{  //  Amend
            vector<std::string> tempVec = amendFunction(inpstr);
            output.insert(output.end(), tempVec.begin(), tempVec.end());
        }
    }
    
    vector<std::string> tempVec = waitingOffers();
    output.insert(output.end(), tempVec.begin(), tempVec.end());
    
    return output;
}

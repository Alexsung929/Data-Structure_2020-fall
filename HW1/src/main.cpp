#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

class Page_rank{
    public:
    int page;
    double rank;
    Page_rank(int a ,double b){this->page = a ; this->rank=b;};
    Page_rank(){};
};
bool compare_PR(Page_rank a, Page_rank b){return a.rank>b.rank;};

int main(int argc, char* argv[])
{
    vector<vector<int>> out_link(501);
    vector<vector<string>> words(500);
    for (int i=0;i<501;i++){
        fstream fin("input/data/page"+to_string(i));
        string x ;
        while(fin>>x){
            if (x=="---------------------"){
                break;}
            else{
                int num = stoi(x.substr(4));
                out_link[i].push_back(num);
            }
        }
        while(fin>>x){
            words[i].push_back(x);
        }
    }

    //eliminate dead end
    // for (auto&i : out_link){
    //     if (i.size()==0){
    //         vector<int>temp(500,1);
    //         i = temp;
    //     }
    // }

    vector<vector<int>> pointed(501,vector<int>());
    for (int i=0;i<501;i++){
        for (auto j : out_link[i]){
            pointed[j].push_back(i);
        }
    }

    vector<double> PR(501);
    for (int i =0 ;i<501;i++){
            PR[i] = 1/501;
    }

    double d = 0.25;
    double diff=0; double DIFF = 0.1;
    while (true){
        diff = 0;
        for (int i =0 ;i<501;i++){
            double transion = 0; double PR_before = PR[i];
            for (auto j : pointed[i]){
                double size = out_link[j].size();
                transion = transion + PR[j]/size;
            }
            PR[i] = (1-d)/501+ d*transion;
            diff = diff + abs(PR_before-PR[i]);
        }
        if (diff<=DIFF){
            break;
        }
    }

    vector<Page_rank> result1(501,Page_rank());
    for (int i = 0 ; i <501 ; i++){
        result1[i] = Page_rank(i,PR[i]);
    }
    fstream fout1;
    string name1 = "pr_"+to_string(d).substr(2,2)+"_"+to_string(DIFF).substr(2,3)+".txt";
    fout1.open(name1,ios::out);
    sort(result1.begin(),result1.end(),compare_PR);
    for (auto x : result1){
        fout1 <<"page"<<left<<setw(5)<<x.page<<' '<<setw(5)<<out_link[x.page].size()<<' '<<x.rank<<endl;
    }
    //d = 0.25, 0.45, 0.65, 0.85 and DIFF = 0.100, 0.010, 0.001



//==============================Q2================================

map<string,vector<int>> WL;
for (int i = 0 ; i<500;i++){
    for (int j = 0 ; j<words[i].size() ; j++){
        string name = words[i][j];
        vector<int>temp = WL[name];
        vector<int>::iterator it;
        it = find (temp.begin(), temp.end(),i);
        if (it != temp.end()){
            continue;}
        temp.push_back(i);
        WL[name] = temp;
    }
}

fstream fout2;
fout2.open("Reverse_index.txt",ios::out);
for (auto x : WL){
    string index;
    for (auto i : x.second){
        index = index + "page" + to_string(i) + " ";
    }
    fout2 << setw(10)<<x.first <<" "<< index <<endl;
}



//==============================Q3==================================

vector<string> list;
fstream fin3("list.txt");
fstream fout3;
string name3 = "result_"+to_string(d).substr(2,2)+"_"+to_string(DIFF).substr(2,3)+".txt";
fout3.open(name3,ios::out);
string wd;
int row=0;
while (getline(fin3,wd)){
    list.push_back(wd);
}

vector<string> line_words(500);
for (int i=0; i<500;i++){
    string string_line;
    for (auto x: words[i]){
        string_line = string_line + x + " ";
    }
    line_words[i] = string_line;
}

for (auto i : list){
    string str;	
    stringstream ss;
    vector<string> res;
    ss<<i;
    while (getline(ss, str, ' ')){
        res.push_back(str);
    }
    //只看一個字的
    if (res.size()==1){
        string result;
        //最大的前10個
        int top10 = min(10,int(WL[i].size()));
        //都沒有match的
        if (top10==0){
            fout3 <<"none"<<endl;
            continue;
        }
        else{
            vector<Page_rank> r3(top10,Page_rank()); int z = 0;
            for (auto x : WL[i]){
                if (z>=top10){
                    break;
                }
                r3[z] = Page_rank(x,PR[x]);
                z++;
            }
            sort(r3.begin(),r3.end(),compare_PR);

            for (auto j: r3){
                result = result + "page" + to_string(j.page)+" ";
            }
            fout3 << result <<endl;
        }
    }
    else{
        //看AND
        vector<int> AND_result = WL[res[0]];
        for (int x=0;x<res.size();x++){
            vector<int> temp_and;
            set_intersection(AND_result.begin(),AND_result.end(),WL[res[x]].begin(),WL[res[x]].end(),back_inserter(temp_and));
            AND_result = temp_and;
        }

        if (AND_result.size()==0){
            fout3<<"AND none"<<endl;;
        }
        else{
            string result_and;
            //最大的前10個
            int len = AND_result.size();
            vector<Page_rank> r3_and(len,Page_rank()); int z = 0;
            for (auto x : AND_result){
                r3_and[z] = Page_rank(x,PR[x]);
                z++;
            }
            sort(r3_and.begin(),r3_and.end(),compare_PR);

            int and_count = 0 ;
            for (auto j: r3_and){
                if (and_count>=10){
                    break;
                }
                result_and = result_and + "page"+to_string(j.page)+" ";
                and_count ++ ;
            }
            fout3 << "AND" << " " << result_and <<endl;
        }

        //看OR
        vector<int> OR_result = WL[res[0]];
        for (int x=0;x<res.size();x++){
            vector<int> temp_or;
            set_union(OR_result.begin(),OR_result.end(),WL[res[x]].begin(),WL[res[x]].end(),back_inserter(temp_or));
            OR_result = temp_or;
        }
        if (OR_result.size()==0){
            fout3<<"OR none"<<endl;;
        }
        else{
            string result_or;
            //最大的前10個
            int len = OR_result.size();
            vector<Page_rank> r3_or(len,Page_rank()); int z = 0;
            for (auto x : OR_result){
                r3_or[z] = Page_rank(x,PR[x]);
                z++;
            }
            sort(r3_or.begin(),r3_or.end(),compare_PR);

            int or_count = 0 ; 
            for (auto j: r3_or){
                if (or_count >= 10){
                    break;
                }
                result_or = result_or + "page"+to_string(j.page)+" ";
                or_count ++;
            }
            fout3 << "OR" << " " << result_or <<endl;
        }
    }
    }


//===============================================search engine====================================================
    string input;
    
    cout << "Enter word: "<< "(enter *end* to leave)"<<endl;
    getline(cin,input);
    while(input!="*end*")
        {
        string str;	
        stringstream ss;
        vector<string> res;
        ss<<input;
        while (getline(ss, str, ' ')){
            res.push_back(str);
        }
        //只看一個字的
        if (res.size()==1){
            string result;
            //最大的前10個
            int top10 = min(10,int(WL[input].size()));
            //都沒有match的
            if (top10==0){
                cout <<"none"<<endl;
            }
            else{
                vector<Page_rank> r3(top10,Page_rank()); int z = 0;
                for (auto x : WL[input]){
                    if (z>=top10){
                        break;
                    }
                    r3[z] = Page_rank(x,PR[x]);
                    z++;
                }
                sort(r3.begin(),r3.end(),compare_PR);

                for (auto j: r3){
                    result = result + "page" + to_string(j.page)+" ";
                }
                cout << result <<endl;
            }
        }
        else{
            //看AND
            vector<int> AND_result = WL[res[0]];
            for (int x=0;x<res.size();x++){
                vector<int> temp_and;
                set_intersection(AND_result.begin(),AND_result.end(),WL[res[x]].begin(),WL[res[x]].end(),back_inserter(temp_and));
                AND_result = temp_and;
            }

            if (AND_result.size()==0){
                cout<<"AND none"<<endl;;
            }
            else{
                string result_and;
                //最大的前10個
                int len = AND_result.size();
                vector<Page_rank> r3_and(len,Page_rank()); int z = 0;
                for (auto x : AND_result){
                    r3_and[z] = Page_rank(x,PR[x]);
                    z++;
                }
                sort(r3_and.begin(),r3_and.end(),compare_PR);

                int and_count = 0 ;
                for (auto j: r3_and){
                    if (and_count>=10){
                        break;
                    }
                    result_and = result_and + "page"+to_string(j.page)+" ";
                    and_count ++ ;
                }
                cout << "AND" << " (" << input << ") " << result_and <<endl;
            }

            //看OR
            vector<int> OR_result = WL[res[0]];
            for (int x=0;x<res.size();x++){
                vector<int> temp_or;
                set_union(OR_result.begin(),OR_result.end(),WL[res[x]].begin(),WL[res[x]].end(),back_inserter(temp_or));
                OR_result = temp_or;
            }
            if (OR_result.size()==0){
                cout<<"OR none"<<endl;;
            }
            else{
                string result_or;
                //最大的前10個
                int len = OR_result.size();
                vector<Page_rank> r3_or(len,Page_rank()); int z = 0;
                for (auto x : OR_result){
                    r3_or[z] = Page_rank(x,PR[x]);
                    z++;
                }
                sort(r3_or.begin(),r3_or.end(),compare_PR);

                int or_count = 0 ; 
                for (auto j: r3_or){
                    if (or_count >= 10){
                        break;
                    }
                    result_or = result_or + "page"+to_string(j.page)+" ";
                    or_count ++;
                }
                cout << "OR" << " (" << input << ") " << result_or <<endl;
            }
        }

    getline(cin,input);
    }

    return 0;
}
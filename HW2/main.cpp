#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int main(int argc, char* argv[]){
    fstream fin(argv[1]); fstream fout;
    fout.open("Dictionary.txt",ios::out);
    string i;
    vector<string> dic;
    vector<string> dic_int(100000);
    vector<long long> dic_pass(100000);
    while (fin>>i){
        dic.push_back(i);
    }
    for (int i = 0 ; i < dic.size() ; i++){
        string word = "";
        for (int z = 0 ; z<dic[i].size();z++){
            word += to_string(int(dic[i][z]));
        }

        for (int j=0; j<1000;j++){
            string salt = to_string(j);
            int size = salt.size();
            for (int x = 3 ; x>size ; x--){
                salt = "0"+salt;
            }
            dic_int[1000*i+j] = salt + word;
        }
    }

    for (int i = 0 ; i < dic_pass.size(); i++){
        string left_s = dic_int[i].substr(0,8);
        long long left = atoi(left_s.c_str());
        string right_s = dic_int[i].substr(8,7);
        long long right = atoi(right_s.c_str());        
        dic_pass[i] =  ((243*left)+right)%85767489;
    }

    for (int i =0 ; i<dic.size();i++){
        for (int j=0; j<1000;j++){
            string salt = to_string(j);
            int size = salt.size();
            for (int x = 3 ; x>size ; x--){
                salt = "0"+salt;
            }
            fout<<dic[i]<<" "<<salt<<" "<<dic_pass[1000*i+j] <<endl;
        }
    }

    fstream gin("list_pa2.txt");
    fstream gout; gout.open("result_pa2.txt",ios::out);
    vector<string> list_pa2;
    while (gin>>i){
        list_pa2.push_back(i);
    }
    for (int i= 0 ; i<list_pa2.size();i++){
        bool pass = false;
        for (int j = 0 ; j<dic_pass.size();j++){
            if (pass == true){break;}
            if (atoi(list_pa2[i].c_str())==dic_pass[j]){
                string salt = to_string(j%1000);
                int size = salt.size();
                int posi = j/1000;
                for (int x = 3 ; x>size ; x--){
                    salt = "0"+salt;
                }
                gout << list_pa2[i]<<" "<< dic[posi] <<" "<< salt <<" "<<j+1<<endl;
                pass = true;
            }
        }
        if (pass == false){gout << list_pa2[i]<<" "<< "******" <<" "<< "***" <<" "<<endl;}
    }

    string input;
    cout << "Enter password: "<<endl;
    while (getline(cin,input)){
        if (input == "*"){break;}
        string answer;
        bool pass = false;
        for(int i = 0 ; i<dic_pass.size(); i++){
            if ( atoi(input.c_str()) == dic_pass[i]) {
                pass = true;
                answer = dic_int[i].substr(3);
                string output;
                for (int i = 0 ; i < answer.size(); i+=2){
                    output += char(atoi(answer.substr(i,2).c_str()));
                }
                cout << "Hash value is "<<input<<" "<< "Password is "<<output<<endl;
                cout<< "The number have been searched: "<< i+1 << endl;
                break;
            }
        }
        if(pass == false){cout <<"Hash value is "<<input<<" "<<"Failed"<<endl;
                        cout<< "The number have been searched:100000 " << endl;
                        }
    }

    // getline(cin,hash_val);
    // int hash_int = atoi(hash_val.c_str());
    // string input_word = "";
    // for (int z = 0 ; z< input.size();z++){
    //     input_word += to_string(int(input[z]));
    // }
    // string hash= to_string(hash_int);
    // int size = hash.size();
    // cout << size<<endl;
    // for (int x = 3 ; x>size ; x--){
    //     hash = "0"+hash;
    // }
    // string input_num = hash + input_word;
    // if (input_num.size()==15){
    //     string left_s = input_num.substr(0,8);
    //     long long left = atoi(left_s.c_str());
    //     string right_s = input_num.substr(8,7);
    //     long long right = atoi(right_s.c_str());        
    //     long long input_hash = ((243*left)+right)%85767489;

    //     for (int i =0 ; i<dic_pass.size();i++){
    //         if (input_hash==dic_pass[i]){
    //             cout <<"pass!"<<" position is "<< i<<endl;
    //             break;
    //         }
    //     }
    // }
}
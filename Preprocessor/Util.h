#ifndef SRC_UTIL_H
#define SRC_UTIL_H


#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

class Util {
public:
    inline void words2Chars(string srcFile, string charFile, string word2CharFile)
    {
        ifstream m_inf;
        m_inf.open(srcFile.c_str());
        string line;
        
        ofstream m_ouf, m_oucf;
		m_oucf.open(charFile.c_str());
		m_ouf.open(word2CharFile.c_str());

		map<string, long long> charsDict;

        int cnt = 0;
        while(getline(m_inf, line)){

            if(cnt++ == -100000)
                break ;

			vector<string> vecs;
			split_bychars(line, vecs, "\t");

            string wordStr = vecs[1];
			
			vector<string> chars;
			string charStr;

            int ind = 0;
			while (true){
                if((wordStr[ind]&0xFF) < 0x80){
					if(wordStr[ind] == 0)
                     {   
                         ind++;
                         break;
                     }
                    
                    charStr = wordStr[ind++];
                }
				else if ((wordStr[ind] & 0xE0) == 0xE0){
                    char tmp[100];
					sprintf(tmp, "%c%c%c", wordStr[ind], wordStr[ind + 1], wordStr[ind + 2]);
					charStr = tmp;
                    
                    ind+=3;
                }
				else if ((wordStr[ind] & 0xC0) == 0xC0){
                    char tmp[100];
					sprintf(tmp, "%c%c", wordStr[ind], wordStr[ind + 1]);
					charStr = tmp;

                    ind+=2;
                }
                else{
                    ind++;
                }
                

                if (charStr != ""){
                    if (charsDict.count(charStr) == 0){
						charsDict.insert(std::pair<string, int>(charStr, 0));
					}
					charsDict[charStr] += atoi(vecs[2].c_str());
				}

				charStr = "";
            }

        }

		m_inf.close();

		map<string, int> charInd;
		vector<pair<string, long long> >  vTemp;

		map<string, long long>::const_iterator map_it = charsDict.begin();
		while (map_it != charsDict.end()){
			vTemp.push_back(make_pair(map_it->first, map_it->second));
			++map_it;
		}

		sort(vTemp.begin(), vTemp.end(), cmpByValue());

		for (int i = 0; i < vTemp.size(); i++){
			m_oucf << i << "\t" << vTemp[i].first << "\t" << vTemp[i].second << endl;
			charInd.insert(std::pair<string, int>(vTemp[i].first, i));
		}
		m_oucf.close();

        cnt = 0;
		m_inf.open(srcFile.c_str());
		while (getline(m_inf, line)){
			vector<string> vecs;
			split_bychars(line, vecs, "\t");
            if(cnt++ == -100000)
                break;
        
			string wordStr = vecs[1];

			vector<int> chars;
			string charStr;

			int ind = 0;
			while (true){
				if ((wordStr[ind] & 0xFF) < 0x80){
					if(wordStr[ind] == 0){
                        ind++;
                        break;
                    }
                    charStr = wordStr[ind++];
				}
				else if ((wordStr[ind] & 0xE0) == 0xE0){
					char tmp[100];
					sprintf(tmp, "%c%c%c", wordStr[ind], wordStr[ind + 1], wordStr[ind + 2]);
					charStr = tmp;

					ind += 3;
				}
				else if ((wordStr[ind] & 0xC0) == 0xC0){
					char tmp[100];
					sprintf(tmp, "%c%c", wordStr[ind], wordStr[ind + 1]);
					charStr = tmp;

					ind += 2;
				}
				else{
                    ind++;
				}
                
                if(charStr!="")
				    chars.push_back(charInd[charStr]);

				charStr = "";
			}
	        m_ouf << vecs[0] << "\t" << join_bystr(chars, "_") << endl;
		}
		
		m_ouf.close();
        m_inf.close();
    }
    
    inline void get_words(const string srcFile, const string dictFile){
        ifstream m_inf;
        m_inf.open(srcFile.c_str());
            
        map<string, int> wordDict;

        string line;
        long long lineCnt = 0;

        while(my_getline(m_inf, line)){

            if(lineCnt++ % 100000 == 0){
                printf("\033[01;31mHaved processed %lld lines!\033[0m\r", lineCnt);
                fflush(stdout);
            }

            vector<string> vecs;
            split_bychars(line, vecs, "\t");
            
            if(vecs.size() != 4)
                continue;

            if(!wordDict.count(vecs[0])){
                wordDict.insert(std::pair<string, int>(vecs[0], 0));
            }
            wordDict[vecs[0]]++;
        }

        printf("Done! Have proccessed %lld\n lines!\n", lineCnt);
        
        m_inf.close();

        ofstream m_ouf;
        m_ouf.open(dictFile.c_str());
        
        vector<pair<string, int> >  vTemp;
        
        map<string, int>::const_iterator map_it = wordDict.begin();
        while(map_it != wordDict.end()){
            vTemp.push_back(make_pair(map_it->first, map_it->second));
            ++map_it;
        }
        
        sort(vTemp.begin(), vTemp.end(), cmpByValue());
        
        for(int i = 0; i < vTemp.size(); i++){
            m_ouf << i << "\t" << vTemp[i].first << "\t" << vTemp[i].second << endl;
        }

        m_ouf.close();
    }

    struct cmpByValue{
        bool operator()(const pair<string, int>& lhs, const pair<string, int>& rhs){
            return lhs.second > rhs.second;
        }
    };

    inline void split_bychars(const string& str, vector<string> & vec, const char *sep = " ") {
        vec.clear();
        string::size_type pos1 = 0, pos2 = 0;
        string word;
        while ((pos2 = str.find_first_of(sep, pos1)) != string::npos) {
            word = str.substr(pos1, pos2 - pos1);
            pos1 = pos2 + 1;
            if (!word.empty())
                vec.push_back(word);
        }
        word = str.substr(pos1);
        if (!word.empty())
            vec.push_back(word);
    }

	inline string join_bystr(const vector<int> &vec, const string &sep) {
		string str = "";
		if (vec.empty())
			return str;
        stringstream ss;
		ss << vec[0];
        
        string tmpStr;
        ss >> tmpStr;

        str = tmpStr;

		int i = 1;
		for (; i < vec.size(); ++i) {
            stringstream tmpss;
            tmpss << vec[i];
            tmpss >> tmpStr;
			str += sep + tmpStr;
		}
        return str;
	}


    inline bool my_getline(ifstream &inf, string &line) {
        if (!getline(inf, line))
            return false;
        int end = line.size() - 1;
        while (end >= 0 && (line[end] == '\r' || line[end] == '\n')) {
            line.erase(end--);
        }
        return true;
    }
};


#endif

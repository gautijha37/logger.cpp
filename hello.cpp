#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<thread>

int main(){
   std::cout << "lmao";
   std::vector<std::string> all{"haha", "hehe"};
   for(const auto& word: all){
      std::cout << word << "\t";
   }
   std::sprintf("this is custom print %s, %d", all[0].c_str(), all.size());
   return 0;
}
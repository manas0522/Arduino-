#include <bits/stdc++.h>
using namespace std;
void print_vector(vector<auto> v){
   cout << "[";
   for(int i = 0; i<v.size(); i++){
      cout << v[i] << ", ";
   }
   cout << "]"<<endl;
}
class Solution {
public:
   vector<int> findAnagrams(string s, string p) {
      map <char, int> m;
      int n = s.size();
      int left = 0, right = 0;
      int counter = p.size();
      vector <int> ans;
      for(int i = 0; i < p.size(); i++) m[p[i]]++;
      for(int right = 0; right < n; right++){
         if(m.find(s[right]) != m.end() && m[s[right]]){
            m[s[right]]--;
            counter--;
            if(counter == 0)ans.push_back(left);
         } else {
            while(left<right){
               if(m.find(s[left]) != m.end()) {
                  counter++;
                  m[s[left]]++;
               }
               left++;
               if(m.find(s[right]) != m.end() && m[s[right]]){
                  right--;
                  break;
               }
            }
            if(m.find(s[left])==m.end())left = right + 1;
         }
      }
      return ans;
   }
};
main(){
   Solution ob;
   print_vector(ob.findAnagrams("cbaebabacd", "abc")) ;
}

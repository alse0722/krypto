#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct message{
   pair<string, string> E;
   string from;
};

struct host{
   string name;
   string en_algo;
   string de_algo;
   vector<pair<message, string> > sent;
   vector<message> get;
   vector<string> msgs;
};

string gen_text(){
   char alpha[73] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789!@#$%^&*<>";
    char msg[20] = {};
    int ind;
    string message;

    for (int i = 0; i < 20; i++)
    {
        ind = rand() % 72;
        msg[i] = alpha[ind];
    }

    message = string(msg, 20);

    return message;
}

void send_message(host &a, host &b){
   message m;

   m.E = make_pair(b.en_algo, gen_text());
   m.from = a.name;

   b.get.push_back(m);
   a.sent.push_back(make_pair(m,b.name));

}

void reply(host &a, host&b){
   message msg(a.get.back());

   msg.E.first = b.en_algo;
   msg.from = a.name;

   a.sent.push_back(make_pair(msg, b.name));
   b.get.push_back(msg);
}

void send_copy(host &a, host &b, host &c){
   message msg(b.get.back());

   msg.from = c.name;

   b.get.push_back(msg);
   c.sent.push_back(make_pair(msg, b.name));   
} 

void decrypt (host &a){
   for (auto m: a.get){
      if (m.E.first == a.en_algo)
         a.msgs.push_back(m.E.second);
      else
         a.msgs.push_back("************");
   }
}

void show(host a){
   printf("\nHost [%s]", a.name.c_str());
   printf("\nEncryption: %s", a.en_algo.c_str());
   printf("\nDecryption: %s", a.de_algo.c_str());
   for(auto  e: a.sent)
      printf("\nSENT {(Encryption: %s, message: %s), %s} to %s", 
         e.first.E.first.c_str(), 
         e.first.E.second.c_str(), 
         e.first.from.c_str(), 
         e.second.c_str());
   
   for(auto e: a.get)
      printf("\nGOT  {(key: %s, message: %s), %s}", 
         e.E.first.c_str(), 
         e.E.second.c_str(), 
         e.from.c_str());

   for(string e: a.msgs)
      printf("\nDECRYPTED: %s", e.c_str());

   printf("\n");
}



int main(){

   setlocale(0, "");
   srand(time(NULL));

   host a, b, c;
   a.name = "A";
   a.en_algo = "E_a";
   a.de_algo = "D_a";
   
   b.name = "B";
   b.en_algo = "E_b";
   b.de_algo = "D_b";

   c.name = "C";
   c.en_algo = "E_c";
   c.de_algo = "D_c";

   send_message(a, b);
   reply(b,a);
   send_copy(a,b,c);
   reply(b,c);

   decrypt(a);
   decrypt(b);
   decrypt(c);


   show (a);
   show (b);
   show (c);
   return 0;
}
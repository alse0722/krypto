#include <iostream>
#include<string.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <limits>

using namespace std;

struct session
{
    string dest_ip;
    int dest_encrypt_algo;
    bool enabled;
};

struct host
{
    string name;
    string ip;
    int encrypt_algo;
    int decrypt_algo;
    vector<session> open_sessions;
};

struct packet
{
    string source_ip;
    string destination_ip;
    int encrypted_by;
    int message;
};

typedef pair<host, host> host_pair;

host get_host()
{

    host host;

    printf("\n_____________[Initialize host]_____________\n");

    printf("[Name]    --> ");
    cin >> host.name;
    printf("[IP]      --> ");
    cin >> host.ip;

    host.encrypt_algo = rand() % (INT16_MAX);
    host.decrypt_algo = rand() % (INT16_MAX);

    printf("[Encrypt] --> %d\n", host.encrypt_algo);
    printf("[Decrypt] --> %d\n", host.decrypt_algo);

    return host;
}

host_pair start_session(host initiator, host affector)
{
    session new_session_init, new_session_affect;

    new_session_init.dest_encrypt_algo = affector.encrypt_algo;
    new_session_init.dest_ip = affector.ip;
    new_session_init.enabled = true;

    new_session_affect.dest_encrypt_algo = initiator.encrypt_algo;
    new_session_affect.dest_ip = initiator.ip;
    new_session_affect.enabled = true;

    initiator.open_sessions.push_back(new_session_init);
    affector.open_sessions.push_back(new_session_affect);

    printf("\n[info] Session [%s] <---> [%s] is UP!\n",
           new_session_init.dest_ip.c_str(),
           new_session_affect.dest_ip.c_str());

    return make_pair(initiator, affector);
}

host_pair drop_session(host initiator, host affector)
{   
    
    vector<session> temp;
    session new_affector, new_initiator;

    new_affector.dest_encrypt_algo = initiator.encrypt_algo;
    new_affector.dest_ip = initiator.ip;
    new_affector.enabled = false;

    new_initiator.dest_encrypt_algo = affector.encrypt_algo;
    new_initiator.dest_ip = affector.ip;
    new_initiator.enabled = false;

    temp = {};
    for(auto s: initiator.open_sessions)
        if (!strcmp(s.dest_ip.c_str(), initiator.ip.c_str()))
            temp.push_back(s);

    initiator.open_sessions = temp;

    temp = {};
    for(auto s: affector.open_sessions)
        if (!strcmp(s.dest_ip.c_str(), affector.ip.c_str()))
            temp.push_back(s);
    
    affector.open_sessions = temp;

    printf("\n[info] Session [%s] <---> [%s] is DOWN!\n",
           initiator.ip.c_str(),
           affector.ip.c_str());

    initiator.open_sessions.push_back(new_initiator);
    affector.open_sessions.push_back(new_initiator);

    return make_pair(initiator, affector);
}

host_pair start_fake_session(host initiator, host affector, host hacker)
{

    host fake_initiator, fake_affector;
    session fake_init_session, fake_affect_session;

    fake_init_session.dest_ip = initiator.ip;
    fake_init_session.dest_encrypt_algo = initiator.encrypt_algo;
    fake_init_session.enabled = true;

    fake_affect_session.dest_ip = affector.ip;
    fake_affect_session.dest_encrypt_algo = affector.encrypt_algo;
    fake_affect_session.enabled = true;

    fake_initiator.ip = initiator.ip;
    fake_initiator.name = initiator.name;
    fake_initiator.encrypt_algo = initiator.encrypt_algo;
    fake_initiator.decrypt_algo = -1;
    fake_initiator.open_sessions.push_back(fake_init_session);

    fake_affector.ip = affector.ip;
    fake_affector.name = affector.name;
    fake_affector.encrypt_algo = affector.encrypt_algo;
    fake_affector.decrypt_algo = -1;
    fake_affector.open_sessions.push_back(fake_affect_session);

    drop_session(fake_initiator, affector);
    drop_session(fake_affector, initiator);

    fake_initiator.encrypt_algo = hacker.encrypt_algo;
    fake_initiator.decrypt_algo = hacker.decrypt_algo;
    fake_affector.encrypt_algo = hacker.encrypt_algo;
    fake_affector.encrypt_algo = hacker.decrypt_algo;

    start_session(fake_initiator, affector);
    start_session(fake_affector, initiator);

    return make_pair(fake_initiator, fake_affector);
}

void print_host(host host)
{

    printf("\n____________________[%s]____________________\n", host.name.c_str());
    printf("| [Name]    --> %s\n", host.name.c_str());
    printf("| [IP]      --> %s\n", host.ip.c_str());
    printf("| [Encrypt] --> %d\n", host.encrypt_algo);
    printf("| [Decrypt] --> %d\n", host.decrypt_algo);
    printf("| [Connections]\n");
    for (auto connection : host.open_sessions)
        printf(connection.enabled ? "|    [UP]   %s : %d\n" : "|    [DOWN] %s : %d\n",
               connection.dest_ip.c_str(),
               connection.dest_encrypt_algo);
    printf("\n");
}

int main()
{
    setlocale(0, "");
    srand(time(NULL));

    host_pair new_session, old_session;

    // host a(get_host()), b(get_host()), c(get_host());
    host a, b, c;
    a.name = "A";
    a.ip = "192.168.0.174";
    a.decrypt_algo = 1111;
    a.encrypt_algo = 0000;
    a.open_sessions = {};

    b.name = "B";
    b.ip = "192.168.0.111";
    b.decrypt_algo = 3333;
    b.encrypt_algo = 2222;
    b.open_sessions = {};

    print_host(a);
    print_host(b);

    new_session = start_session(a, b);
    a = new_session.first;
    b = new_session.second;
    print_host(a);
    print_host(b);

    old_session = drop_session(a, b);
    a = old_session.first;
    b = old_session.second;

    print_host(a);
    print_host(b);

    return 0;
}
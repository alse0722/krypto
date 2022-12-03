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

struct packet
{
    string source_ip;
    string destination_ip;
    int encrypted_with;
    string message;
    string status;
};

struct host
{
    string name;
    string ip;
    int encrypt_algo;
    int decrypt_algo;
    vector<session> open_sessions;
    vector<packet> sent_messages;
    vector<packet> received_messages;
};

struct hacker {
    string name;
    string real_ip;
    int encrypt_algo;
    int decrypt_algo;
    host fake_host_a;
    host fake_host_b;
    vector <packet> obtained_data;
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

    host.sent_messages = {};
    host.received_messages = {};

    return host;
}

hacker get_hacker(host a, host b) {
    hacker hacker;

    printf("\n_____________[Initialize hacker]_____________\n");

    printf("[Name]    --> ");
    cin >> hacker.name;
    printf("[IP]      --> ");
    cin >> hacker.real_ip;

    hacker.encrypt_algo = rand() % (INT16_MAX);
    hacker.decrypt_algo = rand() % (INT16_MAX);

    printf("[Encrypt] --> %d\n", hacker.encrypt_algo);
    printf("[Decrypt] --> %d\n", hacker.decrypt_algo);

    hacker.fake_host_a = a;
    hacker.fake_host_b = b;
    hacker.obtained_data = {};

    hacker.fake_host_a.decrypt_algo = -1;
    hacker.fake_host_a.open_sessions = {};
    hacker.fake_host_a.received_messages = {};
    hacker.fake_host_a.sent_messages = {};

    hacker.fake_host_b.decrypt_algo = -1;
    hacker.fake_host_b.open_sessions = {};
    hacker.fake_host_b.received_messages = {};
    hacker.fake_host_b.sent_messages = {};

    return hacker;
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
    for (auto s : initiator.open_sessions)
        if (s.dest_ip == initiator.ip)
            temp.push_back(s);

    initiator.open_sessions = temp;

    temp = {};
    for (auto s : affector.open_sessions)
        if (s.dest_ip == affector.ip)
            temp.push_back(s);

    affector.open_sessions = temp;

    printf("\n[info] Session [%s] <---> [%s] is DOWN!\n",
        initiator.ip.c_str(),
        affector.ip.c_str());

    initiator.open_sessions.push_back(new_initiator);
    affector.open_sessions.push_back(new_initiator);

    return make_pair(initiator, affector);
}

host_pair interrupt_session(host initiator, host affector, string new_ip, int new_algo) {

    host fake_host(initiator);

    drop_session(fake_host, affector);

    fake_host.ip = new_ip;
    fake_host.encrypt_algo = new_algo;

    start_session(fake_host, initiator);

    return make_pair(fake_host, affector);
}

bool check_session(host source, host destination) {
    bool src_up(false), dst_up(false);

    for (auto session : source.open_sessions)
        if (session.dest_ip == destination.ip)
            src_up = true;

    for (auto session : destination.open_sessions)
        if (session.dest_ip == source.ip)
            dst_up = true;

    return src_up && dst_up;
}

/*host_pair start_fake_session(host initiator, host affector, host hacker)
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
}*/

packet format_message(host source, host destinaton, string message) {
    packet pkg;

    pkg.source_ip = source.ip;
    pkg.destination_ip = destinaton.ip;
    pkg.encrypted_with = destinaton.encrypt_algo;
    pkg.message = message;

    return pkg;
}

host_pair send_message(host source, host destination, string message) {

    packet pkg(format_message(source, destination, message));

    if (check_session(source, destination)) {
        pkg.status = "SENT";
        source.sent_messages.push_back(pkg);
        pkg.status = "RECEIVED";
        destination.received_messages.push_back(pkg);
    }
    else {
        pkg.status = "SENDING FAILURE";
        source.sent_messages.push_back(pkg);
    }
    return make_pair(source, destination);
}

host decrypt_messages(host host) {
    vector <packet> processed;
    for (auto message : host.received_messages)
        if (message.encrypted_with == host.encrypt_algo) {
            message.status = "DECRYPTION SUCCESS";
            processed.push_back(message);
        }
        else {
            message.status = "DECRYPTION FAILURE";
            message.message = "*****************";
            processed.push_back(message);
        }

    host.received_messages = processed;

    return host;
}

void print_host(host host)
{

    printf("\n ______________________________________[%s]_______________________________________\n", host.name.c_str());
    printf("| [Name]    --> %s\n", host.name.c_str());
    printf("| [IP]      --> %s\n", host.ip.c_str());
    printf("| [Encrypt] --> %d\n", host.encrypt_algo);
    printf("| [Decrypt] --> %d\n", host.decrypt_algo);
    printf("| [Connections]\n");

    if (!host.open_sessions.empty())
        for (auto connection : host.open_sessions)
            printf(connection.enabled ? "|    [UP]   %s : %d\n" : "|    [DOWN] %s : %d\n",
                connection.dest_ip.c_str(),
                connection.dest_encrypt_algo);
    else
        printf("|    ! No connections detected\n");

    printf("| [Messages]\n");
    if (!host.sent_messages.empty())
        for (auto msg : host.sent_messages)
            printf("|    Sent {%s} to [%s:%d] --> %s\n",
                msg.message.c_str(),
                msg.destination_ip.c_str(),
                msg.encrypted_with,
                msg.status.c_str());
    else
        printf("|    ! Sent 0 messages\n");

    if (!host.received_messages.empty())
        for (auto msg : host.received_messages)
            printf("|    Received {%s} from [%s:%d] --> %s\n",
                msg.message.c_str(),
                msg.source_ip.c_str(),
                msg.encrypted_with,
                msg.status.c_str());
    else
        printf("|    ! Received 0 messages\n");

    printf("|______________________________________[%s]_______________________________________\n", host.name.c_str());
}

void print_hacker(hacker hacker) {
    printf("\n ======================================[%s]======================================\n", hacker.name.c_str());
    printf("| [Name]    --> %s\n", hacker.name.c_str());
    printf("| [IP]      --> %s\n", hacker.real_ip.c_str());
    printf("| [Encrypt] --> %d\n", hacker.encrypt_algo);
    printf("| [Decrypt] --> %d\n", hacker.decrypt_algo);
    printf("| [Fake hosts]\n");

    print_host(hacker.fake_host_a);
    print_host(hacker.fake_host_b);

    printf("| [Obtained data]\n");
    if (!hacker.obtained_data.empty())
        for (auto msg : hacker.obtained_data)
            printf("|    Received {%s} from [%s:%d] --> %s\n",
                msg.message.c_str(),
                msg.source_ip.c_str(),
                msg.encrypted_with,
                msg.status.c_str());
    else
        printf("|    ! Received 0 messages\n");

    printf("\n|======================================[%s]======================================\n", hacker.name.c_str());
}

int main()
{
    setlocale(0, "");
    srand(time(NULL));

    host_pair new_session, old_session;
    string str1("test1"), str2("test2");

    // host a(get_host()), b(get_host()), c(get_host());
    host a, b, c;
    a.name = "A";
    a.ip = "192.168.0.174";
    a.decrypt_algo = 1111;
    a.encrypt_algo = 9999;
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

    new_session = send_message(a, b, str1);
    a = new_session.first;
    b = decrypt_messages(new_session.second);
    print_host(a);
    print_host(b);

    //old_session = drop_session(a, b);
    //a = old_session.first;
    //b = old_session.second;
    //print_host(a);
    //print_host(b);

    return 0;
}
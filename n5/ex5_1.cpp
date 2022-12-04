#include <iostream>
#include <string.h>
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

struct hacker
{
    string name;
    string real_ip;
    int encrypt_algo;
    int decrypt_algo;
    host fake_host_a;
    host fake_host_b;
    vector<packet> obtained_data;
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

hacker get_hacker(host a, host b)
{
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
    session dropped;

    temp = {};
    for (auto e : initiator.open_sessions)
        if (e.dest_ip == affector.ip)
        {
            dropped = e;
            dropped.enabled = false;
            temp.push_back(dropped);
        }
        else
            temp.push_back(e);

    initiator.open_sessions = temp;

    temp = {};
    for (auto e : affector.open_sessions)
        if (e.dest_ip == initiator.ip)
        {
            dropped = e;
            dropped.enabled = false;
            temp.push_back(dropped);
        }
        else
            temp.push_back(e);

    affector.open_sessions = temp;

    printf("\n[info] Session [%s] <---> [%s] is DOWN!\n",
           initiator.ip.c_str(),
           affector.ip.c_str());

    return make_pair(initiator, affector);
}

host_pair interrupt_session(host initiator, host affector, hacker hacker)
{

    host fake_host(initiator);
    host_pair temp;

    temp = drop_session(fake_host, affector);
    fake_host = temp.first;
    affector = temp.second;

    fake_host.ip = hacker.real_ip;
    fake_host.encrypt_algo = hacker.encrypt_algo;
    fake_host.decrypt_algo = hacker.decrypt_algo;

    temp = start_session(fake_host, affector);
    fake_host = temp.first;
    affector = temp.second;

    return make_pair(fake_host, affector);
}

bool check_session(host source, host destination)
{
    bool src_up(false), dst_up(false);

    for (auto session : source.open_sessions)
        if (session.dest_ip == destination.ip)
            src_up = true;

    for (auto session : destination.open_sessions)
        if (session.dest_ip == source.ip)
            dst_up = true;

    return src_up && dst_up;
}

packet format_message(host source, host destinaton)
{
    packet pkg;

    char alpha[73] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789!@#$%^&*<>";
    char msg[10] = {};
    int ind;
    string message;

    for (int i = 0; i < 10; i++)
    {
        ind = rand() % 72;
        msg[i] = alpha[ind];
    }

    message = string(msg, 10);

    pkg.source_ip = source.ip;
    pkg.destination_ip = destinaton.ip;
    pkg.encrypted_with = destinaton.encrypt_algo;
    pkg.message = message;

    return pkg;
}

host_pair send_message(host source, host destination)
{

    packet pkg(format_message(source, destination));

    if (check_session(source, destination))
    {
        pkg.status = "SENT";
        source.sent_messages.push_back(pkg);
        pkg.status = "RECEIVED";
        destination.received_messages.push_back(pkg);
    }
    else
    {
        pkg.status = "SENDING FAILURE";
        source.sent_messages.push_back(pkg);
    }
    return make_pair(source, destination);
}

host_pair reply_last_message(host source, host fake_source, host destination)
{
    packet msg = source.received_messages.back();
    packet fake(format_message(fake_source, destination));

    fake.message = msg.message;
    fake.source_ip = msg.source_ip;

    if (check_session(fake_source, destination))
    {
        fake.status = "RECEIVED";
        destination.received_messages.push_back(fake);
    }
    else
    {
        fake.status = "SENDING FAILURE";
        fake_source.sent_messages.push_back(fake);
    }

    return make_pair(fake_source, destination);
}

host decrypt_messages(host host)
{
    vector<packet> processed;
    for (auto message : host.received_messages)
        if (message.encrypted_with == host.encrypt_algo)
        {
            message.status = "DECRYPTION SUCCESS";
            processed.push_back(message);
        }
        else
        {
            message.status = "DECRYPTION FAILURE";
            message.message = "*****************";
            processed.push_back(message);
        }

    host.received_messages = processed;

    return host;
}

hacker collect_data(hacker hacker)
{
    for (auto e : hacker.fake_host_a.received_messages)
        hacker.obtained_data.push_back(e);

    for (auto e : hacker.fake_host_b.received_messages)
        hacker.obtained_data.push_back(e);

    return hacker;
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

void print_hacker(hacker hacker)
{
    printf("\n ======================================[%s]=======================================\n", hacker.name.c_str());
    printf("| [Name]    --> %s\n", hacker.name.c_str());
    printf("| [IP]      --> %s\n", hacker.real_ip.c_str());
    printf("| [Encrypt] --> %d\n", hacker.encrypt_algo);
    printf("| [Decrypt] --> %d\n", hacker.decrypt_algo);

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

    // printf("| [Fake hosts]\n");
    //
    // print_host(hacker.fake_host_a);
    // print_host(hacker.fake_host_b);

    printf("\n|======================================[%s]=======================================\n", hacker.name.c_str());
}

void normal_mode()
{
    host a, b;
    host_pair temp;

    // system("color 20");
    printf("\n[sys] Mode: Normal\n");
    printf("\n[sys] Initialize hosts");
    // system("color 70");

    a = get_host();
    b = get_host();

    // system("color 20");
    printf("\n[sys] Start session between hosts %s and %s\n", a.name.c_str(), b.name.c_str());
    // system("color 70");

    temp = start_session(a, b);
    a = temp.first;
    b = temp.second;

    print_host(a);
    print_host(b);

    // system("color 20");
    printf("\n[sys] Message excanges between hosts %s and %s\n", a.name.c_str(), b.name.c_str());
    // system("color 70");

    temp = send_message(a, b);
    a = temp.first;
    b = decrypt_messages(temp.second);

    temp = send_message(b, a);
    b = temp.first;
    a = decrypt_messages(temp.second);

    print_host(a);
    print_host(b);

    // system("color 20");
    printf("\n[sys] Close session between hosts %s and %s\n", a.name.c_str(), b.name.c_str());
    // system("color 70");

    temp = drop_session(a, b);
    a = temp.first;
    b = temp.second;

    print_host(a);
    print_host(b);

    // system("color 20");
    printf("\n[sys] END\n");
    // system("color 70");
}

void hacked_session_mode()
{
    host a, b;
    hacker h;
    host_pair temp;
    pair<hacker, host> reply;

    // system("color 20");
    printf("\n[sys] Mode: Hacked session\n");
    printf("\n[sys] Initialize hosts");
    // system("color 70");

    a = get_host();
    b = get_host();

    // system("color 20");
    printf("\n[sys] Start session between hosts %s and %s\n", a.name.c_str(), b.name.c_str());
    // system("color 70");

    temp = start_session(a, b);
    a = temp.first;
    b = temp.second;

    // print_host(a);
    // print_host(b);

    // system("color 20");
    printf("\n[sys] Initialize hacker");
    // system("color 70");

    h = get_hacker(a, b);
    print_hacker(h);

    // system("color 20");
    printf("\n[sys] Hacker interrupts the session");
    // system("color 70");

    temp = interrupt_session(h.fake_host_b, a, h);
    h.fake_host_b = temp.first;
    a = temp.second;

    temp = interrupt_session(h.fake_host_a, b, h);
    h.fake_host_a = temp.first;
    b = temp.second;

    print_host(a);
    print_host(b);
    print_hacker(h);

    // system("color 20");
    printf("\n[sys] Message excanges between hosts %s and %s\n", a.name.c_str(), b.name.c_str());
    // system("color 70");

    temp = send_message(a, h.fake_host_b);
    a = temp.first;
    h.fake_host_b = decrypt_messages(temp.second);

    temp = reply_last_message(h.fake_host_b, h.fake_host_a, b);
    h.fake_host_a = temp.first;
    b = decrypt_messages(temp.second);

    temp = send_message(b, h.fake_host_a);
    b = temp.first;
    h.fake_host_a = decrypt_messages(temp.second);

    temp = reply_last_message(h.fake_host_a, h.fake_host_b, a);
    h.fake_host_b = temp.first;
    a = decrypt_messages(temp.second);

    h = collect_data(h);

    // print_host(a);
    // print_host(b);
    // print_hacker(h);

    // system("color 20");
    printf("\n[sys] Close all sessions \n");
    // system("color 70");

    temp = drop_session(h.fake_host_a, b);
    h.fake_host_a = temp.first;
    b = temp.second;

    temp = drop_session(h.fake_host_b, a);
    h.fake_host_b = temp.first;
    a = temp.second;

    print_host(a);
    print_host(b);
    print_hacker(h);
}

int main()
{
    setlocale(0, "");
    srand(time(NULL));

    // normal_mode();
    hacked_session_mode();

    return 0;
}
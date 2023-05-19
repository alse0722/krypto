#include <iostream>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <limits>

using namespace std;

typedef unsigned long long lli;

struct host
{
    lli generator;
    lli module;
    lli hidden_key;
    lli part_key;
    lli fin_key;
    bool initiator;
};

// lli max_64_int_prime = 9223372036854775783;
lli max_64_int_prime = 9859;
vector<host> hosts;

int validated_input()
{
    int s = 0;
    while (!(cin >> s))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printf("! Неверный ввод. Повторите ввод, начиная с первого неверного элемента.\n");
    }

    return s;
}

lli rand_between(lli from, lli to)
{
    if (to == from)
        return to;
    if (to < from)
        return rand_between(to, from);
    return from + rand() % (to - from + 1);
}

lli modexp(lli x, lli y, lli N)
{
    if (y == 0)
        return 1;

    lli z = modexp(x, y / 2, N);

    if (y % 2 == 0)
        return (z * z) % N;
    else
        return (x * z * z) % N;
}

void get_hosts(vector<host> hosts)
{
    for (auto i = 0; i < hosts.size(); i++)
    {
        printf("\n-------------------[host %3d]-------------------", i);
        printf("\n[generator]   %lli", hosts[i].generator);
        printf("\n[module]      %lli", hosts[i].module);
        printf("\n[hidden_key]  %lli", hosts[i].hidden_key);
        printf("\n[part_key]    %lli", hosts[i].part_key);
        printf("\n[fin_key]     %lli", hosts[i].fin_key);
        printf(hosts[i].initiator ? "\n[initiator]   true" : "\n[initiator]   false");
        printf("\n-------------------[host %3d]-------------------\n", i);
    }
}

void get_nth_host(host host, int i)
{

    printf("\n-------------------[host %3d]-------------------", i);
    printf("\n[generator]   %lli", host.generator);
    printf("\n[module]      %lli", host.module);
    printf("\n[hidden_key]  %lli", host.hidden_key);
    printf("\n[part_key]    %lli", host.part_key);
    printf("\n[fin_key]     %lli", host.fin_key);
    printf(host.initiator ? "\n[initiator]   true" : "\n[initiator]   false");
    printf("\n-------------------[host %3d]-------------------\n", i);
}

void diff_hell_initiate(int init)
{
    printf("\n-------------------[Инициация]------------------\n");

    hosts[init].initiator = true;
    // hosts[init].part_key = modexp(hosts[init].generator,
    //                               hosts[init].hidden_key,
    //                               hosts[init].module);
    //
    // get_nth_host(hosts[init], init);

    for (int to = 0; to < hosts.size(); to++){
            hosts[to].generator = hosts[init].generator;
            hosts[to].module    = hosts[init].module;
            hosts[to].part_key  = modexp(   hosts[to].generator,
                                            hosts[to].hidden_key,
                                            hosts[to].module);
    }

    get_hosts(hosts);

    printf("\n-------------------[Инициация]------------------\n");
    
    cin.get();
}

void diff_hell_for_two(const int src, const int dst)
{

    hosts[src].part_key = modexp(hosts[src].part_key,
                                 hosts[dst].hidden_key,
                                 hosts[src].module);
}

void diff_hell_for_all(int init)
{

    diff_hell_initiate(init);

    for (int from = 0; from < hosts.size(); from++)
        for (int to = 0; to < hosts.size(); to++)
        {
            if (to != from)
                diff_hell_for_two(from, to);

            // get_nth_host(hosts[from], from);
            // if (hosts[from].initiator)
            //     get_nth_host(hosts[from], from);
        }

    for (int from = 0; from < hosts.size(); from++)
        hosts[from].fin_key = hosts[from].part_key;
}

int main()
{
    int n, start;
    srand(time(NULL));

    printf("\nВведите количество пользователей:\n   ");
    n = validated_input();

    for (int i = 0; i < n; i++)
    {
        host new_host;

        new_host.module = max_64_int_prime;
        new_host.generator = rand_between(0, INT64_MAX);
        new_host.hidden_key = rand_between(0, max_64_int_prime);
        new_host.part_key = 0;
        new_host.fin_key = 0;
        new_host.initiator = false;

        hosts.push_back(new_host);
    }

    get_hosts(hosts);

    printf("\nВведите id инициатора обмена ключами:\n   ");
    start = validated_input();

    diff_hell_for_all(start);

    printf("\nРезультат работы\n");
    get_hosts(hosts);

    return 0;
}
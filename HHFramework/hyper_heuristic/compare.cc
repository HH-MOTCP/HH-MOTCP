/*
 * =====================================================================================
 *
 *       Filename:  compare.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27/04/15 13:32:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <fstream>

#define S 15000

using namespace std;

int main()
{
    string upgroup   = "./tcp.out";
    string downgroup = "./tcs.out";

    const char *up_file = upgroup.c_str();
    const char *do_file = downgroup.c_str();

    ifstream up_in, do_in;
    up_in.open(up_file);
    do_in.open(do_file);

    int mark_up[S];
    int mark_do[S];
    int mark_union[S];
    int mark_inter[S];

    for (int i = 0; i < S; i++)
    {
        mark_union[i] = 0;
        mark_do[i] = 0;
        mark_up[i] = 0;
    }

    int temp;
    while(up_in >> temp)
    {
        mark_up[temp] = 1;
        mark_union[temp] = 1;
    }

    while(do_in >> temp)
    {
        mark_do[temp] = 1;
        mark_union[temp] = 1;
    }

    int upper = 0, down = 0;
    for (int i = 0; i < S; i++)
    {
        if (mark_up[i] == 1) upper++;
        if (mark_do[i] == 1) down++;
    }

    int interaction = 0;
    int uniont = 0;
        
    cout << "union" << endl;
    for (int i = 0; i < S; i++)
    {
        if (mark_union[i] == 1)
        {
            uniont += 1;
            cout << i << " ";
        }
    }
    cout << endl;
    
    cout << "interaction" << endl;
    for (int i = 0; i < S; i++)
    {
        if (mark_up[i] == 1 && mark_do[i] == 1)
        {
            mark_inter[i] = 1;
            interaction += 1;
            cout << i << " ";
        }
    }
    cout << endl;

    cout << "up special" << endl;
    for (int i = 0; i < S; i++)
    {
        if (mark_up[i] == 1 && mark_inter[i] != 1)
        {
            cout << i << " ";
        }
    }
    cout << endl;

    cout << "down special" << endl;
    for (int i = 0; i < S; i++)
    {
        if (mark_do[i] == 1 && mark_inter[i] != 1)
        {
            cout << i << " ";
        }
    }
    cout << endl;

    cout << endl;
    cout << "upper: " << upper << "  down: " << down << endl;
    cout << endl;
    cout << "interaction: " << interaction << endl;
    cout << "union:       " << uniont << endl;

    return 1;
}

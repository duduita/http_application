#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <bits/stdc++.h> // remember to remove it!
#include "urlParser.cpp"

using namespace std;
int main(int argc, char *argv[])
{
    vector<string> parsedURL = urlParser(argv[1]);
    string host_name = parsedURL[1];
    struct hostent *he;

    if (argc == 1)
        return -1;

    he = gethostbyname(&host_name[0]);
    if (he == NULL)
    {
        switch (h_errno)
        {
        case HOST_NOT_FOUND:
            fputs("The host was not found.\n", stderr);
            break;
        case NO_ADDRESS:
            fputs("The name is valid but it has no address.\n", stderr);
            break;
        case NO_RECOVERY:
            fputs("A non-recoverable name server error occurred.\n", stderr);
            break;
        case TRY_AGAIN:
            fputs("The name server is temporarily unavailable.", stderr);
            break;
        }
    }
    else
    {
        puts(inet_ntoa(*((struct in_addr *)he->h_addr_list[0])));
    }

    return he != NULL;
}

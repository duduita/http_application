#include <bits/stdc++.h>

using namespace std;

int getRequestSource(string response)
{
    string header = response.substr(0, response.find("\r\n\r\n"));

    if (response.find("200 OK") != -1)
        return 200;

    if (response.find("400 Bad Request") != -1)
        return 400;

    if (response.find("404 Not Found") != -1)
        return 404;

    return -1;
}

// int main()
// {
//     char str[] = "wireshark-labs/HTTP-wireshark-file1.html";
//     string test = getFilename(str);
//     return 0;
// }
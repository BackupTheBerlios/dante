#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <socket++/sockunix.h>

#include "msgsrv.H"

int main(int argc, char ** argv)
{
    sockunixbuf * l_connectSocket = new sockunixbuf(sockbuf::sock_stream);
    l_connectSocket->connect(MSGSRV_ADDR);
    iosockstream l_stream(l_connectSocket);
    uint32_t l_command = MSGSRV_CMD_NEWCON;
    l_stream << l_command << endl;;
    uint32_t l_connection;
    l_stream >> l_connection;

    delete l_connectSocket;
    l_connectSocket = new sockunixbuf(sockbuf::sock_stream);
    l_connectSocket->connect(MSGSRV_ADDR);
    l_command = MSGSRV_CMD_CLOSE;
    iosockstream l_stream2(l_connectSocket);
    l_stream2 << l_command << endl;
    l_stream2 << l_connection << endl;

    return 0;
}

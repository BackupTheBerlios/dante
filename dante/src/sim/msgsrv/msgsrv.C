#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <socket++/sockunix.h>
#include <string>
#include <queue>
#include <map>

#include "msgsrv.H"

using namespace std;

struct message
{
    uint32_t cv_seq;
    uint32_t cv_cmd;
    uint8_t  cv_data[256];
};

struct message_connection
{
    message_connection() {};
    
    uint32_t cv_id;
    char cv_bind[256];
    queue<message> cv_queue;
    sockbuf * cv_wait;
};  

int main(int argc, char ** argv)
{
    uint32_t l_msgseq = 0, l_conseq = 0;
    map<uint32_t, message_connection *> l_id2con;
    map<char[256], message_connection *> l_bin2con;

    cout << "Dante Message Server..." << endl;
    
    sockunixbuf l_connectSocket(sockbuf::sock_stream);
    unlink(MSGSRV_ADDR);
    l_connectSocket.bind(MSGSRV_ADDR);
    l_connectSocket.listen(128);

    while(1)
    {
	sockbuf l_thisCon(l_connectSocket.accept());
	iosockstream l_thisStream(&l_thisCon);

	uint32_t l_command;
	l_thisStream >> l_command;

	cout << "Received command:" << l_command << endl;
	switch (l_command)
	{
	    case MSGSRV_CMD_NEWCON:
		cout << "New connection." << endl;
		l_conseq++;
		l_id2con[l_conseq] = new message_connection();
		l_id2con[l_conseq]->cv_id = l_conseq;
		l_thisStream << l_conseq;
		break;
	    
	    case MSGSRV_CMD_CLOSE:
		cout << "Close connection." << endl;
		{
		    uint32_t l_tmp;
		    l_thisStream >> l_tmp;
		    if (NULL != l_id2con[l_tmp])
			delete l_id2con[l_tmp];
		}
		break;		
		
	    case MSGSRV_CMD_INVAL:
	    default:
		cout << "Bad command." << endl;
		break;
	}
    }
    
    return 0;
}

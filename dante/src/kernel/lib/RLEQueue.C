#include <lib/RLEQueue.h>
#include <stdint.h>

void RLEQueue::insert(uint32_t i_value, uint32_t i_length)
{
    if (NULL == cv_head)
    {
	cv_head = new_node(i_value, i_length);
    }
    else
    {
	__node * l_node = new_node(i_value, i_length);
	if (cv_head->cv_data.first() > i_value)
	{
	    if ((i_value + i_length) == (cv_head->cv_data.first()))
	    {
		cv_head->cv_data.first() = i_value;
		cv_head->cv_data.second() += i_length;
		delete l_node;
	    }
	    else
	    {
		l_node->cv_next = cv_head;
		cv_head = l_node;
	    }
	}
	__node * l_temp = cv_head;
	bool l_done = false;
	while((l_temp->cv_next != NULL) && !l_done)
	{
	    if (l_temp->cv_next->cv_data.first() > i_value)
	    {
		if ((l_temp->cv_data.first() + l_temp->cv_data.second()) ==
			(i_value))
		{
		    if ((i_value + i_length) == 
			    (l_temp->cv_next->cv_data.first()))
		    {
			l_temp->cv_data.second() += 
			    l_temp->cv_next->cv_data.second() + i_length;
			delete l_node;
			l_node = l_temp->cv_next;
			l_temp->cv_next = l_node->cv_next;
			delete l_temp;
			l_done = true;
		    }
		    else
		    {
			l_temp->cv_data.second() += i_length;
			delete l_node;
			l_done = true;
		    }
		}
		else if((i_value + i_length) == 
			l_temp->cv_next->cv_data.first())
		{
		    l_temp->cv_next->cv_data.first() = i_value;
		    l_temp->cv_next->cv_data.second() += i_length;
		    delete l_node;
		    l_done = true;
		}
		else
		{
		    l_node->cv_next = l_temp->cv_next;
		    l_temp->cv_next = l_node;
		    l_done = true;
		}
	    }
	    else
	    {
		l_temp = l_temp->cv_next;
	    }
	}; // done while.

	if (!l_done)
	{
	    l_temp->cv_next = l_node;
	}
		
    }
}

uint32_t RLEQueue::pop(uint32_t i_length)
{
    return 0;
}

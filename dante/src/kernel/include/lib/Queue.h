#ifndef __LIB_QUEUE_H
#define __LIB_QUEUE_H

#include <stdint.h>

template <typename _A>
class Queue
{
    public:
	Queue() : cv_head((__node *) NULL), cv_size(0) {};

	void push(_A i_value)
	    {
		cv_size++;
		
		if (NULL == cv_head)
		{
		    cv_head = cv_tail = new_node(i_value);
		    return;
		}
		
		cv_head->cv_next = new_node(i_value);
		cv_head = cv_head->cv_next;
    		
	    };

	_A pop(void)
	    {
		if (NULL == cv_head)
		    return _A();

		_A l_rval;

		if (cv_tail == cv_head)
		{
		    l_rval = cv_head->cv_data;
		    delete cv_head;
		    cv_tail = cv_head = (__node *) NULL;
		}
		else
		{
		    l_rval = cv_tail->cv_data;
		    __node * cv_tmp = cv_tail;
		    cv_tail = cv_tail->cv_next;
		    delete cv_tmp;
		}
		
		cv_size--;
		return l_rval;
	    };

	uint32_t size() { return cv_size; };
	
	_A operator[](uint32_t i_pos)
	{
	    if (i_pos >= cv_size)
		return _A();

	    __node * l_temp = cv_head;
	    while((l_temp != NULL) && (0 != i_pos))
	    {
		i_pos--; l_temp = l_temp->cv_next;
	    }
	    if (l_temp == NULL)
		return _A();
	    return l_temp->cv_data;
	};
	
    private:
	class __node
	{
	    public:
		_A cv_data;
		__node * cv_next;
	};

	__node * cv_head;
	__node * cv_tail;
	uint32_t cv_size;

	__node * new_node(_A i_value)
	{
	    __node * l_node = new __node();
	    l_node->cv_next = (__node *) NULL;
	    l_node->cv_data = i_value;

	    return l_node;
	};

};

#endif


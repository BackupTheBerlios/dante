#ifndef __LIB_RLEQUEUE_H
#define __LIB_RLEQUEUE_H

#include <lib/Pair.h>
#include <stdint.h>

class RLEQueue
{
    public:
	RLEQueue() : cv_head((__node *) NULL) {};
	
	// insert/pop multiple (RLE).
	void insert(uint32_t i_value , uint32_t i_length = 1);
	uint32_t pop(uint32_t i_length = 1);	
	
    private:
	class __node
	{
	    public:
		Pair<uint32_t, uint32_t> cv_data;
		__node * cv_next;
	};

	__node * cv_head;

	__node * new_node(uint32_t i_val, uint32_t i_len)
		{
		    __node * l_node = new __node();
		    l_node->cv_data = Pair<uint32_t, uint32_t>(i_val, i_len);
		    l_node->cv_next = (__node *) NULL;

		    return l_node;
		};
};

#endif

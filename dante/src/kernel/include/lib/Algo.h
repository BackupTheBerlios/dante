#ifndef __LIB_ALGO_H
#define __LIB_ALGO_H

template <uint32_t __size>
uint32_t align(uint32_t i_value)
    {
	if ((i_value % __size) == 0)
	    return i_value;
	return i_value + (__size - (i_value % __size));
    };

#endif

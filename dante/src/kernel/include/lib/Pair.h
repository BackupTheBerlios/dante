#ifndef __LIB_PAIR_H
#define __LIB_PAIR_H

template <typename _A, typename _B>
class Pair
{
    private:
	_A cv_first;
	_B cv_second;
	
    public:
	Pair() : cv_first(), cv_second() { };
	Pair(_A i_a, _B i_b) : cv_first(i_a), cv_second(i_b) { };

	_A & first() { return cv_first; };
	const _A & first() const { return cv_first; };

	_B & second() { return cv_second; };
	const _B & second() const { return cv_second; };
};

#include <display/textStream.h>
template <typename _A>
bool pair_overlap(const Pair<_A,_A> i_first, const Pair<_A,_A> i_second)
{
    if ((i_first.first() >= i_second.second()) || 
       (i_first.second() <= i_second.first()))
	   return false;
    return true;
};

#endif

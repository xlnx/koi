#pragma once

#include <iostream>

#ifdef DEBUG
#define LOG_CHECKPOINT()                                        \
	(::std::cout << "[ " << __FILE__ << ":" << __LINE__ << " ]" \
				 << "\n"                                        \
				 << "    " << __PRETTY_FUNCTION__ << ::std::endl )
#else
#define LOG_CHECKPOINT()
#endif

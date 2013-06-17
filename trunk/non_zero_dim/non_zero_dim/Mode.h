#ifndef MODE_H
#define MODE_H

#include <string>

enum Mode {
	DEFAULT = 1,
	MAX,
	KMAX,
	MOSTPOPULAR,
	LEASTPOPULAR
};

std::string modeName(Mode mode);

extern Mode mode;

#endif
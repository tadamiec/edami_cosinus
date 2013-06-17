#include "Mode.h"

std::string modeName(Mode mode){
	switch(mode){
	case DEFAULT:
		return "default order";
	case MAX:
		return "top values";
	case KMAX:
		return "k top values";
	case MOSTPOPULAR:
		return "most popular dimensions";
	case LEASTPOPULAR:
		return "least popular dimensions";
	}
	return "";
}
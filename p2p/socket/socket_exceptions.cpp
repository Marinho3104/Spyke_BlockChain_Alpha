

#include "socket_exceptions.h"

#include <iostream>


const char* p2p::Unknow_Connection_Version_Exception::what() const throw() { return "Unknow Connection Version"; }

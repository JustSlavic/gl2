#pragma once

namespace service {


int string_compare(const char* lhs, const char* rhs);


} // service

#ifdef UNITY_BUILD
#include "string.cpp"
#endif
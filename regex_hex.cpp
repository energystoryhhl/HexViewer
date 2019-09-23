#include "regex_hex.h"

std::regex HexLinePattern("^:([\\d\\w]{2})([\\d\\w]{4})([\\d\\w]{2})([\\d\\w]{2}+)");
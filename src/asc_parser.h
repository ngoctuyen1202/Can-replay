#ifndef ASC_PARSER_H
#define ASC_PARSER_H

#include <string>
#include "can_frame.h"

class AscParser {
public:
    bool parseLine(const std::string& line, CanFrameData& f);
};

#endif // ASC_PARSER_H

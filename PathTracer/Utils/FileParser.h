//
//  FileParser.h
//  PathTracer
//
//  Created by HYZ on 2018/3/30.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#ifndef FileParser_h
#define FileParser_h

#include <iostream>
#include <fstream>
#include <vector>

class FileParser {
    std::fstream f;
public:
    FileParser(std::string fileName);
    ~FileParser();
    
    std::vector<std::string> split(std::string str, char s);
    void readAndSchedule();
};

#endif /* FileParser_h */

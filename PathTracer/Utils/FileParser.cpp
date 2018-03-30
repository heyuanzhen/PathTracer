//
//  FileParser.cpp
//  PathTracer
//
//  Created by HYZ on 2018/3/30.
//  Copyright © 2018年 HYZ. All rights reserved.
//

#include "FileParser.h"
#include "Examples.h"

using namespace std;

FileParser::FileParser(string fileName) {
    f.open(fileName.c_str(), ios::in);
    if (!f.is_open()) {
        cout<<"Cannot open config file !"<<endl;
        exit(1);
    }
}

FileParser::~FileParser() {
    f.close();
}

vector<string> FileParser::split(string str, char s) {
    vector<string> words;
    string word = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == s && word != ""){
            words.push_back(word);
            word = "";
        }
        else {
            word += str[i];
            if (i == str.size() - 1) {
                words.push_back(word);
            }
        }
    }
    return words;
}

void FileParser::readAndSchedule() {
    int sceneNo = 0;
    int reso[2] = {0, 0};
    int sampleCount = 0;
    int maxDepth = 0;
    
    string line;
    while (getline(f, line)) {
        vector<string> words = split(line, ' ');
        if (words[0] == "Scene" || words[0] == "scene") {
            sceneNo = atoi(words[1].data());
        }
        if (words[0] == "Resolution" || words[0] == "resolution") {
            reso[0] = atoi(words[1].data());
            reso[1] = atoi(words[2].data());
        }
        if (words[0] == "SampleCount" || words[0] == "sampleCount") {
            sampleCount = atoi(words[1].data());
        }
        if (words[0] == "MaxDepth" || words[0] == "maxDepth") {
            maxDepth = atoi(words[1].data());
        }
    }
    
    if (!(sceneNo && reso[0] && reso[1] && sampleCount && maxDepth)) {
        cout<<"Error in config file !"<<endl;
        exit(1);
    }
    
    switch (sceneNo) {
        case 1:
            cbox(reso, sampleCount, maxDepth);
            break;
        case 2:
            veach(reso, sampleCount, maxDepth);
            break;
        case 3:
            objTest(reso, sampleCount, maxDepth);
        default:
            break;
    }
}


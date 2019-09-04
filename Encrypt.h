#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <string>
#include <vector>

using namespace std;

vector<unsigned char> DoEncrypt(string type, vector<unsigned char> &hexHeader,const vector<unsigned char> &data);

#endif //ENCRYPT_H
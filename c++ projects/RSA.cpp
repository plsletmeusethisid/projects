#include <cmath>
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void Encrypt(string file, string msg, int e, unsigned long int n);
void Decrypt(string inputfile, string outfile, int d, int n);
int EuclideanAlg(int p, int q, int e, int L);
vector<int> getBinary(int digit);
unsigned long int modExpo(unsigned long int m, vector<int> e, int n);
unsigned long int stringToIntConversion(string input);
unsigned long int leastCommonMultiple(int p, int q);
int gcd(int p, int q);

int main(int argc, char* argv[]) {
    int e = 65537;
    if (argc == 1) {
        cout << "No p and q" << endl;
        return 0;
    } 
    string p = argv[1];
    string q = argv[2];
    
    
    string msg;
    string file;
    string input;
    string output;
    string command;
    string stringN;
    unsigned long int n;
    while (!0) {        //style points right here
        cin >> command;
        if (command.compare ("ENCRYPT") == 0) {
            cin >> file;
            cin >> stringN;
            n = stringToIntConversion(stringN);
            Encrypt(file, msg, e, n);
        } else if (command.compare ("DECRYPT") == 0) {
            cin >> input;
            cin >> output;
            n = stringToIntConversion(q) * stringToIntConversion(p);
            int d = EuclideanAlg(stringToIntConversion(p),stringToIntConversion(q), e, leastCommonMultiple(stringToIntConversion(p), stringToIntConversion(q)));
            Decrypt(input, output, d, n);
        } else {
            return 0;
        }
    }
    return 0;
}
void Encrypt(string file, string msg, int e, unsigned long int n) {
    int x = 1 + log(n/27)/log(100);
    unsigned long int arr [msg.length()];
    for (int i = 0; i < msg.length(); ++i) {
        if (msg[i] == ' ') {
            arr[i] = 0;
            continue;
        }
        arr[i] = (int)msg[i] - 96;
    }
    vector<int> binary = getBinary(e);
    unsigned long int y = 0;
    ofstream ofile(file);
    int i = 0;
    for (i = 0; i < msg.length(); ++i) {
        if (i % x != 0 && i != 0) {
            y += arr[i] * pow(10, 2*(x-i-1));
            if (arr[i] == 0) {
                y *= 100;
            }
        } else if (i % x == 0 && i != 0) {
            ofile << modExpo(y, binary, n) << " ";             
        }
    }
    if (i % x != 0) {
        for (int j = 0; j < i % x; ++j) {
            ofile << "00";
        }
        cout << " ";
    }
    ofile.close();
}
unsigned long int stringToIntConversion(string input) {
    unsigned long int returnVal = 0;
    for (int i = 0; i < input.length(); ++i) {
        returnVal += (int)input[i]-48;
        if (i + 1 == input.length()) {
            break;
        } else {
            returnVal *= 10;
        }
    }
    return returnVal;
}

void Decrypt(string inputfile, string outfile, int d, int n) {
    unsigned long int c;
    ifstream input (inputfile);
    input >> c;
    vector<int> binary = getBinary(d);
    c = modExpo(c, binary, n);
    int temp;
    int numDigit = log10(c) + 1;
    string msg = "";
    ofstream output (outfile);
    while (c != 0) {
        temp = c / pow(10, (numDigit - 2));
        if(temp == 0) {
            output << ' ';
        } else {
            output << (char)(temp + 96);
        }
        c = c % 100;
    }
    output << c << endl;
    input.close();
    output.close();
}

int gcd(int p, int q) {
    if (q == 0) {
        return p;
    } else {
        return gcd(q, p % q);
    }
}


unsigned long int modExpo(unsigned long int m, vector<int> e, int n){
    unsigned long int x = 1;
    int power = m % n;
    for (int i = 0; i < e.size(); ++i) {
        if (e[i] == 1) {
            x = (x * power) % n;
            power = power * power % n; //might need to change
        }
    }
    return x;
}
unsigned long int leastCommonMultiple(int p, int q) {
    unsigned long int leastCommonMultiple = ((p-1) * (q -1));
    leastCommonMultiple /= gcd(p-1, q-1);
    return leastCommonMultiple;
}
vector<int> getBinary(int digit) {
    vector<int> binary;
    while (digit > 0) {
        binary.push_back(digit%2);
        digit/=2;
    }
    return binary;
}

int EuclideanAlg(int p, int q, int e, int L) {
    int s = 0, old_s = 1, t = 1, old_t = 1, r = e, old_r = L;
    int quotient = old_r/r, temp = r;
    while (r != 0) {
        r = old_r - quotient * r;
        old_r = temp;
        temp = s;
        s = old_s - quotient * s;
        old_s = temp;
        temp = t;
        t = old_t - quotient * t;
        old_t = temp;
    }
    return old_t;
    
    
    
    // if (q == e || q < e) {
    //     throw invalid_argument ("p and/or q values are too small");
    // }
}

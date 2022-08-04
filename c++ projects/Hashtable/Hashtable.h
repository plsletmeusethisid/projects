#include <iostream>
#include <utility>
#include <string>
#include <cmath>
#include <fstream>
#include <random>
#include <cstring>
#include <vector>

template<class T>
class Hashtable {
    public:
        Hashtable(bool debug = false, unsigned int size = 11); 
        ~Hashtable();
        int add (std::string k, const T& val);
        const T& lookup (std::string k);
        void reportAll (std::ostream & ofile) const;
        
    private:    
        void resize();
        int hash(std::string k) const;
        std::vector<std::pair<std::string, T>> hashtable;
        int numElements;
        int size [17] = {11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51437, 102877, 205759, 411527, 823117};
        int size_;
        long long int r[5] = {983132572, 1468777056, 552714139, 984953261, 261934300};
        bool debug_;
        bool resizing;
};

template <class T>
Hashtable<T>::Hashtable(bool debug, unsigned int size) :numElements(0), debug_(debug), size_(size) {
        if (size_ == 11) {      //if it uses the default size, then simply make the vector with the default size
            std::vector<std::pair<std::string, T>> hashtable(size_);
        } else {
            for (int i = 0; i < 17; ++i) {      //else find the size that is barely greater than the input size
                if (this->size_ < this->size[i]) {
                   
                    this->size_ = this->size[i];    //assign member variable size_ to that number
                    std::vector<std::pair<std::string, T>> hashtable;   //then create the vector
                    
                    break;
                }
            }
        }
        for (int i = 0; i < size_; ++i) {       //fill up the vector of pairs of string and T with literally empty and 0
            hashtable.push_back(std::make_pair("empty", 0));
        }
        if (debug_ == false) {          //if debug mode is false, then randomly generate the r values
            for (int i = 0; i < 5; ++i) {
                r[i] = rand();
            }
        }
        resizing = false;       //set resizing to false, since we're not resizing
        
    
}

template <class T>
Hashtable<T>::~Hashtable() {}       //no need for a destructor since we didn't dynamically allocate anything

template<class T>
int Hashtable<T>::hash(std::string k) const {
    int w [5];
    int temp = 0;
    long long int sum = 0;
    int count = 1;
    int stringLength = k.length();
    for (int i = 4; i >= 0; --i) {
        if (stringLength > 5) {
            count = 1;
            for (int j = stringLength - 6; j < stringLength; ++j) {     //convert each letter into strings and multiply the numeric value accordingly
                temp += pow(27, (6 - count)) * ((int)k[j] - 96);
                count++;
            }
            stringLength -= 6;
        } else if (stringLength != 0) {
            count = 0;
            for (int j = stringLength-1; j >= 0; --j) {
                temp += pow(27, count) * ((long long)k[j] - 96);
                count++;
            }
            stringLength = 0;
        } else{
            temp = 0;
        }
        w[i] = temp;
        temp = 0;
    }
    for (int i = 0; i < 5; ++i) {
        sum += (long long)this->r[i] * (long long)w[i];     //multiplication step, where we get the values from the string and r, and multiply them then add them all up
    }
    
    sum = sum % this->size_;    //mod the sum by size to make sure it fits into the vector
    return sum;
}


template<class T>
int Hashtable<T>::add (std::string k, const T& val) {   //dynamically allocate memories for the indices that are being used
    if (resizing == false) {        //resizing ensures that numElements doesn't increase while we're resizing the vector
        this->numElements++;
        double loadFactor = (double)this->numElements/(double)this->size_;  //calculate load factor before actually adding the items to make sure we have enough space
        if (loadFactor > 0.5) {
            this->resize();         //resize will be called when load factor becomes too big
        }
    }
    

    int numProbes = 0;

    int hashVal = this->hash(k);        //hash the string
    
    if (this->hashtable[hashVal].second == 0) { //since we filled up the vector of pairs with 0's for values, we can considered the spaces with 0's as empty
        this->hashtable[hashVal].first = k;     //assign the string and values accordingly
        this->hashtable[hashVal].second = val;
        
    } else {
        int originalHashVal = hashVal;
        for (int i = 0; i < this->size_; ++i) {
            ++numProbes;
            hashVal = ((int)(originalHashVal + pow(i + 1, 2)) % this->size_);   //if that space was taken, go through quadratic probing
            if (this->hashtable[hashVal].second == 0) {
                this->hashtable[hashVal].first = k;
                this->hashtable[hashVal].second = val;
                // std::cout << this->hashtable[hashVal].first << " " << this->hashtable[hashVal].second << std::endl;
                break;
            }
        }
    }
    return numProbes;
}

template<class T>
void Hashtable<T>::resize() {

    resizing = true;        //set resizing to true to ensure that numElements isn't increased when we add the items to our resized vector
    std::vector <std::pair<std::string, T>> temp;       //create a temp vector to hold all the items in our original hashtable vector
    for (int i = 0; i < size_; ++i) {
        temp.push_back(hashtable[i]);
    }

    int oldSize = this->size_;
    this->size_ = (this->size_ * 2) + 1;    //increase the size
    
    hashtable.resize(this->size_);      //resize function for vector
    hashtable.clear();              //clear out the vector since we need to rehash everything
    for (int i = 0; i < oldSize; ++i) {
        if (hashtable[i].second) {
            this->add(temp[i].first, temp[i].second);       //rehash then add everything to our resized vector
        } 
    }
    resizing = false;
}

template<class T>
void Hashtable<T>::reportAll (std::ostream & ofile) const {
    for (int i = 0; i < this->size_; ++i) {
        if (this->hashtable[i].second) {
            ofile << this->hashtable[i].first << " " << this->hashtable[i].second << std::endl;     //if an item exists, then output it to the given file stream
        }
    }
}

template<class T>
const T& Hashtable<T>::lookup (std::string k) {
    int hashVal = this->hash(k);            //very similar to add
    int oldHash = hashVal;
    if (this->hashtable[hashVal].second) {
        if (this->hashtable[hashVal].first == k) {      //if the key and the given string match, then output the value
            return this->hashtable[hashVal].second;
        } 
    else {
        for (int i = 0; i < this->size_; ++i) {
            hashVal = (int)(oldHash + pow(i + 1, 2)) % this->size_;     //probing to find the right value if the keys weren't matching
                if (this->hashtable[hashVal].first == k) {
                    return this->hashtable[hashVal].second;
                }
            }
        }
    }
    for (int i = 0; i < hashtable.size(); ++i) {        //if the item was not found, then return the first item avaible in the hashtable.
        if (hashtable[i].second != 0) {
            return hashtable[i].second;
        }
    }
}

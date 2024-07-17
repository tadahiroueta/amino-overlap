#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <string>
#include "AminoAcid.hpp"

using namespace std;

/**
 * @brief class for sequences of any biomolecular sequences with a respective enumerator
 */
template <typename T>
class Sequence {
    public:
        string name;
        vector<T> letters;

        /**
         * @param name of the sequence
         * @param letters a vector of all the elements as enumators
         */
       Sequence(string name, vector<T> letters) : name(name), letters(letters) {}

        /** 
         * @brief for ease of getting
         */
        T get(size_t i) { return letters[i]; }

        /**
         * @brief for ease of getting size
         */
        size_t size() { return letters.size(); }

        /**
         * @return in FASTA format with spaces
         */
        string to_string() {
            string output = "";
            output += ">" + name;
            for (size_t i = 0; i < letters.size(); i++) {
                if (i % 60 == 0) output += "\n";
                if (i % 10 == 0) output += " ";
                output += aminoAcidToString(letters[i]);
            }
            return output;
        }
};

#endif
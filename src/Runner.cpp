#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "AminoAcid.hpp"
#include "Sequence.hpp"
#include "Overlap.hpp"

using namespace std;

int main() {
    ifstream input("../data/melsim_seq.fasta");

    // sequence A
    string sequenceAName;
    getline(input, sequenceAName);
    sequenceAName = sequenceAName.substr(1);
    
    vector<AminoAcid> sequenceALetters;
    string line;
    while (getline(input, line)) {
        if (line.substr(0, 1) == ">") break;
        for (char letter : line) {
            if (letter == ' ') continue;
            sequenceALetters.push_back(char_to_amino_acid(letter));
        }
    }
    Sequence<AminoAcid> sequenceA(sequenceAName, sequenceALetters);

    // sequence B
    string sequenceBName = line.substr(1);

    vector<AminoAcid> sequenceBLetters;
    while (getline(input, line)) {
        if (line.substr(0, 1) == ">") break;
        for (char letter : line) {
            if (letter == ' ') continue;
            sequenceBLetters.push_back(char_to_amino_acid(letter));
        }
    }
    Sequence<AminoAcid> sequenceB(sequenceBName, sequenceBLetters);

    // calculate difference
    Overlap<AminoAcid> overlap(sequenceA, sequenceB);

    cout << overlap.to_string(amino_acid_to_char);

    return 0;
}
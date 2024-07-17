#ifndef OVERLAP_HPP
#define OVERLAP_HPP

#include <iostream>
#include <vector>
#include <stack>
#include <utility>
#include <functional>

#include "Sequence.hpp"

using namespace std;

/**
 * @brief Node for highest points and indices of previous node (where it came from)
 */
template <typename T>
class Node {
    public:
        pair<T,T> values;
        unsigned long points, previousA, previousB;

        Node(pair<T,T> values, unsigned long points, unsigned long previousA, unsigned long previousB) : values(values), points(points), previousA(previousA), previousB(previousB) {}
};

/**
 * @brief Compares two biological sequences, finding best optimal alignment with indels and counting minimum number of differences
 */
template <typename T>
class Overlap {
    // matrix for all the nodes
    vector<vector<Node<T>>> longestPaths;

    /**
     * @brief finds potential alignments (path with a graph analogy)
     * 
     * @param aTh the index of sequence A we are using for the overlap
     * @param bTh the index of sequence B we are using for the overlap
     * @param workingRow a vector including the left-neighbouring node
     * 
     * @return up to 3 options for nodes with different points (matches)
     */
    vector<Node<T>> getOptions(size_t aTh, size_t bTh, vector<Node<T>>& workingRow) {
        vector<Node<T>> options;

        // first
        if (aTh == 0 and bTh == 0) {
            options.push_back(Node<T>(pair<T,T>(X, X), 0, 10000, 10000));
            return options;
        }

        // top
        if (aTh > 0) {
            Node<T>& topPrevious = longestPaths[aTh - 1][bTh];
            pair<T, T> topValues(sequenceA.get(aTh - 1), X);
            unsigned long topPoints = topPrevious.points;
            options.push_back(Node<T>(topValues, topPoints, aTh - 1, bTh));
        }

        // diagonal
        if (aTh > 0 and bTh > 0) {
            Node<T>& diagonalPrevious = longestPaths[aTh - 1][bTh - 1];
            pair<T, T> diagonalValues(sequenceA.get(aTh - 1), sequenceB.get(bTh - 1));
            unsigned long diagonalPoints = diagonalPrevious.points + (diagonalValues.first == diagonalValues.second ? 1 : 0);
            options.push_back(Node<T>(diagonalValues, diagonalPoints, aTh - 1, bTh - 1));
        }

        // left
        if (bTh > 0) {
            Node<T>& leftPrevious = workingRow.back();
            pair<T, T> leftValues(X, sequenceB.get(bTh - 1));
            unsigned long leftPoints = leftPrevious.points;
            options.push_back(Node<T>(leftValues, leftPoints, aTh, bTh - 1));
        }
        return options;
    }

    /**
     * @param options a vector of nodes to be picked from
     * @return the node with the most points
     */
    Node<T> getLongestOption(vector<Node<T>> options) {
        if (options.size() == 1) return options[0];
        Node<T>& longestOfTwo = options[0].points > options[1].points ? options[0] : options[1];
        return longestOfTwo.points < options[2].points ? options[2] : longestOfTwo;
    }

    /**
     * @brief calculate the best possible alignment leading up to a point in the matrix (i.e. fill in longestPaths)
     */
    void calculateLongestPaths() {
        for (size_t aTh = 0; aTh < (sequenceA.size() + 1); aTh++) {
            vector<Node<T>> row;
            for (size_t bTh = 0; bTh < (sequenceB.size() + 1); bTh++) {
                row.push_back(getLongestOption(getOptions(aTh, bTh, row)));
            }
            longestPaths.push_back(row);
        }
    }

    /**
     * @brief trace back paths to find the best path/alignment and its differences (i.e. fill in overlap and differences)
     */
    void calculateLongestPath() {
        stack<pair<T,T>> reversedPath;
        differences = 0;
        Node<T>* node = &longestPaths[sequenceA.size()][sequenceB.size()];
        while (true) {
            if (node->previousA == 10000 and node->previousB == 10000) break;
            reversedPath.push(node->values);
            if (node->values.first != node->values.second) differences++;
            node = &longestPaths[node->previousA][node->previousB];
        }
        while (!reversedPath.empty()) {
            overlap.push_back(reversedPath.top());
            reversedPath.pop();
        }
    }

    public:
        Sequence<T>& sequenceA, sequenceB;
        vector<pair<T,T>> overlap; // the origal sequences plus indels
        unsigned long differences; // raw number of differences once indels are added

        /**
         * @brief immidiately calculates the differences between the sequences creating an overlap and more
         * 
         * @param sequenceA - the first sequence to be considered
         * @param sequenceB - the second sequence to be considered
         */
        Overlap(Sequence<T>& sequenceA, Sequence<T>& sequenceB) : sequenceA(sequenceA), sequenceB(sequenceB) {
            calculateLongestPaths();            
            calculateLongestPath();
        }

        /**
         * @brief for visualisation
         */
        string to_string(function<char(T)> enum_to_string) {
            string output = ">" + sequenceA.name + " v " + sequenceB.name + "; " + std::to_string(differences) + " differences\n";
            string topLine, bottomLine;
            for (pair<T,T> step : overlap) {
                topLine += enum_to_string(step.first);
                bottomLine += enum_to_string(step.second);

                if (topLine.size() == 65) {
                    output += topLine + "\n" + bottomLine + "\n\n";
                    topLine = "";
                    bottomLine = "";
                }

                if (topLine.size() % 11 == 10) {
                    topLine += " ";
                    bottomLine += " ";
                }
            }
            if (topLine.size() > 0) output += topLine + "\n" + bottomLine + "\n";
            return output;
        }        
};

#endif
#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <vector>
#include <string>
#include <climits>
#include "util/BinaryFile.hpp"

using namespace std;
using namespace GL;

class Dictionary {
public:

	Dictionary(string filepath);

	string& operator [] (int idx) { return dict[intmod(idx, dict.size())]; }

	int getClosestWordIndex(string);

	string getClosestWord(string s) { return dict[getClosestWordIndex(s)]; }

	bool contains(string s) { return s == getClosestWord(s); }

	void findSubwords(string s, vector<string>& matches, int limit = -1);

	void findSuperwords(string s, vector<string>& matches, int level = -1);

	struct Statistics {

		int longestWordLength;
		double vowelPrevalence;
		double freqs[26];
		char sortedAlphabet[26];

	};

	Statistics& getStats() { return stats; }

protected:

	vector<string> dict;
	Statistics stats;

	void process();

	static bool isletter(char c) { return c >= 'a' && c <= 'z'; }

	static int intmod(int x, int n) { return x % n + ((x >= 0) ? 0 : n); }

	static int calcfreq(string s, char c) { int freq = 0; for (int i = 0; i < s.size(); i++) if (s[i] == c) freq++; return freq; }

};

Dictionary::Dictionary(string filepath) {

	ReadBinaryFile rbf(filepath.c_str(), 8192 * 4);

	while (!rbf.isEOF()) {

		string word;
		bool foundLetter = false;

		while (true) { 
			
			char c = rbf.readByte(); 
			if (rbf.isEOF()) break;

			bool cIsLetter = isletter(c);
			if (!foundLetter && cIsLetter) foundLetter = true;
			if (foundLetter && !cIsLetter) break;
			
			if (cIsLetter) word += c;
		
		}

		if (!rbf.isEOF()) dict.push_back(word);

	}

	process();

}

int Dictionary::getClosestWordIndex(string s) {

	int lh = 0;
	int rh = dict.size();
	int mid = (lh + rh) / 2;

	while (lh < mid && mid < rh) {

		if (dict[mid] <= s) lh = mid;
		else rh = mid;
		mid = (lh + rh) / 2;

	}

	return lh;

}

void Dictionary::findSubwords(string s, vector<string>& matches, int limit) {

	if (limit < 0) limit = INT_MAX;

	static int freqs[26];
	for (int i = 0; i < 26; i++) freqs[i] = 0;
	for (int i = 0; i < s.size(); i++) freqs[s[i] - 'a']++;

	int counter = 0;

	for (int i = 0; i < dict.size(); i++) {

		if (counter == limit) return;

		bool foundMatch = true;
		for (int j = 0; j < dict[i].size(); j++) {

			char c = dict[i][j];
			if (calcfreq(dict[i], c) > freqs[c - 'a']) { foundMatch = false; break; }

		}

		if (foundMatch) matches.push_back(dict[i]);
		counter++;

	}

}

void Dictionary::findSuperwords(string s, vector<string>& matches, int level) {

	if (level < 0) level = INT_MAX;

	string sortedS;
	vector<int> sortedSFreqs;
	
	for (int i = 0; i < s.size(); i++) {

		double curMinFreq = 1.0;
		char curRarest = 0;

		for (int j = 0; j < s.size(); j++) {

			int idx = s[j] - 'a';
			if (stats.freqs[idx] < curMinFreq) {

				bool isRepeat = false;
				for (int k = 0; k < sortedS.size(); k++) if (s[j] == sortedS[k]) { isRepeat = true; break; }
				if (!isRepeat) { curRarest = s[j]; curMinFreq = stats.freqs[idx]; }

			}

		}

		if (!curRarest) break;

		int freq = calcfreq(s, curRarest);
		sortedS += curRarest; sortedSFreqs.push_back(freq);

	}
		
	for (int i = 0; i < dict.size(); i++) {

		if (dict[i].size() < s.size() || dict[i].size() > s.size() + level) continue;
		
		bool foundMatch = true;
		for (int j = 0; j < sortedS.size(); j++) if (calcfreq(dict[i], sortedS[j]) < sortedSFreqs[j]) { foundMatch = false; break; }

		if (foundMatch) { matches.push_back(dict[i]); }

	}

}

void Dictionary::process() {
	
	double numLetters = 0.0;
	stats.longestWordLength = 0;
	stats.vowelPrevalence = 0.0;
	for (int i = 0; i < 26; i++) { stats.freqs[i] = 0.0; stats.sortedAlphabet[i] = 0; }

	for (int i = 0; i < dict.size(); i++) {

		if (dict[i].size() > stats.longestWordLength) stats.longestWordLength = dict[i].size();

		for (char c : dict[i]) {

			if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') stats.vowelPrevalence += 1.0;
			stats.freqs[c - 'a'] += 1.0;
			numLetters += 1.0;

		}

	}

	stats.vowelPrevalence /= numLetters;
	for (int i = 0; i < 26; i++) stats.freqs[i] /= numLetters;

	for (int i = 0; i < 26; i++) {

		double curMax = 0.0;
		char charMax = 0;

		for (int j = 0; j < 26; j++) if (stats.freqs[j] >= curMax) {

			bool isRepeat = false;
			char c = 'a' + j;

			for (int k = 0; k < i; k++) if (c == stats.sortedAlphabet[k]) { isRepeat = true; break; }
			if (!isRepeat) { curMax = stats.freqs[j]; charMax = c; }

		}

		stats.sortedAlphabet[i] = charMax;

	}

}

#endif
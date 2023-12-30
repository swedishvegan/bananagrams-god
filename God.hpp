#ifndef GOD_HPP
#define GOD_HPP

#include <iostream>
#include "Dictionary.hpp"
#include "Board.hpp"

#define NUM_BAD_LETTERS 13

using namespace std;

class God {
public:

	God() {}

	string think(string, int = 0, int = 0);

	string remove(int);

	static Dictionary dict;
	Board board;

protected:

	struct word { string spelling; ivec2 position; bool vertical = false; double value = -1.0; int degree = 0; };

	vector<word> words;

	bool fits(string, ivec2, bool, ivec2);

	int getDegree(int);

	string addNewWord(string);

	string fill(string);

	string removeWord(string, int);

	static void recurse(int, God&, God&, vector<word>&, string&, double&, string, int);

	static string subtract(string, string, char);

	static double getBananagramsValue(string);

};

Dictionary God::dict("./wordlist.txt");

string God::think(string newLetters, int depth, int breadth) {
	
	if (newLetters.size() == 0) return newLetters;
	
	if (words.size() == 0) {

		vector<string> candidates;
		dict.findSubwords(newLetters, candidates);

		word bestWord;

		for (int i = 0; i < candidates.size(); i++) {
			
			double v = getBananagramsValue(candidates[i]);
			if (v > bestWord.value) {

				bestWord.spelling = candidates[i];
				bestWord.value = v;

			}

		}

		words.push_back(bestWord);
		board.addWord(bestWord.spelling, bestWord.position, bestWord.vertical);
		newLetters = subtract(newLetters, bestWord.spelling, 0);

	}
	
	newLetters = fill(newLetters);
	
	if (newLetters.size() == 0) return newLetters;
	if (words.size() <= 1) return newLetters;

	if (depth >= 1) {

		vector<word> sortedWords = words;
		bool sorted = false;

		while (!sorted) {

			bool done = true;

			for (int i = 0; i < sortedWords.size() - 1; i++) if (sortedWords[i].value > sortedWords[i + 1].value) {

				auto temp = sortedWords[i];
				sortedWords[i] = sortedWords[i + 1];
				sortedWords[i + 1] = temp;
				done = false;

			}

			sorted = done;

		}
		
		God mostPowerfulGod = *this;
		double minValue = 100000.0;
		
		if (breadth < 0) breadth = 0;
		recurse(breadth, *this, mostPowerfulGod, sortedWords, newLetters, minValue, newLetters, depth);
		*this = mostPowerfulGod;

	}

	return newLetters;

}

string God::remove(int num) {

	string newLetters;
	if (num < 1) return newLetters;

	for (int i = 0; i < num; i++) {

		if (words.size() == 0) return newLetters;

		double minValue = 100000.0;
		int minIdx = -1;

		for (int j = 0; j < words.size(); j++) {

			if (words[j].degree > 1) continue;
			if (words[j].value < minValue) { minValue = words[j].value; minIdx = j; }

		}

		newLetters = removeWord(newLetters, minIdx);

	}

	return newLetters;

}

bool God::fits(string s, ivec2 pos, bool vertical, ivec2 intersectionPos) {

#define _God_calcOffsets(isVertical) \
ivec2 offsetSmall = isVertical ? ivec2(1, 0) : ivec2(0, 1); \
ivec2 offsetBig = isVertical ? ivec2(0, 1) : ivec2(1, 0);

	_God_calcOffsets(vertical);
	ivec2 start = pos - offsetSmall;
	ivec2 end = pos + offsetBig * ((int)s.size() - 1) + offsetSmall;
	char c;

#define _God_check(x, y) \
c = board(ivec2(x, y)); \
if (c != ' ' && c != (char)0) return false;

	for (int x = start.x; x <= end.x; x++) for (int y = start.y; y <= end.y; y++) {

		if (vertical && y == intersectionPos.y) continue;
		if (!vertical && x == intersectionPos.x) continue;

		_God_check(x, y);

	}

	start = pos - offsetBig;
	end = pos + (int)s.size() * offsetBig;
	_God_check(start.x, start.y); _God_check(end.x, end.y);

	return true;

}

int God::getDegree(int idx) {

	word& w = words[idx];
	_God_calcOffsets(w.vertical);
	int deg = 0;

	for (int i = 0; i < w.spelling.size(); i++) {

#define _God_isCollision(thepos) ( board(thepos) != ' ' && board(thepos) != (char)0 )

		ivec2 pos = w.position + i * offsetBig;
		if (_God_isCollision(pos - offsetSmall) || _God_isCollision(pos + offsetSmall)) deg++;

	}

	return deg;

}

string God::addNewWord(string newLetters) {

	if (newLetters.size() == 0) return newLetters;
	
	for (int widx = 0; widx < words.size(); widx++) {
		
		vector<string> candidates;
		vector<double> values;
		
		for (int i = 0; i < words[widx].spelling.size(); i++) {

			candidates.clear();
			dict.findSubwords(newLetters + words[widx].spelling[i], candidates);

			if (candidates.size() == 0) continue;

			values.resize(candidates.size());
			for (int j = 0; j < values.size(); j++) values[j] = getBananagramsValue(candidates[j]);

			bool sorted = false;

			while (!sorted) {

				bool allGood = true;

				for (int j = 0; j < candidates.size() - 1; j++) if (values[j] < values[j + 1]) {

					allGood = false;

					string ctemp = candidates[j];
					candidates[j] = candidates[j + 1];
					candidates[j + 1] = ctemp;

					double vtemp = values[j];
					values[j] = values[j + 1];
					values[j + 1] = vtemp;

				}

				sorted = allGood;

			}

			ivec2 intersectionPos = words[widx].position + (words[widx].vertical ? ivec2(0, i) : ivec2(i, 0));

			for (int j = 0; j < candidates.size(); j++)
				
				for (int k = 0; k < candidates[j].size(); k++) if (candidates[j][k] == words[widx].spelling[i]) {

					ivec2 pos = intersectionPos - (words[widx].vertical ? ivec2(k, 0) : ivec2(0, k));

					if (fits(candidates[j], pos, !words[widx].vertical, intersectionPos)) {
						
						words[widx].degree++;

						word bestWord;
						bestWord.spelling = candidates[j];
						bestWord.position = pos;
						bestWord.vertical = !words[widx].vertical;
						bestWord.value = values[j];
						bestWord.degree = 1;

						words.push_back(bestWord);
						board.addWord(bestWord.spelling, bestWord.position, bestWord.vertical);
						
						return subtract(newLetters, bestWord.spelling, words[widx].spelling[i]);

					}
				}
		}
	}

	return newLetters;

}

string God::fill(string newLetters) {
	
	string newerLetters = addNewWord(newLetters);
	while (newerLetters != newLetters) { newLetters = newerLetters; newerLetters = addNewWord(newLetters); }
	return newerLetters;

}

string God::removeWord(string newLetters, int idx) {

	static int freqs[26];
	for (int i = 0; i < 26; i++) freqs[i] = 0;

	word w = words[idx];
	_God_calcOffsets(w.vertical);

	for (int i = 0; i < w.spelling.size(); i++) {

		ivec2 pos = w.position + i * offsetBig;
		if (_God_isCollision(pos - offsetSmall) || _God_isCollision(pos + offsetSmall)) freqs[w.spelling[i] - 'a']++;

	}

	for (int i = 0; i < w.spelling.size(); i++) {

		int fidx = w.spelling[i] - 'a';
		if (freqs[fidx] <= 0) newLetters += w.spelling[i];
		freqs[fidx]--;

	}

	words.erase(words.begin() + idx);

	board.clear();
	for (word& w : words) board.addWord(w.spelling, w.position, w.vertical);
	for (int i = 0; i < words.size(); i++) words[i].degree = getDegree(i);

	return newLetters;

}

void God::recurse(int breadth, God& god, God& mostPowerfulGod, vector<word>& sortedWords, string& finalNewLetters, double& minValue, string newLetters, int depth) {
	
	if (god.words.size() <= 1 || depth == 0) {

		newLetters = god.fill(newLetters);
		double value = getBananagramsValue(newLetters);

		if (value < minValue) {

			mostPowerfulGod = god;
			finalNewLetters = newLetters;
			minValue = value;

		}
		
		return;

	}

	if (breadth == 0) for (int i = 0; i < god.words.size(); i++) {

		if (god.words[i].degree > 1) continue;

		God demigod = god;
		string updatedNewLetters = demigod.removeWord(newLetters, i);
		recurse(breadth, demigod, mostPowerfulGod, sortedWords, finalNewLetters, minValue, updatedNewLetters, depth - 1);

	}
	else {

		int numChildren = 0;

		for (int i = 0; i < sortedWords.size(); i++) {

			int wordIdx = -1;
			for (int j = 0; j < god.words.size(); j++) if (god.words[j].spelling == sortedWords[i].spelling) { wordIdx = j; break; }
			if (wordIdx < 0) continue;
			if (god.words[wordIdx].degree > 1) continue;

			God demigod = god;
			string updatedNewLetters = demigod.removeWord(newLetters, wordIdx);
			recurse(breadth, demigod, mostPowerfulGod, sortedWords, finalNewLetters, minValue, updatedNewLetters, depth - 1);

			numChildren++;
			if (numChildren == breadth) break;

		}
	}
}

string God::subtract(string newLetters, string bestWord, char intersect) {
	
	static int freqs[26];
	for (int i = 0; i < 26; i++) freqs[i] = 0;
	if (intersect) freqs[intersect - 'a']--;

	for (int i = 0; i < bestWord.size(); i++) freqs[bestWord[i] - 'a']++;

	string newerLetters;
	for (int i = 0; i < newLetters.size(); i++) {

		int fidx = newLetters[i] - 'a';
		if (freqs[fidx] <= 0) newerLetters += newLetters[i];
		freqs[fidx]--;
	
	}
	
	return newerLetters;

}

double God::getBananagramsValue(string s) {

	double value = (double)s.size() * 0.01;

	for (int i = 0; i < s.size(); i++) {

		for (int j = 0; j < NUM_BAD_LETTERS; j++)
			if (s[i] == dict.getStats().sortedAlphabet[26 + j - NUM_BAD_LETTERS]) value += (double)j;

	}

	return value;

}

#endif
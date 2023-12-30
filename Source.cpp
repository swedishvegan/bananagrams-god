#include <iostream>
#include "God.hpp"

int main() {

	try {
		
		God god;

		while (true) {
			
			string letters; cout << ">>> "; getline(cin, letters);
			vector<string> words; string word;
			for (int i = 0; i < letters.size(); i++) { if (letters[i] == ' ') { words.push_back(word); word = ""; } else word += letters[i]; } words.push_back(word);

			bool invalid = false;
			if (words.size() == 1 && words[0] == "quit") break;

			if (words.size() > 1) {

				if (words[0] == "add" && words[1] == "letters") {

					if (words.size() > 5) { cout << "Too many arguments\n"; }
					else if (words.size() < 3) { cout << "Too few arguments\n"; }
					else {

						bool isWord = true;
						for (int i = 0; i < words[2].size(); i++) if (words[2][i] < 'a' || words[2][i] > 'z') isWord = false;

						if (!isWord) { cout << "Third argument must consist only of lower-case letters\n"; }
						else {

							int depth = 0;
							int breadth = 0;
							bool error = false;

							if (words.size() > 3) {

								try { depth = stoi(words[3]); }
								catch (...) { cout << "Depth argument must be an integer\n"; error = true; }

								if (!error && words.size() == 5) {

									try { breadth = stoi(words[4]); }
									catch (...) { cout << "Breadth argument must be an integer\n"; error = true; }

								}

							}

							if (!error && depth < 0) { cout << "Depth argument must be non-negative\n"; error = true; }
							if (!error && breadth < 0) { cout << "Breadth argument must be non-negative\n"; error = true; }

							if (!error) {

								try {

									string remainingLetters = god.think(words[2], depth, breadth);
									cout << god.board;
									cout << "Remaining letters: " << remainingLetters << "\n";
									error = false;

								}
								catch (...) { cout << "An unknown error occurred\n"; }

							}
						}
					}
				}

				else if (words[0] == "remove") {

					int n;
					bool error = false;

					try { n = stoi(words[1]); }
					catch (...) { cout << "Second argument must be an integer\n"; error = true; }

					if (!error && n < 0) { cout << "Second argument must be non-negative\n"; error = true; }

					if (!error) {

						try {

							string remainingLetters = god.remove(n);
							cout << god.board;
							cout << "Free letters: " << remainingLetters << "\n";
							error = false;

						}
						catch (...) { cout << "An unknown error occurred\n"; }

					}

				}

				else invalid = true;

			}
			else invalid = true;

			if (invalid) {

				cout << "List of commands:\n";
				cout << "*** add letters [string of lower-case letters] [search depth (default = 0)] [search breadth (full search if omitted)]\n";
				cout << "*** remove [number of words]\n";
				cout << "*** quit\n";

			}
		}
	}
	catch (Exception e) { cout << e.getMessage(); }

	return 0;

}
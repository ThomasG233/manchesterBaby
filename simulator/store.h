/*
	Group 8: Jessio Rodrigues, Jake Traynor, Sam Linehan, Ben Buchan, Thomas Gourlay
*/

#include <vector>
#include <fstream>
#include <bitset>


// error codes to be returned for file reading
const int SUCCESS = 0;
const int EXCEEDED_LENGTH = 1;
const int INVALID_CONTENT = 2;
const int FILE_NOT_FOUND = 3;
const int BITSET_TOO_LONG = 4;

class Store{
	private:
		std::vector<std::bitset<32> > storage;
	public:
		Store();
		std::bitset<32> fetch_line(int line);
		void set_line(std::bitset<32> acc, int ci);
		int load_file(std::string file);
		void resizeStorage(int userSize);
		int get_store_size();
};

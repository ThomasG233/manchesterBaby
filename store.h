#include <vector>
#include <fstream>
#include <bitset>

class Store{
	private:
		std::vector<std::bitset<32> > storage;
	public:
		Store();
		std::bitset<32> fetch_line(int line);
		void set_line(std::bitset<32> acc, int ci);
		void load_file(std::string file);
		int get_store_size();
};
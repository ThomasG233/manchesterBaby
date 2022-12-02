#include <vector>
#include <bitset>

class Store{
	private:
		std::vector<std::bitset<32> > storage;
	public:
		Store();
		std::bitset<32> fetch_line(int line);
		void set_line(std::bitset<32> newData, int line);
};

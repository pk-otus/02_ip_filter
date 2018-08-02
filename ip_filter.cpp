#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

template<typename T> using mini_v = std::vector<T>;
using mini_vvi = std::vector<std::vector<int>>;

namespace
{
	const size_t NUM_BYTES_IN_ADDR = 4;

	template<typename T> T	parse_string(const std::string &str);

	template <> int			parse_string(const std::string &str) { return stoi(str); }
	template <>	std::string parse_string(const std::string &str) { return str; }

	template<typename  T> mini_v<T> split(const std::string &str, char d)
	{
		mini_v<T> r;

		std::string::size_type start = 0;

		auto stop = str.find_first_of(d);
		while (stop != std::string::npos)
		{
			r.push_back(parse_string<T>(str.substr(start, stop - start)));

			start = stop + 1;
			stop = str.find_first_of(d, start);
		}

		r.push_back(parse_string<T>(str.substr(start)));

		return r;
	}

	auto printIPAddress (const mini_v<int>& ip_addr)
	{
		for (auto ip_part = ip_addr.cbegin(); ip_part != ip_addr.cend(); ++ip_part)
		{
			if (ip_part != ip_addr.cbegin())
			{
				std::cout << ".";

			}
			std::cout << *ip_part;
		}
		std::cout << std::endl;
	}

	bool less_ip(const mini_v<int>& a1, const mini_v<int>& a2)
	{
		assert(NUM_BYTES_IN_ADDR == a1.size());
		assert(NUM_BYTES_IN_ADDR == a2.size());

		for (auto i = 0; i < NUM_BYTES_IN_ADDR; ++i)
		{			
			if (a1[i] == a2[i]) continue;
			return a1[i] > a2[i];
		}
		return false;
	}

	template<int B = 0, int E = NUM_BYTES_IN_ADDR - 1> bool filter_bytes(const mini_v<int>& d, int byte_value)
	{
		assert(B >= 0);
		assert(B <= E);
		assert(E < d.size());

		for (size_t i = B; i <= E; ++i)
		{
			if (d[i] == byte_value) return true;
		}
		return false;
	}

	auto filter(const mini_vvi& source, int first_byte)
	{
		mini_vvi result;
		copy_if(source.begin(), source.end(), std::back_inserter(result), 
			[first_byte](const mini_v<int>& d) { return filter_bytes<0, 0>(d, first_byte); });
		return result;
	}

	auto filter(const mini_vvi& source, int first_byte, int second_byte)
	{		
		mini_vvi result;
		copy_if(source.begin(), source.end(), std::back_inserter(result),
			[second_byte](const mini_v<int>& d) { return filter_bytes<1, 1>(d, second_byte); });
		return filter(result, first_byte);		
	}

	auto filter_any(const mini_vvi& source, int byte_value)
	{		
		mini_vvi result;
		copy_if(source.begin(), source.end(), std::back_inserter(result),
			[byte_value](const mini_v<int>& d) { return filter_bytes(d, byte_value); });
		return result;
	}
}
int main(int argc, char const *argv[])
{
	try
	{
		mini_vvi ip_pool;

		for (std::string line; std::getline(std::cin, line);)
		{
			auto v = split<std::string>(line, '\t');
			ip_pool.push_back(split<int>(v.at(0), '.'));
		}
		
		std::sort(ip_pool.begin(), ip_pool.end(), less_ip);

		std::for_each(ip_pool.begin(), ip_pool.end(), printIPAddress);

		auto result = filter(ip_pool, 1); 
		std::for_each(result.begin(), result.end(), printIPAddress);

		result = filter(ip_pool, 46, 70); 
		std::for_each(result.begin(), result.end(), printIPAddress);

		result = filter_any(ip_pool, 46); 
		std::for_each(result.begin(), result.end(), printIPAddress);
		std::cout << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

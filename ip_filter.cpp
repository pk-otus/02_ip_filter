#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

namespace
{
	using ip_addr_t = std::vector<int>;
	using ip_adress_pool_t = std::vector<ip_addr_t>;

	const size_t NUM_BYTES_IN_ADDR = 4;

	template<typename T> T	parse_string(const std::string &str);

	template <> int			parse_string(const std::string &str) { return stoi(str); }
	template <>	std::string parse_string(const std::string &str) { return str; }

	template<typename  T> std::vector<T> split(const std::string &str, char d)
	{
		std::vector<T> r;

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

	auto printIPAddress (const ip_addr_t& ip_addr)

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

	bool less_ip(const ip_addr_t& a1, const ip_addr_t& a2)
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

	template<int BEGIN_OCTET = 0, int END_OCTET = NUM_BYTES_IN_ADDR - 1> 
	bool filter_bytes(const ip_addr_t& d, int byte_value)
	{
		assert(BEGIN_OCTET >= 0);
		assert(BEGIN_OCTET <= END_OCTET);
		assert(END_OCTET < d.size());

		for (size_t i = BEGIN_OCTET; i <= END_OCTET; ++i)

		{
			if (d[i] == byte_value) return true;
		}
		return false;
	}

	ip_adress_pool_t filter(const ip_adress_pool_t& source, int first_byte)
	{
		ip_adress_pool_t result;
		copy_if(source.begin(), source.end(), std::back_inserter(result), 
			[first_byte](const ip_addr_t& d) { return filter_bytes<0, 0>(d, first_byte); });
		return result;
	}

	ip_adress_pool_t filter(const ip_adress_pool_t& source, int first_byte, int second_byte)
	{		
		ip_adress_pool_t result;
		copy_if(source.begin(), source.end(), std::back_inserter(result),
			[second_byte](const ip_addr_t& d) { return filter_bytes<1, 1>(d, second_byte); });
		return filter(result, first_byte);		
	}

	ip_adress_pool_t filter_any(const ip_adress_pool_t& source, int byte_value)
	{		
		ip_adress_pool_t result;
		copy_if(source.begin(), source.end(), std::back_inserter(result),
			[byte_value](const ip_addr_t& d) { return filter_bytes(d, byte_value); });

		return result;
	}
}
int main(int argc, char const *argv[])
{
	try
	{
		ip_adress_pool_t ip_pool;

		for (std::string line; std::getline(std::cin, line);)
		{
			auto v = split<std::string>(line, '\t');
			ip_pool.emplace_back(split<int>(v.at(0), '.'));
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

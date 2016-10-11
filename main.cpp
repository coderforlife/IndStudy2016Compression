#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "get_cpu_time.h"
#include "sort.h"

template<class Comparable>
void copyPart(std::vector<Comparable> & dst, std::vector<Comparable> & src, size_t n)
{
	dst.assign(src.begin(), src.begin() + n);
}

template <class Comparable>
void dummy(std::vector<Comparable> & a) { }

template<class Comparable>
void time_it(void (*sort)(std::vector<Comparable> & v), std::vector<Comparable> & v)
{
	std::vector<Comparable> orig = v;

	size_t trials = 1;
	double begin = get_cpu_time();
	sort(v);
	double end = get_cpu_time();
	double elapsed_secs = end - begin;
	while (elapsed_secs < 0.1)
	{
		trials *= 10;
		begin = get_cpu_time();
		for (size_t i = 0; i < trials; ++i)
		{
			v = orig;
			sort(v);
		}
		end = get_cpu_time();
		elapsed_secs = end - begin;
	}
	std::cout << (elapsed_secs/trials) << " sec per trial - " << trials << " trials" << std::endl;
}

template<class Comparable>
void read_file(std::string filename, std::vector<Comparable> & v, size_t max=0x10000)
{
	std::ifstream file(filename.c_str());
	Comparable item;
	while (v.size() < max && (file >> item)) { v.push_back(item); }
}

template<class Comparable>
void write_file(std::string filename, std::vector<Comparable> & v)
{
	std::ofstream output;
	output.open(filename.c_str());
	for (size_t i = 0; i < v.size(); i++) { output << v[i] << std::endl; }
	output.close();
}

int main(int nargs, char** vargs)
{
	std::cout << "Filename to sort: ";
	std::string filename;
	std::cin >> filename;
	std::vector<std::string> unsorted, out;
	read_file(filename, unsorted);

	std::cout << "Total items: " << unsorted.size() << std::endl;

	std::vector<int> sizes;
	for (size_t i = 2; i < unsorted.size(); i*=2) { sizes.push_back(i); }
	sizes.push_back(unsorted.size());

	// Warm up!
	std::cout << "Warming up..." << std::endl;
	size_t warmUpSize = unsorted.size() > 8192 ? 8192 : unsorted.size();
	for (size_t i = 0; i < 3; ++i) { copyPart(out, unsorted, warmUpSize); time_it(timSort, out); }
	for (size_t i = 0; i < 3; ++i) { copyPart(out, unsorted, warmUpSize); time_it(mergeSort, out); }
	for (size_t i = 0; i < 3; ++i) { copyPart(out, unsorted, warmUpSize); time_it(insertionSort, out); }

	// Test copying
	for (size_t i = 0; i < sizes.size(); ++i)
	{
		std::cout << "Copying: " << sizes[i] << ": ";
		copyPart(out, unsorted, sizes[i]);
		time_it(dummy, out);
	}

	// Test sorting
	for (size_t i = 0; i < sizes.size(); ++i)
	{
		std::cout << "Tim Sort: " << sizes[i] << ": ";
		copyPart(out, unsorted, sizes[i]);
		time_it(timSort, out);
	}

	for (size_t i = 0; i < sizes.size(); ++i)
	{
		std::cout << "Merge Sort: " << sizes[i] << ": ";
		copyPart(out, unsorted, sizes[i]);
		time_it(mergeSort, out);
	}

	for (size_t i = 0; i < sizes.size(); ++i)
	{
		std::cout << "Insertion Sort: " << sizes[i] << ": ";
		copyPart(out, unsorted, sizes[i]);
		time_it(insertionSort, out);
	}

	unsorted = out; // now we do the sorted trials

	for (size_t i = 0; i < sizes.size(); ++i)
	{
		std::cout << "Merge Pre-Sort: " << sizes[i] << ": ";
		copyPart(out, unsorted, sizes[i]);
		time_it(mergeSort, out);
	}

	for (size_t i = 0; i < sizes.size(); ++i)
	{
		std::cout << "Insertion Pre-Sort: " << sizes[i] << ": ";
		copyPart(out, unsorted, sizes[i]);
		time_it(insertionSort, out);
	}

	write_file("output.txt", out);

	return 0;
}

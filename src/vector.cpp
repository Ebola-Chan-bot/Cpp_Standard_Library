/*	Copyright (C) 2004 Garrett A. Kajmowicz

	This file is part of the uClibc++ Library.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#define __UCLIBCXX_COMPILE_VECTOR__ 1


#include <vector>

namespace std{


#ifdef __UCLIBCXX_EXPAND_VECTOR_BASIC__

	template void vector<char, allocator<char> >::reserve(size_type n);
	template void vector<unsigned char, allocator<unsigned char> >::reserve(size_type n);
	template void vector<short int, allocator<short int> >::reserve(size_type n);
	template void vector<unsigned short int, allocator<unsigned short int> >::reserve(size_type n);
	template void vector<int, allocator<int> >::reserve(size_type n);
	template void vector<unsigned int, allocator<unsigned int> >::reserve(size_type n);
	template void vector<long int, allocator<long int> >::reserve(size_type n);
	template void vector<unsigned long int, allocator<unsigned long int> >::reserve(size_type n);
	template void vector<float, allocator<float> >::reserve(size_type n);
	template void vector<double, allocator<double> >::reserve(size_type n);
	template void vector<bool, allocator<bool> >::reserve(size_type n);

	template void vector<char, allocator<char> >::resize(size_type sz, char c);
	template void vector<unsigned char, allocator<unsigned char> >::resize(size_type sz, unsigned char c);
	template void vector<short int, allocator<short int> >::resize(size_type sz, short c);
	template void vector<unsigned short int, allocator<unsigned short int> >::resize(size_type sz, unsigned short int c);
	template void vector<int, allocator<int> >::resize(size_type sz, int c);
	template void vector<unsigned int, allocator<unsigned int> >::resize(size_type sz, unsigned int c);
	template void vector<long int, allocator<long int> >::resize(size_type sz, long int c);
	template void vector<unsigned long int, allocator<unsigned long int> >::resize(size_type sz, unsigned long int c);
	template void vector<float, allocator<float> >::resize(size_type sz, float c);
	template void vector<double, allocator<double> >::resize(size_type sz, double c);
	template void vector<bool, allocator<bool> >::resize(size_type sz, bool c);


#endif




}
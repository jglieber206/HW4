//
//  PagedDiskArray.cpp
//  COMP2355 Winter 2015 HW4
//
#define _CRT_SECURE_NO_WARNINGS    // needed for Visual Studio

#include "PagedDiskArray.h"
#include <cstdint>
#include <cstdio>

// Implement the PagedDiskArray class here
uint8_t obj[] = 12;


PagedDiskArray::PagedDiskArray(size_t pageSize, size_t numPages, const char*fileName) {
    pageSize = obj;
    numPages =
}

PagedDiskArray::~PagedDiskArray() {
    delete this;
}

uint8_t PagedDiskArray::operator[](size_t index) {
    return index;
}

void PagedDiskArray::set(size_t index, uint8_t value) {
    
}


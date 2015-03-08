//
//  PagedDiskArray.cpp
//  COMP2355 Winter 2015 HW4
//
#define _CRT_SECURE_NO_WARNINGS    // needed for Visual Studio

#include "PagedDiskArray.h"
#include <cstdint>
#include <cstdio>

// Implement the PagedDiskArray class here

PagedDiskArray::PagedDiskArray(size_t pageSize, size_t numPages, const char*fileName) : pageSize(1), numPages(1), arraySize(pageSize*numPages) {
    uint8_t array[arraySize];
    
    pageFile = fopen("Paged_Disk_File", "w+b");

    //initialize array
    for(int i = 0; i < arraySize; ++i) {
        array[i] = 0;
    }
}

PagedDiskArray::~PagedDiskArray() {
    Flush();
    fclose(pageFile);
    delete this;
}

uint8_t PagedDiskArray::operator[](size_t index) {
    return index;
}


/*****************************
 *
 *  Help!!
 *
 *****************************/
void PagedDiskArray::set(size_t index, uint8_t value) {
    size_t myPageNum = index/pageSize;
    size_t offset = index % pageSize;
    if(frames->pageLoaded == myPageNum) {
        frames->dirty = true;
        frames->buffer = &value;
    }
    pseudoTime ++;
    frames->accessPTime = pseudoTime;
    
}

void PagedDiskArray::WritePageIfDirty(PageFrame *f) {
    if(frames->dirty) {
        fwrite(f, f->pageLoaded, 1, pageFile);
        f->dirty = false;
    }
}

void PagedDiskArray::Flush() {
    for(int i = 0; i < numPageFrames; i++) {
        if(frames[i].dirty) {
            fwrite(frames[i].buffer, frames->pageLoaded, 1, pageFile);
        }
    }
}

void PagedDiskArray::LoadPage(size_t pageNum, PageFrame *f) {
    f->pageLoaded = pageNum;
}

PagedDiskArray::PageFrame *PagedDiskArray::GetPageFrame(size_t pageNum) {
    for(int i = 0; i < numPages; i++) {
        if(pageNum == frames->pageLoaded) {
            return &frames[i];
        }
    }
    return nullptr;
}

uint8_t *PagedDiskArray::GetElement(size_t index, bool dirty) {
    GetPageFrame(index);
    
    if(dirty) {
        frames->dirty = true;
    }

    uint8_t returnVal = operator[](index);
    
    return &returnVal;
}



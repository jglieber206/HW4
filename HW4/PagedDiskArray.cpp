//
//  PagedDiskArray.cpp
//  COMP2355 Winter 2015 HW4
//
#define _CRT_SECURE_NO_WARNINGS    // needed for Visual Studio

#include "PagedDiskArray.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Implement the PagedDiskArray class here


PagedDiskArray::PagedDiskArray(size_t pageSize_, size_t numPages_, const char*fileName) : pageSize(pageSize_), numPages(numPages_), arraySize(pageSize_*numPages_)
{
    pageFile = fopen(fileName, "w+b");
    
    if (pageFile == nullptr) {
        std::perror("fopen in ctor");
        std::exit(1);
    }
    
    for(int i = 0; i < numPageFrames; i++) {
        frames[i].buffer = new uint8_t[pageSize];
        for (size_t j = 0; j < pageSize; ++j) {
            frames[i].buffer[j] = 0;
        }
        frames[i].pageLoaded = size_t(-1);
        frames[i].dirty = false;
        frames[i].accessPTime = 0;
    }
    
    pseudoTime = 0;
    
    for (int i = 0; i < numPages; ++i) {
        fwrite(frames[0].buffer, frames[0].pageLoaded, 1, pageFile);
        if (pageFile == nullptr) {
            std::perror("fwrite in ctor");
            std::exit(1);
        }
    }
    
}

PagedDiskArray::~PagedDiskArray() {
    Flush();
    fclose(pageFile);
    if (pageFile == nullptr) {
        std::perror("fclose in destructor");
        std::exit(1);
    }
    delete this;
}

uint8_t PagedDiskArray::operator[](size_t index) {
    return *GetElement(index, false);
}

void PagedDiskArray::set(size_t index, uint8_t value)
{
    *GetElement(index, true) = value;
}

// Write all dirty pages to the disk file
void PagedDiskArray::Flush()
{
    for(int i = 0; i < numPageFrames; ++i)
    {
        WritePageIfDirty(&frames[i]);
    }
}

void PagedDiskArray::WritePageIfDirty(PageFrame *f)
{
    if (f->dirty == true)
    {
        fwrite(f, f->pageLoaded, 1, pageFile);
        if (pageFile == nullptr) {
            std::perror("fwrite in WritePageIfDirty");
            std::exit(1);
        }
        f->dirty = false;
    }
}

void PagedDiskArray::LoadPage(size_t pageNum, PageFrame *f)
{
    if( fseek(pageFile, sizeof(uint8_t)*(pageNum * pageSize), SEEK_SET) != 0) {
        std::perror("fseek in LoadPage err");
        std::exit(1);
    }
    if( fread(f->buffer, (uint8_t)sizeof(pageSize), 1, pageFile) != 0) {
        std::perror("fread in LoadPage err");
        std::exit(1);
    }
    f->pageLoaded = pageNum;
    f->dirty = false;
}

PagedDiskArray::PageFrame *PagedDiskArray::GetPageFrame(size_t pageNum)
{
    for(int i = 0; i < numPageFrames; i++)
    {
        if (pageNum == frames[i].pageLoaded)
        {
            return &frames[i];
        }
    }
    
    return nullptr;
    
}

PagedDiskArray::PageFrame *PagedDiskArray::ChooseReplacementFrame()
{
    PageFrame *tmp = &frames[0];
    
    for (int i = 0; i < numPageFrames; i++) {
        if(frames[i].accessPTime < tmp->accessPTime) {
            tmp = &frames[i];

        }
    }
    
    return tmp;
}

uint8_t *PagedDiskArray::GetElement(size_t index, bool dirty)
{
    if (index >= arraySize) {
        std::cerr << "You screw up with index " << index << "\n";
        std::exit(1);
    }
    size_t myPageNum = index / pageSize;
    size_t offset = index % pageSize;
    
    PageFrame *f = GetPageFrame(myPageNum);
    if (f == nullptr)
    {
        f = ChooseReplacementFrame();
        WritePageIfDirty(f);
        LoadPage(myPageNum, f);
    }
    

        f->dirty |= dirty;
    
    // update accessPTime
    ++pseudoTime;
    f->accessPTime = pseudoTime;
    
    return &f->buffer[offset];
}

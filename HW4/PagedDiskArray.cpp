//
//  PagedDiskArray.cpp
//  COMP2355 Winter 2015 HW4
//
#define _CRT_SECURE_NO_WARNINGS    // needed for Visual Studio

#include "PagedDiskArray.h"
#include <cstdint>
#include <cstdio>

// Implement the PagedDiskArray class here


PagedDiskArray::PagedDiskArray(size_t pageSize, size_t numPages, const char*fileName) : pageSize(1), numPages(1), arraySize(pageSize*numPages)
{
    fileName = "Paged_Disk_File";
    
    pageFile = fopen("fileName", "w+b");
    
    uint8_t array[arraySize];
    
    for (int i = 0; i < arraySize; ++i)
    {
        array[i] = 0;
    }
    
}

PagedDiskArray::~PagedDiskArray() {
    Flush();
    fclose(pageFile);
    delete this;
}

uint8_t PagedDiskArray::operator[](size_t index) {
    return *GetElement(index, false);
}


//FIXME
void PagedDiskArray::set(size_t index, uint8_t value)
{
    GetElement(index, false);
    if(frames->pageLoaded == numPages)
    {
        frames->buffer = &value;
        frames->dirty = true;
    }
    pseudoTime++;
    frames->accessPTime = pseudoTime;
}

// Write all dirty pages to the disk file
void PagedDiskArray::Flush()
{
    for(int i = 0; i < numPageFrames; ++i)
    {
        if (frames[i].dirty)
        {
            //            fwrite( , frames->pageLoaded, 1, pageFile);
        }
    }
}

void PagedDiskArray::WritePageIfDirty(PageFrame *f)
{
    if (f->dirty == true)
    {
        fwrite(f, f->pageLoaded, 1, pageFile);
        f->dirty = false;
    }
}

void PagedDiskArray::LoadPage(size_t pageNum, PageFrame *f)
{
    f->pageLoaded = pageNum;
    f->accessPTime++;
}

PagedDiskArray::PageFrame *PagedDiskArray::GetPageFrame(size_t pageNum)
{
    for(int i = 0; i < numPages; i++)
    {
        if (pageNum == frames->pageLoaded)
        {
            return &frames[i];
        }
        
    }
    
    return nullptr;
    
}

PagedDiskArray::PageFrame *PagedDiskArray::ChooseReplacementFrame()
{
    //PageFrame *mew =
    if (frames->dirty)
    {
        WritePageIfDirty(frames);
    }
    
    for (int i = 0; i < frames->accessPTime; i++)
    {
        
    }
    
    return frames;
}

uint8_t *PagedDiskArray::GetElement(size_t index, bool dirty)
{
    size_t myPageNum = index/pageSize;
    size_t offset = index%pageSize;
    
    PageFrame *f = GetPageFrame(myPageNum);
    if (f == nullptr)
    {
        f = ChooseReplacementFrame();
        WritePageIfDirty(f);
        LoadPage(myPageNum, f);
    }
    
    if (dirty == true)
        f->dirty = true;
    
    // update accessPTime
    f->accessPTime++;
    
    return &f->buffer[offset];
    //
    //    if (dirty ==false)
    //        frames->dirty = false;
}

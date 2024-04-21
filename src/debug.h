#pragma once

#define gpuCheckError() glCheckError_(__FILE__, __LINE__)

//#define gpuCheckError() do{} while(0)



void glCheckError_(const char* file, int line);
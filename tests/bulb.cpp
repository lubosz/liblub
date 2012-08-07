#include <cppunit/TestCase.h>

#include "Mesh/MandelBulb.h"
#include <thread>

using fractal::MandelBulb;

class MeshTest: public CppUnit::TestCase {
public:
  MeshTest(std::string name) :
    CppUnit::TestCase(name) {
  }

    void bulbThreads(int size) {

        unsigned threadCount = 16;
        unsigned threadSize = size / threadCount;

        vector<GLubyte> voxels;
        vector<vector<GLubyte>*> voxelParts;

        std::vector<std::thread> threads;

        for(int i = 0; i < threadCount; i++){
            vector<GLubyte> *voxelPart = new vector<GLubyte>();
            voxelParts.push_back(voxelPart);

            threads.push_back(
                        std::thread(
                            MandelBulb::generateVoxels,
                            threadSize * i, threadSize * (i+1), voxelPart, i+1, size));
        }

        for(auto& thread : threads){
            thread.join();
        }

        for (int i = 0; i < threadCount; i++) {
            foreach (GLubyte voxel, *voxelParts.at(i)) {
                voxels.push_back(voxel);
            }
        }
    }


  void runTest() {
    bulbThreads(128);
  }
};

int main() {
  MeshTest foo2("bar");
  foo2.runTest();
}





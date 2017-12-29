#include "catch.hpp"
#include "core/Array.h"
#include "core/Random.h"

void createRandom(int size, int* data, int vmin, int vmax)
{
    lray::RandXorshift64Star32 random(lray::getDefaultSeed());
    for(int i=0; i<size; ++i){
        data[i] = lray::range_ropen(random, vmin, vmax);
    }
}

void createRandomOrder(int size, int* data)
{
    lray::RandXorshift64Star32 random(lray::getDefaultSeed());
    for(int i=0; i<size; ++i){
        data[i] = i;
    }
    shuffle(random, data, data+size);
}

TEST_CASE("Test Array", "[Array]"){

    static const int NumSamples = 64;
    class TestDestruct
    {
    public:
        TestDestruct(int* countDestruct)
            :countDestruct_(countDestruct)
        {}

        ~TestDestruct()
        {
            if(NULL != countDestruct_){
                ++(*countDestruct_);
            }
        }

        int* countDestruct_;
    };

    SECTION("CheckLeak"){
        int countDestruct = 0;
        int destructOrder[NumSamples];
        createRandomOrder(NumSamples, destructOrder);

        lray::Array<TestDestruct*> array;
        for(int i=0; i<NumSamples; ++i){
            array.push_back(LNEW TestDestruct(&countDestruct));
        }
        for(int i=0; i<NumSamples; ++i){
            int index = destructOrder[i];
            REQUIRE(NULL != array[index]);
            LDELETE(array[index]);
        }
        REQUIRE(NumSamples == countDestruct);
    }

    SECTION("CheckDestruct"){
        int countDestruct = 0;
        lray::Array<TestDestruct> array;
        while(array.size()<NumSamples){
            array.push_back(lray::move(TestDestruct(&countDestruct)));
        }
        while(array.size()){
            array.removeAt(0);
        }
        REQUIRE(NumSamples <= countDestruct);
    }

}

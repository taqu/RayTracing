#define CPPIMG_IMPLEMENTATION
#include <cppimg/cppimg.h>
#include <cstdint>

int main(int , char** )
{
    typedef int32_t s32;
    typedef uint8_t u8;

    static const s32 Width = 400;
    static const s32 Height = 300;
    static const s32 Bpp = 3;
    u8* image = new u8[Width*Height*Bpp];

    for(s32 i=0; i<Height; ++i){
        u8 cy = (i&64)? 0xFFU : 0x00U;
        for(s32 j=0; j<Width; ++j){
            s32 pixel = (i*Width + j)*Bpp;
            u8 cx = (j&64)? 0xFFU : 0x00U;
            image[pixel+0] = cx;
            image[pixel+1] = cy;
            image[pixel+2] = ~cx & ~cy;
        }
    }
    cppimg::OFStream file;
    if(file.open("out.ppm")){
        cppimg::PPM::write(file, Width, Height, cppimg::ColorType_RGB, image);
        file.close();
    }
    delete[] image;
    return 0;
}

#include <cppimg/cppimg.h>
#include <cppgltf/cppgltf.h>
#include "lray.h"
#include "Camera.h"
#include "scene/Scene.h"

using namespace lray;

int main(int , char** )
{
    static const s32 Width = 800;
    static const s32 Height = 600;
    static const s32 Bpp = 3;
    u8* image = new u8[Width*Height*Bpp];

    Camera camera;
    camera.setResolution(Width, Height);
    camera.perspective(static_cast<f32>(Width)/Height, 60.0f*DEG_TO_RAD);
    camera.lookAt(Vector3(0.0f, 3.0f, 10.0f), Vector3(0.0f, 3.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));

    //Add object
    Scene scene;
    load(scene, "../../data/hatsune_miku_chibi_w_stand/scene.gltf");

    //Set light environment
    Vector3 lightDirection = normalize(Vector3(0.5f, 0.5f, 0.0f));

    //
    scene.updateFrame();

    //Loop over pixels
    ClockType startTime = getPerformanceCounter();
    for(s32 y=0; y<Height; ++y){
        for(s32 x=0; x<Width; ++x){
            Intersection intersection;
            Ray ray = camera.generateRay(static_cast<f32>(x), static_cast<f32>(y));
            u8 r,g,b;
            Result result = scene.test(intersection, ray);
            if(Result_Success & result){
                f32 d = maximum(dot(intersection.shadingNormal_, lightDirection), 0.0f);
                r = g = b = static_cast<u8>(minimum(clamp01(d)*256, 255.0f));
            }else{
                r = g = b = 128;
            }
            s32 pixel = (y*Width + x)*Bpp;
            image[pixel+0] = r;
            image[pixel+1] = g;
            image[pixel+2] = b;
        }
    }
    f64 elapsedTime = calcTime64(startTime, getPerformanceCounter());

    //Output
    cppimg::OFStream file;
    if(file.open("out.bmp")){
        cppimg::BMP::write(file, Width, Height, cppimg::ColorType_RGB, image);
        file.close();
    }
    delete[] image;

    printf("Render time %lf sec\n", elapsedTime);
    return 0;
}

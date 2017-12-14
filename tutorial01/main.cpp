#define CPPIMG_IMPLEMENTATION
#include <cppimg/cppimg.h>
#include "lray.h"
#include "Camera.h"
#include "Ray.h"
using namespace lray;

class Sphere
{
public:
    Vector3 position_;
    f32 radius_;
};

//-----------------------------------------------------------
bool testRaySphere(f32& t, const Ray& ray, const Sphere& sphere)
{
    Vector3 m = sub(ray.origin_, sphere.position_);

    f32 b = dot(m, ray.direction_);
    f32 c = dot(m, m) - sphere.radius_ * sphere.radius_;

    if(0.0f<c){
        if(0.0f<b){
            return false;
        }
    }else{
        t = 0.0f;
        return true;
    }

    //Check discriminant
    f32 discr = b*b - c;
    if(discr < 0.0f){
        return false;
    }

    discr = lray::sqrt(discr);
    b = -b;
    t = b-discr;
    f32 tmax = b + discr;
    return (tmax<=ray.t_);
}

int main(int , char** )
{
    static const s32 Width = 400;
    static const s32 Height = 300;
    static const s32 Bpp = 3;
    u8* image = new u8[Width*Height*Bpp];

    Camera camera;
    camera.setResolution(Width, Height);
    camera.perspective(static_cast<f32>(Width)/Height, 60.0f*DEG_TO_RAD);
    camera.lookAt(Vector3::zero(), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));

    //Add object
    Sphere sphere;
    sphere.position_ = Vector3(0.0f, 0.0f, 1.0f);
    sphere.radius_ = 0.25f;

    //Set light environment
    Vector3 lightDirection = normalize(Vector3(0.5f, 0.5f, 0.0f));

    //Loop over pixels
    for(s32 y=0; y<Height; ++y){
        for(s32 x=0; x<Width; ++x){
            Ray ray = camera.generateRay(static_cast<f32>(x), static_cast<f32>(y), 0.0f, F32_MAX);
            f32 t;
            u8 r,g,b;
            if(testRaySphere(t, ray, sphere)){
                Vector3 hitPosition = muladd(t, ray.direction_, ray.origin_);
                Vector3 normal = normalize(hitPosition - sphere.position_);
                f32 d = maximum(dot(normal, lightDirection), 0.0f);
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

    //Output
    cppimg::OFStream file;
    if(file.open("out.ppm")){
        cppimg::PPM::write(file, Width, Height, cppimg::ColorType_RGB, image);
        file.close();
    }
    delete[] image;
    return 0;
}

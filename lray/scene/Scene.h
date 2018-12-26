#ifndef INC_LRAY_SCENE_H__
#define INC_LRAY_SCENE_H__
/**
@file Scene.h
@author t-sakai
@date 2018/05/25 create
*/
#include "../lray.h"
#include "../core/Array.h"
#include "../shape/Node.h"
#include "../shape/Mesh.h"
#include "../accel/BinQBVH.h"
#include "../shape/TriangleProxy.h"

namespace lray
{
    /**
    */
    class Scene
    {
    public:
        typedef lray::Array<Mesh> MeshArray;
        typedef lray::Array<Node> NodeArray;
        typedef lray::Array<TriangleProxy> TriangleProxyArray;

        Scene();
        Scene(Scene&& rhs);
        explicit Scene(const Char* name, MeshArray&& meshes, NodeArray&& nodes);

        void updateFrame();
        Result test(Intersection& intersection, Ray& ray);

        Scene& operator=(Scene&& rhs);
    private:
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        String name_;
        MeshArray meshes_;
        MeshArray refinedMeshes_;
        NodeArray nodes_;

        TriangleProxyArray triangleProxies_;
        BinQBVH<TriangleProxy> accelerator_;
    };

    void load(Scene& scene, const Char* filepath);
}
#endif //INC_LRAY_SCENE_H__

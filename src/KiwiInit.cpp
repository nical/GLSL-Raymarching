
#include "kiwi/core/all.hpp"
#include "kiwi/utils/DebugStream.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"
#include "renderer/FrameBuffer.hpp"

#include "nodes/RayMarchingNode.hpp"

//#include <GL/glew.h>

using namespace kiwi;
using namespace glm;
using namespace kiwi::core;
using namespace renderer;

typedef FrameBuffer* FrameBuffer_ptr;
typedef Texture2D* Texture2D_ptr;

KIWI_DECLARE_CONTAINER(GLuint,"Uint");
KIWI_DECLARE_CONTAINER(GLint,"Int");
KIWI_DECLARE_CONTAINER(GLfloat,"Float");
KIWI_DECLARE_CONTAINER(vec4,"Vec4");
KIWI_DECLARE_CONTAINER(vec3,"Vec3");
KIWI_DECLARE_CONTAINER(vec2,"Vec2");
KIWI_DECLARE_CONTAINER(mat4,"Mat4");
KIWI_DECLARE_CONTAINER(Texture2D_ptr,"Texture2D");
KIWI_DECLARE_CONTAINER(FrameBuffer_ptr,"FrameBuffer");

void InitKiwi()
{
    DataTypeManager::RegisterDataType("Int", &NewGLint);
    DataTypeManager::RegisterDataType("Uint", &NewGLuint);
    DataTypeManager::RegisterDataType("Float", &NewGLfloat);
    DataTypeManager::RegisterDataType("Vec4", &Newvec4);
    DataTypeManager::RegisterDataType("Vec3", &Newvec3);
    DataTypeManager::RegisterDataType("Vec2", &Newvec2);
    DataTypeManager::RegisterDataType("Mat4", &Newmat4);
    DataTypeManager::RegisterDataType("Texture2D", &NewTexture2D_ptr);
    DataTypeManager::RegisterDataType("FrameBuffer", &NewFrameBuffer_ptr);


    kiwi::log << "available nodes:" << kiwi::endl;
    for(auto it = NodeTypeManager::Types_begin(); it!= NodeTypeManager::Types_end(); ++it )
    {
        kiwi::log << "# " << it->first << kiwi::endl;
    }

}

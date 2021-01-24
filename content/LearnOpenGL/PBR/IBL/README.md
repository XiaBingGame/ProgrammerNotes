# IBL

## Diffuse irradiance(漫反射辐照度)

* IBL, image based lighting，用于照射物体的技术集合。不像上一章节的直接解析光照那样，而是将周围环境看成一个大的光源。通常通过一个cubemap environment map实现，cubemap environment map 可以从真实世界中得到或者三维场景中生成。将每个 cubemap 纹素看成一个光源发射器。
* 再看反射公式

![ReflectanceEquation](Media/final_reflectance_equation.png)

* 我们的主要目标是求解所有入射光wi在半球上的积分。求解该公式有两个主要的要求：
  * 有某种方式在任意方向向量wi上返回场景的辐射
  * 能够快速实时的求解积分
* 第一个问题，环境立方体映射(environment cubemap)保存了环境或场景的辐照度。给了这样的立方体映射之后，立方体映射的每个纹素可以看成单个发射的光源。通过任意方向向量 wi 采样立方体映射，我们可以从该方向得到场景的辐射度。
* 任意方向向量 wi 的场景辐射度计算如下

```c++
vec3 radiance = texture(_cubemapEnvironment, w_i).rgb; 
```

* 然而，对于计算出半球上所有方向的辐射，在片段着色器中则太过昂贵。这里采用预处理或预计算的方式解决。
* 从反射公式可知， kd 和 ks 相互独立， 所以可以分解为两个部分

![SplitReflectanceEquation](Media/SplitReflectanceEquation.png)

* 关注漫反射部分， 将常量的 Lambert 部分移到积分之外。

![SplitConstantDiffuse](Media/splitconstantdiffuse.png)

* 积分仅仅依赖于wi(假设p位于环境映射的中心)。我们可以在每个位置p处计算出一个新的 cubemap。通过卷积计算出上面每个纹素(也就是每个出射方向wo)的漫反射积分。卷积的数据集为场景的辐射或环境映射。建立的半球(用于采样方向wi)朝向 wo。

![IBLHemisphereSample](Media/ibl_hemisphere_sample.png)

* 生成的 cubemap 称之为 irradiance map
* 可通过生成多个位置处反射探针的 irradiance map，并进行插值来解决某些场景的失真问题。
* 下图cubemap environment map 和相应的 irradiance map([Wave Engine](http://www.indiedb.com/features/using-image-based-lighting-ibl))。

![IBLIrradiance](Media/ibl_irradiance.png)

## PBR 和 HDR

* 需要某种方式将照明的高动态范围存储到环境贴图中。

### 辐射(radiance) HDR 文件格式

* 扩展名为 .hdr，存储六个面的数据，数据范围可在 0.0 和 1.0 之外。使用了一个聪明的技巧存储每个浮点值，不用每个通道存储32位的值，而是使用颜色的 8 位 alpha 通道存储颜色的 alpha 值。该做法需要解析每个颜色至他们的浮点值。
* 可以从 sIBL archive 获得许多免费的辐射HDR环境映射文件。

![IBLHDRRadiance](Media/ibl_hdr_radiance.png)

* 注意该图是从球形映射到平面的结果。

### HDR 和 st_image.h

* stb_image 支持加载辐射HDR图像，将其转换为浮点值数组。

```c++

#include "stb_image.h"
[...]

stbi_set_flip_vertically_on_load(true);
int width, height, nrComponents;
float *data = stbi_loadf("newport_loft.hdr", &width, &height, &nrComponents, 0);
unsigned int hdrTexture;
if (data)
{
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
}
else
{
    std::cout << "Failed to load HDR image." << std::endl;
}  
```

### 从 Equirectangular 变换至 Cubemap

* 虽然 equirectangular map 也可用于查找，但代价较为昂贵。
* 将 equirectangular 图像转换为 cubemap, 从内部投影 equirectangular map 至 cube 的六个面。 cube 的顶点着色器仅仅简单的渲染该 cube 后，将其自己的局部位置作为一个3D采样向量传递给片段着色器。

```c++
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 localPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    localPos = aPos;  
    gl_Position =  projection * view * vec4(localPos, 1.0);
}
```

* 片段着色器将球面内容贴到立方体上

```c++
#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(localPos)); // make sure to normalize localPos
    vec3 color = texture(equirectangularMap, uv).rgb;
    
    FragColor = vec4(color, 1.0);
}
```

* 结果如下

![IBLEquirectangularProjection](Media/ibl_equirectangular_projection.png)

* 渲染六次，使用FBO记录结果

```c++
unsigned int captureFBO, captureRBO;
glGenFramebuffers(1, &captureFBO);
glGenRenderbuffers(1, &captureRBO);

glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
```

* 生成对应的 cubemap 颜色纹理

```c++
unsigned int envCubemap;
glGenTextures(1, &envCubemap);
glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
for (unsigned int i = 0; i < 6; ++i)
{
    // note that we store each face with 16 bit floating point values
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
}
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

* 建立6个不同的视图矩阵(面向cube的每个面)，建立90度的投影矩阵，渲染6次将结果保存至浮点帧缓存内。

```c++

glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
glm::mat4 captureViews[] = 
{
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};

// convert HDR equirectangular environment map to cubemap equivalent
equirectangularToCubemapShader.use();
equirectangularToCubemapShader.setInt("equirectangularMap", 0);
equirectangularToCubemapShader.setMat4("projection", captureProjection);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, hdrTexture);

glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
for (unsigned int i = 0; i < 6; ++i)
{
    equirectangularToCubemapShader.setMat4("view", captureViews[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderCube(); // renders a 1x1 cube
}
glBindFramebuffer(GL_FRAMEBUFFER, 0);  
```

* 通过写一个简单的天空盒着色器测试刚生成的 cubemap

```c++
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

out vec3 localPos;

void main()
{
    localPos = aPos;

    mat4 rotView = mat4(mat3(view)); // remove translation from the view matrix
    vec4 clipPos = projection * rotView * vec4(localPos, 1.0);

    gl_Position = clipPos.xyww;
}
···

* xyww 这里只是确保深度永远最大。注意深度函数设置为 glDepthFunc(GL_LEQUAL);  
* 片段着色器

```c++
#version 330 core
out vec4 FragColor;

in vec3 localPos;
  
uniform samplerCube environmentMap;
  
void main()
{
    vec3 envColor = texture(environmentMap, localPos).rgb;
    
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
  
    FragColor = vec4(envColor, 1.0);
}
```

* 几乎所有的 HDR map 都处于线性空间内，所以在写入帧缓存之前需要应用伽玛校正。

![IBLHDREnvironmentMapped](Media/ibl_hdr_environment_mapped.png)

* [源码](https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/2.1.1.ibl_irradiance_conversion/ibl_irradiance_conversion.cpp)

## Cubemap convolution
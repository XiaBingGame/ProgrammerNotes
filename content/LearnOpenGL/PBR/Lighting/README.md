# 光照
* 实践部分，使用直接的光源或为分析性的光源。可看成为点光源，方向光光源，聚光灯光源。

![](Media/final_reflectance_equation.png)

* 目前该公式仍未知的是如何精确的表示辐照度(irradiance)，即 Li。辐射L，这里测量的是光源的辐射通量或光线能量，其限制于一个给定的球面角w，我们这里假设球面角w无限的小，以表示测量光源单个光线或某个方向向量下的辐射通量。
* 这里假设有点光源，所有方向辐射通量都一样(23.47, 21.31, 20.79)。其照射表面一点p，对于点p其上的半球，仅有光源到p之间连线的光线有入射辐射能量。

![](Media/lighting_radiance_direct.png)

* 这里假设其辐射强度等于其辐射通量(23.47, 21.31, 20.79), 转换代码如下，有衰减，有点乘衰减

```
vec3  lightColor  = vec3(23.47, 21.31, 20.79);
vec3  wi          = normalize(lightPos - fragPos);
float cosTheta    = max(dot(N, Wi), 0.0);
float attenuation = calculateAttenuation(fragPos, lightPos);
vec3  radiance    = lightColor * attenuation * cosTheta;
```

* 上面公式类似于我们之前的漫反射计算。注意的是上面公式假设的是点光源，如果光源有面积或体积，则点p则可能不仅一个光线方向上有入射光辐射能量。
* 对于方向光和聚焦光进行相应调整，比如衰减方式，光线方向等。
* 对于积分部分，由于每个光源仅有一条光线有辐射能量，这里可以简化处理。

## PBR 表面模型
* 片段着色器, 输入如下

```
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
  
uniform vec3 camPos;
  
uniform vec3  albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
```

* 开头进行光线算法需要的计算

```
void main()
{
    vec3 N = normalize(Normal); 
    vec3 V = normalize(camPos - WorldPos);
    [...]
}
```

* 4个光源，BRDF计算等

```
vec3 Lo = vec3(0.0);
for(int i = 0; i < 4; ++i) 
{
    vec3 L = normalize(lightPositions[i] - WorldPos);
    vec3 H = normalize(V + L);
  
    float distance    = length(lightPositions[i] - WorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance     = lightColors[i] * attenuation; 
    [...]  
```

* 这里使用距离的平方衰减辐射。 
* 对于每个光源，我们希望计算完整的 Cook-Torrance BRDF 部分
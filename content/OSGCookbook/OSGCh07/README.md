* 早期 osgdem 项目, 可以从原始的地形和纹理数据构建地形数据, 其结果很容易合并至场景图中.
* BlueMarbleViewer 项目, 使用 NASA 的 BlueMarble 图像构建地球模型. http://www.andesengineering.com/BlueMarbleViewer/
* osgdem 工具现在已经是一个完整的地形生成工具 VirutalPlanetBuilder. 

# OSGCh07Ex01
* 编译 VPB

# OSGCh07Ex02
* 生成一个小的地形数据库
* 一些应用:
    - vpbmaster: 命令行工具, 地形数据库生成
    - vpbcache: 为原始的源数据创建cache, 或者进行重映射
    - vpbsizes: 计算具体地形高宽的瓦片大小的便利计算器
    - osgdem: 地形创建工具, 其内部处理不同的地形瓦片, 如果 vpbmaster 构建了巨大的数据库, 则可能多个 osgdem 应用并行运行.
* Large Geometric Models Archive 项目有一些地形数据
    - http://www.cc.gatech.edu/projects/large_models/
* 将 BMP 数据转换成 GeoTiff 格式. 该格式允许 geo 相关的信息集成至 TIFF 文件.
```
# gdal_translate data/gcanyon_color_4k2k.bmp data/gcanyon_color_4k2k.png
# gdal_translate data/gcanyon_height.bmp data/gcanyon_height.png
```
* vpbmaster 构建我们的地形数据库库. -d 设置使用的数字地形map. -t 决定当作纹理使用的图像 -o 确定输出木和根文件名
```
# vpbmaster -d data/gcanyon_height.png -t data/gcanyon_color_4k2k.png -o output/out.osgb
```
* 查看
```
osgviewer output/out.osgb
```
* 生成的文件名 L[a]_X[b]_Y[c], a 表示level, b, c 表示范围标识. Level 0 表示最粗糙的层级.
* 分页添加的代码类似如下:
```
osg::Group* nextLvGroup;
...
nextLvGroup->addChild( pagedNextTile1 );
nextLvGroup->addChild( pagedNextTile2 );
nextLvGroup->addChild( pagedNextTile3 );
nextLvGroup->addChild( pagedNextTile4 );
osg::PagedLOD* pagedThisTile;
...
thisTile->addChild( dataOfThisLevel );  // The rough level
thisTile->addChild( nextLvGroup );  // The refined level
```
* 默认 VPB 每个瓦片生成 64x64的顶点且映射一个 256x256 的纹理. 我们可以控制生成的层级, 使用 -l 参数
```
# vpbmaster -l 3 -d data/gcanyon_height.png -t data/gcanyon_color_4k2k.png -o output/out.osgb
```
* 另外两个有用的参数是 --terrain (default) 和 --polygonal
    - --terain 表示使用 osgTerrain::TerrainTile 类生成基于高度域的网格几何.
    - --polygonal 将数据看成三角形面.
    - 这两个选项不能共存于一个地形生成的过程中.

# OSGCh07Ex03
* 生成地球上的地形数据
* 投影坐标系 --- 定义在一个平坦的二维表面上
* geographic coordinate system
* 下载地球的图像数据
    - TrueMarble: http://www.unearthedoutdoors.net/global_data/true_marble/download
    - BlueMarble: http://earthobservatory.nasa.gov/Features/BlueMarble/
* 处理数据
```
# vpbmaster -t data/TrueMarble.4km.10800x5400.tif --geocentric -o output/out.osgb
```
* gcanyon 数据没有一个有效的 WKT(well-known text) 坐标系统, 因此无法识别为真实世界的地面.
* OSG 有一个特殊的节点类型 osg::CoordinateSystemNode 可以让 viewer 系统将数据从 XYZ 坐标转换成经纬高坐标. 其还会构建一个本地的过渡矩阵用于场景图中的节点变换. 
  - --polygonal 模式, VPB 设置其为整个场景子图的父节点, 来控制地球上所有子瓦片的生成. 
  - --terrain 模式, 使用 osg::CoordinateSystemNode 的派生类 osgTerrain::Terrain 代替, 这两个类都会存储地球的维度和坐标信息. 
  - --geocentric 选项表示使用地心作为地形的中心, 单位为米. 实际上定义为 ECEF 坐标系统, World Geodetic System 1984(WGS-84).
    - 见 http://en.wikipedia.org/wiki/ECEF
  - -cs 选项可以利用某些坐标系统. 使用 PROJ4 格式字符串声明新的坐标系统, 如 
```
# vpbmaster -cs "+proj=latlong +datum=WGS84" ...
```
  - 更多关于坐标系统字符串格式的信息可见 PROJ4 项目网站
```
http://trac.osgeo.org/proj/
```

# OSGCh07Ex04
* 多个图像以及高度数据一起工作.
* VPB 读取多个输入, 或读取一个包含许多更小瓦片图像的子目录. 并将它们组合成一个完整的地形模型. 可以通过 -d 和 -t 选项完成.
* 多谢 SRTM 项目, 我们可以自由下载和使用全球海拔数据和纹理图. 
    - http://srtm.csi.cgiar.org/SELECTION/inputCoord.asp
* 可以使用 -d 指定一个文件夹
```
# vpbmaster -d data/srtm -t data/TrueMarble.4km.10800x5400.tif --geocentric -o output/out.osgb
```
* 也可以用 -d 指定多个文件
```
# vpbmaster -d data/srtm/srtm_54_05.tif -d data/srtm/srtm_55_05.tif -d data/srtm/srtm_54_06.tif -d data/srtm/srtm_55_06.tif -t data/TrueMarble.4km.10800x5400.tif --geocentric -o output/out.osgb
```
* build_master.source 是一个 ASCII 文件, 包含所有的源数据和构建选项. 类似于 OSG 原生场景文件 (.osgt), 也许可以用 osgDB::readNodeFile() 函数加载. 其有一个 osgTerrain::TerrainTile 节点保存了构建的选项(输出名称, 范围, level 等等), 这些选项都可用于 vpb::DatabaseBuilder 对象, 并保存了用于不同输入数据的子层.
* build_master.task 文件记录了所有要生成的子瓦片. 在 tasks 文件夹可以发现每个子瓦片任务的状态. 一个状态文件可能如下:
```
application : osgdem --run-path /usr/local/bin -s 
  build_master.source --record-subtile-on-leaf-tiles -l 8 --
  subtile 3 0 0 --task tasks/build_subtile_L3_X0_Y0.task --
  log logs/build_subtile_L3_X0_Y0.log
date : [building time]
duration : [building duration]
fileListBaseName : output\out_subtile_L3_X0_Y0/
  out_L3_X0_Y0.osgb.task.0
hostname : [host name]
pid : [pid]
source : build_master.source
status : [pending/completed]
```
* 如果构建过程被取消, 你可以利用 task 文件的状态使用 --tasks 参数重新运行 vpbmaster
```
# vpbmaster --tasks build_master.tasks
```

# OSGCh07Ex05
* 在已有的地形数据库上更新数据
* VPB 支持该功能, 它需要源文件和所有的原始数据, 而后添加新的光栅和高度数据使用更高的分辨率更新数据
* 前提条件:
    - 新的数据
    - 所有的源文件以及用于生成他们的 build_master.source 文件, 这些用于处理分辨率和边界问题.
* 创建一个白色的 sub_gcanyon_height.tif 文件
* 创建一个 sub_gcanyon_height.wld 文件, 填入如下内容:
```
0.5
0.0
0.0
-0.5
1000
500
```
* 更新
```
# vpbmaster --patch build_master.source -d data/sub_gcanyon_height.tif
```
* 世界文件 wld 文件. 用于 geo-referencing raster map 图像. 由 ESRI 首次引用. 这种类型的文件(.tfw, .tifw, .wld)常用六行文字描述位置, 缩放, 和旋转. 当 GDAL 准备读取一个 TIFF 文件时, 常自动寻找同名的世界文件.
* 上面世界文件每一行的意思
    - 第一行: 沿着 X 轴的像素大小
    - 第二行: 关于 X 的旋转(大多数时候为0)
    - 第三行: 关于 Y 的旋转(大多数时候为0)
    - 第四行: 沿着 Y 轴的像素大小. 常为负值. 因为图像大小是从顶到底的顺序存储的
    - 第五行: 中心X的左上像素
    - 第六行: 中心Y的左上像素
* gdalinfo 文件可以用于检查图像和其关联的 wlf 文件
```
# gdalinfo data/sub_gcanyon_height.tif
```

# OSGCh07Ex06
* NVIDA Texture Tools(NVTT), 开源的图像处理和纹理操作项目.

# OSGCh07Ex07
* 使用 SSH 实现群集生成

# OSGCh07Ex08
* 从网络下载和渲染地形
* 由于依赖于 cURL 库的 osgdb_curl 插件, OSG 可以快速从远程服务器通过多个协议读取文件.
* OSG 同样提供一个简单的 file-cache 机制, 该机制写入临时文件, 从网络读取文件至本地硬盘, 当相同的传输请求再次到来时可以直接加载硬盘文件. 该机制目前只用于分页的节点, 该节点由 osgDB::DatabasePager 类动态管理(根据当前的视点加载和移除). 这样的解决方案避免用户应用重复访问和下载远程网站不变的数据, 这样可以节省带宽和加载时间.
* 创建一个包含地形数据的服务器, 可以给匿名访问者访问权限, 可以用 AppServ ((http://www.appservnetwork.com/) 实现这样的网站.
* 本地演示
```
# osgviewer http://127.0.0.1/output/out.osgb
```
* 实际的例子
```
# osgviewer http://www.openscenegraph.org/data/
  earth_bayarea/earth.ive
```
* 使用文件cache机制, 通过环境变量实现
```
# export OSG_FILE_CACHE = /home/cache
# mkdir /home/cache
```
* osgDB::FileLocationCallback 对象可以决定一个文件是否应该被 cached. 该对象有两个重写的方法
```
virtual Location fileLocation(const std::string& filename, const Options* options);
virtual bool useFileCache() const;
```
  - 第一个方法判断文件是本地还是远程的.
  - 第二个方法用于快速允许和禁止 caching 的使用.
* setFileLocationCallback() 方法设置你自己的位置回调.
```
osg::ref_ptr<osgDB::Options> options = new osgDB::Options;
options->setFileLocationCallback( ownCallback );
pagedNode->setDatabaseOptions( options.get() );
```
* 地形数据库的版本控制, 见 osgdatabaserevisions 例子的代码
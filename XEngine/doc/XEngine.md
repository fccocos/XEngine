坐标

材质：表面材质、纹理 物理材质

环境：光照（光源） 环境贴图

对象的变换

三维场景

相机

网格

## 场景绘制

```cpp
while(m_Running){
	RenderCommand::SetClearColor();
	RenderCommand::Clear();

	Renderer::BeginScense();
	Renderer::Submit();
	Renderer::EndScense();

	Renderer::Flush();
}

```

## 相机

```cpp
OpenGL: proj*view*model*VP
DX11: VP*model*view*proj

BeginScense(Camera){
	Camera->VP(Unifrom) 
}
```

实体组件系统(Anti-Component System)--游戏对象

变换系统: Transform Rotate Scale

着色器库 ShaderLibrary

2D渲染器 Render2D 

批处理渲染器: textue text

纹理图集系统

动画精灵 Animation Sprite

精灵表：用于存放动画精灵动画

帧动画：

GUI：Layout Widget Text TextAlign

后期效果： 粒子系统、HDR、LOT、色彩校正

Interaction: Script（脚本）——lua、蓝图

实体组件系统 ECS

Player
_
_ render2D
_ 


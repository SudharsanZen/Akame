
![Akame](/Assets/EngineAssets/logo2.png?raw=true "Akame")

## Introduction
Akame is a Graphics framework built upon OpenGL. It's designed to be simple, fast,easy to use and easily extendible.
It was originally made for the development of Akame Engine, which is a game engine built on this framework (still under development),
(i.e) this framework will grow as the engine grows.

**NOTE:** Akame is still in it's very early-stage and currently only supports Windows, many of the functionalities may change in the future drastically!

## Quick Start
Visual Studio 2019 is recommended.

Make sure Python and CMake version 3.22.0 or higher is installed and is added to the System PATH before proceeding!

<ins>**1.Downloading The Repository:**</ins>

First Start by cloning the repository with `git clone --recursive https://github.com/SudharsanZen/Akame.git` to clone the repo along with all the submodules.

<ins>**2.Configure And Build The Project with Release Configuration:**</ins>
1. Run the [generate_projects.bat](https://github.com/SudharsanZen/Akame/blob/main/generate_projects.bat), this just generates all the dependency, samples and framework projects and integrates all the projects that are needed into one solution (Akame.sln).
2. Open Akame.sln and build solution with Release-Config.


# Akame

**what is akame?**

Akame is both a framework (Akame-Core) and a game engine(Akame-Engine) for building interactive applications.
so to put it straight, these are the things you can do:

  * Use Akame-Core and ignore the Engine.
  * Use Akame-Core and Use the Engine as an IDE for editing the scene.
  * Use the engine like any other game engine.

At the time of writing, The engine part is not fully complete (not fully functional) and as a solo developer I am still working on it as you are reading this!

Here are the stuff the framework and engine currently offers:

## Akame ECS

The custom made Entity Component System (ECS) is the core component of both the framework and you guessed it, the engine! It's designed to be very fast and easy to use.
if you are interested in taking a look at it you can find it in the Akame's repo [here](https://github.com/SudharsanZen/Akame/tree/main/ECS), it's a header only ECS implementation written leveraging the power of templates.

## Akame-core
Akame-core is the Graphics Framework part, it's designed to be simple to use but also to be flexible and easily extendable. Enough said, here's a sample code to create a sphere and a directional light using my engine:
```c++
/*before you do anything, don't forget to set the relative location (relative to your working directory) of the assets folder that contains all the shaders, engine assets and your assets as shown below*/
AssetManager::setAssetRoot("../../../../Assets/");
```

```c++
//initializing a window (800 by 800 pixels wide)
Window window(800, 800, "testWindow");
if (!window.initialize())
{
  std::cout << "Can't init WINDOW";
  return -1;
}
```

```c++
//initializing the scene object
Scene scene(window);
```

and now we are ready to create 'Entities' (directional light and sphere)

```c++
/*-------------Creating Directional Light--------------*/
  //create an entity and get it's id
  Entity dir = scene.CreateEntity();
  //add 'Lights' and Transform Component to 'dir' entity
  scene.AddComponent<Transform>(dir);  
  Lights& d = scene.AddComponent<Lights>(dir);  
  //set the light's properties
  d.setType(LIGHT::DIRECTIONAL);
  d.setColor(1, 1, 1);
  d.setDirection(-90, 0, 0);
  d.setIntensity(3);
  d.setPointLightConst(1, 2, 10);
  d.ambientLigting(0.1f, 0.1f, 0.1f);
```

**Now here come's the part of creating a mesh (Sphere).
for an Entity to become a renderable mesh, we need to add three components to it:**
  * **Material**  : This component is an interface to select the desired Shaders Defined in the [ShaderConf.XML](https://github.com/SudharsanZen/Akame/blob/main/Assets/Shaders/ShaderConf.XML)and set material-properties\Shader-Uniforms to be used.
  * **Transform** : This component is used to describe and modify the position and orientation of an Entity.
  * **Mesh**      : This component is used to transfer mesh/vertex data to the rendering System.

**step1:** creating the entity:
```c++
  Entity sphere = scene.CreateEntity();
```
**step2:** creating a Material:
```c++
  std::string rootDir(AssetManager::getAssetRoot());

  Material rust1("DEFERRED");
  rust1.setTexture2D("material.diffuse", rootDir + "Media/pbr/rust1/diffuse.png");
  rust1.setTexture2D("material.roughness", rootDir + "Media/pbr/rust1/roughness.png");
  rust1.setTexture2D("material.normal", rootDir + "Media/pbr/rust1/normal.png");
  rust1.setTexture2D("material.metallic", rootDir + "Media/pbr/rust1/metallic.png");
  rust1.setValue("noAO", 1);
  rust1.setValue("ambientocclusion", 1);
  rust1.setValue("noMetallic", 0);
  rust1.setValue("normalStrength", 1);

```

**step3:** Add transform, mesh and material component to the entity
```c++
  Mesh& sphere_mesh = scene.AddComponent<Mesh>(sphere);
  Transform& sphere_t = scene.AddComponent<Transform>(sphere);
  scene.AddComponent<Material>(sphere) = rust1;
```

**step4:** set Mesh data and transform parameters:

```c++
sphere_t.SetGlobalScale(glm::vec3(2.0f));
sphere_t.SetGlobalPosition(glm::vec3(0,0.01,0));
//generateSphereVertices(num of latitude segments,num of longitude segments,radius)
sphere_mesh.CreateMesh(generateSphereVertices(32,32,2));
```

and that's it, we've create a renderable entity (The Sphere) and a light entity (The directional light)!

now finally we need to create the main loop and render stuff.
which can be done with a few lines of self-explanatory code as shown below:

```c++
while (!window.closeWindow())
{
  flyCam(scene.cam, scene.getDeltaTime());
  scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());

  scene.clearBuffer();
    scene.Render();
  scene.swapBuffers();
}
```
**here's the final code:**
```c++
#include<iostream>
#include"Core/Engine.h"
#include"Core/Scene.h"

int main()
{

	AssetManager::setAssetRoot("../../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "testWindow");
	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}
	Scene scene(window);

	Entity dir = scene.CreateEntity();
	Lights& d = scene.AddComponent<Lights>(dir);
	scene.AddComponent<Transform>(dir);
	d.setType(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(-90, 0, 0);
	d.setIntensity(3);
	d.setPointLightConst(1, 2, 10);
	d.ambientLigting(0.1f, 0.1f, 0.1f);



	Material rust1("DEFERRED");
	rust1.setTexture2D("material.diffuse", rootDir + "Media/pbr/rust1/diffuse.png");
	rust1.setTexture2D("material.roughness", rootDir + "Media/pbr/rust1/roughness.png");
	rust1.setTexture2D("material.normal", rootDir + "Media/pbr/rust1/normal.png");
	rust1.setTexture2D("material.metallic", rootDir + "Media/pbr/rust1/metallic.png");
	rust1.setValue("noAO", 1);
	rust1.setValue("ambientocclusion", 1);
	rust1.setValue("noMetallic", 0);
	rust1.setValue("normalStrength", 1);


	Entity sphere = scene.CreateEntity();
	Mesh& sphere_mesh = scene.AddComponent<Mesh>(sphere);
	sphere_mesh.CreateMesh(generateSphereVertices(32,32,2));
	Transform& sphere_t = scene.AddComponent<Transform>(sphere);
	sphere_t.SetGlobalScale(30.0f* glm::vec3(1));
	sphere_t.SetGlobalPosition(glm::vec3(0,0.01,0));
	scene.AddComponent<Material>(sphere) = rust1;

	while (!window.closeWindow())
	{
	 flyCam(scene.cam, scene.getDeltaTime());
	 scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());

	 scene.clearBuffer();
			scene.Render();
	 scene.swapBuffers();
	}

	return 0;
}

```

**results:**
![alt text](https://github.com/SudharsanZen/SudharsanZen.github.io/blob/main/img/posts/sphere_dir.png?raw=true)
*Note*: if you've copy pasted the code and ran it and endded up with a black screen it's probably because the camera is inside the large sphere we've created XD, so move the camera out with WASD keys and your mouse.




## Creating custom Materials, Shaders and Rendering pipeline:
In Akame, it's very possible to write your own shader, material and a rendering pipeline to handle all the renderable entities that use a particular shader/material.
To register a new Shader with Akame, find the [_ShaderConf.XML_](https://github.com/SudharsanZen/Akame/blob/main/Assets/Shaders/ShaderConf.XML) file in the Assets folder of Akame and add details describing your shaders.

_**Example:**_
let's say you have a fragment shader named "frag.frag" and a vertex shader named "vert.vert" that's in the [same folder](https://github.com/SudharsanZen/Akame/tree/main/Assets/Shaders) as the [ShaderConf.XML](https://github.com/SudharsanZen/Akame/blob/main/Assets/Shaders/ShaderConf.XML) file.
Then to register the shader under the name "MyNewShader", you add this to the ShaderConf.XML:
```c++
<Shader name="MyNewShader" queue="NULL">
    <Vertex path="vert.vert"/>
    <Fragment path="frag.frag"/>
</Shader>
```

And that's it, it's ready to be used from within the framework's Material component.
The Material component takes in the name of the shader as a string argument. So, to create an entity with Material using 'MyNewShader' you do:
```c++
  Material newMaterial("MyNewShader");

  ...set material paramters...

  scene.AddComponent<Material>(somRenderableEntity)=newMaterial;
```

**Creating a new shader/render pipeline for our new shader:**
if you feel like you need more control than what the Material Interface offers, you can go for a yourown shader/render pipeline. To do that, from within the framework you need to inherit from the ['ShaderRenderPipeline'](https://github.com/SudharsanZen/Akame/blob/main/AkameCore/Header%20Files/Rendering/System/ShaderRenderPipeline.h) class.

```c++
/*This class is a base class that provides the
*interface for overriding callbacks like
*pre-render and post-render actions for
*a particular shader.*/
class AKAME_API ShaderRenderPipeline
{
public:

	//called everytime the windows is resized
	virtual void WindowsResizeCallBacks(int height,int width){
	}

	//called just before the rendering of entities begins
	virtual void OnPreRender(std::shared_ptr<Shader> shader,RenderingSystem *rsys,Camera cam, unsigned int frameBuffer=0){
	}

	//called after rendering stuff
	virtual void OnPostRender(std::shared_ptr<Shader> shader,RenderingSystem *rsys, Camera cam, unsigned int frameBuffer=0){
	}
	//called before every time an entity is rendered
	virtual void OnEntityRender(std::shared_ptr<Shader> shader,std::shared_ptr<ECS> e,Camera cam,Entity eid){
	}

	//destructor
	virtual ~ShaderRenderPipeline(){
	}

};
```

**Example:**

```c++
class MyNewRenderPipeline :public ShaderRenderPipeline
{
public:

	AKAME_API void OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
	AKAME_API void OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
};

**then somewhere before the main loop and after initializing the scene, add your new render/shader pipeline to akame as shown below:**


ShaderManager::AttachShaderPipeline<MyNewRenderPipeline>("MyNewShader");

```

## Physics System
  The engine uses [NVIDIA-PhysX](https://github.com/NVIDIAGameWorks/PhysX) as it's physics engine.
  The engine provides an abstracted and easy to use Interface to the PhysX engine via the 'RigidBody3D' component.
  Here's a [sample-code](https://github.com/SudharsanZen/Akame/tree/main/Samples/Test) utilizing this component.
  And here's it's Visual demo:
  
[![image alt text](https://img.youtube.com/vi/PHQd2YKL90E/0.jpg)](https://www.youtube.com/watch?v=PHQd2YKL90E)

## C++ Scripting System
  The Akame-Engine provides a C++ Scripting+Behviour system for creating off-the-shelf components and modifying the framework's normal function.
  It's C++ Scripting+Behaviour system allows you to create VS-Projects, Compile and run code written in C++ at runtime. It basically exposes the whole Framework's API and you can do everything the framework does from within the C++ 'Scripts' (basically compiling and hot-loading DLLs at runtime).
  It also provides support for Reflection in C++ which allows Serailization and Showing member variables in the Inspector pannel's UI for a better manual debugging and testing experience!

  **UI example:**

```c++
  class DummyCamController:public Behaviour
  {
    Camera& m_cam;

    glm::vec3 pose;
    glm::vec3 angle;
    float fov;
    float far;

    public:

    //UI reflection
    AK_SHOW_IN_INSPECTOR
  	(
  			AK_ID_COMPX(pose)
  			AK_ID_COMPX(angle)
  			AK_ID_COMPX(fov)
  			AK_ID_COMPX(far)
  	)
    .......
    .......
   };
```


  **result**

  if you select the entity in the scene hierarchy to which the scriptable component (with behaviour shown above) has been attached to inside the Engine, you will see this UI in the inspector panel:

  ![alt text](https://github.com/SudharsanZen/SudharsanZen.github.io/blob/main/img/posts/inspector-scriptable.png?=raw=true)

## License

Akame is licensed under the Apache-2.0 License, see [LICENSE.md](https://github.com/SudharsanZen/Akame/blob/main/LICENSE.md) for more information.

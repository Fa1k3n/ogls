# ogls - An OpenGL shader loader library

A small helper library to allow to easy create and use OpenGL GLSL shaders. 

Example using a uniform shader:

```c++
	ogls::SolidColorShader frags(ogls::RED);
	auto prog = ogls::Program().addShader(frags).link();

	// ... do normal gl operations ...
	prog.use();
}
```

Adding source to shader is easily done either via a string

```c++
	auto frags = ogls::FragmentShader().addSource("
		int main() { 
			gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	");
```

or via an stream, e.g. file

```c++
	std::ifstream fragFile;
  	fragFile.open ("shaders/basic_uniform.frag");
  	auto frags = ogls::FragmentShader().addSource(fragFile);
   	fragFile.close();
```

The library provides easy templated access to uniforms

```c++
	auto frags = ogls::FragmentShader().addSource("
		uniform vec4 ourColor;
		int main() { 
			gl_FragColor = ourColor;
		}
	");

	auto prog = ogls::Program().addShader(frags.compile()).link();

	// ... do normal gl operations ...
	prog.use();
	prog.setUniform<float>("ourColor", {1.0f, 0.0f, 0.0f, 1.0f});

```

Error handling is done via C++ exceptions

```c++
	try {
		ogls::SolidColorShader frags(ogls::RED);
		auto prog = ogls::Program().addShader(frags).link();
	} catch ogls::ProgramException {

	} catch ogls::ShaderException {

	}
```
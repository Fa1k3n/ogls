# ogls - An OpenGL shader loader library

[![Build status](https://travis-ci.org/fa1k3n/ogls.svg?branch=master)](https://travis-ci.org/fa1k3n/ogls)
[![Coverage Status](https://coveralls.io/repos/github/fa1k3n/ogls/badge.svg?branch=master&service=github)](https://coveralls.io/github/fa1k3n/ogls?branch=master&service=github)
[![Documentation](https://img.shields.io/badge/docs-reference-blue.svg)](https://rawgit.com/fa1k3n/ogls/documentation/docs/html/index.html)

# Table of Contents

- [Overview](#overview)
- [Installation](#installation)
- [Usage](#usage)
- [Examples](#examples)

# Overview

A small helper library to allow to easy create and use OpenGL GLSL shaders. 
ogls provides:
* Super easy creating of uniform color shaders
* Run-time openGL shading error checking via exceptions
* Very easily create, build and use shaders in source 
* A clean interface to shader uniforms

# Installation

```
mkdir build && cd build
cmake ..
make
sudo make install
```

# Usage

With cmake just do

```cmake
find_package(ogls REQUIRED)

target_link_libraries(
	...
	ogls
	...
	)
```

in your CMakeLists.txt and you are set to go.

When linking manually link with libogls.so

# Examples

Creating and using a red shader

```c++
ogls::SolidColorShader frags(ogls::RED);
auto prog = ogls::Program().addShader(frags).link();

// ... do normal gl operations ...
prog.use();

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
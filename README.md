# ogls - An OpenGL shader loader library

A small helper library to allow to easy create and use OpenGL GLSL shaders. 

Example using a uniform shader:

```c++
#include <ogls/shader.hpp>
#include <ogls/colors.hpp>
#include <ogls/program.hpp>

int main(void) {
	// ... setup normal OpenGL bindings ...
	ogls::SolidColorShader frags(ogls::RED).compile();
	auto prog = ogls::Program().addShader(frags.link());

	// ... do normal gl operations and bind VAO ...
	prog.use();

}
```
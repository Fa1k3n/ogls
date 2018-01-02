#include "colors.hpp"

namespace ogls {
	static ColorRGB color_presets[] = { 
		{0.941f, 0.973f, 1.000f},
		{0.980f, 0.922f, 0.843f},
		{0.000f, 1.000f, 1.000f},
		{0.498f, 1.000f, 0.831f},
		{0.941f, 1.000f, 1.000f},
		{0.961f, 0.961f, 0.863f},
		{1.000f, 0.894f, 0.769f},
		{0.000f, 0.000f, 0.000f},
		{1.000f, 0.922f, 0.804f},
		{0.000f, 0.000f, 1.000f},
		{0.541f, 0.169f, 0.886f},
		{0.647f, 0.165f, 0.165f},
		{0.871f, 0.722f, 0.529f},
		{0.373f, 0.620f, 0.627f},
		{0.498f, 1.000f, 0.000f},
		{0.824f, 0.412f, 0.118f},
		{1.000f, 0.498f, 0.314f},
		{0.392f, 0.584f, 0.929f},
		{1.000f, 0.973f, 0.863f},
		{0.863f, 0.078f, 0.235f},
		{0.000f, 1.000f, 1.000f},
		{0.000f, 0.000f, 0.545f},
		{0.000f, 0.545f, 0.545f},
		{0.722f, 0.525f, 0.043f},
		{0.663f, 0.663f, 0.663f},
		{0.000f, 0.392f, 0.000f},
		{0.663f, 0.663f, 0.663f},
		{0.741f, 0.718f, 0.420f},
		{0.545f, 0.000f, 0.545f},
		{0.333f, 0.420f, 0.184f},
		{1.000f, 0.549f, 0.000f},
		{0.600f, 0.196f, 0.800f},
		{0.545f, 0.000f, 0.000f},
		{0.914f, 0.588f, 0.478f},
		{0.561f, 0.737f, 0.561f},
		{0.282f, 0.239f, 0.545f},
		{0.184f, 0.310f, 0.310f},
		{0.184f, 0.310f, 0.310f},
		{0.000f, 0.808f, 0.820f},
		{0.580f, 0.000f, 0.827f},
		{1.000f, 0.078f, 0.576f},
		{0.000f, 0.749f, 1.000f},
		{0.412f, 0.412f, 0.412f},
		{0.412f, 0.412f, 0.412f},
		{0.118f, 0.565f, 1.000f},
		{0.698f, 0.133f, 0.133f},
		{1.000f, 0.980f, 0.941f},
		{0.133f, 0.545f, 0.133f},
		{1.000f, 0.000f, 1.000f},
		{0.863f, 0.863f, 0.863f},
		{0.973f, 0.973f, 1.000f},
		{1.000f, 0.843f, 0.000f},
		{0.855f, 0.647f, 0.125f},
		{0.502f, 0.502f, 0.502f},
		{0.000f, 0.502f, 0.000f},
		{0.678f, 1.000f, 0.184f},
		{0.502f, 0.502f, 0.502f},
		{0.941f, 1.000f, 0.941f},
		{1.000f, 0.412f, 0.706f},
		{0.804f, 0.361f, 0.361f},
		{0.294f, 0.000f, 0.510f},
		{1.000f, 1.000f, 0.941f},
		{0.941f, 0.902f, 0.549f},
		{0.902f, 0.902f, 0.980f},
		{1.000f, 0.941f, 0.961f},
		{0.486f, 0.988f, 0.000f},
		{1.000f, 0.980f, 0.804f},
		{0.678f, 0.847f, 0.902f},
		{0.941f, 0.502f, 0.502f},
		{0.878f, 1.000f, 1.000f},
		{0.980f, 0.980f, 0.824f},
		{0.827f, 0.827f, 0.827f},
		{0.565f, 0.933f, 0.565f},
		{0.827f, 0.827f, 0.827f},
		{1.000f, 0.714f, 0.757f},
		{1.000f, 0.627f, 0.478f},
		{0.125f, 0.698f, 0.667f},
		{0.529f, 0.808f, 0.980f},
		{0.467f, 0.533f, 0.600f},
		{0.467f, 0.533f, 0.600f},
		{0.690f, 0.769f, 0.871f},
		{1.000f, 1.000f, 0.878f},
		{0.000f, 1.000f, 0.000f},
		{0.196f, 0.804f, 0.196f},
		{0.980f, 0.941f, 0.902f},
		{1.000f, 0.000f, 1.000f},
		{0.502f, 0.000f, 0.000f},
		{0.400f, 0.804f, 0.667f},
		{0.000f, 0.000f, 0.804f},
		{0.729f, 0.333f, 0.827f},
		{0.576f, 0.439f, 0.859f},
		{0.235f, 0.702f, 0.443f},
		{0.482f, 0.408f, 0.933f},
		{0.000f, 0.980f, 0.604f},
		{0.282f, 0.820f, 0.800f},
		{0.780f, 0.082f, 0.522f},
		{0.098f, 0.098f, 0.439f},
		{0.961f, 1.000f, 0.980f},
		{1.000f, 0.894f, 0.882f},
		{1.000f, 0.894f, 0.710f},
		{1.000f, 0.871f, 0.678f},
		{0.000f, 0.000f, 0.502f},
		{0.992f, 0.961f, 0.902f},
		{0.502f, 0.502f, 0.000f},
		{0.420f, 0.557f, 0.137f},
		{1.000f, 0.647f, 0.000f},
		{1.000f, 0.271f, 0.000f},
		{0.855f, 0.439f, 0.839f},
		{0.933f, 0.910f, 0.667f},
		{0.596f, 0.984f, 0.596f},
		{0.686f, 0.933f, 0.933f},
		{0.859f, 0.439f, 0.576f},
		{1.000f, 0.937f, 0.835f},
		{1.000f, 0.855f, 0.725f},
		{0.804f, 0.522f, 0.247f},
		{1.000f, 0.753f, 0.796f},
		{0.867f, 0.627f, 0.867f},
		{0.690f, 0.878f, 0.902f},
		{0.502f, 0.000f, 0.502f},
		{1.000f, 0.000f, 0.000f},
		{0.737f, 0.561f, 0.561f},
		{0.255f, 0.412f, 0.882f},
		{0.545f, 0.271f, 0.075f},
		{0.980f, 0.502f, 0.447f},
		{0.957f, 0.643f, 0.376f},
		{0.180f, 0.545f, 0.341f},
		{1.000f, 0.961f, 0.933f},
		{0.627f, 0.322f, 0.176f},
		{0.753f, 0.753f, 0.753f},
		{0.529f, 0.808f, 0.922f},
		{0.416f, 0.353f, 0.804f},
		{0.439f, 0.502f, 0.565f},
		{0.439f, 0.502f, 0.565f},
		{1.000f, 0.980f, 0.980f},
		{0.000f, 1.000f, 0.498f},
		{0.275f, 0.510f, 0.706f},
		{0.824f, 0.706f, 0.549f},
		{0.000f, 0.502f, 0.502f},
		{0.847f, 0.749f, 0.847f},
		{1.000f, 0.388f, 0.278f},
		{0.251f, 0.878f, 0.816f},
		{0.933f, 0.510f, 0.933f},
		{0.961f, 0.871f, 0.702f},
		{1.000f, 1.000f, 1.000f},
		{0.961f, 0.961f, 0.961f},
		{1.000f, 1.000f, 0.000f},
		{0.604f, 0.804f, 0.196f}	
	}; 
}
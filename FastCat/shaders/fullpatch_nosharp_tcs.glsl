#version 450 core


layout(std140, binding = 1) uniform cbPerLevel
{                                             // start_at    size    accum_size
	float g_TessFactor;                       // 0           4       4
	float g_TessFactorNextLevel;              // 4           4       8
	float g_fMaxTessFactor;                   // 8           4       12
	int g_Level;                              // 12          4       16
	int g_Offset;                             // 16          4       20
	vec4 g_ObjectColor;                       // 32          16      48
};


layout (vertices = 16) out;


void main()
{
	// tessfactors are set once per patch
	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = g_TessFactor;
		gl_TessLevelInner[1] = g_TessFactor;
		gl_TessLevelOuter[0] = g_TessFactor;
		gl_TessLevelOuter[1] = g_TessFactor;
		gl_TessLevelOuter[2] = g_TessFactor;
		gl_TessLevelOuter[3] = g_TessFactor;
	}
	
	// pass through control point position and texture coordinates
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
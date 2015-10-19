float4x4 world;
float4x4 view;
float4x4 proj;

texture tex;
sampler smp = sampler_state
{
	texture = < tex > ;
};

struct VsIn
{
	float3 pos : POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};

struct VsOut
{
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};

VsOut VS(VsIn in)
{
	VsOut out = (VsOut)0;
	float4 mat;

	mat = mul(mul(world, view), proj);

	out.pos = mul(float4(in.pos, 1.0f), mat);
	out.col = in.col;
	out.uv = in.uv;

	return out;
}

float4 PS(VsOut in) : COLOR0
{
	return tex2D(smp, in.uv) * in.col;
}

technique Tec
{
	pass p0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}

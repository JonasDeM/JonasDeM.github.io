//=============================================================================
//// Shader uses position and texture
//=============================================================================
int MiddleBlurOffset = 0;
int AmountSteps = 6;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State 
DepthStencilState EnabledDepth
{
    DepthEnable = TRUE;
};
/// Create Rasterizer State 
RasterizerState BackCulling 
{ 
    CullMode = BACK; 
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
    output.Position = float4(input.Position,1);
	// Set the TexCoord
    output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{

	float width;
	float height;
	gTexture.GetDimensions(width,height);

	float dx = 1.0f /width;
	float dy = 1.0f /height;

	float4 color = float4(0,0,0,1);

	float thisBlurOffset = MiddleBlurOffset;
	float maxTexCoordDistance = sqrt(0.5f);
	float distanceFromMiddleTexCoord = sqrt( pow(input.TexCoord.x - 0.5f,2) + pow(input.TexCoord.y - 0.5f,2) );

	float normalizedDistance = distanceFromMiddleTexCoord/ maxTexCoordDistance;

	for(int p = 1; p <= AmountSteps; p++)
	{
		if( normalizedDistance <= (float)p/(float)AmountSteps)
		{
			thisBlurOffset += p-1;
			break;
		}
	}

	for(int i = -thisBlurOffset; i <= thisBlurOffset; i++)
	{

		for(int p = -thisBlurOffset; p <= thisBlurOffset ; p++)
		{
			float2 newTexCoord = input.TexCoord;
			
			newTexCoord.x += dx * i;
			newTexCoord.y += dy * p;
			color += gTexture.SampleLevel(samPoint,newTexCoord,0);
		}
	}
	color = color/((2*thisBlurOffset+1)*(2*thisBlurOffset+1));

	return float4(color.x,color.y,color.z,1.0f);
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
		SetRasterizerState(BackCulling);
        SetDepthStencilState(EnabledDepth,0);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
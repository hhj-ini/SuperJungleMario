#include "UUi.h"

UUi::UUi(FVertexUI* UIVertex, DirectX::XMFLOAT2 NDCoord, DirectX::XMFLOAT4 rgba, float scale)
	:UIVertex(UIVertex)
	,NDCoord(NDCoord)
	,rgba(rgba)
	,scale(scale)
{
}

void UUi::Render(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight)
{
	renderer.UpdateUI(UUi::NDCoord, vertexBuffer, UIWidth, UIHeight, UUi::rgba);
	renderer.RenderUI(vertexBuffer, numVertices);
}
#include "UUi.h"

UUi::UUi(FVertexUI* UIVertex, FNDCoordinate NDCoord, float scale)
	:UIVertex(UIVertex)
	,NDCoord(NDCoord)
	,scale(scale)
{
}

void UUi::Render(URenderer& renderer, ID3D11Buffer* vertexBuffer, UINT numVertices, float UIWidth, float UIHeight)
{
	renderer.UpdateUI(UUi::NDCoord, vertexBuffer, UIWidth, UIHeight);
	renderer.RenderUI(vertexBuffer, numVertices);
}
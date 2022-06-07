#include "WorldTransform.h"

void WorldTransform::MatrixUpdate()
{
	matWorld_.MatrixUpdate
	(
		scale_,
		rotation_,
		translation_
	);
	TransferMatrix();
}
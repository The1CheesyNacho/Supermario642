void scroll_sr_dl_sr_dl_Plane_mesh_layer_1_mesh_001_mesh_layer_1_vtx_4() {
	int i = 0;
	int count = 4;
	int width = 32 * 0x20;
	int height = 32 * 0x20;

	static int currentX = 0;
	int deltaX;
	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(sr_dl_sr_dl_Plane_mesh_layer_1_mesh_001_mesh_layer_1_vtx_4);

	deltaX = (int)(1.0 * 0x20) % width;
	deltaY = (int)(1.0 * 0x20) % height;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}
	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
		vertices[i].n.tc[1] += deltaY;
	}
	currentX += deltaX;	currentY += deltaY;
}

void scroll_sr() {
	scroll_sr_dl_sr_dl_Plane_mesh_layer_1_mesh_001_mesh_layer_1_vtx_4();
};

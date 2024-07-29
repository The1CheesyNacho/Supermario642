void scroll_dl_sky_bob_mesh_vtx_3() {
	int i = 0;
	int count = 26;
	int width = 248 * 0x20;

	static int currentX = 0;
	int deltaX;
	Vtx *vertices = segmented_to_virtual(dl_sky_bob_mesh_vtx_3);

	deltaX = (int)(0.25 * 0x20) % width;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;
}

void scroll_gb_level_dl_dl() {
	scroll_dl_sky_bob_mesh_vtx_3();
};

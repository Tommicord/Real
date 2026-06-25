float Extrapolate2d(int xsb, int ysb, float dx, float dy) {
    int permIndex = GetPerm(xsb + GetPerm(ysb));
    int gradIndex = permIndex & 7;
    int gi = gradIndex * 2;
    float gradX = float(GRADIENTS_2D[gi]);
    float gradY = float(GRADIENTS_2D[gi + 1]);
    return gradX * dx + gradY * dy;
}

float Extrapolate3d(int xsb, int ysb, int zsb, float dx, float dy, float dz) {
    int permIndex = GetPerm(xsb + GetPerm(ysb + GetPerm(zsb)));
    int gradIndex = GetPermGradIndex3D(permIndex);
    int gi = gradIndex;
    float gradX = float(GRADIENTS_3D[gi]);
    float gradY = float(GRADIENTS_3D[gi + 1]);
    float gradZ = float(GRADIENTS_3D[gi + 2]);
    return gradX * dx + gradY * dy + gradZ * dz;
}
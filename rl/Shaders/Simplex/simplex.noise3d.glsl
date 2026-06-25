float Noise3D(float x, float y, float z) {
    // Place input coordinates on simplectic honeycomb
    float stretchOffset = (x + y + z) * STRETCH_3D;
    float xs = x + stretchOffset;
    float ys = y + stretchOffset;
    float zs = z + stretchOffset;

    // Floor to get simplectic honeycomb coordinates of rhombohedron (stretched cube) super-cell origin
    int xsb = int(floor(xs));
    int ysb = int(floor(ys));
    int zsb = int(floor(zs));

    // Skew out to get actual coordinates of rhombohedron origin
    float squishOffset = float(xsb + ysb + zsb) * SQUISH_3D;
    float xb = float(xsb) + squishOffset;
    float yb = float(ysb) + squishOffset;
    float zb = float(zsb) + squishOffset;

    // Compute simplectic honeycomb coordinates relative to rhombohedral origin
    float xins = xs - float(xsb);
    float yins = ys - float(ysb);
    float zins = zs - float(zsb);

    // Sum those together to get a value that determines which region we're in
    float inSum = xins + yins + zins;

    // Positions relative to origin point
    float dx0 = x - xb;
    float dy0 = y - yb;
    float dz0 = z - zb;

    // We'll be defining these inside the next block and using them afterwards
    float dx_ext0, dy_ext0, dz_ext0;
    float dx_ext1, dy_ext1, dz_ext1;
    int xsv_ext0, ysv_ext0, zsv_ext0;
    int xsv_ext1, ysv_ext1, zsv_ext1;

    float value = 0.0;

    if (inSum <= 1.0) {
        // We're inside the tetrahedron (3-Simplex) at (0,0,0)

        // Determine which two of (0,0,1), (0,1,0), (1,0,0) are closest
        int aPoint = 0x01;
        float aScore = xins;
        int bPoint = 0x02;
        float bScore = yins;
        if (aScore >= bScore && zins > bScore) {
            bScore = zins;
            bPoint = 0x04;
        } else if (aScore < bScore && zins > aScore) {
            aScore = zins;
            aPoint = 0x04;
        }

        // Now we determine the two lattice points not part of the tetrahedron that may contribute
        float wins = 1.0 - inSum;
        if (wins > aScore || wins > bScore) {
            // (0,0,0) is one of the closest two tetrahedral vertices
            int c = (bScore > aScore ? bPoint : aPoint);

            if ((c & 0x01) == 0) {
                xsv_ext0 = xsb - 1;
                xsv_ext1 = xsb;
                dx_ext0 = dx0 + 1.0;
                dx_ext1 = dx0;
            } else {
                xsv_ext0 = xsv_ext1 = xsb + 1;
                dx_ext0 = dx_ext1 = dx0 - 1.0;
            }

            if ((c & 0x02) == 0) {
                ysv_ext0 = ysv_ext1 = ysb;
                dy_ext0 = dy_ext1 = dy0;
                if ((c & 0x01) == 0) {
                    ysv_ext1 -= 1;
                    dy_ext1 += 1.0;
                } else {
                    ysv_ext0 -= 1;
                    dy_ext0 += 1.0;
                }
            } else {
                ysv_ext0 = ysv_ext1 = ysb + 1;
                dy_ext0 = dy_ext1 = dy0 - 1.0;
            }

            if ((c & 0x04) == 0) {
                zsv_ext0 = zsb;
                zsv_ext1 = zsb - 1;
                dz_ext0 = dz0;
                dz_ext1 = dz0 + 1.0;
            } else {
                zsv_ext0 = zsv_ext1 = zsb + 1;
                dz_ext0 = dz_ext1 = dz0 - 1.0;
            }
        } else {
            // (0,0,0) is not one of the closest two tetrahedral vertices
            int c = (aPoint | bPoint);

            if ((c & 0x01) == 0) {
                xsv_ext0 = xsb;
                xsv_ext1 = xsb - 1;
                dx_ext0 = dx0 - 2.0 * SQUISH_3D;
                dx_ext1 = dx0 + 1.0 - SQUISH_3D;
            } else {
                xsv_ext0 = xsv_ext1 = xsb + 1;
                dx_ext0 = dx0 - 1.0 - 2.0 * SQUISH_3D;
                dx_ext1 = dx0 - 1.0 - SQUISH_3D;
            }

            if ((c & 0x02) == 0) {
                ysv_ext0 = ysb;
                ysv_ext1 = ysb - 1;
                dy_ext0 = dy0 - 2.0 * SQUISH_3D;
                dy_ext1 = dy0 + 1.0 - SQUISH_3D;
            } else {
                ysv_ext0 = ysv_ext1 = ysb + 1;
                dy_ext0 = dy0 - 1.0 - 2.0 * SQUISH_3D;
                dy_ext1 = dy0 - 1.0 - SQUISH_3D;
            }

            if ((c & 0x04) == 0) {
                zsv_ext0 = zsb;
                zsv_ext1 = zsb - 1;
                dz_ext0 = dz0 - 2.0 * SQUISH_3D;
                dz_ext1 = dz0 + 1.0 - SQUISH_3D;
            } else {
                zsv_ext0 = zsv_ext1 = zsb + 1;
                dz_ext0 = dz0 - 1.0 - 2.0 * SQUISH_3D;
                dz_ext1 = dz0 - 1.0 - SQUISH_3D;
            }
        }

        // Contribution (0,0,0)
        float attn0 = 2.0 - dx0 * dx0 - dy0 * dy0 - dz0 * dz0;
        if (attn0 > 0.0) {
            attn0 *= attn0;
            value += attn0 * attn0 * Extrapolate3d(xsb + 0, ysb + 0, zsb + 0, dx0, dy0, dz0);
        }

        // Contribution (1,0,0)
        float dx1 = dx0 - 1.0 - SQUISH_3D;
        float dy1 = dy0 - 0.0 - SQUISH_3D;
        float dz1 = dz0 - 0.0 - SQUISH_3D;
        float attn1 = 2.0 - dx1 * dx1 - dy1 * dy1 - dz1 * dz1;
        if (attn1 > 0.0) {
            attn1 *= attn1;
            value += attn1 * attn1 * Extrapolate3d(xsb + 1, ysb + 0, zsb + 0, dx1, dy1, dz1);
        }

        // Contribution (0,1,0)
        float dx2 = dx0 - 0.0 - SQUISH_3D;
        float dy2 = dy0 - 1.0 - SQUISH_3D;
        float dz2 = dz1;
        float attn2 = 2.0 - dx2 * dx2 - dy2 * dy2 - dz2 * dz2;
        if (attn2 > 0.0) {
            attn2 *= attn2;
            value += attn2 * attn2 * Extrapolate3d(xsb + 0, ysb + 1, zsb + 0, dx2, dy2, dz2);
        }

        // Contribution (0,0,1)
        float dx3 = dx2;
        float dy3 = dy1;
        float dz3 = dz0 - 1.0 - SQUISH_3D;
        float attn3 = 2.0 - dx3 * dx3 - dy3 * dy3 - dz3 * dz3;
        if (attn3 > 0.0) {
            attn3 *= attn3;
            value += attn3 * attn3 * Extrapolate3d(xsb + 0, ysb + 0, zsb + 1, dx3, dy3, dz3);
        }
    } else if (inSum >= 2.0) {
        // We're inside the tetrahedron (3-Simplex) at (1,1,1)

        // Determine which two tetrahedral vertices are the closest, out of (1,1,0), (1,0,1), (0,1,1) but not (1,1,1)
        int aPoint = 0x06;
        float aScore = xins;
        int bPoint = 0x05;
        float bScore = yins;
        if (aScore <= bScore && zins < bScore) {
            bScore = zins;
            bPoint = 0x03;
        } else if (aScore > bScore && zins < aScore) {
            aScore = zins;
            aPoint = 0x03;
        }

        // Now we determine the two lattice points not part of the tetrahedron that may contribute
        float wins = 3.0 - inSum;
        if (wins < aScore || wins < bScore) {
            // (1,1,1) is one of the closest two tetrahedral vertices
            int c = (bScore < aScore ? bPoint : aPoint);

            if ((c & 0x01) != 0) {
                xsv_ext0 = xsb + 2;
                xsv_ext1 = xsb + 1;
                dx_ext0 = dx0 - 2.0 - 3.0 * SQUISH_3D;
                dx_ext1 = dx0 - 1.0 - 3.0 * SQUISH_3D;
            } else {
                xsv_ext0 = xsv_ext1 = xsb;
                dx_ext0 = dx_ext1 = dx0 - 3.0 * SQUISH_3D;
            }

            if ((c & 0x02) != 0) {
                ysv_ext0 = ysv_ext1 = ysb + 1;
                dy_ext0 = dy_ext1 = dy0 - 1.0 - 3.0 * SQUISH_3D;
                if ((c & 0x01) != 0) {
                    ysv_ext1 += 1;
                    dy_ext1 -= 1.0;
                } else {
                    ysv_ext0 += 1;
                    dy_ext0 -= 1.0;
                }
            } else {
                ysv_ext0 = ysv_ext1 = ysb;
                dy_ext0 = dy_ext1 = dy0 - 3.0 * SQUISH_3D;
            }

            if ((c & 0x04) != 0) {
                zsv_ext0 = zsb + 1;
                zsv_ext1 = zsb + 2;
                dz_ext0 = dz0 - 1.0 - 3.0 * SQUISH_3D;
                dz_ext1 = dz0 - 2.0 - 3.0 * SQUISH_3D;
            } else {
                zsv_ext0 = zsv_ext1 = zsb;
                dz_ext0 = dz_ext1 = dz0 - 3.0 * SQUISH_3D;
            }
        } else {
            // (1,1,1) is not one of the closest two tetrahedral vertices
            int c = (aPoint & bPoint);

            if ((c & 0x01) != 0) {
                xsv_ext0 = xsb + 1;
                xsv_ext1 = xsb + 2;
                dx_ext0 = dx0 - 1.0 - SQUISH_3D;
                dx_ext1 = dx0 - 2.0 - 2.0 * SQUISH_3D;
            } else {
                xsv_ext0 = xsv_ext1 = xsb;
                dx_ext0 = dx0 - SQUISH_3D;
                dx_ext1 = dx0 - 2.0 * SQUISH_3D;
            }

            if ((c & 0x02) != 0) {
                ysv_ext0 = ysb + 1;
                ysv_ext1 = ysb + 2;
                dy_ext0 = dy0 - 1.0 - SQUISH_3D;
                dy_ext1 = dy0 - 2.0 - 2.0 * SQUISH_3D;
            } else {
                ysv_ext0 = ysv_ext1 = ysb;
                dy_ext0 = dy0 - SQUISH_3D;
                dy_ext1 = dy0 - 2.0 * SQUISH_3D;
            }

            if ((c & 0x04) != 0) {
                zsv_ext0 = zsb + 1;
                zsv_ext1 = zsb + 2;
                dz_ext0 = dz0 - 1.0 - SQUISH_3D;
                dz_ext1 = dz0 - 2.0 - 2.0 * SQUISH_3D;
            } else {
                zsv_ext0 = zsv_ext1 = zsb;
                dz_ext0 = dz0 - SQUISH_3D;
                dz_ext1 = dz0 - 2.0 * SQUISH_3D;
            }
        }

        // Contribution (1,1,0)
        float dx3 = dx0 - 1.0 - 2.0 * SQUISH_3D;
        float dy3 = dy0 - 1.0 - 2.0 * SQUISH_3D;
        float dz3 = dz0 - 0.0 - 2.0 * SQUISH_3D;
        float attn3 = 2.0 - dx3 * dx3 - dy3 * dy3 - dz3 * dz3;
        if (attn3 > 0.0) {
            attn3 *= attn3;
            value += attn3 * attn3 * Extrapolate3d(xsb + 1, ysb + 1, zsb + 0, dx3, dy3, dz3);
        }

        // Contribution (1,0,1)
        float dx2 = dx3;
        float dy2 = dy0 - 0.0 - 2.0 * SQUISH_3D;
        float dz2 = dz0 - 1.0 - 2.0 * SQUISH_3D;
        float attn2 = 2.0 - dx2 * dx2 - dy2 * dy2 - dz2 * dz2;
        if (attn2 > 0.0) {
            attn2 *= attn2;
            value += attn2 * attn2 * Extrapolate3d(xsb + 1, ysb + 0, zsb + 1, dx2, dy2, dz2);
        }

        // Contribution (0,1,1)
        float dx1 = dx0 - 0.0 - 2.0 * SQUISH_3D;
        float dy1 = dy3;
        float dz1 = dz2;
        float attn1 = 2.0 - dx1 * dx1 - dy1 * dy1 - dz1 * dz1;
        if (attn1 > 0.0) {
            attn1 *= attn1;
            value += attn1 * attn1 * Extrapolate3d(xsb + 0, ysb + 1, zsb + 1, dx1, dy1, dz1);
        }

        // Contribution (1,1,1)
        dx0 = dx0 - 1.0 - 3.0 * SQUISH_3D;
        dy0 = dy0 - 1.0 - 3.0 * SQUISH_3D;
        dz0 = dz0 - 1.0 - 3.0 * SQUISH_3D;
        float attn0 = 2.0 - dx0 * dx0 - dy0 * dy0 - dz0 * dz0;
        if (attn0 > 0.0) {
            attn0 *= attn0;
            value += attn0 * attn0 * Extrapolate3d(xsb + 1, ysb + 1, zsb + 1, dx0, dy0, dz0);
        }
    } else {
        // We're inside the octahedron (Rectified 3-Simplex) in between
        float aScore;
        int aPoint;
        bool aIsFurtherSide;
        float bScore;
        int bPoint;
        bool bIsFurtherSide;

        // Decide between point (0,0,1) and (1,1,0) as closest
        float p1 = xins + yins;
        if (p1 > 1.0) {
            aScore = 1.0 - p1;
            aPoint = 0x03;
            aIsFurtherSide = true;
        } else {
            aScore = p1;
            aPoint = 0x04;
            aIsFurtherSide = false;
        }

        // Decide between point (0,1,0) and (1,0,1) as closest
        float p2 = xins + zins;
        if (p2 > 1.0) {
            bScore = 1.0 - p2;
            bPoint = 0x05;
            bIsFurtherSide = true;
        } else {
            bScore = p2;
            bPoint = 0x02;
            bIsFurtherSide = false;
        }

        // The closest out of the two (1,0,0) and (0,1,1) will replace the furthest out of the two decided above, if closer
        float p3 = yins + zins;
        if (p3 > 1.0) {
            float score = 1.0 - p3;
            if (aScore <= bScore && aScore < score) {
                aScore = score;
                aPoint = 0x06;
                aIsFurtherSide = true;
            } else if (aScore > bScore && bScore < score) {
                bScore = score;
                bPoint = 0x06;
                bIsFurtherSide = true;
            }
        } else {
            float score = p3;
            if (aScore <= bScore && aScore < score) {
                aScore = score;
                aPoint = 0x01;
                aIsFurtherSide = false;
            } else if (aScore > bScore && bScore < score) {
                bScore = score;
                bPoint = 0x01;
                bIsFurtherSide = false;
            }
        }

        // Where each of the two closest points are determines how the extra two vertices are calculated
        if (aIsFurtherSide == bIsFurtherSide) {
            if (aIsFurtherSide) {
                // Both closest points on (1,1,1) side
                dx_ext0 = dx0 - 1.0 - 3.0 * SQUISH_3D;
                dy_ext0 = dy0 - 1.0 - 3.0 * SQUISH_3D;
                dz_ext0 = dz0 - 1.0 - 3.0 * SQUISH_3D;
                xsv_ext0 = xsb + 1;
                ysv_ext0 = ysb + 1;
                zsv_ext0 = zsb + 1;

                int c = (aPoint & bPoint);
                if ((c & 0x01) != 0) {
                    dx_ext1 = dx0 - 2.0 - 2.0 * SQUISH_3D;
                    dy_ext1 = dy0 - 2.0 * SQUISH_3D;
                    dz_ext1 = dz0 - 2.0 * SQUISH_3D;
                    xsv_ext1 = xsb + 2;
                    ysv_ext1 = ysb;
                    zsv_ext1 = zsb;
                } else if ((c & 0x02) != 0) {
                    dx_ext1 = dx0 - 2.0 * SQUISH_3D;
                    dy_ext1 = dy0 - 2.0 - 2.0 * SQUISH_3D;
                    dz_ext1 = dz0 - 2.0 * SQUISH_3D;
                    xsv_ext1 = xsb;
                    ysv_ext1 = ysb + 2;
                    zsv_ext1 = zsb;
                } else {
                    dx_ext1 = dx0 - 2.0 * SQUISH_3D;
                    dy_ext1 = dy0 - 2.0 * SQUISH_3D;
                    dz_ext1 = dz0 - 2.0 - 2.0 * SQUISH_3D;
                    xsv_ext1 = xsb;
                    ysv_ext1 = ysb;
                    zsv_ext1 = zsb + 2;
                }
            } else {
                // Both closest points on (0,0,0) side
                dx_ext0 = dx0;
                dy_ext0 = dy0;
                dz_ext0 = dz0;
                xsv_ext0 = xsb;
                ysv_ext0 = ysb;
                zsv_ext0 = zsb;

                int c = (aPoint | bPoint);
                if ((c & 0x01) == 0) {
                    dx_ext1 = dx0 + 1.0 - SQUISH_3D;
                    dy_ext1 = dy0 - 1.0 - SQUISH_3D;
                    dz_ext1 = dz0 - 1.0 - SQUISH_3D;
                    xsv_ext1 = xsb - 1;
                    ysv_ext1 = ysb + 1;
                    zsv_ext1 = zsb + 1;
                } else if ((c & 0x02) == 0) {
                    dx_ext1 = dx0 - 1.0 - SQUISH_3D;
                    dy_ext1 = dy0 + 1.0 - SQUISH_3D;
                    dz_ext1 = dz0 - 1.0 - SQUISH_3D;
                    xsv_ext1 = xsb + 1;
                    ysv_ext1 = ysb - 1;
                    zsv_ext1 = zsb + 1;
                } else {
                    dx_ext1 = dx0 - 1.0 - SQUISH_3D;
                    dy_ext1 = dy0 - 1.0 - SQUISH_3D;
                    dz_ext1 = dz0 + 1.0 - SQUISH_3D;
                    xsv_ext1 = xsb + 1;
                    ysv_ext1 = ysb + 1;
                    zsv_ext1 = zsb - 1;
                }
            }
        } else {
            // One point on (0,0,0) side, one point on (1,1,1) side
            int c1, c2;
            if (aIsFurtherSide) {
                c1 = aPoint;
                c2 = bPoint;
            } else {
                c1 = bPoint;
                c2 = aPoint;
            }

            // One contribution is a permutation of (1,1,-1)
            if ((c1 & 0x01) == 0) {
                dx_ext0 = dx0 + 1.0 - SQUISH_3D;
                dy_ext0 = dy0 - 1.0 - SQUISH_3D;
                dz_ext0 = dz0 - 1.0 - SQUISH_3D;
                xsv_ext0 = xsb - 1;
                ysv_ext0 = ysb + 1;
                zsv_ext0 = zsb + 1;
            } else if ((c1 & 0x02) == 0) {
                dx_ext0 = dx0 - 1.0 - SQUISH_3D;
                dy_ext0 = dy0 + 1.0 - SQUISH_3D;
                dz_ext0 = dz0 - 1.0 - SQUISH_3D;
                xsv_ext0 = xsb + 1;
                ysv_ext0 = ysb - 1;
                zsv_ext0 = zsb + 1;
            } else {
                dx_ext0 = dx0 - 1.0 - SQUISH_3D;
                dy_ext0 = dy0 - 1.0 - SQUISH_3D;
                dz_ext0 = dz0 + 1.0 - SQUISH_3D;
                xsv_ext0 = xsb + 1;
                ysv_ext0 = ysb + 1;
                zsv_ext0 = zsb - 1;
            }

            // One contribution is a permutation of (0,0,2)
            dx_ext1 = dx0 - 2.0 * SQUISH_3D;
            dy_ext1 = dy0 - 2.0 * SQUISH_3D;
            dz_ext1 = dz0 - 2.0 * SQUISH_3D;
            xsv_ext1 = xsb;
            ysv_ext1 = ysb;
            zsv_ext1 = zsb;
            if ((c2 & 0x01) != 0) {
                dx_ext1 -= 2.0;
                xsv_ext1 += 2;
            } else if ((c2 & 0x02) != 0) {
                dy_ext1 -= 2.0;
                ysv_ext1 += 2;
            } else {
                dz_ext1 -= 2.0;
                zsv_ext1 += 2;
            }
        }

        // Contribution (1,0,0)
        float dx1 = dx0 - 1.0 - SQUISH_3D;
        float dy1 = dy0 - 0.0 - SQUISH_3D;
        float dz1 = dz0 - 0.0 - SQUISH_3D;
        float attn1 = 2.0 - dx1 * dx1 - dy1 * dy1 - dz1 * dz1;
        if (attn1 > 0.0) {
            attn1 *= attn1;
            value += attn1 * attn1 * Extrapolate3d(xsb + 1, ysb + 0, zsb + 0, dx1, dy1, dz1);
        }

        // Contribution (0,1,0)
        float dx2 = dx0 - 0.0 - SQUISH_3D;
        float dy2 = dy0 - 1.0 - SQUISH_3D;
        float dz2 = dz1;
        float attn2 = 2.0 - dx2 * dx2 - dy2 * dy2 - dz2 * dz2;
        if (attn2 > 0.0) {
            attn2 *= attn2;
            value += attn2 * attn2 * Extrapolate3d(xsb + 0, ysb + 1, zsb + 0, dx2, dy2, dz2);
        }

        // Contribution (0,0,1)
        float dx3 = dx2;
        float dy3 = dy1;
        float dz3 = dz0 - 1.0 - SQUISH_3D;
        float attn3 = 2.0 - dx3 * dx3 - dy3 * dy3 - dz3 * dz3;
        if (attn3 > 0.0) {
            attn3 *= attn3;
            value += attn3 * attn3 * Extrapolate3d(xsb + 0, ysb + 0, zsb + 1, dx3, dy3, dz3);
        }

        // Contribution (1,1,0)
        float dx4 = dx0 - 1.0 - 2.0 * SQUISH_3D;
        float dy4 = dy0 - 1.0 - 2.0 * SQUISH_3D;
        float dz4 = dz0 - 0.0 - 2.0 * SQUISH_3D;
        float attn4 = 2.0 - dx4 * dx4 - dy4 * dy4 - dz4 * dz4;
        if (attn4 > 0.0) {
            attn4 *= attn4;
            value += attn4 * attn4 * Extrapolate3d(xsb + 1, ysb + 1, zsb + 0, dx4, dy4, dz4);
        }

        // Contribution (1,0,1)
        float dx5 = dx4;
        float dy5 = dy0 - 0.0 - 2.0 * SQUISH_3D;
        float dz5 = dz0 - 1.0 - 2.0 * SQUISH_3D;
        float attn5 = 2.0 - dx5 * dx5 - dy5 * dy5 - dz5 * dz5;
        if (attn5 > 0.0) {
            attn5 *= attn5;
            value += attn5 * attn5 * Extrapolate3d(xsb + 1, ysb + 0, zsb + 1, dx5, dy5, dz5);
        }

        // Contribution (0,1,1)
        float dx6 = dx0 - 0.0 - 2.0 * SQUISH_3D;
        float dy6 = dy4;
        float dz6 = dz5;
        float attn6 = 2.0 - dx6 * dx6 - dy6 * dy6 - dz6 * dz6;
        if (attn6 > 0.0) {
            attn6 *= attn6;
            value += attn6 * attn6 * Extrapolate3d(xsb + 0, ysb + 1, zsb + 1, dx6, dy6, dz6);
        }
    }

    // First extra vertex
    float attn_ext0 = 2.0 - dx_ext0 * dx_ext0 - dy_ext0 * dy_ext0 - dz_ext0 * dz_ext0;
    if (attn_ext0 > 0.0) {
        attn_ext0 *= attn_ext0;
        value += attn_ext0 * attn_ext0 * Extrapolate3d(xsv_ext0, ysv_ext0, zsv_ext0, dx_ext0, dy_ext0, dz_ext0);
    }

    // Second extra vertex
    float attn_ext1 = 2.0 - dx_ext1 * dx_ext1 - dy_ext1 * dy_ext1 - dz_ext1 * dz_ext1;
    if (attn_ext1 > 0.0) {
        attn_ext1 *= attn_ext1;
        value += attn_ext1 * attn_ext1 * Extrapolate3d(xsv_ext1, ysv_ext1, zsv_ext1, dx_ext1, dy_ext1, dz_ext1);
    }

    return value / NORM_3D;
}

#include "Torus.h"
#include <glm/glm.hpp>
#include <cmath>
#include <limits>
#include <algorithm>
#include <vector>


const float EPSILON = 1e-4f;


std::vector<float> solveQuartic(float A, float B, float C, float D, float E) {
    std::vector<float> roots;


    if (fabs(A) < EPSILON) {
 
        if (fabs(B) < EPSILON) {
            if (fabs(C) < EPSILON) {
                if (fabs(D) < EPSILON) return roots;
                roots.push_back(-E / D); // Linear
                return roots;
            }
            float delta = D * D - 4 * C * E;
            if (delta >= 0) {
                float sqrtDelta = sqrt(delta);
                roots.push_back((-D + sqrtDelta) / (2 * C));
                roots.push_back((-D - sqrtDelta) / (2 * C));
            }
            return roots;
        }

        return roots;
    }

    // Normalize: divide by A
    float a = B / A;
    float b = C / A;
    float c = D / A;
    float d = E / A;

    // Use std::complex to solve quartic
    std::complex<float> i(0, 1);

    // Convert to depressed quartic using Ferrari's method
    float a2 = a * a;
    float p = -3.0f / 8.0f * a2 + b;
    float q = a2 * a / 8.0f - a * b / 2.0f + c;
    float r = -3.0f / 256.0f * a2 * a2 + a2 * b / 16.0f - a * c / 4.0f + d;

    std::complex<float> delta0 = p * p - 4.0f * r;
    std::complex<float> delta1 = 2.0f * p * q - 8.0f * r - q * q;

    std::complex<float> Q = std::sqrt((delta1 + std::sqrt(delta1 * delta1 - 4.0f * delta0 * delta0)) / 2.0f);
    if (Q == std::complex<float>(0.0f, 0.0f)) return roots;

    std::complex<float> S = 0.5f * std::sqrt(-2.0f / 3.0f * p + (Q + delta0 / Q) / (3.0f));
    std::complex<float> temp1 = -4.0f * S * S - 2.0f * p + q / S;
    std::complex<float> temp2 = -4.0f * S * S - 2.0f * p - q / S;

    std::vector<std::complex<float>> complexRoots = {
        -a / 4.0f - S + 0.5f * std::sqrt(temp1),
        -a / 4.0f - S - 0.5f * std::sqrt(temp1),
        -a / 4.0f + S + 0.5f * std::sqrt(temp2),
        -a / 4.0f + S - 0.5f * std::sqrt(temp2)
    };

    for (const auto& z : complexRoots) {
        if (fabs(z.imag()) < EPSILON) {
            roots.push_back(z.real());
        }
    }

    return roots;
}

float Torus::intersect(glm::vec3 p0, glm::vec3 dir) {
    glm::vec3 p = p0 - center;

    float dx = dir.x, dy = dir.y, dz = dir.z;
    float x = p.x, y = p.y, z = p.z;

    float R2 = majorRadius * majorRadius;
    float r2 = tubeRadius * tubeRadius;

    float G = x * x + y * y + z * z + R2 - r2;
    float k = dx * dx + dy * dy + dz * dz;
    float L = x * dx + y * dy + z * dz;

    float A = k * k;
    float B = 4.0f * k * L;
    float C = 2.0f * k * G + 4.0f * L * L - 4.0f * R2 * (dx * dx + dz * dz);
    float D = 4.0f * L * G - 8.0f * R2 * (x * dx + z * dz);
    float E = G * G - 4.0f * R2 * (x * x + z * z);

    std::vector<float> roots = solveQuartic(A, B, C, D, E);

    float tMin = std::numeric_limits<float>::max();
    for (float t : roots) {
        if (t > 0.001f && t < tMin) {
            tMin = t;
        }
    }

    return (tMin == std::numeric_limits<float>::max()) ? -1.0f : tMin;
}


glm::vec3 Torus::normal(glm::vec3 p) {
    glm::vec3 localP = p - center;
    float x = localP.x, y = localP.y, z = localP.z;

    float R = majorRadius;

    float sum = x * x + y * y + z * z;
    float scale = sum - (R * R + tubeRadius * tubeRadius);

    glm::vec3 grad;
    grad.x = 4.0f * x * scale - 8.0f * R * R * x;
    grad.y = 4.0f * y * scale;
    grad.z = 4.0f * z * scale - 8.0f * R * R * z;

    return glm::normalize(grad);
}

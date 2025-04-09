#include "ObjLoader.h"

#include "../core/Utils.h"
#include <fstream>
#include <iostream>

namespace renderer {
std::optional<Vector4> ObjLoader::parseVertex(std::string_view line) {
    std::istringstream iss(std::string(line.substr(2)));
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        return std::nullopt;
    }
    return Vector4{ x, y, z, 1.0f };
}

std::optional<Vector4> ObjLoader::parseNormal(std::string_view line) {
    std::istringstream iss(std::string(line.substr(3)));
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        return std::nullopt;
    }
    return Vector4{ x, y, z, 0.0f };
}

std::vector<std::array<int, 3>> ObjLoader::parseFace(std::string_view line) {
    std::istringstream iss(std::string(line.substr(2)));
    std::vector<std::array<int, 3>> faceIndices;
    std::string token;
    while (iss >> token) {
        std::istringstream tokenStream(token);
        std::string vStr, vtStr, vnStr;
        std::getline(tokenStream, vStr, '/');
        std::getline(tokenStream, vtStr, '/');
        std::getline(tokenStream, vnStr, '/');
        int vIndex  = std::stoi(vStr) - 1;
        int vtIndex = vtStr.empty() ? -1 : std::stoi(vtStr) - 1;
        int vnIndex = vnStr.empty() ? -1 : std::stoi(vnStr) - 1;
        faceIndices.push_back({ vIndex, vtIndex, vnIndex });
    }
    return faceIndices;
}

std::optional<Object> ObjLoader::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "File opening error: " << filename << "\n";
        return std::nullopt;
    }

    std::vector<Vector4> positions;
    std::vector<Vector4> normals;

    Object obj;

    std::string line;
    while (std::getline(file, line)) {
        std::string_view sv(line);
        while (!sv.empty() && std::isspace(sv.front())) {
            sv.remove_prefix(1);
        }
        if (sv.empty() || sv.front() == '#') {
            continue;
        }

        if (sv.starts_with("v ")) {
            if (auto posOpt = parseVertex(sv)) {
                positions.push_back(*posOpt);
            }
        } else if (sv.starts_with("vn ")) {
            if (auto normOpt = parseNormal(sv)) {
                normals.push_back(*normOpt);
            }
        } else if (sv.starts_with("f ")) {
            auto faceIndices = parseFace(sv);
            std::vector<Vertex> faceVertices;
            for (const auto& [vIndex, vtIndex, vnIndex] : faceIndices) {
                if (vIndex < 0 || static_cast<size_t>(vIndex) >= positions.size()) {
                    std::cerr << "Invalid vertex index: " << vIndex + 1 << "\n";
                    continue;
                }
                Vector4 pos = positions[vIndex];
                Vector4 norm;
                if (vnIndex >= 0 && static_cast<size_t>(vnIndex) < normals.size()) {
                    norm = normals[vnIndex];
                } else {
                    norm = Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
                }
                faceVertices.emplace_back(pos, norm);
            }
            if (faceVertices.size() < 3) {
                continue;
            }
            std::vector<Triangle> triangles;
            if (faceVertices.size() == 3) {
                triangles.emplace_back(faceVertices[0], faceVertices[1],
                                       faceVertices[2]);
            } else {
                triangles = triangulatePolygon<Triangle, Vertex>(faceVertices);
            }
            for (const auto& tri : triangles) {
                obj.addTriangle(tri);
            }
        }
    }
    file.close();
    return obj;
}
} // namespace renderer

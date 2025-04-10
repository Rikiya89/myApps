#include "ofApp.h"

void ofApp::computeMeshNormals(ofMesh& mesh) {
    vector<glm::vec3> normals(mesh.getVertices().size(), glm::vec3(0, 0, 0));
    const vector<ofIndexType>& indices = mesh.getIndices();
    const vector<glm::vec3>& vertices = mesh.getVertices();

    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        int ia = indices[i];
        int ib = indices[i + 1];
        int ic = indices[i + 2];

        glm::vec3 edge1 = vertices[ib] - vertices[ia];
        glm::vec3 edge2 = vertices[ic] - vertices[ia];
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        normals[ia] += normal;
        normals[ib] += normal;
        normals[ic] += normal;
    }

    for (auto& n : normals) {
        n = glm::normalize(n);
    }

    mesh.clearNormals();
    mesh.addNormals(normals);
}

void ofApp::setup() {
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableLighting();
    ofSetSmoothLighting(true);

    // Load and prepare image
    flowerImg.load("flower.jpg");
    flowerImg.resize(resolutionX, resolutionY);

    // Light settings
    ambientLight.setup();
    ambientLight.setAmbientColor(ofFloatColor(0.3, 0.3, 0.3));
    ambientLight.enable();

    directionalLight.setup();
    directionalLight.setDirectional();
    directionalLight.setDiffuseColor(ofFloatColor(0.8, 0.8, 0.9));
    directionalLight.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    directionalLight.setOrientation(glm::vec3(45, 45, 0));
    directionalLight.enable();

    // Material setup
    material.setShininess(100);
    material.setSpecularColor(ofColor(255, 255, 255));

    // Build mesh
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mesh.enableColors();
    mesh.enableNormals();

    for (int y = 0; y < resolutionY; ++y) {
        for (int x = 0; x < resolutionX; ++x) {
            ofColor color = flowerImg.getColor(x, y);
            float brightness = color.getBrightness() / 255.0;
            float z = brightness * heightScale;

            glm::vec3 pos(x * meshScale, y * meshScale, z);
            mesh.addVertex(pos);
            mesh.addColor(color);
        }
    }

    for (int y = 0; y < resolutionY - 1; ++y) {
        for (int x = 0; x < resolutionX - 1; ++x) {
            int i1 = x + y * resolutionX;
            int i2 = (x + 1) + y * resolutionX;
            int i3 = x + (y + 1) * resolutionX;
            int i4 = (x + 1) + (y + 1) * resolutionX;

            mesh.addTriangle(i1, i2, i3);
            mesh.addTriangle(i2, i4, i3);
        }
    }

    computeMeshNormals(mesh);
}

void ofApp::update() {
    float time = ofGetElapsedTimef();

    for (int y = 0; y < resolutionY; ++y) {
        for (int x = 0; x < resolutionX; ++x) {
            int i = x + y * resolutionX;
            ofColor baseColor = flowerImg.getColor(x, y);

            float brightness = baseColor.getBrightness() / 255.0;
            float wave = sin(time + x * 0.1f + y * 0.1f) * 8.0f;
            float radial = sin(glm::distance(glm::vec2(x, y), glm::vec2(resolutionX/2, resolutionY/2)) * 0.15f - time * 2.0f) * 5.0f;

            float z = brightness * heightScale + wave + radial;

            mesh.setVertex(i, glm::vec3(x * meshScale, y * meshScale, z));

            float hueShift = fmod((baseColor.getHueAngle() + time * 10), 360);
            ofColor animatedColor = ofColor::fromHsb(ofMap(hueShift, 0, 360, 0, 255), baseColor.getSaturation(), baseColor.getBrightness());
            mesh.setColor(i, animatedColor);
        }
    }

    computeMeshNormals(mesh);
}

void ofApp::draw() {
    cam.begin();

    ofPushMatrix();
    ofTranslate(-resolutionX * meshScale / 2, -resolutionY * meshScale / 2, 0);
    ofRotateDeg(ofGetElapsedTimef() * 5, 1, 0.3, 0.2);

    material.begin();
    mesh.draw();
    material.end();

    ofPopMatrix();
    cam.end();
}

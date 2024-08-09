#include "ofApp.h"

void ofApp::setup(){
    // Load the image
    roseImage.load("rose.jpg");
    
    // Create a grid mesh
    int width = 200;
    int height = 200;
    float spacing = 5.0;

    mesh.setMode(OF_PRIMITIVE_TRIANGLES);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate vertex position
            glm::vec3 pos(x * spacing, y * spacing, 0);
            mesh.addVertex(pos);
            
            // Calculate texture coordinate
            glm::vec2 texCoord(x * (roseImage.getWidth() / width),
                               y * (roseImage.getHeight() / height));
            mesh.addTexCoord(texCoord);
        }
    }

    // Create indices for the triangles
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            int i1 = x + y * width;
            int i2 = (x + 1) + y * width;
            int i3 = x + (y + 1) * width;
            int i4 = (x + 1) + (y + 1) * width;
            
            mesh.addIndex(i1);
            mesh.addIndex(i2);
            mesh.addIndex(i3);
            
            mesh.addIndex(i2);
            mesh.addIndex(i4);
            mesh.addIndex(i3);
        }
    }
    
    // Initialize time for animation
    time = 0;
    
    // Setup light
    light.setup();
    light.setPosition(500, 500, 500);
    light.enable();
    
    // Setup GUI
    gui.setup();
    gui.add(waveFrequency.set("Wave Frequency", 0.05, 0.01, 0.2));
    gui.add(waveAmplitude.set("Wave Amplitude", 50, 10, 100));
    gui.add(rotationSpeed.set("Rotation Speed", 0.1, 0.01, 1.0));
    gui.add(showWireframe.set("Show Wireframe", false));
}

void ofApp::update(){
    // Update time
    time += ofGetLastFrameTime();
    
    // Manipulate vertices
    for (int i = 0; i < mesh.getNumVertices(); i++) {
        glm::vec3 vertex = mesh.getVertex(i);
        
        // Create a more complex wave effect
        float distance = glm::length(glm::vec2(vertex.x - 500, vertex.y - 500));
        vertex.z = waveAmplitude * sin(time * rotationSpeed + vertex.x * waveFrequency) * cos(time * rotationSpeed + vertex.y * waveFrequency) - distance * 0.05;
        mesh.setVertex(i, vertex);
    }
}

void ofApp::draw(){
    ofBackground(0);
    
    // Enable depth testing
    ofEnableDepthTest();

    // Start camera
    cam.begin();
    
    // Apply rotation
    ofPushMatrix();
    ofRotateYDeg(ofGetElapsedTimef() * rotationSpeed * 10);
    
    // Bind the texture
    roseImage.bind();
    
    // Draw the mesh
    if (showWireframe) {
        mesh.drawWireframe();
    } else {
        mesh.draw();
    }
    
    // Unbind the texture
    roseImage.unbind();

    ofPopMatrix();
    
    // End camera
    cam.end();
    
    // Disable depth testing
    ofDisableDepthTest();

    // Draw GUI
    gui.draw();
}

void ofApp::keyPressed(int key) {
    if (key == 'w') {
        showWireframe = !showWireframe;
    }
}

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Load the image
    image.load("blue.jpeg");

    // Create a mesh
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    // Define the width and height of the mesh
    int width = image.getWidth();
    int height = image.getHeight();

    // Create vertices and add them to the mesh
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            ofColor color = image.getColor(x, y);
            float brightness = color.getBrightness();
            // Use brightness to create a height map
            float z = ofMap(brightness, 0, 255, -100, 100);
            mesh.addVertex(glm::vec3(x, y, z)); // Add vertex with a z value
            mesh.addColor(color); // Add color to the vertex
        }
    }

    // Create indices for the mesh
    for(int y = 0; y < height-1; y++){
        for(int x = 0; x < width-1; x++){
            int i1 = x + y * width;
            int i2 = (x+1) + y * width;
            int i3 = x + (y+1) * width;
            int i4 = (x+1) + (y+1) * width;
            mesh.addIndex(i1);
            mesh.addIndex(i2);
            mesh.addIndex(i3);
            mesh.addIndex(i2);
            mesh.addIndex(i4);
            mesh.addIndex(i3);
        }
    }

    // Set up lighting
    light.setup();
    light.setPosition(0, 0, 500);
}

//--------------------------------------------------------------
void ofApp::update(){
    // Animate vertices with a wave-like deformation
    float time = ofGetElapsedTimef();
    for (int i = 0; i < mesh.getNumVertices(); i++) {
        glm::vec3 vertex = mesh.getVertex(i);
        float displacement = sin(vertex.x * 0.05 + time) * 10.0;
        vertex.z += displacement;
        mesh.setVertex(i, vertex);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);

    // Enable depth testing for 3D rendering
    ofEnableDepthTest();

    // Enable lighting
    light.enable();

    // Move the camera and rotate the mesh
    cam.begin();
    ofPushMatrix();
    ofTranslate(-image.getWidth() / 2, -image.getHeight() / 2, 0);
    ofRotateYDeg(ofGetElapsedTimef() * 10); // Rotate the mesh
    mesh.draw();
    ofPopMatrix();
    cam.end();

    // Disable lighting
    light.disable();

    // Disable depth testing
    ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

/*
 * =====================================================================
 * SIMULASI RUANGAN 3D - PROYEK GRAFIKA KOMPUTER
 * =====================================================================
 * 
 * Deskripsi: Program simulasi visual ruang kerja 3D menggunakan OpenGL
 * 
 * Fitur:
 * - Model ruangan 3D lengkap (lantai, dinding, pintu, jendela)
 * - Perabot: meja, kursi, lemari, lampu, komputer
 * - Pencahayaan Phong (ambient, diffuse, specular)
 * - Transformasi geometris (translasi, rotasi, skala)
 * - Kontrol kamera bebas (WASD + Mouse)
 * - Multiple view angles
 * 
 * Kontrol:
 * - WASD      : Gerak kamera
 * - Mouse     : Rotasi kamera (pandangan)
 * - Scroll    : Zoom in/out
 * - 1-4       : Preset sudut pandang
 * - L         : Toggle lampu on/off
 * - ESC       : Keluar program
 * 
 * =====================================================================
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

// =====================================================================
// KONFIGURASI WINDOW
// =====================================================================
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// =====================================================================
// VARIABEL KAMERA
// =====================================================================
glm::vec3 cameraPos   = glm::vec3(0.0f, 2.0f, 8.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw   = -90.0f;
float pitch = 0.0f;
float fov   = 45.0f;

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// =====================================================================
// VARIABEL TIMING
// =====================================================================
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// =====================================================================
// VARIABEL PENCAHAYAAN
// =====================================================================
glm::vec3 lightPos(0.0f, 3.5f, 0.0f);  // Posisi lampu di tengah langit-langit
glm::vec3 lightColor(1.0f, 0.95f, 0.8f);  // Warna cahaya warm white
bool lightOn = true;

// =====================================================================
// FUNGSI DEKLARASI
// =====================================================================
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadShader(const char* vertexPath, const char* fragmentPath);
std::string readShaderFile(const char* filePath);

// =====================================================================
// STRUKTUR DATA VERTEX
// =====================================================================
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

// =====================================================================
// KELAS UNTUK MEMBUAT OBJEK 3D
// =====================================================================
class Object3D {
public:
    unsigned int VAO, VBO;
    std::vector<Vertex> vertices;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    Object3D() : position(0.0f), rotation(0.0f), scale(1.0f) {}
    
    void setup() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
                     vertices.data(), GL_STATIC_DRAW);
        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                             (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                             (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        
        // Color attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                             (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
    }
    
    glm::mat4 getModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }
    
    void draw() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
    }
    
    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

// =====================================================================
// FUNGSI HELPER UNTUK MEMBUAT KUBUS
// =====================================================================
void addCubeFace(std::vector<Vertex>& vertices, 
                 glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4,
                 glm::vec3 normal, glm::vec3 color) {
    // Triangle 1
    vertices.push_back({p1, normal, color});
    vertices.push_back({p2, normal, color});
    vertices.push_back({p3, normal, color});
    // Triangle 2
    vertices.push_back({p1, normal, color});
    vertices.push_back({p3, normal, color});
    vertices.push_back({p4, normal, color});
}

Object3D createCube(float width, float height, float depth, glm::vec3 color) {
    Object3D obj;
    float w = width / 2.0f;
    float h = height / 2.0f;
    float d = depth / 2.0f;
    
    // Front face
    addCubeFace(obj.vertices,
        glm::vec3(-w, -h, d), glm::vec3(w, -h, d),
        glm::vec3(w, h, d), glm::vec3(-w, h, d),
        glm::vec3(0.0f, 0.0f, 1.0f), color);
    
    // Back face
    addCubeFace(obj.vertices,
        glm::vec3(w, -h, -d), glm::vec3(-w, -h, -d),
        glm::vec3(-w, h, -d), glm::vec3(w, h, -d),
        glm::vec3(0.0f, 0.0f, -1.0f), color);
    
    // Left face
    addCubeFace(obj.vertices,
        glm::vec3(-w, -h, -d), glm::vec3(-w, -h, d),
        glm::vec3(-w, h, d), glm::vec3(-w, h, -d),
        glm::vec3(-1.0f, 0.0f, 0.0f), color);
    
    // Right face
    addCubeFace(obj.vertices,
        glm::vec3(w, -h, d), glm::vec3(w, -h, -d),
        glm::vec3(w, h, -d), glm::vec3(w, h, d),
        glm::vec3(1.0f, 0.0f, 0.0f), color);
    
    // Top face
    addCubeFace(obj.vertices,
        glm::vec3(-w, h, d), glm::vec3(w, h, d),
        glm::vec3(w, h, -d), glm::vec3(-w, h, -d),
        glm::vec3(0.0f, 1.0f, 0.0f), color);
    
    // Bottom face
    addCubeFace(obj.vertices,
        glm::vec3(-w, -h, -d), glm::vec3(w, -h, -d),
        glm::vec3(w, -h, d), glm::vec3(-w, -h, d),
        glm::vec3(0.0f, -1.0f, 0.0f), color);
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT LANTAI
// =====================================================================
Object3D createFloor(float width, float depth, glm::vec3 color) {
    Object3D obj;
    float w = width / 2.0f;
    float d = depth / 2.0f;
    
    // Lantai dengan pola kotak-kotak
    int tiles = 8;
    float tileW = width / tiles;
    float tileD = depth / tiles;
    
    for (int i = 0; i < tiles; i++) {
        for (int j = 0; j < tiles; j++) {
            float x1 = -w + i * tileW;
            float x2 = x1 + tileW;
            float z1 = -d + j * tileD;
            float z2 = z1 + tileD;
            
            // Warna kotak-kotak
            glm::vec3 tileColor = ((i + j) % 2 == 0) ? color : color * 0.7f;
            
            addCubeFace(obj.vertices,
                glm::vec3(x1, 0.0f, z2), glm::vec3(x2, 0.0f, z2),
                glm::vec3(x2, 0.0f, z1), glm::vec3(x1, 0.0f, z1),
                glm::vec3(0.0f, 1.0f, 0.0f), tileColor);
        }
    }
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT DINDING
// =====================================================================
Object3D createWalls(float width, float height, float depth, glm::vec3 color) {
    Object3D obj;
    float w = width / 2.0f;
    float d = depth / 2.0f;
    
    // Back wall (dengan lubang jendela)
    // Bagian kiri jendela
    addCubeFace(obj.vertices,
        glm::vec3(-w, 0.0f, -d), glm::vec3(-1.5f, 0.0f, -d),
        glm::vec3(-1.5f, height, -d), glm::vec3(-w, height, -d),
        glm::vec3(0.0f, 0.0f, 1.0f), color);
    
    // Bagian kanan jendela
    addCubeFace(obj.vertices,
        glm::vec3(1.5f, 0.0f, -d), glm::vec3(w, 0.0f, -d),
        glm::vec3(w, height, -d), glm::vec3(1.5f, height, -d),
        glm::vec3(0.0f, 0.0f, 1.0f), color);
    
    // Bagian atas jendela
    addCubeFace(obj.vertices,
        glm::vec3(-1.5f, 2.5f, -d), glm::vec3(1.5f, 2.5f, -d),
        glm::vec3(1.5f, height, -d), glm::vec3(-1.5f, height, -d),
        glm::vec3(0.0f, 0.0f, 1.0f), color);
    
    // Bagian bawah jendela
    addCubeFace(obj.vertices,
        glm::vec3(-1.5f, 0.0f, -d), glm::vec3(1.5f, 0.0f, -d),
        glm::vec3(1.5f, 0.8f, -d), glm::vec3(-1.5f, 0.8f, -d),
        glm::vec3(0.0f, 0.0f, 1.0f), color);
    
    // Left wall
    addCubeFace(obj.vertices,
        glm::vec3(-w, 0.0f, d), glm::vec3(-w, 0.0f, -d),
        glm::vec3(-w, height, -d), glm::vec3(-w, height, d),
        glm::vec3(1.0f, 0.0f, 0.0f), color * 0.95f);
    
    // Right wall (dengan lubang pintu)
    // Bagian atas pintu
    addCubeFace(obj.vertices,
        glm::vec3(w, 2.2f, d), glm::vec3(w, 2.2f, 1.0f),
        glm::vec3(w, height, 1.0f), glm::vec3(w, height, d),
        glm::vec3(-1.0f, 0.0f, 0.0f), color * 0.95f);
    
    // Bagian belakang pintu
    addCubeFace(obj.vertices,
        glm::vec3(w, 0.0f, 1.0f), glm::vec3(w, 0.0f, -d),
        glm::vec3(w, height, -d), glm::vec3(w, height, 1.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f), color * 0.95f);
    
    // Bagian depan pintu
    addCubeFace(obj.vertices,
        glm::vec3(w, 0.0f, d), glm::vec3(w, 0.0f, 2.0f),
        glm::vec3(w, height, 2.0f), glm::vec3(w, height, d),
        glm::vec3(-1.0f, 0.0f, 0.0f), color * 0.95f);
    
    // Bagian atas pintu (di antara)
    addCubeFace(obj.vertices,
        glm::vec3(w, 2.2f, 2.0f), glm::vec3(w, 2.2f, 1.0f),
        glm::vec3(w, height, 1.0f), glm::vec3(w, height, 2.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f), color * 0.95f);
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT LANGIT-LANGIT
// =====================================================================
Object3D createCeiling(float width, float depth, float height, glm::vec3 color) {
    Object3D obj;
    float w = width / 2.0f;
    float d = depth / 2.0f;
    
    addCubeFace(obj.vertices,
        glm::vec3(-w, height, -d), glm::vec3(w, height, -d),
        glm::vec3(w, height, d), glm::vec3(-w, height, d),
        glm::vec3(0.0f, -1.0f, 0.0f), color);
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT JENDELA
// =====================================================================
Object3D createWindow(glm::vec3 color) {
    Object3D obj;
    
    // Frame jendela
    float frameWidth = 0.1f;
    glm::vec3 frameColor(0.4f, 0.3f, 0.2f);  // Coklat kayu
    
    // Frame atas
    addCubeFace(obj.vertices,
        glm::vec3(-1.5f, 2.4f, 0.0f), glm::vec3(1.5f, 2.4f, 0.0f),
        glm::vec3(1.5f, 2.5f, 0.0f), glm::vec3(-1.5f, 2.5f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), frameColor);
    
    // Frame bawah
    addCubeFace(obj.vertices,
        glm::vec3(-1.5f, 0.8f, 0.0f), glm::vec3(1.5f, 0.8f, 0.0f),
        glm::vec3(1.5f, 0.9f, 0.0f), glm::vec3(-1.5f, 0.9f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), frameColor);
    
    // Frame kiri
    addCubeFace(obj.vertices,
        glm::vec3(-1.5f, 0.8f, 0.0f), glm::vec3(-1.4f, 0.8f, 0.0f),
        glm::vec3(-1.4f, 2.5f, 0.0f), glm::vec3(-1.5f, 2.5f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), frameColor);
    
    // Frame kanan
    addCubeFace(obj.vertices,
        glm::vec3(1.4f, 0.8f, 0.0f), glm::vec3(1.5f, 0.8f, 0.0f),
        glm::vec3(1.5f, 2.5f, 0.0f), glm::vec3(1.4f, 2.5f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), frameColor);
    
    // Frame tengah vertikal
    addCubeFace(obj.vertices,
        glm::vec3(-0.05f, 0.8f, 0.0f), glm::vec3(0.05f, 0.8f, 0.0f),
        glm::vec3(0.05f, 2.5f, 0.0f), glm::vec3(-0.05f, 2.5f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), frameColor);
    
    // Frame tengah horizontal
    addCubeFace(obj.vertices,
        glm::vec3(-1.5f, 1.6f, 0.0f), glm::vec3(1.5f, 1.6f, 0.0f),
        glm::vec3(1.5f, 1.7f, 0.0f), glm::vec3(-1.5f, 1.7f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), frameColor);
    
    // Kaca jendela (biru transparan effect)
    glm::vec3 glassColor(0.6f, 0.8f, 0.95f);
    addCubeFace(obj.vertices,
        glm::vec3(-1.4f, 0.9f, -0.01f), glm::vec3(1.4f, 0.9f, -0.01f),
        glm::vec3(1.4f, 2.4f, -0.01f), glm::vec3(-1.4f, 2.4f, -0.01f),
        glm::vec3(0.0f, 0.0f, 1.0f), glassColor);
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT PINTU
// =====================================================================
Object3D createDoor(glm::vec3 color) {
    Object3D obj;
    float doorWidth = 1.0f;
    float doorHeight = 2.2f;
    float doorDepth = 0.05f;
    
    // Badan pintu
    Object3D doorBody = createCube(doorWidth, doorHeight, doorDepth, color);
    for (auto& v : doorBody.vertices) {
        v.position.y += doorHeight / 2.0f;
        obj.vertices.push_back(v);
    }
    
    // Handle pintu
    glm::vec3 handleColor(0.8f, 0.7f, 0.2f);  // Gold
    Object3D handle = createCube(0.05f, 0.15f, 0.08f, handleColor);
    for (auto& v : handle.vertices) {
        v.position.x += 0.35f;
        v.position.y += 1.1f;
        v.position.z += 0.05f;
        obj.vertices.push_back(v);
    }
    
    // Panel dekoratif atas
    glm::vec3 panelColor = color * 0.8f;
    addCubeFace(obj.vertices,
        glm::vec3(-0.35f, 1.4f, 0.03f), glm::vec3(0.25f, 1.4f, 0.03f),
        glm::vec3(0.25f, 2.0f, 0.03f), glm::vec3(-0.35f, 2.0f, 0.03f),
        glm::vec3(0.0f, 0.0f, 1.0f), panelColor);
    
    // Panel dekoratif bawah
    addCubeFace(obj.vertices,
        glm::vec3(-0.35f, 0.2f, 0.03f), glm::vec3(0.25f, 0.2f, 0.03f),
        glm::vec3(0.25f, 1.2f, 0.03f), glm::vec3(-0.35f, 1.2f, 0.03f),
        glm::vec3(0.0f, 0.0f, 1.0f), panelColor);
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT MEJA
// =====================================================================
Object3D createDesk(glm::vec3 color) {
    Object3D obj;
    
    // Permukaan meja
    Object3D top = createCube(1.6f, 0.05f, 0.8f, color);
    for (auto& v : top.vertices) {
        v.position.y += 0.75f;
        obj.vertices.push_back(v);
    }
    
    // Kaki meja (4 kaki)
    glm::vec3 legColor = color * 0.7f;
    float legPositions[4][2] = {{-0.7f, -0.3f}, {0.7f, -0.3f}, {-0.7f, 0.3f}, {0.7f, 0.3f}};
    
    for (int i = 0; i < 4; i++) {
        Object3D leg = createCube(0.05f, 0.75f, 0.05f, legColor);
        for (auto& v : leg.vertices) {
            v.position.x += legPositions[i][0];
            v.position.z += legPositions[i][1];
            v.position.y += 0.375f;
            obj.vertices.push_back(v);
        }
    }
    
    // Laci meja
    Object3D drawer = createCube(0.5f, 0.2f, 0.6f, color * 0.9f);
    for (auto& v : drawer.vertices) {
        v.position.x += 0.45f;
        v.position.y += 0.55f;
        obj.vertices.push_back(v);
    }
    
    // Handle laci
    glm::vec3 handleColor(0.6f, 0.6f, 0.6f);
    Object3D handle = createCube(0.15f, 0.03f, 0.03f, handleColor);
    for (auto& v : handle.vertices) {
        v.position.x += 0.45f;
        v.position.y += 0.55f;
        v.position.z += 0.32f;
        obj.vertices.push_back(v);
    }
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT KURSI
// =====================================================================
Object3D createChair(glm::vec3 color) {
    Object3D obj;
    
    // Dudukan kursi
    Object3D seat = createCube(0.45f, 0.05f, 0.45f, color);
    for (auto& v : seat.vertices) {
        v.position.y += 0.45f;
        obj.vertices.push_back(v);
    }
    
    // Sandaran kursi
    Object3D back = createCube(0.45f, 0.5f, 0.05f, color);
    for (auto& v : back.vertices) {
        v.position.y += 0.72f;
        v.position.z -= 0.2f;
        obj.vertices.push_back(v);
    }
    
    // Kaki kursi (4 kaki)
    glm::vec3 legColor(0.3f, 0.3f, 0.3f);
    float legPositions[4][2] = {{-0.18f, -0.18f}, {0.18f, -0.18f}, {-0.18f, 0.18f}, {0.18f, 0.18f}};
    
    for (int i = 0; i < 4; i++) {
        Object3D leg = createCube(0.04f, 0.45f, 0.04f, legColor);
        for (auto& v : leg.vertices) {
            v.position.x += legPositions[i][0];
            v.position.z += legPositions[i][1];
            v.position.y += 0.225f;
            obj.vertices.push_back(v);
        }
    }
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT MONITOR KOMPUTER
// =====================================================================
Object3D createMonitor(glm::vec3 frameColor) {
    Object3D obj;
    
    // Layar monitor
    Object3D screen = createCube(0.6f, 0.4f, 0.03f, glm::vec3(0.1f, 0.1f, 0.1f));
    for (auto& v : screen.vertices) {
        v.position.y += 0.35f;
        obj.vertices.push_back(v);
    }
    
    // Bagian layar yang menyala
    glm::vec3 screenColor(0.2f, 0.4f, 0.6f);
    addCubeFace(obj.vertices,
        glm::vec3(-0.27f, 0.18f, 0.02f), glm::vec3(0.27f, 0.18f, 0.02f),
        glm::vec3(0.27f, 0.5f, 0.02f), glm::vec3(-0.27f, 0.5f, 0.02f),
        glm::vec3(0.0f, 0.0f, 1.0f), screenColor);
    
    // Stand monitor
    Object3D stand = createCube(0.08f, 0.15f, 0.08f, frameColor);
    for (auto& v : stand.vertices) {
        v.position.y += 0.075f;
        obj.vertices.push_back(v);
    }
    
    // Base monitor
    Object3D base = createCube(0.25f, 0.02f, 0.15f, frameColor);
    for (auto& v : base.vertices) {
        v.position.y += 0.01f;
        obj.vertices.push_back(v);
    }
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT KEYBOARD
// =====================================================================
Object3D createKeyboard(glm::vec3 color) {
    Object3D obj;
    
    Object3D keyboard = createCube(0.4f, 0.02f, 0.15f, color);
    for (auto& v : keyboard.vertices) {
        obj.vertices.push_back(v);
    }
    
    // Tombol-tombol keyboard (simplified)
    glm::vec3 keyColor(0.2f, 0.2f, 0.2f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 10; j++) {
            addCubeFace(obj.vertices,
                glm::vec3(-0.18f + j * 0.038f, 0.011f, -0.06f + i * 0.035f),
                glm::vec3(-0.15f + j * 0.038f, 0.011f, -0.06f + i * 0.035f),
                glm::vec3(-0.15f + j * 0.038f, 0.011f, -0.03f + i * 0.035f),
                glm::vec3(-0.18f + j * 0.038f, 0.011f, -0.03f + i * 0.035f),
                glm::vec3(0.0f, 1.0f, 0.0f), keyColor);
        }
    }
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT LEMARI
// =====================================================================
Object3D createCabinet(glm::vec3 color) {
    Object3D obj;
    
    // Badan lemari
    Object3D body = createCube(0.8f, 2.0f, 0.5f, color);
    for (auto& v : body.vertices) {
        v.position.y += 1.0f;
        obj.vertices.push_back(v);
    }
    
    // Pintu lemari kiri
    glm::vec3 doorColor = color * 0.9f;
    addCubeFace(obj.vertices,
        glm::vec3(-0.38f, 0.05f, 0.26f), glm::vec3(-0.02f, 0.05f, 0.26f),
        glm::vec3(-0.02f, 1.95f, 0.26f), glm::vec3(-0.38f, 1.95f, 0.26f),
        glm::vec3(0.0f, 0.0f, 1.0f), doorColor);
    
    // Pintu lemari kanan
    addCubeFace(obj.vertices,
        glm::vec3(0.02f, 0.05f, 0.26f), glm::vec3(0.38f, 0.05f, 0.26f),
        glm::vec3(0.38f, 1.95f, 0.26f), glm::vec3(0.02f, 1.95f, 0.26f),
        glm::vec3(0.0f, 0.0f, 1.0f), doorColor);
    
    // Handle lemari
    glm::vec3 handleColor(0.7f, 0.7f, 0.7f);
    Object3D handleL = createCube(0.02f, 0.15f, 0.02f, handleColor);
    for (auto& v : handleL.vertices) {
        v.position.x -= 0.05f;
        v.position.y += 1.0f;
        v.position.z += 0.28f;
        obj.vertices.push_back(v);
    }
    
    Object3D handleR = createCube(0.02f, 0.15f, 0.02f, handleColor);
    for (auto& v : handleR.vertices) {
        v.position.x += 0.05f;
        v.position.y += 1.0f;
        v.position.z += 0.28f;
        obj.vertices.push_back(v);
    }
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT LAMPU GANTUNG
// =====================================================================
Object3D createCeilingLamp(glm::vec3 color) {
    Object3D obj;
    
    // Tiang gantung
    Object3D rod = createCube(0.02f, 0.3f, 0.02f, glm::vec3(0.3f, 0.3f, 0.3f));
    for (auto& v : rod.vertices) {
        v.position.y += 0.15f;
        obj.vertices.push_back(v);
    }
    
    // Kap lampu (cone simplified as octagon)
    glm::vec3 shadeColor(0.9f, 0.85f, 0.7f);
    int segments = 8;
    float topRadius = 0.1f;
    float bottomRadius = 0.25f;
    float height = 0.15f;
    
    for (int i = 0; i < segments; i++) {
        float angle1 = (float)i / segments * 2.0f * 3.14159f;
        float angle2 = (float)(i + 1) / segments * 2.0f * 3.14159f;
        
        glm::vec3 top1(cos(angle1) * topRadius, 0.0f, sin(angle1) * topRadius);
        glm::vec3 top2(cos(angle2) * topRadius, 0.0f, sin(angle2) * topRadius);
        glm::vec3 bot1(cos(angle1) * bottomRadius, -height, sin(angle1) * bottomRadius);
        glm::vec3 bot2(cos(angle2) * bottomRadius, -height, sin(angle2) * bottomRadius);
        
        glm::vec3 normal = glm::normalize(glm::vec3(cos((angle1 + angle2) / 2), 0.3f, 
                                                     sin((angle1 + angle2) / 2)));
        
        obj.vertices.push_back({top1, normal, shadeColor});
        obj.vertices.push_back({bot1, normal, shadeColor});
        obj.vertices.push_back({bot2, normal, shadeColor});
        
        obj.vertices.push_back({top1, normal, shadeColor});
        obj.vertices.push_back({bot2, normal, shadeColor});
        obj.vertices.push_back({top2, normal, shadeColor});
    }
    
    // Bola lampu
    Object3D bulb = createCube(0.08f, 0.1f, 0.08f, color);
    for (auto& v : bulb.vertices) {
        v.position.y -= 0.15f;
        obj.vertices.push_back(v);
    }
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT BUKU
// =====================================================================
Object3D createBook(glm::vec3 color) {
    Object3D obj;
    
    Object3D book = createCube(0.15f, 0.22f, 0.03f, color);
    for (auto& v : book.vertices) {
        obj.vertices.push_back(v);
    }
    
    // Halaman
    glm::vec3 pageColor(0.95f, 0.95f, 0.9f);
    addCubeFace(obj.vertices,
        glm::vec3(-0.07f, -0.1f, 0.016f), glm::vec3(0.07f, -0.1f, 0.016f),
        glm::vec3(0.07f, 0.1f, 0.016f), glm::vec3(-0.07f, 0.1f, 0.016f),
        glm::vec3(0.0f, 0.0f, 1.0f), pageColor);
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT RUG/KARPET
// =====================================================================
Object3D createRug(float width, float depth, glm::vec3 color) {
    Object3D obj;
    
    addCubeFace(obj.vertices,
        glm::vec3(-width/2, 0.01f, depth/2), glm::vec3(width/2, 0.01f, depth/2),
        glm::vec3(width/2, 0.01f, -depth/2), glm::vec3(-width/2, 0.01f, -depth/2),
        glm::vec3(0.0f, 1.0f, 0.0f), color);
    
    // Border karpet
    glm::vec3 borderColor = color * 0.7f;
    float border = 0.1f;
    
    // Border depan
    addCubeFace(obj.vertices,
        glm::vec3(-width/2, 0.012f, depth/2), glm::vec3(width/2, 0.012f, depth/2),
        glm::vec3(width/2, 0.012f, depth/2 - border), glm::vec3(-width/2, 0.012f, depth/2 - border),
        glm::vec3(0.0f, 1.0f, 0.0f), borderColor);
    
    // Border belakang
    addCubeFace(obj.vertices,
        glm::vec3(-width/2, 0.012f, -depth/2 + border), glm::vec3(width/2, 0.012f, -depth/2 + border),
        glm::vec3(width/2, 0.012f, -depth/2), glm::vec3(-width/2, 0.012f, -depth/2),
        glm::vec3(0.0f, 1.0f, 0.0f), borderColor);
    
    return obj;
}

// =====================================================================
// FUNGSI MEMBUAT LIGHT SOURCE CUBE (untuk visualisasi lampu)
// =====================================================================
Object3D createLightCube() {
    Object3D obj;
    obj = createCube(0.1f, 0.1f, 0.1f, glm::vec3(1.0f, 1.0f, 1.0f));
    return obj;
}

// =====================================================================
// MAIN FUNCTION
// =====================================================================
int main() {
    // Inisialisasi GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Konfigurasi GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Buat window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, 
        "Simulasi Ruang Kerja 3D - Grafika Komputer", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Inisialisasi GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Load shaders
    unsigned int shaderProgram = loadShader("shaders/vertex_shader.glsl", 
                                            "shaders/fragment_shader.glsl");
    unsigned int lightShader = loadShader("shaders/light_vertex.glsl", 
                                          "shaders/light_fragment.glsl");
    
    // =====================================================================
    // BUAT SEMUA OBJEK RUANGAN
    // =====================================================================
    
    // Dimensi ruangan
    float roomWidth = 10.0f;
    float roomHeight = 4.0f;
    float roomDepth = 8.0f;
    
    // Warna-warna
    glm::vec3 floorColor(0.6f, 0.5f, 0.4f);      // Coklat lantai kayu
    glm::vec3 wallColor(0.9f, 0.88f, 0.82f);     // Krem dinding
    glm::vec3 ceilingColor(0.95f, 0.95f, 0.95f); // Putih langit-langit
    glm::vec3 deskColor(0.55f, 0.4f, 0.25f);     // Coklat meja kayu
    glm::vec3 chairColor(0.15f, 0.15f, 0.15f);   // Hitam kursi
    glm::vec3 doorColor(0.5f, 0.35f, 0.2f);      // Coklat pintu
    glm::vec3 cabinetColor(0.6f, 0.45f, 0.3f);   // Coklat lemari
    glm::vec3 rugColor(0.6f, 0.2f, 0.2f);        // Merah karpet
    
    // Buat objek-objek
    Object3D floor = createFloor(roomWidth, roomDepth, floorColor);
    floor.setup();
    
    Object3D walls = createWalls(roomWidth, roomHeight, roomDepth, wallColor);
    walls.setup();
    
    Object3D ceiling = createCeiling(roomWidth, roomDepth, roomHeight, ceilingColor);
    ceiling.setup();
    
    Object3D windowObj = createWindow(glm::vec3(0.6f, 0.8f, 0.9f));
    windowObj.position = glm::vec3(0.0f, 0.0f, -roomDepth/2 + 0.01f);
    windowObj.setup();
    
    Object3D door = createDoor(doorColor);
    door.position = glm::vec3(roomWidth/2 - 0.01f, 0.0f, 1.5f);
    door.rotation = glm::vec3(0.0f, -90.0f, 0.0f);
    door.setup();
    
    // Meja kerja (menempel dinding belakang)
    Object3D desk = createDesk(deskColor);
    desk.position = glm::vec3(-2.5f, 0.0f, -3.6f);  // Menempel dinding belakang
    desk.setup();
    
    // Kursi (di depan meja)
    Object3D chair = createChair(chairColor);
    chair.position = glm::vec3(-2.5f, 0.0f, -2.3f);  // Di depan meja
    chair.rotation = glm::vec3(0.0f, 180.0f, 0.0f);  // Menghadap ke meja
    chair.setup();
    
    // Monitor (di atas meja)
    Object3D monitor = createMonitor(glm::vec3(0.2f, 0.2f, 0.2f));
    monitor.position = glm::vec3(-2.5f, 0.78f, -3.7f);  // Di atas meja dekat dinding
    monitor.setup();
    
    // Keyboard (di atas meja)
    Object3D keyboard = createKeyboard(glm::vec3(0.15f, 0.15f, 0.15f));
    keyboard.position = glm::vec3(-2.5f, 0.78f, -3.4f);  // Di depan monitor, di atas meja
    keyboard.setup();
    
    // Lemari (di sudut kiri belakang)
    Object3D cabinet = createCabinet(cabinetColor);
    cabinet.position = glm::vec3(-4.0f, 0.0f, -3.5f);  // Posisi semula
    cabinet.setup();
    
    // Lampu gantung
    Object3D lamp = createCeilingLamp(glm::vec3(1.0f, 0.95f, 0.8f));
    lamp.position = glm::vec3(0.0f, roomHeight - 0.3f, 0.0f);
    lamp.setup();
    
    // Buku-buku di meja (posisi flat di atas meja)
    Object3D book1 = createBook(glm::vec3(0.8f, 0.2f, 0.2f));
    book1.position = glm::vec3(-3.0f, 0.795f, -3.3f);  // Di atas meja
    book1.rotation = glm::vec3(90.0f, 15.0f, 0.0f);    // Flat di meja
    book1.setup();
    
    Object3D book2 = createBook(glm::vec3(0.2f, 0.4f, 0.8f));
    book2.position = glm::vec3(-3.0f, 0.82f, -3.3f);   // Di atas buku pertama
    book2.rotation = glm::vec3(90.0f, -5.0f, 0.0f);    // Flat di meja
    book2.setup();
    
    // Light source visualization
    Object3D lightCube = createLightCube();
    lightCube.position = lightPos;
    lightCube.setup();
    
    // Print kontrol
    std::cout << "========================================" << std::endl;
    std::cout << "  SIMULASI RUANG KERJA 3D" << std::endl;
    std::cout << "  Grafika Komputer" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "\nKONTROL:" << std::endl;
    std::cout << "  WASD      - Gerak kamera" << std::endl;
    std::cout << "  Mouse     - Rotasi pandangan" << std::endl;
    std::cout << "  Scroll    - Zoom in/out" << std::endl;
    std::cout << "  1         - View depan" << std::endl;
    std::cout << "  2         - View atas" << std::endl;
    std::cout << "  3         - View samping" << std::endl;
    std::cout << "  4         - View sudut" << std::endl;
    std::cout << "  L         - Toggle lampu" << std::endl;
    std::cout << "  ESC       - Keluar" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // =====================================================================
    // RENDER LOOP
    // =====================================================================
    while (!glfwWindowShouldClose(window)) {
        // Hitung delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Input
        processInput(window);
        
        // Clear buffers
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Setup matrices
        glm::mat4 projection = glm::perspective(glm::radians(fov), 
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        // Use main shader
        glUseProgram(shaderProgram);
        
        // Set uniforms
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 
                          1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 
                          1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 
                    1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 
                    1, glm::value_ptr(cameraPos));
        
        // Lighting parameters based on light state
        if (lightOn) {
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 
                        1, glm::value_ptr(lightColor));
            glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), 0.3f);
            glUniform1f(glGetUniformLocation(shaderProgram, "diffuseStrength"), 0.8f);
            glUniform1f(glGetUniformLocation(shaderProgram, "specularStrength"), 0.5f);
        } else {
            glm::vec3 dimLight(0.3f, 0.3f, 0.35f);
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 
                        1, glm::value_ptr(dimLight));
            glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), 0.15f);
            glUniform1f(glGetUniformLocation(shaderProgram, "diffuseStrength"), 0.2f);
            glUniform1f(glGetUniformLocation(shaderProgram, "specularStrength"), 0.1f);
        }
        glUniform1f(glGetUniformLocation(shaderProgram, "shininess"), 32.0f);
        
        // =====================================================================
        // RENDER SEMUA OBJEK DENGAN TRANSFORMASI
        // =====================================================================
        
        // Lantai
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(floor.getModelMatrix()));
        floor.draw();
        
        // Dinding
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(walls.getModelMatrix()));
        walls.draw();
        
        // Langit-langit
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(ceiling.getModelMatrix()));
        ceiling.draw();
        
        // Jendela
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(windowObj.getModelMatrix()));
        windowObj.draw();
        
        // Pintu
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(door.getModelMatrix()));
        door.draw();
        
        // Meja
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(desk.getModelMatrix()));
        desk.draw();
        
        // Kursi
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(chair.getModelMatrix()));
        chair.draw();
        
        // Monitor
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(monitor.getModelMatrix()));
        monitor.draw();
        
        // Keyboard
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(keyboard.getModelMatrix()));
        keyboard.draw();
        
        // Lemari
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(cabinet.getModelMatrix()));
        cabinet.draw();
        
        // Lampu gantung
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(lamp.getModelMatrix()));
        lamp.draw();
        
        // Buku
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(book1.getModelMatrix()));
        book1.draw();
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                          1, GL_FALSE, glm::value_ptr(book2.getModelMatrix()));
        book2.draw();
        
        // Render light source cube
        if (lightOn) {
            glUseProgram(lightShader);
            glUniformMatrix4fv(glGetUniformLocation(lightShader, "projection"), 
                              1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(lightShader, "view"), 
                              1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(lightShader, "model"), 
                              1, GL_FALSE, glm::value_ptr(lightCube.getModelMatrix()));
            glUniform3fv(glGetUniformLocation(lightShader, "lightColor"), 
                        1, glm::value_ptr(lightColor));
            lightCube.draw();
        }
        
        // Swap buffers dan poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    floor.cleanup();
    walls.cleanup();
    ceiling.cleanup();
    windowObj.cleanup();
    door.cleanup();
    desk.cleanup();
    chair.cleanup();
    monitor.cleanup();
    keyboard.cleanup();
    cabinet.cleanup();
    lamp.cleanup();
    book1.cleanup();
    book2.cleanup();
    lightCube.cleanup();
    
    glDeleteProgram(shaderProgram);
    glDeleteProgram(lightShader);
    
    glfwTerminate();
    return 0;
}

// =====================================================================
// FUNGSI IMPLEMENTASI
// =====================================================================

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 90.0f) fov = 90.0f;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float cameraSpeed = 2.5f * deltaTime;
    
    // WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    // Vertical movement
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
    
    // Preset views
    static bool key1Pressed = false;
    static bool key2Pressed = false;
    static bool key3Pressed = false;
    static bool key4Pressed = false;
    static bool keyLPressed = false;
    
    // View 1: Front view (melihat ke arah meja dan jendela)
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !key1Pressed) {
        cameraPos = glm::vec3(0.0f, 2.0f, 3.0f);
        yaw = -90.0f;
        pitch = -5.0f;
        key1Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) key1Pressed = false;
    
    // View 2: Top view (melihat dari atas)
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !key2Pressed) {
        cameraPos = glm::vec3(-2.0f, 6.0f, -2.0f);
        yaw = -90.0f;
        pitch = -75.0f;
        key2Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) key2Pressed = false;
    
    // View 3: Side view (melihat meja dari samping)
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !key3Pressed) {
        cameraPos = glm::vec3(1.0f, 1.5f, -3.5f);
        yaw = -180.0f;
        pitch = 0.0f;
        key3Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE) key3Pressed = false;
    
    // View 4: Corner view (melihat seluruh ruangan dari sudut)
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !key4Pressed) {
        cameraPos = glm::vec3(3.0f, 2.5f, 2.0f);
        yaw = -130.0f;
        pitch = -15.0f;
        key4Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE) key4Pressed = false;
    
    // Toggle light
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !keyLPressed) {
        lightOn = !lightOn;
        std::cout << "Lampu: " << (lightOn ? "ON" : "OFF") << std::endl;
        keyLPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) keyLPressed = false;
    
    // Update camera front
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

std::string readShaderFile(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open shader file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int loadShader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = readShaderFile(vertexPath);
    std::string fragmentCode = readShaderFile(fragmentPath);
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
    }
    
    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR: Fragment shader compilation failed\n" << infoLog << std::endl;
    }
    
    // Shader program
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return program;
}

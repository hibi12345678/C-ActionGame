﻿//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Terrain.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>
#include <cstdio>
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"


///////////////////////////////////////////////////////////////////////////////
// Terrain  class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
Terrain::Terrain(Shader* shader, const Matrix4& view, const Matrix4& proj)
    : mflag(false)
{

    //HightMapのサイズ
    int width= 2624;
    int height = 1756;

    mAngle = 0.0f;
    mScale = glm::vec3(30.0f, 30.0f, 30.0f);
    //インデックスバッファの生成
    std::vector<GLuint> indices;
    for (int z = 0; z <height - 1; ++z) { 
        for (int x = 0; x < width - 1; ++x) {
   
            indices.push_back(z * width + x);
            indices.push_back(z * width + (x + 1));
            indices.push_back((z + 1) * width + x);

            indices.push_back((z + 1) * width + x);
            indices.push_back(z * width + (x + 1));
            indices.push_back((z + 1) * width + (x + 1));
        }
        
    }

    numIndices = indices.size();
    mShader = shader;
  
    std::vector<float> heightMap = loadHeightMap("Assets/Texture/iceland_heightmap.png",width, height);
    controlPoint = generateControlPoints(width, height, heightMap);

    for (auto& point : controlPoint) {
        point.normal = glm::vec3(0.0f);
 
    }

    //法線の生成
    for (size_t i = 0; i < indices.size(); i += 6) 
    {
        glm::vec3 p1 = controlPoint[indices[i]].position;
        glm::vec3 p2 = controlPoint[indices[i + 1]].position;
        glm::vec3 p3 = controlPoint[indices[i + 2]].position;
        glm::vec3 normal1 = CalculateNormal(p1, p2, p3);
        glm::vec3 p4 = controlPoint[indices[i + 5]].position;
        glm::vec3 normal2 = CalculateNormal(p3, p2, p4);
        
        controlPoint[indices[i]].normal += normal1;
        controlPoint[indices[i + 1]].normal += normal1;
        controlPoint[indices[i + 2]].normal += normal1;
        controlPoint[indices[i + 3]].normal += normal2;
        controlPoint[indices[i + 4]].normal += normal2;
        controlPoint[indices[i + 5]].normal += normal2;
        
    }

    //法線の平坦化
    for (auto& point : controlPoint) {
        point.normal = glm::normalize(point.normal); 
    }

    //Terrainの初期化
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, controlPoint.size() * sizeof(Vertex), &controlPoint[0], GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));  // 法線の設定
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));  // UVの設定
    glEnableVertexAttribArray(2);

    textureID = loadTerrainTexture("Assets/Texture/aerial_grass_rock_diff_4k.jpg");
    rockTextureID = loadTerrainTexture("Assets/Texture/rock_boulder_dry_diff_4k.jpg");
    soilTextureID = loadTerrainTexture("Assets/Texture/aerial_grass_rock_diff_4k.jpg");
    snowTextureID = loadTerrainTexture("Assets/Texture/snow_02_diff_4k.jpg");

    glBindVertexArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


//-----------------------------------------------------------------------------
//   Terrainの描画処理
//-----------------------------------------------------------------------------
void Terrain::GenerateTerrain(const Matrix4& view, const Matrix4& proj)
{
    mShader->SetActive();
    
    //ビュー,射影の変換
    glm::mat4 glmView = ConvertToGLM(view);
    glm::mat4 glmProjection = ConvertToGLM(proj);
    glm::mat4 swappedView = glm::mat4(glmView[0], glmView[1], glmView[2], glmView[3]);
    glm::mat4 viewProj = glmProjection * swappedView;

    //シェーダーファイルへの送信
    glUniformMatrix4fv(glGetUniformLocation(mShader->GetID(), "uViewProjection"), 1, GL_FALSE, glm::value_ptr(viewProj));
    glm::vec3 scale = mScale;
    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::translate(model,pos); 
    model = glm::scale(model, scale); 
    model = glm::rotate(model, glm::radians(mAngle), glm::vec3(0.0f, 0.0f, 1.0f)); 
    glUniform1f(glGetUniformLocation(mShader->GetID(), "uZScale"), scale.z);
    glUniformMatrix4fv(glGetUniformLocation(mShader->GetID(), "uModel"), 1, GL_FALSE, glm::value_ptr(model));
    float uvScale = 64.0f; 
    glUniform1f(glGetUniformLocation(mShader->GetID(), "uvScale"), uvScale);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glUniform1i(glGetUniformLocation(mShader->GetID(), "texture1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rockTextureID);

    glUniform1i(glGetUniformLocation(mShader->GetID(), "texture2"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, soilTextureID);

    glUniform1i(glGetUniformLocation(mShader->GetID(), "texture3"), 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, snowTextureID);

    glUniform1i(glGetUniformLocation(mShader->GetID(), "texture4"), 3);
    if (mflag == true ) {
        glUniform1i(glGetUniformLocation(mShader->GetID(), "texture1"), 3);
        glUniform1i(glGetUniformLocation(mShader->GetID(), "texture2"), 1);
        glUniform1i(glGetUniformLocation(mShader->GetID(), "texture3"), 3);
    }


    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}


//-----------------------------------------------------------------------------
//  頂点情報の生成
//-----------------------------------------------------------------------------
std::vector<Vertex> Terrain::generateControlPoints(int gridWidth, int gridHeight, const std::vector<float>& heightMap) {
    std::vector<Vertex> controlPoints;

    float spacingX = 1.0f;
    float spacingY = 1.0f; 
    float startX = -gridWidth * 0.5f * spacingX; 
    float startY = -gridHeight * 0.5f * spacingY;

    for (int z = 0; z < gridHeight; ++z) {
        for (int x = 0; x < gridWidth; ++x) {
            float posX = startX + x * spacingX;
            float posY = startY + z * spacingY;

            int index = z * gridWidth + x; 
            float posZ = heightMap[index] * 255.0f;   

            float uvX = (float)x / (gridWidth - 1); 
            float uvZ = (float)z / (gridHeight - 1); 

            Vertex vertex;
            vertex.position = glm::vec3(posX, posY, posZ) ;
            vertex.uv = glm::vec2(uvX, uvZ);  

            controlPoints.push_back(vertex);
        }
    }

    return controlPoints;
}


//-----------------------------------------------------------------------------
//  grad関数
//-----------------------------------------------------------------------------
float Terrain::grad(int hash, float x, float y) {
    int h = hash & 15;
    float u = h < 8 ? x : y; float v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
    return ((h & 1 ? -1 : 1) * u + (h & 2 ? -1 : 1) * v);
}


//-----------------------------------------------------------------------------
//  perlin noise関数
//-----------------------------------------------------------------------------
float Terrain::perlin(float x, float y) {
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    float xf = x - floor(x);
    float yf = y - floor(y);
    float u = fade(xf);
    float v = fade(yf);
    int aa = p[X] + Y;
    int ab = p[X + 1] + Y;
    int ba = p[X] + Y + 1;
    int bb = p[X + 1] + Y + 1;
    return lerp(v, lerp(u, grad(p[aa], xf, yf), grad(p[ba], xf - 1, yf)),
        lerp(u, grad(p[ab], xf, yf - 1), grad(p[bb], xf - 1, yf - 1)));
}


//-----------------------------------------------------------------------------
//  HightMapの生成
//-----------------------------------------------------------------------------
std::vector<float> Terrain::generateHeightMap(int width, int height, float scale, float heightScale) {
    std::vector<float> heightMap(width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          
            float nx = (float)x / width - 0.5f;
            float ny = (float)y / height - 0.5f;

            float noiseValue = perlin(nx * scale, ny * scale);

            heightMap[y * width + x] = noiseValue * heightScale;
        }
    }
    return heightMap;
}


//-----------------------------------------------------------------------------
//  Textureの取得
//-----------------------------------------------------------------------------
GLuint Terrain::loadTerrainTexture(const char* filePath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 0);

    if (!data) {
        printf("Failed to load texture: %s\nReason: %s\n", filePath, stbi_failure_reason());
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}


//-----------------------------------------------------------------------------
//  HightMapの取得
//-----------------------------------------------------------------------------
std::vector<float> Terrain::loadHeightMap(const char* filePath, int& width, int& height) {
    int channels;
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 1); // グレースケール (force_channels = 1)

    std::vector<float> heightMap(width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            int index = y * width + x;
            
            
            heightMap[index] = static_cast<float>(data[index]) / 255.0f;
            
        }
    }

    stbi_image_free(data);

    return heightMap;
}


//-----------------------------------------------------------------------------
//  法線の計算
//-----------------------------------------------------------------------------
glm::vec3 Terrain::CalculateNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {

    glm::vec3 edge1 = p2 - p1;
    glm::vec3 edge2 = p3 - p1;
    glm::vec3 normal = glm::cross(edge1, edge2);

    
    return glm::normalize(normal);
}


glm::mat4 Terrain::ConvertToGLM(const Matrix4& matrix)
{
    glm::mat4 glmMatrix;
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            glmMatrix[row][col] = matrix.mat[row][col];
        }
    }
    return glmMatrix;
}

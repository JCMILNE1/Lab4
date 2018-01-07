#ifndef MODELLOADER_H_INCLUDED
#define MODELLOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct ModelStorage
{
    unsigned int vertexStorage;
    unsigned int uvStorage;
    unsigned int normalStorage;
    
    bool operator<(const ModelStorage& r) const { return vertexStorage < r.vertexStorage; }
};

class StoredModel
{
public:
    std::vector<glm::vec3> modelPositions;
    std::vector<glm::vec2> modelTextureCoordinates;
    std::vector<glm::vec3> modelNormals;
    std::vector<unsigned int> modelIndices;
    
    void calculateModelNormals();
};

class ModelObject
{
public:
    std::vector<ModelStorage> ModelIndices;
    std::vector<glm::vec3> ModelVertices;
    std::vector<glm::vec2> ModelUvs;
    std::vector<glm::vec3> modelNormals;
    bool UVsPresent;
    bool NormalsPresent;
    
    ModelObject(const std::string& fileName);
    
    StoredModel ToStoredModel();
private:
    unsigned int FindLastVertexStoragePoint(const std::vector<ModelStorage*>& indexLookup, const ModelStorage* currentIndex, const StoredModel& result);
    void CreateModelFace(const std::string& line);
    
    glm::vec2 ParseModelVector2(const std::string& line);
    glm::vec3 ParseModelVector3(const std::string& line);
    ModelStorage ParseModelIndex(const std::string& token, bool* UVsPresent, bool* NormalsPresent);
};

#endif // MODELLOADER_H_INCLUDED

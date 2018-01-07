#include "Modelloader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareodelLocationPtr(const ModelStorage* a, const ModelStorage* b);
static inline unsigned int FindNextCharacter(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseModelIStorageValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseModelFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

ModelObject::ModelObject(const std::string& fileName)
{
	UVsPresent = false;
	NormalsPresent = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            unsigned int lineLength = line.length();
            
            if(lineLength < 2)
                continue;
            
            const char* lineCStr = line.c_str();
            
            switch(lineCStr[0])
            {
                case 'v':
                    if(lineCStr[1] == 't')
                        this->ModelUvs.push_back(ParseModelVector2(line));
                    else if(lineCStr[1] == 'n')
                        this->modelNormals.push_back(ParseModelVector3(line));
                    else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                        this->ModelVertices.push_back(ParseModelVector3(line));
                break;
                case 'f':
                    CreateModelFace(line);
                break;
                default: break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}



StoredModel ModelObject::ToStoredModel()
{
    StoredModel ModelLocation;
    StoredModel normalModel;
    
    unsigned int IndicesCount = ModelIndices.size();
    
    std::vector<ModelStorage*> indexLookup;
    
    for(unsigned int i = 0; i < IndicesCount; i++)
        indexLookup.push_back(&ModelIndices[i]);
    
    std::sort(indexLookup.begin(), indexLookup.end(), CompareodelLocationPtr);
    
    std::map<ModelStorage, unsigned int> normalModelIndexMap;
    std::map<unsigned int, unsigned int> indexMap;
    
    for(unsigned int i = 0; i < IndicesCount; i++)
    {
        ModelStorage* currentIndex = &ModelIndices[i];
        
        glm::vec3 currentPosition = ModelVertices[currentIndex->vertexStorage];
        glm::vec2 currentTexCoord;
        glm::vec3 currentNormal;
        
        if(UVsPresent)
            currentTexCoord = ModelUvs[currentIndex->uvStorage];
        else
            currentTexCoord = glm::vec2(0,0);
            
        if(NormalsPresent)
            currentNormal = modelNormals[currentIndex->normalStorage];
        else
            currentNormal = glm::vec3(0,0,0);
        
        unsigned int normalModelIndex;
        unsigned int resultModelIndex;
        
        //Create model to properly generate normals on
        std::map<ModelStorage, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
        if(it == normalModelIndexMap.end())
        {
            normalModelIndex = normalModel.modelPositions.size();
        
            normalModelIndexMap.insert(std::pair<ModelStorage, unsigned int>(*currentIndex, normalModelIndex));
            normalModel.modelPositions.push_back(currentPosition);
            normalModel.modelTextureCoordinates.push_back(currentTexCoord);
            normalModel.modelNormals.push_back(currentNormal);
        }
        else
            normalModelIndex = it->second;
        
        //Create model which properly separates texture coordinates
        unsigned int previousVertexLocation = FindLastVertexStoragePoint(indexLookup, currentIndex, ModelLocation);
        
        if(previousVertexLocation == (unsigned int)-1)
        {
            resultModelIndex = ModelLocation.modelPositions.size();
        
            ModelLocation.modelPositions.push_back(currentPosition);
            ModelLocation.modelTextureCoordinates.push_back(currentTexCoord);
            ModelLocation.modelNormals.push_back(currentNormal);
        }
        else
            resultModelIndex = previousVertexLocation;
        
        normalModel.modelIndices.push_back(normalModelIndex);
        ModelLocation.modelIndices.push_back(resultModelIndex);
        indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
    }
    
    if(!NormalsPresent)
    {
        normalModel.calculateModelNormals();
        
        for(unsigned int i = 0; i < ModelLocation.modelPositions.size(); i++)
            ModelLocation.modelNormals[i] = normalModel.modelNormals[indexMap[i]];
    }
    
    return ModelLocation;
};

void StoredModel::calculateModelNormals()
{
    for(unsigned int i = 0; i < modelIndices.size(); i += 3)
    {
        int i0 = modelIndices[i];
        int i1 = modelIndices[i + 1];
        int i2 = modelIndices[i + 2];

        glm::vec3 v1 = modelPositions[i1] - modelPositions[i0];
        glm::vec3 v2 = modelPositions[i2] - modelPositions[i0];
        
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
            
        modelNormals[i0] += normal;
        modelNormals[i1] += normal;
        modelNormals[i2] += normal;
    }
    
    for(unsigned int i = 0; i < modelPositions.size(); i++)
        modelNormals[i] = glm::normalize(modelNormals[i]);
}

unsigned int ModelObject::FindLastVertexStoragePoint(const std::vector<ModelStorage*>& positionLookup, const ModelStorage* currentIndex, const StoredModel& result)
{
    unsigned int startposition = 0;
    unsigned int endposition = positionLookup.size();
    unsigned int currentposition = (endposition - startposition) / 2 + startposition;
    unsigned int previousposition = startposition;
    
    while(currentposition != previousposition)
    {
        ModelStorage* testIndex = positionLookup[currentposition];
        
        if(testIndex->vertexStorage == currentIndex->vertexStorage)
        {
            unsigned int countStart = currentposition;
        
            for(unsigned int i = 0; i < currentposition; i++)
            {
                ModelStorage* possibleIndex = positionLookup[currentposition - i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->vertexStorage != currentIndex->vertexStorage)
                    break;
                    
                countStart--;
            }
            
            for(unsigned int i = countStart; i < positionLookup.size() - countStart; i++)
            {
                ModelStorage* possibleIndex = positionLookup[currentposition + i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->vertexStorage != currentIndex->vertexStorage)
                    break;
                else if((!UVsPresent || possibleIndex->uvStorage == currentIndex->uvStorage) 
                    && (!NormalsPresent || possibleIndex->normalStorage == currentIndex->normalStorage))
                {
                    glm::vec3 currentPosition = ModelVertices[currentIndex->vertexStorage];
                    glm::vec2 currentTexCoord;
                    glm::vec3 currentNormal;
                    
                    if(UVsPresent)
                        currentTexCoord = ModelUvs[currentIndex->uvStorage];
                    else
                        currentTexCoord = glm::vec2(0,0);
                        
                    if(NormalsPresent)
                        currentNormal = modelNormals[currentIndex->normalStorage];
                    else
                        currentNormal = glm::vec3(0,0,0);
                    
                    for(unsigned int j = 0; j < result.modelPositions.size(); j++)
                    {
                        if(currentPosition == result.modelPositions[j] 
                            && ((!UVsPresent || currentTexCoord == result.modelTextureCoordinates[j])
                            && (!NormalsPresent || currentNormal == result.modelNormals[j])))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return -1;
        }
        else
        {
            if(testIndex->vertexStorage < currentIndex->vertexStorage)
                startposition = currentposition;
            else
                endposition = currentposition;
        }
    
        previousposition = currentposition;
        currentposition = (endposition - startposition) / 2 + startposition;
    }
    
    return -1;
}

void ModelObject::CreateModelFace(const std::string& line)
{
    std::vector<std::string> tokens = SplitString(line, ' ');

    this->ModelIndices.push_back(ParseModelIndex(tokens[1], &this->UVsPresent, &this->NormalsPresent));
    this->ModelIndices.push_back(ParseModelIndex(tokens[2], &this->UVsPresent, &this->NormalsPresent));
    this->ModelIndices.push_back(ParseModelIndex(tokens[3], &this->UVsPresent, &this->NormalsPresent));

    if((int)tokens.size() > 4)
    {
        this->ModelIndices.push_back(ParseModelIndex(tokens[1], &this->UVsPresent, &this->NormalsPresent));
        this->ModelIndices.push_back(ParseModelIndex(tokens[3], &this->UVsPresent, &this->NormalsPresent));
        this->ModelIndices.push_back(ParseModelIndex(tokens[4], &this->UVsPresent, &this->NormalsPresent));
    }
}

ModelStorage ModelObject::ParseModelIndex(const std::string& adress, bool* UVsPresent, bool* NormalsPresent)
{
    unsigned int adressLength = adress.length();
    const char* adressString = adress.c_str();
    
    unsigned int vertStorageStart = 0;
    unsigned int vertStorageEnd = FindNextCharacter(vertStorageStart, adressString, adressLength, '/');
    
    ModelStorage result;
    result.vertexStorage = ParseModelIStorageValue(adress, vertStorageStart, vertStorageEnd);
    result.uvStorage = 0;
    result.normalStorage = 0;
    
    if(vertStorageEnd >= adressLength)
        return result;
    
    vertStorageStart = vertStorageEnd + 1;
    vertStorageEnd = FindNextCharacter(vertStorageStart, adressString, adressLength, '/');
    
    result.uvStorage = ParseModelIStorageValue(adress, vertStorageStart, vertStorageEnd);
    *UVsPresent = true;
    
    if(vertStorageEnd >= adressLength)
        return result;
    
    vertStorageStart = vertStorageEnd + 1;
    vertStorageEnd = FindNextCharacter(vertStorageStart, adressString, adressLength, '/');
    
    result.normalStorage = ParseModelIStorageValue(adress, vertStorageStart, vertStorageEnd);
    *NormalsPresent = true;
    
    return result;
}

glm::vec3 ModelObject::ParseModelVector3(const std::string& line) 
{
    unsigned int adressLength = line.length();
    const char* adressString = line.c_str();
    
    unsigned int vertIndexStart = 2;
    
    while(vertIndexStart < adressLength)
    {
        if(adressString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextCharacter(vertIndexStart, adressString, adressLength, ' ');
    
    float x = ParseModelFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, adressString, adressLength, ' ');
    
    float y = ParseModelFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, adressString, adressLength, ' ');
    
    float z = ParseModelFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec3(x,y,z);

    //glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

glm::vec2 ModelObject::ParseModelVector2(const std::string& line)
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 3;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseModelFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseModelFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec2(x,y);
}

static bool CompareodelLocationPtr(const ModelStorage* a, const ModelStorage* b)
{
    return a->vertexStorage < b->vertexStorage;
}

static inline unsigned int FindNextCharacter(unsigned int start, const char* str, unsigned int length, char token)
{
    unsigned int result = start;
    while(result < length)
    {
        result++;
        if(str[result] == token)
            break;
    }
    
    return result;
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
        
    const char* cstr = s.c_str();
    unsigned int strLength = s.length();
    unsigned int start = 0;
    unsigned int end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }
            
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
        
    return elems;
}
static inline unsigned int ParseModelIStorageValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseModelFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atof(token.substr(start, end - start).c_str());
}



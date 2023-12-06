#include "nspch.h"
#include <core/Logger.h>

#include "AssimpLoader.h"

namespace TrashEngine {

    Ref<StaticModelData> AssimpLoader::LoadStaticModelFromFile(const std::string& filePath, const std::string& texturePath)
    {
        Assimp::Importer importer;

        const auto aScene = importer.ReadFile(
            filePath, 
            aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_CalcTangentSpace
        );

        if (!aScene) {
            NS_CORE_ERROR("Failed to load model using assimp: {0}", filePath);
            return nullptr;
        }
        if (aScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
            NS_CORE_WARN("This model is incomplete: {0}", filePath);
        }

        Ref<StaticModelData> modelData = CreateRef<StaticModelData>();

        ProcessMaterial(aScene, modelData->materials, texturePath);

        if (!ProcessStaticMeshes(aScene, modelData)) {
            NS_CORE_ERROR("Failed to process meshes: {0}", filePath);
            return nullptr;
        }

        return modelData;
    }

    Ref<AnimatedModelData> AssimpLoader::LoadAnimatedModelFromFile(const std::string& filePath, const std::string& texturePath)
    {

        Assimp::Importer importer;

        const auto aScene = importer.ReadFile(
            filePath,
            aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_CalcTangentSpace
        );

        if (!aScene) {
            NS_CORE_ERROR("Failed to load model using assimp: {0}", filePath);
            return nullptr;
        }
        if (aScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
            NS_CORE_WARN("This model is incomplete: {0}", filePath);
        }

        Ref<AnimatedModelData> modelData = CreateRef<AnimatedModelData>();

        ProcessMaterial(aScene, modelData->materials, texturePath);

        if (!ProcessAnimatedMeshes(aScene, modelData)) {
            NS_CORE_ERROR("Failed to process meshes: {0}", filePath);
            return nullptr;
        }

        return modelData;
    }

    Ref<AnimatedModelAnimation> AssimpLoader::LoadAnimatedModelAnimationFromFile(const std::string& filePath)
    {
        Assimp::Importer importer;

        const auto aScene = importer.ReadFile(filePath, 0);

        if (!aScene) {
            NS_CORE_ERROR("Failed to load model using assimp: {0}", filePath);
            return nullptr;
        }
        if (aScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
            NS_CORE_WARN("This model is incomplete: {0}", filePath);
        }

        if (!aScene->HasAnimations()) {
            return nullptr;
        }

        // just read the first animation of this file
        auto aAnimation = aScene->mAnimations[0];

        Ref<AnimatedModelAnimation> animationData = CreateRef<AnimatedModelAnimation>();
        float tickPerSecond = aAnimation->mTicksPerSecond == 0 ? 1.f : (float)aAnimation->mTicksPerSecond;
        animationData->setDuration((float)aAnimation->mDuration / tickPerSecond);
#ifdef NS_DEBUG
        NS_CORE_TRACE("Loading animation: {0}", aAnimation->mName.C_Str());
        NS_CORE_TRACE("animation time: {0}s", animationData->getDuration());
        NS_CORE_TRACE("channels: {0}", aAnimation->mNumChannels);
#endif // NS_DEBUG
        for (uint32_t i = 0; i < aAnimation->mNumChannels; i++) {
            auto aNodeAnim = aAnimation->mChannels[i];

            std::string boneName = aNodeAnim->mNodeName.C_Str();

            for (uint32_t j = 0; j < aNodeAnim->mNumPositionKeys; j++) {
                auto& aPositionKey = aNodeAnim->mPositionKeys[j];
                auto& aRotationKey = aNodeAnim->mRotationKeys[j];
                auto& aScaleKey = aNodeAnim->mScalingKeys[j];
                Ref<AnimatedModelAnimation::KeyFrame> frame;
                if (animationData->getKeyFrames().size() < j + 1) {
                    float timeStamp = (float)aPositionKey.mTime / tickPerSecond;
                    frame = animationData->getKeyFrames().emplace_back(CreateRef<AnimatedModelAnimation::KeyFrame>());
                    frame->timeStamp = timeStamp;
                }
                else {
                    frame = animationData->getKeyFrames()[j];
                }
                Transform transform;

                transform.setPosition(glm::vec3(aPositionKey.mValue.x, aPositionKey.mValue.y, aPositionKey.mValue.z));
                transform.setRotation(glm::quat(aRotationKey.mValue.w, aRotationKey.mValue.x, aRotationKey.mValue.y, aRotationKey.mValue.z));
                transform.setScale(glm::vec3(aScaleKey.mValue.x, aScaleKey.mValue.y, aScaleKey.mValue.z));

                frame->pose.insert(std::pair<std::string, Transform>(boneName, transform));
            }
        }

#ifdef NS_DEBUG
        NS_CORE_INFO("Total frame: {0}", animationData->getKeyFrames().size());
#endif // NS_DEBUG

        return animationData;
    }

    bool AssimpLoader::ProcessStaticMeshes(const aiScene* aScene, Ref<StaticModelData> modelData)
    {
        if (!aScene->HasMeshes())
            return false;

        // calc total vertices
        Uint32 totalVertices = 0;

        for (Uint32 i = 0; i < aScene->mNumMeshes; i++) {
            totalVertices += aScene->mMeshes[i]->mNumVertices;
        }

        modelData->vertices.resize(totalVertices);

        // the vertex offset of current mesh
        uint32_t meshVertexIndexOffset = 0;
        uint32_t meshIndicesOffset = 0;
        uint32_t meshIndicesCount = 0;
        uint32_t currentVertexPtr = 0;


        for (uint32_t i = 0; i < aScene->mNumMeshes; i++) {
            auto aMesh = aScene->mMeshes[i];

            meshIndicesCount = 0;

            // process indices
            for (uint32_t j = 0; j < aMesh->mNumFaces; j++) {
                auto& aFace = aMesh->mFaces[j];
                for (uint32_t k = 0; k < aFace.mNumIndices; k++) {
                    modelData->indices.emplace_back(meshVertexIndexOffset + aFace.mIndices[k]);
                    meshIndicesCount++;
                }
            }

            // process vertices
            for (uint32_t j = 0; j < aMesh->mNumVertices; j++) {
                auto mVertex = aMesh->mVertices[j];

                // position
                modelData->vertices[currentVertexPtr].position.x = mVertex.x;
                modelData->vertices[currentVertexPtr].position.y = mVertex.y;
                modelData->vertices[currentVertexPtr].position.z = mVertex.z;

                // just use texture coord 0
                float u = 0, v = 0;
                if (aMesh->HasTextureCoords(0)) {
                    u = aMesh->mTextureCoords[0][j].x;
                    v = aMesh->mTextureCoords[0][j].y;
                }
                modelData->vertices[currentVertexPtr].texCoord.x = u;
                modelData->vertices[currentVertexPtr].texCoord.y = v;
                
                if (aMesh->HasNormals()) {
                    auto aNormal = aMesh->mNormals[j];
                    modelData->vertices[currentVertexPtr].normal.x = aNormal.x;
                    modelData->vertices[currentVertexPtr].normal.y = aNormal.y;
                    modelData->vertices[currentVertexPtr].normal.z = aNormal.z;
                }

                if (aMesh->HasTangentsAndBitangents()) {
                    auto aTangent = aMesh->mTangents[j];
                    modelData->vertices[currentVertexPtr].tangent.x = aTangent.x;
                    modelData->vertices[currentVertexPtr].tangent.y = aTangent.y;
                    modelData->vertices[currentVertexPtr].tangent.z = aTangent.z;
                }

                currentVertexPtr++;
            }

            auto& meshData = modelData->meshes.emplace_back();

            meshData.indexCount = meshIndicesCount;
            meshData.indexOffset = meshIndicesOffset;
            meshData.materialIndex = aMesh->mMaterialIndex;

            meshVertexIndexOffset = currentVertexPtr;
            meshIndicesOffset += meshIndicesCount;
        }

        return true;
    }

    glm::mat4 toMatrix(const aiMatrix4x4& from)
    {
        glm::mat4 to;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;
    }

    bool AssimpLoader::ProcessAnimatedMeshes(const aiScene* aScene, Ref<AnimatedModelData> modelData)
    {
        if (!aScene->HasMeshes())
            return false;

        // calc total vertices
        Uint32 totalVertices = 0;

        for (Uint32 i = 0; i < aScene->mNumMeshes; i++) {
            totalVertices += aScene->mMeshes[i]->mNumVertices;
        }

        modelData->vertices.resize(totalVertices);


        // process vertices
        // the vertex offset of current mesh
        uint32_t meshVertexIndexOffset = 0;
        uint32_t meshIndicesOffset = 0;
        uint32_t meshIndicesCount = 0;
        uint32_t currentVertexPtr = 0;
        uint32_t currentBoneIndex = 0;


        for (uint32_t i = 0; i < aScene->mNumMeshes; i++) {
            auto aMesh = aScene->mMeshes[i];

            // vertexID for global vertex index; 
            struct BoneWeight
            {
                uint32_t boneId;
                float weight;
            };

            // for each mesh it have the bones weight for vertex
            std::map<uint32_t, std::vector<BoneWeight>> vwList;
            // process bones
            for (uint32_t j = 0; j < aMesh->mNumBones; j++) {
                auto aBone = aMesh->mBones[j];

                auto& boneData = modelData->bones.emplace_back();

                boneData.name = std::string(aBone->mName.C_Str());

                boneData.offsetMatrix = toMatrix(aBone->mOffsetMatrix);

                for (uint32_t k = 0; k < aBone->mNumWeights; k++) {
                    auto& vertexWeight = aBone->mWeights[k];

                    uint32_t vertexIndex = meshVertexIndexOffset + vertexWeight.mVertexId;
                    vwList[vertexIndex].emplace_back(BoneWeight{currentBoneIndex, vertexWeight.mWeight });

                }
                currentBoneIndex++;
            }

            // process indices
            meshIndicesCount = 0;

            for (uint32_t j = 0; j < aMesh->mNumFaces; j++) {
                auto& aFace = aMesh->mFaces[j];
                for (uint32_t k = 0; k < aFace.mNumIndices; k++) {
                    modelData->indices.emplace_back(meshVertexIndexOffset + aFace.mIndices[k]);
                    meshIndicesCount++;
                }
            }

            // process vertices
            for (uint32_t j = 0; j < aMesh->mNumVertices; j++) {
                auto mVertex = aMesh->mVertices[j];

                // position
                modelData->vertices[currentVertexPtr].position.x = mVertex.x;
                modelData->vertices[currentVertexPtr].position.y = mVertex.y;
                modelData->vertices[currentVertexPtr].position.z = mVertex.z;

                // just use texture coord 0
                float u = 0, v = 0;
                if (aMesh->HasTextureCoords(0)) {
                    u = aMesh->mTextureCoords[0][j].x;
                    v = aMesh->mTextureCoords[0][j].y;
                }
                modelData->vertices[currentVertexPtr].texCoord.x = u;
                modelData->vertices[currentVertexPtr].texCoord.y = v;

                if (aMesh->HasNormals()) {
                    auto aNormal = aMesh->mNormals[j];
                    modelData->vertices[currentVertexPtr].normal.x = aNormal.x;
                    modelData->vertices[currentVertexPtr].normal.y = aNormal.y;
                    modelData->vertices[currentVertexPtr].normal.z = aNormal.z;
                }

                if (aMesh->HasTangentsAndBitangents()) {
                    auto aTangent = aMesh->mTangents[j];
                    modelData->vertices[currentVertexPtr].tangent.x = aTangent.x;
                    modelData->vertices[currentVertexPtr].tangent.y = aTangent.y;
                    modelData->vertices[currentVertexPtr].tangent.z = aTangent.z;
                }

                auto it = vwList.find(currentVertexPtr);
                if (it != vwList.end()) {
                    const auto& list = it->second;

                    float totalWeights = 0;
                    for (uint32_t k = 0; k < 4; k++) {
                        if (k < list.size()) {
                            totalWeights += list[k].weight;
                        }
                    }

                    for (uint32_t k = 0; k < 4; k++) {
                        if (k < list.size()) {
                            modelData->vertices[currentVertexPtr].boneID[k] = list[k].boneId;
                            modelData->vertices[currentVertexPtr].weight[k] = list[k].weight / totalWeights;
                            //modelData->vertices[currentVertexPtr].weight[k] = list[k].weight;
                        }
                        else {
                            modelData->vertices[currentVertexPtr].boneID[k] = 0;
                            modelData->vertices[currentVertexPtr].weight[k] = 0;
                        }
                    }
                }
                else {
                    modelData->vertices[currentVertexPtr].boneID = glm::ivec4(0);
                    modelData->vertices[currentVertexPtr].weight = glm::vec4(0);
                }

                currentVertexPtr++;
            }

            auto& meshData = modelData->meshes.emplace_back();

            meshData.indexCount = meshIndicesCount;
            meshData.indexOffset = meshIndicesOffset;
            meshData.materialIndex = aMesh->mMaterialIndex;

            meshVertexIndexOffset = currentVertexPtr;
            meshIndicesOffset += meshIndicesCount;
        }

        // process bone relation
        modelData->rootJoint = CreateRef<AnimatedModelJointData>();
        ProcessAnimatedJoint(aScene->mRootNode, modelData, modelData->rootJoint);

        return true;
    }

    void AssimpLoader::ProcessAnimatedJoint(const aiNode* node, Ref<AnimatedModelData> modelData, Ref<AnimatedModelJointData> jointData)
    {
        jointData->boneID = UINT32_MAX;
        jointData->transformation = toMatrix(node->mTransformation);
        jointData->name = node->mName.C_Str();
        for (uint32_t i = 0; i < modelData->bones.size(); i++) {
            if (modelData->bones[i].name == node->mName.C_Str()) {
                jointData->boneID = i;
                break;
            }
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            aiNode* aChildNode = node->mChildren[i];

            auto childJoint = jointData->children.emplace_back(CreateRef<AnimatedModelJointData>());
            ProcessAnimatedJoint(aChildNode, modelData, childJoint);
        }
    }

    void AssimpLoader::ProcessMaterial(const aiScene* aScene, std::vector<MaterialData>& materialList, const std::string& texturePath)
    {
        if (!aScene->HasMaterials())
            return;

        for (uint32_t i = 0; i < aScene->mNumMaterials; i++) {
            const auto aMaterial = aScene->mMaterials[i];

            auto& materialData = materialList.emplace_back();

            materialData.diffuseTexture = ProcessTexture(aMaterial, aiTextureType_DIFFUSE, texturePath);
            materialData.normalTexture = ProcessTexture(aMaterial, aiTextureType_NORMALS, texturePath);
            
            aiColor4D color;
            aiGetMaterialColor(aMaterial, AI_MATKEY_COLOR_DIFFUSE, &color);
            materialData.diffuseColor = glm::vec3(color.r, color.g, color.b);
        }
    }

    std::string AssimpLoader::ProcessTexture(const aiMaterial* aMaterial, aiTextureType textureType, const std::string& texturePath)
    {
        aiString aTexturePath;

        // just get the 0 texture
        aiGetMaterialTexture(aMaterial, textureType, 0, &aTexturePath);

        if (aTexturePath.length > 0) {
            std::string textureFilePathStr = texturePath + "/" + aTexturePath.C_Str();
            return textureFilePathStr;
        }

        return std::string();
    }

}

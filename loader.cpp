#include "loader.h"

#include <iostream>
#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <QTimer>
#include <vector>

#define aisgl_min(x, y) (x < y ? x : y)
#define aisgl_max(x, y) (y > x ? y : x)


Mesh_::Mesh_()
{ }

Mesh_::Mesh_(QVector<VertexData> &vertData, QVector<GLuint> &indexes)
{
    vertexes_ = vertData;
    indexes_ = indexes;
}

Mesh_::Mesh_(const Mesh_ &m_)
{
    vertexes_ = m_.vertexes_;
    indexes_ = m_.indexes_;
}

Model_::Model_()
{

}

void Model_::loadModel_()
{
    qDebug() << "path_: " << path_;
    scene = import.ReadFile(path_, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene) {
        get_bounding_box(&scene_min,&scene_max);
        scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
        scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
        scene_center.z = (scene_min.z + scene_max.z) / 2.0f;

        qDebug() << "scene_min.z: " << scene_min.z;
        qDebug() << "scene_max.z: " << scene_max.z;
        qDebug() << "scene_center.x: " << scene_center.x;
        qDebug() << "scene_center.y: " << scene_center.y;
        qDebug() << "scene_center.z: " << scene_center.z;
    }

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);

    complete = true;
    emit loaded(complete);
}

void Model_::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(size_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor4D specularColor;
        aiColor4D diffuseColor;
        aiColor4D ambientColor;
        float shininess;

        aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specularColor);
        aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
        aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
        aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);

        QVector3D specColor(specularColor.r, specularColor.g, specularColor.b);
        QVector3D diffColor(diffuseColor.r, diffuseColor.g, diffuseColor.b);
        QVector3D ambColor(ambientColor.r, ambientColor.g, ambientColor.b);

        //processMesh(mesh, scene);
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(size_t i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh_ Model_::processMesh(aiMesh *mesh, const aiScene *scene)
{
    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;

    for(size_t i = 0; i < mesh->mNumVertices; i++)
    {
        if(mesh->mVertices != nullptr && mesh->mTextureCoords[0] != nullptr && mesh->mNormals != nullptr)
        {
            vertexes.append(VertexData(QVector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
                                       QVector2D(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y),
                                       QVector3D(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z)));
        }
        else if(mesh->mVertices != nullptr && mesh->mNormals != nullptr)
        {
            vertexes.append(VertexData(QVector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
                                       QVector2D(0.0f, 0.0f),
                                       QVector3D(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z)));
        }
        else if(mesh->mVertices != nullptr && mesh->mTextureCoords[0] != nullptr)
        {
            vertexes.append(VertexData(QVector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
                                       QVector2D(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y),
                                       QVector3D(0.0f, 0.0f, 0.0f)));
        }
        else if(mesh->mVertices != nullptr)
        {
            vertexes.append(VertexData(QVector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
                                       QVector2D(0.0f, 0.0f),
                                       QVector3D(0.0f, 0.0f, 0.0f)));
        }
        if(mesh->mTextureCoords[0]) {
            //qDebug() << "texture coord";
        }
        if(mesh->mNormals) {
            //qDebug() << "normal coord";
        }
        if(mesh->mTangents) {
            //qDebug() << "tangent coord";
        }
    }
    // process indices
    for(size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; ++j)
        {
            indexes.append(static_cast<GLuint>(face.mIndices[j]));
        }
    }

    return Mesh_(vertexes, indexes);
}

void Model_::get_bounding_box(aiVector3D *min, aiVector3D *max)
{
    aiMatrix4x4 trafo;
    aiIdentityMatrix4(&trafo);

    min->x = min->y = min->z =  1e10f;
    max->x = max->y = max->z = -1e10f;

    get_bounding_box_for_node(scene->mRootNode, min, max, &trafo);
}

void Model_::get_bounding_box_for_node(const aiNode *nd, aiVector3D *min, aiVector3D *max, aiMatrix4x4 *trafo)
{
    aiMatrix4x4 prev;
    unsigned int n = 0, t;

    prev = *trafo;
    aiMultiplyMatrix4(trafo,&nd->mTransformation);

    for (; n < nd->mNumMeshes; ++n) {
        const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
        for (t = 0; t < mesh->mNumVertices; ++t) {

            aiVector3D tmp = mesh->mVertices[t];
            aiTransformVecByMatrix4(&tmp, trafo);

            min->x = aisgl_min(min->x,tmp.x);
            min->y = aisgl_min(min->y,tmp.y);
            min->z = aisgl_min(min->z,tmp.z);

            max->x = aisgl_max(max->x,tmp.x);
            max->y = aisgl_max(max->y,tmp.y);
            max->z = aisgl_max(max->z,tmp.z);
        }
    }

    for (n = 0; n < nd->mNumChildren; ++n) {
        get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
    }
    *trafo = prev;
}

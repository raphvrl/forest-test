#include "mesh.hpp"

namespace gfx
{

void Mesh::init(
    Device &device,
    const std::vector<Vertex> &vertices,
    const std::vector<u32> &indices)
{
    if (vertices.empty()) {
        throw std::runtime_error("Mesh has no vertices!");
    }

    if (indices.empty()) {
        throw std::runtime_error("Mesh has no indices!");
    }

    m_device = &device;
    m_vertexCount = static_cast<u32>(vertices.size());
    m_indexCount = static_cast<u32>(indices.size());

    VkDeviceSize vertexBufferSize = sizeof(Vertex) * vertices.size();
    VkDeviceSize indexBufferSize = sizeof(u32) * indices.size();

    Buffer stagingVertexBuffer = device.createBuffer(
        vertexBufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_CPU_ONLY
    );

    m_vertexBuffer = device.createBuffer(
        vertexBufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY
    );

    stagingVertexBuffer.uploadData(vertices);
    device.copyBuffer(stagingVertexBuffer, m_vertexBuffer, vertexBufferSize);
    stagingVertexBuffer.destroy();

    Buffer stagingIndexBuffer = device.createBuffer(
        indexBufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_CPU_ONLY
    );

    m_indexBuffer = device.createBuffer(
        indexBufferSize,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY
    );

    stagingIndexBuffer.uploadData(indices);
    device.copyBuffer(stagingIndexBuffer, m_indexBuffer, indexBufferSize);
    stagingIndexBuffer.destroy();
}

void Mesh::destroy()
{
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
}

void Mesh::bind(VkCommandBuffer cmd) const
{
    VkBuffer vertexBuffers[] = { m_vertexBuffer.getBuffer() };
    VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(
        cmd,
        m_indexBuffer.getBuffer(),
        0,
        VK_INDEX_TYPE_UINT32
    );
}

void Mesh::draw(VkCommandBuffer cmd) const
{
    vkCmdDrawIndexed(cmd, m_indexCount, 1, 0, 0, 0);
}

} // namespace gfx
#pragma once
#include <string>
#include <glm/glm.hpp>

struct TextureRef
{
    std::string path;
};

struct IntRect
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

class SpriteRenderer
{
public:
    const TextureRef& GetTexture() const { return m_Texture; }
    void SetTexture(const TextureRef& texture) { m_Texture = texture; }
    void SetTexturePath(const std::string& path) { m_Texture.path = path; }

    const IntRect& GetSourceRect() const { return m_SourceRect; }
    bool UsesSourceRect() const { return m_UseSourceRect; }

    void ClearSourceRect()
    {
        m_SourceRect = {0, 0, 0, 0};
        m_UseSourceRect = false;
    }

    void SetSourceRect(int x, int y, int width, int height)
    {
        m_SourceRect = {x, y, width, height};
        m_Size = {static_cast<float>(width), static_cast<float>(height)};
        m_UseSourceRect = true;
        
    }

    void SetSourceRectFromGrid(int column, int row, int cellWidth, int cellHeight)
    {
        SetSourceRect(
            column * cellWidth,
            row * cellHeight,
            cellWidth,
            cellHeight
        );
    }

    const glm::vec4& GetTint() const { return m_Tint; }
    void SetTint(const glm::vec4& tint) { m_Tint = tint; }

    bool IsVisible() const { return m_Visible; }
    void SetVisible(bool visible) { m_Visible = visible; }

    int GetLayer() const { return m_Layer; }
    void SetLayer(int layer) { m_Layer = layer; }

    bool IsFlippedX() const { return m_FlipX; }
    bool IsFlippedY() const { return m_FlipY; }
    void SetFlipX(bool flip) { m_FlipX = flip; }
    void SetFlipY(bool flip) { m_FlipY = flip; }
    
    const glm::vec2& GetSize() const { return m_Size; }
    void SetSize(const glm::vec2& size) { m_Size = size; }
    void SetSize(float width, float height) { m_Size = {width, height}; }

private:
    TextureRef m_Texture;
    IntRect m_SourceRect{0, 0, 0, 0};
    bool m_UseSourceRect = false;
    glm::vec4 m_Tint{1.0f};
    bool m_Visible = true;
    int m_Layer = 0;
    bool m_FlipX = false;
    bool m_FlipY = false;
    glm::vec2 m_Size{1.0f, 1.0f};
};

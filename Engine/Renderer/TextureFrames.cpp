#include "pch.h"
#include "TextureFrames.h"
#include "Serialization/Json.h"
#include "Renderer/Texture.h"

namespace nu {

	bool TextureFrames::Load(const std::string& filename, Renderer& renderer) {
		
		json::document_t document;

		if (!json::Load(filename, document)) {
			std::cerr << "Could not load Texture Frames: " << filename << std::endl;
			return false;
		}

		std::string texture_name;
		JSON_READ_REQ(document, texture_name);
		if (texture_name.empty()) return false;

		m_texture = Resources().Get<Texture>(texture_name, renderer);
		if (!m_texture) {
			std::cerr << "Could not load Texture Frame texture: " << texture_name << std::endl;
			return false;
		}

		JSON_READ_NAME(document, "columns", m_cols);
		JSON_READ_NAME(document, "rows", m_rows);
		JSON_READ_NAME(document, "start_frame", m_startFrame);
		JSON_READ_NAME(document, "total_frames", m_totalFrames);

		if (m_cols == 0 || m_rows == 0) {
			std::cerr << "Texture Frame rows or columns is 0\n";
			return false;
		}

		//Vector2 textureSize = m_texture->GetSize();
		//m_cellSize = textureSize / Vector2{ m_cols, m_rows };

		m_cellSize = m_texture->GetSize() / Vector2{ m_cols, m_rows };

		return true;
	}
		
	Rect TextureFrames::GetFrameRect(unsigned int frame) {
		
		if (frame >= (unsigned int)m_totalFrames)
		{
			std::cerr << "Texture Frames frame " << frame << " is out of bounds";
			frame = 0;
		}

		int currentFrame = m_startFrame + frame;

		int col = currentFrame % m_cols;
		int row = currentFrame / m_cols;
		float x = col * m_cellSize.x;
		float y = row * m_cellSize.y;

		return Rect
		{
			x,
			y,
			m_cellSize.x,
			m_cellSize.y
		};
	}
}